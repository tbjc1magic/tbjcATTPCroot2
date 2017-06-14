
# coding: utf-8

# In[1]:

from sqlalchemy import *
import numpy as np
import pylab as plt
import pandas as pd
from peakdetect import peakdetect
from scipy.ndimage.interpolation import shift
from matplotlib.colors import LogNorm
from scipy.interpolate import interp1d
from scipy.optimize import newton
import peakutils
import cv2
from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas

import time
from multiprocessing import Pool
import multiprocessing
import subprocess
from collections import defaultdict
import multiSQL
import math
from sklearn import linear_model

engine = create_engine('sqlite+pysqlite:///../data/SQL/ProtoMap.db')
ProtoMapdf = pd.io.sql.read_sql("SELECT * FROM ProtoMap", engine)

def Process(path):

    ########## GET ADC from DB ############
    def ProcessDB(path):
        print "starting...  "+path
        engine = create_engine('sqlite+pysqlite:///'+path)
        ADCdf = pd.io.sql.read_sql("SELECT * FROM ADC", engine)
        return ADCdf

    ADC_df = ProcessDB(path)

    ########## GET Mesh and Convol ############
    def ProcessMeshConvol(ADCdf):
        def rmbackground(Event):
            mesh = Event.iloc[:,3:][Event.iloc[:,3:]>20].sum(axis=0)
            return mesh
        def convolve(event):
            mesh_convol = np.convolve(event.values,([1]*10)[::-1],'same')
            return pd.Series(mesh_convol,index=['t'+str(i) for i in range(512)])

        mesh_df = ADCdf.groupby('EventID').apply(rmbackground)
        convol_df=mesh_df.apply(convolve,axis=1)
        convol_df.reset_index(inplace=True)
        return mesh_df,convol_df

    mesh_df, convol_df = ProcessMeshConvol(ADC_df)

    ########## GET Hit ############
    def ProcessHit(ADCdf):

        def ConvertHit(df):

            tmp = df.iloc[:,3:][df.iloc[:,3:]>20].fillna(0)
            ss = tmp.sum(axis=1)
            pos = np.sum(tmp.values*np.arange(512),axis=1)/(ss+1e-6)
            tmp = pos[pos>0.1]
            tmp = tmp.reset_index()
            tmp = pd.merge(tmp,df,left_on='index',right_index=True,how='left')[['PadNum',0]]
            return tmp

        hitdf = ADCdf.groupby(['EventID']).apply(ConvertHit)
        hitdf = hitdf.reset_index()
        hitdf.columns = ['EventID','d','PadNum','PadZ']
        hitdf = hitdf[['EventID','PadNum','PadZ']];

        return hitdf

    hit_df = ProcessHit(ADC_df)

    ########## GET Lines from Houghtranform ############
    def ProcessLine(hitdf):

        tmp1 = pd.merge(hitdf,ProtoMapdf,on='PadNum')

        def hough(EID):

            tmpdf = tmp1[(tmp1['EventID']==EID)]
            tmpdf = tmpdf[np.sqrt(tmpdf['PadX']*tmpdf['PadX']+tmpdf['PadY']*tmpdf['PadY'])>10]

            image = np.zeros([150,550])

            image[(np.sqrt(tmpdf['PadX']*tmpdf['PadX']+tmpdf['PadY']*tmpdf['PadY'])).values.astype(np.int),
                  tmpdf['PadZ'].values.astype(np.int)] =255

            lines = cv2.HoughLines(image.astype(np.uint8),1,np.pi/90,10)

            if lines is None:
                return

            for rho,theta in lines.reshape(-1,2):
                a = np.cos(theta)
                b = np.sin(theta)
                x0 = a*rho
                y0 = b*rho
                x1 = int(x0 + 1000*(-b))
                y1 = int(y0 + 1000*(a))
                x2 = int(x0 - 1000*(-b))
                y2 = int(y0 - 1000*(a))

            ans = []
            tmp = []

            for rho,theta in sorted(lines.reshape(-1,2),key=lambda x:x[1]):
                if not tmp:
                    tmp.append((rho,theta))
                    continue

                if theta-tmp[-1][-1]<0.1:
                    tmp.append((rho,theta))
                else:
                    ans.append(tmp)
                    tmp = [(rho,theta)]

            if tmp:
                ans.append(tmp)

            def IsClose(p,line):
                rho,theta = line
                x0, y0 = rho*np.cos(theta), rho*np.sin(theta)
                theta1 = (theta+np.pi/2) - int((theta+np.pi/2)/np.pi)*np.pi
                dist = np.abs(-x*np.sin(theta1) + y*np.cos(theta1)-y0*np.cos(theta1)+x0*np.sin(theta1))
                return dist<3.5

            y = np.sqrt(tmpdf['PadX']*tmpdf['PadX']+tmpdf['PadY']*tmpdf['PadY']).values
            x = tmpdf['PadZ'].values

            k,b= None, None
            r = []

            for l in ans:
                l = np.array(l)
                mask = IsClose(np.vstack([x,y]).T,l.T.mean(axis=1))

                for _ in range(10):
                    x1,y1 = np.vstack([x,y]).T[mask].T

                    if len(x1)<1:
                        break

                    #k,b = np.polyfit(x1,y1,1)

                    k = np.sum((x1-np.mean(x1))*(y1-np.mean(y1)))/np.var(x1)/x1.shape[0]
                    #k = np.cov(x1,y1)[0,1]/np.var(x1)/x1.shape[0]*(x1.shape[0]-1)
                    b = np.mean(y1) - k*np.mean(x1)

                    if np.isnan(k):
                        return

                    rho = b/np.sqrt(k*k+1)
                    theta = np.arctan(k)+np.pi/2

                    mask = IsClose(np.vstack([x,y]).T,(rho,theta))

                r.append((k,b))

            return r

        result = defaultdict(list)

        for idx in hitdf['EventID'].unique():
            lines = hough(idx)
            result['EventID'].append(idx)
            for i in range(3):
                try:
                    k,b = lines[i]
                    result[i].append(-b/k)
                except:
                    result[i].append(None)
            print idx#,rtmp

        linedf = pd.DataFrame(result)

        return linedf

    line_df = ProcessLine(hit_df)

    ########## GET T0 ############
    def ProcessT0(convol_df):
        def t0Process(r):
            grad = np.gradient(r.values[1:])
            xs = 10
            ys = -50
            grad_con = grad#np.convolve(grad,([1]*20)[::-1],'same')
            ratio = float(max(grad[xs:]))/max(grad_con)
            grad_con=grad_con*ratio
            idx_gradcon = peakutils.indexes(abs(grad_con[xs:ys]), thres=0.2, min_dist=5)

            idx_gradcon = [x for x,y in zip(idx_gradcon+xs,grad_con[idx_gradcon+xs]) if y < 0]
            if not idx_gradcon:
                return
            return idx_gradcon[-1]#sorted(zip(idx_gradcon,grad_con[idx_gradcon]),key=lambda x:x[1])[-1][0]

        mask = (convol_df.iloc[:,50:-50].sum(axis=1)>300000)&(convol_df.iloc[:,50:-50].max(axis=1)<100000)
        t0_df = convol_df[mask].apply(t0Process, axis=1)
        t0_df = t0_df.to_frame().reset_index()
        t0_df.columns = ['EventID','time0']
        return t0_df

    t0_df =ProcessT0(convol_df)

    ########## GET Histogram ############
    def ProcessHist(linedf,t0df):
        tmp = pd.merge(linedf,t0df,how='inner',on='EventID')
        tmp1 = tmp[(~tmp[0].isnull())&(~tmp['time0'].isnull())]
        return (tmp1['time0']-tmp1[0])*80/1000.0*2.459

    hist_df = ProcessHist(line_df,t0_df)

    return hist_df

def main():
    parant = '../data/SQL/'
    fs = [parant+'0075_{:04d}.db'.format(i) for i in range(16)]
    #Process('../data/SQL/0075_0000.db')
    #return
    pool = Pool(8)
    start_time = time.time()
    hist_dfs = pool.map(Process,fs)

    pd.concat(hist_dfs).to_csv("Position.csv")
    #plt.hist(pd.concat(hist_dfs),bins=200)
    #plt.savefig("ReactionPosition.jpg")
    #plt.show()
    print time.time() - start_time

main()

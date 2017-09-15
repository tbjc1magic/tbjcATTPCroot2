from sqlalchemy import *
import numpy as np
import pylab as plt
import pandas as pd
from scipy.ndimage.interpolation import shift
from scipy.interpolate import interp1d
from scipy.optimize import newton
import peakutils
import cv2
import time
import re
import math
from sklearn import mixture
from skimage.transform import (hough_line, hough_line_peaks,
                               probabilistic_hough_line)
import heapq

class DataProcessor(object):

    def __init__(self,data_path,map_path):
        
        print "initialization will take some time"
        start_time = time.time()
        engine = create_engine('sqlite+pysqlite:///'+data_path)
        ADCdf = pd.io.sql.read_sql("SELECT * FROM ADC limit 3000", engine)
        end_time = time.time()
        print end_time-start_time

        engine = create_engine('sqlite+pysqlite:///'+map_path)
        ProtoMapdf = pd.io.sql.read_sql("SELECT * FROM ProtoMap", engine)
        print ProtoMapdf.head()
        print "loading finished"
        
        print "begin processing"
        start_time = time.time()
        ADCdfn = ADCdf.copy()

        ADCdfn.columns = [np.uint16(_[1:]) if re.match('t\d+',_) is not None else _ for _ in ADCdfn.columns ]
        ADCdfn['max'] = ADCdfn.iloc[:,3+50:-50].max(axis=1)
        mask= (ADCdfn.iloc[:,3:-1]>20) & (ADCdfn.iloc[:,3:-1].gt(ADCdfn['max']*0.2,axis=0))
        ADCdfn.iloc[:,3:-1] = ADCdfn.iloc[:,3:-1][mask].fillna(0)
        end_time = time.time()
        print end_time-start_time
        
        start_time = time.time()
        n1 = [_ for _ in ADCdfn.columns if type(_) is np.uint16]
        n2 = [_ for _ in ADCdfn.columns if type(_) is not np.uint16]
        t2 = pd.melt(ADCdfn.iloc[:],id_vars=n2,value_vars=n1).drop(['ID'],axis=1)
        t2.columns = ['EventID','PadNum','max','time','charge']
        self.t3 = pd.merge(t2[t2['charge']>20],ProtoMapdf,on='PadNum')[['EventID','PadNum','time','PadX','PadY','charge']]
        print time.time() - start_time
        
        print "processing finished"
        
        self.ADCdfn = ADCdfn
        self.ADCdf = ADCdf
        self.ProtoMapdf = ProtoMapdf
        
    def Process(self,EID):
        t3 = self.t3
        
        tmp = t3[(t3['EventID']==EID)&(t3['charge']>3)]
        p = (tmp['PadNum']-1)%63+1

        tmp['PadPos'] = ((p-9)*(p>9)+p)*(tmp['PadNum']!=0)

        Q1 = tmp[(tmp['PadX']>0)&(tmp['PadY']>0)]
        Q2 = tmp[(tmp['PadX']<0)&(tmp['PadY']>0)]
        Q3 = tmp[(tmp['PadX']<0)&(tmp['PadY']<0)]
        Q4 = tmp[(tmp['PadX']>0)&(tmp['PadY']<0)]


        image1 = np.zeros([300,600])

        image1[(-Q1['PadPos'].values+150).astype(np.int), Q1['time'].values.astype(np.int)] =255
        image1[(Q3['PadPos'].values+150).astype(np.int), Q3['time'].values.astype(np.int)] =255

        image2 = np.zeros([300,600])

        image2[(-Q2['PadPos'].values+150).astype(np.int), Q2['time'].values.astype(np.int)] =255
        image2[(Q4['PadPos'].values+150).astype(np.int), Q4['time'].values.astype(np.int)] =255

        if np.sum(image1-image2)>0:
            image = image1 
        else:
            image = image2


        ########################

        gray = cv2.GaussianBlur(image, (3, 3), 0)
        ret,im = cv2.threshold(gray*255, 10, 255, cv2.THRESH_BINARY)

        thresh = im.astype(np.uint8)
        for _ in range(3):
            thresh = cv2.erode(thresh, None, iterations=1)
            thresh = cv2.dilate(thresh, None, iterations=1)

        return thresh

def VertexPos(pic):

    h, theta, d = hough_line(pic)
    ans = []
    for score, angle, dist in zip(*hough_line_peaks(h, theta, d,num_peaks=4,threshold=0.3)):
        y0 = (dist - 0 * np.cos(angle)) / np.sin(angle)
        y1 = (dist - pic.shape[1] * np.cos(angle)) / np.sin(angle)

        if abs(abs(angle)-math.pi/2)>0.1:
            heapq.heappush(ans,(-score,y0,y1))

    return heapq.heappop(ans)

def TipFinder(thresh):

    m1, contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)


    def Distance(n1,n2):

        c1,c2 = contours[n1], contours[n2]

        c1,c2 = c1[:,0,:],c2[:,0,:]

        m1 = np.repeat([c1],c2.shape[0],axis=0)
        m2 = np.repeat([c2],c1.shape[0],axis=0)
        t2 = np.transpose(m2,axes=(1,0,2))
        diff = np.sqrt(np.sum(np.power(m1-t2,2),axis=2))

        return np.min(diff)



    def Groups():
        r,a = [],[]
        ## breadth first search
        pool = range(len(contours))
        area = 0
        while pool:
            seen = set([pool[0]])
            ans = set([pool[0]])
            sea = set([pool[0]])
            area = cv2.contourArea(contours[pool[0]])


            while sea:
                nsea = set([])

                for c in sea:
                    for n in pool:
                        if n not in seen and Distance(c,n)<25:
                            #print Distance(c,n),Distance(c,n)<8
                            area += cv2.contourArea(contours[n])
                            ans.add(n)
                            nsea.add(n)
                            seen.add(n)
                sea = nsea

            pool = [_ for _ in pool if _ not in ans]


            #print pool
            a.append(area)
            r.append(ans)

        r = [[contours[__] for __ in _] for _ in r]

        return zip(*[(x,y) for x,y in zip(r,a) if y>500])
        
    

    try:
        gpc,gpa = Groups()
    except:
        return False
 

    gpc = [np.concatenate(g,axis=0) for g in gpc]

    
    cnt = gpc[np.argmax(gpa)]
    hull = cv2.convexHull(cnt)
    #############################
    def area(*i):
        a,b,c= hull[i,0]
        return np.abs(np.cross(b-c,c-a))

    A = 0;B = 1; C = 2; n=hull.shape[0]
    bA= A; bB= B; bC= C #The "best" triple of points
    while True: #loop A
        while True: #loop B
            while area(A, B, C) <= area(A, B, (C+1)%n): #loop C
                C = (C+1)%n
            if area(A, B, C) <= area(A, (B+1)%n, C): 
                B = (B+1)%n
                continue
            else:
                break

        if area(A, B, C) > area(bA, bB, bC):
            bA = A; bB = B; bC = C

        A = (A+1)%n
        if A==B: B = (B+1)%n
        if B==C: C = (C+1)%n
        if A==0: break

    cnt = gpc[np.argmax(gpa)]
    hull = cv2.convexHull(cnt)

    return hull[[bA,bB,bC],0]


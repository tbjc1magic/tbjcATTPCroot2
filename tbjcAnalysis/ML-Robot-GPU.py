
# coding: utf-8

# In[25]:

from sqlalchemy import *
import numpy as np
import pandas as pd
from scipy.ndimage.interpolation import shift
from scipy.interpolate import interp1d
from scipy.optimize import newton
from scipy import signal
from scipy.signal import savgol_filter
import peakutils
import random
import tensorflow as tf
import time
#from IPython import display
#get_ipython().magic(u'matplotlib inline')

# In[2]:

engine = create_engine('sqlite+pysqlite:///../SQL/72_8.db')

# In[3]:

import time
start_time = time.time()
ADCdf = pd.io.sql.read_sql("SELECT * FROM ADC", engine)
Hitdf = pd.io.sql.read_sql("SELECT * FROM Hit", engine)
PSAdf = pd.io.sql.read_sql("SELECT * FROM PSA", engine)
ProtoMapdf = pd.io.sql.read_sql("SELECT * FROM ProtoMap", engine)
#RawADCdf = pd.io.sql.read_sql("SELECT * FROM RawADC", engine)
end_time = time.time()
print end_time-start_time

# In[4]:

def Process(Event):
    mesh = Event.iloc[:,3:][Event.iloc[:,3:]>20].sum(axis=0)
    return mesh
start_time = time.time()
mesh_df = ADCdf.groupby('EventID').apply(Process)
end_time = time.time()
print end_time-start_time

# In[5]:

def convolve(event):
    mesh_convol = np.convolve(event.values,([1]*10)[::-1],'same')
    return pd.Series(mesh_convol,index=['t'+str(i) for i in range(512)])
convol_df=mesh_df.apply(convolve,axis=1)
convol_df.reset_index(inplace=True)

obs = pd.read_csv('72_8.csv',header=None)
obs= obs.apply(pd.to_numeric)
fd_df = convol_df[obs.iloc[:1928,1]]

# In[8]:

range_df = pd.read_csv('range.csv',header=None)
range_df.columns=['index','range']
range_df = range_df['range'].str.extract('\((.*),(.*)\)',expand=False)
range_df.columns = ['left','right']
range_df = range_df.astype(np.float)

# In[34]:

def Generate1Data(r_df,d_df):

    r = r_df.sample(1)
    try:
        s = random.randint(50-r.values[0][1],470-r.values[0][0])
    except:
        print "bb",r.values[0]
        s = random.randint(50-r.values[0][1],400-r.values[0][1])
    #print r.index[0],s,r.values[0]

    tmp = d_df.ix[r.index[0]].iloc[1:]
    tmp = tmp.copy(deep=True)
    tmp.iloc[:10] = 0

    if s:
        v = (int(s/abs(s)))
    else:
        v = 0

    tmp1 =  tmp.shift(s).replace(np.nan,tmp.iloc[v]).values
    #tmp.plot()
    #tmp1['left'] = r['left'].values[0]
    #tmp1['right'] = r['right'].values[0]
    #print tmp1
    fa = random.uniform(0.2,5)
    return tmp1*fa, abs(r.values[0][0]-r.values[0][1])

def GenerateDataR(r_df,d_df,size):

    xlist = []
    ylist = []
    for _ in range(size):
        x,y = Generate1Data(r_df,d_df)
        xlist.append(x)
        ylist.append(y)

    return np.array(xlist),np.array(ylist)

sub = range_df.ix[fd_df.index]
sub = sub[~sub.isnull()]

GenerateData = lambda x: GenerateDataR(sub,fd_df,x)

# In[35]:

def accuracy(y_,y):
    return 1-np.sum(np.sqrt((y_-y)*(y_-y)))/np.sum(np.abs(y))

optimizer, prediction = None, None
def train(hidden=[200],training_rate = 0.0001,decay_rate = 0.96, batchSize = 30,testSzie = 100,
         trainingLoops = 10000):
    optimizer, prediction = None, None
    graph = tf.Graph()
    with graph.as_default():
        FN=512
        yd = 1
        x = tf.placeholder(tf.float32,[None, FN])
        y = tf.placeholder(tf.float32,[None, yd])

        bs = []
        Ws = []

        pre = FN

        for idx, s in enumerate(hidden):
            W = tf.get_variable("W"+str(idx), shape=[pre,s],initializer=tf.contrib.layers.xavier_initializer())
            b = tf.Variable(tf.zeros([s]))
            pre = s
            Ws.append(W)
            bs.append(b)

        W = tf.get_variable("Wy", shape=[pre,yd],initializer=tf.contrib.layers.xavier_initializer())
        b = tf.Variable(tf.zeros([yd]))
        Ws.append(W)
        bs.append(b)

        pre = x
        for W,b in zip(Ws[:-1],bs[:-1]):
            pre = tf.nn.relu(tf.matmul(pre,W) + b)
            #pre = tf.nn.dropout(pre,0.8)

        logits = tf.matmul(pre,Ws[-1])+bs[-1]

        loss = tf.reduce_mean(tf.square(logits-y))

        #optimizer = tf.train.GradientDescentOptimizer(training_rate).minimize(loss)
        optimizer = tf.train.AdamOptimizer(training_rate).minimize(loss)
        prediction = logits

    ac = []
    st =[]
    lo = []
    r_previous = None
    with tf.Session(graph=graph) as session:
        tf.global_variables_initializer().run()
        print('Initialized')
        for j in range(trainingLoops):

            x_,y_ = GenerateData(batchSize)
            #print x_.shape
            feed_dict = {x:x_,y:y_[:,None]}
            _, l, predict = session.run([optimizer,loss, prediction], feed_dict=feed_dict)
            if j %1000 == 0:
                acc = accuracy(predict.flatten(),y_)
                #st.append(j)
                #ac.append(acc)
                #lo.append(l)

                print "step {} prediction: {}; the loss {}; std:{}".format(j,acc,l,np.std(predict.flatten()))

                time.sleep(0.1)

        x_,y_ = GenerateData(testSzie)
        feed_dict = {x:x_,y:y_[:,None]}
        predict = session.run( prediction, feed_dict=feed_dict)
        print "test accuracy {:.2f}%".format(accuracy(predict.flatten(),y_)*100)

        for a,b in zip(predict.flatten(),y_):
            print a,b

# In[37]:

train(hidden=[200,100,50,10],training_rate=1e-5, batchSize = 100,testSzie = 100,trainingLoops = 100000)

# iN[ ]:

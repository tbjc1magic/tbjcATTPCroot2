
# coding: utf-8

# In[171]:

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

# In[344]:

##### gaussian wave ############

def GenerateGaussianPulse(width, mean, height, sigma):
    def Gaussian(x, mean, height_, sigma):
        return height_*np.exp(-(x-mean)*(x-mean)/(2*float(sigma)*float(sigma)))

    xx = np.array(range(width))
    return Gaussian(xx, mean, height, sigma)

def Generate1GaussianPulse(width,height,sigma):
    gap = 50
    sigma_ = random.uniform( 0.1*sigma,sigma*2)
    mean = random.uniform(gap, width-gap)
    height_ = random.uniform(height*0.2,height*3)
    return GenerateGaussianPulse(width, mean, height_, sigma_), sigma_

def GenerateGaussianData(size):
    data = []
    label = []
    for _ in range(size):
        x,y = Generate1GaussianPulse(512, 20,20)
        data.append(x)
        label.append(y)

    return np.array(data),np.array(label)
#plt.plot(GenerateGaussianData(10)[0].T);

#### square wave ##########

def Generate1SquareWave(width, start, end, height, conv):
    xx = np.array(range(width))
    yy = np.logical_and(xx>start, xx<end)*height
    return np.convolve((range(conv)+range(conv)[::-1])[::-1],yy,'SAME')/float(sum(range(conv)))/2.0

def Generate1SquareData(width,height,conv):
    gap = 50
    start = random.uniform(gap,width-gap*3)
    end = random.uniform(start+gap,width-gap)

    height_ = random.uniform(height*0.2,height*3)
    return Generate1SquareWave(width,start,end,height_,conv),end-start

def GenerateSquareData(size):
    data = []
    label = []
    for _ in range(size):
        x,y = Generate1SquareData(512, 10,20)
        data.append(x)
        label.append(y)

    return np.array(data),np.array(label)

######### noisy gaussian ########
def GenerateNoisyGaussianData(size):
    data = []
    label = []
    for _ in range(size):
        x,y = Generate1GaussianPulse(512, 50,20)
        data.append(x+np.random.normal(0, 1.5, size=512))
        label.append(y)

    return np.array(data),np.array(label)

######### noisy gaussian ########
def GenerateNoisyGaussianData(size):
    data = []
    label = []
    for _ in range(size):
        x,y = Generate1GaussianPulse(512, 50,20)
        data.append(x+np.random.normal(0, 1.5, size=512))
        label.append(y)

    return np.array(data),np.array(label)

######### noisy square wave ########
def GenerateNoisySquareData(size):
    data = []
    label = []
    for _ in range(size):
        x,y = Generate1SquareData(512, 50,20)
        data.append(x+np.random.normal(0, 2, size=512))
        label.append(y)

    return np.array(data),np.array(label)

######### spiky noise ##############

def SpikeWave(mean, sigma, height):
    epsilon =1e1
    x = np.array(range(512))
    return height/(1+np.power(x-mean,4)/float(sigma)/float(sigma))

######### noisy square wave ########
def GenerateSpikyNoisySquareData(size):
    data = []
    label = []
    for _ in range(size):
        x,y = Generate1SquareData(512, 50,20)

        SpikeMean = random.uniform(50,450)
        SpikeSTD = random.uniform(2,20)
        SpikeH = random.uniform(10,100)

        SpikeNoise = np.array([0 for _ in range(512)])

        data.append(x+np.random.normal(0, 2, size=512)+SpikeWave(SpikeMean,SpikeSTD,SpikeH))
        label.append(y)

    return np.array(data),np.array(label)

####################################

#plt.plot(Spike(100,50,100))
def Normalizer(f):
    def wrapper(*args, **kwargs):
        x,y = f(*args, **kwargs)
        return x/np.max(x,axis=1).astype(float)[:,None],y

    return wrapper

GenerateData = Normalizer(GenerateSpikyNoisySquareData)

# In[345]:

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
                st.append(j)
                ac.append(acc)
                lo.append(l)

                print "step {} prediction: {}; the loss {}; std:{}".format(j,acc,l,np.std(predict.flatten()))

                time.sleep(0.1)

        x_,y_ = GenerateData(testSzie)
        feed_dict = {x:x_,y:y_[:,None]}
        predict = session.run( prediction, feed_dict=feed_dict)
        print "test accuracy {:.2f}%".format(accuracy(predict.flatten(),y_)*100)

        for a,b in zip(predict.flatten(),y_):
            print a,b

# In[347]:

train(hidden=[512,200,100,50,10],training_rate=1e-5, batchSize = 100,testSzie = 100,trainingLoops = 1000000)
#train(hidden=[200,10],training_rate=1e-5, batchSize = 100,testSzie = 100,trainingLoops = 30000)

# In[ ]:

x,y = GenerateData(100)
print accuracy(y,y[:,None].flatten())

# In[626]:

x_,y_ = GenerateData(sub,fd_df,100)
x_.shape

# In[ ]:

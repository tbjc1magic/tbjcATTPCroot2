
import random
import numpy as np
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

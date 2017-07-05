import numpy as np
import tensorflow as tf
import time
import cv2
from PIL import Image
from PIL import ImageDraw
import operator
from tensorflow.python.client import device_lib

def get_available_gpus():
    local_device_protos = device_lib.list_local_devices()
    return [x.name for x in local_device_protos if x.device_type == 'GPU']

print get_available_gpus()

def GenerateData(size = 1):

    out_image = []
    out_p = []

    for _ in range(size):

        img = Image.new('L', (500,300), 0)
        draw = ImageDraw.Draw(img)

        cx,cy=150,250
        sx,sy  = 20,20
        ay,ax = np.random.normal()*sx+cx, np.random.normal()*sy+cy
        ly,lx = 20,20
        bx,by = (np.random.normal()+1.5)*lx, (np.random.normal()+1.5)*ly
        cx,cy = (np.random.normal()+1.5)*lx, -(np.random.normal()+1.5)*ly

        dx,dy = np.random.normal()*5-150, np.random.normal()*2

        draw.line([(int(ax), int(ay)),(int(bx+ax), int(by+ay))], fill=255, width=5)
        draw.line([(int(ax), int(ay)),(int(cx+ax), int(cy+ay))], fill=255, width=5)
        draw.line([(int(ax), int(ay)),(int(dx+ax), int(dy+ay))], fill=255, width=5)

        image = np.asarray(img)
        image = cv2.dilate(image, None, iterations=1)
        image = cv2.blur(image,(5,5))
        ret,image = cv2.threshold(image,127,255,0)

        #print image[:,:,None]

        out_image.append(image[:,:,None])
        out_p.append([ax,ay])

    return np.array(out_image), np.array(out_p)

#hidden = [500,300,100,10]
hidden = [8000,4000,1000,500,100,10]

graph = tf.Graph()
tf.device('/gpu:1')
with graph.as_default():
    xr,xc = 300,500
    yd=2

    y = tf.placeholder(tf.float32,[None, yd])

    x = tf.placeholder(tf.float32,[None, xr,xc,1])

    #convol = [{'CM':(4,4,1,32),'CS':(1,4,4,1),'MM':(1,5,5,1),'MS':(1,5,5,1)}]
    convol = [{'CM':(15,15,1,32),'CS':(1,5,5,1),'MM':(1,2,2,1),'MS':(1,2,2,1)},
              {'CM':(4,4,32,64),'CS':(1,1,1,1),'MM':(1,2,2,1),'MS':(1,2,2,1)},
              {'CM':(2,2,64,256),'CS':(1,1,1,1),'MM':(1,2,2,1),'MS':(1,2,2,1)},
            ]
    x_ = x

    for idx,d in enumerate(convol):
        W = tf.get_variable("Wc"+str(idx), shape=d['CM'], initializer=tf.contrib.layers.xavier_initializer())
        b = tf.Variable(tf.zeros([d['CM'][-1]]))
        x_ = tf.nn.relu(tf.nn.conv2d(x_, W, strides=d['CS'], padding='VALID')+b)
        ## max pooling
        x_ = tf.nn.max_pool(x_, ksize=d['MM'], strides=d['MS'], padding='VALID')

        xr = (xr-d['CM'][0])/d['CS'][1]+1
        xr = (xr-d['MM'][1])/d['MS'][1]+1

        xc = (xc-d['CM'][1])/d['CS'][2]+1
        xc = (xc-d['MM'][2])/d['MS'][2]+1

        print xr,xc

    bs = []
    Ws = []

    shape = tf.shape(x_)

    #print xr,xc,convol[-1]['CM'][-1]
    sp = xr*xc*convol[-1]['CM'][-1]
    ss = sp

    for idx, s in enumerate(hidden):
        W = tf.get_variable("W"+str(idx), shape=[ss,s],initializer=tf.contrib.layers.xavier_initializer())
        b = tf.Variable(tf.zeros([s]))
        ss = s
        Ws.append(W)
        bs.append(b)

    W = tf.get_variable("Wy", shape=[ss,yd],initializer=tf.contrib.layers.xavier_initializer())
    b = tf.Variable(tf.zeros([yd]))
    Ws.append(W)
    bs.append(b)

    #shape = tf.shape(x_)
    pre = tf.reshape(x_,[-1,sp])

    for W,b in zip(Ws[:-1],bs[:-1]):
        pre = tf.nn.relu(tf.matmul(pre,W) + b)
        #pre = tf.nn.dropout(pre,0.8)

    logits = tf.matmul(pre,Ws[-1])+bs[-1]

    loss = tf.reduce_mean(tf.sqrt(tf.reduce_sum( tf.square(logits-y),axis=1)))

    #loss = tf.reduce_mean(tf.square(logits-y))

    training_rate = 0.1
    #optimizer = tf.train.GradientDescentOptimizer(training_rate).minimize(loss)
    optimizer = tf.train.AdamOptimizer(training_rate).minimize(loss)
    prediction = logits
    init_op = tf.global_variables_initializer()

st =[]
lo = []
r_previous = None
sess = tf.Session(graph=graph)

sess.run(init_op)
print('Initialized')

for loop in range(5000):
    x_,y_ = GenerateData(100)
    feed_dict = {x:x_,y:y_}

    #print sess.run([shape], feed_dict=feed_dict)[0]
    _, l, predict = sess.run([optimizer,loss, prediction], feed_dict=feed_dict)

    if loop %100 == 0:
        st.append(loop)
        lo.append(l)
	print l


# coding: utf-8

# In[ ]:

from sqlalchemy import *
import ROOT
import numpy as np
import pylab as plt

# In[ ]:

import time
start_time = time.time()

# In[ ]:

import sys

if len(sys.argv)!=3:
    print "Provide input file name and output file name!!!"

#engine = create_engine('sqlite+pysqlite:///../SQL/alpha.db')
engine = create_engine('sqlite+pysqlite:///'+sys.argv[2])

# In[ ]:

import model
from sqlalchemy import orm

# In[ ]:

model.metadata.bind = engine
model.metadata.create_all()

# Set up the session
sm = orm.sessionmaker(bind=engine, autoflush=True, autocommit=False,
    expire_on_commit=True)
session = orm.scoped_session(sm)

# In[ ]:

#with open("../map/proto.map") as f:
#
#    for line in f:
#        padnum,padname,padx,pady,binnum = line.strip().split()
#        padnum,padname,padx,pady,binnum = int(padnum),padname,float(padx),float(pady),int(binnum)
#
#        PadMap = model.ProtoMap()
#        PadMap.PadNum = padnum
#        PadMap.PadName = padname
#        PadMap.PadX = padx
#        PadMap.PadY = pady
#        PadMap.BinNum = binnum
#        session.add(PadMap)
#    session.commit()

# In[ ]:

ROOT.gROOT.ProcessLine(
"struct tbjcPad\
{\
    int EventNum;\
    int PadNum;\
    float PadX;\
    float PadY;\
    int IsValid;\
    int RawAdc[512];\
    int MaxAdcIdx;\
    int IsPedestalSubtracted;\
    float Adc[512];\
    int IsAux;\
};" )
ROOT.gROOT.ProcessLine(
"struct tbjcPSA\
{\
    int EventNum;\
    float mesh[512];\
    float RhoVariance;\
    float QEventTot;\
};" )
ROOT.gROOT.ProcessLine(
"struct tbjcHit\
{\
    int EventNum;\
    int PadNum;\
    int hitNum;\
    float x;\
    float y;\
    float z;\
    float charge;\
};" )

# In[ ]:

#f = ROOT.TFile("../output.root")
f = ROOT.TFile(sys.argv[1])
tree = f.Get("tree")
tb0 = tree.GetBranch("Pad")
tb1 = tree.GetBranch("PSA")
tb2 = tree.GetBranch("Hit")

tbjcPadReg = ROOT.tbjcPad()
tbjcPSAReg = ROOT.tbjcPSA()
tbjcHitReg = ROOT.tbjcHit()

tb0.SetAddress(ROOT.AddressOf(tbjcPadReg,'EventNum'))
tb1.SetAddress(ROOT.AddressOf(tbjcPSAReg,'EventNum'))
tb2.SetAddress(ROOT.AddressOf(tbjcHitReg,'EventNum'))

# In[ ]:

TB = 512
for i in xrange(tb0.GetEntries()):

    if i % 100 == 0:
        print i

    tb0.GetEntry(i)
    RawADC = model.RawADC()
    #RawADC.ID = i
    RawADC.EventID = tbjcPadReg.EventNum
    RawADC.PadNum = tbjcPadReg.PadNum

    for j in range(TB):
        setattr(RawADC, 't'+str(j), tbjcPadReg.RawAdc[j])
    #session.add(RawADC)

    ADC = model.ADC()
    #ADC.ID = i
    ADC.EventID = tbjcPadReg.EventNum
    ADC.PadNum = tbjcPadReg.PadNum
    for j in range(TB):
        setattr(ADC, 't'+str(j), tbjcPadReg.Adc[j])
    session.add(ADC)

    #session.flush()
    if i %10000 == 0:
        session.commit()
session.commit()
# In[ ]:

TB = 512
for i in xrange(tb1.GetEntries()):
    if i % 100 == 0:
        print i
    tb1.GetEntry(i)
    PSA = model.PSA()
    #PSA.ID = i
    PSA.EventID = tbjcPSAReg.EventNum
    PSA.QEventTot = tbjcPSAReg.QEventTot

    for j in range(TB):
        setattr(PSA, 'mesh'+str(j), tbjcPSAReg.mesh[j])
    #session.add(PSA)

    #session.flush()
    if i %10000 == 0:
        session.commit()
session.commit()
# In[ ]:

for i in xrange(tb2.GetEntries()):
    if i % 100 == 0:
        print i
    tb2.GetEntry(i)
    Hit = model.Hit()
    #Hit.ID = i
    Hit.EventID = tbjcHitReg.EventNum
    Hit.PadNum = tbjcHitReg.PadNum
    Hit.x = tbjcHitReg.x
    Hit.y = tbjcHitReg.y
    Hit.z = tbjcHitReg.z
    Hit.charge = tbjcHitReg.charge

    #session.add(Hit)

    #session.flush()
    if i %10000 == 0:
        session.commit()
session.commit()

# In[ ]:

end_time = time.time()

# In[ ]:

print end_time - start_time

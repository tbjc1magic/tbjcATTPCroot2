import ROOT
import numpy as np
from array import array
import pandas as pd
from sqlalchemy import *
import model
from sqlalchemy import orm

#print 'stage0'

ROOT.gROOT.ProcessLine(
"struct tbjcStruct\
{\
    short uval[512];\
};" )

engineMap = create_engine('sqlite+pysqlite:///../data/SQL/Tin132/map.sql')
LookMapdf = pd.io.sql.read_sql("SELECT * FROM LookMap", engineMap)

#print "stage1"

def FindPadNum(AgetID,ChannelID):
    if ChannelID in [11,22,45,56]:
        return -ChannelID
    try:
        return LookMapdf[(LookMapdf['AgetID']==AgetID)&(LookMapdf['ChannelID']==ChannelID)]['PadNum'].values[0]
    except:
        return None

f = ROOT.TFile("../data/ANL_March_2013/ROOT_files/run42.root")
tree = f.Get("AutoTree")

#print "stage2"

engine = create_engine('sqlite+pysqlite:///../d.sql')

model.metadata.bind = engine
model.metadata.create_all()

sm = orm.sessionmaker(bind=engine, autoflush=True, autocommit=False, expire_on_commit=True)
session = orm.scoped_session(sm)

#print "hererere??"

import sys

for AgetID in range(4):
    #print "alalal"
    for ChannelID in range(68):

        tb0 = tree.GetBranch("asic_{}_{:02d}".format(AgetID,ChannelID))
        tbjcStruct = ROOT.tbjcStruct()
        tb0.SetAddress(ROOT.AddressOf(tbjcStruct,'uval'))
        PadNum = FindPadNum(AgetID,ChannelID)

        if not PadNum:
            continue

        #try:
        #    PadNum = FindPadNum(AgetID,ChannelID)
        #except:
        #    print AgetID,ChannelID
        #    sys.exit(1)

        TB = 512
        #for i in xrange(tb0.GetEntries()):
        for i in xrange(100):
            if i % 100 == 0:
                print i

            #continue
            tb0.GetEntry(i)
            RawADC = model.RawADC()
            RawADC.EventID = i
            RawADC.PadNum = PadNum

            for j in range(TB):
                setattr(RawADC, 't'+str(j), tbjcStruct.uval[j])
            session.add(RawADC)

            #session.flush()
            if i %10000 == 0:
                session.commit()
        session.commit()

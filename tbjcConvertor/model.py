from sqlalchemy import orm
import datetime
from sqlalchemy import schema, types

metadata = schema.MetaData()
TB = 512

###################################
############ ProtoMap #############
###################################

protomap_table = schema.Table('ProtoMap', metadata,
    schema.Column('PadNum', types.Integer, primary_key=True ),
    schema.Column('PadName', types.String(15), nullable=False),
    schema.Column('PadX', types.Float, nullable=False),
    schema.Column('PadY', types.Float, nullable=False),
    schema.Column('BinNum', types.Integer, nullable=False),
)

class ProtoMap(object):
    pass

orm.mapper(ProtoMap, protomap_table)

###################################
############ rawadc ###############
###################################

schemalist = []
for i in range(TB):
    tmp = schema.Column('t'+str(i), types.Integer)#, nullable=False),
    schemalist.append(tmp)

rawadc_table = schema.Table('RawADC', metadata,
    schema.Column('ID', types.Integer, primary_key=True, autoincrement=True ),
    schema.Column('EventID', types.Integer, nullable=False),
    schema.Column('PadNum', types.Integer, nullable=False),
    *schemalist
)

class RawADC(object):
    pass

orm.mapper(RawADC, rawadc_table)

###################################
############### adc ###############
###################################

schemalist = []
for i in range(TB):
    tmp = schema.Column('t'+str(i), types.Float)#, nullable=False),
    schemalist.append(tmp)

adc_table = schema.Table('ADC', metadata,
    schema.Column('ID', types.Integer, primary_key=True, autoincrement=True ),
    schema.Column('EventID', types.Integer, nullable=False),
    schema.Column('PadNum', types.Integer, nullable=False),
    *schemalist
)

class ADC(object):
    pass

orm.mapper(ADC, adc_table)

###################################
############### PSA ###############
###################################

schemalist = []
for i in range(TB):
    tmp = schema.Column('mesh'+str(i), types.Float)#, nullable=False),
    schemalist.append(tmp)

psa_table = schema.Table('PSA', metadata,
    schema.Column('ID', types.Integer, primary_key=True, autoincrement=True ),
    schema.Column('EventID', types.Integer, nullable=False),
    schema.Column('QEventTot', types.Float, nullable=False),
    *schemalist
)

class PSA(object):
    pass

orm.mapper(PSA, psa_table)

###################################
############### Hit ###############
###################################

hit_table = schema.Table('Hit', metadata,
    schema.Column('ID', types.Integer, primary_key=True, autoincrement=True ),
    schema.Column('EventID', types.Integer, nullable=False),
    schema.Column('PadNum', types.Integer, nullable=False),
    schema.Column('x', types.Float, nullable=False),
    schema.Column('y', types.Float, nullable=False),
    schema.Column('z', types.Float, nullable=False),
    schema.Column('charge', types.Float, nullable=False),
)

class Hit(object):
    pass

orm.mapper(Hit, hit_table)

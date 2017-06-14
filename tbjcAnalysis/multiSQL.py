from sqlalchemy import *
import numpy as np
import pylab as plt
import pandas as pd
from scipy.ndimage.interpolation import shift
from matplotlib.colors import LogNorm
from scipy.interpolate import interp1d
from scipy.optimize import newton
import peakutils
import time
from multiprocessing import Pool
import multiprocessing
import subprocess

def ProcessDB(path):
    print 'start....', path
    engine = create_engine('sqlite+pysqlite:///'+path)
    ADCdf = pd.io.sql.read_sql("SELECT * FROM ADC", engine)#, chunksize=1000)
    return ADCdf

def ParallelDB2(paths,workers=10):
    pool = Pool(workers)
    time_start = time.time()
    dfs = pool.map(ProcessDB,paths)
    print time.time()-time_start
    return dfs

def ParallelDB1():

    processes = []
    for _i in range(10):
        print 'start....', _i
        _p = multiprocessing.Process(target=ProcessDB, args=(_i,))
        _p.start()
        processes.append(_p)

    time_start = time.time()
    for _p in processes:
        _p.join()

    print time.time()-time_start

#ParallelDB2()

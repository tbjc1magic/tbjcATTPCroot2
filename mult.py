from subprocess import call
from multiprocessing import Pool
import re
import os
import subprocess

def runProcess(exe):
    p = subprocess.Popen(exe,shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    p.wait()

    err = p.stderr.readlines()

    if len(err) != 0:
        for line in err:
            print line
        exit(1)

    out = p.stdout.readlines()
    return out

def run(f):
    p = '.*run_(\d{4}).*CoBo.*_(\d+)\.graw'
    s = re.search(p,f)

    fname = s.group(1)+'_'+s.group(2)

    call(['build/example',fname+".root",f])
    cmd = "python tbjcConvertor/Convertor_sqlite.py "+fname+'.root'+" data/SQL/"+fname+'.db'
    call(cmd.split(),shell=False)
    call(['rm',fname+'.root'])

import time

def main():
    pool = Pool(processes=10)

    parentPath = 'data/10C/'

    param = []
    paths = ['run_0078/','run_0085/']

    for path in paths:

        files = runProcess("ls "+parentPath+path)
        files = [_.strip() for _ in files]

        p = '^CoBo.*_(\d+)\.graw$'

        for f in files:
            s = re.search(p,f)
            try:
                param.append(parentPath+path+s.group(0))
            except:
                pass

    start_time = time.time()
    pool.map(run,param)

    end_time = time.time()

    print "total process cost "+str(end_time - start_time)

main()

import multiSQL

parant = '../data/SQL/'

fs = [parant+'0075_{:04d}.db'.format(i) for i in range(2)]
#print fs
multiSQL.ParallelDB2(fs,2)

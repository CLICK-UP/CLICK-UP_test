import os
import time

starttime = time.time()
print('create start time = %s '%(starttime))
os.system('make')
endtime = time.time()
print('create end time = %s '%(endtime))
print('end time - start time = %ss'%(endtime - starttime))


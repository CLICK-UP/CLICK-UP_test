import os
import time

starttime = time.time()
print('update start time = %s '%(starttime))
os.system('make')
endtime = time.time()
print('update end time = %s '%(endtime))
print('end time - start time = %ss'%(endtime - starttime))


import time  
import os  
import threading  
  
def execCmd(cmd,f):  
    try:  
	start = time.time()
        print "commend%sstart at%s" % (cmd,start)  
        os.system(cmd)  
        end = time.time()
        print "commend%send at%s" % (cmd,end)
	print "commend%s end time - start time = %s"% (cmd, end-start)
	f.write('commend :' + cmd + 'complete time = ' + str(end-start) + '\n')  
    except Exception, e:  
        print '%s\t commend failure,reason\r\n%s' % (cmd,e)  
  
if __name__ == '__main__':    
    cmds = []

    f = open('./time.log', 'a+')

    for x in xrange(1,51):
        cmds.append('wget --timeout=60 http://10.0.0.2:8080/file/'+ str(x) + '.file')
          
       
    threads = []  
    starttime = time.time()
  
    for cmd in cmds:  
        th = threading.Thread(target=execCmd, args=(cmd,f))  
        th.start()  
        threads.append(th)  
             
    for th in threads:  
        th.join()  
    endtime = time.time()       
    print endtime-starttime
    f.write('all time = ' + str(endtime-starttime))
    f.close()  

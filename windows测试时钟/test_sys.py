import time
import datetime
import os

testtime = 10 
board = '5'
rate = 60
after_ticks = 1
before_ticks = 1

def strToint(strr):
    ticks = 0
    c1 = strr.split('result:')
    if len(c1) > 1:
        m1 = c1[1].split('(')
        ticks = int(m1[0].strip())
    return ticks
    
if __name__ == '__main__':
 



    str1 = os.popen("python ser.py tick_ulong_get").read()
    time.sleep(testtime)
    start = time.time()
    str2 = os.popen("python ser.py tick_ulong_get").read()
    end = time.time()
#   print str1
#    print str2

    error_ticks = (end - start) * rate 
#    print error_ticks

    before_ticks = strToint(str1)
    after_ticks = strToint(str2)
    
#    print 'system value: %d'%(after_ticks - before_ticks)
    print 'real value: %d'%(after_ticks- before_ticks - error_ticks)
    print 'expected value: %d'%(rate * testtime)
#    print 'error value: %d'%error_ticks

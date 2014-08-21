#!/usr/bin/env python
"""
A simple module used to log approximate runtimes

Modified from http://stackoverflow.com/a/1557906
Warning: runtimes tested to be not accurate
"""

import atexit
import sys
from time import clock

def secondsToStr(t):
    return "%d:%02d:%02d.%03d" % \
        reduce(lambda ll,b : divmod(ll[0],b) + ll[1:],
            [(t*1000,),1000,60,60])

line = "="*40
def log(s, elapsed=None):
    print line
    print secondsToStr(clock()), '-', s
    if elapsed:
        print "Elapsed time:", elapsed
    print line
    print
    sys.stdout.flush()

def endlog():
    end = clock()
    elapsed = end-start
    log("End Program", secondsToStr(elapsed))

def log_return():
    end = clock()
    elapsed = end - start
    return secondsToStr(elapsed)

def now():
    return secondsToStr(clock())

def reset():
    start = clock()

start = clock()
atexit.register(endlog)
log("Start Program")
sys.stdout.flush()
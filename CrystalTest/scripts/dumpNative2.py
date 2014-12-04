#!/usr/bin/python
import timing
import serial
import sys
import numpy as np
import math
from ROOT import TFile, TTree, TH1D
from array import array

import Queue
import time
from threading import Thread

def source(serialportname, chunkSize, nmax):
    """returns voltage data from the serial port, looping forever"""
    # Get handle to serial port
    s = serial.Serial(serialportname)
    print("port init")
    voltages = []
    print("collecting data...")
    curr = 0
    while curr<nmax:
        print curr
        data = s.read(chunkSize)
        print "here 1"
        data = np.fromstring(data, dtype=np.uint16)
        q.put(data)
        curr = curr + 1
    timing.log("data collected", timing.log_return())


def worker():
    while True:
        vals = q.get()
        print "got data"
        iterable = iter(xrange(0,len(vals)))
        for i in iterable:
            adc[0] = vals[i]
            rawtree.Fill()
                
                

# PARAMETERS
serialportname = '/dev/cu.usbmodem1421'
chunkSize = 2**13 # One loop collects this many data points
loops = 30000  # Must result in > 34000 data points
rise = 1

rootf = TFile.Open("out_direct.root", "recreate")
rawtree = TTree("raw", "raw data tree")
adc = array('i', [0])
rawtree.Branch("adc", adc, 'adc/I')

q = Queue.Queue(maxsize=0)

### worker queue
t = Thread(target=worker)
t.daemon = True
t.start()

source(serialportname, chunkSize, 1000)
#q.join()

### end
rootf.Write()            
rootf.Close()


#!/usr/bin/python

### read out serial port from Arduino, dump data compressed to a root file
### you need PYROOT installed
### uses a Queue and two threads so that we don't loose any data

import timing
import serial

import numpy as np
from ROOT import TFile, TTree
from array import array

import Queue
from threading import Thread

def source(serialportname, chunkSize, nmax):
    """returns voltage data from the serial port, looping forever"""
    # Get handle to serial port
    s = serial.Serial(serialportname)
    print("collecting data...")
    curr = 0
    while curr<nmax:
        data = s.read(chunkSize)
        data = np.fromstring(data, dtype=np.uint16)
        q.put(data)
        curr = curr + 1
    timing.log("data collected", timing.log_return())


def worker():
    """ worker thread that puts data into root tree """
    while True:
        vals = q.get()
        iterable = iter(xrange(0,len(vals)))
        print len(vals)
        for i in iterable:
            adc[0] = vals[i]
            if ( adc[0] - 2036 > 10 ) :
                print adc[0]
            rawtree.Fill()
                
                

# PARAMETERS
serialportname = '/dev/cu.usbmodem1421'
chunkSize = 2**13 # size of each read on the serial port, presumably in bytes

# output root file name FIXME
rootf = TFile.Open("out_direct.root", "recreate")
rawtree = TTree("raw", "raw data tree")
adc = array('i', [0])
rawtree.Branch("adc", adc, 'adc/I')

q = Queue.Queue(maxsize=0)

### worker queue
t = Thread(target=worker)
t.daemon = True
t.start()

# last argument is the number of chunkSize reads to do 
source(serialportname, chunkSize, 5000)
#q.join()

### end
rootf.Write()            
rootf.Close()


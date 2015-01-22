#! /usr/bin/python
# scan through joon's files

# when I see a pulse some number of values above the pedestal (d_avg)
# at some time t_thresh save bins with t_0 < t< t_1 where t0 =
# t_thresh - t_win, t_1 = t_thresh + 3*t_win
#
# store the output in a root file

# uses pyroot so you need a root install
# setenv PYTHONPATH $ROOTSYS/lib:${PYTHONPATH}

from ROOT import TFile, TTree, TH1D
import numpy
import sys
import gzip
from array import array
import atexit

def save():
    rootf.Write()
    rootf.Close()

atexit.register(save)

## FIXME
#filename = "/Users/wittich/Downloads/Crystal5_08_01_2014_1221_05/data.txt"
filename = "/Users/wittich/Downloads/Crystal4_07_31_2014_1641_52/data.txt"
if len(sys.argv) > 1 :
    filename = sys.argv[1]
print "input filename is ", filename


# skip header
nheader = 4
maxwords = 5e6
rootf = TFile.Open("out2.root", "recreate")
rawtree = TTree("raw", "raw data tree")
adc = array('i', [0])
rawtree.Branch("adc", adc, 'adc/I')
## time from root tree: use Entry$ for line number

## because joon's file has all the data in one really long line the
## usual tools of reading the file line-by-line don't work, so instead
## I read it character-by-character, and every so often analyze the
## data (after maxwords characters, give or take)

## The files seem to have nheader informational lines that I skip

## joon also seems to have written other data after the raw data;
## we skip that. 

#with open(filename,"r") as f: ## for regular file
with gzip.open(filename,"r") as f: ## for gzip'd file
    # skip header
    for i in range(nheader):
        f.readline()
    niter = 0
    nhisto = 1
    hall = []
    toquit = 0
    # to read big files in chunks
    while True: # loop over file blocks
        niter = niter + 1
        print 'niter = ', niter
        nread = 0
        tdata = ""
        data = []
        ## look at part of the line
        while True: # loop over sub-blocks
            c = f.read(1)
            nread = nread + 1
            if not c:
                print "End of file"
                break
            elif c == ']':
                print 'end of array.'
                toquit = 1
                break
            elif nread > maxwords and c == ',':
                print 'read maxwrds give or take'
                break
            tdata = tdata + c

        data = map(int,tdata[1:].split(', '))

        # for i in range(20):
        #     print i, data[i]

        ## calculate the pedestal and RMS
        d_avg = numpy.average(data)
        d_rms = numpy.std(data)

        print "data avg, rms = ", d_avg, d_rms


        twin4=15
        # once to fill output tree
        iterable = iter(xrange(twin4,len(data)))
        for i in iterable:
            adc[0] = data[i]
            rawtree.Fill()
        # once to fill output histos
        iterable = iter(xrange(twin4,len(data)))
        for i in iterable:
            #print i, data[i]
            if ( data[i] > (d_avg+1.5*d_rms) ) :
                print 'histo ', nhisto, i
                name = "h%d"%nhisto
                h = TH1D(name, name, 4*twin4, 0, 4*twin4)
                b = 1
                for j in range(i-twin4, i+3*twin4):
                    # do pedestal subtraction
                    h.SetBinContent(b, data[j]-d_avg)
                    b = b + 1
                nhisto = nhisto + 1
                hall.append(h)
                # so we don't have the same histo over and over again        
                [iterable.next() for k in range(2*twin4)]

        if toquit:
            break



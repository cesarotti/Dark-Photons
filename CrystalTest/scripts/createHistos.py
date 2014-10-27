#! /usr/bin/env python
# scan through joon's files
# when I see a pulse some number of values above the pedestal (t_thresh)
# save bins with t_0 < t< t_1 where t0 = t_thresh - t_win, t_1 = t_thresh + 3*t_win
#
# store the output in a root file

# uses pyroot so you need a root install
# setenv PYTHONPATH $ROOTSYS/lib:${PYTHONPATH}

from ROOT import TFile, TTree, TH1D
import numpy

## FIXME
#filename = "/Users/wittich/Downloads/Crystal5_08_01_2014_1221_05/data.txt"
filename = "/Users/wittich/Downloads/Crystal4_07_31_2014_1641_52/data.txt"

# skip header
nheader = 4
maxwords = 5e6
rootf = TFile.Open("out2.root", "recreate")

## because joon's file has all the data in one really long line the usual tools
## of reading the file line-by-line don't work, so instead I read it character-by-character,
## and every so often analyze the data (after maxwords characters, give or take)
## The files seem to have nheader informational lines that I skip

## joon also seems to ahve written other data after the raw data;
## we skip that. 

with open(filename) as f:
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
        iterable = iter(xrange(twin4,len(data)))
        for i in iterable:
            #print i, data[i]
            if ( data[i] > (d_avg+3*d_rms) ) :
                print 'histo ', nhisto, i
                name = "h%d"%nhisto
                h = TH1D(name, name, 4*twin4, 0, 4*twin4)
                b = 1
                for j in range(i-twin4, i+3*twin4):
                    h.SetBinContent(b, data[j])
                    b = b + 1
                nhisto = nhisto + 1
                hall.append(h)
                # so we don't have the same histo over and over again        
                [iterable.next() for k in range(2*twin4)]

        if toquit:
            break
rootf.Write()            
rootf.Close()

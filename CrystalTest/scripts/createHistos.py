#! /usr/bin/env python

from ROOT import TFile, TTree, TH1D
import numpy

#filename = "/Users/wittich/Downloads/Crystal5_08_01_2014_1221_05/data.txt"
filename = "/Users/wittich/Downloads/Crystal4_07_31_2014_1641_52/data.txt"

# skip header
nheader = 4
maxwords = 5e6
rootf = TFile.Open("out2.root", "recreate")

with open(filename) as f:
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

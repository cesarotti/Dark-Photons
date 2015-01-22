#! /usr/bin/python
# scan through root files

# when I see a pulse some number of values above the pedestal (d_avg)
# at some time t_thresh save bins with t_0 < t< t_1 where t0 =
# t_thresh - t_win, t_1 = t_thresh + 3*t_win
#
# store the output in a root file

# uses pyroot so you need a root install
# setenv PYTHONPATH $ROOTSYS/lib:${PYTHONPATH}

from ROOT import TFile, TTree, TH1D, TDirectory, gDirectory, gROOT
import numpy
import sys
import collections
import atexit

def save():
    outfile.Write()
    outfile.Close()



twin = 60
data=collections.deque(maxlen=twin)

## FIXME
#filename = "/Users/wittich/Downloads/Crystal5_08_01_2014_1221_05/data.txt"
filename = "Crystal17_08_06_2014_1421_43.root"
if len(sys.argv) > 1 :
    filename = sys.argv[1]
print "input filename is ", filename


gROOT.SetBatch()

infile = TFile.Open(filename, "read")
rawtree = gDirectory.Get("raw")
entries = rawtree.GetEntriesFast()
print "this many entries: ", entries

rawtree.Draw("adc>>htemp")
htemp = gDirectory.Get("htemp")
htemp.Print()
d_avg = htemp.GetMean()
d_rms = htemp.GetRMS()
print "mean, rms for whole sample is ", d_avg, d_rms

outfile = TFile.Open("test.root", "recreate")
atexit.register(save)


niter = 0
nhisto = 1
hall = []
currpos = 0
xbarn= 0.
xsqn = 0.
count = 0

maxwords=5e6
#while currpos < maxwords:
while True:
    v = rawtree.GetEntry(currpos)
    if ( v == 0 ) :
        break
    currpos = currpos + 1
    count = count + 1 # not always same as currpos?
    data.append(rawtree.adc)
    #print data
    ## calculate the pedestal and RMS of the whole data sample
    # if ( (currpos%(10*twin) == 0) or (currpos < twin) ) :
    #     d_avg = numpy.average(data)
    #     d_rms = numpy.std(data)
#        print "data avg, rms = ", d_avg, d_rms, currpos
    pos = twin/4
    if ( (len(data)>=twin) and (data[pos] > (d_avg+5)) ) :
        print 'histo ', nhisto,
        print 'data[pos] = ', data[pos], d_avg, d_rms, pos,currpos
        name = "h%d"%nhisto
        h = TH1D(name, name, twin, 0, twin)
        b = 1
        for j in range(1,twin):
            # do pedestal subtraction
            h.SetBinContent(b, data[j]-d_avg)
            b = b + 1
        nhisto = nhisto + 1
        hall.append(h)
        # so we don't have the same histo over and over again        
        data.clear()
        #break



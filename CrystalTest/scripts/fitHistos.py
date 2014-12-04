#! /usr/bin/python

from ROOT import TFile, TTree, TH1D, TDirectory, gROOT, gDirectory, TF1, TPad, gPad
import numpy
import sys

filename = "./out2.root"
if len(sys.argv) > 1 :
    filename = sys.argv[1]
print "input filename is ", filename



### fit the pulse shape to a landau convoluted with a gaussian
def fit(hhh):
    hh = hhh.Clone("h" + hhh.GetName())
    nbins = hh.GetNbinsX()
    for i in range(1,nbins):
        ### FIXME : 288 is hard-coded pedestal 
        val = hh.GetBinContent(i) 
        hh.SetBinContent(i,val)
        hh.SetBinError(i, 0.01*(val+288.))
    func = TF1("langaus", langaufun, 0, 60,4)
    func.SetParameter(0,.5)
    func.SetParameter(1, 17) ## most probable value
    func.SetParameter(2,170) ## normalization (area)
    func.SetParameter(3,2) ## width of gaussian
    ## the way I set up the histogram the peak is usually
    ## around bin 15-17. We get some of the pre-pulse
    ## but avoid the undershoot by cutting off pretty early
    func.SetRange(10,27) 
    hh.Fit("langaus", "R")
    peaktime = func.GetParameter(1)
    peak = func.Eval(peaktime)
    hh.Write()
    ## if you comment out these next two lines it'll probably go faster
    gPad.Modified()
    gPad.Update()
    return peak

### FIXME
## this loads a file from the root distribution that has a Landau convoluted with a Gaussian
## which I use to fit the signal pulses
gROOT.ProcessLine(".L $ROOTSYS/tutorials/fit/langaus.C")

from ROOT import langaufun

### output of createHistos.py
# open root file with found peak histograms
if len(sys.argv) > 1 :
    filename = sys.argv[1]
print "input filename is ", filename
f = TFile.Open(filename, "read")

dirlist = f.GetListOfKeys()
iter = dirlist.MakeIterator()
key = iter.Next()
histos = {}

bigpeaks = {}

fout = TFile.Open("summed.root", "RECREATE")
hout = TH1D("hout", "peaks", 80, 0, 80)

histo = None
while key:
    if key.GetClassName() == 'TH1D':
        histo = key.ReadObj()
        hName = histo.GetName()
        print "found histogram", hName,
        histos[hName] = histo
        val = fit(histo)
        if val > 50  :
            bigpeaks[hName] = val
        print val
        hout.Fill(val)
    key = iter.Next()

fout.Write()
fout.Close()

print len(bigpeaks.keys())
for k in bigpeaks.keys():
    print k, bigpeaks[k]

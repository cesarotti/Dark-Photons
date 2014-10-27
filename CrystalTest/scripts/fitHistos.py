#! /usr/bin/env python

from ROOT import TFile, TTree, TH1D, TDirectory, gROOT, gDirectory, TF1, TPad, gPad
import numpy



def fit(hhh):
    hh = hhh.Clone("hh")
    nbins = hh.GetNbinsX()
    for i in range(1,nbins):
        val = hh.GetBinContent(i) - 288
        hh.SetBinContent(i,val)
        hh.SetBinError(i, 0.01*(val+288.))
    func = TF1("langaus", langaufun, 0, 60,4)
    func.SetParameter(0,.5)
    func.SetParameter(1, 17)
    func.SetParameter(2,170)
    func.SetParameter(3,2)
    func.SetRange(10,27)
    hh.Fit("langaus", "R")
    peaktime = func.GetParameter(1)
    peak = func.Eval(peaktime)
    gPad.Modified()
    gPad.Update()
    return peak

gROOT.ProcessLine(".L /Users/wittich/scratch/day/root_v5.34.12/tutorials/fit/langaus.C")

from ROOT import langaufun
    
filename = "/Users/wittich/Downloads/out2.root"

# skip header
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

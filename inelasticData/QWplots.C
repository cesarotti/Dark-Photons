#include "TMath.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TMath.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"

#include <iostream>
#include <iomanip>
#include <vector>

const double ENERGY_IN = 5000; // MeV
const double PROTON_MASS = 940; // MeV

// NOTE: theta is in radians
double QSquared(double energy, double theta) {
  return 4*ENERGY_IN*energy*pow(TMath::Sin(theta/2), 2); // MeV^2
}

double WSquared(double energy, double theta) {
  return pow(PROTON_MASS, 2) + 2*PROTON_MASS*(ENERGY_IN - energy) - QSquared(energy, theta); // MeV^2
}

void QWplots() {
    TFile* file = new TFile("final.root");
    TTree* Hits_Info = (TTree*)file->Get("Hits_Info");

    Int_t pID;
    Double_t energy;
    Double_t theta;

    Hits_Info->SetBranchAddress("PDGID", &pID);
    Hits_Info->SetBranchAddress("Energy", &energy);
    Hits_Info->SetBranchAddress("theta", &theta);

    TH2F* hqw = new TH2F("scatter",            // plot label
                         "Q^2 vs W^2",         // title
                         100,                  // x num of bins
                         -10000,               // x min
                         10000,                // x max
                         100,                  // y num of bins
                         -10000,               // y min
                         10000);               // y max

    // all entries and fill the histograms
    int nentries = Hits_Info->GetEntries();
    for (int i=0;i<nentries;i++) {
        std::cout << pID << endl;
        if (pID == 11) {
            hqw->Fill(QSquared(energy, theta*TMath::Pi()/180), WSquared(energy, theta*TMath::Pi()/180));
        }
    }

    TCanvas* canvas = new TCanvas("canvas", "canvas", 700, 700);
    hqw->Draw();
    hqw->GetYaxis()->SetTitle("Q^2 (MeV^2)");
    hqw->GetXaxis()->SetTitle("W^2 (MeV^2)");
    hqw->GetXaxis()->CenterTitle();
    hqw->GetYaxis()->CenterTitle();

}

#include "TFile.h"
#include "TH1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TTree.h"

void gammaPlots() {

    TFile* file = new TFile("final.root");
    TTree* Hits_Info = (TTree*)file->Get("Hits_Info");

    Int_t pID;
    Double_t energy;
    Double_t theta;

    Hits_Info->SetBranchAddress("PDGID", &pID);
    Hits_Info->SetBranchAddress("Energy", &energy);
    Hits_Info->SetBranchAddress("theta", &theta);

    // Numbers
    double deltaOmega = 0.07;
    int neplus = Hits_Info->GetEntries();
    double binSize = 10; // MeV
    double biasFactor = 10000;

    // Initialize histograms
    int nEnergyMax = 2000; // MeV
    int nEnergyBins = nEnergyMax/binSize;

    TH1D* hgammaEnergy = new TH1D("5 < theta < 10" ,            // plot label
                                  "Gamma Energy Distribution",  // title
                                  nEnergyBins,                  // x number of bins
                                  0,                            // x lower bound
                                  nEnergyMax);                  // x upper bound

    // go through all entries and fill the histograms
    Int_t nentries = (Int_t)Hits_Info->GetEntries();
    for (Int_t i=0;i<nentries;i++) {
        Hits_Info->GetEntry(i);

        if (pID == 22) { // gamma
            hgammaEnergy->Fill(energy);
        }
    }

    // create canvas and draw histogram
    TCanvas* canvas = new TCanvas("canvas", "canvas", 700, 700);
    canvas->SetLogy();
    canvas->SetGrid();

    hgammaEnergy->Scale(1/(binSize * neplus * deltaOmega * biasFactor));
    hgammaEnergy->SetFillColor(kBlue);
    hgammaEnergy->SetFillStyle(3001);
    hgammaEnergy->Draw();
    hgammaEnergy->GetXaxis()->SetTitle("Energy (MeV)");
    hgammaEnergy->GetYaxis()->SetTitle("dN/dE");
    hgammaEnergy->GetXaxis()->CenterTitle();
    hgammaEnergy->GetYaxis()->CenterTitle();

}

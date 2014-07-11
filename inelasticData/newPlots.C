#include "TFile.h"
#include "TH1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TLegend.h"

void newPlots() {

    TFile* brems = new TFile("brems.root");
    TFile* inelastic = new TFile("inelastic.root");

    THStack* hs = new THStack("hs","Gammas from Bremsstrahlung and Inelastic Collision");

    TH1D* hbrems = generatePlot("Bremsstrahlung", brems, 100);
    hbrems->SetFillColor(kRed);
    hbrems->SetFillStyle(3001);

    TH1D* hinelastic = generatePlot("Inelastic", inelastic, 10000);
    hinelastic->SetFillColor(kBlue);
    hinelastic->SetFillStyle(3001);

    hs->Add(hinelastic);
    hs->Add(hbrems);

    // create canvas and draw histogram
    TCanvas* canvas = new TCanvas("canvas", "canvas", 700, 700);
    canvas->SetLogy();
    // canvas->SetLogx();
    canvas->SetGrid();

    // hinelastic->GetXaxis()->SetTitle("Energy (MeV)");
    // hinelastic->GetYaxis()->SetTitle("dN/dE");
    // hinelastic->GetXaxis()->CenterTitle();
    // hinelastic->GetYaxis()->CenterTitle();
    hs->SetMinimum(0.00000000001);
    hs->SetMaximum(0.01);
    hs->Draw();

    TLegend* legend = new TLegend(.75,.80,.95,.95);
    legend->AddEntry(hbrems, "Bremsstrahlung","f");
    legend->AddEntry(hinelastic, "Inelastic","f");
    legend->Draw();
}

TH1D* generatePlot(char* name, TFile* filename, double biasFactor) {
    TTree* Hits_Info = (TTree*)filename->Get("Hits_Info");

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

    // Initialize histograms
    int nEnergyMax = 2000; // MeV
    int nEnergyBins = nEnergyMax/binSize;

    TH1D* hgammaEnergy = new TH1D(name ,                        // plot label
                                  "Gamma Energy Distribution",  // title
                                  nEnergyBins,                  // x number of bins
                                  0,                            // x lower bound
                                  nEnergyMax);                  // x upper bound

    //FOR LOG-LOG SCALE
    // Double_t xbins[201];
    // for (int i = 0; i <= 200; i++) {
    //     xbins[i] = exp(i* 0.038);
    // }

    // TH1D* hgammaEnergy = new TH1D(name,
    //                               "Gamma energy",
    //                               200,
    //                               xbins);

    // go through all entries and fill the histograms
    Int_t nentries = (Int_t)Hits_Info->GetEntries();
    for (Int_t i=0;i<nentries;i++) {
        Hits_Info->GetEntry(i);

        if (pID == 22) { // gamma
            hgammaEnergy->Fill(energy);
        }
    }

    hgammaEnergy->Scale(1/(binSize * neplus * deltaOmega * biasFactor));

    return hgammaEnergy;
};
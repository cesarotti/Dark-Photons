#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TLegend.h"

#include "math.h"

void standard() {

    TFile* file = new TFile("final.root");
    TTree* Hits_Info = (TTree*)file->Get("Hits_Info");

    Int_t pID;
    Double_t energy;
    Double_t theta;

    Hits_Info->SetBranchAddress("PDGID", &pID);
    Hits_Info->SetBranchAddress("Energy", &energy);
    Hits_Info->SetBranchAddress("theta", &theta);

    // Numbers
    // double deltaOmega = 0.07;
    int neplus = Hits_Info->GetEntries();
    // double energyBinSize = 10; // MeV
    // double thetaBinSize = .25; // degrees
    double biasFactor = 10000;
    int eplusPerSec = 6000000000;
    double scaleFactor = eplusPerSec/(neplus*biasFactor);

    double pi = 3.14159265359;
    double me = 0.510998928; // MeV

    // Initialize histograms
    double nEnergyMin = 10; // MeV
    double nEnergyMax = 510; // MeV
    int nEnergyBins = 100;
    double dEnergyBinSize = (double)(nEnergyMax - nEnergyMin) / (double)nEnergyBins;

    double nThetaMin = 2; // degrees
    double nThetaMax = 5; // degrees
    int nThetaBins = 50;
    double dThetaBinSize = (nThetaMax - nThetaMin) / nThetaBins;

    double nM2Min = -5000; // MeV
    double nM2Max = 5000; // MeV
    int nM2Bins = 100;
    double dM2BinSize = (nM2Max - nM2Min)/nM2Bins;

    TH1D* hgammaEnergy = new TH1D("Energy" ,                    // plot label
                                  "Gamma Energy Distribution",  // title
                                  nEnergyBins,                  // x number of bins
                                  nEnergyMin,                   // x lower bound
                                  nEnergyMax);                  // x upper bound

    TH1D* hgammaTheta = new TH1D("#theta" ,                      // plot label
                                 "Gamma #theta Distribution",    // title
                                 nThetaBins,                    // x number of bins
                                 nThetaMin,                     // x lower bound
                                 nThetaMax);                    // x upper bound

    TH1D* hm2 = new TH1D("M_{A'}^{2}" ,             // plot label
                         "Missing Mass Squared",    // title
                         nM2Bins,                   // x number of bins
                         nM2Min,                    // x lower bound
                         nM2Max);                   // x upper bound

    TH2F* henergytheta = new TH2F("scatter",            // plot label
                                  "#theta vs Energy",   // title
                                  nThetaBins,           // x num of bins
                                  nThetaMin,            // x min
                                  nThetaMax,            // x max
                                  nEnergyBins,          // y num of bins
                                  nEnergyMin,           // y min
                                  nEnergyMax);          // y max

    // go through all entries and fill the histograms

    for (Int_t i=0;i<neplus;i++) {
        Hits_Info->GetEntry(i);

        if (pID == 22 && theta>2 && theta<5) { // && energy>10 && energy<510) { // gamma
            hgammaEnergy->Fill(energy,scaleFactor / dEnergyBinSize);
            hgammaTheta->Fill(theta, scaleFactor/(2*pi * sin(theta*pi/180) * dThetaBinSize));
            henergytheta->Fill(theta, energy);
            hm2->Fill(2*me*(5000 - energy*(1 + (5000/(2*me))*pow(theta*pi/180,2))), scaleFactor / dM2BinSize);
        }
    }

    // create canvas and draw histogram
    TCanvas* canvas = new TCanvas("canvas", "canvas", 700, 700);
    canvas->SetCanvasSize(2000, 1000);
    canvas->Divide(3,2);
    TPad* p;

    p = (TPad*)canvas->cd(2);
    p->SetLogy();
    p->SetGrid();
    hgammaEnergy->SetFillColor(kBlue);
    hgammaEnergy->SetFillStyle(3001);
    hgammaEnergy->Draw();
    hgammaEnergy->GetXaxis()->SetTitle("Energy (MeV)");
    hgammaEnergy->GetYaxis()->SetTitle("Photons per MeV per Second (MeV^{-1} s^{-1})");
    hgammaEnergy->GetXaxis()->CenterTitle();
    hgammaEnergy->GetYaxis()->CenterTitle();

    p = (TPad*)canvas->cd(5);
    p->SetLogy();
    p->SetGrid();
    hgammaTheta->SetFillColor(kBlue);
    hgammaTheta->SetFillStyle(3001);
    hgammaTheta->Draw();
    hgammaTheta->GetXaxis()->SetTitle("#theta (degrees)");
    hgammaTheta->GetXaxis()->CenterTitle();
    hgammaTheta->GetYaxis()->SetTitle("Photons per #Omega per Second (sr^{-1} s^{-1})");
    hgammaTheta->GetYaxis()->CenterTitle();

    p = (TPad*)canvas->cd(1);
    p->SetGrid();
    hm2->SetFillColor(kBlue);
    hm2->SetFillStyle(3001);
    hm2->GetXaxis()->SetTitle("M_{A'}^{2} (MeV^{2})");
    hm2->GetYaxis()->SetTitle("Photons per MeV^{2} per Second (MeV^{-2} s^{-1})");
    hm2->GetXaxis()->CenterTitle();
    hm2->GetYaxis()->CenterTitle();
    hm2->Draw();


    p = (TPad*)canvas->cd(4);
    p->SetLogy();
    p->SetGrid();
    hm2->Draw();

    p = (TPad*)canvas->cd(3);
    // Hits_Info->Draw("theta:Energy", "PDGID==22 && theta>2 && theta<5 && Energy>10 && Energy<510");
    henergytheta->Draw();
    henergytheta->GetYaxis()->SetTitle("Energy (MeV)");
    henergytheta->GetXaxis()->SetTitle("#theta (degrees)");
    henergytheta->GetXaxis()->CenterTitle();
    henergytheta->GetYaxis()->CenterTitle();
}

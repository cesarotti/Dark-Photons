#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TH2.h"
#include <iostream>
#include "TCanvas.h"



const double POSITRON_ENERGY = 7200.; //MeV
const double ELECTRON_MASS = .511; //MeV/c^2
const double PROTON_MASS = 940; // MeV
const double GAMMA_PLUS = POSITRON_ENERGY/ELECTRON_MASS;
const double GAMMA_CM_2 = (GAMMA_PLUS+1)/2;


/*UPDATE TO YOUR SIMULATION */

const double NUM_TOT_POSITRONS = 1e+07; 
const double BIAS = 1e+04;
const double POSITRONS_PER_SEC = 6e+09; 
const double BINNING_WEIGHT = POSITRONS_PER_SEC/(BIAS*NUM_TOT_POSITRONS);
const double B = pow(1-pow(GAMMA_PLUS, -2.), .5);

// NOTE: Theta is in rad
// NOTE: Return value is in MeV^2
double QSquared(double energy, double theta) {
  return -4*POSITRON_ENERGY*energy*pow(TMath::Sin(theta/2), 2);
}

double WSquared(double energy, double theta) {
  return pow(PROTON_MASS, 2) + 2*PROTON_MASS*(POSITRON_ENERGY - energy) + QSquared(energy, theta);
}


void validation2() {

  double theta, energy = 0.;  // NOTE: theta comes in degrees
  int prot, piplus, piminus = 0;

  TFile* file = new TFile("complete.root");

  TTree* Hits_Info = (TTree *)file->Get("Signal");

  Hits_Info->SetBranchAddress("eEnergy", &energy);
  Hits_Info->SetBranchAddress("eTheta", &theta);
  Hits_Info->SetBranchAddress("numProtons", &prot);
  Hits_Info->SetBranchAddress("numPiPlus", &piplus);
  Hits_Info->SetBranchAddress("numPiMinus", &piminus);

  TH1F* hist1 = new TH1F("0.3 < Q^{2} < 0.4 GeV^{2}",
                        "0.3 < Q^{2} < 0.4 GeV^{2}",
                        8,
                        -.5,
                        7.5);

  TH1F* hist2 = new TH1F("0.4 < Q^{2} < 0.5 GeV^{2}",
                        "0.4 < Q^{2} < 0.5 GeV^{2}",
                        8,
                        -.5,
                        7.5);

  TH1F* hist3 = new TH1F("0.5 < Q^{2} < 0.6 GeV^{2}",
                        "0.5 < Q^{2} < 0.6 GeV^{2}",
                        8,
                        -.5,
                        7.5);

  TH1F* hist4 = new TH1F("0.6 < Q^{2} < 0.7 GeV^{2}",
                        "0.6 < Q^{2} < 0.7 GeV^{2}",
                        8,
                        -.5,
                        7.5);

  TH1F* hist5 = new TH1F("0.7 < Q^{2} < 1.0 GeV^{2}",
                        "0.7 < Q^{2} < 1.0 GeV^{2}",
                        8,
                        -.5,
                        7.5);

  TH1F* hist6 = new TH1F("1.0 < Q^{2} < 1.4 GeV^{2}",
                        "1.0 < Q^{2} < 1.4 GeV^{2}",
                        8,
                        -.5,
                        7.5);

  // go through all entries and fill the histograms
  int nentries = Hits_Info->GetEntries();
  Double_t w, q2;

  for (int i=0; i<nentries; i++) {
    Hits_Info->GetEntry(i);

    w = TMath::Sqrt(WSquared(energy, theta*TMath::Pi()/180)/pow(1000, 2)); // GeV
    q2 = -QSquared(energy, theta*TMath::Pi()/180)/pow(1000, 2); // GeV

    if (1.5 < w && w < 1.8) {
      if (0.3 < q2 && q2 < 0.4) {
        hist1->Fill(prot + piplus + piminus);
      } else if (0.4 < q2 && q2 < 0.5) {
        hist2->Fill(prot + piplus + piminus);
      } else if (0.5 < q2 && q2 < 0.6) {
        hist3->Fill(prot + piplus + piminus);
      } else if (0.6 < q2 && q2 < 0.7) {
        hist4->Fill(prot + piplus + piminus);
      } else if (0.7 < q2 && q2 < 1) {
        hist5->Fill(prot + piplus + piminus);
      } else if (1 < q2 && q2 < 1.4) {
        hist6->Fill(prot + piplus + piminus);
      }
    }
  }

  // create canvas and draw histogram
  TCanvas* canvas = new TCanvas("canvas", "canvas", 700, 700);
  canvas->Divide(3, 2);
  TVirtualPad* p;

  p = canvas->cd(1);
  p->SetGrid();
  p->SetLogy();
  hist1->Draw();
  hist1->GetYaxis()->SetTitle("n");
  hist1->GetXaxis()->SetTitle("Number of Prongs");
  hist1->GetXaxis()->CenterTitle();
  hist1->GetYaxis()->CenterTitle();
  hist1->SetFillColor(kBlue);
  hist1->SetFillStyle(3001);

  p = canvas->cd(2);
  p->SetGrid();
  p->SetLogy();
  hist2->Draw();
  hist2->GetYaxis()->SetTitle("n");
  hist2->GetXaxis()->SetTitle("Number of Prongs");
  hist2->GetXaxis()->CenterTitle();
  hist2->GetYaxis()->CenterTitle();
  hist2->SetFillColor(kBlue);
  hist2->SetFillStyle(3001);

  p = canvas->cd(3);
  p->SetGrid();
  p->SetLogy();
  hist3->Draw();
  hist3->GetYaxis()->SetTitle("n");
  hist3->GetXaxis()->SetTitle("Number of Prongs");
  hist3->GetXaxis()->CenterTitle();
  hist3->GetYaxis()->CenterTitle();
  hist3->SetFillColor(kBlue);
  hist3->SetFillStyle(3001);

  p = canvas->cd(4);
  p->SetGrid();
  p->SetLogy();
  hist4->Draw();
  hist4->GetYaxis()->SetTitle("n");
  hist4->GetXaxis()->SetTitle("Number of Prongs");
  hist4->GetXaxis()->CenterTitle();
  hist4->GetYaxis()->CenterTitle();
  hist4->SetFillColor(kBlue);
  hist4->SetFillStyle(3001);

  p = canvas->cd(5);
  p->SetGrid();
  p->SetLogy();
  hist5->Draw();
  hist5->GetYaxis()->SetTitle("n");
  hist5->GetXaxis()->SetTitle("Number of Prongs");
  hist5->GetXaxis()->CenterTitle();
  hist5->GetYaxis()->CenterTitle();
  hist5->SetFillColor(kBlue);
  hist5->SetFillStyle(3001);

  p = canvas->cd(6);
  p->SetGrid();
  p->SetLogy();
  hist6->Draw();
  hist6->GetYaxis()->SetTitle("n");
  hist6->GetXaxis()->SetTitle("Number of Prongs");
  hist6->GetXaxis()->CenterTitle();
  hist6->GetYaxis()->CenterTitle();
  hist6->SetFillColor(kBlue);
  hist6->SetFillStyle(3001);


}

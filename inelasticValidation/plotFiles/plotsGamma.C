#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream>
#include "TCanvas.h"



const double POSITRON_ENERGY = 7200.; //MeV
const double ELECTRON_MASS = .511; //MeV/c^2
const double GAMMA_PLUS = POSITRON_ENERGY/ELECTRON_MASS;
const double GAMMA_CM_2 = (GAMMA_PLUS+1)/2;


/*UPDATE TO YOUR SIMULATION */

const double NUM_TOT_POSITRONS = 1e+07; 
const double BIAS = 1e+04;
const double POSITRONS_PER_SEC = 6e+09; 
const double BINNING_WEIGHT = POSITRONS_PER_SEC/(BIAS*NUM_TOT_POSITRONS);
const double B = pow(1-pow(GAMMA_PLUS, -2.), .5);



//Energy in MeV, theta in radians
double mSquared(double energy, double theta)
{
  return 2*ELECTRON_MASS*(ELECTRON_MASS+POSITRON_ENERGY-
     energy*(2*GAMMA_CM_2-B*POSITRON_ENERGY*TMath::Cos(theta)/ELECTRON_MASS));
}




void plotsGamma() {

  double theta, energy;  // NOTE: theta comes in degrees
  int id;

  TFile* file = new TFile("complete.root");

  TTree* Hits_Info = (TTree *)file->Get("Signal");

  Hits_Info->SetBranchAddress("PDGID", &id);
  Hits_Info->SetBranchAddress("Energy", &energy);
  Hits_Info->SetBranchAddress("Theta", &theta);

  // Initialize histograms
  double nEnergyMin = 10; // MeV
  double nEnergyMax = 510; // MeV
  int nEnergyBins = 50;
  double dEnergyBinSize = (nEnergyMax - nEnergyMin) / nEnergyBins;

  double nThetaMin = 0; // rad
  double nThetaMax = 3.2; // rad
  int nThetaBins = 50;
  double dThetaBinSize = (nThetaMax - nThetaMin) / nThetaBins;

  double nM2Min = -7200; // MeV
  double nM2Max = 7200; // MeV
  int nM2Bins = 100;
  double dM2BinSize = (nM2Max - nM2Min) / nM2Bins;

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





  // go through all entries and fill the histograms
  int nentries = Hits_Info->GetEntries();
  for (int i=0; i<nentries; i++) {
    Hits_Info->GetEntry(i);

    if (id == 22) { // gammas only
      theta*= TMath::Pi()/180; //radians
      hgammaEnergy->Fill(energy, BINNING_WEIGHT / dEnergyBinSize);
      hgammaTheta->Fill(theta, BINNING_WEIGHT / (dThetaBinSize * TMath::Sin(theta)));
      hm2->Fill(mSquared(energy, theta), BINNING_WEIGHT / dM2BinSize);
    }
  }




  // create canvas and draw histogram
  TCanvas* canvas = new TCanvas("canvas", "canvas", 700, 700);
  canvas->Divide(2,2);
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

  p = (TPad*)canvas->cd(4);
  p->SetLogy();
  p->SetGrid();
  hgammaTheta->SetFillColor(kBlue);
  hgammaTheta->SetFillStyle(3001);
  hgammaTheta->Draw();
  hgammaTheta->GetXaxis()->SetTitle("#theta (rad)");
  hgammaTheta->GetXaxis()->CenterTitle();
  hgammaTheta->GetYaxis()->SetTitle("Photons per Steradian per Second (str^{-1} s^{-1})");
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

  p = (TPad*)canvas->cd(3);
  p->SetLogy();
  p->SetGrid();
  hm2->Draw();


}

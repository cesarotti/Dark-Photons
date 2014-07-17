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


void QWplotsValid() {
  double theta, energy = 0.;  // NOTE: theta comes in degrees
  int prot, piplus, piminus = 0;

  TFile* file = new TFile("complete.root");

  TTree* Hits_Info = (TTree *)file->Get("Signal");

  Hits_Info->SetBranchAddress("eEnergy", &energy);
  Hits_Info->SetBranchAddress("eTheta", &theta);
  Hits_Info->SetBranchAddress("numProtons", &prot);
  Hits_Info->SetBranchAddress("numPiPlus", &piplus);
  Hits_Info->SetBranchAddress("numPiMinus", &piminus);

  TH2F* hqw = new TH2F("scatter",            // plot label
                       "Q^{2} vs W^{2}",   // title
                        1000,           // x num of bins
                        -25,            // x min
                        0,            // x max
                        1000,          // y num of bins
                        0,           // y min
                        25);          // y max





  // go through all entries and fill the histograms
  int nentries = Hits_Info->GetEntries();
  for (int i=0; i<nentries; i++) {
    Hits_Info->GetEntry(i);

    hqw->Fill(QSquared(energy, theta*TMath::Pi()/180) / pow(1000, 2), WSquared(energy, theta*TMath::Pi()/180) / pow(1000, 2));

  }




  // create canvas and draw histogram
  TCanvas* canvas = new TCanvas("canvas", "canvas", 700, 700);
  canvas->SetGrid();

  hqw->Draw();
  hqw->GetYaxis()->SetTitle("W^{2} (GeV^{2})");
  hqw->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
  hqw->GetXaxis()->CenterTitle();
  hqw->GetYaxis()->CenterTitle();

}

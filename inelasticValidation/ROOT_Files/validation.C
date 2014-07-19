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


void validation() {

  double theta, energy = 0.;  // NOTE: theta comes in degrees
  int prot, piplus, piminus = 0;

  TFile* file = new TFile("complete.root");

  TTree* Hits_Info = (TTree *)file->Get("Signal");

  Hits_Info->SetBranchAddress("eEnergy", &energy);
  Hits_Info->SetBranchAddress("eTheta", &theta);
  Hits_Info->SetBranchAddress("numProtons", &prot);
  Hits_Info->SetBranchAddress("numPiPlus", &piplus);
  Hits_Info->SetBranchAddress("numPiMinus", &piminus);

  const int nBins = 6;
  double Edges[nBins + 1] = {0.3, 0.4, 0.5, 0.6, 0.7, 1.0, 1.4};
  int num[nBins] = {0, 0, 0, 0, 0, 0};

  TH1F* hist = new TH1F("multiplicities",
                        "n",
                        7,
                        0,
                        6);

  // go through all entries and fill the histograms
  int nentries = Hits_Info->GetEntries();
  Double_t w, q2;

  for (int i=0; i<nentries; i++) {
    Hits_Info->GetEntry(i);

    w = TMath::Sqrt(WSquared(energy, theta*TMath::Pi()/180)/pow(1000, 2)); // GeV
    q2 = -QSquared(energy, theta*TMath::Pi()/180)/pow(1000, 2); // GeV

    if (1.3 < w && w < 1.5 && 0.3 < q2 && q2 < 1.4) {
      hist->Fill(q2, prot + piplus + piminus);
    }

    if (0.3 < q2 && q2 < 0.4) {
      num[0]++;
    } else if (0.4 < q2 && q2 < 0.5) {
      num[1]++;
    } else if (0.5 < q2 && q2 < 0.6) {
      num[2]++;
    } else if (0.6 < q2 && q2 < 0.7) {
      num[3]++;
    } else if (0.7 < q2 && q2 < 1.0) {
      num[4]++;
    } else if (1.0 < q2 && q2 < 1.4) {
      num[5]++;
    }

  }

  int n;
  for (int i = 0; i < nBins; i++) {
    n = hist->GetBinContent(i);
    hist->SetBinContent(i, n/num[i]);
  }

  // create canvas and draw histogram
  TCanvas* canvas = new TCanvas("canvas", "canvas", 700, 700);
  canvas->SetGrid();

  hist->Draw();
  hist->GetYaxis()->SetTitle("W^{2} (GeV^{2})");
  hist->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
  hist->GetXaxis()->CenterTitle();
  hist->GetYaxis()->CenterTitle();

}

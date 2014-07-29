#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TH2.h"
#include <iostream>
#include "TCanvas.h"

const double data [4][6] = {
  {1.49, 1.44, 1.37, 1.43, 1.39, 1.41},
  {1.69, 1.72, 1.76, 1.72, 1.67, 1.75},
  {2.06, 2.05, 2.07, 2.14, 2.05, 1.96},
  {2.42, 2.48, 2.42, 2.37, 2.50, 2.48},
};

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

int WtoInt(double w) {
  if (w < 1.3) {return -1;}
  else if (w < 1.5) {return 0;}
  else if (w < 1.8) {return 1;}
  else if (w < 2.2) {return 2;}
  else if (w < 2.8) {return 3;}
  else {return -1;}
}

int Q2toInt(double q2) {
  if (q2 < 0.3) {return -1;}
  else if (q2 < 0.4) {return 0;}
  else if (q2 < 0.5) {return 1;}
  else if (q2 < 0.6) {return 2;}
  else if (q2 < 0.7) {return 3;}
  else if (q2 < 1.0) {return 4;}
  else if (q2 < 1.4) {return 5;}
  else {return -1;}
}

void plotsVal2() {

  double theta, energy = 0.;  // NOTE: theta comes in degrees
  int prot, piplus, piminus = 0;

  // initialize counters to zero
  int count [4][6];
  double sum [4][6];
  for (int i=0; i < 4; i++) {
    for (int j=0; j < 6; j++) {
      count[i][j] = 0;
      sum[i][j] = 0;
    }
  }

  TFile* file = new TFile("complete.root");

  TTree* Hits_Info = (TTree *)file->Get("Prongs");

  Hits_Info->SetBranchAddress("eEnergy", &energy);
  Hits_Info->SetBranchAddress("eTheta", &theta);
  Hits_Info->SetBranchAddress("numProtons", &prot);
  Hits_Info->SetBranchAddress("numPiPlus", &piplus);
  Hits_Info->SetBranchAddress("numPiMinus", &piminus);


  // go through all entries and fill the histograms
  int nentries = Hits_Info->GetEntries();
  int a, b;

  for (int i=0; i<nentries; i++) {
    Hits_Info->GetEntry(i);

    a = WtoInt(TMath::Sqrt(WSquared(energy, theta*TMath::Pi()/180)/pow(1000, 2))); // GeV
    b = Q2toInt(-QSquared(energy, theta*TMath::Pi()/180)/pow(1000, 2)); // GeV

    if (a >= 0 && b >= 0) {
      count[a][b]++;
      sum[a][b]+= prot + piplus + piminus;
    }
  }

  for (int j=0; j<4; j++) {
    for (int i=0; i<6; i++) {
      std::cout << sum[j][i] / count[j][i] << '\t';
    }
    std::cout << '\n';
  }

}

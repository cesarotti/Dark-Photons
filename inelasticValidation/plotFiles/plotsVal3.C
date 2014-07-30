#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TGraph.h"
#include "TH2.h"
#include <iostream>
#include "TCanvas.h"

const double dataDESY [4][6] = {
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

void plotsVal3() {

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


  // go through all entries
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

  // calculate data values
  double dataGeant[4][6];
  for (int i=0; i<4; i++) {
    for (int j=0; j<6; j++) {
      dataGeant[i][j] = sum[i][j] / count[i][j];
    }
  }

  // Draw graphs
  TCanvas* c1 = new TCanvas("c1","Average Number of Charged Hadrons by W and Q^{2}",200,10,700,500);
  c1->Divide(2, 2);
  TVirtualPad* p;
  TLegend* leg;
  TH1D *h1, *h2;
  string legHead;

  double array[6] = {1, 2, 3, 4, 5, 6};
  string wbounds[5] = {"1.3", "1.5", "1.8", "2.2", "2.8"};
  double q2bounds[7] = {0.3, 0.4, 0.5, 0.6, 0.7, 1.0, 1.4};

  for (int i=0; i<4; i++) {
    p = c1->cd(i+1);
    p->SetGrid();

    legHead = wbounds[i] + " GeV < W < " + wbounds[i+1] + " GeV";

    h1 = new TH1D("h1" + i, legHead.c_str(), 6, q2bounds);
    h2 = new TH1D("h2" + i, legHead.c_str(), 6, q2bounds);

    for (int j=0; j<6; j++) {
      h1->SetBinContent(j+1, dataGeant[i][j]);
      h2->SetBinContent(j+1, dataDESY[i][j]);
    }

    h1->Draw();
    h1->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    h1->GetXaxis()->CenterTitle();
    h1->GetYaxis()->SetTitle("n_{ch}");
    h1->GetYaxis()->CenterTitle();
    h1->SetMinimum(0);
    h1->SetMaximum(4);
    h1->SetStats(0);
    h1->SetFillStyle(3005);
    h1->SetFillColor(2);

    h2->Draw("same");
    h2->SetFillStyle(3004);
    h2->SetFillColor(4);
    h2->SetStats(0);

    leg = new TLegend(0.6,0.7,0.85,0.85);
    leg->AddEntry(h1, "Data from Geant4", "f");
    leg->AddEntry(h2, "Data from DESY", "f");
    leg->Draw();
  }

}

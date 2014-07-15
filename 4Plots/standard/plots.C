#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream>
#include "TCanvas.h"

const double POSITRON_ENERGY = 5000.; //MeV
const double ELECTRON_MASS = .511; //MeV/c^2
const double GAMMA_CM_2 = POSITRON_ENERGY/(2*ELECTRON_MASS);

const double NUM_TOT_POSITRONS = 1e+07; 
const double BIAS = 1e+03;
const double POSITRONS_PER_SEC = 6e+09; 
const double BINNING_WEIGHT = POSITRONS_PER_SEC/(BIAS*NUM_TOT_POSITRONS);

//Energy in MeV, theta in radians
double mSquared(double energy, double theta)
{
  return (2*ELECTRON_MASS*(POSITRON_ENERGY-energy*(1+GAMMA_CM_2*pow(theta, 2.))));
}

void plots() {

  double yPos, xPos, theta, energyTot = 0.;
  int numHit, id = 0;

  TFile* file = new TFile("complete.root");

  TTree* tree = (TTree *)file->Get("Signal");

  int nEvents = tree->GetEntries(); 

  tree->SetBranchAddress("numHits", &numHit);
  tree->SetBranchAddress("energyTot", &energyTot);
  tree->SetBranchAddress("XPosition", &xPos);
  tree->SetBranchAddress("YPosition", &yPos);
  tree->SetBranchAddress("Particle_ID", &id);
  tree->SetBranchAddress("Theta", &theta);

  TH2D* engTheta = new TH2D("engTheta", "energy_v_theta", 50, .035, .085, 50, 10, 510);

  TH1D* energyH = new TH1D("energyH", "Energy", 100, 10, 510);

  TH1D* thetaH = new TH1D("thetaH", "Theta", 50, .035, .085);

  TH1D* M2 = new TH1D("M2", "M^2", 100, -5000, 5000);

  for (int i=0; i<nEvents; i++)
    {
      tree->GetEntry(i);
      theta*=TMath::Pi()/180;
      engTheta->Fill(theta, energyTot); 
      energyH->Fill(energyTot, BINNING_WEIGHT/10.); //events per sec per  MeV
      thetaH->Fill(theta, BINNING_WEIGHT); //events per sec per mrad
      M2->Fill(mSquared(energyTot, theta), BINNING_WEIGHT/10); 
    }

  TCanvas* can1 = new TCanvas("can1", "Energy_Mass_and_Theta", 1000, 500);
  TCanvas* can2 = new TCanvas("can3", "Energy", 500, 500);
  TCanvas* can3 = new TCanvas("can4", "Mass", 
  can1->Divide(2,2); 
  can1->cd(1); engTheta->Draw();
  can1->cd(3); energyH->Draw();
  can1->cd(4); thetaH->Draw();
  can1->cd(2); M2->Draw();
  can2->SetLogy(); energyH->Draw();
  can3->SetLogy(); energyH->Draw();


  



}

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

double mSquared(double energy, double theta)
{
  double thetaRad = theta*TMath::Pi()/180.; 
  return (2*ELECTRON_MASS*(POSITRON_ENERGY-energy*(1+GAMMA_CM_2*pow(thetaRad, 2.))));
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

  TH2D* engTheta = new TH2D("engTheta", "energy_v_theta", 50, 2, 5, 50, 10, 510);

  TH1D* energyH = new TH1D("energyH", "Energy", 100, 10, 510);

  TH1D* thetaH = new TH1D("thetaH", "Theta", 50, 2, 5);

  TH1D* M2 = new TH1D("M2", "M^2", 100, -5000, 5000);

  for (int i=0; i<nEvents; i++)
    {
      tree->GetEntry(i);
      engTheta->Fill(theta, energyTot ); 
      energyH->Fill(energyTot);
      thetaH->Fill(theta);
      M2->Fill(mSquared(energyTot, theta));
    }

  TCanvas* can1 = new TCanvas("can1", "Energy_and_Theta", 1000, 500);
  TCanvas* can2 = new TCanvas("can2", "Mass", 1000, 500);
  can1->Divide(2,2); 
  can1->cd(1); engTheta->DrawCopy();
  can1->cd(3); energyH->DrawCopy();
  can1->cd(4); thetaH->DrawCopy();

  can2->cd(); M2->DrawCopy();

  



}

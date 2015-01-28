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
<<<<<<< HEAD
const double BIAS = 1;//1e+03;
=======
const double BIAS = 1e+03;
>>>>>>> 911b5ec345c81b054214c521136984e9d6306827
const double POSITRONS_PER_SEC = 6e+09; 
const double BINNING_WEIGHT = POSITRONS_PER_SEC/(BIAS*NUM_TOT_POSITRONS);

//Energy in MeV, theta in radians
double mSquared(double energy, double theta)
{
  double labEnergy = POSITRON_ENERGY+ELECTRON_MASS;
  double posEnergy2 = pow(POSITRON_ENERGY, 2.);
  double momentum = sqrt(posEnergy2+pow(ELECTRON_MASS,2.));
  return (labEnergy+ELECTRON_MASS)*(labEnergy-2*energy) - 
    momentum*(momentum-2*energy*TMath::Cos(theta)); 
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


  // create canvas and draw histogram
  TCanvas* canvas = new TCanvas("canvas", "canvas", 700, 700);
  //canvas->SetCanvasSize(2000, 1000);
  canvas->Divide(3,2);
  TPad* p; 

  p = (TPad*)canvas->cd(2);
  p->SetLogy();
  p->SetGrid();
  energyH->SetFillColor(kBlue);
  energyH->SetFillStyle(3001);
  energyH->Draw();
  energyH->GetXaxis()->SetTitle("Energy (MeV)");
  energyH->GetYaxis()->SetTitle("Photons per MeV per Second (MeV^{-1} s^{-1})");
  energyH->GetXaxis()->CenterTitle();
  energyH->GetYaxis()->CenterTitle();

  p = (TPad*)canvas->cd(5);
  p->SetLogy();
  p->SetGrid();
  thetaH->SetFillColor(kBlue);
  thetaH->SetFillStyle(3001);
  thetaH->Draw();
  thetaH->GetXaxis()->SetTitle("#theta (mrad)");
  thetaH->GetXaxis()->CenterTitle();
  thetaH->GetYaxis()->SetTitle("Photons per mrad per Second (mrad^{-1} s^{-1})");
  thetaH->GetYaxis()->CenterTitle();

  p = (TPad*)canvas->cd(1);
  p->SetGrid();
  M2->SetFillColor(kBlue);
  M2->SetFillStyle(3001);
  M2->GetXaxis()->SetTitle("M_{A'}^{2} (MeV^{2})");
  M2->GetYaxis()->SetTitle("Photons per MeV^{2} per Second (MeV^{-2} s^{-1})");
  M2->GetXaxis()->CenterTitle();
  M2->GetYaxis()->CenterTitle();
  M2->Draw();


  p = (TPad*)canvas->cd(4);
  p->SetLogy();
  p->SetGrid();
  M2->Draw();

  p = (TPad*)canvas->cd(3);
  // Hits_Info->Draw("theta:Energy", "PDGID==22 && theta>2 && theta<5 && Energy>10 && Energy<510");
  engTheta->Draw();
  engTheta->GetYaxis()->SetTitle("Energy (MeV)");
  engTheta->GetXaxis()->SetTitle("#theta (mrad)");
  engTheta->GetXaxis()->CenterTitle();
  engTheta->GetYaxis()->CenterTitle();

  



}

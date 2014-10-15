#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "THStack.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream>
#include "TCanvas.h"



const double POSITRON_ENERGY = 4975.; //MeV
const double ELECTRON_MASS = .511; //MeV/c^2
const double GAMMA_PLUS = POSITRON_ENERGY/ELECTRON_MASS;
const double GAMMA_CM_2 = (GAMMA_PLUS+1)/2;


/*UPDATE TO YOUR SIMULATION */

const double NUM_TOT_POSITRONS = 100000; 
const double BIAS = 1e+06;
const double XSECyy = 145e-28;
const double XSECyyy = 28e-28;
const double POSITRONS_PER_SEC = 6e+09; 
const double BINNING_WEIGHT = POSITRONS_PER_SEC/(BIAS*NUM_TOT_POSITRONS);
const double B = pow(1-pow(GAMMA_PLUS, -2.), .5);



//Energy in MeV, theta in radians
double mSquared(double energy, double theta)
{
  return 2*ELECTRON_MASS*(ELECTRON_MASS+POSITRON_ENERGY-
     energy*(2*GAMMA_CM_2-B*POSITRON_ENERGY*TMath::Cos(theta)/ELECTRON_MASS));
}




void msquared() {

  double yPos, xPos, theta, energy = 0.;  // NOTE: theta comes in degrees
  int numHit, pID = 0;


  // Initialize histograms
  double nEnergyMin = 10; // MeV
  double nEnergyMax = 510; // MeV
  int nEnergyBins = 50;
  double dEnergyBinSize = (double)(nEnergyMax - nEnergyMin) / (double)nEnergyBins;

  double nThetaMin = 34; // mrad
  double nThetaMax = 88; // mrad
  int nThetaBins = 50;
  double dThetaBinSize = (nThetaMax - nThetaMin) / nThetaBins;

  double nM2Min = -5000; // MeV
  double nM2Max = 5000; // MeV
  int nM2Bins = 100;
  double dM2BinSize = (nM2Max - nM2Min)/nM2Bins;


  THStack *hs = new THStack("hs","Stacked M^2");

  TFile* file = new TFile("e+e-2yyGUN.root");

  TTree* Hits_Info = (TTree *)file->Get("Signal");

  Hits_Info->SetBranchAddress("numHits", &numHit);
  Hits_Info->SetBranchAddress("energyTot", &energy);
  Hits_Info->SetBranchAddress("XPosition", &xPos);
  Hits_Info->SetBranchAddress("YPosition", &yPos);
  Hits_Info->SetBranchAddress("Particle_ID", &pID);
  Hits_Info->SetBranchAddress("Theta", &theta);

  TH1D* hmyy = new TH1D("M_{A'}^{2}" ,             // plot label
                       "e+e- > yy",    // title
                       nM2Bins,                   // x number of bins
                       nM2Min,                    // x lower bound
                       nM2Max);                   // x upper bound

  // go through all entries and fill the histograms
  int nentries = Hits_Info->GetEntries();
  for (int i=0; i<nentries; i++) {
    Hits_Info->GetEntry(i);
    if (pID == 22) { // gammas only
      theta*= TMath::Pi()/180; //radians
      cout << "M^2 is: " << mSquared(energy, theta) << endl;
      cout << "Energy is: " << energy << endl;
           
      hmyy->Fill(mSquared(energy, theta), BINNING_WEIGHT * 145. / dM2BinSize);
    }
  }
  cout << "DM2binsize is:" << dM2BinSize << endl;




  hmyy->SetFillColor(kBlue);
  hmyy->SetFillStyle(3001);
  hmyy->GetXaxis()->SetTitle("M_{A'}^{2} (MeV^{2})");
  hmyy->GetYaxis()->SetTitle("Photons per MeV^{2} per Second (MeV^{-2} s^{-1})");
  hmyy->GetXaxis()->CenterTitle();
  hmyy->GetYaxis()->CenterTitle();
  hs->Add(hmyy);

  TFile* fileyyy = new TFile("e+e-2yyyGUN.root");
  TTree* Hits_Infoyyy = (TTree *)fileyyy->Get("Signal");

  Hits_Infoyyy->SetBranchAddress("numHits", &numHit);
  Hits_Infoyyy->SetBranchAddress("energyTot", &energy);
  Hits_Infoyyy->SetBranchAddress("XPosition", &xPos);
  Hits_Infoyyy->SetBranchAddress("YPosition", &yPos);
  Hits_Infoyyy->SetBranchAddress("Particle_ID", &pID);
  Hits_Infoyyy->SetBranchAddress("Theta", &theta);

  TH1D* hmyyy = new TH1D("M_{A'}^{2}" ,             // plot label
                       "e+e- > yyy",    // title
                       nM2Bins,                   // x number of bins
                       nM2Min,                    // x lower bound
                       nM2Max);                   // x upper bound
  // go through all entries and fill the histograms
  nentries = Hits_Infoyyy->GetEntries();
  for (int i=0; i<nentries; i++) {
    Hits_Infoyyy->GetEntry(i);
    if (pID == 22) { // gammas only
      theta*= TMath::Pi()/180; //radians
      cout << "M^2 is: " << mSquared(energy, theta) << endl;
      cout << "Energy is: " << energy << endl;
           
      hmyyy->Fill(mSquared(energy, theta), BINNING_WEIGHT * 25. / dM2BinSize);
    }
  }
  cout << "DM2binsize is:" << dM2BinSize << endl;


  hmyyy->SetFillColor(kRed);
  hmyyy->SetFillStyle(3001);
  hmyyy->GetXaxis()->SetTitle("M_{A'}^{2} (MeV^{2})");
  hmyyy->GetYaxis()->SetTitle("Photons per MeV^{2} per Second (MeV^{-2} s^{-1})");
  hmyyy->GetXaxis()->CenterTitle();
  hmyyy->GetYaxis()->CenterTitle();

  hs->Add(hmyyy);


  TFile* fileepluseminus = new TFile("e+e-2e+e-GUN.root");
  TTree* Hits_Infoepluseminus = (TTree *)fileepluseminus->Get("Signal");

  Hits_Infoepluseminus->SetBranchAddress("numHits", &numHit);
  Hits_Infoepluseminus->SetBranchAddress("energyTot", &energy);
  Hits_Infoepluseminus->SetBranchAddress("XPosition", &xPos);
  Hits_Infoepluseminus->SetBranchAddress("YPosition", &yPos);
  Hits_Infoepluseminus->SetBranchAddress("Particle_ID", &pID);
  Hits_Infoepluseminus->SetBranchAddress("Theta", &theta);

  TH1D* hmepluseminus = new TH1D("M_{A'}^{2}" ,             // plot label
                       "e+e- > e+e-",    // title
                       nM2Bins,                   // x number of bins
                       nM2Min,                    // x lower bound
                       nM2Max);                   // x upper bound
  // go through all entries and fill the histograms
  nentries = Hits_Infoepluseminus->GetEntries();
  for (int i=0; i<nentries; i++) {
    Hits_Infoepluseminus->GetEntry(i);
    //if (pID == 22) { // gammas only
      theta*= TMath::Pi()/180; //radians
      cout << "M^2 is: " << mSquared(energy, theta) << endl;
      cout << "Energy is: " << energy << endl;
           
      hmepluseminus->Fill(mSquared(energy, theta), BINNING_WEIGHT * 30000. / dM2BinSize);
    //}
  }
  cout << "DM2binsize is:" << dM2BinSize << endl;


  hmepluseminus->SetFillColor(kGreen);
  hmepluseminus->SetFillStyle(3001);
  hmepluseminus->GetXaxis()->SetTitle("M_{A'}^{2} (MeV^{2})");
  hmepluseminus->GetYaxis()->SetTitle("Photons per MeV^{2} per Second (MeV^{-2} s^{-1})");
  hmepluseminus->GetXaxis()->CenterTitle();
  hmepluseminus->GetYaxis()->CenterTitle();

  hs->Add(hmepluseminus);

  // create canvas and draw histogram
  TCanvas* canvas = new TCanvas("canvas", "canvas", 700, 700);
  canvas->Divide(2,2);
  TPad* p;

  p = (TPad*)canvas->cd(1);
  p->SetGrid();

 


  hs->Draw();

  p = (TPad*)canvas->cd(4);
  hmepluseminus->Draw();
  p = (TPad*)canvas->cd(3);
  hmyyy->Draw();
  p = (TPad*)canvas->cd(2);
  hmyy->Draw();

/*
  p = (TPad*)canvas->cd(4);
  p->SetLogy();
  p->SetGrid();
  hm2->Draw();*/


}

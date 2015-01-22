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

double NUM_TOT_POSITRONS = 100000; 
double BIAS = 1e+06;
double XSECyy = 145;
double XSECyyy = 25;
double XSECepluseminusy = 8222;
double NORMAL_FACTOR = 0.3836841728489; //This is the factor for 0.5-2
double POSITRONS_PER_SEC = 6e+09; 
double BINNING_WEIGHT = NORMAL_FACTOR * POSITRONS_PER_SEC/(BIAS*NUM_TOT_POSITRONS);
double B = pow(1-pow(GAMMA_PLUS, -2.), .5);


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
           
      hmyy->Fill(mSquared(energy, theta), XSECyy * BINNING_WEIGHT / dM2BinSize);
    }
  }
  cout << "DM2binsize is:" << dM2BinSize << endl;




  hmyy->SetFillColor(kBlue);
  hmyy->SetFillStyle(3001);
  hmyy->GetXaxis()->SetTitle("M_{A'}^{2} (MeV^{2})");
  hmyy->GetYaxis()->SetTitle("Photons per MeV^{2} per Second (MeV^{-2} s^{-1})");
  hmyy->GetXaxis()->CenterTitle();
  hmyy->GetYaxis()->CenterTitle();


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
           
      hmyyy->Fill(mSquared(energy, theta), XSECyyy * BINNING_WEIGHT / dM2BinSize);
    }
  }
  cout << "DM2binsize is:" << dM2BinSize << endl;


  hmyyy->SetFillColor(kRed);
  hmyyy->SetFillStyle(3001);
  hmyyy->GetXaxis()->SetTitle("M_{A'}^{2} (MeV^{2})");
  hmyyy->GetYaxis()->SetTitle("Photons per MeV^{2} per Second (MeV^{-2} s^{-1})");
  hmyyy->GetXaxis()->CenterTitle();
  hmyyy->GetYaxis()->CenterTitle();



  
  TFile* fileepluseminusy = new TFile("e+e-2e+e-yECUT0.3NOPT_2_5_1000000.root");
  //This is 1000000 events so DOWNEIGHT IT:
  XSECepluseminusy = 0.1 * XSECepluseminusy;
  TTree* Hits_Infoepluseminusy = (TTree *)fileepluseminusy->Get("Signal");

  Hits_Infoepluseminusy->SetBranchAddress("numHits", &numHit);
  Hits_Infoepluseminusy->SetBranchAddress("energyTot", &energy);
  Hits_Infoepluseminusy->SetBranchAddress("XPosition", &xPos);
  Hits_Infoepluseminusy->SetBranchAddress("YPosition", &yPos);
  Hits_Infoepluseminusy->SetBranchAddress("Particle_ID", &pID);
  Hits_Infoepluseminusy->SetBranchAddress("Theta", &theta);

  TH1D* hmepluseminusy = new TH1D("M_{A'}^{2}" ,             // plot label
                       "e+e- > e+e-y (only registering gammas)",    // title
                       nM2Bins,                   // x number of bins
                       nM2Min,                    // x lower bound
                       nM2Max);                   // x upper bound
  // go through all entries and fill the histograms
  nentries = Hits_Infoepluseminusy->GetEntries();
  for (int i=0; i<nentries; i++) {
    Hits_Infoepluseminusy->GetEntry(i);
    if (pID == 22) { // gammas only
      theta*= TMath::Pi()/180; //radians
      cout << "M^2 is: " << mSquared(energy, theta) << endl;
      cout << "Energy is: " << energy << endl;
           
      hmepluseminusy->Fill(mSquared(energy, theta), XSECepluseminusy * BINNING_WEIGHT / dM2BinSize);
    }
  }
  cout << "DM2binsize is:" << dM2BinSize << endl;


  hmepluseminusy->SetFillColor(7);
  hmepluseminusy->SetFillStyle(3001);
  hmepluseminusy->GetXaxis()->SetTitle("M_{A'}^{2} (MeV^{2})");
  hmepluseminusy->GetYaxis()->SetTitle("Photons per MeV^{2} per Second (MeV^{-2} s^{-1})");
  hmepluseminusy->GetXaxis()->CenterTitle();
  hmepluseminusy->GetYaxis()->CenterTitle();


  

  

  //ADD YIMIN'S PLOT

  TFile *f = new TFile("YIMINPLOTBREM.root"); 

  f->ls(); 
  
  TH1F * hYIMIN = (TH1F*)f->Get("M^2"); 
  hYIMIN->SetFillColor(kOrange);
  hYIMIN->SetTitle("Bremsstrahlung");
  hYIMIN->SetFillStyle(3001);
  hYIMIN->GetXaxis()->SetTitle("M_{A'}^{2} (MeV^{2})");
  hYIMIN->GetYaxis()->SetTitle("Photons per MeV^{2} per Second (MeV^{-2} s^{-1})");
  hYIMIN->GetXaxis()->CenterTitle();
  hYIMIN->GetYaxis()->CenterTitle();




  /**********************
      ADD BRIANS PLOT
    ********************/

  NUM_TOT_POSITRONS = 1e+07; 
  BIAS = 1e+04; 
  BINNING_WEIGHT = POSITRONS_PER_SEC/(BIAS*NUM_TOT_POSITRONS);
  B = pow(1-pow(GAMMA_PLUS, -2.), .5);

  TFile* fileinelastic = new TFile("brian_1e7_pos_1e4_bias.root");
  TTree* Hits_Infoinelastic = (TTree *)fileinelastic->Get("Signal");

  Hits_Infoinelastic->SetBranchAddress("numHits", &numHit);
  Hits_Infoinelastic->SetBranchAddress("energyTot", &energy);
  Hits_Infoinelastic->SetBranchAddress("XPosition", &xPos);
  Hits_Infoinelastic->SetBranchAddress("YPosition", &yPos);
  Hits_Infoinelastic->SetBranchAddress("Particle_ID", &pID);
  Hits_Infoinelastic->SetBranchAddress("Theta", &theta);

  TH1D* hminelastic = new TH1D("M_{A'}^{2}" ,             // plot label
                       "Inelastic (only registering gammas)",    // title
                       nM2Bins,                   // x number of bins
                       nM2Min,                    // x lower bound
                       nM2Max);                   // x upper bound
  // go through all entries and fill the histograms
  nentries = Hits_Infoinelastic->GetEntries();
  for (int i=0; i<nentries; i++) {
    Hits_Infoinelastic->GetEntry(i);
    if (pID == 22) { // gammas only
      theta*= TMath::Pi()/180; //radians
      cout << "M^2 is: " << mSquared(energy, theta) << endl;
      cout << "Energy is: " << energy << endl;
           
      hminelastic->Fill(mSquared(energy, theta), BINNING_WEIGHT / dM2BinSize);
    }
  }
  cout << "DM2binsize is:" << dM2BinSize << endl;


  hminelastic->SetFillColor(kGreen);
  hminelastic->SetFillStyle(3001);
  hminelastic->GetXaxis()->SetTitle("M_{A'}^{2} (MeV^{2})");
  hminelastic->GetYaxis()->SetTitle("Photons per MeV^{2} per Second (MeV^{-2} s^{-1})");
  hminelastic->GetXaxis()->CenterTitle();
  hminelastic->GetYaxis()->CenterTitle();



 hs->Add(hminelastic);
  hs->Add(hYIMIN);
      hs->Add(hmyyy);
  hs->Add(hmepluseminusy);
    hs->Add(hmyy);



  // create canvas and draw histogram
  TCanvas* canvas = new TCanvas("canvas", "canvas", 700, 700);
  canvas->Divide(3,2);
  TPad* p;

  p = (TPad*)canvas->cd(1);
  //p->SetGrid();
  p->SetLogy();

 


  hs->Draw();

    hs->GetXaxis()->SetTitle("M_{A'}^{2} (MeV^{2})");
  hs->GetYaxis()->SetTitle("Photons per MeV^{2} per Second (MeV^{-2} s^{-1})");
  hs->GetXaxis()->CenterTitle();
  hs->GetYaxis()->CenterTitle();
  
  p = (TPad*)canvas->cd(4);
  hmepluseminusy->Draw();
  
  p = (TPad*)canvas->cd(3);
  hmyyy->Draw();
  p = (TPad*)canvas->cd(2);
  hmyy->Draw();

  p = (TPad*)canvas->cd(5);
  hYIMIN->Draw();

  p = (TPad*)canvas->cd(6);
  hminelastic->Draw();

/*
  p = (TPad*)canvas->cd(4);
  p->SetLogy();
  p->SetGrid();
  hm2->Draw();*/


}

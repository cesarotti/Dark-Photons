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

<<<<<<< HEAD:Cari's Code/standard/plots.C
const double NUM_TOT_POSITRONS = 1e+07; 
=======
const double NUM_TOT_POSITRONS = 1e+02; 
>>>>>>> b74e7dc340fd7a91328eab7294e6836cfb729b47:4Plots/plots.C
const double BIAS = 1e+03;
const double POSITRONS_PER_SEC = 6e+09; 
const double BINNING_WEIGHT = POSITRONS_PER_SEC/(BIAS*NUM_TOT_POSITRONS);





//Energy in MeV, theta in radians
double mSquared(double energy, double theta)
{
<<<<<<< HEAD:Cari's Code/standard/plots.C
  double labEnergy = POSITRON_ENERGY+ELECTRON_MASS;
  double posEnergy2 = pow(POSITRON_ENERGY, 2.);
  double momentum = sqrt(posEnergy2+pow(ELECTRON_MASS,2.));
  return (labEnergy+ELECTRON_MASS)*(labEnergy-2*energy) - 
    momentum*(momentum-2*energy*TMath::Cos(theta)); 
=======
  return 2*ELECTRON_MASS*(ELECTRON_MASS+POSITRON_ENERGY-
     energy*(2*GAMMA_CM_2-B*POSITRON_ENERGY*TMath::Cos(theta)/ELECTRON_MASS));
>>>>>>> b74e7dc340fd7a91328eab7294e6836cfb729b47:4Plots/plots.C
}




void plots() {

  double yPos, xPos, theta, energy = 0.;  // NOTE: theta comes in degrees
  int numHit, pID = 0;

  TFile* file = new TFile("complete.root");

  TTree* Hits_Info = (TTree *)file->Get("Signal");

  Hits_Info->SetBranchAddress("numHits", &numHit);
  Hits_Info->SetBranchAddress("energyTot", &energy);
  Hits_Info->SetBranchAddress("XPosition", &xPos);
  Hits_Info->SetBranchAddress("YPosition", &yPos);
  Hits_Info->SetBranchAddress("Particle_ID", &pID);
  Hits_Info->SetBranchAddress("Theta", &theta);



  // Numbers
  // check to make sure values correspond to given project
  // (i.e. bias factor is right, etc)
  int neplus = 10000000;
  double biasFactor = 10000;
  int eplusPerSec = 6000000000;
  double scaleFactor = eplusPerSec/(neplus*biasFactor);

  double pi = 3.14159265359;
  double me = 0.510998928; // MeV




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

  TH1D* hgammaEnergy = new TH1D("Energy" ,                    // plot label
                                "Gamma Energy Distribution",  // title
                                nEnergyBins,                  // x number of bins
                                nEnergyMin,                   // x lower bound
                                nEnergyMax);                  // x upper bound

<<<<<<< HEAD:Cari's Code/standard/plots.C
  for (int i=0; i<nEvents; i++)
    {
      tree->GetEntry(i);
      theta*=TMath::Pi()/180;
      engTheta->Fill(theta, energyTot); 
      energyH->Fill(energyTot, BINNING_WEIGHT/10.); //events per sec per  MeV
      thetaH->Fill(theta, BINNING_WEIGHT); //events per sec per mrad
      M2->Fill(mSquared(energyTot, theta), BINNING_WEIGHT/10); 
=======
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

  TH2F* henergytheta = new TH2F("scatter",            // plot label
                                "#theta vs Energy",   // title
                                nThetaBins,           // x num of bins
                                nThetaMin,            // x min
                                nThetaMax,            // x max
                                nEnergyBins,          // y num of bins
                                nEnergyMin,           // y min
                                nEnergyMax);          // y max





  // go through all entries and fill the histograms
  int nentries = Hits_Info->GetEntries();
  for (int i=0; i<nentries; i++) {
    Hits_Info->GetEntry(i);

    if (pID == 22) { // gammas only
      hgammaEnergy->Fill(energy,scaleFactor / dEnergyBinSize);
      hgammaTheta->Fill(theta*pi*1000/180, scaleFactor / dThetaBinSize);
      henergytheta->Fill(theta*pi*1000/180, energy);
      hm2->Fill(mSquared(energy, theta*pi/180), scaleFactor / dM2BinSize);
>>>>>>> b74e7dc340fd7a91328eab7294e6836cfb729b47:4Plots/plots.C
    }
  }




  // create canvas and draw histogram
  TCanvas* canvas = new TCanvas("canvas", "canvas", 700, 700);
  canvas->Divide(3,2);
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

  p = (TPad*)canvas->cd(5);
  p->SetLogy();
  p->SetGrid();
  hgammaTheta->SetFillColor(kBlue);
  hgammaTheta->SetFillStyle(3001);
  hgammaTheta->Draw();
  hgammaTheta->GetXaxis()->SetTitle("#theta (mrad)");
  hgammaTheta->GetXaxis()->CenterTitle();
  hgammaTheta->GetYaxis()->SetTitle("Photons per mrad per Second (mrad^{-1} s^{-1})");
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

  p = (TPad*)canvas->cd(4);
  p->SetLogy();
  p->SetGrid();
  hm2->Draw();

  p = (TPad*)canvas->cd(3);
  henergytheta->Draw();
  henergytheta->GetYaxis()->SetTitle("Energy (MeV)");
  henergytheta->GetXaxis()->SetTitle("#theta (mrad)");
  henergytheta->GetXaxis()->CenterTitle();
  henergytheta->GetYaxis()->CenterTitle();

}

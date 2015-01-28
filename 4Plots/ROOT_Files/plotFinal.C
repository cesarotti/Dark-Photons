#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream>
#include "TCanvas.h"



const double POSITRON_ENERGY = 4975.; //MeV
const double ELECTRON_MASS = .511; //MeV/c^2
const double GAMMA_PLUS = POSITRON_ENERGY/ELECTRON_MASS;
const double GAMMA_CM_2 = (GAMMA_PLUS+1)/2;


/*UPDATE TO YOUR SIMULATION */

<<<<<<< HEAD
const double NUM_TOT_POSITRONS = 100000; 
const double BIAS = 1e+06;
const double POSITRONS_PER_SEC = 6e+09; 
const double XSEC  = 8222.; // e+e-2e+e-y currently
const double NORMAL_FACTOR = 0.3836841728489; //1.8728253857;
const double BINNING_WEIGHT = XSEC * NORMAL_FACTOR * POSITRONS_PER_SEC/(BIAS*NUM_TOT_POSITRONS);
=======
const double NUM_TOT_POSITRONS = 100; 
const double BIAS = 1e+06;
const double POSITRONS_PER_SEC = 6e+09; 
const double BINNING_WEIGHT = POSITRONS_PER_SEC/(BIAS*NUM_TOT_POSITRONS);
>>>>>>> 911b5ec345c81b054214c521136984e9d6306827
const double B = pow(1-pow(GAMMA_PLUS, -2.), .5);



<<<<<<< HEAD

=======
>>>>>>> 911b5ec345c81b054214c521136984e9d6306827
//Energy in MeV, theta in radians
double mSquared(double energy, double theta)
{
  return 2*ELECTRON_MASS*(ELECTRON_MASS+POSITRON_ENERGY-
     energy*(2*GAMMA_CM_2-B*POSITRON_ENERGY*TMath::Cos(theta)/ELECTRON_MASS));
}




void plotFinal() {

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


  // Initialize histograms
  double nEnergyMin = 10; // MeV
<<<<<<< HEAD
  double nEnergyMax = 1510; // MeV
  int nEnergyBins = 50;
  double dEnergyBinSize = (double)(nEnergyMax - nEnergyMin) / (double)nEnergyBins;

  /* 0.5 to 2 
  double nThetaMin = 26.179938779906866; //1.5deg //34; // mrad
  double nThetaMax = 34.90658503987582; //2deg //88; // mrad
  */
  /* 2 to 5 */
  double nThetaMin = 34.90658503987582; //1.5deg //34; // mrad
  double nThetaMax = 87.26646259968956; //2deg //88; // mrad
  /**/
  
  int nThetaBins = 50;
  double dThetaBinSize = (nThetaMax - nThetaMin) / nThetaBins;

  double nM2Min = -5000; // MeV
  double nM2Max = 5000; // MeV
=======
  double nEnergyMax = 510; // MeV
  int nEnergyBins = 50;
  double dEnergyBinSize = (double)(nEnergyMax - nEnergyMin) / (double)nEnergyBins;

  double nThetaMin = 34; // mrad
  double nThetaMax = 88; // mrad
  int nThetaBins = 50;
  double dThetaBinSize = (nThetaMax - nThetaMin) / nThetaBins;

  double nM2Min = -1000; // MeV
  double nM2Max = 1000; // MeV
>>>>>>> 911b5ec345c81b054214c521136984e9d6306827
  int nM2Bins = 100;
  double dM2BinSize = (nM2Max - nM2Min)/nM2Bins;

  TH1D* hgammaEnergy = new TH1D("Energy" ,                    // plot label
                                "Gamma Energy Distribution",  // title
                                nEnergyBins,                  // x number of bins
                                nEnergyMin,                   // x lower bound
                                nEnergyMax);                  // x upper bound

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
<<<<<<< HEAD
    if (pID == 22 && energy > 20) { // gammas only
      theta*= TMath::Pi()/180; //radians
      

      if(mSquared(energy, theta) > 0){
        cout << "M^2 is: " << mSquared(energy, theta) << endl;
      }
      //cout << "Energy is: " << energy << endl;
           
=======

    if (pID == 22) { // gammas only
      theta*= TMath::Pi()/180; //radians
>>>>>>> 911b5ec345c81b054214c521136984e9d6306827
      hgammaEnergy->Fill(energy,BINNING_WEIGHT / dEnergyBinSize);
      hgammaTheta->Fill(theta*1000, BINNING_WEIGHT / dThetaBinSize);
      henergytheta->Fill(theta*1000, energy);
      hm2->Fill(mSquared(energy, theta), BINNING_WEIGHT / dM2BinSize);
    }
  }

<<<<<<< HEAD
  TAxis *axis = hm2->GetXaxis();
  int bmin = axis->FindBin(-5000); //in your case xmin=-1.5
  int bmax = axis->FindBin(5000); //in your case xmax=0.8
  double integral = hm2->Integral(bmin,bmax);
  cout << "Area under graph is" << integral << endl;

=======
>>>>>>> 911b5ec345c81b054214c521136984e9d6306827



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

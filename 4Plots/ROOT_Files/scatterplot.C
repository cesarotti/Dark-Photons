#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "THStack.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream>
#include "TCanvas.h"
#include "TLegend.h"



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
double SCAT_NORM = 2.;
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




void scatterplot() {

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

 THStack *hs = new THStack("hs","Scatterplot");

  TH2F* henergythetayy = new TH2F("scatter4",            // plot label
                                "e+e- > yy",   // title
                                nThetaBins,           // x num of bins
                                nThetaMin,            // x min
                                nThetaMax,            // x max
                                nEnergyBins,          // y num of bins
                                nEnergyMin,           // y min
                                nEnergyMax);          // y max
  henergythetayy->SetMarkerColor(kBlue);
  henergythetayy->SetMarkerStyle(21);
  henergythetayy->SetMarkerSize(1.);

 

  TFile* file = new TFile("e+e-2yyGUN.root");

  TTree* Hits_Info = (TTree *)file->Get("Signal");

  Hits_Info->SetBranchAddress("numHits", &numHit);
  Hits_Info->SetBranchAddress("energyTot", &energy);
  Hits_Info->SetBranchAddress("XPosition", &xPos);
  Hits_Info->SetBranchAddress("YPosition", &yPos);
  Hits_Info->SetBranchAddress("Particle_ID", &pID);
  Hits_Info->SetBranchAddress("Theta", &theta);


  // go through all entries and fill the histograms
  int nentries = Hits_Info->GetEntries();
  for (int i=0; i<nentries; i++) {
    Hits_Info->GetEntry(i);
    if (pID == 22 && i < (XSECyy * SCAT_NORM)) { // gammas only
      theta*= TMath::Pi()/180; //radians
      cout << "M^2 is: " << mSquared(energy, theta) << endl;
      cout << "Energy is: " << energy << endl;
      henergythetayy->Fill(theta*1000, energy);
    }
  }
  cout << "DM2binsize is:" << dM2BinSize << endl;




  //henergythetayy->SetFillColor(kBlue);
  //hmyy->SetFillStyle(3001);
  henergythetayy->GetXaxis()->SetTitle("#theta");
  henergythetayy->GetYaxis()->SetTitle("Energy (MeV)");
  henergythetayy->GetXaxis()->CenterTitle();
  henergythetayy->GetYaxis()->CenterTitle();
  //hmyy->GetYaxis()->SetTitle("Photons per MeV^{2} per Second (MeV^{-2} s^{-1})");
  //hmyy->GetXaxis()->CenterTitle();
  //hmyy->GetYaxis()->CenterTitle();
  hs->Add(henergythetayy);

  TFile* fileyyy = new TFile("e+e-2yyyGUN.root");
  TTree* Hits_Infoyyy = (TTree *)fileyyy->Get("Signal");

  Hits_Infoyyy->SetBranchAddress("numHits", &numHit);
  Hits_Infoyyy->SetBranchAddress("energyTot", &energy);
  Hits_Infoyyy->SetBranchAddress("XPosition", &xPos);
  Hits_Infoyyy->SetBranchAddress("YPosition", &yPos);
  Hits_Infoyyy->SetBranchAddress("Particle_ID", &pID);
  Hits_Infoyyy->SetBranchAddress("Theta", &theta);

  TH2F* henergythetayyy = new TH2F("scatter3",            // plot label
                                "e+e- > yyy",   // title
                                nThetaBins,           // x num of bins
                                nThetaMin,            // x min
                                nThetaMax,            // x max
                                nEnergyBins,          // y num of bins
                                nEnergyMin,           // y min
                                nEnergyMax);          // y max

  henergythetayyy->SetMarkerColor(kRed);
    henergythetayyy->SetMarkerStyle(20);
  henergythetayyy->SetMarkerSize(1.);

  // go through all entries and fill the histograms
  nentries = Hits_Infoyyy->GetEntries();
  for (int i=0; i<nentries; i++) {
    Hits_Infoyyy->GetEntry(i);
    if (pID == 22 && i < (SCAT_NORM * XSECyyy)) { // gammas only
      theta*= TMath::Pi()/180; //radians
      cout << "M^2 is: " << mSquared(energy, theta) << endl;
      cout << "Energy is: " << energy << endl;
           
      henergythetayyy->Fill(theta*1000, energy);
    }
  }
  cout << "DM2binsize is:" << dM2BinSize << endl;


  //henergythetayyy->SetFillColor(kRed);
 // hmyyy->SetFillStyle(3001);
  //hmyyy->GetXaxis()->SetTitle("M_{A'}^{2} (MeV^{2})");
  //hmyyy->GetYaxis()->SetTitle("Photons per MeV^{2} per Second (MeV^{-2} s^{-1})");
  //hmyyy->GetXaxis()->CenterTitle();
  //hmyyy->GetYaxis()->CenterTitle();

  henergythetayyy->GetXaxis()->SetTitle("#theta");
  henergythetayyy->GetYaxis()->SetTitle("Energy (MeV)");
    henergythetayy->GetXaxis()->CenterTitle();
  henergythetayyy->GetYaxis()->CenterTitle();
  hs->Add(henergythetayyy);


/*YIMIN*/
   TFile* fileyimin = new TFile("brem.root");
  //This is 1000000 events so DOWNEIGHT IT:
  //XSECyimin = 0.1 * XSECyimin;
  TTree* Hits_Infoyimin = (TTree *)fileyimin->Get("Signal");

  Hits_Infoyimin->SetBranchAddress("numHits", &numHit);
  Hits_Infoyimin->SetBranchAddress("energyTot", &energy);
  Hits_Infoyimin->SetBranchAddress("XPosition", &xPos);
  Hits_Infoyimin->SetBranchAddress("YPosition", &yPos);
  Hits_Infoyimin->SetBranchAddress("Particle_ID", &pID);
  Hits_Infoyimin->SetBranchAddress("Theta", &theta);

  TH2F* henergythetayimin = new TH2F("scatter1",            // plot label
                                "Bremsstrahlung",   // title
                                nThetaBins,           // x num of bins
                                nThetaMin,            // x min
                                nThetaMax,            // x max
                                nEnergyBins,          // y num of bins
                                nEnergyMin,           // y min
                                nEnergyMax);          // y max

  henergythetayimin->SetMarkerColor(7);
  henergythetayimin->SetMarkerStyle(25);
  henergythetayimin->SetMarkerSize(1.);


  // go through all entries and fill the histograms
  nentries = Hits_Infoyimin->GetEntries();
  for (int i=0; i<nentries; i++) {
    Hits_Infoyimin->GetEntry(i);
    if (pID == 22 && i < (SCAT_NORM * XSECyyy)) { // gammas only
      theta*= TMath::Pi()/180; //radians
      cout << "M^2 is: " << mSquared(energy, theta) << endl;
      cout << "Energy is: " << energy << endl;
           
      henergythetayimin->Fill(theta*1000, energy);
    }
  }
  cout << "DM2binsize is:" << dM2BinSize << endl;


  //henergythetayimin->SetFillColor(7);
  /*
  hmyimin->SetFillStyle(3001);
  hmyimin->GetXaxis()->SetTitle("M_{A'}^{2} (MeV^{2})");
  hmyimin->GetYaxis()->SetTitle("Photons per MeV^{2} per Second (MeV^{-2} s^{-1})");
  hmyimin->GetXaxis()->CenterTitle();
  hmepluseminusy->GetYaxis()->CenterTitle();
  */
  henergythetayimin->GetXaxis()->SetTitle("#theta");
  henergythetayimin->GetYaxis()->SetTitle("Energy (MeV)");
    henergythetayimin->GetXaxis()->CenterTitle();
  henergythetayimin->GetYaxis()->CenterTitle();
  hs->Add(henergythetayimin);

  
  TFile* fileepluseminusy = new TFile("e+e-2e+e-yECUT0.3NOPT_2_5_1000000.root");
  //This is 1000000 events so DOWNEIGHT IT:
  //XSECepluseminusy = 0.1 * XSECepluseminusy;
  TTree* Hits_Infoepluseminusy = (TTree *)fileepluseminusy->Get("Signal");

  Hits_Infoepluseminusy->SetBranchAddress("numHits", &numHit);
  Hits_Infoepluseminusy->SetBranchAddress("energyTot", &energy);
  Hits_Infoepluseminusy->SetBranchAddress("XPosition", &xPos);
  Hits_Infoepluseminusy->SetBranchAddress("YPosition", &yPos);
  Hits_Infoepluseminusy->SetBranchAddress("Particle_ID", &pID);
  Hits_Infoepluseminusy->SetBranchAddress("Theta", &theta);

  TH2F* henergythetay = new TH2F("scatter2",            // plot label
                                "e+e- > e+e-y",   // title
                                nThetaBins,           // x num of bins
                                nThetaMin,            // x min
                                nThetaMax,            // x max
                                nEnergyBins,          // y num of bins
                                nEnergyMin,           // y min
                                nEnergyMax);          // y max

  henergythetay->SetMarkerColor(kBlack);
    henergythetay->SetMarkerStyle(22);
  henergythetay->SetMarkerSize(1.);


  // go through all entries and fill the histograms
  nentries = Hits_Infoepluseminusy->GetEntries();
  for (int i=0; i<nentries; i++) {
    Hits_Infoepluseminusy->GetEntry(i);
    if (pID == 22 && i < (SCAT_NORM * XSECepluseminusy)) { // gammas only
      theta*= TMath::Pi()/180; //radians
      cout << "M^2 is: " << mSquared(energy, theta) << endl;
      cout << "Energy is: " << energy << endl;
           
      henergythetay->Fill(theta*1000, energy);
    }
  }
  cout << "DM2binsize is:" << dM2BinSize << endl;


  //henergythetay->SetFillColor(7);
  /*
  hmepluseminusy->SetFillStyle(3001);
  hmepluseminusy->GetXaxis()->SetTitle("M_{A'}^{2} (MeV^{2})");
  hmepluseminusy->GetYaxis()->SetTitle("Photons per MeV^{2} per Second (MeV^{-2} s^{-1})");
  hmepluseminusy->GetXaxis()->CenterTitle();
  hmepluseminusy->GetYaxis()->CenterTitle();
  */
  henergythetay->GetXaxis()->SetTitle("#theta");
  henergythetay->GetYaxis()->SetTitle("Energy (MeV)");
    henergythetay->GetXaxis()->CenterTitle();
  henergythetay->GetYaxis()->CenterTitle();
  hs->Add(henergythetay);
  

  /*

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
  hs->Add(hYIMIN);

*/

  
  
  /**********************
      ADD BRIANS PLOT
    ********************/
/*
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

  TH2F* henergythetainelastic = new TH2F("scatter",            // plot label
                                "#theta vs Energy",   // title
                                nThetaBins,           // x num of bins
                                nThetaMin,            // x min
                                nThetaMax,            // x max
                                nEnergyBins,          // y num of bins
                                nEnergyMin,           // y min
                                nEnergyMax);          // y max
  // go through all entries and fill the histograms
  nentries = Hits_Infoinelastic->GetEntries();
  for (int i=0; i<nentries; i++) {
    Hits_Infoinelastic->GetEntry(i);
    if (pID == 22) { // gammas only
      theta*= TMath::Pi()/180; //radians
      cout << "M^2 is: " << mSquared(energy, theta) << endl;
      cout << "Energy is: " << energy << endl;
           
      henergythetainelastic->Fill(theta*1000, energy);
    }
  }
  cout << "DM2binsize is:" << dM2BinSize << endl;


  henergythetainelastic->SetFillColor(kGreen);
  /*
  hminelastic->SetFillStyle(3001);
  hminelastic->GetXaxis()->SetTitle("M_{A'}^{2} (MeV^{2})");
  hminelastic->GetYaxis()->SetTitle("Photons per MeV^{2} per Second (MeV^{-2} s^{-1})");
  hminelastic->GetXaxis()->CenterTitle();
  hminelastic->GetYaxis()->CenterTitle();

  hs->Add(henergythetainelastic);
*/



  // create canvas and draw histogram
  TCanvas* canvas = new TCanvas("canvas", "canvas", 700, 700);
  canvas->Divide(1,1);
  TPad* p;

  p = (TPad*)canvas->cd(1);
  //p->SetGrid();
  //p->SetLogy();

 

  

  hs->Draw("SCAT");
  hs->GetXaxis()->SetTitle("#theta");
  hs->GetYaxis()->SetTitle("Energy (MeV)");
  hs->GetXaxis()->CenterTitle();
  hs->GetYaxis()->CenterTitle();
  hs->SetTitle("Scatterplot of #theta vs Energy (MeV)");
  //hs->CenterTitle();

   TLegend *leg = new TLegend(0.1,0.7,0.48,0.9);
   leg->SetHeader("Backgrounds");
   leg->AddEntry(henergythetay,"e+ e- > e+ e- y","P");
   leg->AddEntry(henergythetayy,"e+ e- > y y","P");
   leg->AddEntry(henergythetayyy,"e+ e- > y y y","P");
   leg->AddEntry(henergythetayimin,"Bremsstrahlung","P");
   leg->Draw();
  /*
  p = (TPad*)canvas->cd(4);
  henergythetay->Draw();
  p = (TPad*)canvas->cd(3);
  henergythetayyy->Draw();
  p = (TPad*)canvas->cd(2);
  henergythetayy->Draw();
  */

  /*p = (TPad*)canvas->cd(5);
  hYIMIN->Draw();

  p = (TPad*)canvas->cd(6);
  henergythetainelastic->Draw();

/*
  p = (TPad*)canvas->cd(4);
  p->SetLogy();
  p->SetGrid();
  hm2->Draw();*/


}

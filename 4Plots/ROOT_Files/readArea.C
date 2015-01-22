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

const double NUM_TOT_POSITRONS = 100000; 
const double BIAS = 1e+06;
const double POSITRONS_PER_SEC = 6e+09; 
const double XSEC  = 145.;
const double NORMAL_FACTOR = 1.;
const double BINNING_WEIGHT = XSEC * NORMAL_FACTOR * POSITRONS_PER_SEC/(BIAS*NUM_TOT_POSITRONS);
const double CUSTOM_WEIGHT = 145. * 0.0926562;
const double B = pow(1-pow(GAMMA_PLUS, -2.), .5);




//Energy in MeV, theta in radians
double mSquared(double energy, double theta)
{
  return 2*ELECTRON_MASS*(ELECTRON_MASS+POSITRON_ENERGY-
     energy*(2*GAMMA_CM_2-B*POSITRON_ENERGY*TMath::Cos(theta)/ELECTRON_MASS));
}




void readArea() {

  double yPos, xPos, theta, energy = 0.;  // NOTE: theta comes in degrees
  int numHit, pID = 0;
  
  TFile *f = new TFile("2gamma_geant_small_ang.root"); 

  f->ls(); 
  
  //3.4mb acceptance * xsec
  TCanvas* canvas3 = (TCanvas*)f->Get("canvas");

  TObject *obj; 
  TIter next(canvas3->GetListOfPrimitives());
   while ((obj=next())) {
     cout << "Reading: " << obj->GetName() << endl;
       if (obj->InheritsFrom("TH1")) {
     cout << "histo: " << obj->GetName() << endl;
       }
  }  
  TCanvas* canvas2 = (TCanvas*)canvas3->GetPrimitive("canvas_1");
  TH1F * hYIMIN = (TH1F*)canvas2->GetPrimitive("M_{A'}^{2}"); 

  TAxis *axis = hYIMIN->GetXaxis();
  int bmin = axis->FindBin(-5000); //in your case xmin=-1.5
  int bmax = axis->FindBin(5000); //in your case xmax=0.8
  double integral = hYIMIN->Integral(bmin,bmax);
  cout << "Area under graph is" << integral << endl;




  // create canvas and draw histogram
  TCanvas* canvas = new TCanvas("canvas", "canvas", 700, 700);
  canvas->Divide(3,2);
  TPad* p;

  p = (TPad*)canvas->cd(1);
  p->SetGrid();
  hYIMIN->Draw();


}

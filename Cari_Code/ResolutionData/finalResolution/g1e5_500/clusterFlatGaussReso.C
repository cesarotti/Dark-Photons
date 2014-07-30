#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include <iostream>
#include "TCanvas.h"
#include <sstream>
#include "TF2.h"
#include "TMatrixD.h"
#include "TRandom3.h"


/*
 * This program first finds all of the energy 'bumps' in the detector, or 
 * energy deposition per crystal that surpasses a high threshold. After 
 * finding these, it performs a depth first search on each of the bumps. 
 * After the depth first search, it tries to uncluster any overlap.
 * It then only considers a set number of crystals to sum over when computing
 * total energy and position
 */

double energyGamma = 500.;
vector<pair<int, double> >::iterator VP_iterator; 
vector<vector<pair<int, double> > >::iterator VVP_iterator;

/*
 * x[0] = x  
 * par[0] = x_naught
 * par[1] = width of flat
 * par[2] = sigma
 * par[3] = energy
 */
double flatGauss(double* x, double* par)
{
  double X = x[0]-par[0];
  double flatGaus(0.);
  if (X+par[1]< 0)
    {flatGaus = TMath::Exp(-1*pow((X+par[1])/(sqrt(2.)*par[2]), 2.)); }
  else if (X>par[1])
    {flatGaus = TMath::Exp(-1*pow((X-par[1])/(sqrt(2.)*par[2]), 2.)); }
  else 
    {flatGaus = 1; }

  return par[3]*pow(sqrt(TMath::Pi()*2.)*par[1]+1, -1.)*flatGaus;
  
}

// Changes x and y coordinates in crystal units to the crystal ID
int CoordtoID(int x, int y)
{
  return(y+5)*11+(x+5);
}
 

// Gets x position from crystalID
int IDtoX(int crystalID)
{
  return crystalID%11-5;
}


//Gets y position from crystalID
int IDtoY(int crystalID)
{
  return crystalID/11-5;
}


void flatGaussFit(pair<int, double> bump, vector<pair<int, double> > shower, 
	    TH1D* energyResoX, TH1D* energyResoY, TH1D* posResoX,
	    TH1D* posResoY)
 {
   delete gDirectory->FindObject("flatGX");
   TF1* flatGX = new TF1("flatGX", flatGauss, -5, 5, 4);
   flatGX->SetParameters(IDtoX(bump.first), 1., 1., bump.second);
   delete gDirectory->FindObject("flatGY");
   TF1* flatGY = new TF1("flatGY", flatGauss, -5, 5, 4); 
   flatGY->SetParameters(IDtoY(bump.first), 1.,1., bump.second);
 
  delete gDirectory->FindObject("histo");
   TH2D* histo = new TH2D("histo", "ShowerFit",
			  11, -5, 5,
			  11, -5, 5);


   vector<pair<int, double> >::iterator b; 
   for (b=shower.begin(); b!=shower.end(); b++)
	 {
	   histo->Fill(IDtoX(b->first), IDtoY(b->first), b->second);
	 }

   TH1D* histoX = histo->ProjectionX();
   TH1D* histoY = histo->ProjectionY();
   histoX->Fit(flatGX, "Q");
   energyResoX->Fill(flatGX->Integral(-5, 5)-energyGamma);
   posResoX->Fill(flatGX->GetParameter(0));
   histoY->Fit(flatGY, "Q");
       energyResoY->Fill(flatGY->Integral(-5, 5)-energyGamma);
       posResoY->Fill(flatGY->GetParameter(0));
 }
		    
 
void clusterFlatGaussReso() {

  TFile* file = new TFile("complete.root");

  TTree* tree = (TTree *)file->Get("Signal");

  int nEvents = 30000;

  double addresses[121] = {};
  for (int k=0; k<121; k++){
    std::stringstream ss2;
    ss2 << k; 
    string str = "Crystal_"+ss2.str();
    const char* charstr = str.c_str(); 
    tree->SetBranchAddress(charstr, &addresses[k]);
  }

  TH1D* energyResoX = new TH1D("energyReso", "Flat_Gauss_EnergyX_Resolution",
			      energyGamma/5, -1*energyGamma, energyGamma);

  TH1D* energyResoY = new TH1D("energyResoY", "Flat_Gauss_EnergyY_Resolution",
			      energyGamma/5, -1*energyGamma, energyGamma);

  TH1D* posResoX = new TH1D("posReso", "Flat_Gauss_PosX_Resolution", 
			    50, -5, 5);

  TH1D* posResoY = new TH1D("posReso", "Flat_Gauss_PosY_Resolution", 
			    50, -5, 5);


  double energyThresHi = 50.; //set energy threshold to start looking for bumps

  //Going through each scan of the calorimeter
  for (int k=0; k<nEvents; k++)
    {
      cout << "event number: " << k << endl;
      tree->GetEntry(k);

      pair<int, double> bump(0, 0.); 
      vector<pair<int, double> > geant;
      vector<pair<int, double> > hitMap;

      for(int i=0; i<121; i++)
	{
	  TRandom3* rand = new TRandom3(12191982);
	  double noise = rand->Gaus(0, .5); 
	  pair<int, double> hit(i, addresses[i]+noise);
	  geant.push_back(hit);
	  if (addresses[i] > energyThresHi)
	    { hitMap.push_back(hit);}
	  if (hit.second>bump.second)
	    { bump = hit;}
	}
	
      flatGaussFit(bump, geant, energyResoX, energyResoY, posResoX, posResoY);


    } //end of event
 
 TCanvas* canvas = new TCanvas("canvas", "canvas", 700,700);
canvas->Divide(2, 2);
 TF1* g1d1 = new TF1("g1d1", "gaus", -10, 10);
 TF1* g1d2 = new TF1("g1d2", "gaus", -10, 10);
 TF1* g1d3 = new TF1("g1d3", "gaus", -5, 5);
 TF1* g1d4 = new TF1("g1d4", "gaus", -5, 5);
 energyResoX->Fit(g1d1);
 energyResoY->Fit(g1d2);
 posResoX->Fit(g1d3);
 posResoY->Fit(g1d4);
canvas->cd(1); energyResoX->Draw();
 canvas->cd(2); energyResoY->Draw();
 canvas->cd(3); posResoX->Draw();
 canvas->cd(4); posResoY->Draw();
  

}
 

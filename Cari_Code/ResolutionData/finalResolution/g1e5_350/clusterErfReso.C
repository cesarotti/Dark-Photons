
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


/*
 * This program first finds all of the energy 'bumps' in the detector, or 
 * energy deposition per crystal that surpasses a high threshold. After 
 * finding these, it performs a depth first search on each of the bumps. 
 * After the depth first search, it tries to uncluster any overlap.
 * It then only considers a set number of crystals to sum over when computing
 * total energy and position
 */

double energyGamma = 350.;
vector<pair<int, double> >::iterator VP_iterator; 
vector<vector<pair<int, double> > >::iterator VVP_iterator;

/*
 * x[0] = x  
 * par[0] = x_naught
 * par[1] = sigma
 * par[2] = energy
 */
double erfPulse(double* x, double* par)
{
  double y = x[0]-par[0]; 
  double erfVal(0);
  if (y<0)
    {erfVal = 1+TMath::Erf((y/(2*par[1]))+2*par[1]); }
  else 
    {erfVal = TMath::Erfc((y/(2*par[1]))-2*par[1]); }

  return .5*par[2]*erfVal;
  
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

pair<double, pair<double, double> > reconstruct(std::vector<std::pair<int, double> > shower)
{
  double energy(0.), xPos(0.), yPos(0.);
  //looks at crystals in the shower
  for (VP_iterator=shower.begin(); VP_iterator!=shower.end(); ++VP_iterator)
    {
      energy+= VP_iterator->second;
      xPos+=IDtoX(VP_iterator->first)*VP_iterator->second; 
      yPos+=IDtoY(VP_iterator->first)*VP_iterator->second;
    }
  //takes weighted average
  xPos/=energy; yPos/=energy;
  pair<double, double> position(xPos*5.0, yPos*5.0);
  pair<double, pair<double, double> > photon(energy, position);
  return photon;
}

void erfFit(pair<int, double> bump, vector<pair<int, double> > shower, 
	    TH1D* energyResoX, TH1D* energyResoY, TH1D* posResoX,
	    TH1D* posResoY)
 {
   delete gDirectory->FindObject("erfX");
   TF1* erfX = new TF1("erfX", erfPulse, -5, 5, 3);
   erfX->SetParameters(IDtoX(bump.first), 1., bump.second);
delete gDirectory->FindObject("erfY");
   TF1* erfY = new TF1("erfY", erfPulse, -5, 5, 3); 
   erfY->SetParameters(IDtoY(bump.first), 1., bump.second);

 delete gDirectory->FindObject("histoX");
  delete gDirectory->FindObject("histoY");

   TH1D* histoX = new TH1D("histoX", "ShowerFitX", 11, -5, 5);
   TH1D* histoY = new TH1D("histoY", "ShowerFitY", 11, -5, 5);


   vector<pair<int, double> >::iterator b; 
   for (b=shower.begin(); b!=shower.end(); b++)
	 {
	   histoX->Fill(IDtoX(b->first),b->second);
	   histoY->Fill(IDtoY(b->first), b->second);
	 }

   histoX->Fit(erfX, "Q");
   energyResoX->Fill(erfX->Integral(-5, 5)-energyGamma);
   posResoX->Fill(erfX->GetParameter(0));
   histoY->Fit(erfY, "Q");
   energyResoY->Fill(erfY->Integral(-5, 5)-energyGamma);
   posResoY->Fill(erfY->GetParameter(0));
 
			  
 }
		    
 
void clusterErfReso() {

  TFile* file = new TFile("complete.root");

  TTree* tree = (TTree *)file->Get("Signal");

  int nEvents = tree->GetEntries();

  double addresses[121] = {};
  for (int k=0; k<121; k++){
    std::stringstream ss2;
    ss2 << k; 
    string str = "Crystal_"+ss2.str();
    const char* charstr = str.c_str(); 
    tree->SetBranchAddress(charstr, &addresses[k]);
  }

  TH1D* energyResoX = new TH1D("energyResoX", "Energy_Resolution_X", 100, -10, 10);

  TH1D* energyResoY = new TH1D("energyResoX", "Energy_Resolution_X", 100, -10, 10);

  TH1D* posResoX = new TH1D("posResoX", "Pos_Resolution_X", 50, -5, 5);

  TH1D* posResoY = new TH1D("posResoY", "Pos_Resolution_Y", 50, -5, 5);

  double energyThresHi = 50.;

  //Going through each scan of the calorimeter
  for (int k=0; k<nEvents; k++)
    {
      tree->GetEntry(k);

      pair<int, double> bump(0, 0.); 
      vector<pair<int, double> > geant;
      vector<pair<int, double> > hitMap;

      for(int i=0; i<121; i++)
	{
	  pair<int, double> hit(i, addresses[i]);
	  geant.push_back(hit);
	  if (addresses[i] > energyThresHi)
	    { hitMap.push_back(hit);}
	  if (hit.second>bump.second)
	    { bump = hit;}
	}
	
      erfFit(bump, geant, energyResoX, energyResoY, posResoX, posResoY);


    } //end of event
 
  TF1* g1d1 = new TF1("g1d1", "gaus", -10, 10);
  TF1* g1d2 = new TF1("g1d2", "gaus", -10, 10);
  TF1* g1d3 = new TF1("g1d3", "gaus", -5, 5);
  TF1* g1d4 = new TF1("g1d4", "gaus", -5, 5);

 TCanvas* canvas = new TCanvas("canvas", "canvas", 700,700);
canvas->Divide(2, 2);
 cout << "fitting x energy: " << endl;
 energyResoX->Fit(g1d1);
 canvas->cd(1); energyResoX->Draw();
 cout << "fitting y energy: " << endl;
 energyResoY->Fit(g1d2);
 canvas->cd(2); energyResoY->Draw();
 cout << "fitting position x" << endl;
 posResoX->Fit(g1d3);
 canvas->cd(3); posResoX->Draw();
 cout << "fitting position y" << endl;
 posResoY->Fit(g1d4);
 canvas->cd(4); posResoY->Draw();
  

}
 

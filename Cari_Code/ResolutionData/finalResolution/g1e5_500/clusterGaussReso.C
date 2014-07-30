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
double gammaEnergy = 500.;
vector<pair<int, double> >::iterator VP_iterator; 
vector<vector<pair<int, double> > >::iterator VVP_iterator;

double Gaus2D(double* x, double* par)
{
  double a = par[2]*par[2];
  double b = par[2]*par[3]*par[4];
  double c = par[3]*par[3];
  double v1 = x[0]-par[0];
  double v2 = x[1]-par[1];

  double  exp =  (c*pow(v1,2.) +a*pow(v2, 2.)-2*b*v1*v2)/(-pow(b,2.) + a*c);

  return par[5]*pow(2*TMath::Pi()*sqrt(a*c-b*b), -1.)*pow(TMath::E(), -.5*exp);
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

// Returns distance between two crystals as defined by minimal path
int dist(int crysFID, int crysSID)
{
  return TMath::Max(TMath::Abs(IDtoX(crysFID)-IDtoX(crysSID)), 
		    TMath::Abs(IDtoY(crysFID)-IDtoY(crysSID)));
}

// Returns the total energy in all crystals of a shower
double clusterDep(vector<pair<int, double> > shower)
{
  double totEnergy(0.);
  for (VP_iterator=shower.begin(); VP_iterator!=shower.end(); VP_iterator++)
    {totEnergy+=VP_iterator->second;}
  return totEnergy;
}

//checks if a shower is already in the cluster
// ROOT really hates this method.
bool findVector (std::vector<std::vector<std::pair<int, double> > > detector,
		std::vector<std::pair<int, double> > shower)
{
  // Look through detector
  std::vector<std::vector<std::pair<int, double> > >::iterator a; 
  for (a=detector.begin(); a!=detector.end(); ++a)
    { // remember that crystal ID's are ordered, and touching clusters have
      // the same crystalIDs
        if ((a->front()).first==(shower.front()).first) return true;
    }
      return false;
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
  pair<double, double> position(xPos, yPos);
  pair<double, pair<double, double> > photon(energy, position);
  return photon;
}

//Sorts energies from largest to smallest
vector<pair<int, double> > energySort(vector<pair<int, double> > shower)
{
  std::vector<std::pair<double, int> > energy;
  for (VP_iterator=shower.begin(); VP_iterator!=shower.end(); VP_iterator++)
    {
      pair<double, int> flipped(VP_iterator->second, VP_iterator->first);
      energy.push_back(flipped);
    }
  std::map<double, int> myMap(energy.begin(), energy.end());
  map<double, int>::iterator m;
  vector<pair<int, double> > sorted;
  for (m=myMap.end(); m!=myMap.begin(); --m)
    {
     pair<int, double> orderHit(m->second, m->first);
     sorted.push_back(orderHit);
    }
  sorted.erase(sorted.begin(), sorted.begin()+1);
  return sorted;
}

// Searchs all neighbors of a crystal with nontrivial energy deposit until all
// neighbors are accounted for or have negligible energy deposition
vector<pair<int, double> > DFS(pair<int, double> start, double energyThresh, vector<pair<int, double> > shower, double map[])
{
  shower.push_back(start);
  for (int x=-1; x<2; x++) {
    for (int y=-1; y<2; y++) {
      int ngbrID = CoordtoID(IDtoX(start.first)+x, IDtoY(start.first)+y);
      double ngbrEn = map[ngbrID];
      pair<int, double> ngbr(ngbrID, ngbrEn);
      if (ngbrEn>energyThresh)
	{
	  vector<int> showerID; 
	  //no method for searching pairs
	  for (int f=0; f<shower.size(); f++) {showerID.push_back((shower[f]).first);}
	  if (std::find(showerID.begin(), showerID.end(), ngbrID)!=showerID.end()) 
	    {continue;}
	  // if it has enough energy and has not been counted
	  else {shower = DFS(ngbr, energyThresh, shower, map);}
	}
    }
  }
  //put crystals in correct order to make other methods simpler
  std::sort(shower.begin(), shower.end());
  return shower;
}

void gaussFit(pair<int, double> bump, vector<pair<int, double> > shower, TH1D* reso, TH2D* resoPos)
 {
       TF2* g2d = new TF2("g2D", Gaus2D, -5, 5, -5, 5, 6);
       g2d->SetParameters(IDtoX(bump.first),
			  IDtoY(bump.first), 
			  1., 1., 0., (bump.second));

       TH2D* histo = new TH2D("histo", "ShowerFit",
			      11, -5, 5,
			      11, -5, 5);


       vector<pair<int, double> >::iterator b; 
       for (b=shower.begin(); b!=shower.end(); b++)
	 {
	   histo->Fill(IDtoX(b->first), IDtoY(b->first), b->second);
	 }
       // g2d->FixParameter(4, 0.);
       histo->Fit(g2d, "Q");
       //g2d->Draw("cont1 same");
       reso->Fill(g2d->GetParameter(5)-gammaEnergy);
       resoPos->Fill(g2d->GetParameter(0), g2d->GetParameter(1));	
			  
 }

void clusterGaussReso() {

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

  TH1D* energyResoG = new TH1D("energyResoG", "Gauss_Energy_Resolution",
			      100, -10, 10);

  TH2D* posResoG = new TH2D("posResoG", "Gauss_Pos_Resolution", 
			    11, -5, 5, 11, -5, 5);

  double energyThresHi = 50.; //set energy threshold to start looking for bumps
  double energyThresLo = .5; //energy lower bound


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

      gaussFit(bump, geant, energyResoG, posResoG);


    } //end of event

 TCanvas* canvas = new TCanvas("canvas", "canvas", 700,700);
 canvas->Divide(2, 1);
 TF1* g1d = new TF1("g1d", "gaus", -10, 10);
 TF2* g2d2 = new TF2("g2d2", Gaus2D, -10, 10, -10, 10, 6);
 g2d2->SetParameters(0., 0., 1., 1., 0., 100000);
 g2d2->FixParameter(4, 0.);
 energyResoG->Fit(g1d);
canvas->cd(1); energyResoG->Draw();
 posResoG->Fit(g2d2);
canvas->cd(2); posResoG->Draw("BOX");

}
 

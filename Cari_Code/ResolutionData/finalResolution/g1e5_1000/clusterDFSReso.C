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
 * This program gives the idealized energy resolutions for DFS for a single
 * photon on the program photonTest in Geant. 
 */

double gammaEnergy = 1000.;
vector<pair<int, double> >::iterator VP_iterator; 
vector<vector<pair<int, double> > >::iterator VVP_iterator;


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
 

void clusterDFSReso() {

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

  TH1D* energyResoD = new TH1D("energyResoD", "DFS_Energy_Resolution", 
			       gammaEnergy/5, -1*gammaEnergy, gammaEnergy);

  TH1D* posResoDX = new TH1D("posResoD", "DFS_PosX_Resolution", 
			    1000, -5, 5);

  TH1D* posResoDY = new TH1D("posResoD", "DFS_PosY_Resolution", 
			    1000, -5, 5);

  double energyThresHi = 50.; //set energy threshold to start looking for bumps
  double energyThresLo = .5; //energy lower bound

  //Going through each scan of the calorimeter
  for (int k=0; k<nEvents; k++)
    {
      tree->GetEntry(k);
      cout << "Event number: " <<  k << endl;
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
      // hitMap now stores the <ID, energy> of each crystal with energy 
      // over a certain threshold, geant now contains all crystal information


      std::vector<std::vector<std::pair<int, double> > > detector;

      std::vector<std::pair<int, double> > shower;
      
      detector.push_back(DFS(bump,
			     energyThresLo,
			     shower, addresses));
      

      //detector = uncluster(detector, hitMap);
      // no longer counts two showers if bumps are next to each other
      
      int num = 9; // number of crystals considered

      for (VVP_iterator=detector.begin(); VVP_iterator!=detector.end();
	   ++VVP_iterator)
	{
	  energyResoD->Fill(reconstruct(detector[0]).first-gammaEnergy);
	  posResoDX->Fill(reconstruct(detector[0]).second.first);
	  posResoDY->Fill(reconstruct(detector[0]).second.second);
	}
	
 
       

    } //end of event 
  
 TCanvas* canvas = new TCanvas("canvas", "canvas", 700,700);
 canvas->Divide(2, 2);
 
       TF1* g1dX = new TF1("g1dX", "gaus", -5, 5);
       posResoDX->Fit(g1dX);
       canvas->cd(1); posResoDX->Draw();
       
       TF1* g1dY = new TF1("g1dY", "gaus", -5, 5);
       posResoDY->Fit(g1dY);
       canvas->cd(2); posResoDY->Draw(); 
       
       TF1* g1d = new TF1("g1d", "gaus", -10, 10);
       energyResoD->Fit(g1d);
       canvas->cd(3); energyResoD->Draw();
    
       


}
 

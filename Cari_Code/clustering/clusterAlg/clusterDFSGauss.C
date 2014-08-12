#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include <iostream>
#include "TCanvas.h"
#include <sstream>
#include "TMatrixD.h"
#include "TF2.h"

vector<pair<int, double> >::iterator VP_iterator; 
vector<vector<pair<int, double> > >::iterator VVP_iterator;
vector<pair<double, int> >::iterator VP2_iterator;

double Gaus2D(double* x, double* par)
{
  
  double vectr[2] = {par[0], par[1]};
  double matrix[4]
    = {pow(par[2], 2.), (par[4])*(par[2])*(par[3]),
       (par[4])*(par[2])*(par[3]), pow(par[3], 2.)}; 
  TMatrixD u_vec(2, 1, x, "");
  TMatrixD u_naught(2, 1, vectr, "");
  TMatrixD VMatrix(2, 2, matrix, "");
  
  double det = VMatrix.Determinant();
  VMatrix.Invert();
  
  TMatrixD firstStep((u_vec-u_naught), TMatrixD::kTransposeMult, VMatrix);
  TMatrixD secondStep(firstStep, TMatrixD::kMult, u_vec-u_naught);
  
  const double *data = secondStep.GetMatrixArray();
  double exp = data[0];
  
  return (pow(2*TMath::Pi()*det, -1.)*pow(TMath::E(), -.5*exp)*par[5]);
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

//Reconstruction position and initial energy of photon from shower
//uses energy to weight position
pair<double, pair<double, double> > reconstruct(std::vector<std::pair<double, int> > shower)
{
  double energy(0.), xPos(0.), yPos(0.);
  //looks at crystals in the shower
  for (VP2_iterator=shower.begin(); VP2_iterator!=shower.end(); ++VP2_iterator)
    {
      energy+= VP2_iterator->first;
      xPos+=IDtoX(VP2_iterator->second)*VP2_iterator->first; 
      yPos+=IDtoY(VP2_iterator->second)*VP2_iterator->first;
    }
  //takes weighted average
  xPos/=energy; yPos/=energy;
  pair<double, double> position(xPos*5.0, yPos*5.0);
  pair<double, pair<double, double> > photon(energy, position);
  return photon;
}

//Sorts energies from largest to smallest
//!!!!returns pairs of <energy, crystalID>
vector<pair<double, int> > energySort(vector<pair<int, double> > shower)
{
  std::vector<std::pair<double, int> > energy;
  for (VP_iterator=shower.begin(); VP_iterator!=shower.end(); VP_iterator++)
    {
      pair<double, int> flipped(VP_iterator->second, VP_iterator->first);
      energy.push_back(flipped);
    }
  std::map<double, int> myMap(energy.begin(), energy.end());
  map<double, int>::iterator m;
  vector<pair<double, int> > sorted;
  for (m=myMap.end(); m!=myMap.begin(); --m)
    {
     pair<double, int> orderHit(m->first, m->second);
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

void gaussFit(vector<pair<int, double> > shower, double thresEnergy)
 {
   vector<pair<int, double> > centers;
   double smallX(15.), smallY(15.), bigX(-15.), bigY(-15.);

   for (VP_iterator=shower.begin(); VP_iterator!=shower.end(); VP_iterator++)
     {
       if (VP_iterator->second>thresEnergy) {centers.push_back(*VP_iterator);}
       if (IDtoX(VP_iterator->first) < smallX) 
	 {smallX = IDtoX(VP_iterator->first);}
       if (IDtoY(VP_iterator->first) < smallY) 
	 {smallY = IDtoY(VP_iterator->first);}
       if (IDtoX(VP_iterator->first) > bigX) 
	 {bigX = IDtoX(VP_iterator->first);}
       if (IDtoY(VP_iterator->first) > bigY) 
	 {bigY = IDtoY(VP_iterator->first);}
     }
   cout << smallX  << " " << bigX << endl;
   cout << smallY << " " << bigY << endl;
   for (VP_iterator=centers.begin(); VP_iterator!=centers.end(); ++VP_iterator)
     {
       TF2* g2d = new TF2("g2D", Gaus2D, -5, 5, -5, 5, 6);
       g2d->SetParameters(IDtoX(VP_iterator->first),
			  IDtoY(VP_iterator->first), 
			  1, 1, 0, VP_iterator->first); //subject to change
       
       TH2D* histo = new TH2D("histo", "ShowerFit",
			      11, -5, 5,
			      11, -5, 5);

       TH2D* histo2 = new TH2D("histo2", "ShowerFit",
			      11, -5, 5,
			      11, -5, 5);

       vector<pair<int, double> >::iterator b; 
       for (b=shower.begin(); b!=shower.end(); b++)
	 {
	   histo->Fill(IDtoX(b->first), IDtoY(b->first), b->second);
	   histo2->Fill(IDtoX(b->first), IDtoY(b->first), b->second);
	 }
         
       TCanvas* canvas = new TCanvas("canvas", "canvas", 700,700);
       canvas->Divide(2,1);
       canvas->cd(); 
       histo->Draw("BOX");

       TCanvas* canvas2 = new TCanvas("canvas2", "canvas2", 700, 700);
       histo2->Fit(g2d);
       g2d->Draw("CONT1");
						
     }
			  

 }

 

void clusterDFSGauss() {

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

  double energyThresHi = 50.; //set energy threshold to start looking for bumps
  double energyThresLo = .5; //energy lower bound

  for (int k=0; k<nEvents; k++)
    {
      tree->GetEntry(k);

      vector<pair<int, double> > geant;
      vector<pair<int, double> > hitMap;

      for(int i=0; i<121; i++)
	{
	  pair<int, double> hit(i, addresses[i]);
	  geant.push_back(hit);
	  if (addresses[i] > energyThresHi)
	    { hitMap.push_back(hit);}
	}


      std::vector<std::vector<std::pair<int, double> > > clusters;

      for (VP_iterator=hitMap.begin(); VP_iterator!=hitMap.end(); VP_iterator++)
	{
	  std::vector<std::pair<int, double> > shower;
	  clusters.push_back(DFS(*VP_iterator, energyThresLo, shower, addresses));
	}


      vector<vector<pair<int, double> > > detector;
      for (VVP_iterator=clusters.begin(); VVP_iterator!=clusters.end(); ++VVP_iterator)
	{
	  if (VVP_iterator==clusters.begin()) {detector.push_back(*VVP_iterator);
	  }
	  else
	    {
	      if (!findVector(detector, *VVP_iterator))
		{detector.push_back(*VVP_iterator);}
	    }
	}
      
      for (VVP_iterator=detector.begin(); VVP_iterator!=detector.end(); 
	   ++VVP_iterator)
	{
	  gaussFit(*VVP_iterator, 50.);
  
	}
      


    } //end of event
 
}
 

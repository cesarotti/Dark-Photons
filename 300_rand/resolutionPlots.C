#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TRandom3.h"
#include <iostream>
#include <sstream>

/*
 * THINGS TO CHECK BEFORE YOU RUN
 * 1. Energy
 * 2. Position
 * 3. Binning
 * 4. Title
 */



double CRYStoMM = 50.;
vector<vector<pair<int, double> > >::iterator vv_iter;
vector<pair<int, double> >::iterator v_iter;

// Changes x and y coordinates in crystal units to the crystal ID
int CoordtoID(int x, int y)
{
  return(y+17)*35+(x+17);
}
 

// Gets x position from crystalID
int IDtoX(int crystalID)
{
  return crystalID%35-17;
}


//Gets y position from crystalID
int IDtoY(int crystalID)
{
  return crystalID/35-17;
}

// Returns distance between two crystals as defined by minimal path
int dist(int crysFID, int crysSID)
{
  return TMath::Max(TMath::Abs(IDtoX(crysFID)-IDtoX(crysSID)), 
		    TMath::Abs(IDtoY(crysFID)-IDtoY(crysSID)));
}


int crystalNumOptimized(vector<pair<int, double> > *shower)
{
  double energySum(0.);
  double next(0.);
  int n(0);
  vector<pair<int, double> >::iterator a;
  for (a=shower->begin(); a!=shower->end(); ++a)
    {
      if (n<1) {n++; energySum+= a->second;}
      else
	{
	  next+=a->second;
	  if (energySum/next < .5/a->second)
	    {return n;}
	  else
	    {energySum = next;
	     n++;
	     }
	  
	}
    }
  return n;
}


// Returns the total energy in all crystals of a shower
double clusterDep(vector<pair<int, double> > *shower)
{
  double totEnergy(0.);
  for (v_iter=shower->begin(); v_iter!=shower->end(); v_iter++)
    {totEnergy+=v_iter->second;}
  return totEnergy;
}

//checks if a shower is already in the cluster
// ROOT really hates this method.
bool findVector (vector<vector<pair<int, double> > > *detector,
		vector<pair<int, double> > shower)
{
  // Look through detector
  vector<vector<pair<int, double> > >::iterator a; 
  for (a=detector->begin(); a!=detector->end(); ++a)
    { // remember that crystal ID's are ordered, and touching clusters have
      // the same crystalIDs
        if ((a->front()).first==(shower.front()).first) return true;
    }
      return false;
}

bool findPair (vector<vector<pair<int, double> > > *detector)
{
  vector<vector<pair<int, double> > >::iterator a;
  vector<vector<pair<int, double> > >::iterator b;
  vector<pair<int, double> >::iterator c;
  vector<pair<int, double> >::iterator d;
  for (a=detector->begin(); a!=detector->end()-1; a++)
    {
	for (b=a+1; b!=detector->end(); b++)
	  {
	    for (c=a->begin(); c!=a->end(); c++)
	      {for (d=b->begin(); d!=b->end(); d++)
		  if (c->first==d->first) {return true;}
	      }
	  }
      
    }
  return false;
}


pair<double, pair<double, double> > reconstruct(vector<pair<int, double> > shower)
{
  double energy(0.), xPos(0.), yPos(0.);
  //looks at crystals in the shower
  for (v_iter=shower.begin(); v_iter!=shower.end(); ++v_iter)
    {
      energy+= v_iter->second;
      xPos+=IDtoX(v_iter->first)*v_iter->second; 
      yPos+=IDtoY(v_iter->first)*v_iter->second;
    }
  //takes weighted average
  xPos/=energy; yPos/=energy;
  pair<double, double> position(xPos, yPos);
  pair<double, pair<double, double> > photon(energy, position);
  return photon;
}


vector<pair<int, double> > generateBumpMap(double bumpEnergy, double address[],
					   vector<pair<int, double> > shower)
{
  vector<pair<int, double> > hitMap;
  int ID(0.);
  for (v_iter=shower.begin(); v_iter!=shower.end(); v_iter++)
    {
      if (v_iter->second > bumpEnergy) {
	int counter(0);
	ID = v_iter->first;
	  for (int x=-1; x<2; x++) {
	    for (int y=-1; y<2; y++) {
	      
	      int ngbrID = CoordtoID(IDtoX(ID)+x, IDtoY(ID)+y);
	      if (address[ID] > address[ngbrID])
		{ counter++;}
	      else 
		{}
	    }
	  }
	  if (counter ==8) {hitMap.push_back(*v_iter);}	      
      }
    }
  return hitMap;
}


pair<int, double> reconstructID (vector<pair<int, double> > shower)
{
  pair<double, pair<double, double> > photon = reconstruct(shower);
  int xVal = (int) photon.second.first+.5;
  int yVal = (int) photon.second.second+.5;
  pair<int, double> reconstructed(CoordtoID(xVal, yVal), photon.first);
  return reconstructed;
}

//Sorts energies from largest to smallest
vector<pair<int, double> > energySort(vector<pair<int, double> > shower)
{
  vector<pair<double, int> > energy;
  for (v_iter=shower.begin(); v_iter!=shower.end(); v_iter++)
    {
      pair<double, int> flipped(v_iter->second, v_iter->first);
      energy.push_back(flipped);
    }
  std::map<double, int> myMap(energy.begin(), energy.end()+1);
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

vector<pair<int,double> > *  DFS(pair<int, double> start, double energyThreshLo,       vector<pair<int, double> > * shower,
				 double address[])
{
  shower->push_back(start);
  for (int x=-1; x<2; x++) {
    for (int y=-1; y<2; y++) {
      int ngbrID = CoordtoID(IDtoX(start.first)+x, IDtoY(start.first)+y);
      double ngbrEn = address[ngbrID];
      pair<int, double> ngbr(ngbrID, ngbrEn);
      if (ngbrEn>energyThreshLo)
	{
	  vector<int> showerID; 
	  //no method for searching pairs
	  for (int f=0; f<shower->size(); f++) {showerID.push_back(((*shower)[f]).first);}
	  if (std::find(showerID.begin(), showerID.end(), ngbrID)!=showerID.end()) 
	    {continue;}
	  // if it has enough energy and has not been counted
	  else { shower = DFS(ngbr, energyThreshLo, shower,address);}
	}
    }
  }
  //put crystals in correct order to make other methods simpler
  std::sort(shower->begin(), shower->end());
  return shower;
}


void resolutionPlots() 
{ 

  cout << "Starting plots..." << endl;

  TRandom3* randomGen = new TRandom3(12191982);

  TFile* file = new TFile("complete.root");

  TTree* tree = (TTree *)file->Get("Signal");

  TTree* tree2 = (TTree *)file->Get("Check");
  double ENERGY = 0;
  double XPOS = 0; 
  double YPOS = 0;
  tree2->SetBranchAddress("Energy", &ENERGY);
  tree2->SetBranchAddress("PosX", &XPOS);
  tree2->SetBranchAddress("PosY", &YPOS);


  int nEvents = tree->GetEntries();

   double addresses[1225] = {};
  for (int k=0; k<1225; k++){
    std::stringstream ss2;
    ss2 << k; 
    string str = "Crystal_"+ss2.str();
    const char* charstr = str.c_str(); 
    tree->SetBranchAddress(charstr, &addresses[k]);
  }

  double b1, b2;
  double energyThreshHi = 5.; 
  double energyThreshLo = 0.01;
  TH1D* energyReso = new TH1D("energyReso", "Energy_resolution",  100, -150, 50);
  TH1D* posResoX = new TH1D("posResoX", "XPosition_resolution", 80, -40, 39); 
  TH1D* posResoY = new TH1D("posResoY", "YPosition_resolution", 80, -40, 39); 


  //iterate through all events
  for (int i = 0; i < nEvents; i++)
    {
      tree2->GetEntry(i);
      tree->GetEntry(i);
      vector<pair<int, double> > geant; //stores all geant data
      vector<pair<int, double> > hitMap; //stores all hits above threshold


      for(int w = 0; w < 1225; w++)
	{
	  pair<int, double> hit(w, addresses[w]);
	  geant.push_back(hit);
	  if (addresses[w] > energyThreshHi)
	    { hitMap.push_back(hit);}
	}

      vector<vector<pair<int, double> > > clusters;

      for (v_iter=hitMap.begin(); v_iter!=hitMap.end(); v_iter++)
	{
	  vector<pair<int, double> > shower;
	  clusters.push_back(*DFS(*v_iter, 
				  energyThreshLo, 
				  &shower, 
				  addresses));
	}

      vector<vector<pair<int, double> > > detector; 
      for (vv_iter=clusters.begin(); vv_iter!=clusters.end(); ++vv_iter)
	{
	  if (vv_iter==clusters.begin())
	    {detector.push_back(*vv_iter);}
	  else
	    {
	      if (!findVector(&detector, *vv_iter))
		{detector.push_back(*vv_iter);}
	    }
	}

      //unclustering
      
      vector<vector<pair<int, double> > > detector2;

      for (vv_iter=detector.begin(); vv_iter!=detector.end(); vv_iter++)
	{
	  vector<pair<int, double> > localMax; 
	  localMax = generateBumpMap(energyThreshHi, addresses, *vv_iter);

	  if (localMax.size()==0) {continue; } 
	  //First Case: only one bump, treat as one photon.
	  if (localMax.size() ==1)
	    {detector2.push_back(*vv_iter);
	    continue;}
	  pair<int, double> coe = reconstructID(*vv_iter);
	  
	  localMax = energySort(localMax);
	 

	  //Second Case: many bumps, but centered logically, treat as one photon.

	  if (false)
	    {detector2.push_back(*vv_iter);
	      continue;}
	 

	  
	  //Hopefully optimized for a two pronged event
	  else
	    {
	      for (int q=0; q<2; q++)
		{
		  vector<pair<int, double> > newShower;
		  int ind = (q+1)%2;
		  b1 = localMax[q].second; 
		  b2 = localMax[ind].second;
		  vector<pair<int, double> >::iterator a;
		  for (a=vv_iter->begin(); a!=vv_iter->end(); ++a)
		    {
		      double energy(0.);
		      int d1 = dist(localMax[q].first, a->first);
		      int d2 = dist(localMax[ind].first, a->first);
		      energy = a->second*b1*pow(.1, d1-1)/(b1*pow(.1, d1-1)+b2*pow(.1, d2-1));
		      pair<int, double> newHit(a->first, energy);
		      newShower.push_back(newHit);
		    }

		  
		}
	      
	    }
	}

      vector<vector<pair<int, double> > > ordered; 
      int num(0);
      
      for (vv_iter=detector2.begin(); vv_iter!=detector2.end(); ++vv_iter)
	{
	  vector<pair<int, double> > shower = energySort(*vv_iter);
	  num = crystalNumOptimized(&shower);
	  if (shower.size()>num)
	    {shower.erase(shower.begin()+num, shower.end());}
	  ordered.push_back(shower);
	}

      pair<double, pair<double, double> > photon;
      for (vv_iter=ordered.begin(); vv_iter!=ordered.end(); ++vv_iter)
	{
	  photon = reconstruct(*vv_iter);
	  energyReso->Fill(photon.first-ENERGY);
	  posResoX->Fill(photon.second.first*CRYStoMM-XPOS);
	  posResoY->Fill(photon.second.second*CRYStoMM-YPOS);
	}
    }

  energyReso->GetXaxis()->SetTitle("Energy Resolution:= (measured-expected) in MeV");
  posResoX->GetXaxis()->SetTitle("Position Resolution:=(measured-expected) in mm");
  posResoY->GetXaxis()->SetTitle("Position Resolution:=(measured-expected) in mm");

  if (XPOS > 13.9)
    {
      energyReso->SetTitle("Energy Resolution (Center)");
      posResoX->SetTitle("X Position Resolution (Center)");
      posResoY->SetTitle("Y Position Resolution (Center)");
    }
  else if (YPOS>13.4)
    {
      energyReso->SetTitle("Energy Resolution (Corner)");
      posResoX->SetTitle("X Position Resolution (Corner)");
      posResoY->SetTitle("Y Position Resolution (Corner)");
    }
  else
    {
      energyReso->SetTitle("Energy Resolution (Side)");
      posResoX->SetTitle("X Position Resolution (Side)");
      posResoY->SetTitle("Y Position Resolution (Side)");
    }

  TCanvas* canvas = new TCanvas("canvas", "canvas", 1000, 500);
  canvas->Divide(3,1);
  canvas->cd(1); energyReso->Draw();
  canvas->cd(2); posResoX->Draw();
  canvas->cd(3); posResoY->Draw();
  
}

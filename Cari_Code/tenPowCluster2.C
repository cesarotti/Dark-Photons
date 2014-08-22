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
 *
 * The unclustering is using the power of ten related to the distance
 * between the current energy deposition and the considered bump
 *
 * It then only considers a set number of crystals to sum over when computing
 * total energy and position
 */

double gammaEnergy = 350.;
vector<pair<int, double> >::iterator VP_iterator; 
vector<vector<pair<int, double> > >::iterator VVP_iterator;


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
vector<pair<int, double> > generateBumpMap(double bumpEnergy, double address[],
					   vector<pair<int, double> > shower)
{
  vector<pair<int, double> > hitMap;
  int ID(0.);
  for (VP_iterator=shower.begin(); VP_iterator!=shower.end(); VP_iterator++)
    {
      if (VP_iterator->second > bumpEnergy) {
	int counter(0);
	ID = VP_iterator->first;
	  for (int x=-1; x<2; x++) {
	    for (int y=-1; y<2; y++) {
	      
	      int ngbrID = CoordtoID(IDtoX(ID)+x, IDtoY(ID)+y);
	      if (address[ID] > address[ngbrID])
		{ counter++;}
	      else 
		{}
	    }
	  }
	  if (counter ==8) {hitMap.push_back(*VP_iterator);}	      
      }
    }
  return hitMap;
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
  std::vector<std::pair<double, int> > energy;
  for (VP_iterator=shower.begin(); VP_iterator!=shower.end(); VP_iterator++)
    {
      pair<double, int> flipped(VP_iterator->second, VP_iterator->first);
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

// Searchs all neighbors of a crystal with nontrivial energy deposit until all
// neighbors are accounted for or have negligible energy deposition
vector<pair<int,double> >  DFS(pair<int, double> start, double energyThreshLo, 
						       vector<pair<int, double> > shower,  double map[])
{
  shower.push_back(start);
  for (int x=-1; x<2; x++) {
    for (int y=-1; y<2; y++) {
      int ngbrID = CoordtoID(IDtoX(start.first)+x, IDtoY(start.first)+y);
      double ngbrEn = map[ngbrID];
      pair<int, double> ngbr(ngbrID, ngbrEn);
      if (ngbrEn>energyThreshLo)
	{
	  vector<int> showerID; 
	  //no method for searching pairs
	  for (int f=0; f<shower.size(); f++) {showerID.push_back((shower[f]).first);}
	  if (std::find(showerID.begin(), showerID.end(), ngbrID)!=showerID.end()) 
	    {continue;}
	  // if it has enough energy and has not been counted
	  else { shower = DFS(ngbr, energyThreshLo, shower,map);}
	}
    }
  }
  //put crystals in correct order to make other methods simpler
  std::sort(shower.begin(), shower.end());
  return shower;
}


int crystalNumOptimized(vector<pair<int, double> > shower)
{
  double energySum(0.);
  double next(0.);
  int n(0);
  vector<pair<int, double> >::iterator a;
  for (a=shower.begin(); a!=shower.end(); ++a)
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
 

void tenPowCluster2() {

  TFile* file = new TFile("complete.root");

  TTree* tree = (TTree *)file->Get("Signal");

  int nEvents = tree->GetEntries();

  double addresses[1225] = {};
  for (int k=0; k<1225; k++){
    std::stringstream ss2;
    ss2 << k; 
    string str = "Crystal_"+ss2.str();
    const char* charstr = str.c_str(); 
    tree->SetBranchAddress(charstr, &addresses[k]);
  }

  double energyThresHi = 5.; //set energy threshold to start looking for bumps
  double energyThresLo = 0.2; //energy lower bound

  TH2D* gammaH = new TH2D("gammaH", "Photon_Pos_Energy", 36, -17.5, 17.5, 36, -17.5, 17.5);

  TRandom3* rand = new TRandom3(12191982);
  double b1, b2;

  //Going through each scan of the calorimeter
  for (int k=0; k<nEvents; k++)
    {
      tree->GetEntry(k);

      vector<pair<int, double> > geant; // stores crystal and energy tuples
      vector<pair<int, double> > hitMap; // stores crystals with large edep
     
    

      for(int i=0; i<1225; i++)
	{double noise = rand->Gaus(0, .5); //noise
	  pair<int, double> hit(i, addresses[i]+noise);
	  geant.push_back(hit);
	  if (addresses[i] > energyThresHi)
	    { hitMap.push_back(hit);}
	}
      // hitMap now stores the <ID, energy> of each crystal with energy 
      // over a certain threshold, geant now contains all crystal information


      //clusters will store all DFS results about the entries in hitMap
      std::vector<std::vector<std::pair<int, double> > > clusters;
      

      for (VP_iterator=hitMap.begin(); VP_iterator!=hitMap.end(); VP_iterator++)
	{
	  std::vector<std::pair<int, double> > shower;
	  clusters.push_back(DFS(*VP_iterator,
				 energyThresLo,
				 shower, addresses));
	}
      // clusters now stores all clusters with energy bumps from hitMap.
      // duplicates may exist.


      //detector will store the results in cluster with no repeats
      vector<vector<pair<int, double> > > detector;
      for (VVP_iterator=clusters.begin(); VVP_iterator!=clusters.end(); ++VVP_iterator)
	{
	  if (VVP_iterator==clusters.begin()) 
	    {detector.push_back(*VVP_iterator);}
	  else
	    {
	      if (!findVector(detector, *VVP_iterator))
		{detector.push_back(*VVP_iterator);}
	    }
	}

      //Unclustering. 
      vector<vector<pair<int, double> > > detector2;      

      for (VVP_iterator=detector.begin(); VVP_iterator!=detector.end();
	   ++VVP_iterator)
	{
	  vector<pair<int, double> > localMax;
	  localMax = generateBumpMap(energyThresHi, addresses, (*VVP_iterator));

	  //First Case: only one bump, treat as one photon.
	  if (localMax.size() ==1)
	    {detector2.push_back(*VVP_iterator);
	    continue;}
	  pair<int, double> coe = reconstructID(*VVP_iterator);
	  cout << "Center of energy ID: " << coe.first << endl;
	  cout << "Total energy: " << coe.second << endl;
	  
	  vector<pair<int, double> >::iterator q;
	  localMax = energySort(localMax);
	  cout << localMax[0].second << endl;
	 

	  //Second Case: many bumps, but centered logically, treat as one photon.

	  if (false)// (dist(localMax[0].first, coe.first) < 2 && localMax[0].second > .4*coe.second) {}
	  
	    {detector2.push_back(*VVP_iterator);
	      continue;}
	 

	  
	  //Hopefully optimized for a two pronged event
	  else
	    {
	      cout << "Two Prong Cluster" << endl;
	      for (int i=0; i<2; ++i)
		{
		  vector<pair<int, double> > newShower;
		  int ind = (i+1)%2;
		  b1 = localMax[i].second; b2 = localMax[ind].second;
		  vector<pair<int, double> >::iterator a;
		  for (a=VVP_iterator->begin(); a!=VVP_iterator->end(); ++a)
		    {
		      double energy(0.);
		      int d1 = dist(localMax[i].first, a->first);
		      int d2 = dist(localMax[ind].first, a->first);
		      energy = a->second*b1*pow(.1, d1-1)/(b1*pow(.1, d1-1)+b2*pow(.1, d2-1));
		      cout << "Energy : " << energy << endl; 
		      pair<int, double> newHit(a->first, energy);
		      newShower.push_back(newHit);
		    }

		  detector2.push_back(newShower);
	      	       
		}
    
	    }
	}

      
      //detector2 now holds all clusters with bumps in hitMap. No duplicates.
      
      vector<vector<pair<int, double> > > ordered;
      int num(0.);
	
      for (VVP_iterator=detector2.begin(); VVP_iterator!=detector2.end(); ++VVP_iterator)
	{ 
	  vector<pair<int, double> > shower =  energySort(*VVP_iterator);
	  num = crystalNumOptimized(shower);
	  if (shower.size()>num)
	    {shower.erase(shower.begin()+num, shower.end());}
	  ordered.push_back(shower);
	  vector<pair<int, double> >::iterator c;
	  for (c=shower.begin(); c!=shower.end(); ++c)
	    {
	      cout << "energy: " << c->second << endl;
	      cout << "X Position: " << IDtoX(c->first) << endl;
	      cout << "Y Position: " << IDtoY(c->first) << endl;
	      gammaH->Fill(IDtoX(c->first), IDtoY(c->first), c->second);
	    }
	  
	} 

      // checks if several hits are in the same 
      

      for (VVP_iterator=ordered.begin(); VVP_iterator!=ordered.end();
	   ++VVP_iterator)
	{
	  cout << "size of shower: " << VVP_iterator->size() << endl;
	  cout << "Photon Energy: " << reconstruct(*VVP_iterator).first
	  << endl;
	  cout << "Position in crystal units: (" << reconstruct(*VVP_iterator).second.first
	       << ", " << reconstruct(*VVP_iterator).second.second << ")" << endl;
	}


    } //end of event

 TCanvas* canvas = new TCanvas("canvas", "canvas", 700,700);
 canvas->cd(); gammaH->Draw("BOX");

}

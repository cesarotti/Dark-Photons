#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include <iostream>
#include "TCanvas.h"
#include <sstream>

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

//Reconstruction position and initial energy of photon from shower
//uses energy to weight position
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
  std::sort(energy.begin(), energy.end());
  return energy;
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

/*
 * This method takes a number of showers such that there are no duplicates. 
 * The method then sorts through the clusters and checks if any of the clusters
 * have several bumps. If the bumps are right next to each other, it is taken
 * to be from the same photon. Otherwise, the energy is split by ratio
 * of the bumps.
 */
std::vector<std::vector<std::pair<int, double> > > uncluster( std::vector<std::vector<std::pair<int, double> > > clusters, vector<pair<int, double> > hitMap)
{

  vector<vector<pair<int, double> > > unclustered; 
  for (VVP_iterator=clusters.begin(); VVP_iterator!=clusters.end(); ++VVP_iterator)
    {
      cout << "Cluster size: " << VVP_iterator->size() << endl;
      vector<pair<int, double> > crysIDs; 
      for (VP_iterator=VVP_iterator->begin(); VP_iterator!=VVP_iterator->end(); ++VP_iterator)
	{
	  if (std::find(hitMap.begin(), hitMap.end(), *VP_iterator)!=hitMap.end())
	    {
	      crysIDs.push_back(*VP_iterator);
	    }
	}
      if (crysIDs.size()==1) {unclustered.push_back(*VVP_iterator);} 
      else {
	int p(0); 
	while (p<crysIDs.size())
	  { int q=p+1;
	    while ( q<crysIDs.size())
	      {
		if (dist(crysIDs[p].first, crysIDs[q].first) < 2)
		  {if (crysIDs[p].second > crysIDs[q].second)
		      {crysIDs.erase(crysIDs.begin()+q, crysIDs.begin()+q+1);}
		    else
		      {crysIDs.erase(crysIDs.begin()+p, crysIDs.begin()+p+1);
			p--;}
		  }
		else {q++;}
	      }
	    p++;
	    
      }

	double energyTot = clusterDep(crysIDs);
	for (VP_iterator=crysIDs.begin(); VP_iterator!=crysIDs.end(); ++VP_iterator)
	  {
	    vector<pair<int, double> > shower;
	    std::vector<pair<int, double> >::iterator a; 
	    for(a=VVP_iterator->begin(); a!=VVP_iterator->end(); ++a)
	      {
	        pair<int, double> hit(a->first, (a->second)*(VP_iterator->second)/energyTot);
		shower.push_back(hit);
	      }
	    unclustered.push_back(shower);
	    cout << "shower size " <<  shower.size() << endl;
	  }
      }
    
    }
  return unclustered;

   }


void clusterDFSv2() {

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

  double energyThresHi = 50.; //set energy threshold to start looking for bumps
  double energyThresLo = .5; //energy lower bound

  TH2D* gammaH = new TH2D("gammaH", "Photon_Pos_Energy", 35, -17, 17, 35, -17, 17);

  for (int k=0; k<nEvents; k++)
    {
      tree->GetEntry(k);

      vector<pair<int, double> > geant;
      vector<pair<int, double> > hitMap;

      for(int i=0; i<1225; i++)
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

      detector = uncluster(detector, hitMap);
      cout << "Detector Size: " << detector.size() << endl;
	
      for (VVP_iterator=detector.begin(); VVP_iterator!=detector.end(); ++VVP_iterator)
	{ 
       
	  for (VP_iterator=VVP_iterator->begin(); VP_iterator!=VVP_iterator->end(); ++VP_iterator)
	    { 
	      gammaH->Fill(IDtoX(VP_iterator->first), IDtoY(VP_iterator->first), VP_iterator->second);
	    }
	  
	}
      
      for (VVP_iterator=detector.begin(); VVP_iterator!=detector.end(); ++VVP_iterator)
	{
	  cout << "Photon energy: " << reconstruct(*VVP_iterator).first
	       << " MeV" << endl;
	  cout << "Photon position: ("
	       << reconstruct(*VVP_iterator).second.first << ", " 
	       << reconstruct(*VVP_iterator).second.second << ")" << endl;
	}

	


    } //end of event

 TCanvas* canvas = new TCanvas("canvas", "canvas", 700,700);
 canvas->cd(); 
 gammaH->Draw("BOX");

}
 

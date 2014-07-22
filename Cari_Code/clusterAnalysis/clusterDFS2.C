#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include <iostream>
#include "TCanvas.h"
#include <sstream>

std::vector<int>::iterator vi_iter;
std::vector<vector<int> >::iterator vvi_iter;

//Changes from cartesian coordinates to crystal ID
int CoordtoID(int x, int y)
{
  return (y+17)*35+(x+17); 
}

//Gets x position from ID
int IDtoX(int id)
{
  return id%35-17;
}

//Gets Y position from ID
int IDtoY(int id)
{
  return id/35-17;
}

int dist(int x, int y)
{
  return TMath::Max(TMath::Abs(IDtoX(x)-IDtoX(y)),TMath::Abs(IDtoY(x)-IDtoY(y)));
 }

double clusterDep(vector<int> shower, double map[])
{
  double totEnergy(0.);
  for (vi_iter=shower.begin(); vi_iter!=shower.end(); ++vi_iter)
    {totEnergy+=map[*vi_iter];}
  return totEnergy;
}


bool findVector(vector<vector<int> > detector, vector<int> shower)
{ std::vector<vector<int> >::iterator a;
  for (a=detector.begin(); a!=detector.end(); ++a)
    {if ((*a).front()==shower.front()) return true; }
  return false;
}

pair<double,pair<double, double> > reconstruct(vector<int> shower, double map[])
{
  double energy(0.), xPos(0.), yPos(0.); 
  for (vi_iter=shower.begin(); vi_iter!=shower.end(); vi_iter++)
    {
      energy+=map[*vi_iter];
      xPos+=IDtoX(*vi_iter)*map[*vi_iter]; yPos+=IDtoY(*vi_iter)*map[*vi_iter];
    }
  xPos/=energy; yPos/=energy; //weighted position
  pair<double, double> position(xPos*5.0, yPos*5.0);
  pair<double, pair<double, double> >photon(energy, position);
  return photon;
}

//Sorts in reverse order
bool comp(int i, int j)
{return (i>j);}

/*
 * Sorts showers by returning energy depositions in decreasing order
 */
std::vector<int> energySort(std::vector<int> shower, double map[])
{ std::vector<int> energies;
  for (vi_iter=shower.begin(); vi_iter!=shower.end(); vi_iter++)
    {
      energies.push_back(map[*vi_iter]); 
    }
  std::sort(energies.begin(), energies.end(), comp);
  return energies;
  
}

std::vector<int>  DFS(int startID, double energyThresh, double map[],
	  std::vector<int> shower)
{
  shower.push_back(startID);
  for (int x=-1; x<2; x++) {
    for (int y=-1; y<2; y++) {
      int ngbrID = CoordtoID(IDtoX(startID)+x,IDtoY(startID)+y);
      if (map[ngbrID]> energyThresh)
	{if (std::find(shower.begin(), shower.end(), ngbrID)!= shower.end())
	    continue;
	  else {shower = DFS(ngbrID, energyThresh, map, shower);}
	}
    }
  }

  std::sort(shower.begin(), shower.end()); //easy to compare showers
  return shower;
}

/* Brian's code for maximizing electrical signal
 */
double numCrystals(double totEnergy)
{if (totEnergy<15.) return 4.;
  else {
    return 13.44+(log(totEnergy)*2.56);
  }
}
  
vector<vector<int> > uncluster(double map[], double hitmap[],vector<vector<int> > clusters)
{ 
  vector<vector<int> > unclustered; 

  //Looks through all of the clusters
  for (vvi_iter=clusters.begin(); vvi_iter!=clusters.end(); vvi_iter++)
    {
      vector<int> crysIDs; //stores crystals with high energy depositions
      //Looks through a particular cluster
      for (vi_iter=vvi_iter->begin(); vi_iter!=vvi_iter->end(); vi_iter++)
	{
	  //Stores crystals with high energy depositions
	  if (hitmap[*vi_iter]>0) {crysIDs.push_back(*vi_iter);} 
	}

      // Only one bump ~ only one photon
      if (crysIDs.size()==1) {unclustered.push_back(*vvi_iter);}

      else {
	double energyTot = clusterDep(crysIDs, map);
	for (vi_iter=crysIDs.begin(); vi_iter!=crysIDs.end(); vi_iter++)
	  { 
	    vector<int> shower;
	    std::vector<int>::iterator a;
	    for (a=vvi_iter->begin(); a!=vvi_iter->end(); a++)
	      { shower.push_back((map[*vi_iter])*(map[*a])/energyTot);
	      }
	      
	    unclustered.push_back(shower); 
	  }
      }
    }

  return unclustered;
}



void clusterDFS() {
  
  TFile* file = new TFile("complete.root");

  TTree* tree = (TTree *)file->Get("Signal");

  int nEvents = tree->GetEntries(); 

  double addresses[1225] = {};
  for (int k=0; k<1225; k++){
    std::stringstream ss2; 
    ss2 << k; 
    string str = "Crystal_" + ss2.str();
    const char* charstr = str.c_str();
    tree->SetBranchAddress(charstr, &addresses[k]);
  }

 double energyThresHi = 50.; //set energy threshold to start looking for bumps

 double energyThresLo = .5; //low energy threshold

 
 TH2D* gammaH = new TH2D("gammaH", "Photon_Pos_&_Energy", 35,-17,17,35,-17,17);

 

 int hitMap[1225] = {};

 vector<int> crystalHits;

 for (int k=0; k<nEvents; k++)
   { 
     tree->GetEntry(k);
     for (int f=0; f<1225; f++)
       {
     if (addresses[f]>energyThresHi)
       {hitMap[f]+=1; crystalHits.push_back(f);} 
       } // closes hit map

     int p(0);
     while (p<crystalHits.size())
       {
	 int q=p+1; 
	 while (q<crystalHits.size())
	   { if (dist(crystalHits[p], crystalHits[q]) < 2)
	       { if (addresses[q] < addresses[p])
		   {crystalHits.erase(crystalHits.begin()+q, crystalHits.begin()+q+1);}
		 else 
		   {crystalHits.erase(crystalHits.begin()+p, crystalHits.begin()+p+1); 
		     p--;}
	       }
	     else {q++;}
	   }
	 p++;
       }

     for (vi_iter=crystalHits.begin(); vi_iter!=crystalHits.end(); vi_iter++)
       {
	 cout << *vi_iter << endl;
       }

     std::vector<std::vector<int> > clusters;


     for (int q=0; q<1225; q++)
       {if (hitMap[q]>0)
	   { std::vector<int> shower;
	     clusters.push_back(DFS(q, energyThresLo, addresses, shower));
	   } // closes looking at a bump
       } // closes loop through all crystals for bumps
 

     //Builds vector of vectors of energy that holds no repeat vectors
 vector<vector<int> > detector;
 for (vvi_iter=clusters.begin(); vvi_iter!=clusters.end(); ++vvi_iter)
   { if (vvi_iter==clusters.begin()) {detector.push_back(*vvi_iter);}
     else 
       { 
	 if (!findVector(detector, *vvi_iter)){detector.push_back(*vvi_iter);}
       }
   }

 //Fills histogram 
 for(vvi_iter=detector.begin(); vvi_iter!=detector.end(); ++vvi_iter)
   { 
     cout << "Photon energy: " << reconstruct(*vvi_iter, addresses).first
	  << endl;
     cout << "Photon position: " <<
       reconstruct(*vvi_iter, addresses).second.first << ", " <<
       reconstruct(*vvi_iter, addresses).second.second<< endl;

     for (vi_iter =vvi_iter->begin(); vi_iter!=vvi_iter->end(); ++vi_iter)
       {
	 gammaH->Fill(IDtoX(*vi_iter), IDtoY(*vi_iter), addresses[*vi_iter]);
       }
   }
 
 
   }
 

TCanvas* canvas = new TCanvas("canvas", "canvas", 700,700);
canvas->cd();
gammaH->Draw("BOX");


}
    
   






 





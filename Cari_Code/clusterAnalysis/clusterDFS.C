#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include <iostream>
#include "TCanvas.h"
#include <sstream>

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

std::vector<int>  DFS(int startID, double energyThresh, double map[],
	  std::vector<int> shower)
{
  shower.push_back(startID);
  for (int x=-1; x<2; x++) {
    for (int y=-1; y<2; y++) {
      int ngbrID = CoordtoID(IDtoX(startID)+x,IDtoY(startID)+y);
      if (map[ngbrID]> energyThresh)
	{if (std::find(shower.begin(), shower.end(), ngbrID)!= shower.end())
	    return shower;
	 else
	   shower = DFS(ngbrID, energyThresh, map, shower);
	}
    }
  }

return shower;
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

 /*

 TH2D* gammaH = new TH2D("gammaH", "Photon_Pos_&_Energy", 35,-17,17,35,-17,17);

 TH1D* sumEnergyH = new TH1D("effectH", "Effectiveness_per_hit", 500, 0, 1);
 */

 int hitMap[1225] = {};



 for (int k=0; k<nEvents; k++)
   { 
     tree->GetEntry(k);
     for (int f=0; f<1225; f++)
       {
     if (addresses[f]>energyThresHi)
       {hitMap[f]+=1; cout << f << endl;} //shows pile up between events, too
       }

     std::vector<std::vector<int> > clusters;


 for (int q=0; q<1225; q++)
   {if (hitMap[q]>0)
       { std::vector<int> shower;
	 clusters.push_back(DFS(q, energyThresLo, addresses, shower));
       }
   }
 
 double totalEnergy(0.);
 std::vector<std::vector<int> >::iterator a;
 std::vector<int>::iterator b;
 for (a=clusters.begin(); a!=clusters.end(); ++a)
   {
     for (b=(*a).begin(); b!=(*a).end(); ++b)
     {cout << *b << endl;
       totalEnergy+=addresses[*b];}
     
   } 
 cout << totalEnergy << endl;
 
   }
 
 /*

TCanvas* canvas = new TCanvas("canvas", "canvas", 700,700);
 canvas->Divide(2,1);

canvas->cd(1);
gammaH->Draw("BOX");
 canvas->cd(2); 
 sumEnergyH->Draw();
 */

   
}






 





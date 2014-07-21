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

int distance(int id1, int id2)
{
  return Max(Abs(IDtoX(id1)-IDtoX(id2)), Abs(IDtoY(id1)-IDtoY(id2)));
}

void cluster9cube() {

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

 int hit(-1);

 TTree* tree2 = (TTree *)file->Get("Signal2");

 tree2->SetBranchAddress("Hits", &hit);

 const int nEvents2 = tree2->GetEntries();

 TH2D* gammaH = new TH2D("gammaH", "Photon_Pos_&_Energy", 35,-17,17,35,-17,17);

 TH1D* sumEnergyH = new TH1D("effectH", "Energy_Entirety_per_hit", 500, 0, 1);

 int hitMap[1225] = {};

 for (int k=0; k<nEvents2; k++)
   {
     tree2->GetEntry(k); 
     if (hit>0) {hitMap[hit]+=1;} //shows pile up between events too
   }

   for (int i=0; i<nEvents; i++)
     {
       double energySpread[9] = {};
       int crysID[9] = {};
       tree->GetEntry(i);
       for (int j=0; j<1225; j++) 
	 {
	   if (hitMap[j] > 0)
	     { int x = IDtoX(j);
	       int y = IDtoY(j);
	       if (addresses[j] >0)
		 {
		   //breadth first search
		   for (int q=-1; q<2; q++)
		     {for (int f=-1; f<2; f++)
			 {energySpread[(f+1)+3*(q+1)]=
			     addresses[CoordtoID(x+f, y+q)];
			   gammaH->
			     Fill(x+f,y+q, addresses[CoordtoID(x+f,y+q)]);
			   crysID[(f+1)+3*(q+1)] =
						 CoordtoID(x+f,y+q);
			 }

		     }
		   double energyTot(0.);
		   int a=0;
		   while(a<9) {energyTot+=energySpread[a]; a++;};
		   sumEnergyH->Fill((energyTot/350.));

		 }
	     }
	 }
     }

TCanvas* canvas = new TCanvas("canvas", "canvas", 700,700);
 canvas->Divide(2,1);

canvas->cd(1);
gammaH->Draw("BOX");
 canvas->cd(2); 
 sumEnergyH->Draw();


}







 





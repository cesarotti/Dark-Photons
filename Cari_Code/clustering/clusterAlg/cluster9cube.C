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

void cluster9cube() {

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

 TH2D* gammaH = new TH2D("gammaH", "Photon_Pos_&_Energy", 35,-17,17,35,-17,17);

 double energyThresHi = 50.;
 double energyThresLow = .5;

 int hitMap[1225] = {};

 for (int w=0; w<nEvents; w++)
   {
     tree->GetEntry(w); 
     for (int z=0; z<1225; z++)  
       { 
       if (addresses[z]>energyThresHi)
       { cout << z << endl;
	 hitMap[z]+=1;} //shows pile up between events too
       }
 } 

   for (int i=0; i<nEvents; i++)
     {
       double energySpread[9] = {};
       int crysID[9] = {};
       tree->GetEntry(i);
       for (int j=0; j<1225; j++) 
	 {
	   if (hitMap[j] > 0)
	     { cout << "here1" << endl;
	       int x = IDtoX(j);
	       int y = IDtoY(j);
	       if (addresses[j] >0)
		 {
		   //breadth first search
		   double xPos(0.), yPos(0.), energy(0.);
		   for (int q=-1; q<2; q++)
		     {for (int f=-1; f<2; f++)
			 { cout << "here" << endl;
			   energySpread[(f+1)+3*(q+1)]=
			     addresses[CoordtoID(x+f, y+q)];
			   gammaH->
			     Fill(x+f,y+q, addresses[CoordtoID(x+f,y+q)]);
			   crysID[(f+1)+3*(q+1)] =
						 CoordtoID(x+f,y+q);
			   xPos+=addresses[CoordtoID(x+f, y+q)]*(x+f); 
			   yPos+=addresses[CoordtoID(x+f, y+q)]*(y+q); 
			   energy+=addresses[CoordtoID(x+f, y+q)]; 
			 }

		     }
		   cout << "Photon Energy: " << energy << endl;
		   cout << "Photon Position: (" << xPos*5.0/energy << 
		     ", " << yPos*5.0/energy << ")" << endl; 
		 }
	     }
	 }
     }

TCanvas* canvas = new TCanvas("canvas", "canvas", 700,700);
 canvas->Divide(2,1);

canvas->cd();
gammaH->Draw("BOX");



}







 





#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TMath.h"
#include <iostream>
//no bias
void calcXSection() 
{

  cout << "Plotting begin...." << endl;

  int totalHitNum; 
  int numHit; 

  TFile* file = new TFile("complete.root");

  TTree* tree = (TTree *)file->Get("Gamma_Info");

  int nEvents = tree->GetEntries();

  tree->SetBranchAddress("numHits", &numHit);

  for (int i=0; i<nEvents; i++)
    {tree->GetEvent(i);
      if (numHit>0) {totalHitNum++; }
    }

  cout << "Number of Events: " << nEvents << endl;
  cout << "Number of Hits: " << totalHitNum << endl;

  double eAreaDensity = 2*(.0708/2.02)*(6.022e+23)*10;  
  cout << "Electron area density: " << eAreaDensity << endl;

  cout << "The calculated cross section for given angular range is : "
       <<  totalHitNum/(nEvents*eAreaDensity)*(1e+24) << " in barns" << endl;

}



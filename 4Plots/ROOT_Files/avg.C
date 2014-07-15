#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream>
#include "TCanvas.h"

void avg() {

double targetPos, beamEn, avgTargetPos, avgBeamEnergy= 0.;

double targetHits, beamHits = 0;

  TFile* file = new TFile("complete.root");

  TTree* tree = (TTree *)file->Get("Signal");

  int nEvents = tree->GetEntries(); 

  tree->SetBranchAddress("TargetPos", &targetPos);
  tree->SetBranchAddress("BeamEnergy", &beamEn);


  for (int q=0; q<nEvents; q++)
    { tree->GetEntry(q);
      if (targetPos<-4000) {targetHits++; avgTargetPos+=targetPos; 
	cout << targetPos << endl;}
      if (beamEn>0) {beamHits++; avgBeamEnergy+=beamEn;}
    }

  cout << "Number of events: " << nEvents << endl;

  avgBeamEnergy/=beamHits;
  cout << targetHits << endl;
  avgTargetPos/=targetHits; // distance to target

cout << avgBeamEnergy << endl;
cout << avgTargetPos << endl;
 
}

// ----------------------------------------------------------------------------
// $Id: stdhep2root.C,v 1.2 2001/10/13 02:21:51 masako Exp $
// ----------------------------------------------------------------------------
//
// $Log: stdhep2root.C,v $
// Revision 1.2  2001/10/13 02:21:51  masako
// *** empty log message ***
//
// Revision 1.1  2001/06/29 21:40:23  toshi
// Initial release.
//
//


#include "TBranch.h"

int stdhep2root(int nEvent=100) {

  gROOT->Reset();

  // In order to refer to shareable libraries in this simple form,
  // include their paths in a .rootrc file.
  gSystem->Load("libLCDEvent");
  gSystem->Load("libLCDRootAppsUtil");
  gSystem->Load("libLCDFastMC");
  gSystem->Load("libLCDStdHEPUtil");
    gSystem->Load("libEG");
  gSystem->Load("libEGPythia6");
  gSystem->Load("libLCDEvent");
  gSystem->Load("libLCDRootAppsUtil");
  gSystem->Load("libLCDFastMC");
  gSystem->Load("libLCDPhUtil");
  gSystem->Load("libLCDGenUtil");


  TString parfile_dir;
  parfile_dir += gSystem->Getenv("LCDROOT")   ;parfile_dir += "/";
  parfile_dir += "ParFiles/";

  LCDEvent *event= new LCDEvent(); //Create Physics event container first.

  // Input stdhep data
  Char_t* stdFile = "test.hep";
  // Char_t* stdFile = "../../../GenData/eetobbZ0.dat";
  LCDreadStdFile source(stdFile,event);

  //
  // Open root file.
  //
  Int_t comp=2;
  Char_t* rootFile = "eetobbZ0.root";
  TFile* hfile = new TFile(rootFile,"RECREATE","LCD Event ROOT file");
  hfile->SetCompressionLevel(comp);

  //
  // Create a tree with one superbranch.
  //
  TTree* tree = new TTree( "T", "LCD ROOT tree" );
  tree->SetAutoSave( 1000000000 );
  tree->Branch( "LCDEvent", "LCDEvent", &event);
  
  // Event loop
  Int_t iEvent;
  for (iEvent = 0; iEvent < nEvent; iEvent++) {
    if (!source.GetEvent()) break; // Read an event from the stdhep file.
    tree->Fill();
  }

  //
  // Clean up the ROOT file.
  //
  hfile->Write();
  tree->Print();
  hfile->Close();

  return iEvent;
}

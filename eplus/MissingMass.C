#include <iostream>
#include "TChain.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TFile.h"
#include <cmath>
using namespace std;

int MissingMass(){
  //everything is in units of MeV,m
  const double eMass=0.511;
  const double E1=5000;
  const double E2=eMass;
  const double epz=pow(pow(E1,2)-pow(eMass,2),0.5);
  const double epy=0;
  const double epx=0;
  const double target_x=0;
  const double target_y=0;
  const double target_z=-5;
  const double calo_z=5;

  TBranch *xBranch1;
  TBranch *yBranch1;
  TBranch *EBranch1;
  double pos_x_1;
  double pos_y_1;
  double E_1;
  TBranch *xBranch2;
  TBranch *yBranch2;
  TBranch *EBranch2;
  double pos_x_2;
  double pos_y_2;
  double E_2;
  TBranch *nHitBranch;
  int nhit;
  TBranch *xMomentumBranch1;
  TBranch *yMomentumBranch1;
  TBranch *zMomentumBranch1;
  TBranch *xMomentumBranch2;
  TBranch *yMomentumBranch2;
  TBranch *zMomentumBranch2;
  double px_1;
  double py_1;
  double pz_1;
  double px_2;
  double py_2;
  double pz_2;
  TFile *data = new TFile("2Gamma.root","OPEN");

  TTree *tree = (TTree *)data->Get("ntuples/2Gamma");
  tree->SetMakeClass(1);
  tree->SetBranchAddress("numHits", &nhit, &nHitBranch);
  tree->SetBranchAddress("photon1xPos", &pos_x_1, &xBranch1);
  tree->SetBranchAddress("photon2xPos", &pos_x_2, &xBranch2);
  tree->SetBranchAddress("photon1yPos", &pos_y_1, &yBranch1);
  tree->SetBranchAddress("photon2yPos", &pos_y_2, &yBranch2);
  tree->SetBranchAddress("totEnergyPhoton1", &E_1, &EBranch1);
  tree->SetBranchAddress("totEnergyPhoton2", &E_2, &EBranch2);
  tree->SetBranchAddress("photon1xMomentum", &px_1, &xMomentumBranch1);
  tree->SetBranchAddress("photon1yMomentum", &py_1, &yMomentumBranch1);
  tree->SetBranchAddress("photon1zMomentum", &pz_1, &zMomentumBranch1);
  tree->SetBranchAddress("photon2xMomentum", &px_2, &xMomentumBranch2);
  tree->SetBranchAddress("photon2yMomentum", &py_2, &yMomentumBranch2);
  tree->SetBranchAddress("photon2zMomentum", &pz_2, &zMomentumBranch2);


  TFile *f = new TFile("output.root","RECREAT");
  f->cd();

  TH1D *hmm = new TH1D("h1","Missing mass", 100, 0, 100);
  for( int ev=0; ev<tree->GetEntries(); ev++){
    xBranch1->GetEntry(ev);
    yBranch1->GetEntry(ev);
    EBranch1->GetEntry(ev);
    xMomentumBranch1->GetEntry(ev);
    yMomentumBranch1->GetEntry(ev);
    zMomentumBranch1->GetEntry(ev);
    
    double tempE=E1+E2-E_1;

    double temppx=epx-px_1;
    double temppy=epy-py_1;
    double temppz=epz-pz_1;
    //double misMass=2*pow(eMass,2)+2*E1*(E2-E_1)-2*E2*E_1+2*pz*epz;
    double misMass=pow(pow(tempE,2)-pow(temppx,2)-pow(temppy,2)-pow(temppz,2),0.5);
    /*
    if (misMass<0) {
      cout << "====== Output information ======" << endl;
      cout << px << endl;
      cout << py << endl;
      cout << pz << endl;
      cout << E << endl;
    }*/
    if (E_1!=0) 
      hmm->Fill(misMass);
  }
  
  TCanvas *c1 = new TCanvas("c1", "px_distribution", 800, 800);
  c1->cd();
  c1->SetLogy();
  hmm->Draw();
  c1->Write();

  f->Close();
  data->Close();

  return 0;
}

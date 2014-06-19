#include <iostream>
#include "TChain.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TFile.h"
#include <cmath>
using namespace std;

int MissingMass(){
  //everything is in units of MeV
  const double eMass=0.511;
  const double E1=3000;
  const double E2=eMass;
  const double epz=pow(pow(E1,2)-pow(eMass,2),0.5);
  const double epy=0;
  const double epx=0;
  const double target_x=0;
  const double target_y=0;

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
  TFile *data = new TFile("2Gamma.root","OPEN");

  TTree *tree = (TTree *)data->Get("ntuples/2Gamma");
  tree->SetMakeClass(7);
  tree->SetBranchAddress("Number_of_hits", &nhit, &nHitBranch);
  tree->SetBranchAddress("X_Position_Photon1", &pos_x_1, &xBranch1);
  tree->SetBranchAddress("X_Position_Photon2", &pos_x_2, &xBranch2);
  tree->SetBranchAddress("Y_Position_Photon1", &pos_y_1, &yBranch1);
  tree->SetBranchAddress("Y_Position_Photon2", &pos_y_2, &yBranch2);
  tree->SetBranchAddress("Total_Energy_Per_Photon1", &E_1, &EBranch1);
  tree->SetBranchAddress("Total_Energy_Per_Photon2", &E_2, &EBranch2);


  TFile *f = new TFile("output.root","RECREAT");
  f->cd();

  TH1D *hmm = new TH1D("h1","Missing mass", 100, 0, 100);
  for( int ev=0; ev<tree->GetEntries(); ev++){
    xBranch1->GetEntry(ev);
    yBranch1->GetEntry(ev);
    EBranch1->GetEntry(ev);
    
    double tempE=E1+E2-E_1;
    double arc = pow(pow((pos_x_1-target_x),2)+pow((pos_y_1-target_y),2),0.5);
    double cosine=(pos_x_1-target_x)/arc;
    double sine=(pos_y_1-target_y)/arc;
    double px=E_1*cosine;
    double py=E_1*sine;
    double pz=0;

    double temppx=epx-px;
    double temppy=epy-py;
    double temppz=epz-pz;
    double misMass=pow(pow(tempE,2)-pow(temppx,2)-pow(temppy,2)-pow(temppz,2),0.5);
    
    /*
    if (misMass<0) {
      cout << "====== Output information ======" << endl;
      cout << px << endl;
      cout << py << endl;
      cout << pz << endl;
      cout << E << endl;
    }*/

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

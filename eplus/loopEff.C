#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "TChain.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TFile.h"
#include <cmath>
using namespace std;

int loopEff(){
  //everything is in units of MeV,m
  const double eMass=0.511;
  const double pMass=938;
  const double E1=5000+eMass;
  const double epz=pow(pow(E1,2)-pow(eMass,2),0.5);
  const double epy=0;
  const double epx=0;

  const double angle_low = 1;
  const double angle_high = 10;
  const double angle_step = 1;

  const double pos_low = 1;
  const double pos_high = 10;
  const double pos_step = 1;

  const double brem_eff = 0.1;

  double h_pos_low = pos_low-pos_step/2;
  double h_pos_high = pos_high+pos_step/2;
  int h_pos_bin= (int (pos_high-pos_low)/pos_step) + 1;

  double h_angle_low = angle_low-angle_step/2;
  double h_angle_high = angle_high+angle_step/2;
  int h_angle_bin = (int (angle_high-angle_low)/angle_step) + 1;

  TH2D *h2D_pos_ang_eff = new TH2D("h1","pos_ang_eff", h_pos_bin, h_pos_low, h_pos_high, h_angle_bin, h_angle_low, h_angle_high); 

  for (double ang=angle_low; ang<=angle_high; ang+=angle_step){
    for (double pos=pos_low; pos<=pos_high; pos+=pos_step){

      //create new mac file
      
      ofstream macfile;
      macfile.open("eplusrun.mac");
      macfile << "/tracking/verbose 0\n";
      macfile << "/gps/pos/sigma_r " << pos << " mm\n";
      macfile << "/gps/ang/sigma_r " << ang << "e-3 radian\n";
      macfile << "/run/beamOn 10000\n";
      macfile.close();
      
      //execute mac file
      system("./darkphoton eplusrun.mac");

      //reading data from the root file
      TBranch *PhotonPxBranch;
      TBranch *PhotonPyBranch;
      TBranch *PhotonPzBranch;
      TBranch *PhotonEBranch;
      double photon_px;
      double photon_py;
      double photon_pz;
      double photon_E;
      TBranch *PositronPxBranch;
      TBranch *PositronPyBranch;
      TBranch *PositronPzBranch;
      TBranch *PositronEBranch;
      double positron_px;
      double positron_py;
      double positron_pz;
      double positron_E;
      TBranch *PositronXBranch;
      TBranch *PositronYBranch;
      TBranch *PositronZBranch;
      double positron_x;
      double positron_y;
      double positron_z;

      TFile *data = new TFile("qanalysis.root","OPEN");

      TTree *tree = (TTree *)data->Get("ntuples/qanalysis");
      tree->SetMakeClass(1);
      tree->SetBranchAddress("Energy_Photon", &photon_E, &PhotonEBranch);
      tree->SetBranchAddress("photon_x_Momentum", &photon_px, &PhotonPxBranch);
      tree->SetBranchAddress("photon_y_Momentum", &photon_py, &PhotonPyBranch);
      tree->SetBranchAddress("photon_z_Momentum", &photon_pz, &PhotonPzBranch);

      tree->SetBranchAddress("Energy_Positron", &positron_E, &PositronEBranch);
      tree->SetBranchAddress("positron_x_Momentum", &positron_px, &PositronPxBranch);
      tree->SetBranchAddress("positron_y_Momentum", &positron_py, &PositronPyBranch);
      tree->SetBranchAddress("positron_z_Momentum", &positron_pz, &PositronPzBranch);
      tree->SetBranchAddress("positron_x_Position", &positron_x, &PositronXBranch);
      tree->SetBranchAddress("positron_y_Position", &positron_y, &PositronYBranch);
      tree->SetBranchAddress("positron_z_Position", &positron_z, &PositronZBranch);

      double low =0;
      double high=10;
      double nbin=10000;

      TH1D *h2DPositron_Pos_1= new TH1D("h12","e+_position_noBrem", nbin, low, high);
      TH1D *h2DPositron_Pos_0= new TH1D("h13","e+_position_Brem", nbin, low, high);

      TH1D *h2DPositron_PosC_1= new TH1D("h14","e+_position_noBrem_C", nbin , low, high);
      TH1D *h2DPositron_PosC_0= new TH1D("h15","e+_position_Brem_C", nbin, low, high);

      int entries = tree->GetEntries()> 1000000 ? 1000000 : tree->GetEntries();
      for( int ev=0; ev<entries; ev++){
        
        PhotonPxBranch->GetEntry(ev);
        PhotonPyBranch->GetEntry(ev);
        PhotonPzBranch->GetEntry(ev);
        PhotonEBranch->GetEntry(ev);
        PositronPxBranch->GetEntry(ev);
        PositronPyBranch->GetEntry(ev);
        PositronPzBranch->GetEntry(ev);
        PositronEBranch->GetEntry(ev);
        PositronXBranch->GetEntry(ev);
        PositronYBranch->GetEntry(ev);
        PositronZBranch->GetEntry(ev);

        if (photon_E!=0)
          h2DPositron_Pos_1->Fill(positron_y);
        else 
          h2DPositron_Pos_0->Fill(positron_y);
      }
      
      double total0=0;
      double total1=0;
      for (int i=1; i<=nbin; i++){
        total0 += h2DPositron_Pos_0->GetBinContent(i);
        total1 += h2DPositron_Pos_1->GetBinContent(i);

        h2DPositron_PosC_0->SetBinContent(i,total0);
        h2DPositron_PosC_1->SetBinContent(i,total1);
      }
      h2DPositron_PosC_0->Scale(1.0/total0);
      h2DPositron_PosC_1->Scale(1.0/total1);

      int min1=0;
      for (min1=1; h2DPositron_PosC_0->GetBinContent(min1)<1e-6; min1++);
      int min2=0;
      for (min2=1; h2DPositron_PosC_1->GetBinContent(min2)<1e-6; min2++);
      int min= (min1<min2) ? min1: min2;
      min= (min-10>0) ? min-10: 1;

      int max1=0;
      for (max1=nbin; h2DPositron_PosC_0->GetBinContent(max1)>1-1e-6; max1--);
      int max2=0;
      for (max2=nbin; h2DPositron_PosC_1->GetBinContent(max2)>1-1e-6; max2--);
      int max= (max1>max2) ? max1: max2;
      max= (max+10<nbin) ? max+10: nbin;

      double nlow = h2DPositron_Pos_0->GetBinCenter(min);
      double nhigh = h2DPositron_Pos_0->GetBinCenter(max);

      TH1D *h2DPositron_Pos_1n= new TH1D("h2","e+_position_noBrem_n", nbin, nlow, nhigh);
      TH1D *h2DPositron_Pos_0n= new TH1D("h3","e+_position_Brem_n", nbin, nlow, nhigh);

      TH1D *h2DPositron_PosC_1n= new TH1D("h4","e+_position_noBrem_C_n", nbin , nlow, nhigh);
      TH1D *h2DPositron_PosC_0n= new TH1D("h5","e+_position_Brem_C_n", nbin, nlow, nhigh);

      for( int ev=0; ev<entries; ev++){
        
        PhotonPxBranch->GetEntry(ev);
        PhotonPyBranch->GetEntry(ev);
        PhotonPzBranch->GetEntry(ev);
        PhotonEBranch->GetEntry(ev);
        PositronPxBranch->GetEntry(ev);
        PositronPyBranch->GetEntry(ev);
        PositronPzBranch->GetEntry(ev);
        PositronEBranch->GetEntry(ev);
        PositronXBranch->GetEntry(ev);
        PositronYBranch->GetEntry(ev);
        PositronZBranch->GetEntry(ev);

        if (photon_E!=0)
          h2DPositron_Pos_1n->Fill(positron_y);
        else 
          h2DPositron_Pos_0n->Fill(positron_y);
      }
      
      total0=0;
      total1=0;
      for (int i=1; i<=nbin; i++){
        total0 += h2DPositron_Pos_0n->GetBinContent(i);
        total1 += h2DPositron_Pos_1n->GetBinContent(i);

        h2DPositron_PosC_0n->SetBinContent(i,total0);
        h2DPositron_PosC_1n->SetBinContent(i,total1);
      }
      h2DPositron_PosC_0n->Scale(1.0/total0);
      h2DPositron_PosC_1n->Scale(1.0/total1);

      int i=0;
      for (i=1; h2DPositron_PosC_1->GetBinContent(i)<brem_eff; i++);

      h2D_pos_ang_eff->Fill(pos, ang, 1-h2DPositron_PosC_0->GetBinContent(i) );

      data->Close();
    }
  }
  TCanvas *c1 = new TCanvas("c1", "pos_ang_eff", 800, 800);
  c1->cd();
  h2D_pos_ang_eff->Draw("COLZ");
  h2D_pos_ang_eff->Draw("TEXT0 SAME");
  return 0;
}

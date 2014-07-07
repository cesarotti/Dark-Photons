#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TMath.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"

#include <iostream>
#include <iomanip>
#include <vector>

void makePlots() {
    /*
    // declare objects
    TClonesArray* electronTCl = new TClonesArray("TLorentzVector");
    std::vector<double>* jet_bdisc;

    // open ttree
    TFile* file = new TFile("stop_ntuple.root");
    TTree* tree = (TTree*)(file->Get("Events"));
     
    // set branches
    tree->SetBranchAddress("electronTCl", &electronTCl);
    tree->SetBranchAddress("jet_bdisc", &jet_bdisc);

    // declare histograms
    TH1F* helectron_pt = new TH1F("helectron_pt", "electron p_{T}", 100, 0, 200);
    TH1F* hjet_bdisc = new TH1F("hjet_bdisc", "jet b-discriminator", 100, -2, 2);

    // loop over all events in ttree
    for (int event = 0; event < tree->GetEntries(); ++event) {
        // first, clear all vectors
        electronTCl->Clear();
        jet_bdisc->clear();

        // access event
        tree->GetEntry(event);

        // fill histograms
        for (int i = 0; i < electronTCl->GetEntriesFast(); i++) {
            TLorentzVector* electron_temp = (TLorentzVector*)electronTCl->At(i);
            helectron_pt->Fill(electron_temp->Pt());
        }

        for (int i = 0; i < int(jet_bdisc->size()); i++) {
            hjet_bdisc->Fill(jet_bdisc->at(i));
        }

    }

    // declare canvas, draw histograms
    TCanvas* canvas = new TCanvas("canvas","canvas",700,700);
    canvas->cd();

    helectron_pt->Draw();

    TCanvas* canvas2 = new TCanvas("canvas2","canvas2",700,700);
    canvas2->cd();

    hjet_bdisc->Draw();
    */





    TFile* file = new TFile("final.root");
    TTree* Hits_Info = (TTree*)file->Get("Hits_Info");

    Int_t pID;
    Double_t energy;
    Double_t theta;

    Hits_Info->SetBranchAddress("PDGID", &pID);
    Hits_Info->SetBranchAddress("Energy", &energy);
    Hits_Info->SetBranchAddress("theta", &theta);

    // Initialize histograms
    int nEnergyBins = 50;
    int nThetaBins = 100;
    int nEnergyMax = 6000;
    int nThetaMax = 15;

    // electron
    TH1F* heplusEnergy = new TH1F("heplusEnergy", "positron energy distribution", nEnergyBins, 0, nEnergyMax);
    TH1F* heplusTheta = new TH1F("heplusTheta", "positron theta distribution", nThetaBins, 0, nThetaMax);

    // gamma
    TH1F* hgammaEnergy = new TH1F("hgammaEnergy", "gamma energy distribution", nEnergyBins, 0, nEnergyMax);
    TH1F* hgammaTheta = new TH1F("hgammaTheta", "gamma theta distribution", nThetaBins, 0, nThetaMax);

    // pi plus
    TH1F* hpiplusEnergy = new TH1F("hpiplusEnergy", "pi plus energy distribution", nEnergyBins, 0, nEnergyMax);
    TH1F* hpiplusTheta = new TH1F("hpiplusTheta", "pi plus theta distribution", nThetaBins, 0, nThetaMax);

    // pi minus
    TH1F* hpiminusEnergy = new TH1F("hpiminusEnergy", "pi minus energy distribution", nEnergyBins, 0, nEnergyMax);
    TH1F* hpiminusTheta = new TH1F("hpiminusTheta", "pi minus theta distribution", nThetaBins, 0, nThetaMax);

    // pi zero
    TH1F* hpizeroEnergy = new TH1F("hpizeroEnergy", "pi zero energy distribution", nEnergyBins, 0, nEnergyMax);
    TH1F* hpizeroTheta = new TH1F("hpizeroTheta", "pi zero theta distribution", nThetaBins, 0, nThetaMax);

    // proton
    TH1F* hprotonEnergy = new TH1F("hprotonEnergy", "proton energy distribution", nEnergyBins, 0, nEnergyMax);
    TH1F* hprotonTheta = new TH1F("hprotonheta", "proton theta distribution", nThetaBins, 0, nThetaMax);

    // neutron
    TH1F* hneutronEnergy = new TH1F("hneutronEnergy", "neutron energy distribution", nEnergyBins, 0, nEnergyMax);
    TH1F* hneutronTheta = new TH1F("hneutronheta", "neutron theta distribution", nThetaBins, 0, nThetaMax);

    // all entries and fill the histograms
    Int_t nentries = (Int_t)Hits_Info->GetEntries();
    for (Int_t i=0;i<nentries;i++) {
        Hits_Info->GetEntry(i);

        if (true) { // we're only interested in theta < 6 degrees

            if (pID == -11) { // positron
                heplusEnergy->Fill(energy);
                heplusTheta->Fill(theta);
            } else if (pID == 22) { // gamma
                hgammaEnergy->Fill(energy);
                hgammaTheta->Fill(theta);
            } else if (pID == 211) { // pi plus
                hpiplusEnergy->Fill(energy);
                hpiplusTheta->Fill(theta);
            } else if (pID == -211) { // pi minus
                hpiminusEnergy->Fill(energy);
                hpiminusTheta->Fill(theta);
            } else if (pID == 111) { // pi zero
                hpizeroEnergy->Fill(energy);
                hpizeroTheta->Fill(theta);
            } else if (pID == 2212) { // proton
                hprotonEnergy->Fill(energy);
                hprotonTheta->Fill(theta);
            } else if (pID == 2112) { // neutron
                hneutronEnergy->Fill(energy);
                hneutronTheta->Fill(theta);
            }
            
        }

    }

    TCanvas* canvas = new TCanvas("canvas", "canvas", 700, 700);
    canvas->Divide(7,2);

    // positron
    canvas->cd(1);
    heplusEnergy->Draw();

    canvas->cd(8);
    heplusTheta->Draw();

    // gamma
    canvas->cd(2);
    hgammaEnergy->Draw();

    canvas->cd(9);
    hgammaTheta->Draw();

    // pi plus
    canvas->cd(3);
    hpiplusEnergy->Draw();

    canvas->cd(10);
    hpiplusTheta->Draw();
    
    // pi minus
    canvas->cd(4);
    hpiminusEnergy->Draw();

    canvas->cd(11);
    hpiminusTheta->Draw();

    // pi zero
    canvas->cd(5);
    hpizeroEnergy->Draw();

    canvas->cd(12);
    hpizeroTheta->Draw();

    // proton
    canvas->cd(6);
    hprotonEnergy->Draw();

    canvas->cd(13);
    hprotonTheta->Draw();

    // neutron
    canvas->cd(7);
    hneutronEnergy->Draw();

    canvas->cd(14);
    hneutronTheta->Draw();

}

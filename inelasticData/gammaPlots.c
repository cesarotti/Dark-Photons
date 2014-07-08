#include <string>
#include "TFile.h"
#include "TH1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TStyle.h"

void gammaPlots() {
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
    int nEnergyMax = 2000;
    int nEnergyBins = nEnergyMax/10;

    // gamma
    TH1D* hgammaEnergy = new TH1D("5 < theta < 10" ,                       // plot label
                                  "Gamma Energy Distribution",  // title
                                  nEnergyBins,                  // x number of bins
                                  0,                            // x lower bound
                                  nEnergyMax);                  // x upper bound

    // all entries and fill the histograms
    Int_t nentries = (Int_t)Hits_Info->GetEntries();
    for (Int_t i=0;i<nentries;i++) {
        Hits_Info->GetEntry(i);

        if (pID == 22) { // gamma
            hgammaEnergy->Fill(energy);
        }
    }

    TCanvas* canvas = new TCanvas("canvas", "canvas", 700, 700);
    canvas->SetLogy();
    // gamma
    hgammaEnergy->SetFillColor(kBlue);
    hgammaEnergy->SetFillStyle(3001);
    hgammaEnergy->Draw();
    hgammaEnergy->GetXaxis()->SetTitle("Energy (MeV)");
    hgammaEnergy->GetYaxis()->SetTitle("dN/dE");
    hgammaEnergy->GetXaxis()->CenterTitle();
    hgammaEnergy->GetYaxis()->CenterTitle();
}

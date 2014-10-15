{
//=========Macro generated from canvas: tempo28/
//=========  (Tue Sep 30 16:04:17 2014) by ROOT version5.34/09
   TCanvas *tempo28 = new TCanvas("tempo28", "",0,0,700,500);
   gStyle->SetOptStat(0);
   tempo28->SetHighLightColor(2);
   tempo28->Range(-161.2903,-2.341288e+11,1129.032,1.716945e+12);
   tempo28->SetFillColor(0);
   tempo28->SetBorderMode(0);
   tempo28->SetBorderSize(3);
   tempo28->SetLeftMargin(0.125);
   tempo28->SetBottomMargin(0.12);
   tempo28->SetFrameBorderMode(0);
   tempo28->SetFrameBorderSize(0);
   tempo28->SetFrameBorderMode(0);
   tempo28->SetFrameBorderSize(0);
   
   THStack *mystack = new THStack();
   mystack->SetName("mystack");
   mystack->SetTitle("");
   
   TH1F *mystack_stack_28 = new TH1F("mystack_stack_28","",100,0,1000);
   mystack_stack_28->SetMinimum(0);
   mystack_stack_28->SetMaximum(1.521837e+12);
   mystack_stack_28->SetDirectory(0);
   mystack_stack_28->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   mystack_stack_28->SetLineColor(ci);
   mystack_stack_28->SetLineWidth(2);
   mystack_stack_28->GetXaxis()->SetTitle("#slash{E}_{T} (GeV) ");
   mystack_stack_28->GetXaxis()->SetLabelFont(22);
   mystack_stack_28->GetXaxis()->SetTitleSize(0.06);
   mystack_stack_28->GetXaxis()->SetTitleOffset(0.85);
   mystack_stack_28->GetXaxis()->SetTitleFont(22);
   mystack_stack_28->GetYaxis()->SetTitle("Events  ( L_{int} = 10 fb^{-1} )");
   mystack_stack_28->GetYaxis()->SetLabelFont(22);
   mystack_stack_28->GetYaxis()->SetTitleSize(0.06);
   mystack_stack_28->GetYaxis()->SetTitleOffset(0.98);
   mystack_stack_28->GetYaxis()->SetTitleFont(22);
   mystack_stack_28->GetZaxis()->SetLabelFont(42);
   mystack_stack_28->GetZaxis()->SetLabelSize(0.035);
   mystack_stack_28->GetZaxis()->SetTitleSize(0.035);
   mystack_stack_28->GetZaxis()->SetTitleFont(42);
   mystack->SetHistogram(mystack_stack_28);
   
   
   TH1F *selection_0_10 = new TH1F("selection_0_10","selection_0_10",100,0,1000);
   selection_0_10->SetBinContent(1,1.449369e+12);
   selection_0_10->SetEntries(100);
   selection_0_10->SetStats(0);
   selection_0_10->SetFillColor(9);
   selection_0_10->SetLineColor(9);
   selection_0_10->GetXaxis()->SetLabelFont(22);
   selection_0_10->GetXaxis()->SetLabelSize(0.03);
   selection_0_10->GetXaxis()->SetTitleOffset(0.85);
   selection_0_10->GetXaxis()->SetTitleFont(22);
   selection_0_10->GetYaxis()->SetLabelFont(22);
   selection_0_10->GetYaxis()->SetLabelSize(0.03);
   selection_0_10->GetYaxis()->SetTitleOffset(0.98);
   selection_0_10->GetYaxis()->SetTitleFont(22);
   selection_0_10->GetZaxis()->SetLabelFont(42);
   selection_0_10->GetZaxis()->SetLabelSize(0.035);
   selection_0_10->GetZaxis()->SetTitleSize(0.035);
   selection_0_10->GetZaxis()->SetTitleFont(42);
   mystack->Add(selection_0_10,"");
   mystack->Draw("");
   tempo28->Modified();
   tempo28->cd();
   tempo28->SetSelected(tempo28);
}

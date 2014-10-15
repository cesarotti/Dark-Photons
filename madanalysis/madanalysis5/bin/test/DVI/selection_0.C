{
//=========Macro generated from canvas: tempo5/
//=========  (Tue Sep 30 16:00:07 2014) by ROOT version5.34/09
   TCanvas *tempo5 = new TCanvas("tempo5", "",0,0,700,500);
   gStyle->SetOptStat(0);
   tempo5->SetHighLightColor(2);
   tempo5->Range(-161.2903,-2.341288e+11,1129.032,1.716945e+12);
   tempo5->SetFillColor(0);
   tempo5->SetBorderMode(0);
   tempo5->SetBorderSize(3);
   tempo5->SetLeftMargin(0.125);
   tempo5->SetBottomMargin(0.12);
   tempo5->SetFrameBorderMode(0);
   tempo5->SetFrameBorderSize(0);
   tempo5->SetFrameBorderMode(0);
   tempo5->SetFrameBorderSize(0);
   
   THStack *mystack = new THStack();
   mystack->SetName("mystack");
   mystack->SetTitle("");
   
   TH1F *mystack_stack_5 = new TH1F("mystack_stack_5","",100,0,1000);
   mystack_stack_5->SetMinimum(0);
   mystack_stack_5->SetMaximum(1.521837e+12);
   mystack_stack_5->SetDirectory(0);
   mystack_stack_5->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   mystack_stack_5->SetLineColor(ci);
   mystack_stack_5->SetLineWidth(2);
   mystack_stack_5->GetXaxis()->SetTitle("#slash{E}_{T} (GeV) ");
   mystack_stack_5->GetXaxis()->SetLabelFont(22);
   mystack_stack_5->GetXaxis()->SetTitleSize(0.06);
   mystack_stack_5->GetXaxis()->SetTitleOffset(0.85);
   mystack_stack_5->GetXaxis()->SetTitleFont(22);
   mystack_stack_5->GetYaxis()->SetTitle("Events  ( L_{int} = 10 fb^{-1} )");
   mystack_stack_5->GetYaxis()->SetLabelFont(22);
   mystack_stack_5->GetYaxis()->SetTitleSize(0.06);
   mystack_stack_5->GetYaxis()->SetTitleOffset(0.98);
   mystack_stack_5->GetYaxis()->SetTitleFont(22);
   mystack_stack_5->GetZaxis()->SetLabelFont(42);
   mystack_stack_5->GetZaxis()->SetLabelSize(0.035);
   mystack_stack_5->GetZaxis()->SetTitleSize(0.035);
   mystack_stack_5->GetZaxis()->SetTitleFont(42);
   mystack->SetHistogram(mystack_stack_5);
   
   
   TH1F *selection_0_1 = new TH1F("selection_0_1","selection_0_1",100,0,1000);
   selection_0_1->SetBinContent(1,1.449369e+12);
   selection_0_1->SetEntries(100);
   selection_0_1->SetFillColor(9);
   selection_0_1->SetLineColor(9);
   selection_0_1->GetXaxis()->SetLabelFont(42);
   selection_0_1->GetXaxis()->SetLabelSize(0.035);
   selection_0_1->GetXaxis()->SetTitleSize(0.035);
   selection_0_1->GetXaxis()->SetTitleFont(42);
   selection_0_1->GetYaxis()->SetLabelFont(42);
   selection_0_1->GetYaxis()->SetLabelSize(0.035);
   selection_0_1->GetYaxis()->SetTitleSize(0.035);
   selection_0_1->GetYaxis()->SetTitleFont(42);
   selection_0_1->GetZaxis()->SetLabelFont(42);
   selection_0_1->GetZaxis()->SetLabelSize(0.035);
   selection_0_1->GetZaxis()->SetTitleSize(0.035);
   selection_0_1->GetZaxis()->SetTitleFont(42);
   mystack->Add(selection_0_1,"");
   mystack->Draw("");
   tempo5->Modified();
   tempo5->cd();
   tempo5->SetSelected(tempo5);
}

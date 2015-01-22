{
//=========Macro generated from canvas: tempo33/
//=========  (Tue Sep 30 16:04:18 2014) by ROOT version5.34/09
   TCanvas *tempo33 = new TCanvas("tempo33", "",0,0,700,500);
   gStyle->SetOptStat(0);
   tempo33->SetHighLightColor(2);
   tempo33->Range(-161.2903,-2.341288e+11,1129.032,1.716945e+12);
   tempo33->SetFillColor(0);
   tempo33->SetBorderMode(0);
   tempo33->SetBorderSize(3);
   tempo33->SetLeftMargin(0.125);
   tempo33->SetBottomMargin(0.12);
   tempo33->SetFrameBorderMode(0);
   tempo33->SetFrameBorderSize(0);
   tempo33->SetFrameBorderMode(0);
   tempo33->SetFrameBorderSize(0);
   
   THStack *mystack = new THStack();
   mystack->SetName("mystack");
   mystack->SetTitle("");
   
   TH1F *mystack_stack_33 = new TH1F("mystack_stack_33","",100,0,1000);
   mystack_stack_33->SetMinimum(0);
   mystack_stack_33->SetMaximum(1.521837e+12);
   mystack_stack_33->SetDirectory(0);
   mystack_stack_33->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   mystack_stack_33->SetLineColor(ci);
   mystack_stack_33->SetLineWidth(2);
   mystack_stack_33->GetXaxis()->SetTitle("#slash{E}_{T} (GeV) ");
   mystack_stack_33->GetXaxis()->SetLabelFont(22);
   mystack_stack_33->GetXaxis()->SetTitleSize(0.06);
   mystack_stack_33->GetXaxis()->SetTitleOffset(0.85);
   mystack_stack_33->GetXaxis()->SetTitleFont(22);
   mystack_stack_33->GetYaxis()->SetTitle("Events  ( L_{int} = 10 fb^{-1} )");
   mystack_stack_33->GetYaxis()->SetLabelFont(22);
   mystack_stack_33->GetYaxis()->SetTitleSize(0.06);
   mystack_stack_33->GetYaxis()->SetTitleOffset(0.98);
   mystack_stack_33->GetYaxis()->SetTitleFont(22);
   mystack_stack_33->GetZaxis()->SetLabelFont(42);
   mystack_stack_33->GetZaxis()->SetLabelSize(0.035);
   mystack_stack_33->GetZaxis()->SetTitleSize(0.035);
   mystack_stack_33->GetZaxis()->SetTitleFont(42);
   mystack->SetHistogram(mystack_stack_33);
   
   
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
   tempo33->Modified();
   tempo33->cd();
   tempo33->SetSelected(tempo33);
}

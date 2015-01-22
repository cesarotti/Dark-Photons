{
//=========Macro generated from canvas: tempo39/
//=========  (Tue Sep 30 16:04:18 2014) by ROOT version5.34/09
   TCanvas *tempo39 = new TCanvas("tempo39", "",0,0,700,500);
   gStyle->SetOptStat(0);
   tempo39->SetHighLightColor(2);
   tempo39->Range(-16.12903,-4.682577e+11,112.9032,3.43389e+12);
   tempo39->SetFillColor(0);
   tempo39->SetBorderMode(0);
   tempo39->SetBorderSize(3);
   tempo39->SetLeftMargin(0.125);
   tempo39->SetBottomMargin(0.12);
   tempo39->SetFrameBorderMode(0);
   tempo39->SetFrameBorderSize(0);
   tempo39->SetFrameBorderMode(0);
   tempo39->SetFrameBorderSize(0);
   
   THStack *mystack = new THStack();
   mystack->SetName("mystack");
   mystack->SetTitle("");
   
   TH1F *mystack_stack_39 = new TH1F("mystack_stack_39","",20,0,100);
   mystack_stack_39->SetMinimum(0);
   mystack_stack_39->SetMaximum(3.043675e+12);
   mystack_stack_39->SetDirectory(0);
   mystack_stack_39->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   mystack_stack_39->SetLineColor(ci);
   mystack_stack_39->SetLineWidth(2);
   mystack_stack_39->GetXaxis()->SetTitle("p_{T} [ a ] (GeV/c) ");
   mystack_stack_39->GetXaxis()->SetLabelFont(22);
   mystack_stack_39->GetXaxis()->SetTitleSize(0.06);
   mystack_stack_39->GetXaxis()->SetTitleOffset(0.85);
   mystack_stack_39->GetXaxis()->SetTitleFont(22);
   mystack_stack_39->GetYaxis()->SetTitle("N. of a ( L_{int} = 10 fb^{-1} )");
   mystack_stack_39->GetYaxis()->SetLabelFont(22);
   mystack_stack_39->GetYaxis()->SetTitleSize(0.06);
   mystack_stack_39->GetYaxis()->SetTitleOffset(0.98);
   mystack_stack_39->GetYaxis()->SetTitleFont(22);
   mystack_stack_39->GetZaxis()->SetLabelFont(42);
   mystack_stack_39->GetZaxis()->SetLabelSize(0.035);
   mystack_stack_39->GetZaxis()->SetTitleSize(0.035);
   mystack_stack_39->GetZaxis()->SetTitleFont(42);
   mystack->SetHistogram(mystack_stack_39);
   
   
   TH1F *selection_1_11 = new TH1F("selection_1_11","selection_1_11",20,0,100);
   selection_1_11->SetBinContent(1,2.898738e+12);
   selection_1_11->SetEntries(20);
   selection_1_11->SetStats(0);
   selection_1_11->SetFillColor(9);
   selection_1_11->SetLineColor(9);
   selection_1_11->GetXaxis()->SetLabelFont(22);
   selection_1_11->GetXaxis()->SetLabelSize(0.03);
   selection_1_11->GetXaxis()->SetTitleOffset(0.85);
   selection_1_11->GetXaxis()->SetTitleFont(22);
   selection_1_11->GetYaxis()->SetLabelFont(22);
   selection_1_11->GetYaxis()->SetLabelSize(0.03);
   selection_1_11->GetYaxis()->SetTitleOffset(0.98);
   selection_1_11->GetYaxis()->SetTitleFont(22);
   selection_1_11->GetZaxis()->SetLabelFont(42);
   selection_1_11->GetZaxis()->SetLabelSize(0.035);
   selection_1_11->GetZaxis()->SetTitleSize(0.035);
   selection_1_11->GetZaxis()->SetTitleFont(42);
   mystack->Add(selection_1_11,"");
   mystack->Draw("");
   tempo39->Modified();
   tempo39->cd();
   tempo39->SetSelected(tempo39);
}

{
//=========Macro generated from canvas: tempo29/
//=========  (Tue Sep 30 16:04:17 2014) by ROOT version5.34/09
   TCanvas *tempo29 = new TCanvas("tempo29", "",0,0,700,500);
   gStyle->SetOptStat(0);
   tempo29->SetHighLightColor(2);
   tempo29->Range(-16.12903,-4.682577e+11,112.9032,3.43389e+12);
   tempo29->SetFillColor(0);
   tempo29->SetBorderMode(0);
   tempo29->SetBorderSize(3);
   tempo29->SetLeftMargin(0.125);
   tempo29->SetBottomMargin(0.12);
   tempo29->SetFrameBorderMode(0);
   tempo29->SetFrameBorderSize(0);
   tempo29->SetFrameBorderMode(0);
   tempo29->SetFrameBorderSize(0);
   
   THStack *mystack = new THStack();
   mystack->SetName("mystack");
   mystack->SetTitle("");
   
   TH1F *mystack_stack_29 = new TH1F("mystack_stack_29","",20,0,100);
   mystack_stack_29->SetMinimum(0);
   mystack_stack_29->SetMaximum(3.043675e+12);
   mystack_stack_29->SetDirectory(0);
   mystack_stack_29->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   mystack_stack_29->SetLineColor(ci);
   mystack_stack_29->SetLineWidth(2);
   mystack_stack_29->GetXaxis()->SetTitle("p_{T} [ a ] (GeV/c) ");
   mystack_stack_29->GetXaxis()->SetLabelFont(22);
   mystack_stack_29->GetXaxis()->SetTitleSize(0.06);
   mystack_stack_29->GetXaxis()->SetTitleOffset(0.85);
   mystack_stack_29->GetXaxis()->SetTitleFont(22);
   mystack_stack_29->GetYaxis()->SetTitle("N. of a ( L_{int} = 10 fb^{-1} )");
   mystack_stack_29->GetYaxis()->SetLabelFont(22);
   mystack_stack_29->GetYaxis()->SetTitleSize(0.06);
   mystack_stack_29->GetYaxis()->SetTitleOffset(0.98);
   mystack_stack_29->GetYaxis()->SetTitleFont(22);
   mystack_stack_29->GetZaxis()->SetLabelFont(42);
   mystack_stack_29->GetZaxis()->SetLabelSize(0.035);
   mystack_stack_29->GetZaxis()->SetTitleSize(0.035);
   mystack_stack_29->GetZaxis()->SetTitleFont(42);
   mystack->SetHistogram(mystack_stack_29);
   
   
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
   tempo29->Modified();
   tempo29->cd();
   tempo29->SetSelected(tempo29);
}

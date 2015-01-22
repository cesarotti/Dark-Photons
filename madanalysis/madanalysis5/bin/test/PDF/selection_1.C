{
//=========Macro generated from canvas: tempo4/
//=========  (Tue Sep 30 15:59:57 2014) by ROOT version5.34/09
   TCanvas *tempo4 = new TCanvas("tempo4", "",0,0,700,500);
   gStyle->SetOptStat(0);
   tempo4->SetHighLightColor(2);
   tempo4->Range(-16.12903,-4.682577e+11,112.9032,3.43389e+12);
   tempo4->SetFillColor(0);
   tempo4->SetBorderMode(0);
   tempo4->SetBorderSize(3);
   tempo4->SetLeftMargin(0.125);
   tempo4->SetBottomMargin(0.12);
   tempo4->SetFrameBorderMode(0);
   tempo4->SetFrameBorderSize(0);
   tempo4->SetFrameBorderMode(0);
   tempo4->SetFrameBorderSize(0);
   
   THStack *mystack = new THStack();
   mystack->SetName("mystack");
   mystack->SetTitle("");
   
   TH1F *mystack_stack_4 = new TH1F("mystack_stack_4","",20,0,100);
   mystack_stack_4->SetMinimum(0);
   mystack_stack_4->SetMaximum(3.043675e+12);
   mystack_stack_4->SetDirectory(0);
   mystack_stack_4->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   mystack_stack_4->SetLineColor(ci);
   mystack_stack_4->SetLineWidth(2);
   mystack_stack_4->GetXaxis()->SetTitle("p_{T} [ a ] (GeV/c) ");
   mystack_stack_4->GetXaxis()->SetLabelFont(22);
   mystack_stack_4->GetXaxis()->SetTitleSize(0.06);
   mystack_stack_4->GetXaxis()->SetTitleOffset(0.85);
   mystack_stack_4->GetXaxis()->SetTitleFont(22);
   mystack_stack_4->GetYaxis()->SetTitle("N. of a ( L_{int} = 10 fb^{-1} )");
   mystack_stack_4->GetYaxis()->SetLabelFont(22);
   mystack_stack_4->GetYaxis()->SetTitleSize(0.06);
   mystack_stack_4->GetYaxis()->SetTitleOffset(0.98);
   mystack_stack_4->GetYaxis()->SetTitleFont(22);
   mystack_stack_4->GetZaxis()->SetLabelFont(42);
   mystack_stack_4->GetZaxis()->SetLabelSize(0.035);
   mystack_stack_4->GetZaxis()->SetTitleSize(0.035);
   mystack_stack_4->GetZaxis()->SetTitleFont(42);
   mystack->SetHistogram(mystack_stack_4);
   
   
   TH1F *selection_1_2 = new TH1F("selection_1_2","selection_1_2",20,0,100);
   selection_1_2->SetBinContent(1,2.898738e+12);
   selection_1_2->SetEntries(20);
   selection_1_2->SetFillColor(9);
   selection_1_2->SetLineColor(9);
   selection_1_2->GetXaxis()->SetLabelFont(42);
   selection_1_2->GetXaxis()->SetLabelSize(0.035);
   selection_1_2->GetXaxis()->SetTitleSize(0.035);
   selection_1_2->GetXaxis()->SetTitleFont(42);
   selection_1_2->GetYaxis()->SetLabelFont(42);
   selection_1_2->GetYaxis()->SetLabelSize(0.035);
   selection_1_2->GetYaxis()->SetTitleSize(0.035);
   selection_1_2->GetYaxis()->SetTitleFont(42);
   selection_1_2->GetZaxis()->SetLabelFont(42);
   selection_1_2->GetZaxis()->SetLabelSize(0.035);
   selection_1_2->GetZaxis()->SetTitleSize(0.035);
   selection_1_2->GetZaxis()->SetTitleFont(42);
   mystack->Add(selection_1_2,"");
   mystack->Draw("");
   tempo4->Modified();
   tempo4->cd();
   tempo4->SetSelected(tempo4);
}

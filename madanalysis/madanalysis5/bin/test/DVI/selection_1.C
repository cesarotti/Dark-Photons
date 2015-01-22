{
//=========Macro generated from canvas: tempo6/
//=========  (Tue Sep 30 16:00:07 2014) by ROOT version5.34/09
   TCanvas *tempo6 = new TCanvas("tempo6", "",0,0,700,500);
   gStyle->SetOptStat(0);
   tempo6->SetHighLightColor(2);
   tempo6->Range(-16.12903,-4.682577e+11,112.9032,3.43389e+12);
   tempo6->SetFillColor(0);
   tempo6->SetBorderMode(0);
   tempo6->SetBorderSize(3);
   tempo6->SetLeftMargin(0.125);
   tempo6->SetBottomMargin(0.12);
   tempo6->SetFrameBorderMode(0);
   tempo6->SetFrameBorderSize(0);
   tempo6->SetFrameBorderMode(0);
   tempo6->SetFrameBorderSize(0);
   
   THStack *mystack = new THStack();
   mystack->SetName("mystack");
   mystack->SetTitle("");
   
   TH1F *mystack_stack_6 = new TH1F("mystack_stack_6","",20,0,100);
   mystack_stack_6->SetMinimum(0);
   mystack_stack_6->SetMaximum(3.043675e+12);
   mystack_stack_6->SetDirectory(0);
   mystack_stack_6->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   mystack_stack_6->SetLineColor(ci);
   mystack_stack_6->SetLineWidth(2);
   mystack_stack_6->GetXaxis()->SetTitle("p_{T} [ a ] (GeV/c) ");
   mystack_stack_6->GetXaxis()->SetLabelFont(22);
   mystack_stack_6->GetXaxis()->SetTitleSize(0.06);
   mystack_stack_6->GetXaxis()->SetTitleOffset(0.85);
   mystack_stack_6->GetXaxis()->SetTitleFont(22);
   mystack_stack_6->GetYaxis()->SetTitle("N. of a ( L_{int} = 10 fb^{-1} )");
   mystack_stack_6->GetYaxis()->SetLabelFont(22);
   mystack_stack_6->GetYaxis()->SetTitleSize(0.06);
   mystack_stack_6->GetYaxis()->SetTitleOffset(0.98);
   mystack_stack_6->GetYaxis()->SetTitleFont(22);
   mystack_stack_6->GetZaxis()->SetLabelFont(42);
   mystack_stack_6->GetZaxis()->SetLabelSize(0.035);
   mystack_stack_6->GetZaxis()->SetTitleSize(0.035);
   mystack_stack_6->GetZaxis()->SetTitleFont(42);
   mystack->SetHistogram(mystack_stack_6);
   
   
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
   tempo6->Modified();
   tempo6->cd();
   tempo6->SetSelected(tempo6);
}

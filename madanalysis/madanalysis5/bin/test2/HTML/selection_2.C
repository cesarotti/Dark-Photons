{
//=========Macro generated from canvas: tempo30/
//=========  (Tue Sep 30 16:04:17 2014) by ROOT version5.34/09
   TCanvas *tempo30 = new TCanvas("tempo30", "",0,0,700,500);
   gStyle->SetOptStat(0);
   tempo30->SetHighLightColor(2);
   tempo30->Range(-0.1612903,-3.335165e+11,1.129032,2.445788e+12);
   tempo30->SetFillColor(0);
   tempo30->SetBorderMode(0);
   tempo30->SetBorderSize(3);
   tempo30->SetLeftMargin(0.125);
   tempo30->SetBottomMargin(0.12);
   tempo30->SetFrameBorderMode(0);
   tempo30->SetFrameBorderSize(0);
   tempo30->SetFrameBorderMode(0);
   tempo30->SetFrameBorderSize(0);
   
   THStack *mystack = new THStack();
   mystack->SetName("mystack");
   mystack->SetTitle("");
   
   TH1F *mystack_stack_30 = new TH1F("mystack_stack_30","",50,0,1);
   mystack_stack_30->SetMinimum(0);
   mystack_stack_30->SetMaximum(2.167857e+12);
   mystack_stack_30->SetDirectory(0);
   mystack_stack_30->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   mystack_stack_30->SetLineColor(ci);
   mystack_stack_30->SetLineWidth(2);
   mystack_stack_30->GetXaxis()->SetTitle("p_{T} [ a ] (GeV/c) ");
   mystack_stack_30->GetXaxis()->SetLabelFont(22);
   mystack_stack_30->GetXaxis()->SetTitleSize(0.06);
   mystack_stack_30->GetXaxis()->SetTitleOffset(0.85);
   mystack_stack_30->GetXaxis()->SetTitleFont(22);
   mystack_stack_30->GetYaxis()->SetTitle("N. of a ( L_{int} = 10 fb^{-1} )");
   mystack_stack_30->GetYaxis()->SetLabelFont(22);
   mystack_stack_30->GetYaxis()->SetTitleSize(0.06);
   mystack_stack_30->GetYaxis()->SetTitleOffset(0.98);
   mystack_stack_30->GetYaxis()->SetTitleFont(22);
   mystack_stack_30->GetZaxis()->SetLabelFont(42);
   mystack_stack_30->GetZaxis()->SetLabelSize(0.035);
   mystack_stack_30->GetZaxis()->SetTitleSize(0.035);
   mystack_stack_30->GetZaxis()->SetTitleFont(42);
   mystack->SetHistogram(mystack_stack_30);
   
   
   TH1F *selection_2_12 = new TH1F("selection_2_12","selection_2_12",50,0,1);
   selection_2_12->SetBinContent(1,2.064626e+12);
   selection_2_12->SetBinContent(2,8.341119e+11);
   selection_2_12->SetEntries(50);
   selection_2_12->SetStats(0);
   selection_2_12->SetFillColor(9);
   selection_2_12->SetLineColor(9);
   selection_2_12->GetXaxis()->SetLabelFont(22);
   selection_2_12->GetXaxis()->SetLabelSize(0.03);
   selection_2_12->GetXaxis()->SetTitleOffset(0.85);
   selection_2_12->GetXaxis()->SetTitleFont(22);
   selection_2_12->GetYaxis()->SetLabelFont(22);
   selection_2_12->GetYaxis()->SetLabelSize(0.03);
   selection_2_12->GetYaxis()->SetTitleOffset(0.98);
   selection_2_12->GetYaxis()->SetTitleFont(22);
   selection_2_12->GetZaxis()->SetLabelFont(42);
   selection_2_12->GetZaxis()->SetLabelSize(0.035);
   selection_2_12->GetZaxis()->SetTitleSize(0.035);
   selection_2_12->GetZaxis()->SetTitleFont(42);
   mystack->Add(selection_2_12,"");
   mystack->Draw("");
   tempo30->Modified();
   tempo30->cd();
   tempo30->SetSelected(tempo30);
}

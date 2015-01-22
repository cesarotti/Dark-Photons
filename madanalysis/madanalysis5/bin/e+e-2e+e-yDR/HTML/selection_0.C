{
//=========Macro generated from canvas: tempo1/
//=========  (Mon Oct 20 12:35:38 2014) by ROOT version5.34/09
   TCanvas *tempo1 = new TCanvas("tempo1", "",0,0,700,500);
   gStyle->SetOptStat(0);
   tempo1->SetHighLightColor(2);
   tempo1->Range(-0.0008064516,-0.1538462,0.005645161,1.128205);
   tempo1->SetFillColor(0);
   tempo1->SetBorderMode(0);
   tempo1->SetBorderSize(3);
   tempo1->SetLeftMargin(0.125);
   tempo1->SetBottomMargin(0.12);
   tempo1->SetFrameBorderMode(0);
   tempo1->SetFrameBorderSize(0);
   tempo1->SetFrameBorderMode(0);
   tempo1->SetFrameBorderSize(0);
   
   THStack *mystack = new THStack();
   mystack->SetName("mystack");
   mystack->SetTitle("");
   
   TH1F *mystack_stack_1 = new TH1F("mystack_stack_1","",100,0,0.005);
   mystack_stack_1->SetMinimum(0);
   mystack_stack_1->SetMaximum(0);
   mystack_stack_1->SetDirectory(0);
   mystack_stack_1->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   mystack_stack_1->SetLineColor(ci);
   mystack_stack_1->SetLineWidth(2);
   mystack_stack_1->GetXaxis()->SetTitle("dR [ a e- ] ");
   mystack_stack_1->GetXaxis()->SetLabelFont(22);
   mystack_stack_1->GetXaxis()->SetTitleSize(0.06);
   mystack_stack_1->GetXaxis()->SetTitleOffset(0.85);
   mystack_stack_1->GetXaxis()->SetTitleFont(22);
   mystack_stack_1->GetYaxis()->SetTitle("N. of a e- pairs  ( L_{int} = 10 fb^{-1} )");
   mystack_stack_1->GetYaxis()->SetLabelFont(22);
   mystack_stack_1->GetYaxis()->SetTitleOffset(0.98);
   mystack_stack_1->GetYaxis()->SetTitleFont(22);
   mystack_stack_1->GetZaxis()->SetLabelFont(42);
   mystack_stack_1->GetZaxis()->SetLabelSize(0.035);
   mystack_stack_1->GetZaxis()->SetTitleSize(0.035);
   mystack_stack_1->GetZaxis()->SetTitleFont(42);
   mystack->SetHistogram(mystack_stack_1);
   
   
   TH1F *selection_0_1 = new TH1F("selection_0_1","selection_0_1",100,0,0.005);
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
   tempo1->Modified();
   tempo1->cd();
   tempo1->SetSelected(tempo1);
}

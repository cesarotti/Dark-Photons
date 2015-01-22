{
//=========Macro generated from canvas: tempo2/
//=========  (Mon Oct 20 12:35:38 2014) by ROOT version5.34/09
   TCanvas *tempo2 = new TCanvas("tempo2", "",0,0,700,500);
   gStyle->SetOptStat(0);
   tempo2->SetHighLightColor(2);
   tempo2->Range(-0.0008064516,-0.1538462,0.005645161,1.128205);
   tempo2->SetFillColor(0);
   tempo2->SetBorderMode(0);
   tempo2->SetBorderSize(3);
   tempo2->SetLeftMargin(0.125);
   tempo2->SetBottomMargin(0.12);
   tempo2->SetFrameBorderMode(0);
   tempo2->SetFrameBorderSize(0);
   tempo2->SetFrameBorderMode(0);
   tempo2->SetFrameBorderSize(0);
   
   THStack *mystack = new THStack();
   mystack->SetName("mystack");
   mystack->SetTitle("");
   
   TH1F *mystack_stack_2 = new TH1F("mystack_stack_2","",100,0,0.005);
   mystack_stack_2->SetMinimum(0);
   mystack_stack_2->SetMaximum(0);
   mystack_stack_2->SetDirectory(0);
   mystack_stack_2->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   mystack_stack_2->SetLineColor(ci);
   mystack_stack_2->SetLineWidth(2);
   mystack_stack_2->GetXaxis()->SetTitle("dR [ a e- ] ");
   mystack_stack_2->GetXaxis()->SetLabelFont(22);
   mystack_stack_2->GetXaxis()->SetTitleSize(0.06);
   mystack_stack_2->GetXaxis()->SetTitleOffset(0.85);
   mystack_stack_2->GetXaxis()->SetTitleFont(22);
   mystack_stack_2->GetYaxis()->SetTitle("N. of a e- pairs  ( L_{int} = 10 fb^{-1} )");
   mystack_stack_2->GetYaxis()->SetLabelFont(22);
   mystack_stack_2->GetYaxis()->SetTitleOffset(0.98);
   mystack_stack_2->GetYaxis()->SetTitleFont(22);
   mystack_stack_2->GetZaxis()->SetLabelFont(42);
   mystack_stack_2->GetZaxis()->SetLabelSize(0.035);
   mystack_stack_2->GetZaxis()->SetTitleSize(0.035);
   mystack_stack_2->GetZaxis()->SetTitleFont(42);
   mystack->SetHistogram(mystack_stack_2);
   
   
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
   tempo2->Modified();
   tempo2->cd();
   tempo2->SetSelected(tempo2);
}

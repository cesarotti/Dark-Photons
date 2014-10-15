{
//=========Macro generated from canvas: tempo10/
//=========  (Wed Oct  1 15:49:39 2014) by ROOT version5.34/09
   TCanvas *tempo10 = new TCanvas("tempo10", "",0,0,700,500);
   gStyle->SetOptStat(0);
   tempo10->SetHighLightColor(2);
   tempo10->Range(-0.008064516,-0.1538462,0.05645161,1.128205);
   tempo10->SetFillColor(0);
   tempo10->SetBorderMode(0);
   tempo10->SetBorderSize(3);
   tempo10->SetLeftMargin(0.125);
   tempo10->SetBottomMargin(0.12);
   tempo10->SetFrameBorderMode(0);
   tempo10->SetFrameBorderSize(0);
   tempo10->SetFrameBorderMode(0);
   tempo10->SetFrameBorderSize(0);
   
   THStack *mystack = new THStack();
   mystack->SetName("mystack");
   mystack->SetTitle("");
   
   TH1F *mystack_stack_10 = new TH1F("mystack_stack_10","",100,0,0.05);
   mystack_stack_10->SetMinimum(0);
   mystack_stack_10->SetMaximum(0);
   mystack_stack_10->SetDirectory(0);
   mystack_stack_10->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   mystack_stack_10->SetLineColor(ci);
   mystack_stack_10->SetLineWidth(2);
   mystack_stack_10->GetXaxis()->SetTitle("p_{T} [ a ] (GeV/c) ");
   mystack_stack_10->GetXaxis()->SetLabelFont(22);
   mystack_stack_10->GetXaxis()->SetTitleSize(0.06);
   mystack_stack_10->GetXaxis()->SetTitleOffset(0.85);
   mystack_stack_10->GetXaxis()->SetTitleFont(22);
   mystack_stack_10->GetYaxis()->SetTitle("N. of a ( L_{int} = 10 fb^{-1} )");
   mystack_stack_10->GetYaxis()->SetLabelFont(22);
   mystack_stack_10->GetYaxis()->SetTitleSize(0.06);
   mystack_stack_10->GetYaxis()->SetTitleOffset(0.98);
   mystack_stack_10->GetYaxis()->SetTitleFont(22);
   mystack_stack_10->GetZaxis()->SetLabelFont(42);
   mystack_stack_10->GetZaxis()->SetLabelSize(0.035);
   mystack_stack_10->GetZaxis()->SetTitleSize(0.035);
   mystack_stack_10->GetZaxis()->SetTitleFont(42);
   mystack->SetHistogram(mystack_stack_10);
   
   
   TH1F *selection_0_2 = new TH1F("selection_0_2","selection_0_2",100,0,0.05);
   selection_0_2->SetEntries(100);
   selection_0_2->SetStats(0);
   selection_0_2->SetFillColor(9);
   selection_0_2->SetLineColor(9);
   selection_0_2->GetXaxis()->SetLabelFont(22);
   selection_0_2->GetXaxis()->SetLabelSize(0.03);
   selection_0_2->GetXaxis()->SetTitleOffset(0.85);
   selection_0_2->GetXaxis()->SetTitleFont(22);
   selection_0_2->GetYaxis()->SetLabelFont(22);
   selection_0_2->GetYaxis()->SetLabelSize(0.03);
   selection_0_2->GetYaxis()->SetTitleOffset(0.98);
   selection_0_2->GetYaxis()->SetTitleFont(22);
   selection_0_2->GetZaxis()->SetLabelFont(42);
   selection_0_2->GetZaxis()->SetLabelSize(0.035);
   selection_0_2->GetZaxis()->SetTitleSize(0.035);
   selection_0_2->GetZaxis()->SetTitleFont(42);
   mystack->Add(selection_0_2,"");
   mystack->Draw("");
   tempo10->Modified();
   tempo10->cd();
   tempo10->SetSelected(tempo10);
}

{
//=========Macro generated from canvas: tempo7/
//=========  (Wed Oct  1 15:49:38 2014) by ROOT version5.34/09
   TCanvas *tempo7 = new TCanvas("tempo7", "",0,0,700,500);
   gStyle->SetOptStat(0);
   tempo7->SetHighLightColor(2);
   tempo7->Range(-0.008064516,-0.1538462,0.05645161,1.128205);
   tempo7->SetFillColor(0);
   tempo7->SetBorderMode(0);
   tempo7->SetBorderSize(3);
   tempo7->SetLeftMargin(0.125);
   tempo7->SetBottomMargin(0.12);
   tempo7->SetFrameBorderMode(0);
   tempo7->SetFrameBorderSize(0);
   tempo7->SetFrameBorderMode(0);
   tempo7->SetFrameBorderSize(0);
   
   THStack *mystack = new THStack();
   mystack->SetName("mystack");
   mystack->SetTitle("");
   
   TH1F *mystack_stack_7 = new TH1F("mystack_stack_7","",100,0,0.05);
   mystack_stack_7->SetMinimum(0);
   mystack_stack_7->SetMaximum(0);
   mystack_stack_7->SetDirectory(0);
   mystack_stack_7->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   mystack_stack_7->SetLineColor(ci);
   mystack_stack_7->SetLineWidth(2);
   mystack_stack_7->GetXaxis()->SetTitle("p_{T} [ a ] (GeV/c) ");
   mystack_stack_7->GetXaxis()->SetLabelFont(22);
   mystack_stack_7->GetXaxis()->SetTitleSize(0.06);
   mystack_stack_7->GetXaxis()->SetTitleOffset(0.85);
   mystack_stack_7->GetXaxis()->SetTitleFont(22);
   mystack_stack_7->GetYaxis()->SetTitle("N. of a ( L_{int} = 10 fb^{-1} )");
   mystack_stack_7->GetYaxis()->SetLabelFont(22);
   mystack_stack_7->GetYaxis()->SetTitleSize(0.06);
   mystack_stack_7->GetYaxis()->SetTitleOffset(0.98);
   mystack_stack_7->GetYaxis()->SetTitleFont(22);
   mystack_stack_7->GetZaxis()->SetLabelFont(42);
   mystack_stack_7->GetZaxis()->SetLabelSize(0.035);
   mystack_stack_7->GetZaxis()->SetTitleSize(0.035);
   mystack_stack_7->GetZaxis()->SetTitleFont(42);
   mystack->SetHistogram(mystack_stack_7);
   
   
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
   tempo7->Modified();
   tempo7->cd();
   tempo7->SetSelected(tempo7);
}

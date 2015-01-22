{
//=========Macro generated from canvas: tempo31/
//=========  (Tue Sep 30 16:04:17 2014) by ROOT version5.34/09
   TCanvas *tempo31 = new TCanvas("tempo31", "",0,0,700,500);
   gStyle->SetOptStat(0);
   tempo31->SetHighLightColor(2);
   tempo31->Range(-0.01612903,-7.235049e+10,0.1129032,5.305703e+11);
   tempo31->SetFillColor(0);
   tempo31->SetBorderMode(0);
   tempo31->SetBorderSize(3);
   tempo31->SetLeftMargin(0.125);
   tempo31->SetBottomMargin(0.12);
   tempo31->SetFrameBorderMode(0);
   tempo31->SetFrameBorderSize(0);
   tempo31->SetFrameBorderMode(0);
   tempo31->SetFrameBorderSize(0);
   
   THStack *mystack = new THStack();
   mystack->SetName("mystack");
   mystack->SetTitle("");
   
   TH1F *mystack_stack_31 = new TH1F("mystack_stack_31","",50,0,0.1);
   mystack_stack_31->SetMinimum(0);
   mystack_stack_31->SetMaximum(4.702782e+11);
   mystack_stack_31->SetDirectory(0);
   mystack_stack_31->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   mystack_stack_31->SetLineColor(ci);
   mystack_stack_31->SetLineWidth(2);
   mystack_stack_31->GetXaxis()->SetTitle("p_{T} [ a ] (GeV/c) ");
   mystack_stack_31->GetXaxis()->SetLabelFont(22);
   mystack_stack_31->GetXaxis()->SetTitleSize(0.06);
   mystack_stack_31->GetXaxis()->SetTitleOffset(0.85);
   mystack_stack_31->GetXaxis()->SetTitleFont(22);
   mystack_stack_31->GetYaxis()->SetTitle("N. of a ( L_{int} = 10 fb^{-1} )");
   mystack_stack_31->GetYaxis()->SetLabelFont(22);
   mystack_stack_31->GetYaxis()->SetTitleSize(0.06);
   mystack_stack_31->GetYaxis()->SetTitleOffset(0.98);
   mystack_stack_31->GetYaxis()->SetTitleFont(22);
   mystack_stack_31->GetZaxis()->SetLabelFont(42);
   mystack_stack_31->GetZaxis()->SetLabelSize(0.035);
   mystack_stack_31->GetZaxis()->SetTitleSize(0.035);
   mystack_stack_31->GetZaxis()->SetTitleFont(42);
   mystack->SetHistogram(mystack_stack_31);
   
   
   TH1F *selection_3_13 = new TH1F("selection_3_13","selection_3_13",50,0,0.1);
   selection_3_13->SetBinContent(2,3.028022e+11);
   selection_3_13->SetBinContent(3,4.47884e+11);
   selection_3_13->SetBinContent(4,3.084837e+11);
   selection_3_13->SetBinContent(5,2.471754e+11);
   selection_3_13->SetBinContent(6,1.989983e+11);
   selection_3_13->SetBinContent(7,1.714603e+11);
   selection_3_13->SetBinContent(8,1.434875e+11);
   selection_3_13->SetBinContent(9,1.283851e+11);
   selection_3_13->SetBinContent(10,1.159495e+11);
   selection_3_13->SetBinContent(11,1.058039e+11);
   selection_3_13->SetBinContent(12,9.690481e+10);
   selection_3_13->SetBinContent(13,9.009277e+10);
   selection_3_13->SetBinContent(14,8.586061e+10);
   selection_3_13->SetBinContent(15,8.475909e+10);
   selection_3_13->SetBinContent(16,8.765783e+10);
   selection_3_13->SetBinContent(17,1.000645e+11);
   selection_3_13->SetBinContent(18,1.829684e+11);
   selection_3_13->SetEntries(50);
   selection_3_13->SetStats(0);
   selection_3_13->SetFillColor(9);
   selection_3_13->SetLineColor(9);
   selection_3_13->GetXaxis()->SetLabelFont(22);
   selection_3_13->GetXaxis()->SetLabelSize(0.03);
   selection_3_13->GetXaxis()->SetTitleOffset(0.85);
   selection_3_13->GetXaxis()->SetTitleFont(22);
   selection_3_13->GetYaxis()->SetLabelFont(22);
   selection_3_13->GetYaxis()->SetLabelSize(0.03);
   selection_3_13->GetYaxis()->SetTitleOffset(0.98);
   selection_3_13->GetYaxis()->SetTitleFont(22);
   selection_3_13->GetZaxis()->SetLabelFont(42);
   selection_3_13->GetZaxis()->SetLabelSize(0.035);
   selection_3_13->GetZaxis()->SetTitleSize(0.035);
   selection_3_13->GetZaxis()->SetTitleFont(42);
   mystack->Add(selection_3_13,"");
   mystack->Draw("");
   tempo31->Modified();
   tempo31->cd();
   tempo31->SetSelected(tempo31);
}

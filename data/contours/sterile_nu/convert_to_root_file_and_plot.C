//
// This macro reads in the various *data files,
// converts them to ROOT format,
// sets standard drawing options,
// and plots the appearance and disappearance limits.
//
// C.A.
//

{
  TCanvas * c = new TCanvas("c","",20,20,1500,600);
  c->Divide(2,1);
  c->Draw();

  //
  // Appearance
  //

  TH2F * hfapp = (TH2F*) c->cd(1)->DrawFrame(3E-4,6E-2,2E-1,1.5E+1);
  hfapp->Draw();
  hfapp->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu e}");
  hfapp->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2}/c^{4})");
  c->cd(1)->SetLogx();
  c->cd(1)->SetLogy();

  c->cd(1);

  TGraph * lim_app_3p1_nomad_99pctCL = new TGraph("./app_3+1_NOMAD_99pctCL.data");
  lim_app_3p1_nomad_99pctCL->SetLineWidth(3);
  lim_app_3p1_nomad_99pctCL->SetLineStyle(kSolid);
  lim_app_3p1_nomad_99pctCL->SetLineColor(kCyan-3);
  lim_app_3p1_nomad_99pctCL->Draw("l");
  TGraph * lim_app_3p1_bnl776_99pctCL = new TGraph("./app_3+1_BNL776_99pctCL.data");
  lim_app_3p1_bnl776_99pctCL->SetLineWidth(3);
  lim_app_3p1_bnl776_99pctCL->SetLineStyle(kSolid);
  lim_app_3p1_bnl776_99pctCL->SetLineColor(kGreen+1);
  lim_app_3p1_bnl776_99pctCL->Draw("l");
  TGraph * lim_app_3p1_karmen_99pctCL = new TGraph("./app_3+1_KARMEN_99pctCL.data");
  lim_app_3p1_karmen_99pctCL->SetLineWidth(3);
  lim_app_3p1_karmen_99pctCL->SetLineStyle(kSolid);
  lim_app_3p1_karmen_99pctCL->SetLineColor(kAzure+9);
  lim_app_3p1_karmen_99pctCL->Draw("l");
  TGraph * lim_app_3p1_icarus_99pctCL = new TGraph("./app_3+1_ICARUS_99pctCL.data");
  lim_app_3p1_icarus_99pctCL->SetLineWidth(4);
  lim_app_3p1_icarus_99pctCL->SetLineStyle(kDashed);
  lim_app_3p1_icarus_99pctCL->SetLineColor(kGreen+3);
  lim_app_3p1_icarus_99pctCL->Draw("l");
  TGraph * lim_app_3p1_opera_99pctCL = new TGraph("./app_3+1_OPERA_99pctCL.data");
  lim_app_3p1_opera_99pctCL->SetLineWidth(2);
  lim_app_3p1_opera_99pctCL->SetLineStyle(kSolid);
  lim_app_3p1_opera_99pctCL->SetLineColor(kGreen+3);
  lim_app_3p1_opera_99pctCL->Draw("l");
  TGraph * lim_app_3p1_miniboonenu_99pctCL = new TGraph("./app_3+1_MiniBooNEnu_99pctCL.data");
  lim_app_3p1_miniboonenu_99pctCL->SetLineWidth(4);
  lim_app_3p1_miniboonenu_99pctCL->SetLineStyle(kDashDotted);
  lim_app_3p1_miniboonenu_99pctCL->SetLineColor(kViolet-6);
  lim_app_3p1_miniboonenu_99pctCL->Draw("l");
  TGraph * lim_app_3p1_lsndr1_99pctCL = new TGraph("./app_3+1_LSNDwDiF1_99pctCL.data");
  lim_app_3p1_lsndr1_99pctCL->SetLineWidth(401);
  lim_app_3p1_lsndr1_99pctCL->SetFillStyle(3005);
  lim_app_3p1_lsndr1_99pctCL->SetLineColor(kOrange+10);
  lim_app_3p1_lsndr1_99pctCL->SetFillColor(kOrange+6);
  lim_app_3p1_lsndr1_99pctCL->Draw("l");
  TGraph * lim_app_3p1_lsndr2_99pctCL = new TGraph("./app_3+1_LSNDwDiF2_99pctCL.data");
  lim_app_3p1_lsndr2_99pctCL->SetLineWidth(101);
  lim_app_3p1_lsndr2_99pctCL->SetFillStyle(3005);
  lim_app_3p1_lsndr2_99pctCL->SetLineColor(kOrange+10);
  lim_app_3p1_lsndr2_99pctCL->SetFillColor(kOrange+6);
  lim_app_3p1_lsndr2_99pctCL->Draw("l");
  TGraph * lim_app_3p1_lsndr3_99pctCL = new TGraph("./app_3+1_LSNDwDiF3_99pctCL.data");
  lim_app_3p1_lsndr3_99pctCL->SetLineWidth(-101);
  lim_app_3p1_lsndr3_99pctCL->SetFillStyle(3005);
  lim_app_3p1_lsndr3_99pctCL->SetLineColor(kOrange+10);
  lim_app_3p1_lsndr3_99pctCL->SetFillColor(kOrange+6);
  lim_app_3p1_lsndr3_99pctCL->Draw("l");
  TMultiGraph * lim_app_3p1_lsnd_99pctCL = new TMultiGraph();
  lim_app_3p1_lsnd_99pctCL->Add(lim_app_3p1_lsndr1_99pctCL);// region 1 (high Dmsq)
  lim_app_3p1_lsnd_99pctCL->Add(lim_app_3p1_lsndr2_99pctCL);// region 2 (intermediate Dmsq)
  lim_app_3p1_lsnd_99pctCL->Add(lim_app_3p1_lsndr3_99pctCL);// region 3 (low Dmsq)

  TLegend * lgapp = new TLegend(0.7,0.8,0.9,0.9);
  lgapp->SetFillColor(0);
  lgapp->SetBorderSize(0);
  lgapp->AddEntry(lim_app_3p1_nomad_99pctCL,       "NOMAD 99\% CL",           "L");
  lgapp->AddEntry(lim_app_3p1_bnl776_99pctCL,      "BNL776 99\% CL",          "L");
  lgapp->AddEntry(lim_app_3p1_karmen_99pctCL,      "KARMEN 99\% CL",          "L");
  lgapp->AddEntry(lim_app_3p1_icarus_99pctCL,      "ICARUS 99\% CL",          "L");
  lgapp->AddEntry(lim_app_3p1_opera_99pctCL,       "OPERA 99\% CL",           "L");
  lgapp->AddEntry(lim_app_3p1_miniboonenu_99pctCL, "MiniBooNE (#nu) 99\% CL", "L");
  lgapp->AddEntry(lim_app_3p1_lsndr1_99pctCL,      "LSBD w/ DiF 99\% CL",     "LF");
  lgapp->Draw();

  //
  // Disappearance
  //

  TH2F * hfdis = (TH2F*) c->cd(2)->DrawFrame(3E-4,2E-4,1,8E+2);
  hfdis->Draw();
  hfdis->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu #mu}");
  hfdis->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2}/c^{4})");
  c->cd(2)->SetLogx();
  c->cd(2)->SetLogy();

  c->cd(2);

  TGraph * lim_dis_3p1_minosp_90pctCL = new TGraph("./disap_3+1_MINOS_MINOS+_90pctCL.data");
  lim_dis_3p1_minosp_90pctCL->SetLineWidth(3);
  lim_dis_3p1_minosp_90pctCL->SetLineStyle(kSolid);
  lim_dis_3p1_minosp_90pctCL->SetLineColor(kGreen+3);
  lim_dis_3p1_minosp_90pctCL->Draw("l");

  TLegend * lgdis = new TLegend(0.7,0.8,0.9,0.9);
  lgdis->SetFillColor(0);
  lgdis->SetBorderSize(0);
  lgdis->AddEntry(lim_dis_3p1_minosp_90pctCL,  "MINOS/MINOS+ 90\% CL",  "L");
  lgdis->Draw();

  TFile f("./sterile_3p1_limits.root","recreate");
  lim_app_3p1_nomad_99pctCL        -> Write( "lim_app_3p1_nomad_99pctCL"        );
  lim_app_3p1_bnl776_99pctCL       -> Write( "lim_app_3p1_bnl776_99pctCL"       );
  lim_app_3p1_karmen_99pctCL       -> Write( "lim_app_3p1_karmen_99pctCL"       );
  lim_app_3p1_icarus_99pctCL       -> Write( "lim_app_3p1_icarus_99pctCL"       );
  lim_app_3p1_opera_99pctCL        -> Write( "lim_app_3p1_opera_99pctCL"        );
  lim_app_3p1_miniboonenu_99pctCL  -> Write( "lim_app_3p1_miniboonenu_99pctCL"  );
  lim_app_3p1_lsndr1_99pctCL       -> Write( "lim_app_3p1_lsndr1_99pctCL"       );
  lim_app_3p1_lsndr2_99pctCL       -> Write( "lim_app_3p1_lsndr2_99pctCL"       );
  lim_app_3p1_lsndr3_99pctCL       -> Write( "lim_app_3p1_lsndr3_99pctCL"       );
  lim_dis_3p1_minosp_90pctCL       -> Write( "lim_dis_3p1_minosp_90pctCL"       );
  f.Close();
}

//
// This macro reads in the ROOT file produced by convert_to_root_file_and_plot.C
// and plots the appearance and disappearance limits.
//
// C.A.
//

{
  TCanvas * c = new TCanvas("c","",20,20,1500,600);
  c->Divide(2,1);
  c->Draw();

  TFile * f = new TFile("./sterile_3p1_limits.root","read");

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

  TGraph * lim_app_3p1_nomad_99pctCL        = (TGraph *) f->Get( "lim_app_3p1_nomad_99pctCL"        );
  TGraph * lim_app_3p1_bnl776_99pctCL       = (TGraph *) f->Get( "lim_app_3p1_bnl776_99pctCL"       );
  TGraph * lim_app_3p1_karmen_99pctCL       = (TGraph *) f->Get( "lim_app_3p1_karmen_99pctCL"       );
  TGraph * lim_app_3p1_icarus_99pctCL       = (TGraph *) f->Get( "lim_app_3p1_icarus_99pctCL"       );
  TGraph * lim_app_3p1_opera_99pctCL        = (TGraph *) f->Get( "lim_app_3p1_opera_99pctCL"        );
  TGraph * lim_app_3p1_miniboonenu_99pctCL  = (TGraph *) f->Get( "lim_app_3p1_miniboonenu_99pctCL"  );
  TGraph * lim_app_3p1_lsndr1_99pctCL       = (TGraph *) f->Get( "lim_app_3p1_lsndr1_99pctCL"       );
  TGraph * lim_app_3p1_lsndr2_99pctCL       = (TGraph *) f->Get( "lim_app_3p1_lsndr2_99pctCL"       );
  TGraph * lim_app_3p1_lsndr3_99pctCL       = (TGraph *) f->Get( "lim_app_3p1_lsndr3_99pctCL"       );

  lim_app_3p1_nomad_99pctCL        -> Draw("l");
  lim_app_3p1_bnl776_99pctCL       -> Draw("l");
  lim_app_3p1_karmen_99pctCL       -> Draw("l");
  lim_app_3p1_icarus_99pctCL       -> Draw("l");
  lim_app_3p1_opera_99pctCL        -> Draw("l");
  lim_app_3p1_miniboonenu_99pctCL  -> Draw("l");
  lim_app_3p1_lsndr1_99pctCL       -> Draw("l");
  lim_app_3p1_lsndr2_99pctCL       -> Draw("l");
  lim_app_3p1_lsndr3_99pctCL       -> Draw("l");

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

  TGraph * lim_dis_3p1_minosp_90pctCL = (TGraph *) f->Get( "lim_dis_3p1_minosp_90pctCL");

  lim_dis_3p1_minosp_90pctCL->Draw("l");

  TLegend * lgdis = new TLegend(0.7,0.8,0.9,0.9);
  lgdis->SetFillColor(0);
  lgdis->SetBorderSize(0);
  lgdis->AddEntry(lim_dis_3p1_minosp_90pctCL,  "MINOS/MINOS+ 90\% CL",  "L");
  lgdis->Draw();
}

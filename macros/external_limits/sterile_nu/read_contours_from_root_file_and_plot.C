// -----------------------------------------------------------------------------
// This macro reads-in sterile neutrino limits from the ROOT file produced by
// convert_contours_to_root_format_and_plot.C.
// It plots 3 sets limit compilations:
// - limits in Dm^2_41, sin^2(2\theta_\mu\mu) from nu_mu disappearance data
// - limits in Dm^2_41, sin^2(2\theta_ee)     from nu_e appearance data
// - limits in Dm^2_41, sin^2(2\theta_\mu e)  from nu_e disappearance data
//
// Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
//
// Copyright (c) 2010-2020, The VALOR group. All rights reserved
// -----------------------------------------------------------------------------

void read_contours_from_root_file_and_plot(
   const char * data_file="./sterile_3p1_limits.root")
{
  TFile * f = new TFile(data_file,"read");

  //
  // nue apearance
  //

  TCanvas * cnueapp = new TCanvas("cnueapp","",20,20,900,600);
  cnueapp->Draw();

  TH2F * hfnueapp = (TH2F*) cnueapp->DrawFrame(1E-4,6E-2,2E-1,1.5E+1);
  hfnueapp->Draw();
  hfnueapp->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu e}");
  hfnueapp->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2}/c^{4})");
  cnueapp->SetLogx();
  cnueapp->SetLogy();

  TGraph * lim_nueapp_3p1_sbnproposal_90pctCL  = (TGraph *) f->Get( "lim_nueapp_3p1_sbnproposal_90pctCL"  );
  TGraph * lim_nueapp_3p1_sbnproposal_3sigCL   = (TGraph *) f->Get( "lim_nueapp_3p1_sbnproposal_3sigCL"   );
  TGraph * lim_nueapp_3p1_sbnproposal_5sigCL   = (TGraph *) f->Get( "lim_nueapp_3p1_sbnproposal_5sigCL"   );
  TGraph * lim_nueapp_3p1_nomad_99pctCL        = (TGraph *) f->Get( "lim_nueapp_3p1_nomad_99pctCL"        );
  TGraph * lim_nueapp_3p1_bnl776_99pctCL       = (TGraph *) f->Get( "lim_nueapp_3p1_bnl776_99pctCL"       );
  TGraph * lim_nueapp_3p1_karmen_99pctCL       = (TGraph *) f->Get( "lim_nueapp_3p1_karmen_99pctCL"       );
  TGraph * lim_nueapp_3p1_icarus_99pctCL       = (TGraph *) f->Get( "lim_nueapp_3p1_icarus_99pctCL"       );
  TGraph * lim_nueapp_3p1_opera_99pctCL        = (TGraph *) f->Get( "lim_nueapp_3p1_opera_99pctCL"        );
  TGraph * lim_nueapp_3p1_miniboonenu_99pctCL  = (TGraph *) f->Get( "lim_nueapp_3p1_miniboonenu_99pctCL"  );
  TGraph * lim_nueapp_3p1_lsndr1_99pctCL       = (TGraph *) f->Get( "lim_nueapp_3p1_lsndr1_99pctCL"       );
  TGraph * lim_nueapp_3p1_lsndr2_99pctCL       = (TGraph *) f->Get( "lim_nueapp_3p1_lsndr2_99pctCL"       );
  TGraph * lim_nueapp_3p1_lsndr3_99pctCL       = (TGraph *) f->Get( "lim_nueapp_3p1_lsndr3_99pctCL"       );

  lim_nueapp_3p1_sbnproposal_90pctCL  -> Draw("l");
  lim_nueapp_3p1_sbnproposal_3sigCL   -> Draw("l");
  lim_nueapp_3p1_sbnproposal_5sigCL   -> Draw("l");
  lim_nueapp_3p1_nomad_99pctCL        -> Draw("l");
  lim_nueapp_3p1_bnl776_99pctCL       -> Draw("l");
  lim_nueapp_3p1_karmen_99pctCL       -> Draw("l");
  lim_nueapp_3p1_icarus_99pctCL       -> Draw("l");
  lim_nueapp_3p1_opera_99pctCL        -> Draw("l");
  lim_nueapp_3p1_miniboonenu_99pctCL  -> Draw("l");
  lim_nueapp_3p1_lsndr1_99pctCL       -> Draw("l");
  lim_nueapp_3p1_lsndr2_99pctCL       -> Draw("l");
  lim_nueapp_3p1_lsndr3_99pctCL       -> Draw("l");

  TLegend * lgnueapp = new TLegend(0.13,0.1,0.35,0.4);
  lgnueapp->SetFillColor(0);
  lgnueapp->SetFillStyle(0);
  lgnueapp->SetBorderSize(0);
  lgnueapp->AddEntry(lim_nueapp_3p1_sbnproposal_90pctCL, "SBN proposal 90\% CL",    "L");
  lgnueapp->AddEntry(lim_nueapp_3p1_sbnproposal_3sigCL,  "SBN proposal 3#sigma CL", "L");
  lgnueapp->AddEntry(lim_nueapp_3p1_sbnproposal_5sigCL,  "SBN proposal 5#sigma CL", "L");
  lgnueapp->AddEntry(lim_nueapp_3p1_nomad_99pctCL,       "NOMAD 99\% CL",           "L");
  lgnueapp->AddEntry(lim_nueapp_3p1_bnl776_99pctCL,      "BNL776 99\% CL",          "L");
  lgnueapp->AddEntry(lim_nueapp_3p1_karmen_99pctCL,      "KARMEN 99\% CL",          "L");
  lgnueapp->AddEntry(lim_nueapp_3p1_icarus_99pctCL,      "ICARUS 99\% CL",          "L");
  lgnueapp->AddEntry(lim_nueapp_3p1_opera_99pctCL,       "OPERA 99\% CL",           "L");
  lgnueapp->AddEntry(lim_nueapp_3p1_miniboonenu_99pctCL, "MiniBooNE (#nu) 99\% CL", "L");
  lgnueapp->AddEntry(lim_nueapp_3p1_lsndr1_99pctCL,      "LSBD w/ DiF 99\% CL",     "LF");
  lgnueapp->Draw();

  //
  // numu disappearance
  //

  TCanvas * cnumudis = new TCanvas("cnumudis","",20,20,900,600);
  cnumudis->Draw();

  TH2F * hfnumudis = (TH2F*) cnumudis->DrawFrame(3E-4,1E-3,1,2E+2);
  hfnumudis->Draw();
  hfnumudis->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu #mu}");
  hfnumudis->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2}/c^{4})");
  cnumudis->SetLogx();
  cnumudis->SetLogy();

  TGraph * lim_numudis_3p1_sbnproposal_90pctCL  = (TGraph *) f->Get( "lim_numudis_3p1_sbnproposal_90pctCL"  );
  TGraph * lim_numudis_3p1_sbnproposal_3sigCL   = (TGraph *) f->Get( "lim_numudis_3p1_sbnproposal_3sigCL"   );
  TGraph * lim_numudis_3p1_sbnproposal_5sigCL   = (TGraph *) f->Get( "lim_numudis_3p1_sbnproposal_5sigCL"   );
  TGraph * lim_numudis_3p1_minosp_90pctCL       = (TGraph *) f->Get( "lim_numudis_3p1_minosp_90pctCL"       );
  TGraph * lim_numudis_3p1_minisciboone_90pctCL = (TGraph *) f->Get( "lim_numudis_3p1_minisciboone_90pctCL" );
  TGraph * lim_numudis_3p1_icecube_90pctCL      = (TGraph *) f->Get( "lim_numudis_3p1_icecube_90pctCL"      );
  TGraph * lim_numudis_3p1_icecube_99pctCL      = (TGraph *) f->Get( "lim_numudis_3p1_icecube_99pctCL"      );

  lim_numudis_3p1_sbnproposal_90pctCL  -> Draw("l");
  lim_numudis_3p1_sbnproposal_3sigCL   -> Draw("l");
  lim_numudis_3p1_sbnproposal_5sigCL   -> Draw("l");
  lim_numudis_3p1_minosp_90pctCL       -> Draw("l");
  lim_numudis_3p1_minisciboone_90pctCL -> Draw("l");
  lim_numudis_3p1_icecube_90pctCL      -> Draw("l");
  lim_numudis_3p1_icecube_99pctCL      -> Draw("l");

  TLegend * lgnumudis = new TLegend(0.13,0.10,0.35,0.45);
  lgnumudis->SetFillColor(0);
  lgnumudis->SetFillStyle(0);
  lgnumudis->SetBorderSize(0);
  lgnumudis->AddEntry(lim_numudis_3p1_sbnproposal_90pctCL,  "SBN proposal 90\% CL",       "L");
  lgnumudis->AddEntry(lim_numudis_3p1_sbnproposal_3sigCL,   "SBN proposal 3#sigma CL",    "L");
  lgnumudis->AddEntry(lim_numudis_3p1_sbnproposal_5sigCL,   "SBN proposal 5#sigma CL",    "L");
  lgnumudis->AddEntry(lim_numudis_3p1_minosp_90pctCL,       "MINOS/MINOS+ 90\% CL",       "L");
  lgnumudis->AddEntry(lim_numudis_3p1_minisciboone_90pctCL, "MiniBooNE/SciBooNE 90\% CL", "L");
  lgnumudis->AddEntry(lim_numudis_3p1_icecube_90pctCL,      "IceCube 90\% CL",            "L");
  lgnumudis->AddEntry(lim_numudis_3p1_icecube_99pctCL,      "IceCube 99\% CL",            "L");
  lgnumudis->Draw();

  //
  // nue disappearance
  //

  TCanvas * cnuedis = new TCanvas("cnuedis","",20,20,900,600);
  cnumudis->Draw();

  TH2F * hfnuedis = (TH2F*) cnuedis->DrawFrame(1E-2,1E-2,1,1E+2);
  hfnuedis->Draw();
  hfnuedis->GetXaxis()->SetTitle("sin^{2}2#theta_{e e}");
  hfnuedis->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2}/c^{4})");
  cnuedis->SetLogx();
  cnuedis->SetLogy();

  TGraph * lim_nuedis_3p1_t2knd280excluded_95pctCL  = (TGraph *) f->Get("lim_nuedis_3p1_t2knd280excluded_95pctCL");
  TGraph * lim_nuedis_3p1_t2knd280allowed_90pctCL   = (TGraph *) f->Get("lim_nuedis_3p1_t2knd280allowed_90pctCL");
  TGraph * lim_nuedis_3p1_t2knd280allowed_68pctCL   = (TGraph *) f->Get("lim_nuedis_3p1_t2knd280allowed_68pctCL");
  TGraph * lim_nueapp_3p1_reactor_allowed_1_95pctCL = (TGraph *) f->Get("lim_nueapp_3p1_reactor_allowed_1_95pctCL");
  TGraph * lim_nueapp_3p1_reactor_allowed_2_95pctCL = (TGraph *) f->Get("lim_nueapp_3p1_reactor_allowed_2_95pctCL");
  TGraph * lim_nueapp_3p1_reactor_allowed_3_95pctCL = (TGraph *) f->Get("lim_nueapp_3p1_reactor_allowed_3_95pctCL");
  TGraph * lim_nueapp_3p1_gallium_allowed_95pctCL   = (TGraph *) f->Get("lim_nueapp_3p1_gallium_allowed_95pctCL");
  TGraph * lim_nueapp_3p1_solar_excluded_95pctCL    = (TGraph *) f->Get("lim_nueapp_3p1_solar_excluded_95pctCL");
  TGraph * lim_nueapp_3p1_nueC_excluded_95pctCL     = (TGraph *) f->Get("lim_nueapp_3p1_nueC_excluded_95pctCL");

  lim_nuedis_3p1_t2knd280excluded_95pctCL  -> Draw("l");
  lim_nuedis_3p1_t2knd280allowed_90pctCL   -> Draw("l");
  lim_nuedis_3p1_t2knd280allowed_68pctCL   -> Draw("l");
  lim_nueapp_3p1_reactor_allowed_1_95pctCL -> Draw("l");
  lim_nueapp_3p1_reactor_allowed_2_95pctCL -> Draw("l");
  lim_nueapp_3p1_reactor_allowed_3_95pctCL -> Draw("l");
  lim_nueapp_3p1_gallium_allowed_95pctCL   -> Draw("l");
  lim_nueapp_3p1_solar_excluded_95pctCL    -> Draw("l");
  lim_nueapp_3p1_nueC_excluded_95pctCL     -> Draw("l");

  TLegend * lgnuedis = new TLegend(0.13,0.1,0.35,0.34);
  lgnuedis->SetFillColor(0);
  lgnuedis->SetFillStyle(0);
  lgnuedis->SetBorderSize(0);
  lgnuedis->AddEntry(lim_nuedis_3p1_t2knd280excluded_95pctCL,  "T2K/ND280 excluded 95\% CL",    "L");
  lgnuedis->AddEntry(lim_nuedis_3p1_t2knd280allowed_90pctCL,   "T2K/ND280 allowed 90\% CL",   "LF");
  lgnuedis->AddEntry(lim_nuedis_3p1_t2knd280allowed_68pctCL,   "T2K/ND280 allowed 68\% CL",   "LF");
  lgnuedis->AddEntry(lim_nueapp_3p1_reactor_allowed_1_95pctCL, "Reactor allowed 95\% CL",      "L");
  lgnuedis->AddEntry(lim_nueapp_3p1_gallium_allowed_95pctCL,   "Gallium allowed 95\% CL",      "L");
  lgnuedis->AddEntry(lim_nueapp_3p1_solar_excluded_95pctCL,    "Solar excluded 95\% CL",       "L");
  lgnuedis->AddEntry(lim_nueapp_3p1_nueC_excluded_95pctCL,     "#nu_{e}+C excluded 95\% CL",   "L");
  lgnuedis->Draw();
}

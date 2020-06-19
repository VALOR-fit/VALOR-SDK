// -----------------------------------------------------------------------------
// This macro reads in the various data files with digitised sterile neutrino
// limits, converts limits to ROOT format, and sets standard drawing options.
// Optionally, it plots 3 sets limit compilations:
// - limits in Dm^2_41, sin^2(2\theta_\mu\mu) from nu_mu disappearance data
// - limits in Dm^2_41, sin^2(2\theta_ee)     from nu_e appearance data
// - limits in Dm^2_41, sin^2(2\theta_\mu e)  from nu_e disappearance data
//
// The digitised contours are stored in $VALOR_FMWK/data/contours/sterile_nu/
//
// The function convert_from_sinsqtheta_to_sinsq2theta() was adapted from
// Rhiannon's translate_MINOS_MINOS+_to_sinsq2theta.C
//
// Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
//
// Copyright (c) 2010-2020, The VALOR group. All rights reserved
// -----------------------------------------------------------------------------

// convert limits
TGraph * convert_from_sinsqtheta_to_sinsq2theta(TGraph * in);

void convert_contours_to_root_format_and_plot(bool plot=true)
{
  // ---------------------------------------------------------------------------
  // nue appearance
  // ---------------------------------------------------------------------------

  // Convert to ROOT for format

  // SBN proposal, 90% CL
  TGraph * lim_nueapp_3p1_sbnproposal_90pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nueapp_3+1_SBNproposal_90pctCL.data");
  lim_nueapp_3p1_sbnproposal_90pctCL->SetLineWidth(5);
  lim_nueapp_3p1_sbnproposal_90pctCL->SetLineColor(kBlack);
  lim_nueapp_3p1_sbnproposal_90pctCL->SetLineStyle(kSolid);
  // SBN proposal, 3 sigma CL
  TGraph * lim_nueapp_3p1_sbnproposal_3sigCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nueapp_3+1_SBNproposal_3sigCL.data");
  lim_nueapp_3p1_sbnproposal_3sigCL->SetLineWidth(5);
  lim_nueapp_3p1_sbnproposal_3sigCL->SetLineColor(kBlack);
  lim_nueapp_3p1_sbnproposal_3sigCL->SetLineStyle(kDashed);
  // SBN proposal, 5 sigma CL
  TGraph * lim_nueapp_3p1_sbnproposal_5sigCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nueapp_3+1_SBNproposal_5sigCL.data");
  lim_nueapp_3p1_sbnproposal_5sigCL->SetLineWidth(5);
  lim_nueapp_3p1_sbnproposal_5sigCL->SetLineColor(kBlack);
  lim_nueapp_3p1_sbnproposal_5sigCL->SetLineStyle(kDashDotted);
  // NOMAD 99% CL
  TGraph * lim_nueapp_3p1_nomad_99pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nueapp_3+1_NOMAD_99pctCL.data");
  lim_nueapp_3p1_nomad_99pctCL->SetLineWidth(3);
  lim_nueapp_3p1_nomad_99pctCL->SetLineStyle(kSolid);
  lim_nueapp_3p1_nomad_99pctCL->SetLineColor(kCyan-3);
  // BNL776 99% CL
  TGraph * lim_nueapp_3p1_bnl776_99pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nueapp_3+1_BNL776_99pctCL.data");
  lim_nueapp_3p1_bnl776_99pctCL->SetLineWidth(3);
  lim_nueapp_3p1_bnl776_99pctCL->SetLineStyle(kSolid);
  lim_nueapp_3p1_bnl776_99pctCL->SetLineColor(kGreen+1);
  // KARMEN 99% CL
  TGraph * lim_nueapp_3p1_karmen_99pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nueapp_3+1_KARMEN_99pctCL.data");
  lim_nueapp_3p1_karmen_99pctCL->SetLineWidth(3);
  lim_nueapp_3p1_karmen_99pctCL->SetLineStyle(kSolid);
  lim_nueapp_3p1_karmen_99pctCL->SetLineColor(kAzure+9);
  // CNGS/ICARUS 99% CL
  TGraph * lim_nueapp_3p1_icarus_99pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nueapp_3+1_ICARUS_99pctCL.data");
  lim_nueapp_3p1_icarus_99pctCL->SetLineWidth(4);
  lim_nueapp_3p1_icarus_99pctCL->SetLineStyle(kDashed);
  lim_nueapp_3p1_icarus_99pctCL->SetLineColor(kGreen+3);
  // OPERA 99% CL
  TGraph * lim_nueapp_3p1_opera_99pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nueapp_3+1_OPERA_99pctCL.data");
  lim_nueapp_3p1_opera_99pctCL->SetLineWidth(2);
  lim_nueapp_3p1_opera_99pctCL->SetLineStyle(kSolid);
  lim_nueapp_3p1_opera_99pctCL->SetLineColor(kGreen+3);
  // MiniBooNE 99% CL
  TGraph * lim_nueapp_3p1_miniboonenu_99pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nueapp_3+1_MiniBooNEnu_99pctCL.data");
  lim_nueapp_3p1_miniboonenu_99pctCL->SetLineWidth(4);
  lim_nueapp_3p1_miniboonenu_99pctCL->SetLineStyle(kDashDotted);
  lim_nueapp_3p1_miniboonenu_99pctCL->SetLineColor(kViolet-6);
  // LSND 99% CL
  TGraph * lim_nueapp_3p1_lsndr1_99pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nueapp_3+1_LSNDwDiF1_99pctCL.data");
  lim_nueapp_3p1_lsndr1_99pctCL->SetLineWidth(401);
  lim_nueapp_3p1_lsndr1_99pctCL->SetFillStyle(3005);
  lim_nueapp_3p1_lsndr1_99pctCL->SetLineColor(kOrange+10);
  lim_nueapp_3p1_lsndr1_99pctCL->SetFillColor(kOrange+6);
  TGraph * lim_nueapp_3p1_lsndr2_99pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nueapp_3+1_LSNDwDiF2_99pctCL.data");
  lim_nueapp_3p1_lsndr2_99pctCL->SetLineWidth(101);
  lim_nueapp_3p1_lsndr2_99pctCL->SetFillStyle(3005);
  lim_nueapp_3p1_lsndr2_99pctCL->SetLineColor(kOrange+10);
  lim_nueapp_3p1_lsndr2_99pctCL->SetFillColor(kOrange+6);
  TGraph * lim_nueapp_3p1_lsndr3_99pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nueapp_3+1_LSNDwDiF3_99pctCL.data");
  lim_nueapp_3p1_lsndr3_99pctCL->SetLineWidth(-101);
  lim_nueapp_3p1_lsndr3_99pctCL->SetFillStyle(3005);
  lim_nueapp_3p1_lsndr3_99pctCL->SetLineColor(kOrange+10);
  lim_nueapp_3p1_lsndr3_99pctCL->SetFillColor(kOrange+6);
  TMultiGraph * lim_nueapp_3p1_lsnd_99pctCL = new TMultiGraph();
  lim_nueapp_3p1_lsnd_99pctCL->Add(lim_nueapp_3p1_lsndr1_99pctCL);// region 1 (high Dmsq)
  lim_nueapp_3p1_lsnd_99pctCL->Add(lim_nueapp_3p1_lsndr2_99pctCL);// region 2 (intermediate Dmsq)
  lim_nueapp_3p1_lsnd_99pctCL->Add(lim_nueapp_3p1_lsndr3_99pctCL);// region 3 (low Dmsq)

  // Plot

  if(plot) {
    TCanvas * cnueapp = new TCanvas("cnueapp","",20,20,900,600);
    cnueapp->Draw();
    TH2F * hfnueapp = (TH2F*) cnueapp->DrawFrame(1E-4,6E-2,2E-1,1.5E+1);
    hfnueapp->Draw();
    hfnueapp->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu e}");
    hfnueapp->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2}/c^{4})");
    cnueapp->SetLogx();
    cnueapp->SetLogy();
    lim_nueapp_3p1_sbnproposal_90pctCL->Draw("l");
    lim_nueapp_3p1_sbnproposal_3sigCL ->Draw("l");
    lim_nueapp_3p1_sbnproposal_5sigCL ->Draw("l");
    lim_nueapp_3p1_nomad_99pctCL      ->Draw("l");
    lim_nueapp_3p1_bnl776_99pctCL     ->Draw("l");
    lim_nueapp_3p1_karmen_99pctCL     ->Draw("l");
    lim_nueapp_3p1_icarus_99pctCL     ->Draw("l");
    lim_nueapp_3p1_opera_99pctCL      ->Draw("l");
    lim_nueapp_3p1_miniboonenu_99pctCL->Draw("l");
    lim_nueapp_3p1_lsndr1_99pctCL     ->Draw("l");
    lim_nueapp_3p1_lsndr2_99pctCL     ->Draw("l");
    lim_nueapp_3p1_lsndr3_99pctCL     ->Draw("l");
    TLegend * lgnueapp = new TLegend(0.13,0.1,0.35,0.4);
    lgnueapp->SetFillColor(0);
    lgnueapp->SetFillStyle(0);
    lgnueapp->SetBorderSize(0);
    lgnueapp->AddEntry(lim_nueapp_3p1_sbnproposal_90pctCL,
       "SBN proposal 90\% CL", "L");
    lgnueapp->AddEntry(lim_nueapp_3p1_sbnproposal_3sigCL,
       "SBN proposal 3#sigma CL", "L");
    lgnueapp->AddEntry(lim_nueapp_3p1_sbnproposal_5sigCL,
       "SBN proposal 5#sigma CL", "L");
    lgnueapp->AddEntry(lim_nueapp_3p1_nomad_99pctCL,
       "NOMAD 99\% CL", "L");
    lgnueapp->AddEntry(lim_nueapp_3p1_bnl776_99pctCL,
       "BNL776 99\% CL", "L");
    lgnueapp->AddEntry(lim_nueapp_3p1_karmen_99pctCL,
       "KARMEN 99\% CL", "L");
    lgnueapp->AddEntry(lim_nueapp_3p1_icarus_99pctCL,
       "ICARUS 99\% CL", "L");
    lgnueapp->AddEntry(lim_nueapp_3p1_opera_99pctCL,
       "OPERA 99\% CL", "L");
    lgnueapp->AddEntry(lim_nueapp_3p1_miniboonenu_99pctCL,
       "MiniBooNE (#nu) 99\% CL", "L");
    lgnueapp->AddEntry(lim_nueapp_3p1_lsndr1_99pctCL,
       "LSBD w/ DiF 99\% CL", "LF");
    lgnueapp->Draw();
  }

  // ---------------------------------------------------------------------------
  // numu disappearance
  // ---------------------------------------------------------------------------

  // Convert to ROOT for format

  // SBN proposal, 90% CL
  TGraph * lim_numudis_3p1_sbnproposal_90pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/numudis_3+1_SBNproposal_90pctCL.data");
  lim_numudis_3p1_sbnproposal_90pctCL->SetLineWidth(3);
  lim_numudis_3p1_sbnproposal_90pctCL->SetLineColor(kBlack);
  lim_numudis_3p1_sbnproposal_90pctCL->SetLineStyle(kSolid);
  // SBN proposal, 3 sigma CL
  TGraph * lim_numudis_3p1_sbnproposal_3sigCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/numudis_3+1_SBNproposal_3sigCL.data");
  lim_numudis_3p1_sbnproposal_3sigCL->SetLineWidth(3);
  lim_numudis_3p1_sbnproposal_3sigCL->SetLineColor(kBlack);
  lim_numudis_3p1_sbnproposal_3sigCL->SetLineStyle(kDashed);
  // SBN proposal, 5 sigma CL
  TGraph * lim_numudis_3p1_sbnproposal_5sigCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/numudis_3+1_SBNproposal_5sigCL.data");
  lim_numudis_3p1_sbnproposal_5sigCL->SetLineWidth(3);
  lim_numudis_3p1_sbnproposal_5sigCL->SetLineColor(kBlack);
  lim_numudis_3p1_sbnproposal_5sigCL->SetLineStyle(kDashDotted);
  // MINOS/MINOS+ 90% CL
  TGraph * temp1 =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/numudis_3+1_MINOS_MINOS+_90pctCL.data");
  TGraph * lim_numudis_3p1_minosp_90pctCL =
    convert_from_sinsqtheta_to_sinsq2theta(temp1);
  lim_numudis_3p1_minosp_90pctCL->SetLineWidth(2);
  lim_numudis_3p1_minosp_90pctCL->SetLineStyle(kSolid);
  lim_numudis_3p1_minosp_90pctCL->SetLineColor(kGreen+3);
  // MiniBooNE+SciBooNE 90% CL
  TGraph * lim_numudis_3p1_minisciboone_90pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/numudis_3+1_MiniBooNE_SciBooNE_90pctCL.data");
  lim_numudis_3p1_minisciboone_90pctCL->SetLineWidth(2);
  lim_numudis_3p1_minisciboone_90pctCL->SetLineStyle(kSolid);
  lim_numudis_3p1_minisciboone_90pctCL->SetLineColor(kViolet-6);
  // IceCuBE 90% CL
  TGraph * lim_numudis_3p1_icecube_90pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/numudis_3+1_8yrIceCube_90pctCL.data");
  lim_numudis_3p1_icecube_90pctCL->SetLineWidth(2);
  lim_numudis_3p1_icecube_90pctCL->SetLineStyle(kDashed);
  lim_numudis_3p1_icecube_90pctCL->SetLineColor(kMagenta);
  // IceCuBE 99% CL
  TGraph * lim_numudis_3p1_icecube_99pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/numudis_3+1_8yrIceCube_99pctCL.data");
  lim_numudis_3p1_icecube_99pctCL->SetLineWidth(2);
  lim_numudis_3p1_icecube_99pctCL->SetLineStyle(kSolid);
  lim_numudis_3p1_icecube_99pctCL->SetLineColor(kMagenta);

  // Plot

  if(plot) {
    TCanvas * cnumudis = new TCanvas("cnumudis","",20,20,900,600);
    cnumudis->Draw();
    TH2F * hfnumudis = (TH2F*) cnumudis->DrawFrame(3E-4,1E-3,1,2E+2);
    hfnumudis->Draw();
    hfnumudis->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu #mu}");
    hfnumudis->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2}/c^{4})");
    cnumudis->SetLogx();
    cnumudis->SetLogy();
    lim_numudis_3p1_sbnproposal_90pctCL ->Draw("l");
    lim_numudis_3p1_sbnproposal_3sigCL  ->Draw("l");
    lim_numudis_3p1_sbnproposal_5sigCL  ->Draw("l");
    lim_numudis_3p1_minosp_90pctCL      ->Draw("l");
    lim_numudis_3p1_minisciboone_90pctCL->Draw("l");
    lim_numudis_3p1_icecube_90pctCL     ->Draw("l");
    lim_numudis_3p1_icecube_99pctCL     ->Draw("l");
    TLegend * lgnumudis = new TLegend(0.13,0.10,0.35,0.45);
    lgnumudis->SetFillColor(0);
    lgnumudis->SetFillStyle(0);
    lgnumudis->SetBorderSize(0);
    lgnumudis->AddEntry(lim_numudis_3p1_sbnproposal_90pctCL,
      "SBN proposal 90\% CL", "L");
    lgnumudis->AddEntry(lim_numudis_3p1_sbnproposal_3sigCL,
      "SBN proposal 3#sigma CL", "L");
    lgnumudis->AddEntry(lim_numudis_3p1_sbnproposal_5sigCL,
      "SBN proposal 5#sigma CL", "L");
    lgnumudis->AddEntry(lim_numudis_3p1_minosp_90pctCL,
      "MINOS/MINOS+ 90\% CL", "L");
    lgnumudis->AddEntry(lim_numudis_3p1_minisciboone_90pctCL,
      "MiniBooNE/SciBooNE 90\% CL", "L");
    lgnumudis->AddEntry(lim_numudis_3p1_icecube_90pctCL,
      "IceCube 90\% CL", "L");
    lgnumudis->AddEntry(lim_numudis_3p1_icecube_99pctCL,
      "IceCube 99\% CL", "L");
    lgnumudis->Draw();
  }
  // ---------------------------------------------------------------------------
  // nue disappearance
  // ---------------------------------------------------------------------------

  // Convert to ROOT for format

  // T2K
  TGraph * lim_nuedis_3p1_t2knd280excluded_95pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nuedis_3+1_T2KND280_excluded_95pctCL.data");
  lim_nuedis_3p1_t2knd280excluded_95pctCL->SetLineWidth(3);
  lim_nuedis_3p1_t2knd280excluded_95pctCL->SetLineStyle(kSolid);
  lim_nuedis_3p1_t2knd280excluded_95pctCL->SetLineColor(kGreen+3);
  TGraph * lim_nuedis_3p1_t2knd280allowed_90pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nuedis_3+1_T2KND280_allowed_90pctCL.data");
  lim_nuedis_3p1_t2knd280allowed_90pctCL->SetLineWidth(1102);
  lim_nuedis_3p1_t2knd280allowed_90pctCL->SetFillStyle(3005);
  lim_nuedis_3p1_t2knd280allowed_90pctCL->SetLineColor(kOrange-2);
  lim_nuedis_3p1_t2knd280allowed_90pctCL->SetFillColor(kOrange-2);
  TGraph * lim_nuedis_3p1_t2knd280allowed_68pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nuedis_3+1_T2KND280_allowed_68pctCL.data");
  lim_nuedis_3p1_t2knd280allowed_68pctCL->SetLineWidth(702);
  lim_nuedis_3p1_t2knd280allowed_68pctCL->SetFillStyle(3005);
  lim_nuedis_3p1_t2knd280allowed_68pctCL->SetLineColor(kOrange-1);
  lim_nuedis_3p1_t2knd280allowed_68pctCL->SetFillColor(kOrange-1);
  // Reactor
  TGraph * lim_nueapp_3p1_reactor_allowed_1_95pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nuedis_3+1_Reactor_allowed_1_95pctCL.data");
  lim_nueapp_3p1_reactor_allowed_1_95pctCL->SetLineWidth(3);
  lim_nueapp_3p1_reactor_allowed_1_95pctCL->SetLineColor(kRed);
  TGraph * lim_nueapp_3p1_reactor_allowed_2_95pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nuedis_3+1_Reactor_allowed_2_95pctCL.data");
  lim_nueapp_3p1_reactor_allowed_2_95pctCL->SetLineWidth(3);
  lim_nueapp_3p1_reactor_allowed_2_95pctCL->SetLineColor(kRed);
  TGraph * lim_nueapp_3p1_reactor_allowed_3_95pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nuedis_3+1_Reactor_allowed_3_95pctCL.data");
  lim_nueapp_3p1_reactor_allowed_3_95pctCL->SetLineWidth(3);
  lim_nueapp_3p1_reactor_allowed_3_95pctCL->SetLineColor(kRed);
  TMultiGraph * lim_nueapp_3p1_reactor_allowed_95pctCL = new TMultiGraph();
  lim_nueapp_3p1_reactor_allowed_95pctCL->Add(lim_nueapp_3p1_reactor_allowed_1_95pctCL);// region 1 (high Dmsq)
  lim_nueapp_3p1_reactor_allowed_95pctCL->Add(lim_nueapp_3p1_reactor_allowed_2_95pctCL);// region 2 (intermediate Dmsq)
  lim_nueapp_3p1_reactor_allowed_95pctCL->Add(lim_nueapp_3p1_reactor_allowed_3_95pctCL);// region 3 (low Dmsq)
  // Gallium
  TGraph * lim_nueapp_3p1_gallium_allowed_95pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nuedis_3+1_Gallium_allowed_95pctCL.data");
  lim_nueapp_3p1_gallium_allowed_95pctCL->SetLineWidth(3);
  lim_nueapp_3p1_gallium_allowed_95pctCL->SetLineColor(kBlue);
  TGraph * lim_nueapp_3p1_solar_excluded_95pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nuedis_3+1_Solar_excluded_95pctCL.data");
  lim_nueapp_3p1_solar_excluded_95pctCL->SetLineWidth(4);
  lim_nueapp_3p1_solar_excluded_95pctCL->SetLineColor(kGray+10);
  TGraph * lim_nueapp_3p1_nueC_excluded_95pctCL =
    new TGraph("$VALOR_FMWK/data/contours/sterile_nu/nuedis_3+1_nueC_excluded_95pctCL.data");
  lim_nueapp_3p1_nueC_excluded_95pctCL->SetLineWidth(3);
  lim_nueapp_3p1_nueC_excluded_95pctCL->SetLineColor(kMagenta-4);
  lim_nueapp_3p1_nueC_excluded_95pctCL->SetLineStyle(kDashed);

  // Plot

  if(plot) {
    TCanvas * cnuedis = new TCanvas("cnuedis","",20,20,900,600);
    cnuedis->Draw();
    TH2F * hfnuedis = (TH2F*) cnuedis->DrawFrame(1E-2,1E-2,1,1E+2);
    hfnuedis->Draw();
    hfnuedis->GetXaxis()->SetTitle("sin^{2}2#theta_{e e}");
    hfnuedis->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2}/c^{4})");
    cnuedis->SetLogx();
    cnuedis->SetLogy();
    lim_nuedis_3p1_t2knd280excluded_95pctCL ->Draw("l");
    lim_nuedis_3p1_t2knd280allowed_90pctCL  ->Draw("l");
    lim_nuedis_3p1_t2knd280allowed_68pctCL  ->Draw("l");
    lim_nueapp_3p1_reactor_allowed_1_95pctCL->Draw("l");
    lim_nueapp_3p1_reactor_allowed_2_95pctCL->Draw("l");
    lim_nueapp_3p1_reactor_allowed_3_95pctCL->Draw("l");
    lim_nueapp_3p1_gallium_allowed_95pctCL  ->Draw("l");
    lim_nueapp_3p1_solar_excluded_95pctCL   ->Draw("l");
    lim_nueapp_3p1_nueC_excluded_95pctCL    ->Draw("l");
    TLegend * lgnuedis = new TLegend(0.13,0.1,0.35,0.34);
    lgnuedis->SetFillColor(0);
    lgnuedis->SetFillStyle(0);
    lgnuedis->SetBorderSize(0);
    lgnuedis->AddEntry(lim_nuedis_3p1_t2knd280excluded_95pctCL,
       "T2K/ND280 excluded 95\% CL", "L");
    lgnuedis->AddEntry(lim_nuedis_3p1_t2knd280allowed_90pctCL,
       "T2K/ND280 allowed 90\% CL", "LF");
    lgnuedis->AddEntry(lim_nuedis_3p1_t2knd280allowed_68pctCL,
       "T2K/ND280 allowed 68\% CL", "LF");
    lgnuedis->AddEntry(lim_nueapp_3p1_reactor_allowed_95pctCL,
       "Reactor allowed 95\% CL", "L");
    lgnuedis->AddEntry(lim_nueapp_3p1_gallium_allowed_95pctCL,
       "Gallium allowed 95\% CL", "L");
    lgnuedis->AddEntry(lim_nueapp_3p1_solar_excluded_95pctCL,
       "Solar excluded 95\% CL", "L");
    lgnuedis->AddEntry(lim_nueapp_3p1_nueC_excluded_95pctCL,
       "#nu_{e}+C excluded 95\% CL", "L");
    lgnuedis->Draw();
  }

  //
  // Write everything out in a ROOT file
  //

  TFile f("./sterile_3p1_limits.root","recreate");
  lim_nueapp_3p1_sbnproposal_90pctCL        -> Write( "lim_nueapp_3p1_sbnproposal_90pctCL"      );
  lim_nueapp_3p1_sbnproposal_3sigCL         -> Write( "lim_nueapp_3p1_sbnproposal_3sigCL"       );
  lim_nueapp_3p1_sbnproposal_5sigCL         -> Write( "lim_nueapp_3p1_sbnproposal_5sigCL"       );
  lim_nueapp_3p1_nomad_99pctCL              -> Write( "lim_nueapp_3p1_nomad_99pctCL"            );
  lim_nueapp_3p1_bnl776_99pctCL             -> Write( "lim_nueapp_3p1_bnl776_99pctCL"           );
  lim_nueapp_3p1_karmen_99pctCL             -> Write( "lim_nueapp_3p1_karmen_99pctCL"           );
  lim_nueapp_3p1_icarus_99pctCL             -> Write( "lim_nueapp_3p1_icarus_99pctCL"           );
  lim_nueapp_3p1_opera_99pctCL              -> Write( "lim_nueapp_3p1_opera_99pctCL"            );
  lim_nueapp_3p1_miniboonenu_99pctCL        -> Write( "lim_nueapp_3p1_miniboonenu_99pctCL"      );
  lim_nueapp_3p1_lsndr1_99pctCL             -> Write( "lim_nueapp_3p1_lsndr1_99pctCL"           );
  lim_nueapp_3p1_lsndr2_99pctCL             -> Write( "lim_nueapp_3p1_lsndr2_99pctCL"           );
  lim_nueapp_3p1_lsndr3_99pctCL             -> Write( "lim_nueapp_3p1_lsndr3_99pctCL"           );
  lim_numudis_3p1_sbnproposal_90pctCL       -> Write( "lim_numudis_3p1_sbnproposal_90pctCL"     );
  lim_numudis_3p1_sbnproposal_3sigCL        -> Write( "lim_numudis_3p1_sbnproposal_3sigCL"      );
  lim_numudis_3p1_sbnproposal_5sigCL        -> Write( "lim_numudis_3p1_sbnproposal_5sigCL"      );
  lim_numudis_3p1_minosp_90pctCL            -> Write( "lim_numudis_3p1_minosp_90pctCL"          );
  lim_numudis_3p1_minisciboone_90pctCL      -> Write( "lim_numudis_3p1_minisciboone_90pctCL"    );
  lim_numudis_3p1_icecube_90pctCL           -> Write( "lim_numudis_3p1_icecube_90pctCL"         );
  lim_numudis_3p1_icecube_99pctCL           -> Write( "lim_numudis_3p1_icecube_99pctCL"         );
  lim_nuedis_3p1_t2knd280excluded_95pctCL   -> Write( "lim_nuedis_3p1_t2knd280excluded_95pctCL" );
  lim_nuedis_3p1_t2knd280allowed_90pctCL    -> Write( "lim_nuedis_3p1_t2knd280allowed_90pctCL"  );
  lim_nuedis_3p1_t2knd280allowed_68pctCL    -> Write( "lim_nuedis_3p1_t2knd280allowed_68pctCL"  );
  lim_nueapp_3p1_reactor_allowed_1_95pctCL  -> Write( "lim_nueapp_3p1_reactor_allowed_1_95pctCL");
  lim_nueapp_3p1_reactor_allowed_2_95pctCL  -> Write( "lim_nueapp_3p1_reactor_allowed_2_95pctCL");
  lim_nueapp_3p1_reactor_allowed_3_95pctCL  -> Write( "lim_nueapp_3p1_reactor_allowed_3_95pctCL");
  lim_nueapp_3p1_gallium_allowed_95pctCL    -> Write( "lim_nueapp_3p1_gallium_allowed_95pctCL"  );
  lim_nueapp_3p1_solar_excluded_95pctCL     -> Write( "lim_nueapp_3p1_solar_excluded_95pctCL"   );
  lim_nueapp_3p1_nueC_excluded_95pctCL      -> Write( "lim_nueapp_3p1_nueC_excluded_95pctCL"    );
  f.Close();
}

// -----------------------------------------------------------------------------
TGraph * convert_from_sinsqtheta_to_sinsq2theta(TGraph * in)
{
  // Get the x-axis bin edges and redefine
  const int np   = in->GetN();
  double * x_arr = in->GetX();
  double * y_arr = in->GetY();

  std::vector<double> y_vec    (y_arr, y_arr+np);
  std::vector<double> x_vec_old(x_arr, x_arr+np);
  std::vector<double> x_vec_new;

  if(np != x_vec_old.size() || np != y_vec.size()){
    std::cerr << " Error: Lengths of x and y arrays don't match: "
              << std::endl;
    std::cerr << " N Points: " << np
              << ", X Points: " << x_vec_old.size()
              << ", Y Points: " << y_vec.size() << std::endl;
    exit(1);
  }
  for(int i = 0; i < np; ++i){
    // Get point on the x-axis
    double sinsqtheta  = x_vec_old.at(i);
    double sinsq2theta = 4 * sinsqtheta * (1-sinsqtheta);
    x_vec_new.push_back(sinsq2theta);
  }

  // Fill the new TGraph and write to new file
  TVectorD x(np, x_vec_new.data());
  TVectorD y(np, y_vec.data());
  TGraph * out = new TGraph(x, y);
  return out;
}
// -----------------------------------------------------------------------------

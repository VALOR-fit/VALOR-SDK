//
// This macro reads in the ROOT file produced by convert_to_root_file_and_plot.C
// and plots the appearance and disappearance limits.
//
// *********************************************************
//
// Added in modern SBN sensitivities from CAFAna and VALOR
// DISAPPEARANCE ONLY
//
// As of December 2019
//
// R.J.
//
// *********************************************************
// C.A.
//

{

  TFile * f = new TFile("./sterile_3p1_limits.root","read");
  
  // VALOR modern disappearance files

  TFile *f_stat      = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/mock_data_challenge/true_baseline/contours/new-stat-only_contours_as_graphs.root","read"); // true baseline
  TFile *f_syst      = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/mock_data_challenge/true_baseline/contours/new-syst-etrue_contours_as_graphs.root","read"); // true baseline
  //TFile *f_stat      = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/contours/december_2019/ubfix_mdc_stat_3_det_contours_as_graphs.root","read");
  TFile *f_syst_old  = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/contours/december_2019/ubfix_mdc_syst_3_det_contours_as_graphs.root","read");
  TFile *f_stat_p    = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/contours/december_2019/proposal_stat_3_det_contours_as_graphs.root","read");
  TFile *f_syst_p    = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/contours/december_2019/proposal_syst_3_det_contours_as_graphs.root","read");
  TFile *f_syst_a    = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/contours/december_2019/ubfix_injection_syst_contours_as_graphs.root");
  TFile *f_stat_a    = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/contours/december_2019/ubfix_injection_stat_contours_as_graphs.root");

  // CAFAna proposal and modern numu files
  TFile *f_caf_prop  = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/data/sbn/CAFAna/CAFAna_numu_proposal.root","read");
  TFile *f_caf_mod   = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/data/sbn/CAFAna/CAFAna_numu_modern.root","read");

  // SBNFit proposal and modern numu files
  TFile *f_sbn_prop    = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/data/sbn/SBNFit/SBNfit_numu_proposal.root","read");
  TFile *f_sbn_prop25  = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/data/sbn/SBNFit/SBNfit_numu_proposal_25var.root","read");
//  TFile *f_sbn_mod     = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/data/sbn/SBNFit/SBNfit_numu_modern.root","read");
  TFile *f_sbn_mod25   = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/data/sbn/SBNFit/SBNfit_numu_modern_25var.root","read");
  TFile *f_sbn_mod_a   = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/data/sbn/SBNFit/SBNfit_numu_modern_allow.root","read");

  //
  // Dummy plot for stats in legend
  //

  TGraph * dummy_stat = (TGraph *) f_stat_a->Get("contour_90pct_lin_0");
  dummy_stat -> SetLineColor(kBlack);
  dummy_stat -> SetLineStyle(2);
  dummy_stat -> SetLineWidth(2);
  dummy_stat -> SetMarkerStyle(0);

  // 
  // All graphs
  //

  TGraph * lim_numudis_3p1_sbnproposal_90pctCL     = (TGraph *) f->Get( "lim_numudis_3p1_sbnproposal_90pctCL"  );
  TGraph * lim_numudis_3p1_minosp_90pctCL          = (TGraph *) f->Get( "lim_numudis_3p1_minosp_90pctCL"       );
  TGraph * lim_numudis_3p1_minisciboone_90pctCL    = (TGraph *) f->Get( "lim_numudis_3p1_minisciboone_90pctCL" );
  //TGraph * valor_modern_sbn_3p1_contour_90_stat    = (TGraph *) f_stat->Get("contour_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_syst_old = (TGraph *) f_syst_old->Get("contour_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_stat    = (TGraph *) f_stat->Get("stat-only_90pct_lin_0"); // true baseline 
  TGraph * valor_modern_sbn_3p1_contour_90_syst    = (TGraph *) f_syst->Get("new-syst-etrue_90pct_lin_0");
  TGraph * valor_proposal_sbn_3p1_contour_90_stat  = (TGraph *) f_stat_p->Get("contour_90pct_lin_0");
  TGraph * valor_proposal_sbn_3p1_contour_90_syst  = (TGraph *) f_syst_p->Get("contour_90pct_lin_0");
  TGraph * cafana_proposal_sbn_3p1_contour_90_stat = (TGraph *) f_caf_prop->Get("numu_exclusion_all_stat_90pct"); 
  TGraph * cafana_proposal_sbn_3p1_contour_90_syst = (TGraph *) f_caf_prop->Get("numu_exclusion_all_syst_90pct"); 
  TGraph * cafana_modern_sbn_3p1_contour_90_stat   = (TGraph *) f_caf_mod->Get("numu_exclusion_all_stat_90pct"); 
  TGraph * cafana_modern_sbn_3p1_contour_90_syst   = (TGraph *) f_caf_mod->Get("numu_exclusion_all_syst_90pct"); 
  TGraph * sbnfit_proposal_sbn_3p1_contour_90_stat = (TGraph *) f_sbn_prop->Get("fit_STATONLY_90CL"); 
  TGraph * sbnfit_proposal_sbn_3p1_contour_90_syst20 = (TGraph *) f_sbn_prop->Get("fit_all_90CL"); 
  TGraph * sbnfit_proposal_sbn_3p1_contour_90_syst = (TGraph *) f_sbn_prop25->Get("all_90CL"); 
  TGraph * sbnfit_modern_sbn_3p1_contour_90_stat   = (TGraph *) f_sbn_mod25->Get("STATONLY_90CL");  
  TGraph * sbnfit_modern_sbn_3p1_contour_90_syst   = (TGraph *) f_sbn_mod25->Get("all_90CL"); 

  std::vector<double> x = {0.07};
  std::vector<double> y = {1.32};

  TGraph *injected = new TGraph(1,&(x[0]),&(y[0]));
  injected->SetMarkerStyle(43);
  injected->SetMarkerSize(1.5);
  injected->SetMarkerColor(kGray+3);

  lim_numudis_3p1_sbnproposal_90pctCL     -> SetLineColor(kGray+3);
  valor_modern_sbn_3p1_contour_90_stat    -> SetLineColor(kPink+5);
  valor_modern_sbn_3p1_contour_90_stat    -> SetLineStyle(9);
  valor_modern_sbn_3p1_contour_90_syst    -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_90_syst    -> SetLineStyle(1); 
  valor_modern_sbn_3p1_contour_90_syst_old-> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_90_syst_old-> SetLineStyle(5); 
  valor_proposal_sbn_3p1_contour_90_stat  -> SetLineColor(kPink+5);
  valor_proposal_sbn_3p1_contour_90_stat  -> SetLineStyle(8);
  valor_proposal_sbn_3p1_contour_90_syst  -> SetLineColor(kPink+5);  
  valor_proposal_sbn_3p1_contour_90_syst  -> SetLineStyle(2);  
  cafana_modern_sbn_3p1_contour_90_stat   -> SetLineColor(kOrange+1);
  cafana_modern_sbn_3p1_contour_90_stat   -> SetLineStyle(9);
  cafana_modern_sbn_3p1_contour_90_syst   -> SetLineColor(kOrange+1);  
  cafana_modern_sbn_3p1_contour_90_syst   -> SetLineStyle(1);
  cafana_proposal_sbn_3p1_contour_90_stat -> SetLineColor(kOrange+1);
  cafana_proposal_sbn_3p1_contour_90_stat -> SetLineStyle(8);
  cafana_proposal_sbn_3p1_contour_90_syst -> SetLineColor(kOrange+1);  
  cafana_proposal_sbn_3p1_contour_90_syst -> SetLineStyle(2);
  sbnfit_modern_sbn_3p1_contour_90_stat   -> SetLineColor(kAzure+7);
  sbnfit_modern_sbn_3p1_contour_90_stat   -> SetLineStyle(9);
  sbnfit_modern_sbn_3p1_contour_90_syst   -> SetLineColor(kAzure+7);  
  sbnfit_modern_sbn_3p1_contour_90_syst   -> SetLineStyle(1);
  sbnfit_proposal_sbn_3p1_contour_90_stat -> SetLineColor(kAzure+7);
  sbnfit_proposal_sbn_3p1_contour_90_stat -> SetLineStyle(8);
  sbnfit_proposal_sbn_3p1_contour_90_syst -> SetLineColor(kAzure+7);  
  sbnfit_proposal_sbn_3p1_contour_90_syst -> SetLineStyle(2);
  lim_numudis_3p1_minosp_90pctCL          -> SetLineColor(kTeal-5);  
  lim_numudis_3p1_minisciboone_90pctCL    -> SetLineColor(kYellow-3); 

  lim_numudis_3p1_sbnproposal_90pctCL     -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_stat    -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_syst    -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_syst_old-> SetLineWidth(2);
  valor_proposal_sbn_3p1_contour_90_stat  -> SetLineWidth(2);
  valor_proposal_sbn_3p1_contour_90_syst  -> SetLineWidth(2);
  cafana_modern_sbn_3p1_contour_90_stat   -> SetLineWidth(2);
  cafana_modern_sbn_3p1_contour_90_syst   -> SetLineWidth(2);
  cafana_proposal_sbn_3p1_contour_90_stat -> SetLineWidth(2);
  cafana_proposal_sbn_3p1_contour_90_syst -> SetLineWidth(2);
  sbnfit_modern_sbn_3p1_contour_90_stat   -> SetLineWidth(2);
  sbnfit_modern_sbn_3p1_contour_90_syst   -> SetLineWidth(2);
  sbnfit_proposal_sbn_3p1_contour_90_stat -> SetLineWidth(2);
  sbnfit_proposal_sbn_3p1_contour_90_syst -> SetLineWidth(2);
  lim_numudis_3p1_minosp_90pctCL          -> SetLineWidth(2);
  lim_numudis_3p1_minisciboone_90pctCL    -> SetLineWidth(2);
  
  lim_numudis_3p1_sbnproposal_90pctCL     -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_stat    -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_syst    -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_syst_old-> SetMarkerStyle(0);
  valor_proposal_sbn_3p1_contour_90_stat  -> SetMarkerStyle(0);
  valor_proposal_sbn_3p1_contour_90_syst  -> SetMarkerStyle(0);
  cafana_modern_sbn_3p1_contour_90_stat   -> SetMarkerStyle(0);
  cafana_modern_sbn_3p1_contour_90_syst   -> SetMarkerStyle(0);
  cafana_proposal_sbn_3p1_contour_90_stat -> SetMarkerStyle(0);
  cafana_proposal_sbn_3p1_contour_90_syst -> SetMarkerStyle(0);
  sbnfit_modern_sbn_3p1_contour_90_stat   -> SetMarkerStyle(0);
  sbnfit_modern_sbn_3p1_contour_90_syst   -> SetMarkerStyle(0);
  sbnfit_proposal_sbn_3p1_contour_90_stat -> SetMarkerStyle(0);
  sbnfit_proposal_sbn_3p1_contour_90_syst -> SetMarkerStyle(0);
  lim_numudis_3p1_minosp_90pctCL          -> SetMarkerStyle(0);
  lim_numudis_3p1_minisciboone_90pctCL    -> SetMarkerStyle(0);

  // 
  // Modern only, VALOR iteration
  //

  TCanvas * cnumudismodernvalor = new TCanvas("cnumudismodernvalor","",20,20,700,900);
  cnumudismodernvalor->Draw();

  TH2F * hfnumudismodernvalor = (TH2F*) cnumudismodernvalor->DrawFrame(1E-3,1E-2,1,1E+2);
  hfnumudismodernvalor->Draw();
  hfnumudismodernvalor->GetYaxis()->SetTitleOffset(1.3);
  hfnumudismodernvalor->GetYaxis()->SetTitleOffset(1.1);
  hfnumudismodernvalor->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudismodernvalor->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudismodernvalor->GetXaxis()->SetTitleFont(132);
  hfnumudismodernvalor->GetYaxis()->SetTitleFont(132);
  hfnumudismodernvalor->GetXaxis()->SetLabelFont(132);
  hfnumudismodernvalor->GetYaxis()->SetLabelFont(132);
  cnumudismodernvalor->SetLogx();
  cnumudismodernvalor->SetLogy();

  valor_modern_sbn_3p1_contour_90_stat    -> Draw("l");
  valor_modern_sbn_3p1_contour_90_syst    -> Draw("l");
  valor_modern_sbn_3p1_contour_90_syst_old-> Draw("l");
  cafana_modern_sbn_3p1_contour_90_stat   -> Draw("l");
  cafana_modern_sbn_3p1_contour_90_syst   -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_90_stat   -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_90_syst   -> Draw("l");

  TLegend * lgnumudismodernvalor = new TLegend(0.12,0.12,0.67,0.38);
  lgnumudismodernvalor->SetFillStyle(0);
  lgnumudismodernvalor->SetBorderSize(0);
  lgnumudismodernvalor->SetTextFont(132);
  lgnumudismodernvalor->AddEntry(cafana_modern_sbn_3p1_contour_90_syst,   "CAFAna modern 90\% CL ",                    "L");
  lgnumudismodernvalor->AddEntry(cafana_modern_sbn_3p1_contour_90_stat,   "CAFAna modern 90\% CL (stat-only)",         "L");
  lgnumudismodernvalor->AddEntry(sbnfit_modern_sbn_3p1_contour_90_syst,   "SBNFit modern 90\% CL ",                    "L");
  lgnumudismodernvalor->AddEntry(sbnfit_modern_sbn_3p1_contour_90_stat,   "SBNFit modern 90\% CL (stat-only)",         "L");
  lgnumudismodernvalor->AddEntry(valor_modern_sbn_3p1_contour_90_syst_old,"VALOR modern 90\% CL (E_{Reco}, spline BL)","L");
  lgnumudismodernvalor->AddEntry(valor_modern_sbn_3p1_contour_90_syst,    "VALOR modern 90\% CL (E_{True}, true BL)",  "L");
  lgnumudismodernvalor->AddEntry(valor_modern_sbn_3p1_contour_90_stat,    "VALOR modern 90\% CL (stat-only)",          "L");
  lgnumudismodernvalor->Draw();

  // 
  // External data, modern only 
  //

  TCanvas * cnumudismodern = new TCanvas("cnumudismodern","",20,20,700,900);
  cnumudismodern->Draw();

  TH2F * hfnumudismodern = (TH2F*) cnumudismodern->DrawFrame(1E-3,1E-2,1,1E+2);
  hfnumudismodern->Draw();
  hfnumudismodern->GetYaxis()->SetTitleOffset(1.3);
  hfnumudismodern->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudismodern->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudismodern->GetXaxis()->SetTitleFont(132);
  hfnumudismodern->GetYaxis()->SetTitleFont(132);
  hfnumudismodern->GetXaxis()->SetLabelFont(132);
  hfnumudismodern->GetYaxis()->SetLabelFont(132);
  cnumudismodern->SetLogx();
  cnumudismodern->SetLogy();

  valor_modern_sbn_3p1_contour_90_stat    -> Draw("l");
  valor_modern_sbn_3p1_contour_90_syst    -> Draw("l");
  cafana_modern_sbn_3p1_contour_90_stat   -> Draw("l");
  cafana_modern_sbn_3p1_contour_90_syst   -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_90_stat   -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_90_syst   -> Draw("l");
  lim_numudis_3p1_minosp_90pctCL          -> Draw("l");
  lim_numudis_3p1_minisciboone_90pctCL    -> Draw("l");

  TLegend * lgnumudismodern = new TLegend(0.36,0.12,0.67,0.35);
  lgnumudismodern->SetFillStyle(0);
  lgnumudismodern->SetBorderSize(0);
  lgnumudismodern->SetTextFont(132);
  lgnumudismodern->AddEntry(cafana_modern_sbn_3p1_contour_90_syst,   "CAFAna modern 90\% CL ",            "L");
  lgnumudismodern->AddEntry(cafana_modern_sbn_3p1_contour_90_stat,   "CAFAna modern 90\% CL (stat-only)", "L");
  lgnumudismodern->AddEntry(lim_numudis_3p1_minisciboone_90pctCL,    "MiniBooNE/SciBooNE 90\% CL",        "L");
  lgnumudismodern->AddEntry(lim_numudis_3p1_minosp_90pctCL,          "MINOS/MINOS+ 90\% CL",              "L");
  lgnumudismodern->AddEntry(sbnfit_modern_sbn_3p1_contour_90_syst,   "SBNFit modern 90\% CL ",            "L");
  lgnumudismodern->AddEntry(sbnfit_modern_sbn_3p1_contour_90_stat,   "SBNFit modern 90\% CL (stat-only)", "L");
  lgnumudismodern->AddEntry(valor_modern_sbn_3p1_contour_90_syst,    "VALOR modern 90\% CL ",             "L");
  lgnumudismodern->AddEntry(valor_modern_sbn_3p1_contour_90_stat,    "VALOR modern 90\% CL (stat-only)",  "L");
  lgnumudismodern->Draw();

  // 
  // Modern only 
  //

  TCanvas * cnumudismodernnoext = new TCanvas("cnumudismodernnoext","",20,20,700,900);
  cnumudismodernnoext->Draw();

  TH2F * hfnumudismodernnoext = (TH2F*) cnumudismodernnoext->DrawFrame(1E-3,1E-2,1,1E+2);
  hfnumudismodernnoext->Draw();
  hfnumudismodernnoext->GetYaxis()->SetTitleOffset(1.3);
  hfnumudismodernnoext->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudismodernnoext->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudismodernnoext->GetXaxis()->SetTitleFont(132);
  hfnumudismodernnoext->GetYaxis()->SetTitleFont(132);
  hfnumudismodernnoext->GetXaxis()->SetLabelFont(132);
  hfnumudismodernnoext->GetYaxis()->SetLabelFont(132);
  cnumudismodernnoext->SetLogx();
  cnumudismodernnoext->SetLogy();

  valor_modern_sbn_3p1_contour_90_stat    -> Draw("l");
  valor_modern_sbn_3p1_contour_90_syst    -> Draw("l");
  cafana_modern_sbn_3p1_contour_90_stat   -> Draw("l");
  cafana_modern_sbn_3p1_contour_90_syst   -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_90_stat   -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_90_syst   -> Draw("l");
  injected->Draw("P");

  TLegend * lgnumudismodernnoext = new TLegend(0.12,0.14,0.71,0.36);
  lgnumudismodernnoext->SetFillStyle(0);
  lgnumudismodernnoext->SetBorderSize(0);
  lgnumudismodernnoext->SetTextFont(132);
  lgnumudismodernnoext->AddEntry(cafana_modern_sbn_3p1_contour_90_syst,   "CAFAna modern 90\% CL ",            "L");
  lgnumudismodernnoext->AddEntry(cafana_modern_sbn_3p1_contour_90_stat,   "CAFAna modern 90\% CL (stat-only)", "L");
  lgnumudismodernnoext->AddEntry(sbnfit_modern_sbn_3p1_contour_90_syst,   "SBNFit modern 90\% CL ",            "L");
  lgnumudismodernnoext->AddEntry(sbnfit_modern_sbn_3p1_contour_90_stat,   "SBNFit modern 90\% CL (stat-only)", "L");
  lgnumudismodernnoext->AddEntry(valor_modern_sbn_3p1_contour_90_syst,    "VALOR modern 90\% CL ",             "L");
  lgnumudismodernnoext->AddEntry(valor_modern_sbn_3p1_contour_90_stat,    "VALOR modern 90\% CL (stat-only)",  "L");
  lgnumudismodernnoext->AddEntry(injected, "Injected point - sin^{2}2#theta_{#mu#mu} = 0.07, #Delta m^{2}_{41} = 1.32", "P");
  lgnumudismodernnoext->Draw();

  // 
  // Modern only with OG proposal 
  //

  TCanvas * cnumudismodernproponly = new TCanvas("cnumudismodernproponly","",20,20,700,900);
  cnumudismodernproponly->Draw();

  TH2F * hfnumudismodernproponly = (TH2F*) cnumudismodernproponly->DrawFrame(1E-3,1E-2,1,1E+2);
  hfnumudismodernproponly->Draw();
  hfnumudismodernproponly->GetYaxis()->SetTitleOffset(1.3);
  hfnumudismodernproponly->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudismodernproponly->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudismodernproponly->GetXaxis()->SetTitleFont(132);
  hfnumudismodernproponly->GetYaxis()->SetTitleFont(132);
  hfnumudismodernproponly->GetXaxis()->SetLabelFont(132);
  hfnumudismodernproponly->GetYaxis()->SetLabelFont(132);
  cnumudismodernproponly->SetLogx();
  cnumudismodernproponly->SetLogy();

  lim_numudis_3p1_sbnproposal_90pctCL     -> Draw("l");
  valor_modern_sbn_3p1_contour_90_stat    -> Draw("l");
  valor_modern_sbn_3p1_contour_90_syst    -> Draw("l");
  cafana_modern_sbn_3p1_contour_90_stat   -> Draw("l");
  cafana_modern_sbn_3p1_contour_90_syst   -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_90_stat   -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_90_syst   -> Draw("l");
  injected->Draw("P");

  TLegend * lgnumudismodernproponly = new TLegend(0.12,0.14,0.71,0.36);
  lgnumudismodernproponly->SetFillStyle(0);
  lgnumudismodernproponly->SetBorderSize(0);
  lgnumudismodernproponly->SetTextFont(132);
  lgnumudismodernproponly->AddEntry(lim_numudis_3p1_sbnproposal_90pctCL,     "SBN proposal 90\% CL",              "L");
  lgnumudismodernproponly->AddEntry(cafana_modern_sbn_3p1_contour_90_syst,   "CAFAna modern 90\% CL ",            "L");
  lgnumudismodernproponly->AddEntry(cafana_modern_sbn_3p1_contour_90_stat,   "CAFAna modern 90\% CL (stat-only)", "L");
  lgnumudismodernproponly->AddEntry(sbnfit_modern_sbn_3p1_contour_90_syst,   "SBNFit modern 90\% CL ",            "L");
  lgnumudismodernproponly->AddEntry(sbnfit_modern_sbn_3p1_contour_90_stat,   "SBNFit modern 90\% CL (stat-only)", "L");
  lgnumudismodernproponly->AddEntry(valor_modern_sbn_3p1_contour_90_syst,    "VALOR modern 90\% CL ",             "L");
  lgnumudismodernproponly->AddEntry(valor_modern_sbn_3p1_contour_90_stat,    "VALOR modern 90\% CL (stat-only)",  "L");
  lgnumudismodernproponly->AddEntry(injected, "Injected point - sin^{2}2#theta_{#mu#mu} = 0.07, #Delta m^{2}_{41} = 1.32", "P");
  lgnumudismodernproponly->Draw();

  // 
  // External data, proposal only
  //
 
  TCanvas * cnumudisprop = new TCanvas("cnumudisprop","",20,20,700,900);
  cnumudisprop->Draw();

  TH2F * hfnumudisprop = (TH2F*) cnumudisprop->DrawFrame(1E-3,1E-2,1,1E+2);
  hfnumudisprop->Draw();
  hfnumudisprop->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisprop->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisprop->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisprop->GetXaxis()->SetTitleFont(132);
  hfnumudisprop->GetYaxis()->SetTitleFont(132);
  hfnumudisprop->GetXaxis()->SetLabelFont(132);
  hfnumudisprop->GetYaxis()->SetLabelFont(132);
  cnumudisprop->SetLogx();
  cnumudisprop->SetLogy();

  lim_numudis_3p1_sbnproposal_90pctCL     -> Draw("l");
  valor_proposal_sbn_3p1_contour_90_stat  -> Draw("l");
  valor_proposal_sbn_3p1_contour_90_syst  -> Draw("l");
  cafana_proposal_sbn_3p1_contour_90_stat -> Draw("l");
  cafana_proposal_sbn_3p1_contour_90_syst -> Draw("l");
  sbnfit_proposal_sbn_3p1_contour_90_stat -> Draw("l");
  sbnfit_proposal_sbn_3p1_contour_90_syst -> Draw("l");
  lim_numudis_3p1_minosp_90pctCL          -> Draw("l");
  lim_numudis_3p1_minisciboone_90pctCL    -> Draw("l");

  TLegend * lgnumudisprop = new TLegend(0.36,0.12,0.67,0.35);
  lgnumudisprop->SetFillStyle(0);
  lgnumudisprop->SetBorderSize(0);
  lgnumudisprop->SetTextFont(132);
  lgnumudisprop->AddEntry(cafana_proposal_sbn_3p1_contour_90_syst, "CAFAna proposal 90\% CL ",            "L");
  lgnumudisprop->AddEntry(cafana_proposal_sbn_3p1_contour_90_stat, "CAFAna proposal 90\% CL (stat-only)", "L");
  lgnumudisprop->AddEntry(lim_numudis_3p1_minisciboone_90pctCL,    "MiniBooNE/SciBooNE 90\% CL",          "L");
  lgnumudisprop->AddEntry(lim_numudis_3p1_minosp_90pctCL,          "MINOS/MINOS+ 90\% CL",                "L");
  lgnumudisprop->AddEntry(sbnfit_proposal_sbn_3p1_contour_90_syst, "SBNFit proposal 90\% CL ",            "L");
  lgnumudisprop->AddEntry(sbnfit_proposal_sbn_3p1_contour_90_stat, "SBNFit proposal 90\% CL (stat-only)", "L");
  lgnumudisprop->AddEntry(lim_numudis_3p1_sbnproposal_90pctCL,     "SBN proposal 90\% CL",                "L");
  lgnumudisprop->AddEntry(valor_proposal_sbn_3p1_contour_90_syst,  "VALOR proposal 90\% CL ",             "L");
  lgnumudisprop->AddEntry(valor_proposal_sbn_3p1_contour_90_stat,  "VALOR proposal 90\% CL (stat-only)",  "L");
  lgnumudisprop->Draw();

  //
  // numu disappearance sbn only
  //

  TCanvas * cnumudissbnonly = new TCanvas("cnumudissbnonly","",20,20,700,900);
  cnumudissbnonly->Draw();

  TH2F * hfnumudissbnonly = (TH2F*) cnumudissbnonly->DrawFrame(1E-3,4E-2,1,1E+2);
  hfnumudissbnonly->Draw();
  hfnumudissbnonly->GetYaxis()->SetTitleOffset(1.3);
  hfnumudissbnonly->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudissbnonly->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudissbnonly->GetXaxis()->SetTitleFont(132);
  hfnumudissbnonly->GetYaxis()->SetTitleFont(132);
  hfnumudissbnonly->GetXaxis()->SetLabelFont(132);
  hfnumudissbnonly->GetYaxis()->SetLabelFont(132);
  cnumudissbnonly->SetLogx();
  cnumudissbnonly->SetLogy();

  cafana_modern_sbn_3p1_contour_90_stat   -> Draw("l");
  cafana_modern_sbn_3p1_contour_90_syst   -> Draw("l");
  cafana_proposal_sbn_3p1_contour_90_stat -> Draw("l");
  cafana_proposal_sbn_3p1_contour_90_syst -> Draw("l");
  lim_numudis_3p1_sbnproposal_90pctCL     -> Draw("l");
  valor_modern_sbn_3p1_contour_90_stat    -> Draw("l");
  valor_modern_sbn_3p1_contour_90_syst    -> Draw("l");
  valor_proposal_sbn_3p1_contour_90_stat  -> Draw("l");
  valor_proposal_sbn_3p1_contour_90_syst  -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_90_stat   -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_90_syst   -> Draw("l");
  sbnfit_proposal_sbn_3p1_contour_90_stat -> Draw("l");
  sbnfit_proposal_sbn_3p1_contour_90_syst -> Draw("l");


  TLegend * lgnumudissbnonly = new TLegend(0.46,0.36,0.91,0.74);
  lgnumudissbnonly->SetFillStyle(0);
  lgnumudissbnonly->SetBorderSize(0);
  lgnumudissbnonly->SetTextFont(132);

  lgnumudissbnonly->AddEntry(cafana_proposal_sbn_3p1_contour_90_stat, "CAFAna proposal 90\% CL (stat-only)", "L");
  lgnumudissbnonly->AddEntry(cafana_proposal_sbn_3p1_contour_90_syst, "CAFAna proposal 90\% CL ",            "L");
  lgnumudissbnonly->AddEntry(cafana_modern_sbn_3p1_contour_90_stat,   "CAFAna modern 90\% CL (stat-only)",   "L");
  lgnumudissbnonly->AddEntry(cafana_modern_sbn_3p1_contour_90_syst,   "CAFAna modern 90\% CL ",              "L");
  lgnumudissbnonly->AddEntry(sbnfit_proposal_sbn_3p1_contour_90_stat, "SBNFit proposal 90\% CL (stat-only)", "L");
  lgnumudissbnonly->AddEntry(sbnfit_proposal_sbn_3p1_contour_90_syst, "SBNFit proposal 90\% CL ",            "L");
  lgnumudissbnonly->AddEntry(sbnfit_modern_sbn_3p1_contour_90_stat,   "SBNFit modern 90\% CL (stat-only)",   "L");
  lgnumudissbnonly->AddEntry(sbnfit_modern_sbn_3p1_contour_90_syst,   "SBNFit modern 90\% CL ",              "L");
  lgnumudissbnonly->AddEntry(lim_numudis_3p1_sbnproposal_90pctCL,     "SBN proposal 90\% CL",                "L");
  lgnumudissbnonly->AddEntry(valor_proposal_sbn_3p1_contour_90_stat,  "VALOR proposal 90\% CL (stat-only)",  "L");
  lgnumudissbnonly->AddEntry(valor_proposal_sbn_3p1_contour_90_syst,  "VALOR proposal 90\% CL ",             "L");
  lgnumudissbnonly->AddEntry(valor_modern_sbn_3p1_contour_90_stat,    "VALOR modern 90\% CL (stat-only)",    "L");
  lgnumudissbnonly->AddEntry(valor_modern_sbn_3p1_contour_90_syst,    "VALOR modern 90\% CL ",               "L");
  lgnumudissbnonly->Draw();

  //
  // numu disappearance VALOR sbn only
  //

  TCanvas * cnumudissbnonlyvalor = new TCanvas("cnumudissbnonlyvalor","",20,20,700,900);
  cnumudissbnonlyvalor->Draw();

  TH2F * hfnumudissbnonlyvalor = (TH2F*) cnumudissbnonlyvalor->DrawFrame(1E-3,4E-2,1,1E+2);
  hfnumudissbnonlyvalor->Draw();
  hfnumudissbnonlyvalor->GetYaxis()->SetTitleOffset(1.3);
  hfnumudissbnonlyvalor->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudissbnonlyvalor->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudissbnonlyvalor->GetXaxis()->SetTitleFont(132);
  hfnumudissbnonlyvalor->GetYaxis()->SetTitleFont(132);
  hfnumudissbnonlyvalor->GetXaxis()->SetLabelFont(132);
  hfnumudissbnonlyvalor->GetYaxis()->SetLabelFont(132);
  cnumudissbnonlyvalor->SetLogx();
  cnumudissbnonlyvalor->SetLogy();

  lim_numudis_3p1_sbnproposal_90pctCL     -> Draw("l");
  valor_modern_sbn_3p1_contour_90_stat    -> Draw("l");
  valor_modern_sbn_3p1_contour_90_syst    -> Draw("l");
  valor_proposal_sbn_3p1_contour_90_stat  -> Draw("l");
  valor_proposal_sbn_3p1_contour_90_syst  -> Draw("l");

  TLegend * lgnumudissbnonlyvalor = new TLegend(0.46,0.36,0.91,0.74);
  lgnumudissbnonlyvalor->SetFillStyle(0);
  lgnumudissbnonlyvalor->SetBorderSize(0);
  lgnumudissbnonlyvalor->SetTextFont(132);

  lgnumudissbnonlyvalor->AddEntry(lim_numudis_3p1_sbnproposal_90pctCL,     "SBN proposal 90\% CL",                "L");
  lgnumudissbnonlyvalor->AddEntry(valor_proposal_sbn_3p1_contour_90_stat,  "VALOR proposal 90\% CL (stat-only)",  "L");
  lgnumudissbnonlyvalor->AddEntry(valor_proposal_sbn_3p1_contour_90_syst,  "VALOR proposal 90\% CL ",             "L");
  lgnumudissbnonlyvalor->AddEntry(valor_modern_sbn_3p1_contour_90_stat,    "VALOR modern 90\% CL (stat-only)",    "L");
  lgnumudissbnonlyvalor->AddEntry(valor_modern_sbn_3p1_contour_90_syst,    "VALOR modern 90\% CL ",               "L");
  lgnumudissbnonlyvalor->Draw();

  //
  // numu disappearance CAFAna sbn only
  //

  TCanvas * cnumudissbnonlycafana = new TCanvas("cnumudissbnonlycafana","",20,20,700,900);
  cnumudissbnonlycafana->Draw();

  TH2F * hfnumudissbnonlycafana = (TH2F*) cnumudissbnonlycafana->DrawFrame(1E-3,4E-2,1,1E+2);
  hfnumudissbnonlycafana->Draw();
  hfnumudissbnonlycafana->GetYaxis()->SetTitleOffset(1.3);
  hfnumudissbnonlycafana->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudissbnonlycafana->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudissbnonlycafana->GetXaxis()->SetTitleFont(132);
  hfnumudissbnonlycafana->GetYaxis()->SetTitleFont(132);
  hfnumudissbnonlycafana->GetXaxis()->SetLabelFont(132);
  hfnumudissbnonlycafana->GetYaxis()->SetLabelFont(132);
  cnumudissbnonlycafana->SetLogx();
  cnumudissbnonlycafana->SetLogy();

  lim_numudis_3p1_sbnproposal_90pctCL     -> Draw("l");
  cafana_modern_sbn_3p1_contour_90_stat   -> Draw("l");
  cafana_modern_sbn_3p1_contour_90_syst   -> Draw("l");
  cafana_proposal_sbn_3p1_contour_90_stat -> Draw("l");
  cafana_proposal_sbn_3p1_contour_90_syst -> Draw("l");

  TLegend * lgnumudissbnonlycafana = new TLegend(0.46,0.36,0.91,0.74);
  lgnumudissbnonlycafana->SetFillStyle(0);
  lgnumudissbnonlycafana->SetBorderSize(0);
  lgnumudissbnonlycafana->SetTextFont(132);
  lgnumudissbnonlycafana->AddEntry(cafana_proposal_sbn_3p1_contour_90_stat, "CAFAna proposal 90\% CL (stat-only)", "L");
  lgnumudissbnonlycafana->AddEntry(cafana_proposal_sbn_3p1_contour_90_syst, "CAFAna proposal 90\% CL ",            "L");
  lgnumudissbnonlycafana->AddEntry(cafana_modern_sbn_3p1_contour_90_stat,   "CAFAna modern 90\% CL (stat-only)",   "L");
  lgnumudissbnonlycafana->AddEntry(cafana_modern_sbn_3p1_contour_90_syst,   "CAFAna modern 90\% CL ",              "L");
  lgnumudissbnonlycafana->AddEntry(lim_numudis_3p1_sbnproposal_90pctCL,     "SBN proposal 90\% CL",                "L");
  lgnumudissbnonlycafana->Draw();

  //
  // numu disappearance SBNFit sbn only
  //
  
  TCanvas * cnumudissbnonlysbnfit = new TCanvas("cnumudissbnonlysbnfit","",20,20,700,900);
  cnumudissbnonlysbnfit->Draw();

  TH2F * hfnumudissbnonlysbnfit = (TH2F*) cnumudissbnonlysbnfit->DrawFrame(1E-3,4E-2,1,1E+2);
  hfnumudissbnonlysbnfit->Draw();
  hfnumudissbnonlysbnfit->GetYaxis()->SetTitleOffset(1.3);
  hfnumudissbnonlysbnfit->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudissbnonlysbnfit->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudissbnonlysbnfit->GetXaxis()->SetTitleFont(132);
  hfnumudissbnonlysbnfit->GetYaxis()->SetTitleFont(132);
  hfnumudissbnonlysbnfit->GetXaxis()->SetLabelFont(132);
  hfnumudissbnonlysbnfit->GetYaxis()->SetLabelFont(132);
  cnumudissbnonlysbnfit->SetLogx();
  cnumudissbnonlysbnfit->SetLogy();

  lim_numudis_3p1_sbnproposal_90pctCL     -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_90_stat   -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_90_syst   -> Draw("l");
  sbnfit_proposal_sbn_3p1_contour_90_stat -> Draw("l");
  sbnfit_proposal_sbn_3p1_contour_90_syst -> Draw("l");

  TLegend * lgnumudissbnonlysbnfit = new TLegend(0.46,0.36,0.91,0.74);
  lgnumudissbnonlysbnfit->SetFillStyle(0);
  lgnumudissbnonlysbnfit->SetBorderSize(0);
  lgnumudissbnonlysbnfit->SetTextFont(132);
  lgnumudissbnonlysbnfit->AddEntry(sbnfit_proposal_sbn_3p1_contour_90_stat, "SBNFit proposal 90\% CL (stat-only)", "L");
  lgnumudissbnonlysbnfit->AddEntry(sbnfit_proposal_sbn_3p1_contour_90_syst, "SBNFit proposal 90\% CL ",            "L");
  lgnumudissbnonlysbnfit->AddEntry(sbnfit_modern_sbn_3p1_contour_90_stat,   "SBNFit modern 90\% CL (stat-only)",   "L");
  lgnumudissbnonlysbnfit->AddEntry(sbnfit_modern_sbn_3p1_contour_90_syst,   "SBNFit modern 90\% CL ",              "L");
  lgnumudissbnonlysbnfit->AddEntry(lim_numudis_3p1_sbnproposal_90pctCL,     "SBN proposal 90\% CL",                "L");
  lgnumudissbnonlysbnfit->Draw();
  
  //
  // numu disappearance VALOR sbn only Stat+Syst only proposal
  //

  TCanvas * cnumudissbnonlysystonlyvalor = new TCanvas("cnumudissbnonlysystonlyvalor","",20,20,700,900);
  cnumudissbnonlysystonlyvalor->Draw();

  TH2F * hfnumudissbnonlysystonlyvalor = (TH2F*) cnumudissbnonlysystonlyvalor->DrawFrame(1E-3,4E-2,1,1E+2);
  hfnumudissbnonlysystonlyvalor->Draw();
  hfnumudissbnonlysystonlyvalor->GetYaxis()->SetTitleOffset(1.3);
  hfnumudissbnonlysystonlyvalor->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudissbnonlysystonlyvalor->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudissbnonlysystonlyvalor->GetXaxis()->SetTitleFont(132);
  hfnumudissbnonlysystonlyvalor->GetYaxis()->SetTitleFont(132);
  hfnumudissbnonlysystonlyvalor->GetXaxis()->SetLabelFont(132);
  hfnumudissbnonlysystonlyvalor->GetYaxis()->SetLabelFont(132);
  cnumudissbnonlysystonlyvalor->SetLogx();
  cnumudissbnonlysystonlyvalor->SetLogy();

  lim_numudis_3p1_sbnproposal_90pctCL     -> Draw("l");
  valor_proposal_sbn_3p1_contour_90_syst  -> Draw("l");
  valor_proposal_sbn_3p1_contour_90_syst  -> SetLineStyle(1);

  TLegend * lgnumudissbnonlysystonlyvalor = new TLegend(0.39,0.41,0.94,0.62);
  lgnumudissbnonlysystonlyvalor->SetFillStyle(0);
  lgnumudissbnonlysystonlyvalor->SetBorderSize(0);
  lgnumudissbnonlysystonlyvalor->SetTextFont(132);

  lgnumudissbnonlysystonlyvalor->AddEntry(lim_numudis_3p1_sbnproposal_90pctCL,     "SBN proposal 90\% CL",    "L");
  lgnumudissbnonlysystonlyvalor->AddEntry(valor_proposal_sbn_3p1_contour_90_syst,  "VALOR proposal 90\% CL ", "L");
  lgnumudissbnonlysystonlyvalor->Draw();

  valor_proposal_sbn_3p1_contour_90_syst  -> SetLineStyle(2);

  //
  // numu disappearance CAFAna sbn only
  //

  TCanvas * cnumudissbnonlysystonlycafana = new TCanvas("cnumudissbnonlysystonlycafana","",20,20,700,900);
  cnumudissbnonlysystonlycafana->Draw();

  TH2F * hfnumudissbnonlysystonlycafana = (TH2F*) cnumudissbnonlysystonlycafana->DrawFrame(1E-3,4E-2,1,1E+2);
  hfnumudissbnonlysystonlycafana->Draw();
  hfnumudissbnonlysystonlycafana->GetYaxis()->SetTitleOffset(1.3);
  hfnumudissbnonlysystonlycafana->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudissbnonlysystonlycafana->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudissbnonlysystonlycafana->GetXaxis()->SetTitleFont(132);
  hfnumudissbnonlysystonlycafana->GetYaxis()->SetTitleFont(132);
  hfnumudissbnonlysystonlycafana->GetXaxis()->SetLabelFont(132);
  hfnumudissbnonlysystonlycafana->GetYaxis()->SetLabelFont(132);
  cnumudissbnonlysystonlycafana->SetLogx();
  cnumudissbnonlysystonlycafana->SetLogy();

  lim_numudis_3p1_sbnproposal_90pctCL     -> Draw("l");
  cafana_proposal_sbn_3p1_contour_90_syst -> Draw("l");
  cafana_proposal_sbn_3p1_contour_90_syst -> SetLineStyle(1);

  TLegend * lgnumudissbnonlysystonlycafana = new TLegend(0.39,0.41,0.94,0.62);
  lgnumudissbnonlysystonlycafana->SetFillStyle(0);
  lgnumudissbnonlysystonlycafana->SetBorderSize(0);
  lgnumudissbnonlysystonlycafana->SetTextFont(132);
  lgnumudissbnonlysystonlycafana->AddEntry(lim_numudis_3p1_sbnproposal_90pctCL,     "SBN proposal 90\% CL",     "L");
  lgnumudissbnonlysystonlycafana->AddEntry(cafana_proposal_sbn_3p1_contour_90_syst, "CAFAna proposal 90\% CL ", "L");
  lgnumudissbnonlysystonlycafana->Draw();

  cafana_proposal_sbn_3p1_contour_90_syst -> SetLineStyle(2);
  
  //
  // numu disappearance SBNFit sbn only
  //
  
  TCanvas * cnumudissbnonlysystonlysbnfit = new TCanvas("cnumudissbnonlysystonlysbnfit","",20,20,700,900);
  cnumudissbnonlysystonlysbnfit->Draw();

  TH2F * hfnumudissbnonlysystonlysbnfit = (TH2F*) cnumudissbnonlysystonlysbnfit->DrawFrame(1E-3,4E-2,1,1E+2);
  hfnumudissbnonlysystonlysbnfit->Draw();
  hfnumudissbnonlysystonlysbnfit->GetYaxis()->SetTitleOffset(1.3);
  hfnumudissbnonlysystonlysbnfit->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudissbnonlysystonlysbnfit->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudissbnonlysystonlysbnfit->GetXaxis()->SetTitleFont(132);
  hfnumudissbnonlysystonlysbnfit->GetYaxis()->SetTitleFont(132);
  hfnumudissbnonlysystonlysbnfit->GetXaxis()->SetLabelFont(132);
  hfnumudissbnonlysystonlysbnfit->GetYaxis()->SetLabelFont(132);
  cnumudissbnonlysystonlysbnfit->SetLogx();
  cnumudissbnonlysystonlysbnfit->SetLogy();

  lim_numudis_3p1_sbnproposal_90pctCL     -> Draw("l");
  sbnfit_proposal_sbn_3p1_contour_90_syst -> Draw("l");
  sbnfit_proposal_sbn_3p1_contour_90_syst20 -> Draw("l");
  sbnfit_proposal_sbn_3p1_contour_90_syst20 -> SetLineColor(kAzure+7);
  sbnfit_proposal_sbn_3p1_contour_90_syst20 -> SetLineWidth(2);
  sbnfit_proposal_sbn_3p1_contour_90_syst20 -> SetLineStyle(9);
  sbnfit_proposal_sbn_3p1_contour_90_syst20 -> SetMarkerStyle(0);

  sbnfit_proposal_sbn_3p1_contour_90_syst -> SetLineStyle(1);

  TLegend * lgnumudissbnonlysystonlysbnfit = new TLegend(0.39,0.41,0.94,0.62);
  lgnumudissbnonlysystonlysbnfit->SetFillStyle(0);
  lgnumudissbnonlysystonlysbnfit->SetBorderSize(0);
  lgnumudissbnonlysystonlysbnfit->SetTextFont(132);
  lgnumudissbnonlysystonlysbnfit->AddEntry(lim_numudis_3p1_sbnproposal_90pctCL,     "SBN proposal 90\% CL",     "L");
  lgnumudissbnonlysystonlysbnfit->AddEntry(sbnfit_proposal_sbn_3p1_contour_90_syst, "SBNFit proposal 90\% CL 25 syst", "L");
  lgnumudissbnonlysystonlysbnfit->AddEntry(sbnfit_proposal_sbn_3p1_contour_90_syst20, "SBNFit proposal 90\% CL 20 syst", "L");
  lgnumudissbnonlysystonlysbnfit->Draw();
  
  sbnfit_proposal_sbn_3p1_contour_90_syst -> SetLineStyle(2);

  // 
  // External data + injected only 
  //

  TCanvas * cnumudisextinj = new TCanvas("cnumudisextinj","",20,20,700,900);
  cnumudisextinj->Draw();

  TH2F * hfnumudisextinj = (TH2F*) cnumudisextinj->DrawFrame(1E-3,1E-2,1,1E+2);
  hfnumudisextinj->Draw();
  hfnumudisextinj->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisextinj->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisextinj->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisextinj->GetXaxis()->SetTitleFont(132);
  hfnumudisextinj->GetYaxis()->SetTitleFont(132);
  hfnumudisextinj->GetXaxis()->SetLabelFont(132);
  hfnumudisextinj->GetYaxis()->SetLabelFont(132);
  cnumudisextinj->SetLogx();
  cnumudisextinj->SetLogy();

  lim_numudis_3p1_sbnproposal_90pctCL     -> Draw("l");
  lim_numudis_3p1_minosp_90pctCL          -> Draw("l");
  lim_numudis_3p1_minisciboone_90pctCL    -> Draw("l");
  injected                                -> Draw("P");

  TLegend * lgnumudisextinj = new TLegend(0.33,0.14,0.85,0.30);
  lgnumudisextinj->SetFillStyle(0);
  lgnumudisextinj->SetBorderSize(0);
  lgnumudisextinj->SetTextFont(132);
  lgnumudisextinj->AddEntry(lim_numudis_3p1_minisciboone_90pctCL,    "MiniBooNE/SciBooNE 90\% CL",        "L");
  lgnumudisextinj->AddEntry(lim_numudis_3p1_minosp_90pctCL,          "MINOS/MINOS+ 90\% CL",              "L");
  lgnumudisextinj->AddEntry(lim_numudis_3p1_sbnproposal_90pctCL,     "SBN proposal 90\% CL",              "L");
  lgnumudisextinj->AddEntry(injected, "Injected point - sin^{2}2#theta_{#mu#mu} = 0.07, #Delta m^{2}_{41} = 1.32", "P");

  lgnumudisextinj->Draw();

  //
  // numu disappearance allowed regions
  //
  
  TGraph * valor_modern_sbn_3p1_contour_90_stat_allowed      = (TGraph *) f_stat_a->Get("contour_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_syst_allowed      = (TGraph *) f_syst_a->Get("contour_90pct_lin_6");
  TGraph * cafana_modern_sbn_3p1_contour_90_stat_allowed     = (TGraph *) f_caf_mod->Get("numu_allowed_all_stat_90pct"); 
  TGraph * cafana_modern_sbn_3p1_contour_90_syst_allowed     = (TGraph *) f_caf_mod->Get("numu_allowed_all_syst_90pct"); 
  TGraph * sbnfit_modern_sbn_3p1_contour_90_stat_allowed     = (TGraph *) f_sbn_mod_a->Get("allow_STATONLY_90CL"); 
  TGraph * sbnfit_modern_sbn_3p1_contour_90_syst_allowed     = (TGraph *) f_sbn_mod_a->Get("allow_all_90CL"); 
  TGraph * valor_modern_sbn_3p1_contour_3sig_stat_allowed    = (TGraph *) f_stat_a->Get("contour_3sigma_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_3sig_syst_allowed    = (TGraph *) f_syst_a->Get("contour_3sigma_lin_6");
  TGraph * cafana_modern_sbn_3p1_contour_3sig_stat_allowed   = (TGraph *) f_caf_mod->Get("numu_allowed_all_stat_3sig"); 
  TGraph * cafana_modern_sbn_3p1_contour_3sig_syst_allowed   = (TGraph *) f_caf_mod->Get("numu_allowed_all_syst_3sig"); 
  TGraph * sbnfit_modern_sbn_3p1_contour_3sig_stat_allowed   = (TGraph *) f_sbn_mod_a->Get("allow_STATONLY_3sigma"); 
  TGraph * sbnfit_modern_sbn_3p1_contour_3sig_syst_allowed   = (TGraph *) f_sbn_mod_a->Get("allow_all_3sigma"); 
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_allowed    = (TGraph *) f_stat_a->Get("contour_5sigma_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_allowed    = (TGraph *) f_syst_a->Get("contour_5sigma_lin_5");
  TGraph * cafana_modern_sbn_3p1_contour_5sig_stat_allowed   = (TGraph *) f_caf_mod->Get("numu_allowed_all_stat_5sig"); 
  TGraph * cafana_modern_sbn_3p1_contour_5sig_syst_allowed   = (TGraph *) f_caf_mod->Get("numu_allowed_all_syst_5sig");
  TGraph * sbnfit_modern_sbn_3p1_contour_5sig_stat_allowed   = (TGraph *) f_sbn_mod_a->Get("allow_STATONLY_5sigma"); 
  TGraph * sbnfit_modern_sbn_3p1_contour_5sig_syst_allowed   = (TGraph *) f_sbn_mod_a->Get("allow_all_5sigma"); 

  valor_modern_sbn_3p1_contour_90_stat_allowed      -> SetLineColor(kPink+5);
  valor_modern_sbn_3p1_contour_90_stat_allowed      -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_90_syst_allowed      -> SetLineColor(kPink+5);  
  cafana_modern_sbn_3p1_contour_90_stat_allowed     -> SetLineColor(kOrange+1);
  cafana_modern_sbn_3p1_contour_90_stat_allowed     -> SetLineStyle(2);
  cafana_modern_sbn_3p1_contour_90_syst_allowed     -> SetLineColor(kOrange+1);  
  sbnfit_modern_sbn_3p1_contour_90_stat_allowed     -> SetLineColor(kAzure+7);
  sbnfit_modern_sbn_3p1_contour_90_stat_allowed     -> SetLineStyle(2);
  sbnfit_modern_sbn_3p1_contour_90_syst_allowed     -> SetLineColor(kAzure+7);  
  valor_modern_sbn_3p1_contour_3sig_stat_allowed    -> SetLineColor(kPink+5);
  valor_modern_sbn_3p1_contour_3sig_stat_allowed    -> SetLineStyle(7);
  valor_modern_sbn_3p1_contour_3sig_syst_allowed    -> SetLineColor(kPink+5);  
  cafana_modern_sbn_3p1_contour_3sig_stat_allowed   -> SetLineColor(kOrange+1);
  cafana_modern_sbn_3p1_contour_3sig_stat_allowed   -> SetLineStyle(7);
  cafana_modern_sbn_3p1_contour_3sig_syst_allowed   -> SetLineColor(kOrange+1);  
  sbnfit_modern_sbn_3p1_contour_3sig_stat_allowed   -> SetLineColor(kAzure+7);
  sbnfit_modern_sbn_3p1_contour_3sig_stat_allowed   -> SetLineStyle(7);
  sbnfit_modern_sbn_3p1_contour_3sig_syst_allowed   -> SetLineColor(kAzure+7);  
  valor_modern_sbn_3p1_contour_5sig_stat_allowed    -> SetLineColor(kPink+5);
  valor_modern_sbn_3p1_contour_5sig_stat_allowed    -> SetLineStyle(9);
  valor_modern_sbn_3p1_contour_5sig_syst_allowed    -> SetLineColor(kPink+5);  
  cafana_modern_sbn_3p1_contour_5sig_stat_allowed   -> SetLineColor(kOrange+1);
  cafana_modern_sbn_3p1_contour_5sig_stat_allowed   -> SetLineStyle(9);
  cafana_modern_sbn_3p1_contour_5sig_syst_allowed   -> SetLineColor(kOrange+1);  
  sbnfit_modern_sbn_3p1_contour_5sig_stat_allowed   -> SetLineColor(kAzure+7);
  sbnfit_modern_sbn_3p1_contour_5sig_stat_allowed   -> SetLineStyle(9);
  sbnfit_modern_sbn_3p1_contour_5sig_syst_allowed   -> SetLineColor(kAzure+7);  
  
  valor_modern_sbn_3p1_contour_90_stat_allowed      -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_syst_allowed      -> SetLineWidth(2);
  cafana_modern_sbn_3p1_contour_90_stat_allowed     -> SetLineWidth(2);
  cafana_modern_sbn_3p1_contour_90_syst_allowed     -> SetLineWidth(2);
  sbnfit_modern_sbn_3p1_contour_90_stat_allowed     -> SetLineWidth(2);
  sbnfit_modern_sbn_3p1_contour_90_syst_allowed     -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_3sig_stat_allowed    -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_3sig_syst_allowed    -> SetLineWidth(2);
  cafana_modern_sbn_3p1_contour_3sig_stat_allowed   -> SetLineWidth(2);
  cafana_modern_sbn_3p1_contour_3sig_syst_allowed   -> SetLineWidth(2);
  sbnfit_modern_sbn_3p1_contour_3sig_stat_allowed   -> SetLineWidth(2);
  sbnfit_modern_sbn_3p1_contour_3sig_syst_allowed   -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_stat_allowed    -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_syst_allowed    -> SetLineWidth(2);
  cafana_modern_sbn_3p1_contour_5sig_stat_allowed   -> SetLineWidth(2);
  cafana_modern_sbn_3p1_contour_5sig_syst_allowed   -> SetLineWidth(2);
  sbnfit_modern_sbn_3p1_contour_5sig_stat_allowed   -> SetLineWidth(2);
  sbnfit_modern_sbn_3p1_contour_5sig_syst_allowed   -> SetLineWidth(2);


  valor_modern_sbn_3p1_contour_90_stat_allowed      -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_syst_allowed      -> SetMarkerStyle(0);
  cafana_modern_sbn_3p1_contour_90_stat_allowed     -> SetMarkerStyle(0);
  cafana_modern_sbn_3p1_contour_90_syst_allowed     -> SetMarkerStyle(0);
  sbnfit_modern_sbn_3p1_contour_90_stat_allowed     -> SetMarkerStyle(0);
  sbnfit_modern_sbn_3p1_contour_90_syst_allowed     -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_3sig_stat_allowed    -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_3sig_syst_allowed    -> SetMarkerStyle(0);
  cafana_modern_sbn_3p1_contour_3sig_stat_allowed   -> SetMarkerStyle(0);
  cafana_modern_sbn_3p1_contour_3sig_syst_allowed   -> SetMarkerStyle(0);
  sbnfit_modern_sbn_3p1_contour_3sig_stat_allowed   -> SetMarkerStyle(0);
  sbnfit_modern_sbn_3p1_contour_3sig_syst_allowed   -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_5sig_stat_allowed    -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_5sig_syst_allowed    -> SetMarkerStyle(0);
  cafana_modern_sbn_3p1_contour_5sig_stat_allowed   -> SetMarkerStyle(0);
  cafana_modern_sbn_3p1_contour_5sig_syst_allowed   -> SetMarkerStyle(0);
  sbnfit_modern_sbn_3p1_contour_5sig_stat_allowed   -> SetMarkerStyle(0);
  sbnfit_modern_sbn_3p1_contour_5sig_syst_allowed   -> SetMarkerStyle(0);
  
  // 
  // VALOR 3 Sigma
  //
  TCanvas * cnumudisallowedvalor3sig = new TCanvas("cnumudisallowedvalor3sig","",20,20,700,900);
  cnumudisallowedvalor3sig->Draw();

  TH2F * hfnumudisallowedvalor3sig = (TH2F*) cnumudisallowedvalor3sig->DrawFrame(3E-2,7E-1,2E-1,3.);
  hfnumudisallowedvalor3sig->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisallowedvalor3sig->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisallowedvalor3sig->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisallowedvalor3sig->GetXaxis()->SetTitleFont(132);
  hfnumudisallowedvalor3sig->GetYaxis()->SetTitleFont(132);
  hfnumudisallowedvalor3sig->GetXaxis()->SetLabelFont(132);
  hfnumudisallowedvalor3sig->GetYaxis()->SetLabelFont(132);
  cnumudisallowedvalor3sig->SetLogx();
  cnumudisallowedvalor3sig->SetLogy();

  TLegend * lgnumudisallowedvalor3sig = new TLegend(0.12,0.62,0.67,0.9);
  lgnumudisallowedvalor3sig->SetFillStyle(0);
  lgnumudisallowedvalor3sig->SetBorderSize(0);
  lgnumudisallowedvalor3sig->SetTextFont(132);

  hfnumudisallowedvalor3sig->Draw();
  valor_modern_sbn_3p1_contour_3sig_stat_allowed    -> Draw("l");
  valor_modern_sbn_3p1_contour_3sig_syst_allowed    -> Draw("l");
  injected->Draw("P");

  lgnumudisallowedvalor3sig->AddEntry(valor_modern_sbn_3p1_contour_3sig_stat_allowed,  "VALOR 3#sigma CL (stat-only)",  "L");
  lgnumudisallowedvalor3sig->AddEntry(valor_modern_sbn_3p1_contour_3sig_syst_allowed,  "VALOR 3#sigma CL ",             "L");
  lgnumudisallowedvalor3sig->AddEntry(injected, "Injected point - sin^{2}2#theta_{#mu#mu} = 0.07, #Delta m^{2}_{41} = 1.32", "P");
  lgnumudisallowedvalor3sig->Draw();
  
  // 
  // VALOR 5 Sigma
  //
  
  TCanvas * cnumudisallowedvalor5sig = new TCanvas("cnumudisallowedvalor5sig","",20,20,700,900);
  cnumudisallowedvalor5sig->Draw();

  TH2F * hfnumudisallowedvalor5sig = (TH2F*) cnumudisallowedvalor5sig->DrawFrame(3E-2,7E-1,2E-1,3.);
  hfnumudisallowedvalor5sig->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisallowedvalor5sig->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisallowedvalor5sig->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisallowedvalor5sig->GetXaxis()->SetTitleFont(132);
  hfnumudisallowedvalor5sig->GetYaxis()->SetTitleFont(132);
  hfnumudisallowedvalor5sig->GetXaxis()->SetLabelFont(132);
  hfnumudisallowedvalor5sig->GetYaxis()->SetLabelFont(132);
  cnumudisallowedvalor5sig->SetLogx();
  cnumudisallowedvalor5sig->SetLogy();

  TLegend * lgnumudisallowedvalor5sig = new TLegend(0.12,0.62,0.67,0.9);
  lgnumudisallowedvalor5sig->SetFillStyle(0);
  lgnumudisallowedvalor5sig->SetBorderSize(0);
  lgnumudisallowedvalor5sig->SetTextFont(132);

  hfnumudisallowedvalor5sig->Draw();
  valor_modern_sbn_3p1_contour_5sig_stat_allowed    -> Draw("l");
  valor_modern_sbn_3p1_contour_5sig_syst_allowed    -> Draw("l");
  injected->Draw("P");

  lgnumudisallowedvalor5sig->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_allowed,  "VALOR 5#sigma CL (stat-only)",  "L");
  lgnumudisallowedvalor5sig->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_allowed,  "VALOR 5#sigma CL ",             "L");
  lgnumudisallowedvalor5sig->AddEntry(injected, "Injected point - sin^{2}2#theta_{#mu#mu} = 0.07, #Delta m^{2}_{41} = 1.32", "P");
  lgnumudisallowedvalor5sig->Draw();

  // 
  // CAFAna 3 Sigma
  //
  
  TCanvas * cnumudisallowedcafana3sig = new TCanvas("cnumudisallowedcafana3sig","",20,20,700,900);
  cnumudisallowedcafana3sig->Draw();

  TH2F * hfnumudisallowedcafana3sig = (TH2F*) cnumudisallowedcafana3sig->DrawFrame(3E-2,7E-1,2E-1,3.);
  hfnumudisallowedcafana3sig->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisallowedcafana3sig->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisallowedcafana3sig->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisallowedcafana3sig->GetXaxis()->SetTitleFont(132);
  hfnumudisallowedcafana3sig->GetYaxis()->SetTitleFont(132);
  hfnumudisallowedcafana3sig->GetXaxis()->SetLabelFont(132);
  hfnumudisallowedcafana3sig->GetYaxis()->SetLabelFont(132);
  cnumudisallowedcafana3sig->SetLogx();
  cnumudisallowedcafana3sig->SetLogy();

  TLegend * lgnumudisallowedcafana3sig = new TLegend(0.12,0.62,0.67,0.9);
  lgnumudisallowedcafana3sig->SetFillStyle(0);
  lgnumudisallowedcafana3sig->SetBorderSize(0);
  lgnumudisallowedcafana3sig->SetTextFont(132);

  hfnumudisallowedcafana3sig->Draw();
  cafana_modern_sbn_3p1_contour_3sig_stat_allowed    -> Draw("l");
  cafana_modern_sbn_3p1_contour_3sig_syst_allowed    -> Draw("l");
  injected->Draw("P");

  lgnumudisallowedcafana3sig->AddEntry(cafana_modern_sbn_3p1_contour_3sig_stat_allowed,  "CAFAna 3#sigma CL (stat-only)",  "L");
  lgnumudisallowedcafana3sig->AddEntry(cafana_modern_sbn_3p1_contour_3sig_syst_allowed,  "CAFAna 3#sigma CL ",             "L");
  lgnumudisallowedcafana3sig->AddEntry(injected, "Injected point - sin^{2}2#theta_{#mu#mu} = 0.07, #Delta m^{2}_{41} = 1.32", "P");
  lgnumudisallowedcafana3sig->Draw();

  // 
  // CAFAna 5 Sigma
  //
  
  TCanvas * cnumudisallowedcafana5sig = new TCanvas("cnumudisallowedcafana5sig","",20,20,700,900);
  cnumudisallowedcafana5sig->Draw();

  TH2F * hfnumudisallowedcafana5sig = (TH2F*) cnumudisallowedcafana5sig->DrawFrame(3E-2,7E-1,2E-1,3.);
  hfnumudisallowedcafana5sig->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisallowedcafana5sig->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisallowedcafana5sig->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisallowedcafana5sig->GetXaxis()->SetTitleFont(132);
  hfnumudisallowedcafana5sig->GetYaxis()->SetTitleFont(132);
  hfnumudisallowedcafana5sig->GetXaxis()->SetLabelFont(132);
  hfnumudisallowedcafana5sig->GetYaxis()->SetLabelFont(132);
  cnumudisallowedcafana5sig->SetLogx();
  cnumudisallowedcafana5sig->SetLogy();

  TLegend * lgnumudisallowedcafana5sig = new TLegend(0.12,0.62,0.67,0.9);
  lgnumudisallowedcafana5sig->SetFillStyle(0);
  lgnumudisallowedcafana5sig->SetBorderSize(0);
  lgnumudisallowedcafana5sig->SetTextFont(132);

  hfnumudisallowedcafana5sig->Draw();
  cafana_modern_sbn_3p1_contour_5sig_stat_allowed    -> Draw("l");
  cafana_modern_sbn_3p1_contour_5sig_syst_allowed    -> Draw("l");
  injected->Draw("P");

  lgnumudisallowedcafana5sig->AddEntry(cafana_modern_sbn_3p1_contour_5sig_stat_allowed,  "CAFAna 5#sigma CL (stat-only)",  "L");
  lgnumudisallowedcafana5sig->AddEntry(cafana_modern_sbn_3p1_contour_5sig_syst_allowed,  "CAFAna 5#sigma CL ",             "L");
  lgnumudisallowedcafana5sig->AddEntry(injected, "Injected point - sin^{2}2#theta_{#mu#mu} = 0.07, #Delta m^{2}_{41} = 1.32", "P");
  lgnumudisallowedcafana5sig->Draw();
  
  // 
  // SBNFit 3 Sigma
  //
  
  TCanvas * cnumudisallowedsbnfit3sig = new TCanvas("cnumudisallowedsbnfit3sig","",20,20,700,900);
  cnumudisallowedsbnfit3sig->Draw();

  TH2F * hfnumudisallowedsbnfit3sig = (TH2F*) cnumudisallowedsbnfit3sig->DrawFrame(3E-2,7E-1,2E-1,3.);
  hfnumudisallowedsbnfit3sig->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisallowedsbnfit3sig->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisallowedsbnfit3sig->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisallowedsbnfit3sig->GetXaxis()->SetTitleFont(132);
  hfnumudisallowedsbnfit3sig->GetYaxis()->SetTitleFont(132);
  hfnumudisallowedsbnfit3sig->GetXaxis()->SetLabelFont(132);
  hfnumudisallowedsbnfit3sig->GetYaxis()->SetLabelFont(132);
  cnumudisallowedsbnfit3sig->SetLogx();
  cnumudisallowedsbnfit3sig->SetLogy();

  TLegend * lgnumudisallowedsbnfit3sig = new TLegend(0.12,0.62,0.67,0.9);
  lgnumudisallowedsbnfit3sig->SetFillStyle(0);
  lgnumudisallowedsbnfit3sig->SetBorderSize(0);
  lgnumudisallowedsbnfit3sig->SetTextFont(132);

  hfnumudisallowedsbnfit3sig->Draw();
  sbnfit_modern_sbn_3p1_contour_3sig_stat_allowed    -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_3sig_syst_allowed    -> Draw("l");
  injected->Draw("P");

  lgnumudisallowedsbnfit3sig->AddEntry(sbnfit_modern_sbn_3p1_contour_3sig_stat_allowed,  "SBNFit 3#sigma CL (stat-only)",  "L");
  lgnumudisallowedsbnfit3sig->AddEntry(sbnfit_modern_sbn_3p1_contour_3sig_syst_allowed,  "SBNFit 3#sigma CL ",             "L");
  lgnumudisallowedsbnfit3sig->AddEntry(injected, "Injected point - sin^{2}2#theta_{#mu#mu} = 0.07, #Delta m^{2}_{41} = 1.32", "P");
  lgnumudisallowedsbnfit3sig->Draw();

  // 
  // SBNFit 5 Sigma
  //
  
  TCanvas * cnumudisallowedsbnfit5sig = new TCanvas("cnumudisallowedsbnfit5sig","",20,20,700,900);
  cnumudisallowedsbnfit5sig->Draw();

  TH2F * hfnumudisallowedsbnfit5sig = (TH2F*) cnumudisallowedsbnfit5sig->DrawFrame(3E-2,7E-1,2E-1,3.);
  hfnumudisallowedsbnfit5sig->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisallowedsbnfit5sig->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisallowedsbnfit5sig->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisallowedsbnfit5sig->GetXaxis()->SetTitleFont(132);
  hfnumudisallowedsbnfit5sig->GetYaxis()->SetTitleFont(132);
  hfnumudisallowedsbnfit5sig->GetXaxis()->SetLabelFont(132);
  hfnumudisallowedsbnfit5sig->GetYaxis()->SetLabelFont(132);
  cnumudisallowedsbnfit5sig->SetLogx();
  cnumudisallowedsbnfit5sig->SetLogy();

  TLegend * lgnumudisallowedsbnfit5sig = new TLegend(0.12,0.62,0.67,0.9);
  lgnumudisallowedsbnfit5sig->SetFillStyle(0);
  lgnumudisallowedsbnfit5sig->SetBorderSize(0);
  lgnumudisallowedsbnfit5sig->SetTextFont(132);

  hfnumudisallowedsbnfit5sig->Draw();
  sbnfit_modern_sbn_3p1_contour_5sig_stat_allowed    -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_5sig_syst_allowed    -> Draw("l");
  injected->Draw("P");

  lgnumudisallowedsbnfit5sig->AddEntry(sbnfit_modern_sbn_3p1_contour_5sig_stat_allowed,  "SBNFit 5#sigma CL (stat-only)",  "L");
  lgnumudisallowedsbnfit5sig->AddEntry(sbnfit_modern_sbn_3p1_contour_5sig_syst_allowed,  "SBNFit 5#sigma CL ",             "L");
  lgnumudisallowedsbnfit5sig->AddEntry(injected, "Injected point - sin^{2}2#theta_{#mu#mu} = 0.07, #Delta m^{2}_{41} = 1.32", "P");
  lgnumudisallowedsbnfit5sig->Draw();
  
  //
  // SBNFit CAFAna VALOR 3 sig overlay
  //

  TCanvas * cnumudisallowed3sigoverlay = new TCanvas("cnumudisallowed3sigoverlay","",20,20,700,900);
  cnumudisallowed3sigoverlay->Draw();

  TH2F * hfnumudisallowed3sigoverlay = (TH2F*) cnumudisallowed3sigoverlay->DrawFrame(3E-2,7E-1,2E-1,3.);
  hfnumudisallowed3sigoverlay->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisallowed3sigoverlay->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisallowed3sigoverlay->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisallowed3sigoverlay->GetXaxis()->SetTitleFont(132);
  hfnumudisallowed3sigoverlay->GetYaxis()->SetTitleFont(132);
  hfnumudisallowed3sigoverlay->GetXaxis()->SetLabelFont(132);
  hfnumudisallowed3sigoverlay->GetYaxis()->SetLabelFont(132);
  cnumudisallowed3sigoverlay->SetLogx();
  cnumudisallowed3sigoverlay->SetLogy();

  TLegend * lgnumudisallowed3sigoverlay = new TLegend(0.12,0.62,0.67,0.9);
  lgnumudisallowed3sigoverlay->SetFillStyle(0);
  lgnumudisallowed3sigoverlay->SetBorderSize(0);
  lgnumudisallowed3sigoverlay->SetTextFont(132);

  hfnumudisallowed3sigoverlay->Draw();
  cafana_modern_sbn_3p1_contour_3sig_stat_allowed    -> Draw("l");
  cafana_modern_sbn_3p1_contour_3sig_syst_allowed    -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_3sig_stat_allowed    -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_3sig_syst_allowed    -> Draw("l");
  valor_modern_sbn_3p1_contour_3sig_stat_allowed    -> Draw("l");
  valor_modern_sbn_3p1_contour_3sig_syst_allowed    -> Draw("l");
  injected->Draw("P");

  lgnumudisallowed3sigoverlay->AddEntry(cafana_modern_sbn_3p1_contour_3sig_stat_allowed,  "CAFAna 3#sigma CL (stat-only)",  "L");
  lgnumudisallowed3sigoverlay->AddEntry(cafana_modern_sbn_3p1_contour_3sig_syst_allowed,  "CAFAna 3#sigma CL ",             "L");
  lgnumudisallowed3sigoverlay->AddEntry(sbnfit_modern_sbn_3p1_contour_3sig_stat_allowed,  "SBNFit 3#sigma CL (stat-only)",  "L");
  lgnumudisallowed3sigoverlay->AddEntry(sbnfit_modern_sbn_3p1_contour_3sig_syst_allowed,  "SBNFit 3#sigma CL ",             "L");
  lgnumudisallowed3sigoverlay->AddEntry(valor_modern_sbn_3p1_contour_3sig_stat_allowed,   "VALOR 3#sigma CL (stat-only)",  "L");
  lgnumudisallowed3sigoverlay->AddEntry(valor_modern_sbn_3p1_contour_3sig_syst_allowed,   "VALOR 3#sigma CL ",             "L");
  lgnumudisallowed3sigoverlay->AddEntry(injected, "Injected point - sin^{2}2#theta_{#mu#mu} = 0.07, #Delta m^{2}_{41} = 1.32", "P");
  lgnumudisallowed3sigoverlay->Draw();

  //
  // SBNFit CAFAna VALOR 3 sig stat+syst only overlay
  //

  TCanvas * cnumudisallowed3sigsystoverlay = new TCanvas("cnumudisallowed3sigsystoverlay","",20,20,700,900);
  cnumudisallowed3sigsystoverlay->Draw();

  TH2F * hfnumudisallowed3sigsystoverlay = (TH2F*) cnumudisallowed3sigsystoverlay->DrawFrame(3E-2,7E-1,2E-1,3.);
  hfnumudisallowed3sigsystoverlay->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisallowed3sigsystoverlay->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisallowed3sigsystoverlay->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisallowed3sigsystoverlay->GetXaxis()->SetTitleFont(132);
  hfnumudisallowed3sigsystoverlay->GetYaxis()->SetTitleFont(132);
  hfnumudisallowed3sigsystoverlay->GetXaxis()->SetLabelFont(132);
  hfnumudisallowed3sigsystoverlay->GetYaxis()->SetLabelFont(132);
  cnumudisallowed3sigsystoverlay->SetLogx();
  cnumudisallowed3sigsystoverlay->SetLogy();

  TLegend * lgnumudisallowed3sigsystoverlay = new TLegend(0.12,0.62,0.67,0.9);
  lgnumudisallowed3sigsystoverlay->SetFillStyle(0);
  lgnumudisallowed3sigsystoverlay->SetBorderSize(0);
  lgnumudisallowed3sigsystoverlay->SetTextFont(132);

  hfnumudisallowed3sigsystoverlay->Draw();
  cafana_modern_sbn_3p1_contour_3sig_syst_allowed    -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_3sig_syst_allowed    -> Draw("l");
  valor_modern_sbn_3p1_contour_3sig_syst_allowed    -> Draw("l");
  injected->Draw("P");

  lgnumudisallowed3sigsystoverlay->AddEntry(cafana_modern_sbn_3p1_contour_3sig_syst_allowed,  "CAFAna 3#sigma CL ",             "L");
  lgnumudisallowed3sigsystoverlay->AddEntry(sbnfit_modern_sbn_3p1_contour_3sig_syst_allowed,  "SBNFit 3#sigma CL ",             "L");
  lgnumudisallowed3sigsystoverlay->AddEntry(valor_modern_sbn_3p1_contour_3sig_syst_allowed,   "VALOR 3#sigma CL ",             "L");
  lgnumudisallowed3sigsystoverlay->AddEntry(injected, "Injected point - sin^{2}2#theta_{#mu#mu} = 0.07, #Delta m^{2}_{41} = 1.32", "P");
  lgnumudisallowed3sigsystoverlay->Draw();

  //
  // SBNFit CAFAna VALOR 5 sig overlay
  //

  TCanvas * cnumudisallowed5sigoverlay = new TCanvas("cnumudisallowed5sigoverlay","",20,20,700,900);
  cnumudisallowed5sigoverlay->Draw();

  TH2F * hfnumudisallowed5sigoverlay = (TH2F*) cnumudisallowed5sigoverlay->DrawFrame(3E-2,7E-1,2E-1,3.);
  hfnumudisallowed5sigoverlay->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisallowed5sigoverlay->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisallowed5sigoverlay->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisallowed5sigoverlay->GetXaxis()->SetTitleFont(132);
  hfnumudisallowed5sigoverlay->GetYaxis()->SetTitleFont(132);
  hfnumudisallowed5sigoverlay->GetXaxis()->SetLabelFont(132);
  hfnumudisallowed5sigoverlay->GetYaxis()->SetLabelFont(132);
  cnumudisallowed5sigoverlay->SetLogx();
  cnumudisallowed5sigoverlay->SetLogy();

  TLegend * lgnumudisallowed5sigoverlay = new TLegend(0.12,0.62,0.67,0.9);
  lgnumudisallowed5sigoverlay->SetFillStyle(0);
  lgnumudisallowed5sigoverlay->SetBorderSize(0);
  lgnumudisallowed5sigoverlay->SetTextFont(132);

  hfnumudisallowed5sigoverlay->Draw();
  cafana_modern_sbn_3p1_contour_5sig_stat_allowed    -> Draw("l");
  cafana_modern_sbn_3p1_contour_5sig_syst_allowed    -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_5sig_stat_allowed    -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_5sig_syst_allowed    -> Draw("l");
  valor_modern_sbn_3p1_contour_5sig_stat_allowed    -> Draw("l");
  valor_modern_sbn_3p1_contour_5sig_syst_allowed    -> Draw("l");
  injected->Draw("P");

  lgnumudisallowed5sigoverlay->AddEntry(cafana_modern_sbn_3p1_contour_5sig_stat_allowed,  "CAFAna 5#sigma CL (stat-only)",  "L");
  lgnumudisallowed5sigoverlay->AddEntry(cafana_modern_sbn_3p1_contour_5sig_syst_allowed,  "CAFAna 5#sigma CL ",             "L");
  lgnumudisallowed5sigoverlay->AddEntry(sbnfit_modern_sbn_3p1_contour_5sig_stat_allowed,  "SBNFit 5#sigma CL (stat-only)",  "L");
  lgnumudisallowed5sigoverlay->AddEntry(sbnfit_modern_sbn_3p1_contour_5sig_syst_allowed,  "SBNFit 5#sigma CL ",             "L");
  lgnumudisallowed5sigoverlay->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_allowed,   "VALOR 5#sigma CL (stat-only)",  "L");
  lgnumudisallowed5sigoverlay->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_allowed,   "VALOR 5#sigma CL ",             "L");
  lgnumudisallowed5sigoverlay->AddEntry(injected, "Injected point - sin^{2}2#theta_{#mu#mu} = 0.07, #Delta m^{2}_{41} = 1.32", "P");
  lgnumudisallowed5sigoverlay->Draw();
  
  //
  // SBNFit CAFAna VALOR 5 sig overlay stat+syst only
  //

  TCanvas * cnumudisallowed5sigsystoverlay = new TCanvas("cnumudisallowed5sigsystoverlay","",20,20,700,900);
  cnumudisallowed5sigsystoverlay->Draw();

  TH2F * hfnumudisallowed5sigsystoverlay = (TH2F*) cnumudisallowed5sigsystoverlay->DrawFrame(3E-2,7E-1,2E-1,3.);
  hfnumudisallowed5sigsystoverlay->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisallowed5sigsystoverlay->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisallowed5sigsystoverlay->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisallowed5sigsystoverlay->GetXaxis()->SetTitleFont(132);
  hfnumudisallowed5sigsystoverlay->GetYaxis()->SetTitleFont(132);
  hfnumudisallowed5sigsystoverlay->GetXaxis()->SetLabelFont(132);
  hfnumudisallowed5sigsystoverlay->GetYaxis()->SetLabelFont(132);
  cnumudisallowed5sigsystoverlay->SetLogx();
  cnumudisallowed5sigsystoverlay->SetLogy();

  TLegend * lgnumudisallowed5sigsystoverlay = new TLegend(0.12,0.62,0.67,0.9);
  lgnumudisallowed5sigsystoverlay->SetFillStyle(0);
  lgnumudisallowed5sigsystoverlay->SetBorderSize(0);
  lgnumudisallowed5sigsystoverlay->SetTextFont(132);

  hfnumudisallowed5sigsystoverlay->Draw();
  cafana_modern_sbn_3p1_contour_5sig_syst_allowed    -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_5sig_syst_allowed    -> Draw("l");
  valor_modern_sbn_3p1_contour_5sig_syst_allowed    -> Draw("l");
  injected->Draw("P");

  lgnumudisallowed5sigsystoverlay->AddEntry(cafana_modern_sbn_3p1_contour_5sig_syst_allowed,  "CAFAna 5#sigma CL ",             "L");
  lgnumudisallowed5sigsystoverlay->AddEntry(sbnfit_modern_sbn_3p1_contour_5sig_syst_allowed,  "SBNFit 5#sigma CL ",             "L");
  lgnumudisallowed5sigsystoverlay->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_allowed,   "VALOR 5#sigma CL ",             "L");
  lgnumudisallowed5sigsystoverlay->AddEntry(injected, "Injected point - sin^{2}2#theta_{#mu#mu} = 0.07, #Delta m^{2}_{41} = 1.32", "P");
  lgnumudisallowed5sigsystoverlay->Draw();
}

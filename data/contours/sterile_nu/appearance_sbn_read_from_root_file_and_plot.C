//
// This macro reads in the ROOT file produced by convert_to_root_file_and_plot.C
// and plots the appearance and disappearance limits.
//
// C.A.
//

{

  TFile * f              = new TFile("./old_sterile_3p1_limits.root","read");
  TFile *f_syst_03d1     = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/tn_2022/03d1/output_contours_as_graphs_nue_app_03d1.root"); 
  TFile *f_syst_03d1_all = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/tn_2022/03d1/output_contours_as_graphs_nue_app_03d1_dmsq_1.32_sinsq2th_0.003.root");
  TFile *f_stat_all      = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/tn_2022/03d1/output_contours_as_graphs_nue_app_stat_dmsq_1.32_sinsq2th_0.003.root");

  std::string location = "/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/tn_2022/output/";
  
  // 
  // Global style settings
  //
  gStyle->SetTitleSize(0.06,"X");
  gStyle->SetTitleSize(0.06,"Y");
  gStyle->SetLabelSize(0.05,"X");
  gStyle->SetLabelSize(0.05,"Y");

  //
  // nue apearance
  //
  TGraph * lim_nueapp_3p1_sbnproposal_3sigCL   = (TGraph *) f->Get( "lim_nueapp_3p1_sbnproposal_3sigCL"   );
  TGraph * lim_nueapp_3p1_nomad_99pctCL        = (TGraph *) f->Get( "lim_nueapp_3p1_nomad_99pctCL"        );
  TGraph * lim_nueapp_3p1_bnl776_99pctCL       = (TGraph *) f->Get( "lim_nueapp_3p1_bnl776_99pctCL"       );
  TGraph * lim_nueapp_3p1_karmen_99pctCL       = (TGraph *) f->Get( "lim_nueapp_3p1_karmen_99pctCL"       );
  TGraph * lim_nueapp_3p1_icarus_99pctCL       = (TGraph *) f->Get( "lim_nueapp_3p1_icarus_99pctCL"       );
  TGraph * lim_nueapp_3p1_opera_99pctCL        = (TGraph *) f->Get( "lim_nueapp_3p1_opera_99pctCL"        );
  TGraph * lim_nueapp_3p1_miniboonenu_99pctCL  = (TGraph *) f->Get( "lim_nueapp_3p1_miniboonenu_99pctCL"  );
  TGraph * lim_nueapp_3p1_lsndr1_99pctCL       = (TGraph *) f->Get( "lim_nueapp_3p1_lsndr1_99pctCL"       );
  TGraph * lim_nueapp_3p1_lsndr2_99pctCL       = (TGraph *) f->Get( "lim_nueapp_3p1_lsndr2_99pctCL"       );
  TGraph * lim_nueapp_3p1_lsndr3_99pctCL       = (TGraph *) f->Get( "lim_nueapp_3p1_lsndr3_99pctCL"       );
  
  TGraph * valor_modern_sbn_3p1_contour_90pct_syst_03d1         = dynamic_cast<TGraph *>( f_syst_03d1->Get("contour_90pct_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_99pct_syst_03d1         = dynamic_cast<TGraph *>( f_syst_03d1->Get("contour_99pct_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03d1          = dynamic_cast<TGraph *>( f_syst_03d1->Get("contour_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_90pct_syst_03d1_all     = dynamic_cast<TGraph *>( f_syst_03d1_all->Get("contour_90pct_lin_5"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03d1_all      = dynamic_cast<TGraph *>( f_syst_03d1_all->Get("contour_5sigma_lin_5"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03d1_allW     = dynamic_cast<TGraph *>( f_syst_03d1_all->Get("contour_5sigma_lin_5"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_all           = dynamic_cast<TGraph *>( f_stat_all->Get("contour_5sigma_lin_1"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_allW          = dynamic_cast<TGraph *>( f_stat_all->Get("contour_5sigma_lin_1"));
  
  // Allowed
  const std::vector<double> x = {0.003};
  const std::vector<double> y = {1.32};
  TGraph *injectedBF  = new TGraph(1,&x.data()[0],&y.data()[0]);
  TGraph *injectedW   = new TGraph(1,&x.data()[0],&y.data()[0]);

  lim_nueapp_3p1_sbnproposal_3sigCL   -> SetLineColor(kGray+2); 
  lim_nueapp_3p1_nomad_99pctCL        -> SetLineColor(kGray+2); 
  lim_nueapp_3p1_bnl776_99pctCL       -> SetLineColor(kGray+2); 
  lim_nueapp_3p1_karmen_99pctCL       -> SetLineColor(kGray+2); 
  lim_nueapp_3p1_icarus_99pctCL       -> SetLineColor(kGray+2); 
  lim_nueapp_3p1_opera_99pctCL        -> SetLineColor(kGray+2); 
  lim_nueapp_3p1_miniboonenu_99pctCL  -> SetLineColor(kGray+2); 

  lim_nueapp_3p1_lsndr1_99pctCL       -> SetLineColor(kGray+2); 
  lim_nueapp_3p1_lsndr2_99pctCL       -> SetLineColor(kGray+2); 
  lim_nueapp_3p1_lsndr3_99pctCL       -> SetLineColor(kGray+2); 
  lim_nueapp_3p1_lsndr1_99pctCL       -> SetFillStyle(3115); 
  lim_nueapp_3p1_lsndr2_99pctCL       -> SetFillStyle(3115); 
  lim_nueapp_3p1_lsndr3_99pctCL       -> SetFillStyle(3115); 
  lim_nueapp_3p1_lsndr1_99pctCL       -> SetFillColor(kGray+2); 
  lim_nueapp_3p1_lsndr2_99pctCL       -> SetFillColor(kGray+2); 
  lim_nueapp_3p1_lsndr3_99pctCL       -> SetFillColor(kGray+2); 

  lim_nueapp_3p1_sbnproposal_3sigCL   -> SetLineWidth(3);
  lim_nueapp_3p1_nomad_99pctCL        -> SetLineWidth(3);
  lim_nueapp_3p1_bnl776_99pctCL       -> SetLineWidth(3);
  lim_nueapp_3p1_karmen_99pctCL       -> SetLineWidth(3);
  lim_nueapp_3p1_icarus_99pctCL       -> SetLineWidth(3);
  lim_nueapp_3p1_opera_99pctCL        -> SetLineWidth(3);
  lim_nueapp_3p1_miniboonenu_99pctCL  -> SetLineWidth(3);
  lim_nueapp_3p1_lsndr1_99pctCL       -> SetLineWidth(3);
  lim_nueapp_3p1_lsndr2_99pctCL       -> SetLineWidth(3);
  lim_nueapp_3p1_lsndr3_99pctCL       -> SetLineWidth(3);
  
  lim_nueapp_3p1_sbnproposal_3sigCL   -> SetLineStyle(1);
  lim_nueapp_3p1_miniboonenu_99pctCL  -> SetLineStyle(2);
  lim_nueapp_3p1_lsndr1_99pctCL       -> SetLineStyle(3);
  lim_nueapp_3p1_lsndr2_99pctCL       -> SetLineStyle(3);
  lim_nueapp_3p1_lsndr3_99pctCL       -> SetLineStyle(3);
  lim_nueapp_3p1_bnl776_99pctCL       -> SetLineStyle(4);
  lim_nueapp_3p1_icarus_99pctCL       -> SetLineStyle(5);
  lim_nueapp_3p1_opera_99pctCL        -> SetLineStyle(6);
  lim_nueapp_3p1_karmen_99pctCL       -> SetLineStyle(7);
  lim_nueapp_3p1_nomad_99pctCL        -> SetLineStyle(8);
  
  lim_nueapp_3p1_sbnproposal_3sigCL   -> SetMarkerStyle(0);
  lim_nueapp_3p1_nomad_99pctCL        -> SetMarkerStyle(0);
  lim_nueapp_3p1_bnl776_99pctCL       -> SetMarkerStyle(0);
  lim_nueapp_3p1_karmen_99pctCL       -> SetMarkerStyle(0);
  lim_nueapp_3p1_icarus_99pctCL       -> SetMarkerStyle(0);
  lim_nueapp_3p1_opera_99pctCL        -> SetMarkerStyle(0);
  lim_nueapp_3p1_miniboonenu_99pctCL  -> SetMarkerStyle(0);
  lim_nueapp_3p1_lsndr1_99pctCL       -> SetMarkerStyle(0);
  lim_nueapp_3p1_lsndr2_99pctCL       -> SetMarkerStyle(0);
  lim_nueapp_3p1_lsndr3_99pctCL       -> SetMarkerStyle(0);
 
  // SBN contours 

  valor_modern_sbn_3p1_contour_90pct_syst_03d1  -> SetLineColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_90pct_syst_03d1  -> SetLineStyle(9);
  valor_modern_sbn_3p1_contour_90pct_syst_03d1  -> SetLineWidth(4);

  valor_modern_sbn_3p1_contour_99pct_syst_03d1  -> SetLineColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_99pct_syst_03d1  -> SetLineStyle(7);
  valor_modern_sbn_3p1_contour_99pct_syst_03d1  -> SetLineWidth(4);

  valor_modern_sbn_3p1_contour_5sig_syst_03d1  -> SetLineColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_03d1  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03d1  -> SetLineWidth(4);

  valor_modern_sbn_3p1_contour_90pct_syst_03d1_all -> SetLineColor(kOrange+4); 
  valor_modern_sbn_3p1_contour_90pct_syst_03d1_all -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_90pct_syst_03d1_all -> SetLineWidth(4);
  valor_modern_sbn_3p1_contour_90pct_syst_03d1_all -> SetFillColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_90pct_syst_03d1_all -> SetFillStyle(3002);

  valor_modern_sbn_3p1_contour_5sig_syst_03d1_all -> SetLineColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_all -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_all -> SetLineWidth(4);
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_all -> SetFillColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_all -> SetFillStyle(3001);
  
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_allW -> SetFillColor(kWhite);  
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_allW -> SetFillStyle(1001);
  
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetLineColor(kOrange+5);  
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetLineWidth(4);
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetFillColor(kOrange+5);  
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetFillStyle(3001);
  
  valor_modern_sbn_3p1_contour_5sig_stat_allW -> SetFillColor(kWhite);  
  valor_modern_sbn_3p1_contour_5sig_stat_allW -> SetFillStyle(1001);
  
  // Allowed
  injectedW->SetMarkerColor(kWhite);

  injectedBF->SetMarkerStyle(43);
  injectedBF->SetMarkerSize(2);
  injectedBF->SetMarkerColor(kGray+3);

  // Drawing
  TCanvas * c1 = new TCanvas("c1","",20,20,1000,900);
  c1->SetTopMargin(0.024);
  c1->SetBottomMargin(0.13);
  c1->SetLeftMargin(0.13);
  c1->SetRightMargin(0.024);

  TH1F * hf2 = dynamic_cast<TH1F*>(c1->DrawFrame(1E-4,1E-2,1,1e2));
  hf2->Draw();
  hf2->GetYaxis()->SetTitleOffset(1);
  hf2->GetXaxis()->SetTitle("sin^{2}2#theta_{#mue}");
  hf2->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hf2->GetXaxis()->SetTitleFont(132);
  hf2->GetYaxis()->SetTitleFont(132);
  hf2->GetXaxis()->SetLabelFont(132);
  hf2->GetYaxis()->SetLabelFont(132);
  hf2->GetXaxis()->SetTitleSize(0.06);
  hf2->GetYaxis()->SetTitleSize(0.06);
  hf2->GetXaxis()->SetLabelSize(0.05);
  hf2->GetYaxis()->SetLabelSize(0.05);
  
  c1->SetLogx();
  c1->SetLogy();

  lim_nueapp_3p1_lsndr1_99pctCL       -> Draw("lf");
  lim_nueapp_3p1_lsndr2_99pctCL       -> Draw("lf");
  lim_nueapp_3p1_lsndr3_99pctCL       -> Draw("lf");
  lim_nueapp_3p1_karmen_99pctCL       -> Draw("l");
  lim_nueapp_3p1_miniboonenu_99pctCL  -> Draw("l");
  /*
  lim_nueapp_3p1_nomad_99pctCL        -> Draw("l");
  lim_nueapp_3p1_bnl776_99pctCL       -> Draw("l");
  lim_nueapp_3p1_icarus_99pctCL       -> Draw("l");
  lim_nueapp_3p1_opera_99pctCL        -> Draw("l");
  */

  valor_modern_sbn_3p1_contour_5sig_syst_03d1      -> Draw("L");
  valor_modern_sbn_3p1_contour_99pct_syst_03d1     -> Draw("L");
  //valor_modern_sbn_3p1_contour_90pct_syst_03d1     -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_allW      -> Draw("F");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_allW -> Draw("F");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_all  -> Draw("FL");
  valor_modern_sbn_3p1_contour_5sig_stat_all       -> Draw("FL");
  
  injectedBF                                      -> Draw("P");
  
  TLegend * lg1 = new TLegend(0.16,0.16,0.5,0.46);
  lg1->SetFillStyle(0);
  lg1->SetBorderSize(0);
  lg1->SetMargin(0.22);
  lg1->SetTextFont(132);
  lg1->SetTextSize(0.028);

  TLegend * lg1b = new TLegend(0.68,0.75,0.98,0.90);
  lg1b->SetFillStyle(0);
  lg1b->SetBorderSize(0);
  lg1b->SetMargin(0.18);
  lg1b->SetTextFont(132);
  lg1b->SetTextSize(0.028);
  
  //  lg1->AddEntry(lim_nueapp_3p1_sbnproposal_3sigCL,  "SBN proposal 3#sigma CL", "L");
  /*
  lg1->AddEntry(lim_nueapp_3p1_nomad_99pctCL,                    "NOMAD 99\% CL",           "L");
  lg1->AddEntry(lim_nueapp_3p1_bnl776_99pctCL,                   "BNL776 99\% CL",          "L");
  lg1->AddEntry(lim_nueapp_3p1_icarus_99pctCL,                   "ICARUS 99\% CL",          "L");
  lg1->AddEntry(lim_nueapp_3p1_opera_99pctCL,                    "OPERA 99\% CL",           "L");
  */
  lg1->AddEntry(lim_nueapp_3p1_miniboonenu_99pctCL,              "MiniBooNE (#nu) 99\% CL", "L");
  lg1->AddEntry(lim_nueapp_3p1_karmen_99pctCL,                   "KARMEN 99\% CL",          "L");
  lg1->AddEntry(lim_nueapp_3p1_lsndr1_99pctCL,                   "LSND w/ DiF 99\% CL",     "F");
  lg1->AddEntry(injectedW,                                       " ", "P");
  //lg1->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_03d1,    "90\% SBN Stat+Syst", "L");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_99pct_syst_03d1,    "99\% SBN Stat+Syst", "L");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1,     "5#sigma SBN Stat+Syst", "L");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1_all, "5#sigma SBN Stat+Syst", "F");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,      "5#sigma SBN Stat-Only", "F");
  lg1b->AddEntry(injectedW,                                      "Injected Point,", "P");
  lg1b->AddEntry(injectedBF,                                     "sin^{2}2#theta_{#mu e} = 0.003,", "P");
  lg1b->AddEntry(injectedW,                                      "#Deltam_{41}^{2} = 1.32", "P");
  lg1->Draw();
  lg1b->Draw();
  
  c1->SaveAs((location+"nue_app_full_03d1_sensitivity_comparison.C").c_str());
  c1->SaveAs((location+"nue_app_full_03d1_sensitivity_comparison.root").c_str());
  c1->SaveAs((location+"nue_app_full_03d1_sensitivity_comparison.pdf").c_str());
  c1->SaveAs((location+"nue_app_full_03d1_sensitivity_comparison.png").c_str());
}

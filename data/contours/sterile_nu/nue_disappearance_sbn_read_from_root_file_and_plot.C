//
// This macro reads in the ROOT file produced by convert_to_root_file_and_plot.C
// and plots the appearance and disappearance limits.
//
// C.A.
//

{

  TFile * f = new TFile("./old_sterile_3p1_limits.root","read");
  TFile *f_syst_03d1     = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/tn_2022/03d1/output_contours_as_graphs_nue_disapp_03d1.root"); 
  TFile *f_syst_03d1_all = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/tn_2022/03d1/output_contours_as_graphs_nue_disapp_03d1_dmsq_3_sinsq2th_0.4.root");
  TFile *f_stat_all      = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/tn_2022/03d1/output_contours_as_graphs_nue_disapp_stat_dmsq_3_sinsq2th_0.4.root");

  std::string location = "/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/tn_2022/output/";
  

  // 
  // Global style settings
  //
  gStyle->SetTitleSize(0.06,"X");
  gStyle->SetTitleSize(0.06,"Y");
  gStyle->SetLabelSize(0.05,"X");
  gStyle->SetLabelSize(0.05,"Y");

  //
  // nue disappearance
  //

  TGraph * lim_nuedis_3p1_t2knd280excluded_95pctCL  = (TGraph *) f->Get("lim_nuedis_3p1_t2knd280excluded_95pctCL");
  TGraph * lim_nuedis_3p1_t2knd280allowed_90pctCL   = (TGraph *) f->Get("lim_nuedis_3p1_t2knd280allowed_90pctCL");
  TGraph * lim_nuedis_3p1_t2knd280allowed_90pctCLW  = (TGraph *) f->Get("lim_nuedis_3p1_t2knd280allowed_90pctCL");
  TGraph * lim_nuedis_3p1_t2knd280allowed_68pctCL   = (TGraph *) f->Get("lim_nuedis_3p1_t2knd280allowed_68pctCL");
  TGraph * lim_nuedis_3p1_t2knd280allowed_68pctCLW  = (TGraph *) f->Get("lim_nuedis_3p1_t2knd280allowed_68pctCL");
  TGraph * lim_nuedis_3p1_reactor_allowed_1_95pctCL = (TGraph *) f->Get("lim_nueapp_3p1_reactor_allowed_1_95pctCL");
  TGraph * lim_nuedis_3p1_reactor_allowed_2_95pctCL = (TGraph *) f->Get("lim_nueapp_3p1_reactor_allowed_2_95pctCL");
  TGraph * lim_nuedis_3p1_reactor_allowed_3_95pctCL = (TGraph *) f->Get("lim_nueapp_3p1_reactor_allowed_3_95pctCL");
  TGraph * lim_nuedis_3p1_gallium_allowed_95pctCL   = (TGraph *) f->Get("lim_nueapp_3p1_gallium_allowed_95pctCL");
  TGraph * lim_nuedis_3p1_solar_excluded_95pctCL    = (TGraph *) f->Get("lim_nueapp_3p1_solar_excluded_95pctCL");
  TGraph * lim_nuedis_3p1_nueC_excluded_95pctCL     = (TGraph *) f->Get("lim_nueapp_3p1_nueC_excluded_95pctCL");
  
  TGraph * valor_modern_sbn_3p1_contour_90pct_syst_03d1         = dynamic_cast<TGraph *>( f_syst_03d1->Get("contour_90pct_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03d1          = dynamic_cast<TGraph *>( f_syst_03d1->Get("contour_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_90pct_syst_03d1_all     = dynamic_cast<TGraph *>( f_syst_03d1_all->Get("contour_90pct_lin_2"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03d1_all      = dynamic_cast<TGraph *>( f_syst_03d1_all->Get("contour_5sigma_lin_2"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03d1_allW     = dynamic_cast<TGraph *>( f_syst_03d1_all->Get("contour_5sigma_lin_2"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_all           = dynamic_cast<TGraph *>( f_stat_all->Get("contour_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_allW          = dynamic_cast<TGraph *>( f_stat_all->Get("contour_5sigma_lin_0"));

  // Allowed
  const std::vector<double> x = {0.4};
  const std::vector<double> y = {3};
  TGraph *injectedBF  = new TGraph(1,&x.data()[0],&y.data()[0]);
  TGraph *injectedW   = new TGraph(1,&x.data()[0],&y.data()[0]);

  lim_nuedis_3p1_t2knd280excluded_95pctCL -> SetLineColor(kGray+2);  
  lim_nuedis_3p1_t2knd280allowed_90pctCL  -> SetLineColor(kGray+2); 
  lim_nuedis_3p1_t2knd280allowed_68pctCL  -> SetLineColor(kGray+2); 
  lim_nuedis_3p1_reactor_allowed_1_95pctCL-> SetLineColor(kGray+2); 
  lim_nuedis_3p1_reactor_allowed_2_95pctCL-> SetLineColor(kGray+2); 
  lim_nuedis_3p1_reactor_allowed_3_95pctCL-> SetLineColor(kGray+2); 
  lim_nuedis_3p1_gallium_allowed_95pctCL  -> SetLineColor(kGray+2); 
  lim_nuedis_3p1_solar_excluded_95pctCL   -> SetLineColor(kGray+2); 
  lim_nuedis_3p1_nueC_excluded_95pctCL    -> SetLineColor(kGray+2); 

  lim_nuedis_3p1_t2knd280allowed_90pctCL  -> SetFillColor(kGray+2); 
  lim_nuedis_3p1_t2knd280allowed_68pctCL  -> SetFillColor(kGray+2); 
  lim_nuedis_3p1_reactor_allowed_1_95pctCL-> SetFillColor(kGray+2); 
  lim_nuedis_3p1_reactor_allowed_2_95pctCL-> SetFillColor(kGray+2); 
  lim_nuedis_3p1_reactor_allowed_3_95pctCL-> SetFillColor(kGray+2); 
  lim_nuedis_3p1_gallium_allowed_95pctCL  -> SetFillColor(kGray+2);

  lim_nuedis_3p1_t2knd280allowed_90pctCL  -> SetFillStyle(3251); 
  lim_nuedis_3p1_t2knd280allowed_68pctCL  -> SetFillStyle(3115); 
  lim_nuedis_3p1_reactor_allowed_1_95pctCL-> SetFillStyle(3115);
  lim_nuedis_3p1_reactor_allowed_2_95pctCL-> SetFillStyle(3115); 
  lim_nuedis_3p1_reactor_allowed_3_95pctCL-> SetFillStyle(3115); 
  lim_nuedis_3p1_gallium_allowed_95pctCL  -> SetFillStyle(3115);

  lim_nuedis_3p1_t2knd280excluded_95pctCL -> SetLineWidth(3);
  lim_nuedis_3p1_t2knd280allowed_90pctCL  -> SetLineWidth(3);
  lim_nuedis_3p1_t2knd280allowed_68pctCL  -> SetLineWidth(3);
  lim_nuedis_3p1_reactor_allowed_1_95pctCL-> SetLineWidth(3);
  lim_nuedis_3p1_reactor_allowed_2_95pctCL-> SetLineWidth(3);
  lim_nuedis_3p1_reactor_allowed_3_95pctCL-> SetLineWidth(3);
  lim_nuedis_3p1_gallium_allowed_95pctCL  -> SetLineWidth(3);
  lim_nuedis_3p1_solar_excluded_95pctCL   -> SetLineWidth(3);
  lim_nuedis_3p1_nueC_excluded_95pctCL    -> SetLineWidth(3);
  
  lim_nuedis_3p1_t2knd280excluded_95pctCL -> SetLineStyle(2);
  lim_nuedis_3p1_t2knd280allowed_90pctCL  -> SetLineStyle(3);
  lim_nuedis_3p1_t2knd280allowed_68pctCL  -> SetLineStyle(4);
  lim_nuedis_3p1_reactor_allowed_1_95pctCL-> SetLineStyle(5);
  lim_nuedis_3p1_reactor_allowed_2_95pctCL-> SetLineStyle(6);
  lim_nuedis_3p1_reactor_allowed_3_95pctCL-> SetLineStyle(7);
  lim_nuedis_3p1_gallium_allowed_95pctCL  -> SetLineStyle(8);
  lim_nuedis_3p1_solar_excluded_95pctCL   -> SetLineStyle(9);
  lim_nuedis_3p1_nueC_excluded_95pctCL    -> SetLineStyle(10);
  
  lim_nuedis_3p1_t2knd280excluded_95pctCL -> SetMarkerStyle(0);
  lim_nuedis_3p1_t2knd280allowed_90pctCL  -> SetMarkerStyle(0);
  lim_nuedis_3p1_t2knd280allowed_68pctCL  -> SetMarkerStyle(0);
  lim_nuedis_3p1_reactor_allowed_1_95pctCL-> SetMarkerStyle(0);
  lim_nuedis_3p1_reactor_allowed_2_95pctCL-> SetMarkerStyle(0);
  lim_nuedis_3p1_reactor_allowed_3_95pctCL-> SetMarkerStyle(0);
  lim_nuedis_3p1_gallium_allowed_95pctCL  -> SetMarkerStyle(0);
  lim_nuedis_3p1_solar_excluded_95pctCL   -> SetMarkerStyle(0);
  lim_nuedis_3p1_nueC_excluded_95pctCL    -> SetMarkerStyle(0);
  
  // White versions
  lim_nuedis_3p1_t2knd280allowed_68pctCLW  -> SetFillStyle(1001);
  lim_nuedis_3p1_t2knd280allowed_68pctCLW  -> SetFillColor(kWhite);
  lim_nuedis_3p1_t2knd280allowed_90pctCLW  -> SetFillStyle(1001);
  lim_nuedis_3p1_t2knd280allowed_90pctCLW  -> SetFillColor(kWhite);

  // SBN contours 

  valor_modern_sbn_3p1_contour_90pct_syst_03d1  -> SetLineColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_90pct_syst_03d1  -> SetLineStyle(9);
  valor_modern_sbn_3p1_contour_90pct_syst_03d1  -> SetLineWidth(4);

  valor_modern_sbn_3p1_contour_5sig_syst_03d1  -> SetLineColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_03d1  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03d1  -> SetLineWidth(4);

  valor_modern_sbn_3p1_contour_90pct_syst_03d1_all -> SetLineColor(kOrange+4); 
  valor_modern_sbn_3p1_contour_90pct_syst_03d1_all -> SetLineStyle(9);
  valor_modern_sbn_3p1_contour_90pct_syst_03d1_all -> SetLineWidth(4);
  valor_modern_sbn_3p1_contour_90pct_syst_03d1_all -> SetFillColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_90pct_syst_03d1_all -> SetFillStyle(3001);

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

  TH1F * hf2 = dynamic_cast<TH1F*>(c1->DrawFrame(1E-2,1E-2,1,1e2));
  hf2->Draw();
  hf2->GetYaxis()->SetTitleOffset(1);
  hf2->GetXaxis()->SetTitle("sin^{2}2#theta_{ee}");
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

  /*
  lim_nuedis_3p1_reactor_allowed_1_95pctCL-> Draw("lf");
  lim_nuedis_3p1_reactor_allowed_2_95pctCL-> Draw("lf");
  lim_nuedis_3p1_reactor_allowed_3_95pctCL-> Draw("lf");
  lim_nuedis_3p1_gallium_allowed_95pctCL  -> Draw("lf");
  lim_nuedis_3p1_solar_excluded_95pctCL   -> Draw("l");
  lim_nuedis_3p1_nueC_excluded_95pctCL    -> Draw("l");
  */
  lim_nuedis_3p1_t2knd280allowed_90pctCLW  -> Draw("f");
  lim_nuedis_3p1_t2knd280allowed_90pctCL  -> Draw("lf");
  lim_nuedis_3p1_t2knd280allowed_68pctCLW  -> Draw("f");
  lim_nuedis_3p1_t2knd280allowed_68pctCL  -> Draw("lf");
  lim_nuedis_3p1_t2knd280excluded_95pctCL -> Draw("l");

  valor_modern_sbn_3p1_contour_5sig_syst_03d1     -> Draw("L");
  valor_modern_sbn_3p1_contour_90pct_syst_03d1    -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_allW -> Draw("FL");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_all -> Draw("FL");
  valor_modern_sbn_3p1_contour_5sig_stat_all      -> Draw("FL");
  
  injectedBF                                      -> Draw("P");
  
  TLegend * lg1 = new TLegend(0.16,0.16,0.5,0.44);
  lg1->SetFillStyle(0);
  lg1->SetBorderSize(0);
  lg1->SetMargin(0.22);
  lg1->SetTextFont(132);
  lg1->SetTextSize(0.028);

  TLegend * lg1b = new TLegend(0.18,0.75,0.66,0.90);
  lg1b->SetFillStyle(0);
  lg1b->SetBorderSize(0);
  lg1b->SetMargin(0.18);
  lg1b->SetTextFont(132);
  lg1b->SetTextSize(0.028);
  
  /*
  lg1->AddEntry(lim_nuedis_3p1_reactor_allowed_1_95pctCL, "Reactor allowed 95\% CL",     "LF");
  lg1->AddEntry(lim_nuedis_3p1_gallium_allowed_95pctCL,   "Gallium allowed 95\% CL",     "LF");
  lg1->AddEntry(lim_nuedis_3p1_solar_excluded_95pctCL,    "Solar excluded 95\% CL",      "L");
  lg1->AddEntry(lim_nuedis_3p1_nueC_excluded_95pctCL,     "#nu_{e}+C excluded 95\% CL",  "L");
  */
  lg1->AddEntry(lim_nuedis_3p1_t2knd280excluded_95pctCL,  "T2K/ND280 excluded 95\% CL",  "L");
  lg1->AddEntry(lim_nuedis_3p1_t2knd280allowed_90pctCL,   "T2K/ND280 allowed 90\% CL",   "F");
  lg1->AddEntry(lim_nuedis_3p1_t2knd280allowed_68pctCL,   "T2K/ND280 allowed 68\% CL",   "F");
  lg1->AddEntry(injectedW,                                       " ", "P");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_03d1,    "90\% SBN Stat+Syst", "L");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1,     "5#sigma SBN Stat+Syst", "L");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1_all, "5#sigma SBN Stat+Syst", "F");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,      "5#sigma SBN Stat-Only", "F");
  lg1b->AddEntry(injectedW,                                      "Injected Point,", "P");
  lg1b->AddEntry(injectedBF,                                     "sin^{2}2#theta_{#mu e} = 0.003,", "P");
  lg1b->AddEntry(injectedW,                                      "#Deltam_{41}^{2} = 1.32", "P");
  lg1->Draw();
  lg1b->Draw();
  
  c1->SaveAs((location+"nue_disapp_full_03d1_sensitivity_comparison.C").c_str());
  c1->SaveAs((location+"nue_disapp_full_03d1_sensitivity_comparison.root").c_str());
  c1->SaveAs((location+"nue_disapp_full_03d1_sensitivity_comparison.pdf").c_str());
  c1->SaveAs((location+"nue_disapp_full_03d1_sensitivity_comparison.png").c_str());

  
}

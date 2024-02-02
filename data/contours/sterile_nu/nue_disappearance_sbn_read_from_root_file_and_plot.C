//
// This macro reads in the ROOT file produced by convert_to_root_file_and_plot.C
// and plots the appearance and disappearance limits.
//
// C.A.
//

{

  TFile * f = new TFile("./old_sterile_3p1_limits.root","read");
  TFile *f_syst_03d1     = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/tn_2022/03d1/output_contours_as_graphs_nue_disapp_03d1.root"); 
  TFile *f_syst_03d1_all = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/tn_2022/03d1/output_contours_as_graphs_nue_disapp_03d1_dmsq_3_sinsq2th_0.4.root");
  TFile *f_stat_all      = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/tn_2022/03d1/output_contours_as_graphs_nue_disapp_stat_dmsq_3_sinsq2th_0.4.root");

  // Updated POT:
  //    SBND   = 10e20
  //    ICARUS = 14e20
  //
  TFile *f_syst_sbnPOT  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/newpot/stat-syst-nue-disapp-2020a1-03d1-newpot_contours_as_graphs.root");
  TFile *f_syst_2detPOT = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/newpot/stat-syst-nue-disapp-2020a1-03d1-newpot-nd-fd_contours_as_graphs.root");


  std::string location = "/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/tn_2022/output/";
  

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
  
  TGraph * valor_modern_sbn_3p1_contour_90pct_syst_sbn_newPOT   = dynamic_cast<TGraph *>( f_syst_sbnPOT->Get("stat_syst_90pct_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_90pct_syst_2det_newPOT  = dynamic_cast<TGraph *>( f_syst_2detPOT->Get("stat_syst_90pct_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_sbn_newPOT    = dynamic_cast<TGraph *>( f_syst_sbnPOT->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_2det_newPOT   = dynamic_cast<TGraph *>( f_syst_2detPOT->Get("stat_syst_5sigma_lin_0"));
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

  lim_nuedis_3p1_t2knd280excluded_95pctCL -> SetLineWidth(2);
  lim_nuedis_3p1_t2knd280allowed_90pctCL  -> SetLineWidth(2);
  lim_nuedis_3p1_t2knd280allowed_68pctCL  -> SetLineWidth(2);
  lim_nuedis_3p1_reactor_allowed_1_95pctCL-> SetLineWidth(2);
  lim_nuedis_3p1_reactor_allowed_2_95pctCL-> SetLineWidth(2);
  lim_nuedis_3p1_reactor_allowed_3_95pctCL-> SetLineWidth(2);
  lim_nuedis_3p1_gallium_allowed_95pctCL  -> SetLineWidth(2);
  lim_nuedis_3p1_solar_excluded_95pctCL   -> SetLineWidth(2);
  lim_nuedis_3p1_nueC_excluded_95pctCL    -> SetLineWidth(2);
  
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

  valor_modern_sbn_3p1_contour_90pct_syst_2det_newPOT  -> SetLineColor(kAzure+4);  
  valor_modern_sbn_3p1_contour_90pct_syst_2det_newPOT  -> SetLineStyle(9);
  valor_modern_sbn_3p1_contour_90pct_syst_2det_newPOT  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_90pct_syst_sbn_newPOT  -> SetLineColor(kAzure+4);  
  valor_modern_sbn_3p1_contour_90pct_syst_sbn_newPOT  -> SetLineStyle(9);
  valor_modern_sbn_3p1_contour_90pct_syst_sbn_newPOT  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_2det_newPOT  -> SetLineColor(kAzure+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_2det_newPOT  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_2det_newPOT  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_sbn_newPOT  -> SetLineColor(kAzure+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_sbn_newPOT  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_sbn_newPOT  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_90pct_syst_03d1  -> SetLineColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_90pct_syst_03d1  -> SetLineStyle(9);
  valor_modern_sbn_3p1_contour_90pct_syst_03d1  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_03d1  -> SetLineColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_03d1  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03d1  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_90pct_syst_03d1_all -> SetLineColor(kOrange+4); 
  valor_modern_sbn_3p1_contour_90pct_syst_03d1_all -> SetLineStyle(9);
  valor_modern_sbn_3p1_contour_90pct_syst_03d1_all -> SetLineWidth(3);
  valor_modern_sbn_3p1_contour_90pct_syst_03d1_all -> SetFillColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_90pct_syst_03d1_all -> SetFillStyle(3001);

  valor_modern_sbn_3p1_contour_5sig_syst_03d1_all -> SetLineColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_all -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_all -> SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_all -> SetFillColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_all -> SetFillStyle(3001);
  
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_allW -> SetFillColor(kWhite);  
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_allW -> SetFillStyle(1001);
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_allW -> SetLineStyle(0);
  
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetLineColor(kOrange+5);  
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetFillColor(kOrange+5);  
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetFillStyle(3001);
  
  valor_modern_sbn_3p1_contour_5sig_stat_allW -> SetFillColor(kWhite);  
  valor_modern_sbn_3p1_contour_5sig_stat_allW -> SetFillStyle(1001);
  valor_modern_sbn_3p1_contour_5sig_stat_allW -> SetLineStyle(0);
  
  // Allowed
  injectedW->SetMarkerColor(kWhite);

  injectedBF->SetMarkerStyle(43);
  injectedBF->SetMarkerSize(2);
  injectedBF->SetMarkerColor(kGray+3);
  
  // Drawing
  TCanvas * c1 = new TCanvas("c1","",20,20,945,900);
  c1->SetTopMargin(0.05);
  c1->SetBottomMargin(0.13);
  c1->SetLeftMargin(0.13);
  c1->SetRightMargin(0.024);

  TH1F * hf1 = dynamic_cast<TH1F*>(c1->DrawFrame(1E-2,1E-2,1,1e2));
  hf1->Draw();
  hf1->GetYaxis()->SetTitleOffset(1);
  hf1->GetXaxis()->SetTitle("sin^{2}2#theta_{ee}");
  hf1->GetYaxis()->SetTitle("#Deltam^{2}_{41} (eV^{2})");
  hf1->GetXaxis()->SetTitleFont(132);
  hf1->GetYaxis()->SetTitleFont(132);
  hf1->GetXaxis()->SetLabelFont(132);
  hf1->GetYaxis()->SetLabelFont(132);
  hf1->GetXaxis()->SetTitleSize(0.06);
  hf1->GetYaxis()->SetTitleSize(0.06);
  hf1->GetXaxis()->SetLabelSize(0.05);
  hf1->GetYaxis()->SetLabelSize(0.05);
  
  c1->SetLogx();
  c1->SetLogy();

  lim_nuedis_3p1_t2knd280allowed_90pctCLW  -> Draw("f");
  lim_nuedis_3p1_t2knd280allowed_90pctCL  -> Draw("lf");
  lim_nuedis_3p1_t2knd280allowed_68pctCLW  -> Draw("f");
  lim_nuedis_3p1_t2knd280allowed_68pctCL  -> Draw("lf");
  lim_nuedis_3p1_t2knd280excluded_95pctCL -> Draw("l");

  valor_modern_sbn_3p1_contour_5sig_syst_03d1     -> Draw("L");
  valor_modern_sbn_3p1_contour_90pct_syst_03d1    -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_allW -> Draw("FL");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_all -> Draw("FL");
  valor_modern_sbn_3p1_contour_5sig_stat_allW     -> Draw("FL");
  valor_modern_sbn_3p1_contour_5sig_stat_all      -> Draw("FL");
  
  injectedBF                                      -> Draw("P");
  
  TLegend * lg1 = new TLegend(0.16,0.16,0.5,0.44);
  lg1->SetFillStyle(0);
  lg1->SetBorderSize(0);
  lg1->SetMargin(0.22);
  lg1->SetTextFont(132);
  lg1->SetTextSize(0.026);

  TLegend * lg1b = new TLegend(0.14,0.72,0.66,0.88);
  lg1b->SetFillStyle(0);
  lg1b->SetBorderSize(0);
  lg1b->SetMargin(0.18);
  lg1b->SetTextFont(132);
  lg1b->SetTextSize(0.026);
  
  lg1->AddEntry(lim_nuedis_3p1_t2knd280excluded_95pctCL,  "T2K/ND280 excluded 95\% CL",  "L");
  lg1->AddEntry(lim_nuedis_3p1_t2knd280allowed_90pctCL,   "T2K/ND280 allowed 90\% CL",   "F");
  lg1->AddEntry(lim_nuedis_3p1_t2knd280allowed_68pctCL,   "T2K/ND280 allowed 68\% CL",   "F");
  lg1->AddEntry(injectedW,                                       " ", "P");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_03d1,    "90\% SBN Stat+Syst", "L");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1,     "5#sigma SBN Stat+Syst", "L");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1_all, "5#sigma SBN Stat+Syst", "F");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,      "5#sigma SBN Stat-Only", "F");
  lg1b->AddEntry(injectedW,                                      "Injected Point,", "P");
  lg1b->AddEntry(injectedBF,                                     "sin^{2}2#theta_{ee} = 0.4,", "P");
  lg1b->AddEntry(injectedW,                                      "sin^{2}2#theta_{#mue} = 0,", "P");
  lg1b->AddEntry(injectedW,                                      "#Deltam_{41}^{2} = 3 eV^{2}", "P");
  lg1->Draw();
  lg1b->Draw();
  
  TLatex latex1;
  latex1.SetTextFont(132);
  latex1.SetTextSize(0.026);
  latex1.DrawLatexNDC(0.13,0.96,"#color[2]{SBN Preliminary}");
  latex1.SetTextSize(0.026);
  latex1.DrawLatexNDC(0.30,0.96,"#color[1]{SBND (6.6e20 POT) MicroBooNE (13.2e20 POT) ICARUS (6.6e20 POT)}");
  
  c1->SaveAs((location+"nue_disapp_full_03d1_sensitivity_comparison.C").c_str());
  c1->SaveAs((location+"nue_disapp_full_03d1_sensitivity_comparison.root").c_str());
  c1->SaveAs((location+"nue_disapp_full_03d1_sensitivity_comparison.pdf").c_str());
  c1->SaveAs((location+"nue_disapp_full_03d1_sensitivity_comparison.png").c_str());

  // SBN modern-only
  TCanvas * c2 = new TCanvas("c2","",20,20,1000,900);
  c2->SetTopMargin(0.024);
  c2->SetBottomMargin(0.13);
  c2->SetLeftMargin(0.13);
  c2->SetRightMargin(0.024);

  hf1->Draw();
  
  c2->SetLogx();
  c2->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_syst_03d1 -> Draw("L");
  
  TLegend * lg2 = new TLegend(0.18,0.16,0.6,0.4);
  lg2->SetFillStyle(0);
  lg2->SetBorderSize(0);
  lg2->SetTextFont(132);
  lg2->SetTextSize(0.032);
  
  lg2->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1, "5#sigma Modern Stat+Syst", "L");
  lg2->Draw();
  
  c2->SaveAs((location+"nue_disapp_mod_03d1_sensitivity_comparison.C").c_str());
  c2->SaveAs((location+"nue_disapp_mod_03d1_sensitivity_comparison.root").c_str());
  c2->SaveAs((location+"nue_disapp_mod_03d1_sensitivity_comparison.pdf").c_str());
  c2->SaveAs((location+"nue_disapp_mod_03d1_sensitivity_comparison.png").c_str());
  
  TCanvas * c3 = new TCanvas("c3","",20,20,945,900);
  c3->SetTopMargin(0.05);
  c3->SetBottomMargin(0.13);
  c3->SetLeftMargin(0.13);
  c3->SetRightMargin(0.024);

  hf1->Draw();
  
  c3->SetLogx();
  c3->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_syst_03d1     -> Draw("L");
  valor_modern_sbn_3p1_contour_90pct_syst_03d1    -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_allW -> Draw("FL");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_all -> Draw("FL");
  valor_modern_sbn_3p1_contour_5sig_stat_allW     -> Draw("FL");
  valor_modern_sbn_3p1_contour_5sig_stat_all      -> Draw("FL");
  
  injectedBF                                      -> Draw("P");
  
  TLegend * lg3 = new TLegend(0.16,0.16,0.5,0.44);
  lg3->SetFillStyle(0);
  lg3->SetBorderSize(0);
  lg3->SetMargin(0.22);
  lg3->SetTextFont(132);
  lg3->SetTextSize(0.028);

  TLegend * lg3b = new TLegend(0.14,0.72,0.66,0.88);
  lg3b->SetFillStyle(0);
  lg3b->SetBorderSize(0);
  lg3b->SetMargin(0.18);
  lg3b->SetTextFont(132);
  lg3b->SetTextSize(0.028);
  
  lg3->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_03d1,    "90\% SBN Stat+Syst", "L");
  lg3->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1,     "5#sigma SBN Stat+Syst", "L");
  lg3->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1_all, "5#sigma SBN Stat+Syst", "F");
  lg3->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,      "5#sigma SBN Stat-Only", "F");
  lg3b->AddEntry(injectedW,                                      "Injected Point,", "P");
  lg3b->AddEntry(injectedBF,                                     "sin^{2}2#theta_{ee} = 0.4,", "P");
  lg3b->AddEntry(injectedW,                                      "sin^{2}2#theta_{#mue} = 0,", "P");
  lg3b->AddEntry(injectedW,                                      "#Deltam_{41}^{2} = 3 eV^{2}", "P");
  lg3->Draw();
  lg3b->Draw();
  
  latex1.SetTextFont(132);
  latex1.SetTextSize(0.026);
  latex1.DrawLatexNDC(0.13,0.96,"#color[2]{SBN Preliminary}");
  latex1.SetTextSize(0.026);
  latex1.DrawLatexNDC(0.30,0.96,"#color[1]{SBND (6.6e20 POT) MicroBooNE (13.2e20 POT) ICARUS (6.6e20 POT)}");
  
  c3->SaveAs((location+"nue_disapp_sbn_03d1_sensitivity_comparison.C").c_str());
  c3->SaveAs((location+"nue_disapp_sbn_03d1_sensitivity_comparison.root").c_str());
  c3->SaveAs((location+"nue_disapp_sbn_03d1_sensitivity_comparison.pdf").c_str());
  c3->SaveAs((location+"nue_disapp_sbn_03d1_sensitivity_comparison.png").c_str());

  TCanvas * c4 = new TCanvas("c4","",20,20,945,900);
  c4->SetTopMargin(0.05);
  c4->SetBottomMargin(0.13);
  c4->SetLeftMargin(0.13);
  c4->SetRightMargin(0.024);

  hf1->Draw();
  
  c4->SetLogx();
  c4->SetLogy();

  lim_nuedis_3p1_t2knd280allowed_90pctCLW  -> Draw("f");
  lim_nuedis_3p1_t2knd280allowed_90pctCL   -> Draw("lf");
  lim_nuedis_3p1_t2knd280allowed_68pctCLW  -> Draw("f");
  lim_nuedis_3p1_t2knd280allowed_68pctCL   -> Draw("lf");
  lim_nuedis_3p1_t2knd280excluded_95pctCL  -> Draw("l");

  valor_modern_sbn_3p1_contour_90pct_syst_sbn_newPOT -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1        -> Draw("L");
  valor_modern_sbn_3p1_contour_90pct_syst_03d1       -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_allW   -> Draw("FL");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_all    -> Draw("FL");
  valor_modern_sbn_3p1_contour_5sig_stat_allW     -> Draw("FL");
  valor_modern_sbn_3p1_contour_5sig_stat_all         -> Draw("FL");
                                                     
  injectedBF                                         -> Draw("P");
  
  TLegend * lg4 = new TLegend(0.16,0.16,0.5,0.52);
  lg4->SetFillStyle(0);
  lg4->SetBorderSize(0);
  lg4->SetMargin(0.22);
  lg4->SetTextFont(132);
  lg4->SetTextSize(0.026);

  TLegend * lg4b = new TLegend(0.14,0.72,0.66,0.88);
  lg4b->SetFillStyle(0);
  lg4b->SetBorderSize(0);
  lg4b->SetMargin(0.18);
  lg4b->SetTextFont(132);
  lg4b->SetTextSize(0.026);
  
  lg4->AddEntry(lim_nuedis_3p1_t2knd280excluded_95pctCL,           "T2K/ND280 excluded 95\% CL",  "L");
  lg4->AddEntry(lim_nuedis_3p1_t2knd280allowed_90pctCL,            "T2K/ND280 allowed 90\% CL",   "F");
  lg4->AddEntry(lim_nuedis_3p1_t2knd280allowed_68pctCL,            "T2K/ND280 allowed 68\% CL",   "F");
  lg4->AddEntry(injectedW,                                          " ", "P");
  lg4->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1_all,    "5#sigma SBN Stat+Syst", "F");
  lg4->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,         "5#sigma SBN Stat-Only", "F");
  lg4->AddEntry(injectedW,                                          " ", "P");
  lg4->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1,        "5#sigma SBN Stat+Syst", "L");
  lg4->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_03d1,       "90\% SBN Stat+Syst", "L");
  lg4->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_sbn_newPOT, "90\% SBN, Increased POT Projection:", "L");
  lg4->AddEntry(injectedW,                                          "SBND (10e20) MicroBooNE (13.2e20) ICARUS (14e20)", "P");

  lg4b->AddEntry(injectedW,                                        "Injected Point,", "P");
  lg4b->AddEntry(injectedBF,                                       "sin^{2}2#theta_{ee} = 0.4,", "P");
  lg4b->AddEntry(injectedW,                                        "sin^{2}2#theta_{#mue} = 0,", "P");
  lg4b->AddEntry(injectedW,                                        "#Deltam_{41}^{2} = 3 eV^{2}", "P");
  lg4->Draw();
  lg4b->Draw();
  
  TLatex latex4;
  latex4.SetTextFont(132);
  latex4.SetTextSize(0.026);
  latex4.DrawLatexNDC(0.13,0.96,"#color[2]{SBN Preliminary}");
  latex4.SetTextSize(0.026);
  latex4.DrawLatexNDC(0.30,0.96,"#color[1]{SBND (6.6e20 POT) MicroBooNE (13.2e20 POT) ICARUS (6.6e20 POT)}");
  
  c4->SaveAs((location+"nue_disapp_full_newPOT_sensitivity_comparison.C").c_str());
  c4->SaveAs((location+"nue_disapp_full_newPOT_sensitivity_comparison.root").c_str());
  c4->SaveAs((location+"nue_disapp_full_newPOT_sensitivity_comparison.pdf").c_str());
  c4->SaveAs((location+"nue_disapp_full_newPOT_sensitivity_comparison.png").c_str());

  TCanvas * c5 = new TCanvas("c5","",20,20,945,900);
  c5->SetTopMargin(0.05);
  c5->SetBottomMargin(0.13);
  c5->SetLeftMargin(0.13);
  c5->SetRightMargin(0.024);

  hf1->Draw();
  
  c5->SetLogx();
  c5->SetLogy();

  lim_nuedis_3p1_t2knd280allowed_90pctCLW  -> Draw("f");
  lim_nuedis_3p1_t2knd280allowed_90pctCL   -> Draw("lf");
  lim_nuedis_3p1_t2knd280allowed_68pctCLW  -> Draw("f");
  lim_nuedis_3p1_t2knd280allowed_68pctCL   -> Draw("lf");
  lim_nuedis_3p1_t2knd280excluded_95pctCL  -> Draw("l");

  valor_modern_sbn_3p1_contour_90pct_syst_2det_newPOT -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1         -> Draw("L");
  valor_modern_sbn_3p1_contour_90pct_syst_03d1        -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_allW    -> Draw("FL");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1_all     -> Draw("FL");
  valor_modern_sbn_3p1_contour_5sig_stat_allW         -> Draw("FL");
  valor_modern_sbn_3p1_contour_5sig_stat_all          -> Draw("FL");
                                                      
  injectedBF                                          -> Draw("P");
  
  TLegend * lg5 = new TLegend(0.16,0.16,0.5,0.52);
  lg5->SetFillStyle(0);
  lg5->SetBorderSize(0);
  lg5->SetMargin(0.22);
  lg5->SetTextFont(132);
  lg5->SetTextSize(0.026);

  TLegend * lg5b = new TLegend(0.14,0.72,0.66,0.88);
  lg5b->SetFillStyle(0);
  lg5b->SetBorderSize(0);
  lg5b->SetMargin(0.18);
  lg5b->SetTextFont(132);
  lg5b->SetTextSize(0.026);
  
  lg5->AddEntry(lim_nuedis_3p1_t2knd280excluded_95pctCL,            "T2K/ND280 excluded 95\% CL",  "L");
  lg5->AddEntry(lim_nuedis_3p1_t2knd280allowed_90pctCL,             "T2K/ND280 allowed 90\% CL",   "F");
  lg5->AddEntry(lim_nuedis_3p1_t2knd280allowed_68pctCL,             "T2K/ND280 allowed 68\% CL",   "F");
  lg5->AddEntry(injectedW,                                           " ", "P");
  lg5->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1_all,     "5#sigma SBN Stat+Syst", "F");
  lg5->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,          "5#sigma SBN Stat-Only", "F");
  lg5->AddEntry(injectedW,                                           " ", "P");
  lg5->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1,         "5#sigma SBN Stat+Syst", "L");
  lg5->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_03d1,        "90\% SBN Stat+Syst", "L");
  lg5->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_2det_newPOT, "90\% SBN, Increased POT Projection:", "L");
  lg5->AddEntry(injectedW,                                           "SBND (10e20) ICARUS (14e20) Only", "P");

  lg5b->AddEntry(injectedW,                                        "Injected Point,", "P");
  lg5b->AddEntry(injectedBF,                                       "sin^{2}2#theta_{ee} = 0.4,", "P");
  lg5b->AddEntry(injectedW,                                        "sin^{2}2#theta_{#mue} = 0,", "P");
  lg5b->AddEntry(injectedW,                                        "#Deltam_{41}^{2} = 3 eV^{2}", "P");
  lg5->Draw();
  lg5b->Draw();
  
  TLatex latex5;
  latex5.SetTextFont(132);
  latex5.SetTextSize(0.026);
  latex5.DrawLatexNDC(0.13,0.96,"#color[2]{SBN Preliminary}");
  latex5.SetTextSize(0.026);
  latex5.DrawLatexNDC(0.30,0.96,"#color[1]{SBND (6.6e20 POT) MicroBooNE (13.2e20 POT) ICARUS (6.6e20 POT)}");
  
  c5->SaveAs((location+"nue_disapp_2det_newPOT_sensitivity_comparison.C").c_str());
  c5->SaveAs((location+"nue_disapp_2det_newPOT_sensitivity_comparison.root").c_str());
  c5->SaveAs((location+"nue_disapp_2det_newPOT_sensitivity_comparison.pdf").c_str());
  c5->SaveAs((location+"nue_disapp_2det_newPOT_sensitivity_comparison.png").c_str());

  TCanvas * c6 = new TCanvas("c6","",20,20,945,900);
  c6->SetTopMargin(0.05);
  c6->SetBottomMargin(0.13);
  c6->SetLeftMargin(0.13);
  c6->SetRightMargin(0.024);

  hf1->Draw();
  
  c6->SetLogx();
  c6->SetLogy();

  lim_nuedis_3p1_t2knd280allowed_90pctCLW  -> Draw("f");
  lim_nuedis_3p1_t2knd280allowed_90pctCL   -> Draw("lf");
  lim_nuedis_3p1_t2knd280allowed_68pctCLW  -> Draw("f");
  lim_nuedis_3p1_t2knd280allowed_68pctCL   -> Draw("lf");
  lim_nuedis_3p1_t2knd280excluded_95pctCL  -> Draw("l");

  valor_modern_sbn_3p1_contour_90pct_syst_sbn_newPOT -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_sbn_newPOT -> Draw("L");
  
  TLegend * lg6 = new TLegend(0.16,0.16,0.5,0.52);
  lg6->SetFillStyle(0);
  lg6->SetBorderSize(0);
  lg6->SetMargin(0.22);
  lg6->SetTextFont(132);
  lg6->SetTextSize(0.026);

  TLegend * lg6b = new TLegend(0.14,0.72,0.66,0.88);
  lg6b->SetFillStyle(0);
  lg6b->SetBorderSize(0);
  lg6b->SetMargin(0.18);
  lg6b->SetTextFont(132);
  lg6b->SetTextSize(0.026);
  
  lg6->AddEntry(lim_nuedis_3p1_t2knd280excluded_95pctCL,           "T2K/ND280 excluded 95\% CL",  "L");
  lg6->AddEntry(lim_nuedis_3p1_t2knd280allowed_90pctCL,            "T2K/ND280 allowed 90\% CL",   "F");
  lg6->AddEntry(lim_nuedis_3p1_t2knd280allowed_68pctCL,            "T2K/ND280 allowed 68\% CL",   "F");
  lg6->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_sbn_newPOT, "90\% SBN Stat+Syst", "L");
  lg6->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_sbn_newPOT,  "5#sigma SBN Stat+Syst", "L");
  lg6->Draw();
  lg6b->Draw();
  
  TLatex latex6;
  latex6.SetTextFont(132);
  latex6.SetTextSize(0.026);
  latex6.DrawLatexNDC(0.13,0.96,"#color[2]{SBN Preliminary}");
  latex6.SetTextSize(0.026);
  latex6.DrawLatexNDC(0.30,0.96,"#color[1]{SBND (10e20 POT) MicroBooNE (13.2e20 POT) ICARUS (14e20 POT)}");
  
  c6->SaveAs((location+"nue_disapp_full_newPOT_sensitivity.C").c_str());
  c6->SaveAs((location+"nue_disapp_full_newPOT_sensitivity.root").c_str());
  c6->SaveAs((location+"nue_disapp_full_newPOT_sensitivity.pdf").c_str());
  c6->SaveAs((location+"nue_disapp_full_newPOT_sensitivity.png").c_str());

  TCanvas * c7 = new TCanvas("c7","",20,20,945,900);
  c7->SetTopMargin(0.05);
  c7->SetBottomMargin(0.13);
  c7->SetLeftMargin(0.13);
  c7->SetRightMargin(0.024);

  hf1->Draw();
  
  c7->SetLogx();
  c7->SetLogy();

  lim_nuedis_3p1_t2knd280allowed_90pctCLW  -> Draw("f");
  lim_nuedis_3p1_t2knd280allowed_90pctCL   -> Draw("lf");
  lim_nuedis_3p1_t2knd280allowed_68pctCLW  -> Draw("f");
  lim_nuedis_3p1_t2knd280allowed_68pctCL   -> Draw("lf");
  lim_nuedis_3p1_t2knd280excluded_95pctCL  -> Draw("l");

  valor_modern_sbn_3p1_contour_90pct_syst_2det_newPOT -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_2det_newPOT -> Draw("L");
  
  TLegend * lg7 = new TLegend(0.16,0.16,0.5,0.52);
  lg7->SetFillStyle(0);
  lg7->SetBorderSize(0);
  lg7->SetMargin(0.22);
  lg7->SetTextFont(132);
  lg7->SetTextSize(0.026);

  TLegend * lg7b = new TLegend(0.14,0.72,0.66,0.88);
  lg7b->SetFillStyle(0);
  lg7b->SetBorderSize(0);
  lg7b->SetMargin(0.18);
  lg7b->SetTextFont(132);
  lg7b->SetTextSize(0.026);
  
  lg7->AddEntry(lim_nuedis_3p1_t2knd280excluded_95pctCL,            "T2K/ND280 excluded 95\% CL",  "L");
  lg7->AddEntry(lim_nuedis_3p1_t2knd280allowed_90pctCL,             "T2K/ND280 allowed 90\% CL",   "F");
  lg7->AddEntry(lim_nuedis_3p1_t2knd280allowed_68pctCL,             "T2K/ND280 allowed 68\% CL",   "F");
  lg7->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_2det_newPOT, "90\% SBN Stat+Syst", "L");
  lg7->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_2det_newPOT,  "5#sigma SBN Stat+Syst", "L");
  lg7->Draw();
  lg7b->Draw();
  
  TLatex latex7;
  latex7.SetTextFont(132);
  latex7.SetTextSize(0.026);
  latex7.DrawLatexNDC(0.13,0.96,"#color[2]{SBN Preliminary}");
  latex7.SetTextSize(0.026);
  latex7.DrawLatexNDC(0.60,0.96,"#color[1]{SBND (10e20 POT) ICARUS (14e20 POT)}");
  
  c7->SaveAs((location+"nue_disapp_2det_newPOT_sensitivity.C").c_str());
  c7->SaveAs((location+"nue_disapp_2det_newPOT_sensitivity.root").c_str());
  c7->SaveAs((location+"nue_disapp_2det_newPOT_sensitivity.pdf").c_str());
  c7->SaveAs((location+"nue_disapp_2det_newPOT_sensitivity.png").c_str());

}

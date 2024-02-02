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

  TGraph GetRatio(const TGraph *num, const TGraph *den);
  
  TFile * f = new TFile("./sterile_3p1_limits.root","read");
  
  // Standard analysis
  TFile *f_stat_nd     = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-only-2020a1-nd_contours_as_graphs.root","read");
  TFile *f_stat_fd     = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-only-2020a1-fd_contours_as_graphs.root","read");
  TFile *f_stat_nd_fd  = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-only-2020a1-nd-fd_contours_as_graphs.root","read");
  TFile *f_stat        = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/inclusive/stat-only-2020a1_contours_as_graphs.root","read");

  TFile *f_syst_nd     = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-syst-2020a1-03c1-nd_contours_as_graphs.root","read");
  TFile *f_syst_fd     = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-syst-2020a1-03c1-fd_contours_as_graphs.root","read");
  TFile *f_syst_nd_fd  = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/oldpot/stat-syst-numu-disapp-2020a1-oldpot_contours_as_graphs.root","read");
  TFile *f_syst        = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/inclusive/stat-syst-2020a1-03c1_contours_as_graphs.root","read");

  // POTs
  //    New
  //    SBND   = 10e20
  //    ICARUS = 14e20
  //
  //    Low
  //    SBND   = 1e20
  //    ICARUS = 5e20
  TFile *f_stat_lowPOT_nd_fd = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/lowpot/stat-only-2020a1-lowpot-nd-fd_contours_as_graphs.root","read");
  TFile *f_stat_newPOT_nd_fd = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/newpot/stat-only-2020a1-newpot-nd-fd_contours_as_graphs.root","read");
  
  //TFile *f_syst_lowPOT_nd    = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/lowpot/stat-syst-numu-disapp-2020a1-03d1-lowpot-nd_contours_as_graphs.root","read");
  //TFile *f_syst_lowPOT_fd    = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/lowpot/stat-syst-numu-disapp-2020a1-03d1-lowpot-fd_contours_as_graphs.root","read");
  TFile *f_syst_lowPOT_nd_fd = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/lowpot/stat-syst-numu-disapp-2020a1-03d1-lowpot-nd-fd_contours_as_graphs.root","read");
  //TFile *f_syst_lowPOT       = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/lowpot/stat-syst-numu-disapp-2020a1-03d1-lowpot_contours_as_graphs.root","read");
  
  TFile *f_syst_newPOT_nd    = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/newpot/stat-syst-numu-disapp-2020a1-03d1-newpot-nd_contours_as_graphs.root","read");
  TFile *f_syst_newPOT_fd    = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/newpot/stat-syst-numu-disapp-2020a1-03d1-newpot-fd_contours_as_graphs.root","read");
  TFile *f_syst_newPOT_nd_fd = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/newpot/stat-syst-numu-disapp-2020a1-03d1-newpot-nd-fd_contours_as_graphs.root","read");
  TFile *f_syst_newPOT       = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/newpot/stat-syst-numu-disapp-2020a1-03d1-newpot_contours_as_graphs.root","read");
  
  // 100e20 and 50e20 POT
  TFile *f_syst_nd_fd_100e20  = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/newpot/stat-syst-numu-disapp-2020a1-100e20pot_contours_as_graphs.root","read");
  TFile *f_syst_nd_fd_50e20   = new TFile("/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/newpot/stat-syst-numu-disapp-2020a1-50e20pot_contours_as_graphs.root","read");

  // Output location
  std::string location = "/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/output/";
  
  // Palette
  std::vector<int> palDark{
    kViolet-6,
    kAzure-6,
    kTeal-6,
    kSpring+4,
    kOrange+4,
    kPink+4
  };
  std::vector<int> palDim{
    kViolet-7,
    kAzure-7,
    kTeal-7,
    kSpring+3,
    kOrange+3,
    kPink+3
  };
  std::vector<int> palMed{
    kBlue-2,
    kCyan-2,
    kGreen-2,
    kYellow+2,
    kRed+2,
    kMagenta+2
  };
  std::vector<int> palLit{
    kViolet-7,
    kAzure-7,
    kTeal-7,
    kSpring+7,
    kOrange+7,
    kPink+7
  };
  std::vector<int> palLight{
    kViolet-5,
    kAzure-5,
    kTeal-5,
    kSpring+5,
    kOrange+5,
    kPink+5
  };
   
  // All graphs
  //
  // Externals
  TGraph * lim_numudis_3p1_sbnproposal_5sigCL   = (TGraph*)f->Get("lim_numudis_3p1_sbnproposal_5sigCL");
  TGraph * lim_numudis_3p1_minosp_90pctCL       = (TGraph*)f->Get("lim_numudis_3p1_minosp_90pctCL");
  TGraph * lim_numudis_3p1_minisciboone_90pctCL = (TGraph*)f->Get("lim_numudis_3p1_minisciboone_90pctCL" );
  TGraph * lim_numudis_3p1_icecube_90pctCL      = (TGraph*)f->Get("lim_numudis_3p1_icecube_90pctCL" );
  TGraph * lim_numudis_3p1_icecube_99pctCL      = (TGraph*)f->Get("lim_numudis_3p1_icecube_99pctCL" );
  
  // Detectors
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_nd    = dynamic_cast<TGraph *>( f_stat_nd->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_fd    = dynamic_cast<TGraph *>( f_stat_fd->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_nd_fd = dynamic_cast<TGraph *>( f_stat_nd_fd->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_90pct_stat      = dynamic_cast<TGraph *>( f_stat->Get("stat_only_90pct_lin_0")); 
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat       = dynamic_cast<TGraph *>( f_stat->Get("stat_only_5sigma_lin_0"));
  
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_nd    = dynamic_cast<TGraph *>( f_syst_nd->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_fd    = dynamic_cast<TGraph *>( f_syst_fd->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_nd_fd = dynamic_cast<TGraph *>( f_syst_nd_fd->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_90pct_syst      = dynamic_cast<TGraph *>( f_syst->Get("stat_syst_90pct_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst       = dynamic_cast<TGraph *>( f_syst->Get("stat_syst_5sigma_lin_0"));
 
  // POTs
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_lowPOT_nd_fd = dynamic_cast<TGraph *>( f_stat_lowPOT_nd_fd->Get("stat_only_2020a1_lowpot_nd_fd_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_newPOT_nd_fd = dynamic_cast<TGraph *>( f_stat_newPOT_nd_fd->Get("stat_only_2020a1_newpot_nd_fd_5sigma_lin_0"));
 
  //TGraph * valor_modern_sbn_3p1_contour_5sig_syst_lowPOT_nd    = dynamic_cast<TGraph *>( f_syst_lowPOT_nd->Get("stat_syst_5sigma_lin_0"));
  //TGraph * valor_modern_sbn_3p1_contour_5sig_syst_lowPOT_fd    = dynamic_cast<TGraph *>( f_syst_lowPOT_fd->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_lowPOT_nd_fd = dynamic_cast<TGraph *>( f_syst_lowPOT_nd_fd->Get("stat_syst_5sigma_lin_0"));
  //TGraph * valor_modern_sbn_3p1_contour_5sig_syst_lowPOT       = dynamic_cast<TGraph *>( f_syst_lowPOT->Get("stat_syst_5sigma_lin_0"));
                     
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_newPOT_nd    = dynamic_cast<TGraph *>( f_syst_newPOT_nd->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_newPOT_fd    = dynamic_cast<TGraph *>( f_syst_newPOT_fd->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_newPOT_nd_fd = dynamic_cast<TGraph *>( f_syst_newPOT_nd_fd->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_newPOT       = dynamic_cast<TGraph *>( f_syst_newPOT->Get("stat_syst_5sigma_lin_0"));
                     
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_100e20POT_nd_fd = dynamic_cast<TGraph *>( f_syst_nd_fd_100e20->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_50e20POT_nd_fd  = dynamic_cast<TGraph *>( f_syst_nd_fd_50e20->Get("stat_syst_5sigma_lin_0"));
  
  // Allowed
  const std::vector<double> x = {0.07, 0.007, 0.7, 0.07, 0.07, 0.0065, 0.0185};
  const std::vector<double> y = {1.32, 1.32, 1.32, 13.2, 0.132, 100, 1};
  TGraph *injectedW   = new TGraph(1,&x.data()[0],&y.data()[0]);
  
  std::cout << " Got contours" << std::endl; 
 
  // Stat only
  valor_modern_sbn_3p1_contour_5sig_stat_nd    -> SetLineColor(palLight.at(0));
  valor_modern_sbn_3p1_contour_5sig_stat_nd    -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_nd    -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_stat_fd    -> SetLineColor(palLight.at(1));
  valor_modern_sbn_3p1_contour_5sig_stat_fd    -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_fd    -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_stat_nd_fd -> SetLineColor(palLight.at(2));
  valor_modern_sbn_3p1_contour_5sig_stat_nd_fd -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_nd_fd -> SetLineWidth(3);
  
  valor_modern_sbn_3p1_contour_5sig_stat       -> SetLineColor(palLight.at(3));
  valor_modern_sbn_3p1_contour_5sig_stat       -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat       -> SetLineWidth(3);

  // Stat syst
  valor_modern_sbn_3p1_contour_5sig_syst_nd    -> SetLineColor(palLight.at(0));
  valor_modern_sbn_3p1_contour_5sig_syst_nd    -> SetLineStyle(7);
  valor_modern_sbn_3p1_contour_5sig_syst_nd    -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_fd    -> SetLineColor(palLight.at(1));
  valor_modern_sbn_3p1_contour_5sig_syst_fd    -> SetLineStyle(7);
  valor_modern_sbn_3p1_contour_5sig_syst_fd    -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_nd_fd -> SetLineColor(palDark.at(2));
  valor_modern_sbn_3p1_contour_5sig_syst_nd_fd -> SetLineStyle(7);
  valor_modern_sbn_3p1_contour_5sig_syst_nd_fd -> SetLineWidth(2);
  
  valor_modern_sbn_3p1_contour_5sig_syst       -> SetLineColor(palLight.at(3));
  valor_modern_sbn_3p1_contour_5sig_syst       -> SetLineStyle(7);
  valor_modern_sbn_3p1_contour_5sig_syst       -> SetLineWidth(3);

  // LowPOT
  valor_modern_sbn_3p1_contour_5sig_stat_lowPOT_nd_fd -> SetLineColor(palLight.at(2));
  valor_modern_sbn_3p1_contour_5sig_stat_lowPOT_nd_fd -> SetLineStyle(4);
  valor_modern_sbn_3p1_contour_5sig_stat_lowPOT_nd_fd -> SetLineWidth(3);
  /*
  valor_modern_sbn_3p1_contour_5sig_syst_lowPOT_nd -> SetLineColor(palMed.at(0));
  valor_modern_sbn_3p1_contour_5sig_syst_lowPOT_nd -> SetLineStyle(4);
  valor_modern_sbn_3p1_contour_5sig_syst_lowPOT_nd -> SetLineWidth(3);
  
  valor_modern_sbn_3p1_contour_5sig_syst_lowPOT_fd -> SetLineColor(palMed.at(1));
  valor_modern_sbn_3p1_contour_5sig_syst_lowPOT_fd -> SetLineStyle(4);
  valor_modern_sbn_3p1_contour_5sig_syst_lowPOT_fd -> SetLineWidth(3);
  */
  valor_modern_sbn_3p1_contour_5sig_syst_lowPOT_nd_fd -> SetLineColor(palMed.at(1));
  valor_modern_sbn_3p1_contour_5sig_syst_lowPOT_nd_fd -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_lowPOT_nd_fd -> SetLineWidth(3);
  /*
  valor_modern_sbn_3p1_contour_5sig_syst_lowPOT -> SetLineColor(palMed.at(3));
  valor_modern_sbn_3p1_contour_5sig_syst_lowPOT -> SetLineStyle(4);
  valor_modern_sbn_3p1_contour_5sig_syst_lowPOT -> SetLineWidth(3);
  */
 
  // New POT
  valor_modern_sbn_3p1_contour_5sig_stat_newPOT_nd_fd -> SetLineColor(palLight.at(2));
  valor_modern_sbn_3p1_contour_5sig_stat_newPOT_nd_fd -> SetLineStyle(3);
  valor_modern_sbn_3p1_contour_5sig_stat_newPOT_nd_fd -> SetLineWidth(3);
  
  valor_modern_sbn_3p1_contour_5sig_syst_newPOT_nd -> SetLineColor(palDark.at(0));
  valor_modern_sbn_3p1_contour_5sig_syst_newPOT_nd -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_newPOT_nd -> SetLineWidth(3);
  
  valor_modern_sbn_3p1_contour_5sig_syst_newPOT_fd -> SetLineColor(palDark.at(1));
  valor_modern_sbn_3p1_contour_5sig_syst_newPOT_fd -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_newPOT_fd -> SetLineWidth(3);
  
  valor_modern_sbn_3p1_contour_5sig_syst_newPOT_nd_fd -> SetLineColor(palDim.at(0));
  valor_modern_sbn_3p1_contour_5sig_syst_newPOT_nd_fd -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_newPOT_nd_fd -> SetLineWidth(3);
  
  valor_modern_sbn_3p1_contour_5sig_syst_newPOT -> SetLineColor(palDark.at(3));
  valor_modern_sbn_3p1_contour_5sig_syst_newPOT -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_newPOT -> SetLineWidth(3);
  
  valor_modern_sbn_3p1_contour_5sig_syst_100e20POT_nd_fd -> SetLineColor(palDim.at(2));
  valor_modern_sbn_3p1_contour_5sig_syst_100e20POT_nd_fd -> SetLineStyle(4);
  valor_modern_sbn_3p1_contour_5sig_syst_100e20POT_nd_fd -> SetLineWidth(2);
  
  valor_modern_sbn_3p1_contour_5sig_syst_50e20POT_nd_fd -> SetLineColor(palLit.at(2));
  valor_modern_sbn_3p1_contour_5sig_syst_50e20POT_nd_fd -> SetLineStyle(9);
  valor_modern_sbn_3p1_contour_5sig_syst_50e20POT_nd_fd -> SetLineWidth(2);
  
  // Allowed
  injectedW->SetMarkerColor(kWhite);

  // External
  lim_numudis_3p1_sbnproposal_5sigCL       -> SetLineColor(kGray+2);
  lim_numudis_3p1_sbnproposal_5sigCL       -> SetLineWidth(2);
  
  lim_numudis_3p1_minosp_90pctCL           -> SetLineColor(kGray+2); 
  lim_numudis_3p1_minosp_90pctCL           -> SetLineWidth(2); 
  lim_numudis_3p1_minosp_90pctCL           -> SetLineStyle(2); 
  
  lim_numudis_3p1_minisciboone_90pctCL     -> SetLineColor(kGray+2); 
  lim_numudis_3p1_minisciboone_90pctCL     -> SetLineWidth(2); 
  lim_numudis_3p1_minisciboone_90pctCL     -> SetLineStyle(3); 

  lim_numudis_3p1_icecube_90pctCL          -> SetLineColor(kGray+2); 
  lim_numudis_3p1_icecube_90pctCL          -> SetLineWidth(2); 
  lim_numudis_3p1_icecube_90pctCL          -> SetLineStyle(4); 

  lim_numudis_3p1_icecube_99pctCL          -> SetLineColor(kGray+2); 
  lim_numudis_3p1_icecube_99pctCL          -> SetLineWidth(2); 
  lim_numudis_3p1_icecube_99pctCL          -> SetLineStyle(5); 

  TCanvas * c1 = new TCanvas("c1","",20,20,900,900);
  c1->SetTopMargin(0.024);
  c1->SetBottomMargin(0.13);
  c1->SetLeftMargin(0.13);
  c1->SetRightMargin(0.024);
  c1->Draw();
  c1->SetLogx();
  c1->SetLogy();

  TLegend * lg1 = new TLegend(0.18,0.18,0.75,0.42);
  lg1->SetFillStyle(0);
  lg1->SetBorderSize(0);
  lg1->SetTextFont(132);

  // Reusable frame
  TH1F * hf1 = dynamic_cast<TH1F*>(c1->DrawFrame(1E-3,1E-2,1,100));
  hf1->Draw();
  hf1->GetYaxis()->SetTitleOffset(1);
  hf1->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hf1->GetYaxis()->SetTitle("#Deltam^{2}_{41} (eV^{2})");
  hf1->GetXaxis()->SetTitleFont(132);
  hf1->GetYaxis()->SetTitleFont(132);
  hf1->GetXaxis()->SetLabelFont(132);
  hf1->GetYaxis()->SetLabelFont(132);
  hf1->GetXaxis()->SetTitleSize(0.06);
  hf1->GetYaxis()->SetTitleSize(0.06);
  hf1->GetXaxis()->SetLabelSize(0.05);
  hf1->GetYaxis()->SetLabelSize(0.05);

  // --------------------------------------------------------
  // Stat POTs
  // -------------------------------------------------------

  valor_modern_sbn_3p1_contour_5sig_stat_nd_fd-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_lowPOT_nd_fd-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_newPOT_nd_fd-> Draw("L");

  lg1->AddEntry(injectedW,                                           "5#sigma SBND, ICARUS", "P");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_lowPOT_nd_fd, "  1e20 POT,   5e20 POT", "L");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_nd_fd,        "6.6e20 POT, 6.6e20 POT", "L");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_newPOT_nd_fd, " 10e20 POT,  14e20 POT", "L");
  lg1->Draw("same");
  
  c1->SaveAs((location+"numu_disapp_pot_nd_fd_stat_comparison.root").c_str());
  c1->SaveAs((location+"numu_disapp_pot_nd_fd_stat_comparison.png").c_str());
  c1->SaveAs((location+"numu_disapp_pot_nd_fd_stat_comparison.pdf").c_str());

  lg1->Clear();
  
  // --------------------------------------------------------
  // Stat+syst POTs
  // -------------------------------------------------------
  TCanvas * c2 = new TCanvas("c2","",20,20,900,900);
  c2->SetTopMargin(0.024);
  c2->SetBottomMargin(0.13);
  c2->SetLeftMargin(0.13);
  c2->SetRightMargin(0.024);
  hf1->Draw();
  c2->SetLogx();
  c2->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_syst_nd_fd-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_lowPOT_nd_fd-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_newPOT_nd_fd-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_50e20POT_nd_fd-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_100e20POT_nd_fd-> Draw("L");

  lg1->AddEntry(injectedW,                                           "5#sigma SBND, ICARUS", "P");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_lowPOT_nd_fd, "  1e20 POT,   5e20 POT", "L");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_nd_fd,        "6.6e20 POT, 6.6e20 POT", "L");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_newPOT_nd_fd, " 10e20 POT,  14e20 POT", "L");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_50e20POT_nd_fd, " 50e20 POT,  50e20 POT", "L");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_100e20POT_nd_fd, " 100e20 POT,  100e20 POT", "L");
  lg1->Draw("same");
  
  c2->SaveAs((location+"numu_disapp_pot_nd_fd_syst_comparison.root").c_str());
  c2->SaveAs((location+"numu_disapp_pot_nd_fd_syst_comparison.png").c_str());
  c2->SaveAs((location+"numu_disapp_pot_nd_fd_syst_comparison.pdf").c_str());

  lg1->Clear();
  
}
TGraph GetRatio(const TGraph *num, const TGraph *den){
  // Get the dimension from the denomenator 
  int nPointsNum = num->GetN();
  int nPointsDen = den->GetN();

  // sinsq
  double *sinsqNum = num->GetX();
  double *sinsqDen = den->GetX();

  // dmsq
  double *dmsqNum = num->GetY();
  double *dmsqDen = den->GetY();

  // Invert the numerator so we can evaluate at the points from the denomenator
  TGraph numInv(nPointsNum, dmsqNum, sinsqNum);

  // Setup the vectors for filling the new TGraph
  TVectorD x(nPointsDen);
  TVectorD y(nPointsDen);
  // Now loop over the points and get the values
  for(int i = 0; i < nPointsDen; ++i){

    // From the old TGraph
    double denSinsq = *(sinsqDen+i);
    double denDmsq  = *(dmsqDen+i);
    double numVal   = numInv.Eval(denDmsq);

    // For the new TGraph
    double ratioVal = numVal/denSinsq;
    x(i) = ratioVal;
    y(i) = denDmsq;
  }
  TGraph ratio(x,y);
  return ratio;
}

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
  
  // VALOR modern disappearance files
  TFile *f_stat = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/inclusive/stat-only-2020a1_contours_as_graphs.root","read");
 
  // Updated POT:
  //    SBND   = 10e20
  //    ICARUS = 14e20
  //
  TFile *f_syst_sbnPOT  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/newpot/stat-syst-numu-disapp-2020a1-03d1-newpot_contours_as_graphs.root");
  TFile *f_syst_2detPOT = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/newpot/stat-syst-numu-disapp-2020a1-03d1-newpot-nd-fd_contours_as_graphs.root");

  // Inclusive
  TFile *f_syst_03d1 = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/tn_2022/03d1/output_contours_as_graphs_numu_disapp_03d1.root"); 
  TFile *f_syst_03c1 = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/rebase_test/contours/stat-syst-2020a1-03c1_contours_as_graphs.root"); 
  TFile *f_syst_03c2 = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/inclusive/stat-syst-2020a1-03c2-fix_contours_as_graphs.root");
  TFile *f_syst_03c3 = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/inclusive/stat-syst-2020a1-03c3-fix_contours_as_graphs.root");
  
  // Exclusive
  TFile *f_syst_03c1_exc      = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/rebase_test/contours/stat-syst-2020a1-03c1-exc_contours_as_graphs.root"); 
  TFile *f_syst_03c2_exc      = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/exclusive/stat-syst-2020a1-03c2-exc-fix_contours_as_graphs.root");
  TFile *f_syst_03c3_exc      = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/exclusive/stat-syst-2020a1-03c3-noescale-exc_contours_as_graphs.root");
  TFile *f_syst_03c3_bias_exc = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/rebase_test/contours/stat-syst-2020a1-03c3-exc-bias-only_contours_as_graphs.root"); 

  // Selection
  TFile *f_stat_sel_nd       = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/selection/stat-only-selection-nd_contours_as_graphs.root");
  TFile *f_stat_sel_fd       = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/selection/stat-only-selection-fd_contours_as_graphs.root");
  TFile *f_stat_sel_nd_fd    = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/selection/stat-only-selection-nd-fd_contours_as_graphs.root");

  TFile *f_syst_sel_03c2_nd    = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/selection/stat-syst-selection-03c2-nd_contours_as_graphs.root");
  TFile *f_syst_sel_03c2_fd    = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/selection/stat-syst-selection-03c2-fd_contours_as_graphs.root");
  TFile *f_syst_sel_03c2_nd_fd = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/selection/stat-syst-selection-03c2-nd-fd_contours_as_graphs.root");
  //TFile *f_syst_2020a1_03c2_nd_fd = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/selection/no_eff/stat-syst-2020a1-03c2-exc-nd-fd_contours_as_graphs.root");
  TFile *f_syst_2020a1_03c2_nd_fd = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/selection/stat-syst-2020a1-03c2-nd-fd-exc_contours_as_graphs.root");

  // Systematics
  TFile *f_syst_escale_1pct = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Feb2021/energy_scale/stat-syst-e-scale-1pct_contours_as_graphs.root");
  TFile *f_syst_escale_2pct = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Feb2021/energy_scale/stat-syst-e-scale-2pct_contours_as_graphs.root");
  TFile *f_syst_escale_5pct = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Feb2021/energy_scale/stat-syst-e-scale-5pct_contours_as_graphs.root");

  TFile *f_syst_det  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/systematic_groups/stat-syst-2020a1-det_contours_as_graphs.root"); 
  TFile *f_syst_prop = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/systematic_groups/stat-syst-2020a1-proposal-alt_contours_as_graphs.root");
  TFile *f_syst_mod  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/systematic_groups/stat-syst-2020a1-modern-alt_contours_as_graphs.root");
  TFile *f_syst_xsec = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/systematic_groups/stat-syst-2020a1-03c1-all-xsec-alt_contours_as_graphs.root");
  TFile *f_syst_flux = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/systematic_groups/stat-syst-2020a1-flux-alt_contours_as_graphs.root");
 
  // Detectors
  TFile *f_stat_nd     = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-only-2020a1-nd_contours_as_graphs.root","read");
  TFile *f_stat_ub     = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-only-2020a1-ub_contours_as_graphs.root","read");
  TFile *f_stat_fd     = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-only-2020a1-fd_contours_as_graphs.root","read");
  TFile *f_stat_nd_fd  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-only-2020a1-nd-fd_contours_as_graphs.root","read");
  TFile *f_stat_nd_ub  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-only-2020a1-nd-ub_contours_as_graphs.root","read");
  TFile *f_stat_ub_fd  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-only-2020a1-ub-fd_contours_as_graphs.root","read");
  TFile *f_stat_lowPOT_nd_fd = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/lowpot/stat-only-2020a1-lowpot-nd-fd_contours_as_graphs.root","read");
  TFile *f_stat_newPOT_nd_fd = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/newpot/stat-only-2020a1-newpot-nd-fd_contours_as_graphs.root","read");

  TFile *f_syst_nd     = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-syst-2020a1-03c1-nd_contours_as_graphs.root","read");
  TFile *f_syst_ub     = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-syst-2020a1-03c1-ub_contours_as_graphs.root","read");
  TFile *f_syst_fd     = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-syst-2020a1-03c1-fd_contours_as_graphs.root","read");
  TFile *f_syst_nd_fd  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/2023/oldpot/stat-syst-numu-disapp-2020a1-oldpot_contours_as_graphs.root","read");
  TFile *f_syst_nd_ub  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-syst-2020a1-03c1-nd-ub_contours_as_graphs.root","read");
  TFile *f_syst_ub_fd  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-syst-2020a1-03c1-ub-fd_contours_as_graphs.root","read");

  TFile *f_stat_2019b3_nd = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-only-2019b3-nd_contours_as_graphs.root","read");
  TFile *f_stat_2019b3_fd = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/detectors/stat-only-2019b3-fd_contours_as_graphs.root","read");
  
  // Models
  TFile *f_stat_2019b3 = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/models/stat-only-2019b3-nd-fd_contours_as_graphs.root","read");
  TFile *f_stat_susav2 = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/models/stat-only-susav2-nd-fd_contours_as_graphs.root","read");
  TFile *f_stat_smith  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/models/stat-only-smith-nd-fd_contours_as_graphs.root","read");
  TFile *f_stat_lse    = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/models/stat-only-lse-nd-fd_contours_as_graphs.root","read");

  // Exclusive allows regions
  TFile *f_stat_all               = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/allowed/stat-only-inj-2020a1_contours_as_graphs.root"); 
  TFile *f_syst_03d1_all          = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/tn_2022/03d1/output_contours_as_graphs_numu_03d1_dmsq_1.32_sinsq2th_0.07.root"); 
  TFile *f_syst_03c1_all          = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/allowed/stat-syst-inj-2020a1-03c1_contours_as_graphs.root"); 
  TFile *f_syst_03c1_exc_all      = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/allowed/stat-syst-2020a1-03c1-inj-exc_contours_as_graphs.root"); 
  TFile *f_syst_03c2_exc_all      = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/allowed/stat-syst-2020a1-03c2-inj-exc_contours_as_graphs.root");
  TFile *f_syst_03c3_exc_all      = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/allowed/stat-syst-2020a1-03c3-inj-exc_contours_as_graphs.root");
  TFile *f_syst_03c1_exc_all_bias = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/allowed/stat-syst-2020a1-03c1-bias-inj-exc_contours_as_graphs.root");

  // Allowed regions, different points
  /*
  TFile *f_stat_all_a = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/allowed/stat-only-2020a1-inj-a_contours_as_graphs.root"); 
  TFile *f_stat_all_b = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/allowed/stat-only-2020a1-inj-b_contours_as_graphs.root"); 
  TFile *f_stat_all_c = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/allowed/stat-only-2020a1-inj-c_contours_as_graphs.root"); 
  TFile *f_stat_all_d = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/thesis/allowed/stat-only-2020a1-inj-d_contours_as_graphs.root"); 
  */

  std::string location = "/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/tn_2022/output/";
  
  // 
  // All graphs
  //
  // Externals
  TGraph * lim_numudis_3p1_sbnproposal_5sigCL   = (TGraph*)f->Get("lim_numudis_3p1_sbnproposal_5sigCL");
  TGraph * lim_numudis_3p1_minosp_90pctCL       = (TGraph*)f->Get("lim_numudis_3p1_minosp_90pctCL");
  TGraph * lim_numudis_3p1_minisciboone_90pctCL = (TGraph*)f->Get("lim_numudis_3p1_minisciboone_90pctCL" );
  TGraph * lim_numudis_3p1_icecube_90pctCL      = (TGraph*)f->Get("lim_numudis_3p1_icecube_90pctCL" );
  TGraph * lim_numudis_3p1_icecube_99pctCL      = (TGraph*)f->Get("lim_numudis_3p1_icecube_99pctCL" );
  
  // New POT
  TGraph * valor_modern_sbn_3p1_contour_90pct_syst_sbn_newPOT   = dynamic_cast<TGraph *>( f_syst_sbnPOT->Get("stat_syst_90pct_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_sbn_newPOT   = dynamic_cast<TGraph *>( f_syst_sbnPOT->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_90pct_syst_2det_newPOT  = dynamic_cast<TGraph *>( f_syst_2detPOT->Get("stat_syst_90pct_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_2det_newPOT  = dynamic_cast<TGraph *>( f_syst_2detPOT->Get("stat_syst_5sigma_lin_0"));
  
  // Stat+syst                                                 
  TGraph * valor_modern_sbn_3p1_contour_90pct_syst_03d1         = dynamic_cast<TGraph *>( f_syst_03d1->Get("contour_90pct_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03d1          = dynamic_cast<TGraph *>( f_syst_03d1->Get("contour_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_90pct_syst_03c1         = dynamic_cast<TGraph *>( f_syst_03c1->Get("stat_syst_90pct_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c1          = dynamic_cast<TGraph *>( f_syst_03c1->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c2          = dynamic_cast<TGraph *>( f_syst_03c2->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c3          = dynamic_cast<TGraph *>( f_syst_03c3->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc      = dynamic_cast<TGraph *>( f_syst_03c1_exc->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc      = dynamic_cast<TGraph *>( f_syst_03c2_exc->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c3_exc      = dynamic_cast<TGraph *>( f_syst_03c3_exc->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c1_inc      = dynamic_cast<TGraph *>( f_syst_03c1->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c3_bias_exc = dynamic_cast<TGraph *>( f_syst_03c3_bias_exc->Get("stat_syst_5sigma_lin_0"));

  // Energy scale
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_escale_1pct  = dynamic_cast<TGraph *>( f_syst_escale_1pct->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_escale_2pct  = dynamic_cast<TGraph *>( f_syst_escale_2pct->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_escale_5pct  = dynamic_cast<TGraph *>( f_syst_escale_5pct->Get("stat_syst_5sigma_lin_0"));
  
  // Systematic groups
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_det   = dynamic_cast<TGraph *>( f_syst_det->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_prop  = dynamic_cast<TGraph *>( f_syst_prop->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_mod   = dynamic_cast<TGraph *>( f_syst_mod->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_xsec  = dynamic_cast<TGraph *>( f_syst_xsec->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_flux  = dynamic_cast<TGraph *>( f_syst_flux->Get("stat_syst_5sigma_lin_0"));

  // Stat-only                                                 
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat           = dynamic_cast<TGraph *>( f_stat->Get("stat_only_5sigma_lin_0")); // 2020a1 sample
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_exc       = dynamic_cast<TGraph *>( f_stat->Get("stat_only_5sigma_lin_0")); // 2020a1 sample
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_3det      = dynamic_cast<TGraph *>( f_stat->Get("stat_only_5sigma_lin_0")); // 2020a1 sample

  // Selection
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_sel_nd    = dynamic_cast<TGraph *>( f_stat_sel_nd->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_sel_fd    = dynamic_cast<TGraph *>( f_stat_sel_fd->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_sel_nd_fd = dynamic_cast<TGraph *>( f_stat_sel_nd_fd->Get("stat_only_5sigma_lin_0"));
  
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_2_sel_nd    = dynamic_cast<TGraph *>( f_stat_sel_nd->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_2_sel_fd    = dynamic_cast<TGraph *>( f_stat_sel_fd->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_2_sel_nd_fd = dynamic_cast<TGraph *>( f_stat_sel_nd_fd->Get("stat_only_5sigma_lin_0"));
  
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_sel_03c2_nd    = dynamic_cast<TGraph *>( f_syst_sel_03c2_nd->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_sel_03c2_fd    = dynamic_cast<TGraph *>( f_syst_sel_03c2_fd->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_sel_03c2_nd_fd = dynamic_cast<TGraph *>( f_syst_sel_03c2_nd_fd->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_2020a1_03c2_nd_fd = dynamic_cast<TGraph *>( f_syst_2020a1_03c2_nd_fd->Get("stat_syst_5sigma_lin_0"));
  
  // Detectors
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_nd           = dynamic_cast<TGraph *>( f_stat_nd->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_ub           = dynamic_cast<TGraph *>( f_stat_ub->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_fd           = dynamic_cast<TGraph *>( f_stat_fd->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_nd_fd        = dynamic_cast<TGraph *>( f_stat_nd_fd->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_nd_ub        = dynamic_cast<TGraph *>( f_stat_nd_ub->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_ub_fd        = dynamic_cast<TGraph *>( f_stat_ub_fd->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_lowPOT_nd_fd = dynamic_cast<TGraph *>( f_stat_lowPOT_nd_fd->Get("stat_only_2020a1_lowpot_nd_fd_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_newPOT_nd_fd = dynamic_cast<TGraph *>( f_stat_newPOT_nd_fd->Get("stat_only_2020a1_newpot_nd_fd_5sigma_lin_0"));
  
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_nd    = dynamic_cast<TGraph *>( f_syst_nd->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_ub    = dynamic_cast<TGraph *>( f_syst_ub->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_fd    = dynamic_cast<TGraph *>( f_syst_fd->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_nd_fd = dynamic_cast<TGraph *>( f_syst_nd_fd->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_nd_ub = dynamic_cast<TGraph *>( f_syst_nd_ub->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_ub_fd = dynamic_cast<TGraph *>( f_syst_ub_fd->Get("stat_syst_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_3det  = dynamic_cast<TGraph *>( f_syst_03c1->Get("stat_syst_5sigma_lin_0"));
  
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_2020a1_nd    = dynamic_cast<TGraph *>( f_stat_nd->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_2020a1_fd    = dynamic_cast<TGraph *>( f_stat_fd->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_2020a1_nd_fd = dynamic_cast<TGraph *>( f_stat_nd_fd->Get("stat_only_5sigma_lin_0"));
  
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_2019b3_nd    = dynamic_cast<TGraph *>( f_stat_2019b3_nd->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_2019b3_fd    = dynamic_cast<TGraph *>( f_stat_2019b3_fd->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_2019b3_nd_fd = dynamic_cast<TGraph *>( f_stat_2019b3->Get("stat_only_5sigma_lin_0"));

  // Models
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_2019b3 = dynamic_cast<TGraph *>( f_stat_2019b3->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_susav2 = dynamic_cast<TGraph *>( f_stat_susav2->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_smith  = dynamic_cast<TGraph *>( f_stat_smith->Get("stat_only_5sigma_lin_0"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_lse    = dynamic_cast<TGraph *>( f_stat_lse->Get("stat_only_5sigma_lin_0"));

  // Model ratios
  TGraph ratio_ref    = GetRatio(valor_modern_sbn_3p1_contour_5sig_stat_2019b3,valor_modern_sbn_3p1_contour_5sig_stat_2019b3);
  TGraph ratio_nd_fd  = GetRatio(valor_modern_sbn_3p1_contour_5sig_stat_nd_fd,valor_modern_sbn_3p1_contour_5sig_stat_2019b3);
  TGraph ratio_susav2 = GetRatio(valor_modern_sbn_3p1_contour_5sig_stat_susav2,valor_modern_sbn_3p1_contour_5sig_stat_2019b3);
  TGraph ratio_smith  = GetRatio(valor_modern_sbn_3p1_contour_5sig_stat_smith,valor_modern_sbn_3p1_contour_5sig_stat_2019b3);
  TGraph ratio_lse    = GetRatio(valor_modern_sbn_3p1_contour_5sig_stat_lse,valor_modern_sbn_3p1_contour_5sig_stat_2019b3);

  // Systematic group ratios
  TGraph ratio_stat_ref = GetRatio(valor_modern_sbn_3p1_contour_5sig_stat,valor_modern_sbn_3p1_contour_5sig_stat);
  TGraph ratio_prop     = GetRatio(valor_modern_sbn_3p1_contour_5sig_syst_prop,valor_modern_sbn_3p1_contour_5sig_stat);
  TGraph ratio_mod      = GetRatio(valor_modern_sbn_3p1_contour_5sig_syst_mod,valor_modern_sbn_3p1_contour_5sig_stat);
  TGraph ratio_xsec     = GetRatio(valor_modern_sbn_3p1_contour_5sig_syst_xsec,valor_modern_sbn_3p1_contour_5sig_stat);
  TGraph ratio_flux     = GetRatio(valor_modern_sbn_3p1_contour_5sig_syst_flux,valor_modern_sbn_3p1_contour_5sig_stat);
  TGraph ratio_det      = GetRatio(valor_modern_sbn_3p1_contour_5sig_syst_det,valor_modern_sbn_3p1_contour_5sig_stat);
                                                                  
  // MC/Reco ratios
  TGraph ratio_mc_reco_nd    = GetRatio(valor_modern_sbn_3p1_contour_5sig_stat_nd,valor_modern_sbn_3p1_contour_5sig_stat_sel_nd);
  TGraph ratio_mc_reco_fd    = GetRatio(valor_modern_sbn_3p1_contour_5sig_stat_fd,valor_modern_sbn_3p1_contour_5sig_stat_sel_fd);
  TGraph ratio_mc_reco_nd_fd = GetRatio(valor_modern_sbn_3p1_contour_5sig_stat_nd_fd,valor_modern_sbn_3p1_contour_5sig_stat_sel_nd_fd);
  
  TGraph ratio_mc_reco_v2_nd    = GetRatio(valor_modern_sbn_3p1_contour_5sig_stat_2019b3_nd,valor_modern_sbn_3p1_contour_5sig_stat_sel_nd);
  TGraph ratio_mc_reco_v2_fd    = GetRatio(valor_modern_sbn_3p1_contour_5sig_stat_2019b3_fd,valor_modern_sbn_3p1_contour_5sig_stat_sel_fd);
  TGraph ratio_mc_reco_v2_nd_fd = GetRatio(valor_modern_sbn_3p1_contour_5sig_stat_2019b3_nd_fd,valor_modern_sbn_3p1_contour_5sig_stat_sel_nd_fd);
  
  TGraph ratio_escale_1pct = GetRatio(valor_modern_sbn_3p1_contour_5sig_syst_escale_1pct,valor_modern_sbn_3p1_contour_5sig_stat);
  TGraph ratio_escale_2pct = GetRatio(valor_modern_sbn_3p1_contour_5sig_syst_escale_2pct,valor_modern_sbn_3p1_contour_5sig_stat);
  TGraph ratio_escale_5pct = GetRatio(valor_modern_sbn_3p1_contour_5sig_syst_escale_5pct,valor_modern_sbn_3p1_contour_5sig_stat);
  
  // Allowed
  const std::vector<double> x = {0.07, 0.007, 0.7, 0.07, 0.07, 0.0065, 0.0185};
  const std::vector<double> y = {1.32, 1.32, 1.32, 13.2, 0.132, 100, 1};
  TGraph *injectedBF  = new TGraph(1,&x.data()[0],&y.data()[0]);
  TGraph *injectedW   = new TGraph(1,&x.data()[0],&y.data()[0]);
  TGraph *injecteda   = new TGraph(1,&x.data()[1],&y.data()[1]);
  TGraph *injectedb   = new TGraph(1,&x.data()[2],&y.data()[2]);
  TGraph *injectedc   = new TGraph(1,&x.data()[3],&y.data()[3]);
  TGraph *injectedd   = new TGraph(1,&x.data()[4],&y.data()[4]);
  TGraph *injected100 = new TGraph(1,&x.data()[5],&y.data()[5]);
  TGraph *injected1   = new TGraph(1,&x.data()[6],&y.data()[6]);
  
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_all               = dynamic_cast<TGraph *>( f_stat_all->Get("stat_only_5sigma_lin_0")); // 2020a1 sample
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_allW              = dynamic_cast<TGraph *>( f_stat_all->Get("stat_only_5sigma_lin_0")); // 2020a1 sample
  TGraph * valor_modern_sbn_3p1_contour_90pct_syst_03c1_all         = dynamic_cast<TGraph *>( f_syst_03c1_all->Get("stat_syst_90pct_lin_1"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c1_all          = dynamic_cast<TGraph *>( f_syst_03c1_all->Get("stat_syst_5sigma_lin_1"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c1_allW         = dynamic_cast<TGraph *>( f_syst_03c1_all->Get("stat_syst_5sigma_lin_1"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all      = dynamic_cast<TGraph *>( f_syst_03c1_exc_all->Get("stat_syst_5sigma_lin_1"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc_all      = dynamic_cast<TGraph *>( f_syst_03c2_exc_all->Get("stat_syst_5sigma_lin_2"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c3_exc_all      = dynamic_cast<TGraph *>( f_syst_03c3_exc_all->Get("stat_syst_5sigma_lin_4"));
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all_bias = dynamic_cast<TGraph *>( f_syst_03c1_exc_all_bias->Get("stat_syst_5sigma_lin_6"));
 
  /*
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_all_a             = dynamic_cast<TGraph *>( f_stat_all_a->Get("stat_only_5sigma_lin_0")); // 2020a1 sample
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_all_b             = dynamic_cast<TGraph *>( f_stat_all_b->Get("stat_only_5sigma_lin_0")); // 2020a1 sample
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_all_c             = dynamic_cast<TGraph *>( f_stat_all_c->Get("stat_only_5sigma_lin_0")); // 2020a1 sample
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_all_d             = dynamic_cast<TGraph *>( f_stat_all_d->Get("stat_only_5sigma_lin_0")); // 2020a1 sample
  */
  std::cout << " Got contours" << std::endl; 
  
  // Stat only
  valor_modern_sbn_3p1_contour_5sig_stat       -> SetLineColor(kOrange+4);
  valor_modern_sbn_3p1_contour_5sig_stat       -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat       -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_stat_exc   -> SetLineColor(kAzure+3);
  valor_modern_sbn_3p1_contour_5sig_stat_exc   -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_exc   -> SetLineWidth(3);

  // Detectors
  valor_modern_sbn_3p1_contour_5sig_stat_nd    -> SetLineColor(kOrange+2);
  valor_modern_sbn_3p1_contour_5sig_stat_nd    -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_nd    -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_stat_ub    -> SetLineColor(kAzure+5);
  valor_modern_sbn_3p1_contour_5sig_stat_ub    -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_ub    -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_stat_fd    -> SetLineColor(kSpring-5);
  valor_modern_sbn_3p1_contour_5sig_stat_fd    -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_fd    -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_stat_nd_fd -> SetLineColor(kViolet-6);
  valor_modern_sbn_3p1_contour_5sig_stat_nd_fd -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_stat_nd_fd -> SetLineWidth(2);
  
  valor_modern_sbn_3p1_contour_5sig_stat_nd_ub -> SetLineColor(kOrange+4);
  valor_modern_sbn_3p1_contour_5sig_stat_nd_ub -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_stat_nd_ub -> SetLineWidth(2);
  
  valor_modern_sbn_3p1_contour_5sig_stat_ub_fd -> SetLineColor(kTeal-6);
  valor_modern_sbn_3p1_contour_5sig_stat_ub_fd -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_stat_ub_fd -> SetLineWidth(2);
  
  valor_modern_sbn_3p1_contour_5sig_stat_lowPOT_nd_fd -> SetLineColor(kAzure+4);
  valor_modern_sbn_3p1_contour_5sig_stat_lowPOT_nd_fd -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_lowPOT_nd_fd -> SetLineWidth(3);
  
  valor_modern_sbn_3p1_contour_5sig_stat_newPOT_nd_fd -> SetLineColor(kSpring+4);
  valor_modern_sbn_3p1_contour_5sig_stat_newPOT_nd_fd -> SetLineStyle(4);
  valor_modern_sbn_3p1_contour_5sig_stat_newPOT_nd_fd -> SetLineWidth(3);
  
  valor_modern_sbn_3p1_contour_5sig_stat_3det  -> SetLineColor(kGray+3);
  valor_modern_sbn_3p1_contour_5sig_stat_3det  -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_3det  -> SetLineWidth(2);
 
  valor_modern_sbn_3p1_contour_5sig_syst_nd    -> SetLineColor(kOrange+2);
  valor_modern_sbn_3p1_contour_5sig_syst_nd    -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_syst_nd    -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_ub    -> SetLineColor(kAzure+5);
  valor_modern_sbn_3p1_contour_5sig_syst_ub    -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_syst_ub    -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_fd    -> SetLineColor(kSpring-5);
  valor_modern_sbn_3p1_contour_5sig_syst_fd    -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_syst_fd    -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_nd_fd -> SetLineColor(kViolet-6);
  valor_modern_sbn_3p1_contour_5sig_syst_nd_fd -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_nd_fd -> SetLineWidth(2);
  
  valor_modern_sbn_3p1_contour_5sig_syst_nd_ub -> SetLineColor(kOrange+4);
  valor_modern_sbn_3p1_contour_5sig_syst_nd_ub -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_nd_ub -> SetLineWidth(2);
  
  valor_modern_sbn_3p1_contour_5sig_syst_ub_fd -> SetLineColor(kTeal-6);
  valor_modern_sbn_3p1_contour_5sig_syst_ub_fd -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_ub_fd -> SetLineWidth(2);
  
  valor_modern_sbn_3p1_contour_5sig_syst_3det  -> SetLineColor(kGray+3);
  valor_modern_sbn_3p1_contour_5sig_syst_3det  -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_syst_3det  -> SetLineWidth(2);
 
  // 2019b3
  valor_modern_sbn_3p1_contour_5sig_stat_2019b3_nd    -> SetLineColor(kOrange+2);
  valor_modern_sbn_3p1_contour_5sig_stat_2019b3_nd    -> SetLineStyle(3);
  valor_modern_sbn_3p1_contour_5sig_stat_2019b3_nd    -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_stat_2019b3_fd    -> SetLineColor(kSpring-5);
  valor_modern_sbn_3p1_contour_5sig_stat_2019b3_fd    -> SetLineStyle(3);
  valor_modern_sbn_3p1_contour_5sig_stat_2019b3_fd    -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_stat_2019b3_nd_fd -> SetLineColor(kViolet-6);
  valor_modern_sbn_3p1_contour_5sig_stat_2019b3_nd_fd -> SetLineStyle(3);
  valor_modern_sbn_3p1_contour_5sig_stat_2019b3_nd_fd -> SetLineWidth(3);
 
  // 2020a1
  valor_modern_sbn_3p1_contour_5sig_stat_2020a1_nd    -> SetLineColor(kOrange+2);
  valor_modern_sbn_3p1_contour_5sig_stat_2020a1_nd    -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_2020a1_nd    -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_stat_2020a1_fd    -> SetLineColor(kSpring-5);
  valor_modern_sbn_3p1_contour_5sig_stat_2020a1_fd    -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_2020a1_fd    -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_stat_2020a1_nd_fd -> SetLineColor(kViolet-6);
  valor_modern_sbn_3p1_contour_5sig_stat_2020a1_nd_fd -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_2020a1_nd_fd -> SetLineWidth(3);
  
  // Selection
  valor_modern_sbn_3p1_contour_5sig_stat_sel_nd    -> SetLineColor(kOrange+2);
  valor_modern_sbn_3p1_contour_5sig_stat_sel_nd    -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_stat_sel_nd    -> SetLineWidth(2);
                                                   
  valor_modern_sbn_3p1_contour_5sig_stat_sel_fd    -> SetLineColor(kSpring-5);
  valor_modern_sbn_3p1_contour_5sig_stat_sel_fd    -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_stat_sel_fd    -> SetLineWidth(2);
                                                   
  valor_modern_sbn_3p1_contour_5sig_stat_sel_nd_fd -> SetLineColor(kViolet-6);
  valor_modern_sbn_3p1_contour_5sig_stat_sel_nd_fd -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_stat_sel_nd_fd -> SetLineWidth(2);

  valor_modern_sbn_3p1_contour_5sig_syst_sel_03c2_nd_fd -> SetLineColor(kSpring-5);
  valor_modern_sbn_3p1_contour_5sig_syst_sel_03c2_nd_fd -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_sel_03c2_nd_fd -> SetLineWidth(2);

  valor_modern_sbn_3p1_contour_5sig_syst_2020a1_03c2_nd_fd -> SetLineColor(kViolet-6);
  valor_modern_sbn_3p1_contour_5sig_syst_2020a1_03c2_nd_fd -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_2020a1_03c2_nd_fd -> SetLineWidth(2);

  valor_modern_sbn_3p1_contour_5sig_stat_2_sel_nd_fd -> SetLineColor(kSpring-5);
  valor_modern_sbn_3p1_contour_5sig_stat_2_sel_nd_fd -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_2_sel_nd_fd -> SetLineWidth(3);

  // MC/Reco
  ratio_mc_reco_nd    . SetLineColor(kOrange+2);
  ratio_mc_reco_nd    . SetLineStyle(2);
  ratio_mc_reco_nd    . SetLineWidth(3);
                                                   
  ratio_mc_reco_fd    . SetLineColor(kSpring-5);
  ratio_mc_reco_fd    . SetLineStyle(2);
  ratio_mc_reco_fd    . SetLineWidth(3);
                                                   
  ratio_mc_reco_nd_fd . SetLineColor(kViolet-6);
  ratio_mc_reco_nd_fd . SetLineStyle(2);
  ratio_mc_reco_nd_fd . SetLineWidth(3);

  ratio_mc_reco_v2_nd    . SetLineColor(kOrange+2);
  ratio_mc_reco_v2_nd    . SetLineStyle(1);
  ratio_mc_reco_v2_nd    . SetLineWidth(2);
                                                   
  ratio_mc_reco_v2_fd    . SetLineColor(kSpring-5);
  ratio_mc_reco_v2_fd    . SetLineStyle(3);
  ratio_mc_reco_v2_fd    . SetLineWidth(3);
                                                   
  ratio_mc_reco_v2_nd_fd . SetLineColor(kViolet-6);
  ratio_mc_reco_v2_nd_fd . SetLineStyle(3);
  ratio_mc_reco_v2_nd_fd . SetLineWidth(3);

  // Models
  valor_modern_sbn_3p1_contour_5sig_stat_2019b3-> SetLineColor(kOrange+4);
  valor_modern_sbn_3p1_contour_5sig_stat_2019b3-> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_stat_2019b3-> SetLineWidth(2);

  valor_modern_sbn_3p1_contour_5sig_stat_susav2-> SetLineColor(kAzure+5);
  valor_modern_sbn_3p1_contour_5sig_stat_susav2-> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_stat_susav2-> SetLineWidth(2);
  
  valor_modern_sbn_3p1_contour_5sig_stat_smith -> SetLineColor(kSpring-5);
  valor_modern_sbn_3p1_contour_5sig_stat_smith -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_stat_smith -> SetLineWidth(2);
  
  valor_modern_sbn_3p1_contour_5sig_stat_lse   -> SetLineColor(kOrange+2);
  valor_modern_sbn_3p1_contour_5sig_stat_lse   -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_stat_lse   -> SetLineWidth(2);
 
  // Model ratios
  ratio_ref    . SetLineColor(kBlack);
  ratio_ref    . SetLineStyle(2);
  ratio_ref    . SetLineWidth(1);

  ratio_susav2 . SetLineColor(kAzure+5);
  ratio_susav2 . SetLineStyle(1);
  ratio_susav2 . SetLineWidth(2);
  
  ratio_smith  . SetLineColor(kSpring-5);
  ratio_smith  . SetLineStyle(1);
  ratio_smith  . SetLineWidth(2);
               
  ratio_lse    . SetLineColor(kOrange+2);
  ratio_lse    . SetLineStyle(1);
  ratio_lse    . SetLineWidth(2);
               
  ratio_nd_fd  . SetLineColor(kViolet-6);
  ratio_nd_fd  . SetLineStyle(1);
  ratio_nd_fd  . SetLineWidth(2);
  
  // New POT
  valor_modern_sbn_3p1_contour_90pct_syst_sbn_newPOT  -> SetLineColor(kAzure+4);  
  valor_modern_sbn_3p1_contour_90pct_syst_sbn_newPOT  -> SetLineStyle(9);
  valor_modern_sbn_3p1_contour_90pct_syst_sbn_newPOT  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_90pct_syst_2det_newPOT  -> SetLineColor(kAzure+4);  
  valor_modern_sbn_3p1_contour_90pct_syst_2det_newPOT  -> SetLineStyle(9);
  valor_modern_sbn_3p1_contour_90pct_syst_2det_newPOT  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_sbn_newPOT  -> SetLineColor(kAzure+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_sbn_newPOT  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_sbn_newPOT  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_2det_newPOT  -> SetLineColor(kAzure+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_2det_newPOT  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_2det_newPOT  -> SetLineWidth(3);

  // VALOR modern syst
  // Inclusive
  valor_modern_sbn_3p1_contour_90pct_syst_03d1  -> SetLineColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_90pct_syst_03d1  -> SetLineStyle(9);
  valor_modern_sbn_3p1_contour_90pct_syst_03d1  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_03d1  -> SetLineColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_03d1  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03d1  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_90pct_syst_03c1  -> SetLineColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_90pct_syst_03c1  -> SetLineStyle(9);
  valor_modern_sbn_3p1_contour_90pct_syst_03c1  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_03c1  -> SetLineColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_03c2  -> SetLineColor(kAzure+5);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c2  -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_syst_03c2  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_03c3  -> SetLineColor(kOrange+2);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c3  -> SetLineStyle(3);
  valor_modern_sbn_3p1_contour_5sig_syst_03c3  -> SetLineWidth(3);
 
  // IncExc
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_inc  -> SetLineColor(kTeal-6);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_inc  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_inc  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc  -> SetLineColor(kAzure+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc  -> SetLineColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc  -> SetLineWidth(2);

  valor_modern_sbn_3p1_contour_5sig_syst_03c3_exc  -> SetLineColor(kViolet-6);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c3_exc  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03c3_exc  -> SetLineWidth(2);

  valor_modern_sbn_3p1_contour_5sig_syst_03c3_bias_exc  -> SetLineColor(kViolet-6);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c3_bias_exc  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03c3_bias_exc  -> SetLineWidth(3);

  // Systematic groups
  valor_modern_sbn_3p1_contour_5sig_syst_det   -> SetLineColor(kAzure+5);  
  valor_modern_sbn_3p1_contour_5sig_syst_det   -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_det   -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_prop  -> SetLineColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_prop  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_prop  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_mod   -> SetLineColor(kSpring-5);  
  valor_modern_sbn_3p1_contour_5sig_syst_mod   -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_mod   -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_xsec  -> SetLineColor(kViolet-6);  
  valor_modern_sbn_3p1_contour_5sig_syst_xsec  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_xsec  -> SetLineWidth(3);

  valor_modern_sbn_3p1_contour_5sig_syst_flux  -> SetLineColor(kOrange+2);  
  valor_modern_sbn_3p1_contour_5sig_syst_flux  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_flux  -> SetLineWidth(3);

  // Systematic group ratios
  ratio_stat_ref . SetLineColor(kGray+3);
  ratio_stat_ref . SetLineStyle(2);
  ratio_stat_ref . SetLineWidth(3);

  ratio_det      . SetLineColor(kAzure+5);  
  ratio_det      . SetLineStyle(1);
  ratio_det      . SetLineWidth(3);
                 
  ratio_prop     . SetLineColor(kSpring-5);  
  ratio_prop     . SetLineStyle(1);
  ratio_prop     . SetLineWidth(3);
                 
  ratio_mod      . SetLineColor(kOrange+4);  
  ratio_mod      . SetLineStyle(1);
  ratio_mod      . SetLineWidth(3);
                 
  ratio_xsec     . SetLineColor(kViolet-6);  
  ratio_xsec     . SetLineStyle(1);
  ratio_xsec     . SetLineWidth(3);
                 
  ratio_flux     . SetLineColor(kAzure+5);  
  ratio_flux     . SetLineStyle(1);
  ratio_flux     . SetLineWidth(3);

  // Energy scale
  valor_modern_sbn_3p1_contour_5sig_syst_escale_1pct   -> SetLineColor(kOrange+2);  
  valor_modern_sbn_3p1_contour_5sig_syst_escale_1pct   -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_syst_escale_1pct   -> SetLineWidth(3);
  
  valor_modern_sbn_3p1_contour_5sig_syst_escale_2pct   -> SetLineColor(kAzure+5); 
  valor_modern_sbn_3p1_contour_5sig_syst_escale_2pct   -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_syst_escale_2pct   -> SetLineWidth(3);
  
  valor_modern_sbn_3p1_contour_5sig_syst_escale_5pct   -> SetLineColor(kSpring-5);  
  valor_modern_sbn_3p1_contour_5sig_syst_escale_5pct   -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_syst_escale_5pct   -> SetLineWidth(3);
  
  ratio_escale_1pct   . SetLineColor(kOrange+2);  
  ratio_escale_1pct   . SetLineStyle(2);
  ratio_escale_1pct   . SetLineWidth(3);
  
  ratio_escale_2pct   . SetLineColor(kAzure+5);  
  ratio_escale_2pct   . SetLineStyle(2);
  ratio_escale_2pct   . SetLineWidth(3);
  
  ratio_escale_5pct   . SetLineColor(kSpring-5);  
  ratio_escale_5pct   . SetLineStyle(2);
  ratio_escale_5pct   . SetLineWidth(3);
 
  // Allowed
  injectedW->SetMarkerColor(kWhite);

  injectedBF->SetMarkerStyle(43);
  injectedBF->SetMarkerSize(2);
  injectedBF->SetMarkerColor(kGray+3);

  injecteda->SetMarkerStyle(33);
  injecteda->SetMarkerSize(1.5);
  injecteda->SetMarkerColor(kAzure+5);
  
  injectedb->SetMarkerStyle(33);
  injectedb->SetMarkerSize(1.5);
  injectedb->SetMarkerColor(kOrange+2);
  
  injectedc->SetMarkerStyle(33);
  injectedc->SetMarkerSize(1.5);
  injectedc->SetMarkerColor(kYellow-3);
  
  injectedd->SetMarkerStyle(33);
  injectedd->SetMarkerSize(1.5);
  injectedd->SetMarkerColor(kViolet-6);
  
  injected100->SetMarkerStyle(39);
  injected100->SetMarkerSize(2);
  injected100->SetMarkerColor(kViolet-6);

  injected1->SetMarkerStyle(41);
  injected1->SetMarkerSize(2);
  injected1->SetMarkerColor(kTeal-6);

  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetLineColor(kOrange+5);
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetLineStyle(7);
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetFillColor(kOrange+5);
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetFillStyle(3001);
  valor_modern_sbn_3p1_contour_5sig_stat_allW -> SetFillColor(kWhite);
  valor_modern_sbn_3p1_contour_5sig_stat_allW -> SetFillStyle(1001);
  valor_modern_sbn_3p1_contour_5sig_stat_allW -> SetLineStyle(0);

  valor_modern_sbn_3p1_contour_90pct_syst_03c1_all -> SetLineColor(kOrange+4); 
  valor_modern_sbn_3p1_contour_90pct_syst_03c1_all -> SetLineStyle(9);
  valor_modern_sbn_3p1_contour_90pct_syst_03c1_all -> SetLineWidth(3);
  valor_modern_sbn_3p1_contour_90pct_syst_03c1_all -> SetFillColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_90pct_syst_03c1_all -> SetFillStyle(3002);

  valor_modern_sbn_3p1_contour_5sig_syst_03c1_all -> SetLineColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_all -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_all -> SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_all -> SetFillColor(kOrange+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_all -> SetFillStyle(3001);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_allW -> SetFillColor(kWhite);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_allW -> SetFillStyle(1001);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_allW -> SetLineStyle(0);

  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all  -> SetLineColor(kOrange+5);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all  -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all  -> SetFillColor(kOrange+5);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all  -> SetFillStyle(3002);

  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc_all  -> SetLineColor(kAzure+5);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc_all  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc_all  -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc_all  -> SetFillColor(kAzure+5);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc_all  -> SetFillStyle(3002);

  valor_modern_sbn_3p1_contour_5sig_syst_03c3_exc_all  -> SetLineColor(kOrange+2);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c3_exc_all  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03c3_exc_all  -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_syst_03c3_exc_all  -> SetFillColor(kOrange+2);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c3_exc_all  -> SetFillStyle(3002);

  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all_bias  -> SetLineColor(kOrange+2);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all_bias  -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all_bias  -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all_bias  -> SetFillColor(kOrange+2);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all_bias  -> SetFillStyle(3002);

  /*
  valor_modern_sbn_3p1_contour_5sig_stat_all_a -> SetLineColor(kTeal-6);
  valor_modern_sbn_3p1_contour_5sig_stat_all_a -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_all_a -> SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_stat_all_a -> SetFillColor(kTeal-6);
  valor_modern_sbn_3p1_contour_5sig_stat_all_a -> SetFillStyle(3002);

  valor_modern_sbn_3p1_contour_5sig_stat_all_b -> SetLineColor(kTeal-6);
  valor_modern_sbn_3p1_contour_5sig_stat_all_b -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_all_b -> SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_stat_all_b -> SetFillColor(kTeal-6);
  valor_modern_sbn_3p1_contour_5sig_stat_all_b -> SetFillStyle(3002);

  valor_modern_sbn_3p1_contour_5sig_stat_all_c -> SetLineColor(kTeal-6);
  valor_modern_sbn_3p1_contour_5sig_stat_all_c -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_all_c -> SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_stat_all_c -> SetFillColor(kTeal-6);
  valor_modern_sbn_3p1_contour_5sig_stat_all_c -> SetFillStyle(3002);

  valor_modern_sbn_3p1_contour_5sig_stat_all_d -> SetLineColor(kTeal-6);
  valor_modern_sbn_3p1_contour_5sig_stat_all_d -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_stat_all_d -> SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_stat_all_d -> SetFillColor(kTeal-6);
  valor_modern_sbn_3p1_contour_5sig_stat_all_d -> SetFillStyle(3002);
  */

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

  // Reusable frame
  TH1F * hf1 = dynamic_cast<TH1F*>(c1->DrawFrame(1E-3,1E-2,1,100));
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
  // Demonstration of points in oscillation parameter space
  // --------------------------------------------------------
  
  TCanvas * cd = new TCanvas("cd","",20,20,900,900);
  cd->SetTopMargin(0.024);
  cd->SetBottomMargin(0.13);
  cd->SetLeftMargin(0.13);
  cd->SetRightMargin(0.024);
  hf1->Draw();
  cd->SetLogx();
  cd->SetLogy();

  injectedBF->SetMarkerColor(kAzure+5);
  valor_modern_sbn_3p1_contour_5sig_stat     -> SetLineColor(kGray+2);
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetLineColor(kGray+2);
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetFillColor(kGray+2);
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetFillStyle(3015);
  valor_modern_sbn_3p1_contour_5sig_stat     -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_all -> Draw("FL");
  injectedBF                                 -> Draw("P");
  injected100                                -> Draw("P");
  injected1                                  -> Draw("P");

  TLegend * lgd = new TLegend(0.2,0.18,0.75,0.42);
  lgd->SetFillStyle(0);
  lgd->SetBorderSize(0);
  lgd->SetTextFont(132);
  
  lgd->AddEntry(injected1,                                  "sin^{2}2#theta_{#mu#mu} = 0.0185, #Deltam_{41}^{2} = 1 eV^{2}", "P");
  lgd->AddEntry(injectedBF,                                 "sin^{2}2#theta_{#mu#mu} = 0.0700, #Deltam_{41}^{2} = 1.32 eV^{2}", "P");
  lgd->AddEntry(injected100,                                "sin^{2}2#theta_{#mu#mu} = 0.0065, #Deltam_{41}^{2} = 100 eV^{2}", "P");
  lgd->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat,     "5#sigma Stat-Only Exclusion Region", "L");
  lgd->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all, "5#sigma Stat-Only Allowed Region",   "F");
  lgd->Draw();
  
  cd->SaveAs((location+"oscillation_point_demonstration.root").c_str());
  cd->SaveAs((location+"oscillation_point_demonstration.png").c_str());
  cd->SaveAs((location+"oscillation_point_demonstration.pdf").c_str());

  injectedBF->SetMarkerColor(kGray+3);
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetLineColor(kOrange+6);
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetFillColor(kOrange+6);
  valor_modern_sbn_3p1_contour_5sig_stat_all -> SetFillStyle(3001);
  //valor_modern_sbn_3p1_contour_5sig_stat     -> SetLineColor(kOrange+4);
  
  /*
  // --------------------------------------------------------
  // Energy scale comparison
  // -------------------------------------------------------

  TCanvas * c1 = new TCanvas("c1","",20,20,900,900);
  c1->SetTopMargin(0.024);
  c1->SetBottomMargin(0.13);
  c1->SetLeftMargin(0.13);
  c1->SetRightMargin(0.024);
  c1->Draw();
  c1->SetLogx();
  c1->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_stat            ->Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_escale_1pct->Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_escale_2pct->Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_escale_5pct->Draw("L");

  TLegend * lg1 = new TLegend(0.2,0.2,0.75,0.45);
  lg1->SetFillStyle(0);
  lg1->SetBorderSize(0);
  lg1->SetTextFont(132);
  
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat,             "5#sigma Stat-Only",             "L");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_escale_1pct, "5#sigma Stat+1\% Energy Scale", "L");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_escale_2pct, "5#sigma Stat+2\% Energy Scale", "L");
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_escale_5pct, "5#sigma Stat+5\% Energy Scale", "L");
  lg1->Draw();
  
  c1->SaveAs((location+"energy_scale_comparison.root").c_str());
  c1->SaveAs((location+"energy_scale_comparison.png").c_str());
  c1->SaveAs((location+"energy_scale_comparison.pdf").c_str());
 
  */
  // --------------------------------------------------------
  // InclusiveExclusive
  // -------------------------------------------------------

  TCanvas * c2 = new TCanvas("c2","",20,20,900,900);
  c2->SetTopMargin(0.024);
  c2->SetBottomMargin(0.13);
  c2->SetLeftMargin(0.13);
  c2->SetRightMargin(0.024);
  hf1->Draw();
  c2->SetLogx();
  c2->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_stat               -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_inc      -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc      -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c3_bias_exc -> Draw("L");

  TLegend * lg2 = new TLegend(0.18,0.18,0.85,0.45);
  lg2->SetFillStyle(0);
  lg2->SetBorderSize(0);
  lg2->SetTextFont(132);
  lg2->SetTextSize(0.035);
  
  lg2->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat,               "5#sigma Stat-Only", "L");
  lg2->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_inc,      "5#sigma Inclusive Flux+Interaction Syst", "L");
  lg2->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc,      "5#sigma Exclusive Flux+Interaction Syst", "L");
  lg2->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c3_bias_exc, "5#sigma Exclusive Flux+Interaction+Bias Syst", "L");
  lg2->Draw("same");
  
  c2->SaveAs((location+"exclusive_inclusive_03c1_bias_syst_comparison.root").c_str());
  c2->SaveAs((location+"exclusive_inclusive_03c1_bias_syst_comparison.pdf").c_str());
  c2->SaveAs((location+"exclusive_inclusive_03c1_bias_syst_comparison.png").c_str());

  /*
  // --------------------------------------------------------
  // MC Reco
  // -------------------------------------------------------

  TCanvas * c3 = new TCanvas("c3","",20,20,900,900);
  c3->SetTopMargin(0.024);
  c3->SetBottomMargin(0.13);
  c3->SetLeftMargin(0.13);
  c3->SetRightMargin(0.024);
  hf1->Draw();
  c3->SetLogx();
  c3->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_stat_sel_nd       -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_2020a1_nd    -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_2019b3_nd    -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_sel_fd       -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_2020a1_fd    -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_2019b3_fd    -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_sel_nd_fd    -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_2020a1_nd_fd -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_2019b3_nd_fd -> Draw("L");

  TLegend * lg3 = new TLegend(0.15,0.15,0.82,0.52);
  lg3->SetFillStyle(0);
  lg3->SetBorderSize(0);
  lg3->SetTextFont(132);
  lg3->SetTextSize(0.035);

  lg3->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_sel_nd,       "5#sigma Reco. ND", "L");
  lg3->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_sel_fd,       "5#sigma Reco. FD", "L");
  lg3->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_sel_nd_fd,    "5#sigma Reco. ND+FD", "L");
  lg3->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_2020a1_nd,    "5#sigma MC v3 ND", "L");
  lg3->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_2020a1_fd,    "5#sigma MC v3 FD", "L");
  lg3->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_2020a1_nd_fd, "5#sigma MC v3 ND+FD", "L");
  lg3->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_2019b3_nd,    "5#sigma MC v2 ND", "L");
  lg3->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_2019b3_fd,    "5#sigma MC v2 FD", "L");
  lg3->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_2019b3_nd_fd, "5#sigma MC v2 ND+FD", "L");
  lg3->Draw("same");
  
  c3->SaveAs((location+"nd_fd_v2_v3_stat_reco_mc_comparison.root").c_str());
  c3->SaveAs((location+"nd_fd_v2_v3_stat_reco_mc_comparison.png").c_str());
  c3->SaveAs((location+"nd_fd_v2_v3_stat_reco_mc_comparison.pdf").c_str());

  // --------------------------------------------------------
  // Systematic groups
  // -------------------------------------------------------

  TCanvas * c4 = new TCanvas("c4","",20,20,900,900);
  c4->SetTopMargin(0.024);
  c4->SetBottomMargin(0.13);
  c4->SetLeftMargin(0.13);
  c4->SetRightMargin(0.024);
  hf1->Draw();
  c4->SetLogx();
  c4->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_stat          -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_flux     -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_prop     -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_mod      -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_xsec     -> Draw("L");
//  valor_modern_sbn_3p1_contour_5sig_syst_det      -> Draw("L");

  TLegend * lg4 = new TLegend(0.18,0.18,0.75,0.42);
  lg4->SetFillStyle(0);
  lg4->SetBorderSize(0);
  lg4->SetTextFont(132);

  lg4->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat,          "5#sigma Stat-Only",             "L");
  lg4->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_flux,     "5#sigma Flux Syst.", "L");
  lg4->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_prop,     "5#sigma Proposal Interaction Syst.", "L");
  lg4->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_mod,      "5#sigma Modern Interaction Syst.", "L");
  lg4->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_xsec,     "5#sigma Proposal+Modern Interaction Syst.", "L");
 // lg4->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_det,      "5#sigma Detector Syst.",        "L");
  lg4->Draw("same");
  
  c4->SaveAs((location+"systematic_group_comparison_nodet.root").c_str());
  c4->SaveAs((location+"systematic_group_comparison_nodet.png").c_str());
  c4->SaveAs((location+"systematic_group_comparison_nodet.pdf").c_str());
*/  
  
  // --------------------------------------------------------
  // Detectors
  // -------------------------------------------------------

  TCanvas * c5 = new TCanvas("c5","",20,20,900,900);
  c5->SetTopMargin(0.024);
  c5->SetBottomMargin(0.13);
  c5->SetLeftMargin(0.13);
  c5->SetRightMargin(0.024);
  hf1->Draw();
  c5->SetLogx();
  c5->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_stat_nd   -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_ub   -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_fd   -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_nd_fd-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_nd_ub-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_ub_fd-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_3det -> Draw("L");

  TLegend * lg5 = new TLegend(0.18,0.18,0.75,0.42);
  lg5->SetFillStyle(0);
  lg5->SetBorderSize(0);
  lg5->SetTextFont(132);

  lg5->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_nd,    "5#sigma SBND", "L");
  lg5->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_ub,    "5#sigma #muBooNE", "L");
  lg5->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_fd,    "5#sigma ICARUS", "L");
  lg5->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_nd_fd, "5#sigma SBND+ICARUS", "L");
  lg5->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_nd_ub, "5#sigma SBND+#muBooNE", "L");
  lg5->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_ub_fd, "5#sigma #muBooNE+ICARUS", "L");
  lg5->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_3det,  "5#sigma SBND+#muBooNE+ICARUS", "L");
  lg5->Draw("same");
  
  c5->SaveAs((location+"numu_disapp_detector_stat_comparison.root").c_str());
  c5->SaveAs((location+"numu_disapp_detector_stat_comparison.png").c_str());
  c5->SaveAs((location+"numu_disapp_detector_stat_comparison.pdf").c_str());
  
  // --------------------------------------------------------
  // Detectors
  // -------------------------------------------------------

  TCanvas * c5a = new TCanvas("c5a","",20,20,900,900);
  c5a->SetTopMargin(0.024);
  c5a->SetBottomMargin(0.13);
  c5a->SetLeftMargin(0.13);
  c5a->SetRightMargin(0.024);
  hf1->Draw();
  c5a->SetLogx();
  c5a->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_syst_nd   -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_ub   -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_fd   -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_nd_fd-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_nd_ub-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_ub_fd-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_3det -> Draw("L");

  TLegend * lg5a = new TLegend(0.18,0.18,0.85,0.42);
  lg5a->SetFillStyle(0);
  lg5a->SetBorderSize(0);
  lg5a->SetTextFont(132);

  lg5a->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_nd,    "5#sigma SBND", "L");
  lg5a->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_ub,    "5#sigma #muBooNE", "L");
  lg5a->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_fd,    "5#sigma ICARUS", "L");
  lg5a->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_nd_fd, "5#sigma SBND+ICARUS", "L");
  lg5a->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_nd_ub, "5#sigma SBND+#muBooNE", "L");
  lg5a->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_ub_fd, "5#sigma #muBooNE+ICARUS", "L");
  lg5a->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_3det,  "5#sigma SBND+#muBooNE+ICARUS", "L");
  lg5a->Draw("same");
  
  c5a->SaveAs((location+"numu_disapp_detector_syst_comparison.root").c_str());
  c5a->SaveAs((location+"numu_disapp_detector_syst_comparison.png").c_str());
  c5a->SaveAs((location+"numu_disapp_detector_syst_comparison.pdf").c_str());
/*
  // --------------------------------------------------------
  // Models
  // -------------------------------------------------------

  TCanvas * c6 = new TCanvas("c6","",20,20,900,900);
  c6->SetTopMargin(0.024);
  c6->SetBottomMargin(0.13);
  c6->SetLeftMargin(0.13);
  c6->SetRightMargin(0.024);
  hf1->Draw();
  c6->SetLogx();
  c6->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_stat_nd_fd-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_2019b3-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_susav2-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_smith-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_lse-> Draw("L");

  TLegend * lg6 = new TLegend(0.15,0.15,0.90,0.45);
  lg6->SetFillStyle(0);
  lg6->SetBorderSize(0);
  lg6->SetMargin(0.2);
  lg6->SetTextSize(0.035);
  lg6->SetTextFont(132);

  lg6->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_2019b3, "5#sigma GENIE v2_12_10 Default+MEC", "L");
  lg6->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_nd_fd,  "5#sigma GENIE v3 G18_10a, Valencia 1p1h+2p2h", "L");
  lg6->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_susav2, "5#sigma GENIE v3 G18_10a, SuSAv2 1p1h+2p2h", "L");
  lg6->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_smith,  "5#sigma GENIE v3 G18_10a, Smith-Moniz QE", "L");
  lg6->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_lse,    "5#sigma GENIE v3 G18_10a, LS-E QE+MEC", "L");
  lg6->Draw("same");
  
  c6->SaveAs((location+"model_stat_comparison.root").c_str());
  c6->SaveAs((location+"model_stat_comparison.png").c_str());
  c6->SaveAs((location+"model_stat_comparison.pdf").c_str());

  // --------------------------------------------------------
  // Model ratios
  // -------------------------------------------------------
  TCanvas * c7 = new TCanvas("c7","",20,20,900,900);
  c7->SetTopMargin(0.024);
  c7->SetBottomMargin(0.13);
  c7->SetLeftMargin(0.13);
  c7->SetRightMargin(0.024);
  // Reusable frame (in theory)
  TH1F * hfR = dynamic_cast<TH1F*>(c7->DrawFrame(0.8,1.2E-1,1.8,1E+2));
  hfR->Draw();
  hfR->GetYaxis()->SetTitleOffset(1);
  hfR->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}/sin^{2}2#theta_{#mu#mu, REF}");
  hfR->GetYaxis()->SetTitle("#Deltam^{2}_{41} (eV^{2})");
  hfR->GetXaxis()->SetTitleFont(132);
  hfR->GetYaxis()->SetTitleFont(132);
  hfR->GetXaxis()->SetLabelFont(132);
  hfR->GetYaxis()->SetLabelFont(132);
  hfR->GetXaxis()->SetTitleSize(0.06);
  hfR->GetYaxis()->SetTitleSize(0.06);
  hfR->GetXaxis()->SetLabelSize(0.05);
  hfR->GetYaxis()->SetLabelSize(0.05);
  c7->SetLogy();

  ratio_ref   . Draw("L");
  ratio_nd_fd . Draw("L");
  ratio_susav2. Draw("L");
  ratio_smith . Draw("L");
  ratio_lse   . Draw("L");

  TLegend * lg7 = new TLegend(0.55,0.35,0.9,0.75);
  lg7->SetFillStyle(0);
  lg7->SetBorderSize(0);
  lg7->SetTextSize(0.035);
  lg7->SetTextFont(132);

  lg7->AddEntry(&ratio_ref,    "v2_12_10 [REF]", "L");
  lg7->AddEntry(&ratio_nd_fd,  "v3, Valencia 1p1h+2p2h", "L");
  lg7->AddEntry(&ratio_susav2, "v3, SuSAv2 1p1h+2p2h", "L");
  lg7->AddEntry(&ratio_smith,  "v3, Smith-Moniz QE", "L");
  lg7->AddEntry(&ratio_lse,    "v3, LS-E QE+MEC", "L");
  lg7->Draw("same");
  
  c7->SaveAs((location+"model_stat_ratio_comparison.root").c_str());
  c7->SaveAs((location+"model_stat_ratio_comparison.png").c_str());
  c7->SaveAs((location+"model_stat_ratio_comparison.pdf").c_str());
*/
  // --------------------------------------------------------
  // Systematic group ratios
  // -------------------------------------------------------

  TCanvas * c8 = new TCanvas("c8","",20,20,1200,900);
  c8->SetTopMargin(0.024);
  c8->SetBottomMargin(0.13);
  c8->SetLeftMargin(0.13);
  c8->SetRightMargin(0.024);
  TH1F * hfR2 = dynamic_cast<TH1F*>(c8->DrawFrame(0.2,1.2E-1,32,1E+2));
  hfR2->Draw();
  hfR2->GetYaxis()->SetTitleOffset(1);
  hfR2->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}/sin^{2}2#theta_{#mu#mu, REF}");
  hfR2->GetYaxis()->SetTitle("#Deltam^{2}_{41} (eV^{2})");
  hfR2->GetXaxis()->SetTitleFont(132);
  hfR2->GetYaxis()->SetTitleFont(132);
  hfR2->GetXaxis()->SetLabelFont(132);
  hfR2->GetYaxis()->SetLabelFont(132);
  hfR2->GetXaxis()->SetTitleSize(0.06);
  hfR2->GetYaxis()->SetTitleSize(0.06);
  hfR2->GetXaxis()->SetLabelSize(0.05);
  hfR2->GetYaxis()->SetLabelSize(0.05);
  c8->SetLogy();

  ratio_stat_ref . Draw("L");
  ratio_flux     . Draw("L");
  ratio_prop     . Draw("L");
  //ratio_mod      . Draw("L");
  ratio_xsec     . Draw("L");
  //ratio_det      . Draw("L");

  TLegend * lg8 = new TLegend(0.33,0.18,0.9,0.62);
  lg8->SetFillStyle(0);
  lg8->SetBorderSize(0);
  lg8->SetTextFont(132);
  lg8->SetTextSize(0.035);

  lg8->AddEntry(&ratio_stat_ref, "5#sigma Stat-Only [REF]",            "L");
  lg8->AddEntry(&ratio_flux,     "5#sigma Flux Syst.",                 "L");
  lg8->AddEntry(&ratio_prop,     "5#sigma Proposal Interaction Syst.", "L");
  //lg8->AddEntry(&ratio_mod,      "5#sigma Modern Interaction Syst.",   "L");
  lg8->AddEntry(&ratio_xsec,      "5#sigma Proposal+Modern Interaction Syst.",   "L");
  //lg8->AddEntry(&ratio_det,      "5#sigma Detector Syst.",             "L");
  lg8->Draw("same");
  
  c8->SaveAs((location+"systematic_group_ratio_comparison_nodet_nomod.root").c_str());
  c8->SaveAs((location+"systematic_group_ratio_comparison_nodet_nomod.png").c_str());
  c8->SaveAs((location+"systematic_group_ratio_comparison_nodet_nomod.pdf").c_str());
  /*
  
  // --------------------------------------------------------
  // MC Reco Ratios
  // -------------------------------------------------------

  TCanvas * c9 = new TCanvas("c9","",20,20,900,900);
  c9->SetTopMargin(0.024);
  c9->SetBottomMargin(0.13);
  c9->SetLeftMargin(0.13);
  c9->SetRightMargin(0.024);
  c9->SetLogy();
  TH1F * hfR3 = dynamic_cast<TH1F*>(c9->DrawFrame(0,4E-1,2,1E+2));
  hfR3->Draw();
  hfR3->GetYaxis()->SetTitleOffset(1);
  hfR3->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu, MC}/sin^{2}2#theta_{#mu#mu, Reco}");
  hfR3->GetYaxis()->SetTitle("#Deltam^{2}_{41} (eV^{2})");
  hfR3->GetXaxis()->SetTitleFont(132);
  hfR3->GetYaxis()->SetTitleFont(132);
  hfR3->GetXaxis()->SetLabelFont(132);
  hfR3->GetYaxis()->SetLabelFont(132);
  hfR3->GetXaxis()->SetTitleSize(0.06);
  hfR3->GetYaxis()->SetTitleSize(0.06);
  hfR3->GetXaxis()->SetLabelSize(0.05);
  hfR3->GetYaxis()->SetLabelSize(0.05);

  ratio_mc_reco_nd    . Draw("L");
  ratio_mc_reco_fd    . Draw("L");
  ratio_mc_reco_nd_fd . Draw("L");
  ratio_mc_reco_v2_nd    . Draw("L");
  ratio_mc_reco_v2_fd    . Draw("L");
  ratio_mc_reco_v2_nd_fd . Draw("L");

  TLegend * lg9 = new TLegend(0.18,0.53,0.45,0.95);
  lg9->SetFillStyle(0);
  lg9->SetBorderSize(0);
  lg9->SetTextFont(132);
  lg9->SetTextSize(0.03);

  lg9->AddEntry(&ratio_mc_reco_nd,       "5#sigma ND, v3", "L");
  lg9->AddEntry(&ratio_mc_reco_fd,       "5#sigma FD, v3", "L");
  lg9->AddEntry(&ratio_mc_reco_nd_fd,    "5#sigma ND+FD, v3", "L");
  lg9->AddEntry(&ratio_mc_reco_v2_nd,    "5#sigma ND, v2", "L");
  lg9->AddEntry(&ratio_mc_reco_v2_fd,    "5#sigma FD, v2", "L");
  lg9->AddEntry(&ratio_mc_reco_v2_nd_fd, "5#sigma ND+FD, v2", "L");
  lg9->Draw("same");
  
  c9->SaveAs((location+"nd_fd_v2_v3_stat_reco_mc_ratio_comparison.root").c_str());
  c9->SaveAs((location+"nd_fd_v2_v3_stat_reco_mc_ratio_comparison.png").c_str());
  c9->SaveAs((location+"nd_fd_v2_v3_stat_reco_mc_ratio_comparison.pdf").c_str());

  // --------------------------------------------------------
  // Energy scale ratio comparison
  // -------------------------------------------------------

  TCanvas * c10 = new TCanvas("c10","",20,20,900,900);
  c10->SetTopMargin(0.024);
  c10->SetBottomMargin(0.13);
  c10->SetLeftMargin(0.13);
  c10->SetRightMargin(0.024);
  c10->Draw();
  c10->SetLogy();

  // Reusable frame (in theory)
  TH1F * hfR4 = dynamic_cast<TH1F*>(c10->DrawFrame(0.96,1E-1,1.125,24));
  hfR4->Draw();
  hfR4->GetYaxis()->SetTitleOffset(1);
  hfR4->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}/sin^{2}2#theta_{#mu#mu, REF}");
  hfR4->GetYaxis()->SetTitle("#Deltam^{2}_{41} (eV^{2})");
  hfR4->GetXaxis()->SetTitleFont(132);
  hfR4->GetYaxis()->SetTitleFont(132);
  hfR4->GetXaxis()->SetLabelFont(132);
  hfR4->GetYaxis()->SetLabelFont(132);
  hfR4->GetXaxis()->SetTitleSize(0.06);
  hfR4->GetYaxis()->SetTitleSize(0.06);
  hfR4->GetXaxis()->SetLabelSize(0.05);
  hfR4->GetYaxis()->SetLabelSize(0.05);
  
  ratio_stat_ref.Draw("L");
  ratio_escale_1pct.Draw("L");
  ratio_escale_2pct.Draw("L");
  ratio_escale_5pct.Draw("L");

  TLegend * lg10 = new TLegend(0.54,0.35,0.88,0.75);
  lg10->SetFillStyle(0);
  lg10->SetBorderSize(0);
  lg10->SetTextFont(132);
  lg10->SetTextSize(0.035);
  
  lg10->AddEntry(&ratio_stat_ref,    "5#sigma Stat-Only [REF]", "L");
  lg10->AddEntry(&ratio_escale_1pct, "5#sigma Stat+1\% Energy Scale", "L");
  lg10->AddEntry(&ratio_escale_2pct, "5#sigma Stat+2\% Energy Scale", "L");
  lg10->AddEntry(&ratio_escale_5pct, "5#sigma Stat+5\% Energy Scale", "L");
  lg10->Draw();
  
  c10->SaveAs((location+"energy_scale_ratio_comparison.root").c_str());
  c10->SaveAs((location+"energy_scale_ratio_comparison.png").c_str());
  c10->SaveAs((location+"energy_scale_ratio_comparison.pdf").c_str());

  // --------------------------------------------------------
  // Exclusive systematic groups
  // -------------------------------------------------------

  TCanvas * c11 = new TCanvas("c11","",20,20,900,900);
  c11->SetTopMargin(0.024);
  c11->SetBottomMargin(0.13);
  c11->SetLeftMargin(0.13);
  c11->SetRightMargin(0.024);
  hf1->Draw();
  c11->SetLogx();
  c11->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_stat               -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc      -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc      -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c3_bias_exc -> Draw("L");

  TLegend * lg11 = new TLegend(0.18,0.18,0.88,0.45);
  lg11->SetFillStyle(0);
  lg11->SetBorderSize(0);
  lg11->SetTextFont(132);
  lg11->SetTextSize(0.03);
  
  lg11->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat,               "5#sigma Stat-Only", "L");
  lg11->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc,      "5#sigma Exclusive Flux+Interaction Syst", "L");
  lg11->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc,      "5#sigma Exclusive Flux+Interaction+Detector Syst", "L");
  lg11->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c3_bias_exc, "5#sigma Exclusive Flux+Interaction+Bias Syst", "L");
  lg11->Draw("same");
  
  c11->SaveAs((location+"exclusive_syst_comparison.root").c_str());
  c11->SaveAs((location+"exclusive_syst_comparison.pdf").c_str());
  c11->SaveAs((location+"exclusive_syst_comparison.png").c_str());
  
  // --------------------------------------------------------
  // Reco Detectors stat+syst
  // -------------------------------------------------------

  TCanvas * c12 = new TCanvas("c12","",20,20,900,900);
  c12->SetTopMargin(0.024);
  c12->SetBottomMargin(0.13);
  c12->SetLeftMargin(0.13);
  c12->SetRightMargin(0.024);
  hf1->Draw();
  c12->SetLogx();
  c12->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_stat_2020a1_nd_fd      -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_2020a1_03c2_nd_fd -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_2_sel_nd_fd       -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_sel_03c2_nd_fd    -> Draw("L");

  TLegend * lg12 = new TLegend(0.15,0.15,0.9,0.47);
  lg12->SetFillStyle(0);
  lg12->SetBorderSize(0);
  lg12->SetTextFont(132);

  lg12->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_2020a1_nd_fd,      "5#sigma MC ND+FD Stat-Only", "L");
  lg12->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_2_sel_nd_fd,       "5#sigma Reco. ND+FD, Stat-only", "L");
  lg12->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_2020a1_03c2_nd_fd, "5#sigma MC ND+FD Interaction+Flux+Detector Syst", "L");
  lg12->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_sel_03c2_nd_fd,    "5#sigma Reco. ND+FD, Interaction+Flux+Detector Syst", "L");
  lg12->Draw("same");
  
  c12->SaveAs((location+"nd_fd_stat_syst_reco_comparison.root").c_str());
  c12->SaveAs((location+"nd_fd_stat_syst_reco_comparison.png").c_str());
  c12->SaveAs((location+"nd_fd_stat_syst_reco_comparison.pdf").c_str());

  // --------------------------------------------------------
  // Allowed 03c1 03c2
  // -------------------------------------------------------

  TCanvas * c13 = new TCanvas("c13","",20,20,900,900);
  c13->SetTopMargin(0.024);
  c13->SetBottomMargin(0.13);
  c13->SetLeftMargin(0.13);
  c13->SetRightMargin(0.024);
  hf1->Draw();
  c13->SetLogx();
  c13->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc_all -> Draw("LF");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all -> Draw("LF");
  valor_modern_sbn_3p1_contour_5sig_stat_all          -> Draw("LF");
  valor_modern_sbn_3p1_contour_5sig_stat_all_a        -> Draw("LF");
  valor_modern_sbn_3p1_contour_5sig_stat_all_b        -> Draw("LF");
  valor_modern_sbn_3p1_contour_5sig_stat_all_c        -> Draw("LF");
  valor_modern_sbn_3p1_contour_5sig_stat_all_d        -> Draw("LF");
  injectedBF                                          -> Draw("P");
  injecteda                                           -> Draw("P");
  injectedb                                           -> Draw("P");
  injectedc                                           -> Draw("P");
  injectedd                                           -> Draw("P");

  TLegend * lg13 = new TLegend(0.15,0.18,0.55,0.43);
  lg13->SetFillStyle(0);
  lg13->SetBorderSize(0);
  lg13->SetTextFont(132);
  lg13->SetTextSize(0.03);
  lg13->SetMargin(0.2);
  
  TLegend * lg13b = new TLegend(0.18,0.77,0.58,0.92);
  lg13b->SetFillStyle(0);
  lg13b->SetBorderSize(0);
  lg13b->SetTextFont(132);
  lg13b->SetTextSize(0.03);
  lg13b->SetMargin(0.2);
  
  lg13->AddEntry(injectedBF,                                          "sin^{2}2#theta_{#mu#mu} = 0.07, #Deltam_{41}^{2} = 1.32", "P");
  lg13->AddEntry(injecteda,                                           "sin^{2}2#theta_{#mu#mu} = 0.007, #Deltam_{41}^{2} = 1.32", "P");
  lg13->AddEntry(injectedb,                                           "sin^{2}2#theta_{#mu#mu} = 0.7, #Deltam_{41}^{2} = 1.32", "P");
  lg13->AddEntry(injectedc,                                           "sin^{2}2#theta_{#mu#mu} = 0.07, #Deltam_{41}^{2} = 13.2", "P");
  lg13->AddEntry(injectedd,                                           "sin^{2}2#theta_{#mu#mu} = 0.07, #Deltam_{41}^{2} = 0.132", "P");
  lg13b->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc_all, "5#sigma Flux+Interaction+Detector Syst", "L");
  lg13b->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all, "5#sigma Flux+Interaction Syst", "L");
  lg13b->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,          "5#sigma Stat-Only", "L");
  lg13->Draw("same");
  lg13b->Draw("same");
  
  c13->SaveAs((location+"allowed_multiple_03c1_03c2_syst_comparison.root").c_str());
  c13->SaveAs((location+"allowed_multiple_03c1_03c2_syst_comparison.pdf").c_str());
  c13->SaveAs((location+"allowed_multiple_03c1_03c2_syst_comparison.png").c_str());

  // --------------------------------------------------------
  // Allowed 03c1 03c1+bias
  // -------------------------------------------------------
  
  TCanvas * c14 = new TCanvas("c14","",20,20,900,900);
  c14->SetTopMargin(0.024);
  lg1->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,      "5#sigma SBN Stat-Only", "F");
  c14->SetBottomMargin(0.13);
  c14->SetLeftMargin(0.13);
  c14->SetRightMargin(0.024);
  hf1->Draw();
  c14->SetLogx();
  c14->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all_bias -> Draw("LF");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all      -> Draw("LF");
  valor_modern_sbn_3p1_contour_5sig_stat_all               -> Draw("LF");
  injected                                                 -> Draw("P");

  TLegend * lg14 = new TLegend(0.18,0.18,0.85,0.55);
  lg14->SetFillStyle(0);
  lg14->SetBorderSize(0);
  lg14->SetTextFont(132);
  lg14->SetTextSize(0.035);
  
  lg14->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,               "5#sigma Stat-Only", "L");
  lg14->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all,      "5#sigma Flux+Interaction Syst", "L");
  lg14->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all_bias, "5#sigma Flux+Interaction+Bias Syst", "L");
  lg14->AddEntry(injected,                                                 "Injected Point: sin^{2}2#theta_{#mu#mu} = 0.07, #Deltam_{41}^{2} = 1.32", "P");
  lg14->Draw("same");
  
  c14->SaveAs((location+"allowed_03c1_bias_syst_comparison.root").c_str());
  c14->SaveAs((location+"allowed_03c1_bias_syst_comparison.pdf").c_str());
  c14->SaveAs((location+"allowed_03c1_bias_syst_comparison.png").c_str());
  */

  // --------------------------------------------------------
  // Allowed 03c1
  // -------------------------------------------------------
  
  TCanvas * c14 = new TCanvas("c14","",20,20,900,900);
  c14->SetTopMargin(0.024);
  c14->SetBottomMargin(0.13);
  c14->SetLeftMargin(0.13);
  c14->SetRightMargin(0.024);
  hf1->Draw();
  c14->SetLogx();
  c14->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_syst_03c1_all      -> Draw("LF");
  valor_modern_sbn_3p1_contour_5sig_stat_allW          -> Draw("F");
  valor_modern_sbn_3p1_contour_5sig_stat_all           -> Draw("LF");
  injectedBF                                           -> Draw("P");

  TLegend * lg14 = new TLegend(0.18,0.18,0.85,0.55);
  lg14->SetFillStyle(0);
  lg14->SetBorderSize(0);
  lg14->SetTextFont(132);
  lg14->SetTextSize(0.035);
  
  lg14->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,           "5#sigma Stat", "L");
  lg14->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_all,      "5#sigma Stat+Syst", "L");
  lg14->AddEntry(injectedBF,                                           "Injected Point: sin^{2}2#theta_{#mu#mu} = 0.07, #Deltam_{41}^{2} = 1.32 eV^{2}", "P");
  lg14->Draw("same");
  
  c14->SaveAs((location+"numu_disapp_allowed_stat_03c1_syst_comparison.root").c_str());
  c14->SaveAs((location+"numu_disapp_allowed_stat_03c1_syst_comparison.pdf").c_str());
  c14->SaveAs((location+"numu_disapp_allowed_stat_03c1_syst_comparison.png").c_str());
  
  // --------------------------------------------------------
  // All, TN sensitivity
  // -------------------------------------------------------

  TCanvas * c15 = new TCanvas("c15","",20,20,945,900);
  c15->SetTopMargin(0.05);
  c15->SetBottomMargin(0.13);
  c15->SetLeftMargin(0.13);
  c15->SetRightMargin(0.024);
  TH1F * hf2 = dynamic_cast<TH1F*>(c15->DrawFrame(1.5E-4,1E-2,1,100));
  hf2->Draw();
  hf2->GetYaxis()->SetTitleOffset(1);
  hf2->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hf2->GetYaxis()->SetTitle("#Deltam^{2}_{41} (eV^{2})");
  hf2->GetXaxis()->SetTitleFont(132);
  hf2->GetYaxis()->SetTitleFont(132);
  hf2->GetXaxis()->SetLabelFont(132);
  hf2->GetYaxis()->SetLabelFont(132);
  hf2->GetXaxis()->SetTitleSize(0.06);
  hf2->GetYaxis()->SetTitleSize(0.06);
  hf2->GetXaxis()->SetLabelSize(0.05);
  hf2->GetYaxis()->SetLabelSize(0.05);
  
  c15->SetLogx();
  c15->SetLogy();

  lim_numudis_3p1_icecube_90pctCL                     -> Draw("L");
  lim_numudis_3p1_icecube_99pctCL                     -> Draw("L");
  lim_numudis_3p1_minisciboone_90pctCL                -> Draw("L");
  lim_numudis_3p1_minosp_90pctCL                      -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1         -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_allW    -> Draw("F");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_all     -> Draw("FL");
  valor_modern_sbn_3p1_contour_90pct_syst_03d1        -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_allW         -> Draw("F");
  valor_modern_sbn_3p1_contour_5sig_stat_all          -> Draw("FL");
  injectedBF                                          -> Draw("P");

  TLegend * lg15 = new TLegend(0.16,0.16,0.5,0.48);
  lg15->SetFillStyle(0);
  lg15->SetBorderSize(0);
  lg15->SetMargin(0.22);
  lg15->SetTextFont(132);
  lg15->SetTextSize(0.028);

  TLegend * lg15b = new TLegend(0.14,0.72,0.56,0.88);
  lg15b->SetFillStyle(0);
  lg15b->SetBorderSize(0);
  lg15b->SetMargin(0.18);
  lg15b->SetTextFont(132);
  lg15b->SetTextSize(0.028);
  
  lg15->AddEntry(lim_numudis_3p1_icecube_90pctCL,                     "90\% IceCube", "L");
  lg15->AddEntry(lim_numudis_3p1_icecube_99pctCL,                     "99\% IceCube", "L");
  lg15->AddEntry(lim_numudis_3p1_minisciboone_90pctCL,                "90\% MiniBooNE/SciBooNE", "L");
  lg15->AddEntry(lim_numudis_3p1_minosp_90pctCL,                      "90\% MINOS/MINOS+", "L");
  lg15->AddEntry(injectedW,                                           " ", "P");
  lg15->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_03d1,        "90\% SBN Stat+Syst", "L");
  lg15->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1,         "5#sigma SBN Stat+Syst", "L");
  lg15->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_all,     "5#sigma SBN Stat+Syst", "F");
  lg15->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,          "5#sigma SBN Stat-Only", "F");
  lg15b->AddEntry(injectedW,                                          "Injected Point,", "P");
  lg15b->AddEntry(injectedBF,                                         "sin^{2}2#theta_{#mu#mu} = 0.07,", "P");
  lg15b->AddEntry(injectedW,                                          "#Deltam_{41}^{2} = 1.32 eV^{2}", "P");
  lg15->Draw("same");
  lg15b->Draw("same");
  
  TLatex latex1;
  latex1.SetTextFont(132);
  latex1.SetTextSize(0.026);
  latex1.DrawLatexNDC(0.13,0.96,"#color[2]{SBN Preliminary}");
  latex1.SetTextSize(0.026);
  latex1.DrawLatexNDC(0.30,0.96,"#color[1]{SBND (6.6e20 POT) MicroBooNE (13.2e20 POT) ICARUS (6.6e20 POT)}");
  
  c15->SaveAs((location+"numu_disapp_full_03c1_sensitivity_comparison.C").c_str());
  c15->SaveAs((location+"numu_disapp_full_03c1_sensitivity_comparison.root").c_str());
  c15->SaveAs((location+"numu_disapp_full_03c1_sensitivity_comparison.pdf").c_str());
  c15->SaveAs((location+"numu_disapp_full_03c1_sensitivity_comparison.png").c_str());
  
  // --------------------------------------------------------
  // Proposal vs modern
  // -------------------------------------------------------

  TCanvas * c16 = new TCanvas("c16","",20,20,1000,900);
  c16->SetTopMargin(0.024);
  c16->SetBottomMargin(0.13);
  c16->SetLeftMargin(0.13);
  c16->SetRightMargin(0.024);
  hf2->Draw();
  c16->SetLogx();
  c16->SetLogy();
  
  lim_numudis_3p1_sbnproposal_5sigCL          -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1 -> Draw("L");

  TLegend * lg16 = new TLegend(0.18,0.16,0.6,0.4);
  lg16->SetFillStyle(0);
  lg16->SetBorderSize(0);
  lg16->SetTextFont(132);
  lg16->SetTextSize(0.032);

  lg16->AddEntry(lim_numudis_3p1_sbnproposal_5sigCL,          "5#sigma Proposal Stat+Syst", "L");
  lg16->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1, "5#sigma Modern Stat+Syst", "L");
  lg16->Draw("same");
  
  c16->SaveAs((location+"numu_disapp_prop_mod_03c1_sensitivity_comparison.C").c_str());
  c16->SaveAs((location+"numu_disapp_prop_mod_03c1_sensitivity_comparison.root").c_str());
  c16->SaveAs((location+"numu_disapp_prop_mod_03c1_sensitivity_comparison.pdf").c_str());
  c16->SaveAs((location+"numu_disapp_prop_mod_03c1_sensitivity_comparison.png").c_str());
  
  // --------------------------------------------------------
  // All SBN
  // -------------------------------------------------------

  TCanvas * c17 = new TCanvas("c17","",20,20,945,900);
  c17->SetTopMargin(0.05);
  c17->SetBottomMargin(0.13);
  c17->SetLeftMargin(0.13);
  c17->SetRightMargin(0.024);
  hf2->Draw();
  
  c17->SetLogx();
  c17->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_syst_03d1         -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_allW    -> Draw("F");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_all     -> Draw("FL");
  valor_modern_sbn_3p1_contour_90pct_syst_03d1        -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_allW         -> Draw("F");
  valor_modern_sbn_3p1_contour_5sig_stat_all          -> Draw("FL");
  injectedBF                                          -> Draw("P");

  TLegend * lg17 = new TLegend(0.16,0.16,0.5,0.4);
  lg17->SetFillStyle(0);
  lg17->SetBorderSize(0);
  lg17->SetMargin(0.22);
  lg17->SetTextFont(132);
  lg17->SetTextSize(0.028);

  TLegend * lg17b = new TLegend(0.14,0.72,0.56,0.88);
  lg17b->SetFillStyle(0);
  lg17b->SetBorderSize(0);
  lg17b->SetMargin(0.18);
  lg17b->SetTextFont(132);
  lg17b->SetTextSize(0.028);
  
  lg17->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_03d1,        "90\% SBN Stat+Syst", "L");
  lg17->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1,         "5#sigma SBN Stat+Syst", "L");
  lg17->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_all,     "5#sigma SBN Stat+Syst", "F");
  lg17->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,          "5#sigma SBN Stat-Only", "F");
  lg17b->AddEntry(injectedW,                                          "Injected Point,", "P");
  lg17b->AddEntry(injectedBF,                                         "sin^{2}2#theta_{#mu#mu} = 0.07,", "P");
  lg17b->AddEntry(injectedW,                                          "#Deltam_{41}^{2} = 1.32 eV^{2}", "P");
  lg17->Draw("same");
  lg17b->Draw("same");
  
  latex1.SetTextFont(132);
  latex1.SetTextSize(0.026);
  latex1.DrawLatexNDC(0.13,0.96,"#color[2]{SBN Preliminary}");
  latex1.SetTextSize(0.026);
  latex1.DrawLatexNDC(0.30,0.96,"#color[1]{SBND (6.6e20 POT) MicroBooNE (13.2e20 POT) ICARUS (6.6e20 POT)}");
  
  c17->SaveAs((location+"numu_disapp_sbn_03c1_sensitivity_comparison.C").c_str());
  c17->SaveAs((location+"numu_disapp_sbn_03c1_sensitivity_comparison.root").c_str());
  c17->SaveAs((location+"numu_disapp_sbn_03c1_sensitivity_comparison.pdf").c_str());
  c17->SaveAs((location+"numu_disapp_sbn_03c1_sensitivity_comparison.png").c_str());
  
  // --------------------------------------------------------
  // All, TN sensitivity + full SBN new POT
  // -------------------------------------------------------

  TCanvas * c18 = new TCanvas("c18","",20,20,945,900);
  c18->SetTopMargin(0.05);
  c18->SetBottomMargin(0.13);
  c18->SetLeftMargin(0.13);
  c18->SetRightMargin(0.024);
  
  hf2->Draw();
  
  c18->SetLogx();
  c18->SetLogy();

  lim_numudis_3p1_icecube_90pctCL                     -> Draw("L");
  lim_numudis_3p1_icecube_99pctCL                     -> Draw("L");
  lim_numudis_3p1_minisciboone_90pctCL                -> Draw("L");
  lim_numudis_3p1_minosp_90pctCL                      -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1         -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_allW    -> Draw("F");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_all     -> Draw("FL");
  valor_modern_sbn_3p1_contour_90pct_syst_03d1        -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_allW         -> Draw("F");
  valor_modern_sbn_3p1_contour_5sig_stat_all          -> Draw("FL");
  valor_modern_sbn_3p1_contour_90pct_syst_sbn_newPOT  -> Draw("L");
  injectedBF                                          -> Draw("P");

  TLegend * lg18 = new TLegend(0.16,0.16,0.5,0.44);
  lg18->SetFillStyle(0);
  lg18->SetBorderSize(0);
  lg18->SetMargin(0.22);
  lg18->SetTextFont(132);
  lg18->SetTextSize(0.026);

  TLegend * lg18b = new TLegend(0.16,0.66,0.56,0.93);
  lg18b->SetFillStyle(0);
  lg18b->SetBorderSize(0);
  lg18b->SetMargin(0.18);
  lg18b->SetTextFont(132);
  lg18b->SetTextSize(0.026);
  
  lg18->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_all,     "5#sigma SBN Stat+Syst", "F");
  lg18->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,          "5#sigma SBN Stat-Only", "F");
  lg18->AddEntry(injectedW,                                           " ", "P");
  lg18->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1,         "5#sigma SBN Stat+Syst", "L");
  lg18->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_03d1,        "90\% SBN Stat+Syst", "L");
  lg18->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_sbn_newPOT,  "90\% SBN, Increased POT Projection:", "L");
  lg18->AddEntry(injectedW,                                           "SBND (10e20) MicroBooNE (13.2e20) ICARUS (14e20)", "P");
  
  lg18b->AddEntry(injectedW,                                          "Injected Point,", "P");
  lg18b->AddEntry(injectedBF,                                         "sin^{2}2#theta_{#mu#mu} = 0.07,", "P");
  lg18b->AddEntry(injectedW,                                          "#Deltam_{41}^{2} = 1.32 eV^{2}", "P");
  lg18b->AddEntry(injectedW,                                           " ", "P");
  lg18b->AddEntry(lim_numudis_3p1_minosp_90pctCL,                     "90\% MINOS/MINOS+", "L");
  lg18b->AddEntry(lim_numudis_3p1_minisciboone_90pctCL,               "90\% MiniBooNE/SciBooNE", "L");
  lg18b->AddEntry(lim_numudis_3p1_icecube_90pctCL,                    "90\% IceCube", "L");
  lg18b->AddEntry(lim_numudis_3p1_icecube_99pctCL,                    "99\% IceCube", "L");
  lg18->Draw("same");
  lg18b->Draw("same");
  
  TLatex latex18;
  latex18.SetTextFont(132);
  latex18.SetTextSize(0.026);
  latex18.DrawLatexNDC(0.13,0.96,"#color[2]{SBN Preliminary}");
  latex18.SetTextSize(0.026);
  latex18.DrawLatexNDC(0.30,0.96,"#color[1]{SBND (6.6e20 POT) MicroBooNE (13.2e20 POT) ICARUS (6.6e20 POT)}");
  
  c18->SaveAs((location+"numu_disapp_full_newPOT_sensitivity_comparison.C").c_str());
  c18->SaveAs((location+"numu_disapp_full_newPOT_sensitivity_comparison.root").c_str());
  c18->SaveAs((location+"numu_disapp_full_newPOT_sensitivity_comparison.pdf").c_str());
  c18->SaveAs((location+"numu_disapp_full_newPOT_sensitivity_comparison.png").c_str());
  
  // --------------------------------------------------------
  // All, TN sensitivity + SBND+ICARUS new POT
  // -------------------------------------------------------

  TCanvas * c19 = new TCanvas("c19","",20,20,945,900);
  c19->SetTopMargin(0.05);
  c19->SetBottomMargin(0.13);
  c19->SetLeftMargin(0.13);
  c19->SetRightMargin(0.024);
  
  hf2->Draw();
  
  c19->SetLogx();
  c19->SetLogy();

  lim_numudis_3p1_icecube_90pctCL                     -> Draw("L");
  lim_numudis_3p1_icecube_99pctCL                     -> Draw("L");
  lim_numudis_3p1_minisciboone_90pctCL                -> Draw("L");
  lim_numudis_3p1_minosp_90pctCL                      -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03d1         -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_allW    -> Draw("F");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_all     -> Draw("FL");
  valor_modern_sbn_3p1_contour_90pct_syst_03d1        -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_allW         -> Draw("F");
  valor_modern_sbn_3p1_contour_5sig_stat_all          -> Draw("FL");
  valor_modern_sbn_3p1_contour_90pct_syst_2det_newPOT -> Draw("L");
  injectedBF                                          -> Draw("P");

  TLegend * lg19 = new TLegend(0.16,0.16,0.5,0.44);
  lg19->SetFillStyle(0);
  lg19->SetBorderSize(0);
  lg19->SetMargin(0.22);
  lg19->SetTextFont(132);
  lg19->SetTextSize(0.026);

  TLegend * lg19b = new TLegend(0.16,0.66,0.56,0.93);
  lg19b->SetFillStyle(0);
  lg19b->SetBorderSize(0);
  lg19b->SetMargin(0.18);
  lg19b->SetTextFont(132);
  lg19b->SetTextSize(0.026);
  
  lg19->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_all,     "5#sigma SBN Stat+Syst", "F");
  lg19->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,          "5#sigma SBN Stat-Only", "F");
  lg19->AddEntry(injectedW,                                           " ", "P");
  lg19->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03d1,         "5#sigma SBN Stat+Syst", "L");
  lg19->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_03d1,        "90\% SBN Stat+Syst", "L");
  lg19->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_2det_newPOT, "90\% SBN, Increased POT Projection:", "L");
  lg19->AddEntry(injectedW,                                           "SBND (10e20) ICARUS (14e20) Only", "P");
  
  lg19b->AddEntry(injectedW,                                          "Injected Point,", "P");
  lg19b->AddEntry(injectedBF,                                         "sin^{2}2#theta_{#mu#mu} = 0.07,", "P");
  lg19b->AddEntry(injectedW,                                          "#Deltam_{41}^{2} = 1.32 eV^{2}", "P");
  lg19b->AddEntry(injectedW,                                           " ", "P");
  lg19b->AddEntry(lim_numudis_3p1_minosp_90pctCL,                     "90\% MINOS/MINOS+", "L");
  lg19b->AddEntry(lim_numudis_3p1_minisciboone_90pctCL,               "90\% MiniBooNE/SciBooNE", "L");
  lg19b->AddEntry(lim_numudis_3p1_icecube_90pctCL,                    "90\% IceCube", "L");
  lg19b->AddEntry(lim_numudis_3p1_icecube_99pctCL,                    "99\% IceCube", "L");
  lg19->Draw("same");
  lg19b->Draw("same");
  
  TLatex latex19;
  latex19.SetTextFont(132);
  latex19.SetTextSize(0.026);
  latex19.DrawLatexNDC(0.13,0.96,"#color[2]{SBN Preliminary}");
  latex19.SetTextSize(0.026);
  latex19.DrawLatexNDC(0.30,0.96,"#color[1]{SBND (6.6e20 POT) MicroBooNE (13.2e20 POT) ICARUS (6.6e20 POT)}");
  
  c19->SaveAs((location+"numu_disapp_2det_newPOT_sensitivity_comparison.C").c_str());
  c19->SaveAs((location+"numu_disapp_2det_newPOT_sensitivity_comparison.root").c_str());
  c19->SaveAs((location+"numu_disapp_2det_newPOT_sensitivity_comparison.pdf").c_str());
  c19->SaveAs((location+"numu_disapp_2det_newPOT_sensitivity_comparison.png").c_str());
  
  // --------------------------------------------------------
  // All, TN sensitivity + full SBN new POT
  // -------------------------------------------------------

  TCanvas * c20 = new TCanvas("c20","",20,20,945,900);
  c20->SetTopMargin(0.05);
  c20->SetBottomMargin(0.13);
  c20->SetLeftMargin(0.13);
  c20->SetRightMargin(0.024);
  
  hf2->Draw();
  
  c20->SetLogx();
  c20->SetLogy();

  lim_numudis_3p1_icecube_90pctCL                        -> Draw("L");
  lim_numudis_3p1_icecube_99pctCL                        -> Draw("L");
  lim_numudis_3p1_minisciboone_90pctCL                   -> Draw("L");
  lim_numudis_3p1_minosp_90pctCL                         -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_sbn_newPOT      -> Draw("L");
  valor_modern_sbn_3p1_contour_90pct_syst_sbn_newPOT     -> Draw("L");

  TLegend * lg20 = new TLegend(0.16,0.16,0.5,0.44);
  lg20->SetFillStyle(0);
  lg20->SetBorderSize(0);
  lg20->SetMargin(0.22);
  lg20->SetTextFont(132);
  lg20->SetTextSize(0.026);

  TLegend * lg20b = new TLegend(0.16,0.66,0.56,0.93);
  lg20b->SetFillStyle(0);
  lg20b->SetBorderSize(0);
  lg20b->SetMargin(0.20);
  lg20b->SetTextFont(132);
  lg20b->SetTextSize(0.026);
  
  lg20->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_sbn_newPOT,  "5#sigma SBN Stat+Syst", "L");
  lg20->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_sbn_newPOT, "90\% SBN Stat+Syst", "L");
  
  lg20b->AddEntry(lim_numudis_3p1_minosp_90pctCL,                     "90\% MINOS/MINOS+", "L");
  lg20b->AddEntry(lim_numudis_3p1_minisciboone_90pctCL,               "90\% MiniBooNE/SciBooNE", "L");
  lg20b->AddEntry(lim_numudis_3p1_icecube_90pctCL,                    "90\% IceCube", "L");
  lg20b->AddEntry(lim_numudis_3p1_icecube_99pctCL,                    "99\% IceCube", "L");
  lg20->Draw("same");
  lg20b->Draw("same");
  
  TLatex latex20;
  latex20.SetTextFont(132);
  latex20.SetTextSize(0.026);
  latex20.DrawLatexNDC(0.13,0.96,"#color[2]{SBN Preliminary}");
  latex20.SetTextSize(0.026);
  latex20.DrawLatexNDC(0.30,0.96,"#color[1]{SBND (10e20 POT) MicroBooNE (13.2e20 POT) ICARUS (14e20 POT)}");
  
  c20->SaveAs((location+"numu_disapp_full_newPOT_sensitivity.C").c_str());
  c20->SaveAs((location+"numu_disapp_full_newPOT_sensitivity.root").c_str());
  c20->SaveAs((location+"numu_disapp_full_newPOT_sensitivity.pdf").c_str());
  c20->SaveAs((location+"numu_disapp_full_newPOT_sensitivity.png").c_str());
  
  // --------------------------------------------------------
  // All, TN sensitivity + SBND+ICARUS new POT
  // -------------------------------------------------------

  TCanvas * c21 = new TCanvas("c21","",20,20,945,900);
  c21->SetTopMargin(0.05);
  c21->SetBottomMargin(0.13);
  c21->SetLeftMargin(0.13);
  c21->SetRightMargin(0.024);
  
  hf2->Draw();
  
  c21->SetLogx();
  c21->SetLogy();

  lim_numudis_3p1_icecube_90pctCL                     -> Draw("L");
  lim_numudis_3p1_icecube_99pctCL                     -> Draw("L");
  lim_numudis_3p1_minisciboone_90pctCL                -> Draw("L");
  lim_numudis_3p1_minosp_90pctCL                      -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_2det_newPOT  -> Draw("L");
  valor_modern_sbn_3p1_contour_90pct_syst_2det_newPOT -> Draw("L");

  TLegend * lg21 = new TLegend(0.16,0.16,0.5,0.44);
  lg21->SetFillStyle(0);
  lg21->SetBorderSize(0);
  lg21->SetMargin(0.22);
  lg21->SetTextFont(132);
  lg21->SetTextSize(0.026);

  TLegend * lg21b = new TLegend(0.16,0.66,0.56,0.93);
  lg21b->SetFillStyle(0);
  lg21b->SetBorderSize(0);
  lg21b->SetMargin(0.18);
  lg21b->SetTextFont(132);
  lg21b->SetTextSize(0.026);
  
  lg21->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_2det_newPOT,  "5#sigma SBN Stat+Syst", "L");
  lg21->AddEntry(valor_modern_sbn_3p1_contour_90pct_syst_2det_newPOT, "90\% SBN Stat+Syst", "L");
  
  lg21b->AddEntry(lim_numudis_3p1_minosp_90pctCL,                     "90\% MINOS/MINOS+", "L");
  lg21b->AddEntry(lim_numudis_3p1_minisciboone_90pctCL,               "90\% MiniBooNE/SciBooNE", "L");
  lg21b->AddEntry(lim_numudis_3p1_icecube_90pctCL,                    "90\% IceCube", "L");
  lg21b->AddEntry(lim_numudis_3p1_icecube_99pctCL,                    "99\% IceCube", "L");
  lg21->Draw("same");
  lg21b->Draw("same");
  
  TLatex latex21;
  latex21.SetTextFont(132);
  latex21.SetTextSize(0.026);
  latex21.DrawLatexNDC(0.13,0.96,"#color[2]{SBN Preliminary}");
  latex21.SetTextSize(0.026);
  latex21.DrawLatexNDC(0.60,0.96,"#color[1]{SBND (10e20 POT) ICARUS (14e20 POT)}");
  
  c21->SaveAs((location+"numu_disapp_2det_newPOT_sensitivity.C").c_str());
  c21->SaveAs((location+"numu_disapp_2det_newPOT_sensitivity.root").c_str());
  c21->SaveAs((location+"numu_disapp_2det_newPOT_sensitivity.pdf").c_str());
  c21->SaveAs((location+"numu_disapp_2det_newPOT_sensitivity.png").c_str());
 /* 
  // --------------------------------------------------------
  // All, final sensitivity
  // -------------------------------------------------------

  TCanvas * c15 = new TCanvas("c15","",20,20,900,900);
  c15->SetTopMargin(0.024);
  c15->SetBottomMargin(0.13);
  c15->SetLeftMargin(0.13);
  c15->SetRightMargin(0.024);
  TH1F * hf2 = dynamic_cast<TH1F*>(c15->DrawFrame(1.1E-4,1E-2,1,100));
  hf2->Draw();
  hf2->GetYaxis()->SetTitleOffset(1);
  hf2->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hf2->GetYaxis()->SetTitle("#Deltam^{2}_{41} (eV^{2})");
  hf2->GetXaxis()->SetTitleFont(132);
  hf2->GetYaxis()->SetTitleFont(132);
  hf2->GetXaxis()->SetLabelFont(132);
  hf2->GetYaxis()->SetLabelFont(132);
  hf2->GetXaxis()->SetTitleSize(0.06);
  hf2->GetYaxis()->SetTitleSize(0.06);
  hf2->GetXaxis()->SetLabelSize(0.05);
  hf2->GetYaxis()->SetLabelSize(0.05);
  
  c15->SetLogx();
  c15->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc->SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_stat->SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_stat_all->SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc_all->SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_stat_all->SetFillStyle(3001);
  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc_all->SetFillStyle(3001);

  lim_numudis_3p1_icecube_90pctCL                     -> Draw("L");
  lim_numudis_3p1_icecube_99pctCL                     -> Draw("L");
  lim_numudis_3p1_minisciboone_90pctCL                -> Draw("L");
  lim_numudis_3p1_minosp_90pctCL                      -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc     -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc_all -> Draw("LF");
  valor_modern_sbn_3p1_contour_5sig_stat_all          -> Draw("LF");
  valor_modern_sbn_3p1_contour_5sig_stat              -> Draw("L");
  injectedBF                                          -> Draw("P");

  TLegend * lg15 = new TLegend(0.18,0.16,0.5,0.48);
  lg15->SetFillStyle(0);
  lg15->SetBorderSize(0);
  lg15->SetMargin(0.22);
  lg15->SetTextFont(132);
  lg15->SetTextSize(0.028);

  TLegend * lg15b = new TLegend(0.18,0.75,0.66,0.90);
  lg15b->SetFillStyle(0);
  lg15b->SetBorderSize(0);
  lg15b->SetMargin(0.18);
  lg15b->SetTextFont(132);
  lg15b->SetTextSize(0.028);
  
  lg15->AddEntry(lim_numudis_3p1_icecube_90pctCL,                     "90\% IceCube", "L");
  lg15->AddEntry(lim_numudis_3p1_icecube_99pctCL,                     "99\% IceCube", "L");
  lg15->AddEntry(lim_numudis_3p1_minisciboone_90pctCL,                "90\% MiniBooNE", "L");
  lg15->AddEntry(lim_numudis_3p1_minosp_90pctCL,                      "90\% MINOS/MINOS+", "L");
  lg15->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,          "5#sigma SBN Stat-Only", "L");
  lg15->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc_all, "5#sigma SBN Stat+Syst", "L");
  lg15b->AddEntry(injectedW,                                           "Injected Point,", "P");
  lg15b->AddEntry(injectedBF,                                          "sin^{2}2#theta_{#mu#mu} = 0.07,", "P");
  lg15b->AddEntry(injectedW,                                           "#Deltam_{41}^{2} = 1.32", "P");
  lg15->Draw("same");
  lg15b->Draw("same");
  
  c15->SaveAs((location+"full_nobias_sensitivity_comparison.root").c_str());
  c15->SaveAs((location+"full_nobias_sensitivity_comparison.pdf").c_str());
  c15->SaveAs((location+"full_nobias_sensitivity_comparison.png").c_str());
  
  // --------------------------------------------------------
  // Exclusive systematic groups
  // -------------------------------------------------------

  TCanvas * c16 = new TCanvas("c16","",20,20,900,900);
  c16->SetTopMargin(0.024);
  c16->SetBottomMargin(0.13);
  c16->SetLeftMargin(0.13);
  c16->SetRightMargin(0.024);
  hf1->Draw();
  c16->SetLogx();
  c16->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_stat          -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c3_exc -> Draw("L");

  TLegend * lg16 = new TLegend(0.18,0.18,0.88,0.45);
  lg16->SetFillStyle(0);
  lg16->SetBorderSize(0);
  lg16->SetTextFont(132);
  lg16->SetTextSize(0.03);
  
  lg16->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat,               "5#sigma Stat-Only", "L");
  lg16->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc,      "5#sigma Exclusive Flux+Interaction Syst", "L");
  lg16->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc,      "5#sigma Exclusive Flux+Interaction+Detector Syst", "L");
  lg16->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c3_exc,      "5#sigma Exclusive Flux+Interaction+Detector+Bias Syst", "L");
  lg16->Draw("same");
  
  c16->SaveAs((location+"exclusive_03c1_03c2_03c3_syst_comparison.root").c_str());
  c16->SaveAs((location+"exclusive_03c1_03c2_03c3_syst_comparison.pdf").c_str());
  c16->SaveAs((location+"exclusive_03c1_03c2_03c3_syst_comparison.png").c_str());
  
  // --------------------------------------------------------
  // Allowed 03c1 03c2 03c3
  // -------------------------------------------------------

  TCanvas * c17 = new TCanvas("c17","",20,20,900,900);
  c17->SetTopMargin(0.024);
  c17->SetBottomMargin(0.13);
  c17->SetLeftMargin(0.13);
  c17->SetRightMargin(0.024);
  hf1->Draw();
  c17->SetLogx();
  c17->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_syst_03c3_exc_all  -> Draw("LF");
  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc_all  -> Draw("LF");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all  -> Draw("LF");
  valor_modern_sbn_3p1_contour_5sig_stat_all           -> Draw("LF");
  injected                                             -> Draw("P");

  TLegend * lg17 = new TLegend(0.18,0.18,0.85,0.55);
  lg17->SetFillStyle(0);
  lg17->SetBorderSize(0);
  lg17->SetTextFont(132);
  lg17->SetTextSize(0.035);
  
  lg17->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,          "5#sigma Stat-Only", "L");
  lg17->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all, "5#sigma Flux+Interaction Syst", "L");
  lg17->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc_all, "5#sigma Flux+Interaction+Detector Syst", "L");
  lg17->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c3_exc_all, "5#sigma Flux+Interaction+Detector+Bias Syst", "L");
  lg17->AddEntry(injected,                                            "Injected Point: sin^{2}2#theta_{#mu#mu} = 0.07, #Deltam_{41}^{2} = 1.32", "P");
  lg17->Draw("same");
  
  c17->SaveAs((location+"allowed_03c1_03c2_03c3_syst_comparison.root").c_str());
  c17->SaveAs((location+"allowed_03c1_03c2_03c3_syst_comparison.pdf").c_str());
  c17->SaveAs((location+"allowed_03c1_03c2_03c3_syst_comparison.png").c_str());

  // --------------------------------------------------------
  // Models and syst
  // -------------------------------------------------------

  TCanvas * c18 = new TCanvas("c18","",20,20,900,900);
  c18->SetTopMargin(0.024);
  c18->SetBottomMargin(0.13);
  c18->SetLeftMargin(0.13);
  c18->SetRightMargin(0.024);
  hf1->Draw();
  c18->SetLogx();
  c18->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_stat_2019b3   -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_susav2   -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_smith    -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_lse      -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_nd_fd    -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_prop     -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_mod      -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1     -> Draw("L");

  TLegend * lg18 = new TLegend(0.15,0.15,0.90,0.45);
  lg18->SetFillStyle(0);
  lg18->SetBorderSize(0);
  lg18->SetMargin(0.2);
  lg18->SetTextSize(0.025);
  lg18->SetTextFont(132);

  lg18->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_2019b3, "5#sigma GENIE v2_12_10 Default+MEC", "L");
  lg18->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_susav2, "5#sigma GENIE v3 G18_10a, SuSAv2 1p1h+2p2h", "L");
  lg18->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_smith,  "5#sigma GENIE v3 G18_10a, Smith-Moniz QE", "L");
  lg18->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_lse,    "5#sigma GENIE v3 G18_10a, LS-E QE+MEC", "L");
  lg18->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_nd_fd,  "5#sigma GENIE v3 G18_10a, Valencia 1p1h+2p2h", "L");
  lg18->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_prop,   "5#sigma GENIE v3 G18_10a, Valencia + Prop. Int. Syst.", "L");
  lg18->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_mod,    "5#sigma GENIE v3 G18_10a, Valencia + Mod. Int. Syst.", "L");
  lg18->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1,   "5#sigma GENIE v3 G18_10a, Valencia + Int.+Flux Syst.", "L");
  lg18->Draw("same");
  
  c18->SaveAs((location+"model_stat_syst_comparison.root").c_str());
  c18->SaveAs((location+"model_stat_syst_comparison.png").c_str());
  c18->SaveAs((location+"model_stat_syst_comparison.pdf").c_str());

  // --------------------------------------------------------
  // Detectors no uboone
  // -------------------------------------------------------

  TCanvas * c19 = new TCanvas("c19","",20,20,900,900);
  c19->SetTopMargin(0.024);
  c19->SetBottomMargin(0.13);
  c19->SetLeftMargin(0.13);
  c19->SetRightMargin(0.024);
  hf1->Draw();
  c19->SetLogx();
  c19->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_stat_nd   -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_fd   -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_nd_fd-> Draw("L");

  TLegend * lg19 = new TLegend(0.18,0.18,0.75,0.42);
  lg19->SetFillStyle(0);
  lg19->SetBorderSize(0);
  lg19->SetTextFont(132);

  lg19->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_nd,    "5#sigma SBND", "L");
  lg19->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_fd,    "5#sigma ICARUS", "L");
  lg19->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_nd_fd, "5#sigma ND+FD", "L");
  lg19->Draw("same");
  
  c19->SaveAs((location+"detector_nd_fd_stat_comparison.root").c_str());
  c19->SaveAs((location+"detector_nd_fd_stat_comparison.png").c_str());
  c19->SaveAs((location+"detector_nd_fd_stat_comparison.pdf").c_str());
  
  // --------------------------------------------------------
  // Exclusive systematic groups no bias
  // -------------------------------------------------------

  TCanvas * c20 = new TCanvas("c20","",20,20,900,900);
  c20->SetTopMargin(0.024);
  c20->SetBottomMargin(0.13);
  c20->SetLeftMargin(0.13);
  c20->SetRightMargin(0.024);
  hf1->Draw();
  c20->SetLogx();
  c20->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_stat          -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc -> Draw("L");

  TLegend * lg20 = new TLegend(0.18,0.18,0.88,0.45);
  lg20->SetFillStyle(0);
  lg20->SetBorderSize(0);
  lg20->SetTextFont(132);
  lg20->SetTextSize(0.03);
  
  lg20->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat,               "5#sigma Stat-Only", "L");
  lg20->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc,      "5#sigma Exclusive Flux+Interaction Syst", "L");
  lg20->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc,      "5#sigma Exclusive Flux+Interaction+Detector Syst", "L");
  lg20->Draw("same");
  
  c20->SaveAs((location+"exclusive_03c1_03c2_syst_comparison.root").c_str());
  c20->SaveAs((location+"exclusive_03c1_03c2_syst_comparison.pdf").c_str());
  c20->SaveAs((location+"exclusive_03c1_03c2_syst_comparison.png").c_str());

  // --------------------------------------------------------
  // Allowed 03c1 03c2
  // -------------------------------------------------------

  TCanvas * c21 = new TCanvas("c21","",20,20,900,900);
  c21->SetTopMargin(0.024);
  c21->SetBottomMargin(0.13);
  c21->SetLeftMargin(0.13);
  c21->SetRightMargin(0.024);
  hf1->Draw();
  c21->SetLogx();
  c21->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc_all  -> Draw("LF");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all  -> Draw("LF");
  valor_modern_sbn_3p1_contour_5sig_stat_all           -> Draw("LF");
  injected                                             -> Draw("P");

  TLegend * lg21 = new TLegend(0.18,0.18,0.85,0.55);
  lg21->SetFillStyle(0);
  lg21->SetBorderSize(0);
  lg21->SetTextFont(132);
  lg21->SetTextSize(0.035);
  
  lg21->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_all,          "5#sigma Stat-Only", "L");
  lg21->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_all, "5#sigma Flux+Interaction Syst", "L");
  lg21->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c2_exc_all, "5#sigma Flux+Interaction+Detector Syst", "L");
  lg21->AddEntry(injected,                                            "Injected Point: sin^{2}2#theta_{#mu#mu} = 0.07, #Deltam_{41}^{2} = 1.32", "P");
  lg21->Draw("same");
  
  c21->SaveAs((location+"allowed_03c1_03c2_syst_comparison.root").c_str());
  c21->SaveAs((location+"allowed_03c1_03c2_syst_comparison.pdf").c_str());
  c21->SaveAs((location+"allowed_03c1_03c2_syst_comparison.png").c_str());
  // --------------------------------------------------------
  // MC Reco SBND
  // -------------------------------------------------------

  TCanvas * c22 = new TCanvas("c22","",20,20,900,900);
  c22->SetTopMargin(0.024);
  c22->SetBottomMargin(0.13);
  c22->SetLeftMargin(0.13);
  c22->SetRightMargin(0.024);
  hf1->Draw();
  c22->SetLogx();
  c22->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_stat_sel_nd       -> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_2019b3_nd    -> Draw("L");

  TLegend * lg22 = new TLegend(0.22,0.22,0.72,0.42);
  lg22->SetFillStyle(0);
  lg22->SetBorderSize(0);
  lg22->SetTextFont(132);
  lg22->SetTextSize(0.05);

  lg22->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_2019b3_nd,    "5#sigma SBND-only, MC", "L");
  lg22->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_sel_nd,       "5#sigma SBND-only, Reconstruction", "L");
  lg22->Draw("same");
  
  c22->SaveAs((location+"nd_v2_stat_reco_mc_comparison.root").c_str());
  c22->SaveAs((location+"nd_v2_stat_reco_mc_comparison.png").c_str());
  c22->SaveAs((location+"nd_v2_stat_reco_mc_comparison.pdf").c_str());

  // --------------------------------------------------------
  // MC Reco Ratios SBND
  // -------------------------------------------------------

  TCanvas * c23 = new TCanvas("c23","",20,20,900,900);
  c23->SetTopMargin(0.024);
  c23->SetBottomMargin(0.13);
  c23->SetLeftMargin(0.13);
  c23->SetRightMargin(0.024);
  c23->SetLogy();
  TH1F * hfR3 = dynamic_cast<TH1F*>(c23->DrawFrame(0.78,4E-1,1.22,1E+2));
  hfR3->Draw();
  hfR3->GetYaxis()->SetTitleOffset(1);
  hfR3->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu, MC}/sin^{2}2#theta_{#mu#mu, Reco}");
  hfR3->GetYaxis()->SetTitle("#Deltam^{2}_{41} (eV^{2})");
  hfR3->GetXaxis()->SetTitleFont(132);
  hfR3->GetYaxis()->SetTitleFont(132);
  hfR3->GetXaxis()->SetLabelFont(132);
  hfR3->GetYaxis()->SetLabelFont(132);
  hfR3->GetXaxis()->SetTitleSize(0.06);
  hfR3->GetYaxis()->SetTitleSize(0.06);
  hfR3->GetXaxis()->SetLabelSize(0.05);
  hfR3->GetYaxis()->SetLabelSize(0.05);

  ratio_ref   . Draw("L");
  ratio_mc_reco_v2_nd . Draw("L");
  
  c23->SaveAs((location+"nd_v2_stat_reco_mc_ratio_comparison.root").c_str());
  c23->SaveAs((location+"nd_v2_stat_reco_mc_ratio_comparison.png").c_str());
  c23->SaveAs((location+"nd_v2_stat_reco_mc_ratio_comparison.pdf").c_str());
*/
  // --------------------------------------------------------
  // Stat POTs
  // -------------------------------------------------------

  TCanvas * c24 = new TCanvas("c24","",20,20,900,900);
  c24->SetTopMargin(0.024);
  c24->SetBottomMargin(0.13);
  c24->SetLeftMargin(0.13);
  c24->SetRightMargin(0.024);
  hf1->Draw();
  c24->SetLogx();
  c24->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_stat_nd_fd-> SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_stat_nd_fd-> SetLineStyle(7);

  valor_modern_sbn_3p1_contour_5sig_stat_nd_fd-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_lowPOT_nd_fd-> Draw("L");
  valor_modern_sbn_3p1_contour_5sig_stat_newPOT_nd_fd-> Draw("L");

  TLegend * lg24 = new TLegend(0.18,0.18,0.75,0.42);
  lg24->SetFillStyle(0);
  lg24->SetBorderSize(0);
  lg24->SetTextFont(132);

  lg24->AddEntry(injectedW,                                           "5#sigma SBND, ICARUS", "P");
  lg24->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_lowPOT_nd_fd, "  1e20 POT,   5e20 POT", "L");
  lg24->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_nd_fd,        "6.6e20 POT, 6.6e20 POT", "L");
  lg24->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_newPOT_nd_fd, " 10e20 POT,  14e20 POT", "L");
  lg24->Draw("same");
  
  c24->SaveAs((location+"numu_disapp_pot_nd_fd_stat_comparison.root").c_str());
  c24->SaveAs((location+"numu_disapp_pot_nd_fd_stat_comparison.png").c_str());
  c24->SaveAs((location+"numu_disapp_pot_nd_fd_stat_comparison.pdf").c_str());
  
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

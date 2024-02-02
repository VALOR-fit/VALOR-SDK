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

  TFile * f = new TFile("./old_sterile_3p1_limits.root","read");
  
  // VALOR modern disappearance files

  
  TFile *f_stat_nc         = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/July2020/stat-only-logged_contours_as_graphs.root","read");
  TFile *f_stat            = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/contours/numu_disappearance/July2020_StatOnly/stat-only-nonc_contours_as_graphs.root","read");
  TFile *f_syst            = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/mock_data_challenge/true_baseline/contours/new-syst-etrue_contours_as_graphs.root","read"); // true baseline
  TFile *f_stat_p          = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/contours/december_2019/proposal_stat_3_det_contours_as_graphs.root","read");
  TFile *f_syst_p          = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/contours/december_2019/proposal_syst_3_det_contours_as_graphs.root","read");
  TFile *f_syst_a          = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/contours/december_2019/ubfix_injection_syst_contours_as_graphs.root");
  TFile *f_stat_a          = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/contours/december_2019/ubfix_injection_stat_contours_as_graphs.root");
  //TFile *f_syst_03c1       = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Sept2020/stat-syst-03c1_contours_as_graphs.root"); 
  TFile *f_syst_02c2       = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/August2020/stat_syst_02c1_contours_as_graphs.root");
  //TFile *f_syst_02c2  === new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Sept2020/stat-syst-02c2_contours_as_graphs.root");
  TFile *f_syst_02c1       = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Nov2020/stat-syst-2019b3_2-02c1_contours_as_graphs.root"); 
  TFile *f_syst_02b2       = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/contours/numu_disappearance/June2020/02b2/stat-syst-mec_contours_as_graphs.root");
  TFile *f_syst_02b1       = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Nov2020/stat-syst-2019b3_2-02b1_contours_as_graphs.root");
  TFile *f_syst_02a2       = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/contours/numu_disappearance/May2020_2019b3_02a2/stat-allsyst_contours_as_graphs.root");
                           
  TFile *f_syst_03a1       = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Nov2020/stat-syst-2019b3_2-03a1_contours_as_graphs.root");
  TFile *f_syst_03b1       = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Nov2020/stat-syst-2019b3_2-03b1_contours_as_graphs.root");
  TFile *f_syst_03a1_mec   = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Nov2020/stat-syst-2019b3_2-03a1-mec_contours_as_graphs.root");
  TFile *f_syst_03b1_mec   = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Nov2020/stat-syst-2019b3_2-03b1-mec_contours_as_graphs.root");
  TFile *f_syst_03c1_mec   = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Nov2020/stat-syst-2019b3_2-03c1-mec_contours_as_graphs.root");

  TFile *f_syst_03c1_exc        = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Jan2021/stat-syst-03c1-exc_contours_as_graphs.root"); 
  TFile *f_syst_03c1_bias_exc   = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Jan2021/stat-syst-03c1-bias-exc_contours_as_graphs.root"); 
  TFile *f_stat_2020a1_exc      = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Nov2020/stat-only-2020a1-exc_contours_as_graphs.root","read");

  TFile *f_syst_03c1_bias        = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Feb2021/stat-syst-2020a1-03c1-1sidedBias_contours_as_graphs.root");
  TFile *f_syst_03c1_bias_2sided = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Jan2021/stat-syst-03c1-bias-1p5M_contours_as_graphs.root");
  TFile *f_syst_03c1             = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Feb2021/stat-syst-2020a1-03c1_contours_as_graphs.root");
  TFile *f_syst_03c1_all         = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Nov2020/stat-syst-03c1-2020a1-allowed_contours_as_graphs.root");
  TFile *f_syst_03c1_prop_nomec  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Dec2020/stat-syst-03c1-prop-flipped-1M_contours_as_graphs.root");
  TFile *f_syst_02c1_prop_mec    = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Nov2020/stat-syst-02c1-2-2020a1_contours_as_graphs.root");
  TFile *f_syst_03c1_prop_nm_all = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Nov2020/stat-syst-2020a1-proposal-allowed_contours_as_graphs.root");
  
  TFile *f_stat_2020a1           = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Dec2020/stat-only-flipped_contours_as_graphs.root","read");
  //TFile *f_stat_2020a1           = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Nov2020/stat-only-2020a1_contours_as_graphs.root","read");
  TFile *f_stat_2020a1_all       = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Nov2020/stat-only-2020a1-allowed_contours_as_graphs.root","read");

  TFile *f_reco_syst  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/contours/numu_disappearance/Selection/all-sbn/reco-mc-energy-stat-syst_contours_as_graphs.root");
  TFile *f_reco_stat  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/contours/numu_disappearance/Selection/all-sbn/reco-mc-energy-stat-only_contours_as_graphs.root");
  TFile *f_reco_stat2 = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/contours/numu_disappearance/Selection/all-sbn/reco-energy-stat-only_contours_as_graphs.root");

  // CAFAna proposal and modern numu files
  TFile *f_caf_prop  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/data/sbn/CAFAna/CAFAna_numu_proposal.root","read");
  TFile *f_caf_mod   = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/data/sbn/CAFAna/CAFAna_numu_modern.root","read");
  TFile *f_caf_2020  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/data/sbn/CAFAna/July2020/CAFAna_numu_exclusion.root","read");

  // SBNFit proposal and modern numu files
  TFile *f_sbn_prop     = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/data/sbn/SBNFit/SBNfit_numu_proposal.root","read");
  TFile *f_sbn_prop25   = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/data/sbn/SBNFit/SBNfit_numu_proposal_25var.root","read");
  TFile *f_sbn_mod25    = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/data/sbn/SBNFit/SBNfit_numu_modern_25var.root","read");
  TFile *f_sbn_mod_a    = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-DUNE-LBL/data/sbn/SBNFit/SBNfit_numu_modern_allow.root","read");
  TFile *f_sbn_2020     = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/data/sbn/SBNFit/SBNfit_sensitivity_curve.root","read");
  TFile *f_sbn_2020_sig = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/data/sbn/SBNFit/SBNfit_sensitivity_curve_5sig.root","read");
  TFile *f_sbn_noalt    = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/data/sbn/SBNFit/SBNfit_sensitivity_curve_woutAlt.root","read");

  // 2020a1 sample
  TFile *f_sbnfit_exc_prop = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/data/sbn/SBNFit/Nov2020/proposal/updated_SBNfit_Numu_disappear_sensitivity.root");
  TFile *f_sbnfit_all_prop = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/data/sbn/SBNFit/Nov2020/proposal/SBNfit_numu_disappear_allowed_region.root");
  TFile *f_sbnfit_exc_mod  = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/data/sbn/SBNFit/Nov2020/modern/SBNfit_Numu_disappear_sensitivity_noMEC.root");

  TFile *f_cafana_exc_prop     = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/data/sbn/CAFAna/Nov2020/CAFAna_numu_disapp_exclusion.root");
  TFile *f_cafana_exc_fixed    = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/data/sbn/CAFAna/Feb2021/CAFAna_numu_dissapp_exclusion.root");
  TFile *f_cafana_all_prop     = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/data/sbn/CAFAna/Nov2020/CAFAna_numu_disapp_allowed.root");
  TFile *f_cafana_exc_prop_mec = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/data/sbn/CAFAna/Nov2020/CAFAna_numu_disapp_exclusion_mec.root");
  TFile *f_cafana_all_prop_mec = new TFile("/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/data/sbn/CAFAna/Nov2020/CAFAna_numu_disapp_allowed_mec.root");
  
  std::string location = "/exp/sbnd/app/users/rsjones/VALOR/VALOR-SBN/contours/numu_disappearance/Feb2021/output/";
  
  // 
  // Global style settings
  //
  gStyle->SetTitleSize(0.04,"X");
  gStyle->SetTitleSize(0.04,"Y");
  gStyle->SetLabelSize(0.03,"X");
  gStyle->SetLabelSize(0.03,"Y");

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

  TGraph * lim_numudis_3p1_sbnproposal_90pctCL                   = (TGraph *) f->Get( "lim_numudis_3p1_sbnproposal_90pctCL"  );
  TGraph * lim_numudis_3p1_minosp_90pctCL                        = (TGraph *) f->Get( "lim_numudis_3p1_minosp_90pctCL_sinsq2theta_corrected");
  TGraph * lim_numudis_3p1_minisciboone_90pctCL                  = (TGraph *) f->Get( "lim_numudis_3p1_minisciboone_90pctCL" );
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c1           = (TGraph *) f_syst_03c1->Get("stat_syst_5sigma_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c1_bias      = (TGraph *) f_syst_03c1_bias->Get("stat_syst_5sigma_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c1_2s_bias   = (TGraph *) f_syst_03c1_bias_2sided->Get("stat_syst_5sigma_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c1_mec       = (TGraph *) f_syst_03c1_mec->Get("stat_syst_5sigma_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c1_pr_nomec  = (TGraph *) f_syst_03c1_prop_nomec->Get("stat_syst_5sigma_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_02c1_pr_mec    = (TGraph *) f_syst_02c1_prop_mec->Get("stat_syst_5sigma_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c1_pr_nm_all = (TGraph *) f_syst_03c1_prop_nm_all->Get("stat_syst_5sigma_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c1_all       = (TGraph *) f_syst_03c1_all->Get("stat_syst_5sigma_lin_8");
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc       = (TGraph *) f_syst_03c1_exc->Get("stat_syst_5sigma_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_bias  = (TGraph *) f_syst_03c1_bias_exc->Get("stat_syst_5sigma_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03b1           = (TGraph *) f_syst_03b1->Get("stat_syst_5sigma_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_02c2           = (TGraph *) f_syst_02c2->Get("stat_syst_5sigma_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_02c1           = (TGraph *) f_syst_02c1->Get("stat_syst_5sigma_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_02b1           = (TGraph *) f_syst_02b1->Get("stat_syst_5sigma_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_5sig_syst_03a1           = (TGraph *) f_syst_03a1->Get("stat_syst_5sigma_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_syst_03c1_exc         = (TGraph *) f_syst_03c1_exc->Get("stat_syst_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_syst_03c1_pr_nomec    = (TGraph *) f_syst_03c1_prop_nomec->Get("stat_syst_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_syst_03c1             = (TGraph *) f_syst_03c1->Get("stat_syst_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_syst_02c2             = (TGraph *) f_syst_02c2->Get("stat_syst_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_syst_02c1             = (TGraph *) f_syst_02c1->Get("stat_syst_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_syst_02b2             = (TGraph *) f_syst_02b2->Get("stat-syst-mec_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_syst_02b1             = (TGraph *) f_syst_02b1->Get("stat_syst_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_syst_02a2             = (TGraph *) f_syst_02a2->Get("stat-allsyst_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_syst_02a1             = (TGraph *) f_syst->Get("new-syst-etrue_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_syst_03b1             = (TGraph *) f_syst_03b1->Get("stat_syst_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_syst_03a1             = (TGraph *) f_syst_03a1->Get("stat_syst_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_syst_03a1_mec         = (TGraph *) f_syst_03a1_mec->Get("stat_syst_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_syst_03b1_mec         = (TGraph *) f_syst_03b1_mec->Get("stat_syst_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_syst_03c1_mec         = (TGraph *) f_syst_03c1_mec->Get("stat_syst_90pct_lin_0");
  TGraph * valor_modern_sbn_3p1_contour_90_stat_nc               = (TGraph *) f_stat_nc->Get("stat_only_90pct_lin_0"); // nc oscillations 
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_nc             = (TGraph *) f_stat_nc->Get("stat_only_5sigma_lin_0"); // nc oscillations 
  TGraph * valor_modern_sbn_3p1_contour_90_stat_2020             = (TGraph *) f_stat_2020a1->Get("stat_only_90pct_lin_0");  // 2020a1 sample 
  TGraph * valor_modern_sbn_3p1_contour_90_stat_exc              = (TGraph *) f_stat_2020a1_exc->Get("stat_only_90pct_lin_0");  // 2020a1 sample 
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_2020           = (TGraph *) f_stat_2020a1->Get("stat_only_5sigma_lin_0"); // 2020a1 sample
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_2020_all       = (TGraph *) f_stat_2020a1_all->Get("stat_only_5sigma_lin_0"); // 2020a1 sample
  TGraph * valor_modern_sbn_3p1_contour_5sig_stat_exc            = (TGraph *) f_stat_2020a1_exc->Get("stat_only_5sigma_lin_0"); // 2020a1 sample
  TGraph * valor_modern_sbn_3p1_contour_90_stat                  = (TGraph *) f_stat->Get("stat-only_90pct_lin_0"); // true baseline 
  TGraph * valor_reco_e_sbn_3p1_contour_90_stat                  = (TGraph *) f_reco_stat2->Get("reco-energy-stat-only_90pct_lin_0");
  TGraph * valor_reco_sbn_3p1_contour_90_stat                    = (TGraph *) f_reco_stat->Get("reco-mc-energy-stat-only_90pct_lin_0");
  TGraph * valor_reco_sbn_3p1_contour_90_stat2                   = (TGraph *) f_reco_stat->Get("reco-mc-energy-stat-only_90pct_lin_1");
  TGraph * valor_reco_sbn_3p1_contour_90_syst                    = (TGraph *) f_reco_syst->Get("reco-mc-energy-stat-syst_90pct_lin_0");
  TGraph * valor_proposal_sbn_3p1_contour_90_stat                = (TGraph *) f_stat_p->Get("contour_90pct_lin_0");
  TGraph * valor_proposal_sbn_3p1_contour_90_syst                = (TGraph *) f_syst_p->Get("contour_90pct_lin_0");
  TGraph * cafana_proposal_sbn_3p1_contour_90_stat               = (TGraph *) f_caf_prop->Get("numu_exclusion_all_stat_90pct"); 
  TGraph * cafana_proposal_sbn_3p1_contour_90_syst               = (TGraph *) f_caf_prop->Get("numu_exclusion_all_syst_90pct"); 
  TGraph * cafana_modern_sbn_3p1_contour_90_stat                 = (TGraph *) f_caf_2020->Get("all_stat_90pct"); 
  TGraph * cafana_modern_sbn_3p1_contour_5sig_stat               = (TGraph *) f_caf_2020->Get("all_stat_5sig"); 
  TGraph * cafana_modern_sbn_3p1_contour_90_syst                 = (TGraph *) f_caf_2020->Get("all_syst_90pct"); 
  TGraph * cafana_modern_sbn_3p1_contour_5sig_syst               = (TGraph *) f_caf_2020->Get("all_syst_5sig"); 
  TGraph * sbnfit_proposal_sbn_3p1_contour_90_stat               = (TGraph *) f_sbn_prop->Get("fit_STATONLY_90CL"); 
  TGraph * sbnfit_proposal_sbn_3p1_contour_90_syst20             = (TGraph *) f_sbn_prop->Get("fit_all_90CL"); 
  TGraph * sbnfit_proposal_sbn_3p1_contour_90_syst               = (TGraph *) f_sbn_prop25->Get("all_90CL"); 
  TGraph * sbnfit_modern_sbn_3p1_contour_90_stat                 = (TGraph *) f_sbn_2020->Get("STATONLY_90CL");  
  TGraph * sbnfit_modern_sbn_3p1_contour_90_syst                 = (TGraph *) f_sbn_noalt->Get("g90CL"); 
  TGraph * sbnfit_modern_sbn_3p1_contour_5sig_stat               = (TGraph *) f_sbn_2020_sig->Get("STATONLY_5sigma");  
  TGraph * sbnfit_modern_sbn_3p1_contour_5sig_syst               = (TGraph *) f_sbn_noalt->Get("g5sigma"); 

  // 2020a1
  TGraph * sbnfit_proposal_nomec_exc_90 = (TGraph *) f_sbnfit_exc_prop->Get("g90CL");
  TGraph * sbnfit_proposal_nomec_exc_5s = (TGraph *) f_sbnfit_exc_prop->Get("g5sigma");
  TGraph * sbnfit_modern_nomec_exc_90   = (TGraph *) f_sbnfit_exc_mod->Get("g90CL");
  TGraph * sbnfit_modern_nomec_exc_5s   = (TGraph *) f_sbnfit_exc_mod->Get("g5sigma");
  TGraph * sbnfit_proposal_nomec_all_90 = (TGraph *) f_sbnfit_all_prop->Get("g90CL");
  TGraph * sbnfit_proposal_nomec_all_5s = (TGraph *) f_sbnfit_all_prop->Get("g5sigma");
  TGraph * sbnfit_stat_exc_90           = (TGraph *) f_sbnfit_exc_prop->Get("STATONLY_90CL");
  TGraph * sbnfit_stat_exc_5s           = (TGraph *) f_sbnfit_exc_prop->Get("STATONLY_5sigma");

  TGraph * cafana_proposal_exc_nomec_90 = (TGraph *) f_cafana_exc_prop->Get("all_syst_90pct");
  TGraph * cafana_proposal_exc_nomec_5s = (TGraph *) f_cafana_exc_prop->Get("all_syst_5sig");
  TGraph * cafana_proposal_exc_mec_90   = (TGraph *) f_cafana_exc_prop_mec->Get("all_syst_90pct");
  TGraph * cafana_proposal_exc_mec_5s   = (TGraph *) f_cafana_exc_prop_mec->Get("all_syst_5sig");
  TGraph * cafana_proposal_all_nomec_90 = (TGraph *) f_cafana_all_prop->Get("all_syst_90pct_1");
  TGraph * cafana_proposal_all_nomec_5s = (TGraph *) f_cafana_all_prop->Get("all_syst_5sig_1");
  TGraph * cafana_proposal_all_mec_90   = (TGraph *) f_cafana_all_prop_mec->Get("all_syst_90pct_1");
  TGraph * cafana_proposal_all_mec_5s   = (TGraph *) f_cafana_all_prop_mec->Get("all_syst_5sig_1");
  TGraph * cafana_stat_exc_90           = (TGraph *) f_cafana_exc_fixed->Get("all_stat_90pct");
  TGraph * cafana_stat_exc_5s           = (TGraph *) f_cafana_exc_fixed->Get("all_stat_5sig");
  TGraph * cafana_stat_all_90           = (TGraph *) f_cafana_all_prop->Get("all_stat_90pct_1");
  TGraph * cafana_stat_all_5s           = (TGraph *) f_cafana_all_prop->Get("all_stat_5sig_2");
  
  const std::vector<double> x = {2.4e-2,2.8e-2,3e-2,3.46e-2,6e-2,5.88e-2};
  const std::vector<double> y = {6,38.8,2,2.29,40,38.88};

  TGraph *injected = new TGraph(6,x.data(),y.data());
  injected->SetMarkerStyle(43);
  injected->SetMarkerSize(1.5);
  injected->SetMarkerColor(kGray+3);

  std::cout << " Got histograms" << std::endl;

  // VALOR Reconstructed
  valor_reco_sbn_3p1_contour_90_stat2          -> SetLineColor(kPink+4);
  valor_reco_sbn_3p1_contour_90_stat2          -> SetLineStyle(2);
  valor_reco_sbn_3p1_contour_90_stat           -> SetLineColor(kPink+4);
  valor_reco_sbn_3p1_contour_90_stat           -> SetLineStyle(2);
  valor_reco_e_sbn_3p1_contour_90_stat         -> SetLineColor(kPink+4);
  valor_reco_e_sbn_3p1_contour_90_stat         -> SetLineStyle(5);
  valor_reco_sbn_3p1_contour_90_syst           -> SetLineColor(kPink+5);  
  valor_reco_sbn_3p1_contour_90_syst           -> SetLineStyle(4);

  // VALOR modern stat
  valor_modern_sbn_3p1_contour_90_stat_nc      -> SetLineColor(kPink+4);
  valor_modern_sbn_3p1_contour_90_stat_nc      -> SetLineStyle(4);
  valor_modern_sbn_3p1_contour_5sig_stat_nc    -> SetLineColor(kPink+4);
  valor_modern_sbn_3p1_contour_5sig_stat_nc    -> SetLineStyle(5);
  valor_modern_sbn_3p1_contour_90_stat_2020    -> SetLineColor(kPink+4);
  valor_modern_sbn_3p1_contour_90_stat_2020    -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_90_stat_exc     -> SetLineColor(kPink+4);
  valor_modern_sbn_3p1_contour_90_stat_exc     -> SetLineStyle(3);
  valor_modern_sbn_3p1_contour_5sig_stat_exc   -> SetLineColor(kPink+4);
  valor_modern_sbn_3p1_contour_5sig_stat_exc   -> SetLineStyle(6);
  valor_modern_sbn_3p1_contour_90_stat         -> SetLineColor(kPink+4);
  valor_modern_sbn_3p1_contour_90_stat         -> SetLineStyle(4);

  // VALOR modern syst
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_bias -> SetLineColor(kPink+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_bias -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc      -> SetLineColor(kPink+4);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc      -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_bias     -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_bias     -> SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_2s_bias  -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_2s_bias  -> SetLineStyle(3);
  valor_modern_sbn_3p1_contour_5sig_syst_02c2          -> SetLineColor(kPink+2);  
  valor_modern_sbn_3p1_contour_5sig_syst_02c2          -> SetLineStyle(9); 
  valor_modern_sbn_3p1_contour_5sig_syst_02c1          -> SetLineColor(kPink+2);  
  valor_modern_sbn_3p1_contour_5sig_syst_02c1          -> SetLineStyle(1); 
  valor_modern_sbn_3p1_contour_5sig_syst_03b1          -> SetLineColor(kPink+2);  
  valor_modern_sbn_3p1_contour_5sig_syst_03b1          -> SetLineStyle(8); 
  valor_modern_sbn_3p1_contour_5sig_syst_02b1          -> SetLineColor(kPink+2);  
  valor_modern_sbn_3p1_contour_5sig_syst_02b1          -> SetLineStyle(8); 
  valor_modern_sbn_3p1_contour_5sig_syst_03a1          -> SetLineColor(kPink+2);  
  valor_modern_sbn_3p1_contour_5sig_syst_03a1          -> SetLineStyle(6); 
  
  valor_modern_sbn_3p1_contour_90_syst_02c2       -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_90_syst_02c2       -> SetLineStyle(1); 
  valor_modern_sbn_3p1_contour_90_syst_02c1       -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_90_syst_02c1       -> SetLineStyle(2); 
  valor_modern_sbn_3p1_contour_90_syst_02b2       -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_90_syst_02b2       -> SetLineStyle(3); 
  valor_modern_sbn_3p1_contour_90_syst_02b1       -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_90_syst_02b1       -> SetLineStyle(1); 
  valor_modern_sbn_3p1_contour_90_syst_02a2       -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_90_syst_02a2       -> SetLineStyle(6); 
  valor_modern_sbn_3p1_contour_90_syst_02a1       -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_90_syst_02a1       -> SetLineStyle(10); 
  valor_modern_sbn_3p1_contour_90_syst_03a1       -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_90_syst_03a1       -> SetLineStyle(10); 
  valor_modern_sbn_3p1_contour_90_syst_03b1       -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_90_syst_03b1       -> SetLineStyle(8); 
  valor_modern_sbn_3p1_contour_90_syst_03c1       -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_90_syst_03c1       -> SetLineStyle(5); 
  valor_modern_sbn_3p1_contour_90_syst_03c1_exc   -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_90_syst_03c1_exc   -> SetLineStyle(6); 
  valor_modern_sbn_3p1_contour_90_syst_03a1_mec   -> SetLineColor(kPink+4);  
  valor_modern_sbn_3p1_contour_90_syst_03a1_mec   -> SetLineStyle(10); 
  valor_modern_sbn_3p1_contour_90_syst_03b1_mec   -> SetLineColor(kPink+4);  
  valor_modern_sbn_3p1_contour_90_syst_03b1_mec   -> SetLineStyle(8); 
  valor_modern_sbn_3p1_contour_90_syst_03c1_mec   -> SetLineColor(kPink+4);  
  valor_modern_sbn_3p1_contour_90_syst_03c1_mec   -> SetLineStyle(5); 

  // VALOR proposal
  valor_proposal_sbn_3p1_contour_90_stat   -> SetLineColor(kPink+4);
  valor_proposal_sbn_3p1_contour_90_stat   -> SetLineStyle(3);
  valor_proposal_sbn_3p1_contour_90_syst   -> SetLineColor(kPink+5);  
  valor_proposal_sbn_3p1_contour_90_syst   -> SetLineStyle(9);  

  // VALOR 2020a1 PAC
  valor_modern_sbn_3p1_contour_5sig_syst_03c1          -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1          -> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_pr_nomec -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_pr_nomec -> SetLineStyle(4); 
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_pr_nomec -> SetLineWidth(2); 
  valor_modern_sbn_3p1_contour_5sig_syst_02c1_pr_mec   -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_5sig_syst_02c1_pr_mec   -> SetLineStyle(4); 
  valor_modern_sbn_3p1_contour_5sig_syst_02c1_pr_mec   -> SetLineWidth(2); 
  valor_modern_sbn_3p1_contour_90_syst_03c1_pr_nomec   -> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_90_syst_03c1_pr_nomec   -> SetLineStyle(4); 
  valor_modern_sbn_3p1_contour_90_syst_03c1_pr_nomec   -> SetLineWidth(2); 
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_pr_nm_all-> SetLineColor(kPink+5);  
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_pr_nm_all-> SetLineStyle(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_pr_nm_all-> SetLineWidth(1);
  valor_modern_sbn_3p1_contour_5sig_stat_2020          -> SetLineColor(kPink+2);
  valor_modern_sbn_3p1_contour_5sig_stat_2020          -> SetLineStyle(8);
  valor_modern_sbn_3p1_contour_5sig_stat_2020          -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_stat_2020            -> SetLineColor(kPink+4);
  valor_modern_sbn_3p1_contour_90_stat_2020            -> SetLineStyle(8);
  valor_modern_sbn_3p1_contour_90_stat_2020            -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_stat_2020_all      -> SetLineColor(kPink+2);
  valor_modern_sbn_3p1_contour_5sig_stat_2020_all      -> SetLineStyle(4);
  valor_modern_sbn_3p1_contour_5sig_stat_2020_all      -> SetLineWidth(2);

  // CAFAna
  cafana_modern_sbn_3p1_contour_90_stat    -> SetLineColor(kOrange+2);
  cafana_modern_sbn_3p1_contour_90_stat    -> SetLineStyle(4);
  cafana_modern_sbn_3p1_contour_5sig_stat  -> SetLineColor(kOrange+2);
  cafana_modern_sbn_3p1_contour_5sig_stat  -> SetLineStyle(5);
  cafana_modern_sbn_3p1_contour_90_syst    -> SetLineColor(kOrange+1);  
  cafana_modern_sbn_3p1_contour_90_syst    -> SetLineStyle(2);
  cafana_modern_sbn_3p1_contour_5sig_syst  -> SetLineColor(kOrange+1);
  cafana_modern_sbn_3p1_contour_5sig_syst  -> SetLineStyle(1);
  cafana_proposal_sbn_3p1_contour_90_stat  -> SetLineColor(kOrange+2);
  cafana_proposal_sbn_3p1_contour_90_stat  -> SetLineStyle(3);
  cafana_proposal_sbn_3p1_contour_90_syst  -> SetLineColor(kOrange+1);  
  cafana_proposal_sbn_3p1_contour_90_syst  -> SetLineStyle(9);

  cafana_proposal_exc_nomec_90 -> SetLineColor(kOrange+1);
  cafana_proposal_exc_nomec_90 -> SetLineStyle(4);
  cafana_proposal_exc_nomec_5s -> SetLineColor(kOrange+1);
  cafana_proposal_exc_nomec_5s -> SetLineStyle(4);
  cafana_proposal_exc_nomec_5s -> SetLineWidth(2);
  cafana_proposal_exc_mec_90   -> SetLineColor(kOrange+1);
  cafana_proposal_exc_mec_90   -> SetLineStyle(5);
  cafana_proposal_exc_mec_5s   -> SetLineColor(kOrange+1);
  cafana_proposal_exc_mec_5s   -> SetLineStyle(5);
  cafana_proposal_all_nomec_90 -> SetLineColor(kOrange+1);
  cafana_proposal_all_nomec_90 -> SetLineStyle(6);
  cafana_proposal_all_nomec_5s -> SetLineColor(kOrange+1);
  cafana_proposal_all_nomec_5s -> SetLineStyle(1);
  cafana_proposal_all_nomec_5s -> SetLineWidth(1);
  cafana_proposal_all_mec_90   -> SetLineColor(kOrange+1);
  cafana_proposal_all_mec_90   -> SetLineStyle(7);
  cafana_proposal_all_mec_5s   -> SetLineColor(kOrange+1);
  cafana_proposal_all_mec_5s   -> SetLineStyle(7);
  cafana_stat_exc_90           -> SetLineColor(kOrange+2);
  cafana_stat_exc_90           -> SetLineStyle(8);
  cafana_stat_exc_5s           -> SetLineColor(kOrange+2);
  cafana_stat_exc_5s           -> SetLineStyle(8);
  cafana_stat_exc_5s           -> SetLineWidth(2);
  cafana_stat_all_90           -> SetLineColor(kOrange+2);
  cafana_stat_all_90           -> SetLineStyle(9);
  cafana_stat_all_5s           -> SetLineColor(kOrange+2);
  cafana_stat_all_5s           -> SetLineStyle(4);
  cafana_stat_all_5s           -> SetLineWidth(2);

  // SBNFit
  sbnfit_modern_sbn_3p1_contour_90_stat    -> SetLineColor(kAzure+5);
  sbnfit_modern_sbn_3p1_contour_90_stat    -> SetLineStyle(4);
  sbnfit_modern_sbn_3p1_contour_90_syst    -> SetLineColor(kAzure+7);  
  sbnfit_modern_sbn_3p1_contour_90_syst    -> SetLineStyle(2);
  sbnfit_modern_sbn_3p1_contour_5sig_stat  -> SetLineColor(kAzure+5);
  sbnfit_modern_sbn_3p1_contour_5sig_stat  -> SetLineStyle(5);
  sbnfit_modern_sbn_3p1_contour_5sig_syst  -> SetLineColor(kAzure+7);
  sbnfit_modern_sbn_3p1_contour_5sig_syst  -> SetLineStyle(1);
  sbnfit_proposal_sbn_3p1_contour_90_stat  -> SetLineColor(kAzure+5);
  sbnfit_proposal_sbn_3p1_contour_90_stat  -> SetLineStyle(3);
  sbnfit_proposal_sbn_3p1_contour_90_syst  -> SetLineColor(kAzure+7);  
  sbnfit_proposal_sbn_3p1_contour_90_syst  -> SetLineStyle(9);

  sbnfit_proposal_nomec_exc_90  -> SetLineColor(kAzure+7);
  sbnfit_proposal_nomec_exc_90  -> SetLineStyle(4);
  sbnfit_proposal_nomec_exc_5s  -> SetLineColor(kAzure+7);  
  sbnfit_proposal_nomec_exc_5s  -> SetLineStyle(4);
  sbnfit_proposal_nomec_exc_5s  -> SetLineWidth(2);
  sbnfit_modern_nomec_exc_90    -> SetLineColor(kAzure+7);
  sbnfit_modern_nomec_exc_90    -> SetLineStyle(5);
  sbnfit_modern_nomec_exc_5s    -> SetLineColor(kAzure+7);
  sbnfit_modern_nomec_exc_5s    -> SetLineStyle(5);
  sbnfit_proposal_nomec_all_90  -> SetLineColor(kAzure+7);
  sbnfit_proposal_nomec_all_90  -> SetLineStyle(6);
  sbnfit_proposal_nomec_all_5s  -> SetLineColor(kAzure+7);  
  sbnfit_proposal_nomec_all_5s  -> SetLineStyle(1);
  sbnfit_proposal_nomec_all_5s  -> SetLineWidth(1);
  sbnfit_stat_exc_90            -> SetLineColor(kAzure-7);
  sbnfit_stat_exc_90            -> SetLineStyle(8);
  sbnfit_stat_exc_5s            -> SetLineColor(kAzure-7);
  sbnfit_stat_exc_5s            -> SetLineStyle(8);
  sbnfit_stat_exc_5s            -> SetLineWidth(2);
                               
  // External
  lim_numudis_3p1_sbnproposal_90pctCL      -> SetLineColor(kGray+2);
  lim_numudis_3p1_minosp_90pctCL           -> SetLineColor(kTeal-5);  
  lim_numudis_3p1_minisciboone_90pctCL     -> SetLineColor(kYellow-3); 
  

  // Additional styling
  valor_reco_sbn_3p1_contour_90_stat                   -> SetLineWidth(2);
  valor_reco_e_sbn_3p1_contour_90_stat                 -> SetLineWidth(2);
  valor_reco_sbn_3p1_contour_90_stat2                  -> SetLineWidth(2);
  valor_reco_sbn_3p1_contour_90_syst                   -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_stat                 -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_stat_nc              -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_stat_nc            -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_stat_2020            -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_stat_exc             -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_stat_2020          -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_stat_exc           -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1          -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc      -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_bias -> SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_bias     -> SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_2s_bias  -> SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_syst_02c2          -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_syst_02c1          -> SetLineWidth(1);
  valor_modern_sbn_3p1_contour_5sig_syst_03b1          -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_syst_02b1          -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_syst_03a1          -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_syst_03c1_exc        -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_syst_02c2            -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_syst_02c1            -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_syst_02b2            -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_syst_02b1            -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_syst_02a2            -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_syst_02a1            -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_syst_03c1            -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_syst_03b1            -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_syst_03a1            -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_syst_03c1_mec        -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_syst_03b1_mec        -> SetLineWidth(2);
  valor_modern_sbn_3p1_contour_90_syst_03a1_mec        -> SetLineWidth(2);
  valor_proposal_sbn_3p1_contour_90_stat               -> SetLineWidth(2);
  valor_proposal_sbn_3p1_contour_90_syst               -> SetLineWidth(2);
  cafana_modern_sbn_3p1_contour_90_stat                -> SetLineWidth(2);
  cafana_modern_sbn_3p1_contour_5sig_stat              -> SetLineWidth(1);
  cafana_modern_sbn_3p1_contour_90_syst                -> SetLineWidth(2);
  cafana_modern_sbn_3p1_contour_5sig_syst              -> SetLineWidth(2);
  cafana_proposal_sbn_3p1_contour_90_stat              -> SetLineWidth(2);
  cafana_proposal_sbn_3p1_contour_90_syst              -> SetLineWidth(2);
  sbnfit_modern_sbn_3p1_contour_90_stat                -> SetLineWidth(2);
  sbnfit_modern_sbn_3p1_contour_5sig_stat              -> SetLineWidth(2);
  sbnfit_modern_sbn_3p1_contour_90_syst                -> SetLineWidth(2);
  sbnfit_modern_sbn_3p1_contour_5sig_syst              -> SetLineWidth(1);
  sbnfit_proposal_sbn_3p1_contour_90_stat              -> SetLineWidth(2);
  sbnfit_proposal_sbn_3p1_contour_90_syst              -> SetLineWidth(2);
  lim_numudis_3p1_sbnproposal_90pctCL                  -> SetLineWidth(2);
  lim_numudis_3p1_minosp_90pctCL                       -> SetLineWidth(2);
  lim_numudis_3p1_minisciboone_90pctCL                 -> SetLineWidth(2);
  
  lim_numudis_3p1_sbnproposal_90pctCL                  -> SetMarkerStyle(0);
  valor_reco_e_sbn_3p1_contour_90_stat                 -> SetMarkerStyle(0);
  valor_reco_sbn_3p1_contour_90_stat                   -> SetMarkerStyle(0);
  valor_reco_sbn_3p1_contour_90_stat2                  -> SetMarkerStyle(0);
  valor_reco_sbn_3p1_contour_90_syst                   -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_stat_nc              -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_5sig_stat_nc            -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_stat_2020            -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_stat_exc             -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_5sig_stat_2020          -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_5sig_stat_exc           -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_stat                 -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc      -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_bias -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_bias     -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_2s_bias  -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1          -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_5sig_syst_02c2          -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_5sig_syst_02c1          -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_5sig_syst_03b1          -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_5sig_syst_02b1          -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_5sig_syst_03a1          -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_syst_03c1_exc        -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_syst_02c2            -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_syst_02c1            -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_syst_02b2            -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_syst_02b1            -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_syst_02a2            -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_syst_02a1            -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_syst_03c1            -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_syst_03b1            -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_syst_03a1            -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_syst_03c1_mec        -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_syst_03b1_mec        -> SetMarkerStyle(0);
  valor_modern_sbn_3p1_contour_90_syst_03a1_mec        -> SetMarkerStyle(0);
  valor_proposal_sbn_3p1_contour_90_stat               -> SetMarkerStyle(0);
  valor_proposal_sbn_3p1_contour_90_syst               -> SetMarkerStyle(0);
  cafana_modern_sbn_3p1_contour_90_stat                -> SetMarkerStyle(0);
  cafana_modern_sbn_3p1_contour_5sig_stat              -> SetMarkerStyle(0);
  cafana_modern_sbn_3p1_contour_90_syst                -> SetMarkerStyle(0);
  cafana_modern_sbn_3p1_contour_5sig_syst              -> SetMarkerStyle(0);
  cafana_proposal_sbn_3p1_contour_90_stat              -> SetMarkerStyle(0);
  cafana_proposal_sbn_3p1_contour_90_syst              -> SetMarkerStyle(0);
  sbnfit_modern_sbn_3p1_contour_90_stat                -> SetMarkerStyle(0);
  sbnfit_modern_sbn_3p1_contour_5sig_stat              -> SetMarkerStyle(0);
  sbnfit_modern_sbn_3p1_contour_90_syst                -> SetMarkerStyle(0);
  sbnfit_modern_sbn_3p1_contour_5sig_syst              -> SetMarkerStyle(0);
  sbnfit_proposal_sbn_3p1_contour_90_stat              -> SetMarkerStyle(0);
  sbnfit_proposal_sbn_3p1_contour_90_syst              -> SetMarkerStyle(0);
  lim_numudis_3p1_minosp_90pctCL                       -> SetMarkerStyle(0);
  lim_numudis_3p1_minisciboone_90pctCL                 -> SetMarkerStyle(0);

  // Reconstruction compared with MC and external data 
  //

  TCanvas * cnumudisnostatallfittersextmodern = new TCanvas("cnumudisnostatallfittersextmodern","",20,20,900,900);
  cnumudisnostatallfittersextmodern->SetTopMargin(0.022);
  cnumudisnostatallfittersextmodern->SetBottomMargin(0.099);
  cnumudisnostatallfittersextmodern->SetLeftMargin(0.116);
  cnumudisnostatallfittersextmodern->SetRightMargin(0.024);
  cnumudisnostatallfittersextmodern->Draw();

  TH2F * hfnumudisnostatallfittersextmodern = (TH2F*) cnumudisnostatallfittersextmodern->DrawFrame(1E-3,1E-2,1,1E+2);
  hfnumudisnostatallfittersextmodern->Draw();
  hfnumudisnostatallfittersextmodern->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisnostatallfittersextmodern->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisnostatallfittersextmodern->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisnostatallfittersextmodern->GetXaxis()->SetTitleFont(132);
  hfnumudisnostatallfittersextmodern->GetYaxis()->SetTitleFont(132);
  hfnumudisnostatallfittersextmodern->GetXaxis()->SetLabelFont(132);
  hfnumudisnostatallfittersextmodern->GetYaxis()->SetLabelFont(132);
  cnumudisnostatallfittersextmodern->SetLogx();
  cnumudisnostatallfittersextmodern->SetLogy();


  valor_reco_sbn_3p1_contour_90_syst        -> Draw("l");
  valor_reco_e_sbn_3p1_contour_90_stat      -> Draw("l");
  valor_modern_sbn_3p1_contour_90_stat      -> Draw("l");
  valor_modern_sbn_3p1_contour_90_syst_02c2 -> Draw("l");
  lim_numudis_3p1_sbnproposal_90pctCL       -> Draw("l");
  lim_numudis_3p1_minosp_90pctCL            -> Draw("l");
  lim_numudis_3p1_minisciboone_90pctCL      -> Draw("l");

  TLegend * lgnumudisnostatallfittersextmodern = new TLegend(0.140,0.127,0.529,0.482);
  lgnumudisnostatallfittersextmodern->SetFillStyle(0);
  lgnumudisnostatallfittersextmodern->SetBorderSize(0);
  lgnumudisnostatallfittersextmodern->SetTextFont(132);
  lgnumudisnostatallfittersextmodern->AddEntry(lim_numudis_3p1_minisciboone_90pctCL,      "MiniBooNE/SciBooNE 90\% CL",             "L");
  lgnumudisnostatallfittersextmodern->AddEntry(lim_numudis_3p1_minosp_90pctCL,            "MINOS/MINOS+ 90\% CL",                   "L");
  lgnumudisnostatallfittersextmodern->AddEntry(lim_numudis_3p1_sbnproposal_90pctCL,       "SBN proposal 90\% CL",                   "L");
  lgnumudisnostatallfittersextmodern->AddEntry(valor_modern_sbn_3p1_contour_90_stat,      "VALOR 90\% CL MC stat-only",             "L");
  lgnumudisnostatallfittersextmodern->AddEntry(valor_reco_e_sbn_3p1_contour_90_stat,      "VALOR 90\% CL Reco stat-only",           "L");
  lgnumudisnostatallfittersextmodern->AddEntry(valor_modern_sbn_3p1_contour_90_syst_02c2, "VALOR 90\% CL MC stat+syst",             "L");
  lgnumudisnostatallfittersextmodern->AddEntry(valor_reco_sbn_3p1_contour_90_syst,        "VALOR 90\% CL Reco MC-energy stat+syst", "L");
  lgnumudisnostatallfittersextmodern->Draw();

  cnumudisnostatallfittersextmodern->SaveAs((location+"all_external_valor_plus_reco.root").c_str());

  // 
  // GENIE v2 vs GENIE v3
  //

  TCanvas * cnumudismodernvalor = new TCanvas("cnumudismodernvalor","",20,20,900,900);
  cnumudismodernvalor->SetTopMargin(0.022);
  cnumudismodernvalor->SetBottomMargin(0.099);
  cnumudismodernvalor->SetLeftMargin(0.116);
  cnumudismodernvalor->SetRightMargin(0.024);
  cnumudismodernvalor->Draw();

  TH2F * hfnumudismodernvalor = (TH2F*) cnumudismodernvalor->DrawFrame(1E-3,1E-2,1,1E+2);
  hfnumudismodernvalor->Draw();
  hfnumudismodernvalor->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudismodernvalor->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudismodernvalor->GetYaxis()->SetTitleOffset(1.3);
  hfnumudismodernvalor->GetYaxis()->SetTitleOffset(1.1);
  hfnumudismodernvalor->GetXaxis()->SetTitleFont(132);
  hfnumudismodernvalor->GetYaxis()->SetTitleFont(132);
  hfnumudismodernvalor->GetXaxis()->SetLabelFont(132);
  hfnumudismodernvalor->GetYaxis()->SetLabelFont(132);
  cnumudismodernvalor->SetLogx();
  cnumudismodernvalor->SetLogy();

 // valor_modern_sbn_3p1_contour_90_syst_02c1   -> Draw("l");
 // valor_modern_sbn_3p1_contour_90_syst_03c1   -> Draw("l");
//  valor_modern_sbn_3p1_contour_5sig_syst_02c1 -> Draw("l");
  //valor_modern_sbn_3p1_contour_5sig_syst_02c1_pr_mec -> Draw("l");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1         -> Draw("l");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_bias    -> Draw("l");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_2s_bias -> Draw("l");
  //  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc -> Draw("l");
 // valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_bias -> Draw("l");
 // valor_modern_sbn_3p1_contour_90_stat_nc     -> Draw("l");
//  valor_modern_sbn_3p1_contour_5sig_stat_nc   -> Draw("l");
 // valor_modern_sbn_3p1_contour_90_stat_2020     -> Draw("l");
//  valor_modern_sbn_3p1_contour_5sig_stat_2020   -> Draw("l");

  TLegend * lgnumudismodernvalor = new TLegend(0.132,0.114,0.750,0.35);
  lgnumudismodernvalor->SetFillStyle(0);
  lgnumudismodernvalor->SetBorderSize(0);
  lgnumudismodernvalor->SetTextFont(132);
 // lgnumudismodernvalor->AddEntry(valor_modern_sbn_3p1_contour_90_stat_nc,  "SBN 90\% CL stat-only",                          "L");
//  lgnumudismodernvalor->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_nc,"SBN 5#sigma CL stat-only GENIE v2",              "L");
 // lgnumudismodernvalor->AddEntry(valor_modern_sbn_3p1_contour_90_stat_2020,  "SBN 90\% CL stat-only GENIE v3",               "L");
//  lgnumudismodernvalor->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_2020,"SBN 5#sigma CL stat-only GENIE v3",               "L");
 // lgnumudismodernvalor->AddEntry(valor_modern_sbn_3p1_contour_90_syst_02c1,"SBN 90\% CL ",                                   "L");
//  lgnumudismodernvalor->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_02c1,"SBN 5#sigma CL stat+syst GENIE v2",            "L");
 // lgnumudismodernvalor->AddEntry(valor_modern_sbn_3p1_contour_90_syst_03c1,"SBN 90\% CL GENIE v3",                           "L");
  lgnumudismodernvalor->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1,        "SBN 5#sigma CL stat+syst+MEC GENIE v3",              "L");
  lgnumudismodernvalor->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_bias,   "SBN 5#sigma CL stat+syst+MEC+1-sided Bias GENIE v3", "L");
  lgnumudismodernvalor->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_2s_bias,"SBN 5#sigma CL stat+syst+MEC+2-sided Bias GENIE v3", "L");
 // lgnumudismodernvalor->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc,"SBN 5#sigma CL stat+syst+MEC EXC GENIE v3",           "L");
 // lgnumudismodernvalor->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc_bias,"SBN 5#sigma CL stat+syst+MEC+Bias EXC GENIE v3", "L");
  //lgnumudismodernvalor->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_02c1_pr_mec,"SBN 5#sigma CL stat+syst+MEC GENIE v3", "L");
  lgnumudismodernvalor->Draw();

  cnumudismodernvalor->SaveAs((location+"valor_1_2_sided_bias_nobias.root").c_str());
  
  // 
  // Modern only, stat-only
  //

  TCanvas * cnumudismodernstat = new TCanvas("cnumudismodernstat","",20,20,900,900);
  cnumudismodernstat->SetTopMargin(0.022);
  cnumudismodernstat->SetBottomMargin(0.099);
  cnumudismodernstat->SetLeftMargin(0.116);
  cnumudismodernstat->SetRightMargin(0.024);
  cnumudismodernstat->Draw();

  TH2F * hfnumudismodernstat = (TH2F*) cnumudismodernstat->DrawFrame(1E-3,1E-2,1,1E+2);
  hfnumudismodernstat->Draw();
  hfnumudismodernstat->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudismodernstat->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudismodernstat->GetYaxis()->SetTitleOffset(1.3);
  hfnumudismodernstat->GetYaxis()->SetTitleOffset(1.1);
  hfnumudismodernstat->GetXaxis()->SetTitleFont(132);
  hfnumudismodernstat->GetYaxis()->SetTitleFont(132);
  hfnumudismodernstat->GetXaxis()->SetLabelFont(132);
  hfnumudismodernstat->GetYaxis()->SetLabelFont(132);
  cnumudismodernstat->SetLogx();
  cnumudismodernstat->SetLogy();

  cafana_stat_exc_90->Draw("l");                                  
  cafana_stat_exc_5s->Draw("l");
  sbnfit_stat_exc_90->Draw("l");                                  
  sbnfit_stat_exc_5s->Draw("l");
  cafana_modern_sbn_3p1_contour_90_stat      -> Draw("l");
  cafana_modern_sbn_3p1_contour_5sig_stat    -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_90_stat      -> Draw("l");
  sbnfit_modern_sbn_3p1_contour_5sig_stat    -> Draw("l");
  valor_modern_sbn_3p1_contour_90_stat_nc    -> Draw("l");
  valor_modern_sbn_3p1_contour_5sig_stat_nc  -> Draw("l");
  valor_modern_sbn_3p1_contour_90_stat_2020  -> Draw("l");
  valor_modern_sbn_3p1_contour_5sig_stat_2020-> Draw("l");

  TLegend * lgnumudismodernstat = new TLegend(0.132,0.135,0.8,0.40);
  lgnumudismodernstat->SetFillStyle(0);
  lgnumudismodernstat->SetBorderSize(0);
  lgnumudismodernstat->SetTextFont(132);
  lgnumudismodernstat->AddEntry(cafana_modern_sbn_3p1_contour_90_stat,      "CAFAna 90\% CL stat-only",           "L");
  lgnumudismodernstat->AddEntry(cafana_modern_sbn_3p1_contour_5sig_stat,    "CAFAna 5#sigma CL stat-only",        "L");
  lgnumudismodernstat->AddEntry(sbnfit_modern_sbn_3p1_contour_90_stat,      "SBNFit 90\% CL stat-only",           "L");
  lgnumudismodernstat->AddEntry(sbnfit_modern_sbn_3p1_contour_5sig_stat,    "SBNFit 5#sigma CL stat-only",        "L");
  lgnumudismodernstat->AddEntry(valor_modern_sbn_3p1_contour_90_stat_nc,    "VALOR 90\% CL stat-only",            "L");
  lgnumudismodernstat->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_nc,  "VALOR 5#sigma CL stat-only",         "L");
  lgnumudismodernstat->AddEntry(valor_modern_sbn_3p1_contour_90_stat_2020,  "VALOR 90\% CL stat-only, 2020",      "L");
  lgnumudismodernstat->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_2020,"VALOR 5#sigma CL stat-only, 2020",   "L");
  lgnumudismodernstat->AddEntry(cafana_stat_exc_90,                         "CAFAna 90\% CL stat-only, 2020",     "L");
  lgnumudismodernstat->AddEntry(cafana_stat_exc_5s,                         "CAFAna 5#sigma CL stat-only, 2020",  "L");
  lgnumudismodernstat->AddEntry(sbnfit_stat_exc_90,                         "SBNFit 90\% CL stat-only, 2020",     "L");
  lgnumudismodernstat->AddEntry(sbnfit_stat_exc_5s,                         "SBNFit 5#sigma CL stat-only, 2020",  "L");
  lgnumudismodernstat->Draw();

  cnumudismodernstat->SaveAs((location+"all_fitters_stat.root").c_str());
  
  // 
  // VALOR 2020a1 inclusive exclusive comparison
  //

  TCanvas * cnumudisincexccomp = new TCanvas("cnumudisincexccomp","",20,20,900,900);
  cnumudisincexccomp->SetTopMargin(0.022);
  cnumudisincexccomp->SetBottomMargin(0.099);
  cnumudisincexccomp->SetLeftMargin(0.116);
  cnumudisincexccomp->SetRightMargin(0.024);
  cnumudisincexccomp->Draw();

  TH2F * hfnumudisincexccomp = (TH2F*) cnumudisincexccomp->DrawFrame(1E-3,1E-2,1,1E+2);
  hfnumudisincexccomp->Draw();
  hfnumudisincexccomp->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisincexccomp->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisincexccomp->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisincexccomp->GetXaxis()->SetTitleFont(132);
  hfnumudisincexccomp->GetYaxis()->SetTitleFont(132);
  hfnumudisincexccomp->GetXaxis()->SetLabelFont(132);
  hfnumudisincexccomp->GetYaxis()->SetLabelFont(132);
  cnumudisincexccomp->SetLogx();
  cnumudisincexccomp->SetLogy();

  valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc-> Draw("l");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1-> Draw("l");
  valor_modern_sbn_3p1_contour_90_syst_03c1_exc-> Draw("l");
  valor_modern_sbn_3p1_contour_90_syst_03c1-> Draw("l");
  valor_modern_sbn_3p1_contour_5sig_stat_2020-> Draw("l");
  valor_modern_sbn_3p1_contour_90_stat_2020-> Draw("l");
  valor_modern_sbn_3p1_contour_90_stat_exc-> Draw("l");
  valor_modern_sbn_3p1_contour_5sig_stat_exc-> Draw("l");
  
  TLegend * lgnumudisincexccomp = new TLegend(0.132,0.114,0.750,0.45);
  lgnumudisincexccomp->SetFillStyle(0);
  lgnumudisincexccomp->SetBorderSize(0);
  lgnumudisincexccomp->SetTextFont(132);
  
  lgnumudisincexccomp->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_2020,     "VALOR 5#sigma CL stat Inc.", "L");
  lgnumudisincexccomp->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_exc,      "VALOR 5#sigma CL stat Exc.", "L");
  lgnumudisincexccomp->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1,     "VALOR 5#sigma CL Inclusive", "L");
  lgnumudisincexccomp->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_exc, "VALOR 5#sigma CL Exclusive", "L");
  lgnumudisincexccomp->AddEntry(valor_modern_sbn_3p1_contour_90_stat_2020,       "VALOR 90\% CL stat Inc.",    "L");
  lgnumudisincexccomp->AddEntry(valor_modern_sbn_3p1_contour_90_stat_exc,        "VALOR 90\% CL stat Exc.",    "L");
  lgnumudisincexccomp->AddEntry(valor_modern_sbn_3p1_contour_90_syst_03c1,       "VALOR 90\% CL Inclusive",    "L");
  lgnumudisincexccomp->AddEntry(valor_modern_sbn_3p1_contour_90_syst_03c1_exc,   "VALOR 90\% CL Exclusive",    "L");
  lgnumudisincexccomp->Draw();
  
  cnumudisincexccomp->SaveAs((location+"valor_inclusive_exclusive_comparison.root").c_str());

  // 
  // VALOR 2020a1 inclusive exclusive comparison
  //

  TCanvas * cnumudisincpropfitterscomp = new TCanvas("cnumudisincpropfitterscomp","",20,20,900,900);
  cnumudisincpropfitterscomp->SetTopMargin(0.022);
  cnumudisincpropfitterscomp->SetBottomMargin(0.099);
  cnumudisincpropfitterscomp->SetLeftMargin(0.116);
  cnumudisincpropfitterscomp->SetRightMargin(0.024);
  cnumudisincpropfitterscomp->Draw();

  TH2F * hfnumudisincpropfitterscomp = (TH2F*) cnumudisincpropfitterscomp->DrawFrame(1E-3,1E-2,1,1E+2);
  hfnumudisincpropfitterscomp->Draw();
  hfnumudisincpropfitterscomp->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisincpropfitterscomp->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisincpropfitterscomp->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisincpropfitterscomp->GetXaxis()->SetTitleFont(132);
  hfnumudisincpropfitterscomp->GetYaxis()->SetTitleFont(132);
  hfnumudisincpropfitterscomp->GetXaxis()->SetLabelFont(132);
  hfnumudisincpropfitterscomp->GetYaxis()->SetLabelFont(132);
  cnumudisincpropfitterscomp->SetLogx();
  cnumudisincpropfitterscomp->SetLogy();

  cafana_modern_sbn_3p1_contour_5sig_stat              -> Draw("l");             
  sbnfit_modern_sbn_3p1_contour_5sig_stat              -> Draw("l");             
  valor_modern_sbn_3p1_contour_5sig_stat_nc            -> Draw("l");           
                                                                 
  cafana_modern_sbn_3p1_contour_5sig_syst              -> Draw("l");             
  sbnfit_modern_sbn_3p1_contour_5sig_syst              -> Draw("l");             
  valor_modern_sbn_3p1_contour_5sig_syst_02c1          -> Draw("l");         
  
  cafana_stat_exc_5s                                   -> Draw("l");                                  
  sbnfit_stat_exc_5s                                   -> Draw("l");                                  
  valor_modern_sbn_3p1_contour_5sig_stat_2020          -> Draw("l");         
  cafana_proposal_exc_nomec_5s                         -> Draw("l");                        
  sbnfit_proposal_nomec_exc_5s                         -> Draw("l");                        
  valor_modern_sbn_3p1_contour_5sig_syst_03c1          -> Draw("l");

  TLegend * lgnumudisincpropfitterscomp = new TLegend(0.132,0.114,0.550,0.45);
  lgnumudisincpropfitterscomp->SetFillStyle(0);
  lgnumudisincpropfitterscomp->SetBorderSize(0);
  lgnumudisincpropfitterscomp->SetTextFont(132);
  
  lgnumudisincpropfitterscomp->AddEntry(cafana_modern_sbn_3p1_contour_5sig_stat,              "CAFAna 5 #sigma CL stat",  "L");
  lgnumudisincpropfitterscomp->AddEntry(sbnfit_modern_sbn_3p1_contour_5sig_stat,              "SBNFit 5 #sigma CL stat",  "L");
  lgnumudisincpropfitterscomp->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_nc,            "VALOR 5 #sigma CL stat",   "L");

  lgnumudisincpropfitterscomp->AddEntry(cafana_modern_sbn_3p1_contour_5sig_syst,              "CAFAna 5 #sigma CL syst",       "L");
  lgnumudisincpropfitterscomp->AddEntry(sbnfit_modern_sbn_3p1_contour_5sig_syst,              "SBNFit 5 #sigma CL syst",       "L");
  lgnumudisincpropfitterscomp->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_02c1,          "VALOR 5 #sigma CL syst",        "L");
  
  lgnumudisincpropfitterscomp->AddEntry(cafana_stat_exc_5s,                                   "CAFAna GENIE v3 CL stat",       "L");
  lgnumudisincpropfitterscomp->AddEntry(sbnfit_stat_exc_5s,                                   "SBNFit GENIE v3 CL stat",       "L");
  lgnumudisincpropfitterscomp->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_2020,          "VALOR GENIE v3 CL stat",        "L");
  
  lgnumudisincpropfitterscomp->AddEntry(cafana_proposal_exc_nomec_5s,                         "CAFAna GENIE v3 CL syst",       "L");
  lgnumudisincpropfitterscomp->AddEntry(sbnfit_proposal_nomec_exc_5s,                         "SBNFit GENIE v3 CL syst",       "L");
  lgnumudisincpropfitterscomp->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1,          "VALOR GENIE v3 CL syst",        "L");
  lgnumudisincpropfitterscomp->Draw();
  
  cnumudisincpropfitterscomp->SaveAs((location+"all_fitters_inclusive_proposal_comparison.root").c_str());

  // 
  // 2020a1 systematic comparison
  //

  TCanvas * cnumudisincpropsystfitterscomp = new TCanvas("cnumudisincpropsystfitterscomp","",20,20,900,900);
  cnumudisincpropsystfitterscomp->SetTopMargin(0.022);
  cnumudisincpropsystfitterscomp->SetBottomMargin(0.099);
  cnumudisincpropsystfitterscomp->SetLeftMargin(0.116);
  cnumudisincpropsystfitterscomp->SetRightMargin(0.024);
  cnumudisincpropsystfitterscomp->Draw();

  TH2F * hfnumudisincpropsystfitterscomp = (TH2F*) cnumudisincpropsystfitterscomp->DrawFrame(1E-3,1E-2,1,1E+2);
  hfnumudisincpropsystfitterscomp->Draw();
  hfnumudisincpropsystfitterscomp->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisincpropsystfitterscomp->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisincpropsystfitterscomp->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisincpropsystfitterscomp->GetXaxis()->SetTitleFont(132);
  hfnumudisincpropsystfitterscomp->GetYaxis()->SetTitleFont(132);
  hfnumudisincpropsystfitterscomp->GetXaxis()->SetLabelFont(132);
  hfnumudisincpropsystfitterscomp->GetYaxis()->SetLabelFont(132);
  cnumudisincpropsystfitterscomp->SetLogx();
  cnumudisincpropsystfitterscomp->SetLogy();

  cafana_proposal_exc_mec_5s->Draw("l");                        
//  sbnfit_modern_nomec_exc_5s->Draw("l");                        
//  sbnfit_proposal_nomec_exc_5s->Draw("l");                        
//  valor_modern_sbn_3p1_contour_5sig_syst_02c1_pr_mec->Draw("l");
//  valor_modern_sbn_3p1_contour_5sig_syst_02c1_pr_mec->Draw("l");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_pr_nomec->Draw("l");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_pr_nomec->SetLineStyle(2);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_pr_nomec->SetLineWidth(3);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_pr_nomec->SetLineColor(kAzure+7);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1->Draw("l");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1->SetLineStyle(1);
//  valor_modern_sbn_3p1_contour_5sig_syst_03c1->SetLineWidth(2);
  valor_modern_sbn_3p1_contour_5sig_syst_03c1->SetLineColor(kPink+5);
  //injected->Draw("P");
  
  TLegend * lgnumudisincpropsystfitterscomp = new TLegend(0.132,0.114,0.750,0.35);
  lgnumudisincpropsystfitterscomp->SetFillStyle(0);
  lgnumudisincpropsystfitterscomp->SetBorderSize(0);
  lgnumudisincpropsystfitterscomp->SetTextFont(132);
  lgnumudisincpropsystfitterscomp->SetTextSize(0.035);
  
  lgnumudisincpropsystfitterscomp->AddEntry(cafana_proposal_exc_mec_5s,                           "SBN Proposal, MEC 5#sigma CL syst", "L");
 // lgnumudisincpropsystfitterscomp->AddEntry(sbnfit_proposal_nomec_exc_5s,                         "SBN Proposal, no MEC 5#sigma CL syst", "L");
 // lgnumudisincpropsystfitterscomp->AddEntry(sbnfit_modern_nomec_exc_5s,                           "SBN Modern, no MEC 5#sigma CL syst", "L");
  lgnumudisincpropsystfitterscomp->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_pr_nomec, "VALOR, GENIE v3, 5#sigma CL proposal syst no-MEC",  "L");
//  lgnumudisincpropsystfitterscomp->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_02c1_pr_mec,   "VALOR, GENIE v3, 5#sigma CL proposal syst+MEC",     "L");
  lgnumudisincpropsystfitterscomp->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1,          "VALOR, GENIE v3, 5#sigma CL modern syst+MEC",     "L");
  lgnumudisincpropsystfitterscomp->Draw();
  
  cnumudisincpropsystfitterscomp->SaveAs((location+"sbn_inclusive_proposal_syst_comparison.root").c_str());

  // 
  // 2020a1 external comparison
  //

  TCanvas * cnumudisincextcomp = new TCanvas("cnumudisincextcomp","",20,20,900,900);
  cnumudisincextcomp->SetTopMargin(0.022);
  cnumudisincextcomp->SetBottomMargin(0.099);
  cnumudisincextcomp->SetLeftMargin(0.116);
  cnumudisincextcomp->SetRightMargin(0.024);
  cnumudisincextcomp->Draw();

  TH2F * hfnumudisincextcomp = (TH2F*) cnumudisincextcomp->DrawFrame(1E-3,1E-2,1,1E+2);
  hfnumudisincextcomp->Draw();
  hfnumudisincextcomp->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisincextcomp->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisincextcomp->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisincextcomp->GetXaxis()->SetTitleFont(132);
  hfnumudisincextcomp->GetYaxis()->SetTitleFont(132);
  hfnumudisincextcomp->GetXaxis()->SetLabelFont(132);
  hfnumudisincextcomp->GetYaxis()->SetLabelFont(132);
  hfnumudisincextcomp->GetXaxis()->SetTitleSize(0.06);
  hfnumudisincextcomp->GetYaxis()->SetTitleSize(0.06);
  hfnumudisincextcomp->GetXaxis()->SetLabelSize(0.05);
  hfnumudisincextcomp->GetYaxis()->SetLabelSize(0.05);
  cnumudisincextcomp->SetLogx();
  cnumudisincextcomp->SetLogy();

 // valor_modern_sbn_3p1_contour_5sig_syst_02c1-> Draw("l");
 // valor_modern_sbn_3p1_contour_5sig_stat_nc  -> Draw("l");
  lim_numudis_3p1_minosp_90pctCL             -> Draw("l");
  lim_numudis_3p1_minisciboone_90pctCL       -> Draw("l");
  lim_numudis_3p1_sbnproposal_90pctCL        -> Draw("l");
  
  TLegend * lgnumudisincextcomp = new TLegend(0.132,0.132,0.55,0.45);
  lgnumudisincextcomp->SetFillStyle(0);
  lgnumudisincextcomp->SetBorderSize(0);
  lgnumudisincextcomp->SetTextFont(132);
  
  lgnumudisincextcomp->AddEntry(lim_numudis_3p1_minisciboone_90pctCL,        "MiniBooNE/SciBooNE 90\% CL", "L");
  lgnumudisincextcomp->AddEntry(lim_numudis_3p1_minosp_90pctCL,              "MINOS/MINOS+ 90\% CL",       "L");
  lgnumudisincextcomp->AddEntry(lim_numudis_3p1_sbnproposal_90pctCL,         "SBN Proposal 90\% CL",       "L");
 // lgnumudisincextcomp->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_nc,   "SBN 5#sigma CL stat-only",   "L");
 // lgnumudisincextcomp->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_02c1, "SBN 5#sigma CL stat+syst",   "L");
  lgnumudisincextcomp->Draw();
  
  cnumudisincextcomp->SaveAs((location+"sbn_external_comparison.root").c_str());

  // 
  // 2020a1 allowed region
  //

  TCanvas * cnumudisallowedpropsyst = new TCanvas("cnumudisallowedpropsyst","",20,20,900,900);
  cnumudisallowedpropsyst->SetTopMargin(0.022);
  cnumudisallowedpropsyst->SetBottomMargin(0.099);
  cnumudisallowedpropsyst->SetLeftMargin(0.116);
  cnumudisallowedpropsyst->SetRightMargin(0.024);
  cnumudisallowedpropsyst->Draw();

  TH2F * hfnumudisallowedpropsyst = (TH2F*) cnumudisallowedpropsyst->DrawFrame(1E-2,4e-1,5E-1,4.);
  hfnumudisallowedpropsyst->Draw();
  hfnumudisallowedpropsyst->GetYaxis()->SetTitleOffset(1.3);
  hfnumudisallowedpropsyst->GetXaxis()->SetTitle("sin^{2}2#theta_{#mu#mu}");
  hfnumudisallowedpropsyst->GetYaxis()->SetTitle("#Delta m^{2}_{41} (eV^{2})");
  hfnumudisallowedpropsyst->GetXaxis()->SetTitleFont(132);
  hfnumudisallowedpropsyst->GetYaxis()->SetTitleFont(132);
  hfnumudisallowedpropsyst->GetXaxis()->SetLabelFont(132);
  hfnumudisallowedpropsyst->GetYaxis()->SetLabelFont(132);
  cnumudisallowedpropsyst->SetLogx();
  cnumudisallowedpropsyst->SetLogy();

 // sbnfit_proposal_nomec_all_5s                          -> Draw("l");
  cafana_stat_all_5s                                    -> Draw("l");
  cafana_proposal_all_nomec_5s                          -> Draw("l");
  valor_modern_sbn_3p1_contour_5sig_syst_03c1_pr_nm_all -> Draw("l");
  valor_modern_sbn_3p1_contour_5sig_stat_2020_all       -> Draw("l");
  injected->Draw("P");

  TLegend * lgnumudisallowedpropsyst = new TLegend(0.132,0.72,0.750,0.95);
  lgnumudisallowedpropsyst->SetFillStyle(0);
  lgnumudisallowedpropsyst->SetBorderSize(0);
  lgnumudisallowedpropsyst->SetTextFont(132);
  lgnumudisallowedpropsyst->SetTextSize(0.03);
  
  lgnumudisallowedpropsyst->AddEntry(cafana_stat_all_5s,                                    "CAFAna 5#sigma CL stat", "L");
  lgnumudisallowedpropsyst->AddEntry(valor_modern_sbn_3p1_contour_5sig_stat_2020_all,       "VALOR 5#sigma CL stat",  "L");
  lgnumudisallowedpropsyst->AddEntry(cafana_proposal_all_nomec_5s,                          "CAFAna 5#sigma CL syst", "L");
  //lgnumudisallowedpropsyst->AddEntry(sbnfit_proposal_nomec_all_5s,                          "SBNFit 5#sigma CL syst", "L");
  lgnumudisallowedpropsyst->AddEntry(valor_modern_sbn_3p1_contour_5sig_syst_03c1_pr_nm_all, "VALOR 5#sigma CL syst",  "L");
  lgnumudisallowedpropsyst->AddEntry(injected, "#Delta m^{2}_{41} = 1.32, sin^{2}2#theta_{#mu#mu} = 0.07",  "P");

  lgnumudisallowedpropsyst->Draw();
  
  cnumudisallowedpropsyst->SaveAs((location+"allowed_fitter_comparison.root").c_str());

}

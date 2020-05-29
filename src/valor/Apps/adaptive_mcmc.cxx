//____________________________________________________________________________
/*!

\app       valor-adaptive_mcmc

\brief     Generate and fit ensembles of toy-MC experiments.

\options    --input                    : File containing VALOR ND Tree of fit result
            --output                   : Output file name
            --config                   : Analysis configuration
           [--nsteps ]                 : Number of steps to process [ 30000 ]
           [--fit-number ]             : Entry in the input tree to use [ 0 ]
           [--enable-adaptive]         : Modify the size of the steps in the proposal function over time
           [--proposal-step]           : Proposal step size factor. Smaller makes it it more likely to accept a given step, but slows mixing. [0.3]
           [--burn-in]                 : Number of iterations to ignore in final covariance
           [--start-at-bestfit]        : Don't throw a random start point
           [--mesg-thresholds]         : Message thresholds [ "verbosity.xml" ]
           [--use-prefit-correlations] : Use correlations from prefit rather than postfit matrix for proposal
           [--use-no-correlations]     : Use diagonal matrix for proposals
           [--use-prefit-errors]       : Use errors from prefit rather than postfit matrix for proposal
           [--write-expanded-tree]     : Write each parameter value in its own branch. No extra info, just easier to browse.
           [--pot-override ]           : Use this POT for every sample (ignoring the one in config) [off]

\author    Steve Dennis <s.r.dennis \at liverpool.ac.uk>
           University of Liverpool

\created   July 2016

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

// System
#include <cstdlib>
#include <algorithm>
#include <iostream>
// ROOT
#include "TFile.h"
#include "TMath.h"
#include "TSystem.h"
#include "TObjArray.h"
#include "TString.h"
#include "TVectorD.h"
#include "TPrincipal.h"
#include "TStopwatch.h"
// VALOR/SDK
#include "valor/Conventions/Constants.h"
#include "valor/Conventions/Opt.h"
#include "valor/CmdLine/CmdLnArgList.h"
#include "valor/FitCore/PhysParamList.h"
#include "valor/FitCore/FitParamList.h"
#include "valor/FitCore/FitModelABC.h"
#include "valor/FitCore/FitDataABC.h"
#include "valor/FitCore/FitInputs.h"
#include "valor/FitCore/FitterABC.h"
#include "valor/FitCore/UtlFit.h"
#include "valor/FitCore/UtlFitParamList.h"
#include "valor/FitCore/CompoundFitElements.h"
#include "valor/Utilities/UtlNumStat.h"
#include "valor/Utilities/UtlStyle.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/Env.h"
#include "valor/Utilities/VThreadPool.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Ntuple/StdFitNtp.h"

using namespace std;
using namespace valor;
using namespace valor::conv;

int main(int argc, char ** argv)
{

  valor::util::PrintBanner(Env::Instance()->Banner(),500);

  CmdLnArgList arg;
  arg.Read(argc,argv,"$VALOR_FMWK/config/apps/adaptive_mcmc.arg:$VALOR_FMWK/config/apps/seeds.arg");
  LOG("valor", pNOTICE) << arg;

  std::string thresholds = util::CompleteConfigPath(arg.GetString("mesg-thresholds"));
  Messenger::Instance()->Configure(thresholds);

  string input_filename        = arg.GetString("input");
  string output_filename       = arg.GetString("output");
  string config                = arg.GetString("config");
  int nsteps                   = arg.GetInt("nsteps");
  int fitnumber                = arg.GetInt("fit-number");
  int burn_in                  = arg.GetInt("burn-in");
  int n_threads                = arg.GetInt("nthreads");
  double proposal_step         = arg.GetDouble("proposal-step");
  bool start_at_bestfit        = arg.GetBool("start-at-bestfit");
  bool write_expanded_tree     = arg.GetBool("write-expanded-tree");
  bool disable_adaptive        = arg.GetBool("disable-adaptive");
  bool use_prefit_correlations = arg.GetBool("use-prefit-correlations");
  bool use_no_correlations     = arg.GetBool("use-no-correlations");
  bool use_prefit_errors       = arg.GetBool("use-prefit-errors");

  // If pot_override is set to a positive number, every sample will be initialised with the same POT - this number
  double pot_override     = arg.GetDouble("pot-override");
  if (pot_override > 0 ) {
    Opt::Instance()->MutableGlobalRegistry().Set("pot_override",pot_override);
  }

  const double target_n_attempts = 3;
  const int recalculate_step_every = 50;
  const int print_step_every = 500;

  // Set the random seeds
  Rnd::Instance()->SetSeeds(arg);
  LOG("valor", pNOTICE) << *Rnd::Instance();

  // Set up the thread pool
  if(n_threads>0) {//without n-cores specified, use as many as the host has
    util::VThreadPool::Instance()->SetNThreads(n_threads);
  }

  // Set up the model
  config = util::CompleteConfigPath(config);
  LOG("valor", pNOTICE) << "Getting and configuring the fit elements";
  CompoundFitElements * elements = new CompoundFitElements();
  elements->Read(config);
  LOG("valor", pNOTICE) << "FitElements read complete.";
  FitDataABC  * data     = elements->FitData();
  FitModelABC * modelabc = elements->FitModel();
  CompoundFitModel * model = dynamic_cast<CompoundFitModel*>(modelabc); assert(model);

  // Get the input FPL
  TFile * input_file = new TFile(input_filename.c_str()); assert(input_file->IsOpen());
  TTree * in_tr = dynamic_cast<TTree*>(input_file->Get("valor_fit_ntp")); assert(in_tr);
  FitParamList *  best_fit_params      = 0; // best-fit params
  in_tr->SetBranchAddress("best_fit_params",    &best_fit_params);
  assert(in_tr->GetEntries() > fitnumber);
  in_tr->GetEntry(fitnumber); assert(best_fit_params);
  best_fit_params->CopyBestFitToCurrValues();
  LOG("valor", pNOTICE) << "Read input FPL.";
  LOG("valor", pNOTICE) << *best_fit_params;

  // Param list size
  const int npar      = best_fit_params->GetNumberParameters();
  const int npar_free = best_fit_params->GetNumberFreeParameters();

  conv::PhysParamId_t * free_parameters = new PhysParamId_t[npar_free];
  int free_counter = 0;
  for (int ip = 0 ; ip < npar ; ip++) {
    if (best_fit_params->IsFixedByIndex(ip)) {continue;}
    free_parameters[free_counter] = best_fit_params->WhichParam(ip);
  }

  // Make the BF data
  LOG("valor", pNOTICE) << "Generating data";
  data->Generate(model,*best_fit_params,false);

  // Setup the output tree
  TFile * output_file = new TFile(output_filename.c_str(),"RECREATE");
  output_file->cd();
  TTree * out_tr = new TTree("mcsteps","mcsteps");
  int o_npar = npar;
  double o_chisq = -999;
  double * o_params = new double[npar];

  out_tr->Branch("npar"  ,&o_npar  ,"npar/I");
  out_tr->Branch("params",o_params,"o_params[npar]/D");
  out_tr->Branch("chisq" ,&o_chisq ,"chisq/D");
  if (write_expanded_tree) {
    for (int ip = 0 ; ip < npar ; ip++) {
      std::string pname  = best_fit_params->NameByIndex(ip);
      std::string pnameD = pname+"/D";
      out_tr->Branch(pname.c_str(),&(o_params[ip]),pnameD.c_str());
    }
  }
  LOG("valor", pNOTICE) << "Output tree built";

  // Build a Cholesky matrix we can use for throws
  CorrelatedSystematics * syst = model->GetSystematics();
  const TMatrixD & prefit_cov = syst->CovMatrix();
  assert(best_fit_params);
  TMatrixD fitted_cov(*best_fit_params->GetCovMatrix());

  for (int ip = 0 ; ip < fitted_cov.GetNrows() ; ip++) {
    for (int jp = 0 ; jp < fitted_cov.GetNcols() ; jp++) {
      double val = fitted_cov(ip,jp);
      if (!TMath::Finite(val) || std::fabs(val) < conv::kAVerySmallNum) {
        fitted_cov(ip,jp) = (ip==jp);
      }
    }
  }

  TMatrixD prop_corr(util::CovarianceMatrixToCorrelationMatrix(fitted_cov));
  TVectorD prop_err (util::CovarianceMatrixToErrorVector(fitted_cov));

  if (use_prefit_errors) {
    TVectorD tmp_err = util::CovarianceMatrixToErrorVector(prefit_cov);
    for (int ip = 0 ; ip < npar; ip++) {
      PhysParamId_t p = best_fit_params->WhichParam(ip);
      if (syst->Contains(p)) {
        int idx = syst->Param2Idx(p);
        prop_err[ip] = tmp_err[idx];
      }
    }
  }
  if (use_prefit_correlations) {
    TMatrixD tmp_corr = util::CovarianceMatrixToCorrelationMatrix(prefit_cov);
    for (int ip = 0 ; ip < npar; ip++) {
      PhysParamId_t p1 = best_fit_params->WhichParam(ip);
      if (syst->Contains(p1)) {
        int idx1 = syst->Param2Idx(p1);
        for (int jp = 0 ; jp < npar; jp++) {
          PhysParamId_t p2 = best_fit_params->WhichParam(jp);
          if (syst->Contains(p2)) {
            int idx2 = syst->Param2Idx(p2);
            prop_corr(ip,jp) = tmp_corr[idx1][idx2];
          }
        }
      }
    }
  }
  else if (use_no_correlations) {
    for (int i = 0 ; i < npar ; i++) {
      for (int j = 0 ; j < npar ; j++ ) {
        prop_corr[i][j] = (i==j);
      }
    }
  }

  TMatrixD init_proposal_cov(util::CorrelationAndErrorToCovarianceMatrix(prop_corr,prop_err));
  LOG("valor", pNOTICE) << "Proposal covariance built.";

  const TMatrixD chol_matrix(util::CholeskyDecomposition(init_proposal_cov));

  // Throw a random start point, unless told not to
  if (!start_at_bestfit) {
    TRandom3 & rnd = Rnd::Instance()->RndOther();
    TVectorD throwvec(npar);
    bool bad_throw = true;
    while (bad_throw) {
      bad_throw = false;
      for (int ip = 0 ; ip < npar ; ip++) {
        throwvec[ip] = rnd.Gaus();
      }
      // Correlate the throws
      throwvec *= (chol_matrix);
      // Check they're valid, then copy into FPL
      for (int ip = 0 ; ip < npar ; ip++) {
        if (best_fit_params->IsFixedByIndex(ip)) {continue;}
        PhysParamId_t p = best_fit_params->WhichParam(ip);
        assert(p!=conv::kPhypUndefined);
        double min   = best_fit_params->MinValue(p);
        double start = best_fit_params->BestFitValue(p);
        double max   = best_fit_params->MaxValue(p);
        double val  = start+throwvec[ip];
        if (val > max || val < min) {
          bad_throw=true;
          break;
        }
        best_fit_params->SetCurrentValue(p,val);
      }
    }
  }

  // Setup fitinputs
  FitInputs fitinputs(data,model,best_fit_params);

  best_fit_params->Print();

  // Get starting statistic
  double current_chisq = fitinputs.Statistic();
  //~ std::cout<<current_chisq<<"\t"<<best_fit_params->GetBestFitStatistic()<<std::endl;
  if (start_at_bestfit) {
    assert(std::fabs(current_chisq - best_fit_params->GetBestFitStatistic()) < 1E-5);
  }

  // Write start point
  o_chisq = current_chisq;
  TVectorD bf_vec = best_fit_params->AsTVector();
  std::copy(bf_vec.GetMatrixArray(),bf_vec.GetMatrixArray()+npar, o_params);
  out_tr->Fill();

  double mean_n_attempts = 0;
  int n_steps_in_mean = 0;

  // PCA class to easily calculate the covariance
  TPrincipal pca(npar);

  // Record time
  TStopwatch clock;

  for (int istep = 1 ; istep < nsteps ; istep++) {
    // Every N steps, print out a status
    if (istep%print_step_every == 0 && istep) {
      LOG("adaptive_mcmc",pNOTICE)<<istep<<"\t"<<clock.RealTime()<<"\t";
      //~ std::cout<<proposal_step<<std::endl;
      out_tr->AutoSave();

      clock.Start();
    }

    // Every N steps, recalculate the step size so we're trying ~target_n_attempts throws per evaluation
    if (!disable_adaptive && istep%recalculate_step_every == 0) {
      if (mean_n_attempts > target_n_attempts * 1.5) {
        proposal_step *= 0.8;
      }
      else if (mean_n_attempts < target_n_attempts * 0.6) {
        proposal_step *= 1.2;
      }
      mean_n_attempts = 0.;
      n_steps_in_mean = 0;
    }

    int n_attempts;
    bool success = util::AttemptMCMCStep(fitinputs,current_chisq,n_attempts,proposal_step,&chol_matrix);

    // Recalculate mean attempts
    n_steps_in_mean++;
    mean_n_attempts *= 1. - 1./(n_steps_in_mean);
    mean_n_attempts += ((double)n_attempts)/(n_steps_in_mean);

    if (!success) {
      if (!disable_adaptive) {
        proposal_step *= 0.1; // If we failed, do smaller steps
        LOG("adaptive_mcmc",pNOTICE)<<"New proposal step is "<<proposal_step;
      }
      continue;
    }
    o_chisq = current_chisq;
    TVectorD par_vec = best_fit_params->AsTVector();
    std::copy(par_vec.GetMatrixArray(),par_vec.GetMatrixArray()+npar, o_params);
    out_tr->Fill();

    if (istep > burn_in) {
      pca.AddRow(o_params);
    }

  } //istep


  // Write the ttree
  output_file->cd();
  out_tr->Write();

  // Get the covariance back
  TMatrixD cov_out(util::LowerTriangularMatrixToSymMatrix(*(pca.GetCovarianceMatrix())));
  cov_out.Write("cov");

  prefit_cov.Write("cov_in");

  const TVectorD mean_values(*(pca.GetMeanValues()));

  // Write out the ratio of mean to best_fit
  ostringstream summary;
  summary<<"Mean from chain / best fit: "<<std::endl;;
  for (int ip = 0 ; ip < npar; ip++ ) {
    double mean = mean_values[ip];
    double bf = best_fit_params->BestFitValueByIndex(ip);
    summary<<best_fit_params->NameByIndex(ip)<<"\t"<< mean / bf<<endl;
  }
  LOG("adaptive_mcmc",pNOTICE)<<summary.str();

  LOG("mcmc_step",pNOTICE)<<"Final step size was: "<<proposal_step;

  output_file->Close();
  delete[] o_params;

  return 0;
}

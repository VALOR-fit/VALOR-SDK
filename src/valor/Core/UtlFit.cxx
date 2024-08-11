//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

ChangeLog:

*/
//________________________________________________________________________________________

#include <iostream>
#include <cmath>

#include <TMath.h>
#include <TString.h>
#include <TRandom3.h>
#include <TVectorD.h>
#include <TPrincipal.h>

#include "valor/Conventions/Constants.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/UtlNumStat.h"

#include "valor/Core/FitParamList.h"
#include "valor/Core/Likelihood.h"
#include "valor/Core/FitterABC.h"
//#include "valor/Core/FitterGSL.h"
#include "valor/Fitters/FitterMinuit.h"
#include "valor/Fitters/FitterMinuit2.h"
#include "valor/Core/UtlFit.h"

using namespace valor;

using std::cerr;
using std::cout;
using std::endl;

FitterABC * valor::util::GetFitter (Likelihood * lkhd, Option_t * option)
{
  if(!lkhd)   return 0;
  if(!option) return 0;

  TString opt(option);

  if ( opt.Contains("minuit2") || opt.Contains ("fumili") )
  {
     cout << "Fitter >> Instantianting MINUIT2 fitter with option: " << option << endl;
     return new FitterMinuit2(lkhd,option);
  }

  if ( opt.Contains("minuit") )
  {
     cout << "Fitter >> Instantianting MINUIT fitter with option: " << option << endl;
     return new FitterMinuit(lkhd,option);
  }
/*
  if ( opt.Contains("SimAnneal")   ||
       opt.Contains("LevMarq")     ||
       opt.Contains("ConjugateFR") ||
       opt.Contains("ConjugatePR") ||
       opt.Contains("VectorBFGS")  ||
       opt.Contains("SteepestDescent")
     )
  {
     cout << "Fitter >> Instantianting GSL fitter with option: " << option << endl;
     return new FitterGSL(inputs,option);
  }
*/
  cerr << "Fitter >> No fitter was instantiated. Input fitter option was: " << option << endl;
  return 0;
}
//______________________________________________________________________________
double valor::util::RandomiseParametersByAcceptance(
  Likelihood & lkhd,
  const TMatrixD * cholesky_for_throws, int max_attempts, double scaling_factor)
{
  TRandom3 & rnd = Rnd::Instance()->RndOther();
  // For now we don't respect parameters fixed in the fit. Should we?
  FitParamList & fp = lkhd.FitParams();
  double chisqmin = fp.GetBestFitStatistic();
  int np = fp.Size();

  for (int i_attempt = 0; i_attempt < max_attempts; i_attempt++) {

    double acceptance_chisq = -2. * std::log(rnd.Uniform(0,1));
    double throw_chisq = 0;
    // Randomise the parameters
    if (cholesky_for_throws) {
      TVectorD throwvec(np);
      for (int ip = 0 ; ip <np; ip++) {
        double x = rnd.Gaus();
        throw_chisq += x*x;
        throwvec[ip] = x;
      }
      // Correlate the throws
      throwvec *= (*cholesky_for_throws);
      for (int ip = 0 ; ip <np; ip++) {
        double mean  = fp.BestFitValueByIndex(ip);
        double val = throwvec[ip] + mean;;
        val = std::min(val,fp.MaxValueByIndex(ip));
        val = std::max(val,fp.MinValueByIndex(ip));
        fp.SetCurrentValueByIndex(ip,val);
      }
    }
    else {
      for (int ip = 0 ; ip <np; ip++) {
        double mean  = fp.BestFitValueByIndex(ip);
        double sigma = fp.MigradErrorByIndex(ip);
        double x = rnd.Gaus(mean,sigma);
        double var = (x-mean)/sigma;
        throw_chisq += var*var;
        fp.SetCurrentValueByIndex(ip,x);
      }
    }

    double chisq = lkhd.Statistic();
    double dchisq = chisq - chisqmin;
    double final_chisq = dchisq-throw_chisq;

    if (final_chisq < acceptance_chisq - std::log(scaling_factor)) {
      return final_chisq;
    }
  }

  VLOG("UtlFit",pERROR)<<"Failed to properly randomise the parameters";

  return conv::kBadResult;
}
//______________________________________________________________________________
TMatrixDSym valor::util::CalculateCovariance(std::vector<PhysParamList> param_sets,
  const CorrelatedSystematics & example_format)
{
  int nvar = param_sets[0].NParams();
  std::vector<TVectorD> all_vec;
  TPrincipal pca(nvar);
  for (unsigned int i = 0; i < param_sets.size(); i++) {
    all_vec.push_back(param_sets[i].AsTVector(&example_format));
    pca.AddRow(all_vec.back().GetMatrixArray());
  }
  TMatrixD lower_triangle_cov(*(pca.GetCovarianceMatrix()));
  return util::LowerTriangularMatrixToSymMatrix(lower_triangle_cov);
}
//______________________________________________________________________________
bool valor::util::AttemptMCMCStep(Likelihood & lkhd,
  double & current_chisq, int & n_attempts_output, double step_size_factor,
  const TMatrixD * cholesky_for_throws, int max_attempts)
{
  const bool respect_fixed  = true;
  const bool respect_limits = false;
  TRandom3 & rnd = Rnd::Instance()->RndOther();
  FitParamList & fp = lkhd.FitParams();

  int npar = fp.Size();
  //~ int nparfree = fp.GetNumberFreeParameters();
  TVectorD start_values(fp.AsTVector("current"));

  for (int i_attempt = 0; i_attempt < max_attempts; i_attempt++) {

    bool bad_throw = false;

    // Randomise the parameters
    if (cholesky_for_throws) { // propose based on correlations
      TVectorD throwvec(npar);
      for (int ip = 0 ; ip <npar; ip++) {
        throwvec[ip] = rnd.Gaus();
      }
      // Correlate the throws
      TVectorD correlated_throws(throwvec);
      correlated_throws *= (*cholesky_for_throws);
      for (int ip = 0 ; ip <npar; ip++) {
        if (fp.IsFixedByIndex(ip) && respect_fixed) {
          continue;
        }
        double current  = start_values[ip];
        double corr_step = step_size_factor * correlated_throws[ip];
        //~ double unco_step = step_size_factor * throwvec[ip];
        double val = corr_step + current;

        // Reject the throw if we go outside the limits
        if (respect_limits && (val > fp.MaxValueByIndex(ip) || val < fp.MinValueByIndex(ip))) {
          bad_throw = true;
          //~ cerr<<fp.NameByIndex(ip)<<"\t"<<val<<"\t"<<fp.MinValueByIndex(ip)<<"\t"<<fp.MaxValueByIndex(ip)<<std::endl;
          break;
        }
        fp.SetCurrentValueByIndex(ip,val);
      }
    }
    else { // random walk based on MIGRAD error
      for (int ip = 0 ; ip < npar; ip++) {
        if (fp.IsFixedByIndex(ip) && respect_fixed) {
          continue;
        }
        double current  = start_values[ip];
        double sigma = step_size_factor * fp.MigradErrorByIndex(ip);
        double val = rnd.Gaus(current,sigma);
        if (respect_limits && (val > fp.MaxValueByIndex(ip) || val < fp.MinValueByIndex(ip))) {
          bad_throw = true;
          //~ cerr<<fp.NameByIndex(ip)<<"\t"<<val<<"\t"<<fp.MinValueByIndex(ip)<<"\t"<<fp.MaxValueByIndex(ip)<<std::endl;
          break;
        }
        fp.SetCurrentValueByIndex(ip,val);
      }
    }

    // If our throw was bad, don't evaluate the likelihood, and thus don't count as an attempt
    if (bad_throw) {
      i_attempt--;
      continue;
    }

    double step_chisq = lkhd.Statistic();
    // The acceptance probability is: A = (P(curr) * q(curr) / (P(step) * q(step))
    // We're using a symmetric proposal so q(curr)==q(step)
    double delta_chisq = step_chisq-current_chisq;
    //~ double step_prob = std::exp( -delta_chisq/2. );
    double rand_prob = rnd.Uniform();
    double rand_chisq = -2 * std::log(rand_prob);
    //~ assert((delta_chisq < rand_chisq)==(step_prob > rand_prob));
    //~ if (step_prob > rand_prob) {
    if (delta_chisq < rand_chisq) {
      n_attempts_output = i_attempt+1;
      VLOG("UtlFit",pINFO)<<"Made a step after "<<n_attempts_output<<" attempts. "
        <<"Delta chisq was "<<Form("%.2lg",delta_chisq)<<", random chisq was "<<rand_chisq;
      VLOG("UtlFit",pDEBUG)<<"Step chisq = "<<step_chisq<<", prev chisq = "<<current_chisq << ", stepped chisq = "<<step_chisq;
      current_chisq = step_chisq;
      VLOG("UtlFit",pDEBUG)<<fp;
      return true;
    }
    VLOG("UtlFit",pDEBUG)<<"Step not made - delta chisq was "<<Form("%.2lg",delta_chisq)<<", random chisq was "<<rand_chisq;
  }

  VLOG("UtlFit",pERROR)<<"Failed to successfully make a step";
  for (int ip = 0 ; ip < npar; ip++) {
    fp.SetCurrentValueByIndex(ip,start_values[ip]);
  }
  n_attempts_output = max_attempts;
  return false;
}
//______________________________________________________________________________

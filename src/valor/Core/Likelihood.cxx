//____________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

//#define __show_bin_contributions_to_chisq__
//#define __show_contributions_to_penalty_term__
//#define __show_chisq__

#include <cassert>
#include <cstdlib>
#include <iostream>

#include <TMinuit.h>
#include <TString.h>
#include <TMath.h>
#include "Math/FitMethodFunction.h"
#include "HFitInterface.h"

#include "valor/Messenger/Messenger.h"
// #include "valor/Conventions/Constants.h"
// #include "valor/Conventions/Opt.h"
#include "valor/Core/FitDataABC.h"
#include "valor/Core/FitModelABC.h"
#include "valor/Core/Likelihood.h"

using std::cout;
using std::cerr;
using std::endl;

using namespace valor;
using namespace valor::conv;

ClassImp(Likelihood)

//____________________________________________________________________________
Likelihood::Likelihood(
   FitDataABC * o, FitModelABC * e, FitParamList * fpar) :
fObs(o),
fExp(e),
fFitParams(fpar),
fIncludePenaltySyst(true),
fIncludePenaltyHyp (true)
{

}
//____________________________________________________________________________
Likelihood::~Likelihood()
{

}
//____________________________________________________________________________
void Likelihood::UpdateParamList(FitParamList * fpar)
{
  fFitParams = fpar;
}
//____________________________________________________________________________
FitDataABC & Likelihood::Observation(void)
{
  return *fObs;
}
//____________________________________________________________________________
FitModelABC & Likelihood::Expectation(void)
{
  return *fExp;
}
//____________________________________________________________________________
FitParamList & Likelihood::FitParams(void)
{
  return *fFitParams;
}
//____________________________________________________________________________
double Likelihood::Statistic(
  Option_t * option, std::map<std::string,double> * likelihood_components) const
{
  bool goodnessOfFit = false;

  double total_statistic = 0;
  double data_statistic  = 0;
  double syst_penalty    = 0;
  double phys_penalty    = 0;
  if (option || goodnessOfFit) {
    const char * s = goodnessOfFit ? "%s gof" : "%s";
    TString newopt =  TString::Format(s,option);
    data_statistic =  fObs->GetFitStatistic(fExp,*fFitParams, newopt.Data(), likelihood_components);
  }
  else {
    data_statistic = fObs->GetFitStatistic(fExp,*fFitParams, NULL, likelihood_components);
  }
  TString option_ts(option);
  if (!option_ts.Contains("nopenalty")){
    if (fIncludePenaltySyst) {syst_penalty = fExp->GetFitStatisticPenalty(option);}
    if (fIncludePenaltyHyp ) {phys_penalty = fFitParams->HypothesisFitStatisticPenalty(option);}
  }

  if (likelihood_components) {
    (*likelihood_components)["penalty_syst"] = syst_penalty;
    (*likelihood_components)["penalty_hyp" ] = phys_penalty;
  }
  total_statistic = syst_penalty + phys_penalty;
#ifndef __VALOR_FMWK_PENALTY_ONLY__
  total_statistic += data_statistic;
#endif
  VLOG("Likelihood",pINFO)
    << "Statistic = " << total_statistic
    << " (data/MC = " << data_statistic
    << ", syst penalty = " <<syst_penalty
    << ", phys penalty = " <<phys_penalty << ")";

  assert(TMath::Finite(total_statistic));
  return total_statistic;
}
//____________________________________________________________________________
void Likelihood::IncludePenaltySyst(bool use_penalty)
{
  fIncludePenaltySyst = use_penalty;
}
//____________________________________________________________________________
void Likelihood::IncludePenaltyHyp(bool use_penalty)
{
  fIncludePenaltyHyp = use_penalty;
}
//____________________________________________________________________________

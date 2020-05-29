//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//________________________________________________________________________________________

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
#include "valor/Conventions/Constants.h"
#include "valor/Conventions/Opt.h"
#include "valor/FitCore/FitDataABC.h"
#include "valor/FitCore/FitModelABC.h"
#include "valor/FitCore/Likelihood.h"

using std::cout;
using std::cerr;
using std::endl;

using namespace valor;
using namespace valor::conv;

ClassImp(Likelihood)

//________________________________________________________________________________________
Likelihood::Likelihood(
   FitDataABC * o, FitModelABC * e, FitParamList * fpar, string opt, double errdef, bool goodnessoffit) :
fObs(o),
fExp(e),
fFitParams(fpar),
fOpt(opt),
fErrDef(errdef),
fGoodnessOfFit(goodnessoffit),
fIncludePenaltySyst(true),
fIncludePenaltyHyp (true)
{

}
//________________________________________________________________________________________
Likelihood::~Likelihood()
{

}
//________________________________________________________________________________________
void Likelihood::UpdateParamList(FitParamList * fpar)
{
  fFitParams = fpar;
}
//________________________________________________________________________________________
FitDataABC & Likelihood::Observation(void)
{
  return *fObs;
}
//________________________________________________________________________________________
FitModelABC & Likelihood::Expectation(void)
{
  return *fExp;
}
//________________________________________________________________________________________
FitParamList & Likelihood::FitParams(void)
{
  return *fFitParams;
}
//________________________________________________________________________________________
double Likelihood::Statistic(Option_t * option, std::map<std::string,double> * likelihood_components) const
{
  double total_statistic = 0;
  double data_statistic  = 0;
  double syst_penalty    = 0;
  double phys_penalty    = 0;
  if (option || fGoodnessOfFit) {
    const char * s = fGoodnessOfFit ? "%s gof" : "%s";
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
  LOG("Likelihood",pINFO)
    <<"Statistic = "<<total_statistic<<" (data/MC="<<data_statistic
    <<", syst penalty="<<syst_penalty<<", phys penalty="<<phys_penalty<<")";

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
// MINUIT fit functions
//____________________________________________________________________________
double Likelihood::operator() (const std::vector<double>& x) const
{
 fFitParams->Sync(x);

 return this->Statistic("poisson");
}
//____________________________________________________________________________
void valor::FitFunc (
  Int_t &, Double_t *, Double_t &f, Double_t *par, Int_t /*iflag*/)
{
// To be used with a TVirtualFitter

  Likelihood * lkhd = (Likelihood *) gMinuit->GetObjectFit();

  // get fit params
  FitParamList & fpar = lkhd->FitParams();

  // synchronize FitParamList with the paramerer array used by MINUIT
  fpar.Sync(par,true);

  f = lkhd->Statistic();

  fpar.SetCurrStatistic(f);
}
//____________________________________________________________________________
FitFunctor::FitFunctor(Likelihood * lkhd)
{
  fLikelihood = lkhd;
}
//____________________________________________________________________________
FitFunctor::~FitFunctor()
{

}
//____________________________________________________________________________
double FitFunctor::operator() (const double * x) const
{
  return this->DoEval(x);
}
//____________________________________________________________________________
double FitFunctor::DoEval(const double * x) const
{
  fLikelihood->FitParams().Sync(x);

  return fLikelihood->Statistic();
}
//____________________________________________________________________________
ROOT::Math::IBaseFunctionMultiDim * FitFunctor::Clone(void) const
{
  return new FitFunctor(fLikelihood);
}
//____________________________________________________________________________
unsigned int FitFunctor::NDim(void) const
{
  return fLikelihood->FitParams().GetNumberParameters();
}
//____________________________________________________________________________
FitMethodFunc::~FitMethodFunc()
{

}
//____________________________________________________________________________
double FitMethodFunc::operator() (const double * x) const
{
  return this->DoEval(x);
}
//____________________________________________________________________________
double FitMethodFunc::DoEval(const double * x) const
{
  fLikelihood->FitParams().Sync(x);
  return fLikelihood->Statistic("poisson");
}
//____________________________________________________________________________
ROOT::Math::BasicFitMethodFunction<ROOT::Math::IBaseFunctionMultiDim>::Type_t
 FitMethodFunc::Type(void) const
{
  return ROOT::Math::FitMethodFunction::kLeastSquare;
}
//____________________________________________________________________________
double FitMethodFunc::DataElement(
    const double *par, unsigned int /*ipoint*/, double * /* g */) const
{
  fLikelihood->FitParams().Sync(par);
  double chisqbin = 0;
  assert(!"FitMethodFunc::DataElement not implemented yet");
  //Todo: Implement
  return chisqbin;
}
//____________________________________________________________________________

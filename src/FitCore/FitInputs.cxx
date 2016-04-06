//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

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

#include "Conventions/Constants.h"
#include "Conventions/Opt.h" 
#include "FitCore/FitDataABC.h"
#include "FitCore/FitModelABC.h"
#include "FitCore/FitInputs.h"

using std::cout;
using std::cerr;
using std::endl;

using namespace valor;
using namespace valor::conv;

ClassImp(FitInputs)

//________________________________________________________________________________________
FitInputs::FitInputs(
   FitDataABC * o, FitModelABC * e, FitParamList * fpar, string opt, double errdef, bool goodnessoffit) :
fObs(o), 
fExp(e), 
fFitParams(fpar),
fOpt(opt),
fErrDef(errdef),
fGoodnessOfFit(goodnessoffit)
{ 

}
//________________________________________________________________________________________
FitInputs::~FitInputs()  
{ 

}
//________________________________________________________________________________________
void FitInputs::UpdateParamList(FitParamList * fpar)
{
  fFitParams = fpar;
}
//________________________________________________________________________________________
FitDataABC & FitInputs::Observation(void) 
{ 
  return *fObs;       
}
//________________________________________________________________________________________
FitModelABC & FitInputs::Expectation(void) 
{ 
  return *fExp;       
}
//________________________________________________________________________________________
FitParamList & FitInputs::FitParams(void) 
{ 
  return *fFitParams; 
}
//________________________________________________________________________________________
double FitInputs::Statistic(Option_t * option) const
{
  double statistic = 0;
  if (option || fGoodnessOfFit) {
    const char * s = fGoodnessOfFit ? "%s gof" : "%s";
    Option_t * newopt( Form(s,option) );
    statistic =  fObs->Statistic(fExp,*fFitParams, newopt);
  }
  else {
    statistic = fObs->Statistic(fExp,*fFitParams, NULL);
  }
  return statistic;
}
//____________________________________________________________________________ 
// MINUIT fit functions 
//____________________________________________________________________________ 
double FitInputs::operator() (const std::vector<double>& /*x*/) const 
{
// To be used with Minuit2
/* 
 fFitParams->Sync(x);

 //return  this->Chisquare(fOpt.c_str());
 return  this->Chisquare(fMode);
*/
  return 0;
}
//____________________________________________________________________________ 
void valor::FitFunc (
  Int_t &, Double_t *, Double_t &f, Double_t *par, Int_t /*iflag*/) 
{ 
// To be used with a TVirtualFitter

  FitInputs * inputs = (FitInputs *) gMinuit->GetObjectFit();

  // get fit params
  FitParamList & fpar = inputs->FitParams(); 

  // synchronize FitParamList with the paramerer array used by MINUIT
  fpar.Sync(par);

  // judge the agreement between the data and the current prediction
  FitDataABC &  data_abc  = inputs->Observation();
  FitModelABC & model_abc = inputs->Expectation();
  
  string fit_opt(Form("likelihood-ratio%s",(inputs->GetChisqDefinition() ? " gof" : "")));
  f = data_abc.Statistic(&model_abc,fpar,fit_opt.c_str());
  fpar.SetCurrStatistic(f);

  inputs->Statistic(); ///
}
//____________________________________________________________________________
FitFunctor::FitFunctor(FitInputs * inputs) 
{ 
  fFitInputs = inputs; 
}
FitFunctor::~FitFunctor() 
{

}
double FitFunctor::operator() (const double * x) const
{
  return this->DoEval(x);
}
double FitFunctor::DoEval(const double * x) const 
{
  fFitInputs->FitParams().Sync(x);
/*
	HACK
  return fFitInputs->Chisquare("poisson");
*/
	return 0;
}
ROOT::Math::IBaseFunctionMultiDim * FitFunctor::Clone(void) const 
{
  return new FitFunctor(fFitInputs);
}
unsigned int FitFunctor::NDim(void) const 
{ 
  return fFitInputs->FitParams().GetNumberParameters();
}
//____________________________________________________________________________
FitMethodFunc::~FitMethodFunc() 
{

}
double FitMethodFunc::operator() (const double * x) const
{
  return this->DoEval(x);
}
double FitMethodFunc::DoEval(const double * x) const 
{
  fFitInputs->FitParams().Sync(x);
/* HACK
  return fFitInputs->Chisquare("poisson");
*/
   	return 0;
}
ROOT::Math::BasicFitMethodFunction<ROOT::Math::IBaseFunctionMultiDim>::Type_t FitMethodFunc::Type(void) const
{
  return ROOT::Math::FitMethodFunction::kLeastSquare;
}


double  FitMethodFunc::DataElement(const double *par, unsigned int /*ipoint*/, double * /* g */) const 
{
  fFitInputs->FitParams().Sync(par);
  double chisqbin = 0;
  assert(!"FitMethodFunc::DataElement not implemented yet");
  //Todo: Implement
  return chisqbin;
}
//____________________________________________________________________________


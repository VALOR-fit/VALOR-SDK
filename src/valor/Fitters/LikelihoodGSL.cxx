//____________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#include <cassert>
#include <cstdlib>

#include "Math/FitMethodFunction.h"
#include "HFitInterface.h"

#include "valor/Messenger/Messenger.h"
#include "valor/Core/Likelihood.h"
#include "valor/Fitters/LikelihoodGSL.h"

using namespace valor;
using namespace valor::conv;

ClassImp(LikelihoodGSL)

//____________________________________________________________________________
LikelihoodGSL::LikelihoodGSL(Likelihood * likelihood) :
fLikelihood(likelihood)
{

}
//____________________________________________________________________________
LikelihoodGSL::~LikelihoodGSL()
{

}
//____________________________________________________________________________
ROOT::Math::IMultiGenFunction * LikelihoodGSL::AsFunctor(void)
{
  return new FitFunctor(this);
}
//____________________________________________________________________________
FitFunctor::FitFunctor(LikelihoodGSL * lkhd_gsl)
{
  fLikelihoodGSL = lkhd_gsl;
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
  fLikelihoodGSL->Kernel()->FitParams().Sync(x);
  return fLikelihoodGSL->Kernel()->Statistic();
}
//____________________________________________________________________________
ROOT::Math::IBaseFunctionMultiDim * FitFunctor::Clone(void) const
{
  return new FitFunctor(fLikelihoodGSL);
}
//____________________________________________________________________________
unsigned int FitFunctor::NDim(void) const
{
  return fLikelihoodGSL->Kernel()->FitParams().GetNumberParameters();
}
//____________________________________________________________________________

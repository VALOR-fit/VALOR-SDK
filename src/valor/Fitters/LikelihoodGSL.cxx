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
ROOT::Math::FitMethodFunction * LikelihoodGSL::AsFitMethodFunction(void)
{
  return new FitMethodFunc(this->Kernel()->GetNDim(), this->Kernel()->GetNPoints(), this);
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
FitMethodFunc::FitMethodFunc( int dim, int npoints, LikelihoodGSL * lkhd_gsl ):
  ROOT::Math::FitMethodFunction(dim, npoints), fLikelihood( lkhd_gsl ) {};
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
  fLikelihood->Kernel()->FitParams().Sync(x);
  return fLikelihood->Kernel()->Statistic("poisson"); // as in v1.5.0 - can look at this?
}
//____________________________________________________________________________
ROOT::Math::IMultiGenFunction * FitMethodFunc::Clone(void) const
{
  return new FitMethodFunc(*this);
}
//____________________________________________________________________________
ROOT::Math::BasicFitMethodFunction<ROOT::Math::IBaseFunctionMultiDim>::Type_t
FitMethodFunc::Type(void) const
{
  return ROOT::Math::FitMethodFunction::kLeastSquare;
}
//____________________________________________________________________________
double FitMethodFunc::DataElement( const double * x,
				   unsigned int /*i*/, double * /*g*/,
				   double * /*h*/, bool /*fullHessian*/ ) const
{
  fLikelihood->Kernel()->FitParams().Sync(x);
  double chisqbin = 0.0;
  VLOG( "LikelihoodGSL", pERROR ) << "FitMethodFunc::DataElement not implemented yet!";
  // RETHERE
  return chisqbin;
}
//____________________________________________________________________________

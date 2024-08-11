//____________________________________________________________________________
/*!

\class     valor::LikelihoodGSL

\brief     VALOR likelihood wrapper object for GSL.

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   May 30, 2020

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_likelihood_gsl_h_
#define _valor_likelihood_gsl_h_

#include <Math/IFunction.h>
#include <Math/FitMethodFunction.h>
#include <HFitInterface.h>

#include "valor/Core/FitParamList.h"

namespace valor {

class Likelihood;
class LikelihoodGSL;

//____________________________________________________________________________
//
//
class FitFunctor: public ROOT::Math::IMultiGenFunction
{
public:
  FitFunctor(LikelihoodGSL * lkhd_gsl);
  ~FitFunctor();
  double operator() (const double * x) const;
  double DoEval(const double * x) const;
  ROOT::Math::IMultiGenFunction * Clone(void) const;
  unsigned int NDim(void) const;
private:
  LikelihoodGSL * fLikelihoodGSL;
};
//____________________________________________________________________________
//
//
class LikelihoodGSL
{
public :
  LikelihoodGSL(Likelihood * likelihood);
  ~LikelihoodGSL();

  // Return the core VALOR likelihood object
  Likelihood * Kernel(void) const { return fLikelihood; }

  // --------------------
  // GSL-specific methods
  // --------------------

  // Return likelihood object as a IMultiGenFunction
  ROOT::Math::IMultiGenFunction * AsFunctor(void);

private:

  mutable Likelihood * fLikelihood;

ClassDef(LikelihoodGSL, 1)
};

} // valor

#endif

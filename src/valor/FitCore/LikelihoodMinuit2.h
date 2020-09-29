//____________________________________________________________________________
/*!

\class     valor::LikelihoodMinuit2

\brief     VALOR likelihood wrapper object for Minuit2.

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   October 20, 2010

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_likelihood_minuit2_h_
#define _valor_likelihood_minuit2_h_

#include <Minuit2/FumiliFCNBase.h>

namespace valor {

class Likelihood;

class LikelihoodMinuit2: public ROOT::Minuit2::FCNBase
{
public :
  LikelihoodMinuit2(Likelihood * likelihood);
 ~LikelihoodMinuit2();

  // Return the core VALOR likelihood object
  Likelihood * Kernel(void) const { return fLikelihood; }

  // ------------------------
  // Minuit2-specific methods
  // ------------------------

  // Likelihood calculation
  double operator() (const std::vector<double>& x) const;

  // Minimum chi2 fit (delta chi2 = 1 for 1 sigma error)
  double Up          (void) const    { return fErrDef;   }
  double ErrorDef    (void) const    { return Up();      }
  void   SetErrorDef (double errdef) { fErrDef = errdef; }

  // Might be used for Fumili
  // ROOT::Math::FitMethodFunction * AsFitMethodFunction(void)
  // { return new FitMethodFunc(this->GetNDim(),this->GetNPoints(),this); }

private:

  mutable Likelihood * fLikelihood;
  double  fErrDef;

ClassDef(LikelihoodMinuit2, 1)
};

// //
// // The following helper class might be used for Fumili
// //
// class FitMethodFunc : public ROOT::Math::FitMethodFunction
// {
// public:
//   FitMethodFunc( int dim, int npoints, Likelihood * lkhd) :
//      ROOT::Math::FitMethodFunction(dim,npoints),
//      fLikelihood(lkhd) {}
//   ~FitMethodFunc();
//   double operator() (const double * x) const;
//   ROOT::Math::IMultiGenFunction * Clone(void) const{
//     return new FitMethodFunc(*this);
//   }
//   double DataElement(const double* x, unsigned int i, double* g = 0) const;
//   virtual  ROOT::Math::BasicFitMethodFunction<ROOT::Math::IBaseFunctionMultiDim>::Type_t Type(void) const ;
// private:
//   mutable Likelihood * fLikelihood;
//   virtual double DoEval(const double * x) const;
// };

} // valor

#endif

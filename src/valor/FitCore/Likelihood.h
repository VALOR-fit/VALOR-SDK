//____________________________________________________________________________
/*!

\class     valor::Likelihood

\brief     Bundles fitter inputs into a single TObject-derived class so that
           they can be passed to the minuit fitting function via
           TMinuit::Set/GetObjectFit();

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   October 20, 2010

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_likelihood_h_
#define _valor_likelihood_h_

#include <Minuit2/FumiliFCNBase.h>
#include <Math/IFunction.h>
#include <Math/FitMethodFunction.h>
#include <HFitInterface.h>

#include "valor/FitCore/FitParamList.h"

namespace valor {

class FitDataABC;
class FitModelABC;
class Likelihood;

//____________________________________________________________________________
class FitFunctor: public ROOT::Math::IMultiGenFunction
{
public:
  FitFunctor(Likelihood * lkhd);
  ~FitFunctor();
  double operator() (const double * x) const;
  double DoEval(const double * x) const;
  //ROOT::Math::IBaseFunctionMultiDim * Clone(void) const;
  ROOT::Math::IMultiGenFunction * Clone(void) const;
  unsigned int NDim(void) const;
 private:
  Likelihood * fLikelihood;
};
//____________________________________________________________________________
class FitMethodFunc : public ROOT::Math::FitMethodFunction
{
 public:
 FitMethodFunc( int dim, int npoints, Likelihood * lkhd) :
  ROOT::Math::FitMethodFunction(dim,npoints),
    fLikelihood(lkhd) {}
  ~FitMethodFunc();
  double operator() (const double * x) const;
  ROOT::Math::IMultiGenFunction * Clone(void) const{
    return new FitMethodFunc(*this);
  }

  double DataElement(const double* x, unsigned int i, double* g = 0) const;

  virtual  ROOT::Math::BasicFitMethodFunction<ROOT::Math::IBaseFunctionMultiDim>::Type_t Type(void) const ;

 private:
  mutable Likelihood * fLikelihood;
  virtual double DoEval(const double * x) const;
};
//____________________________________________________________________________
class Likelihood: public ROOT::Minuit2::FCNBase, public TObject
{
public :
  Likelihood(FitDataABC * o, FitModelABC * e, FitParamList * p,
    string opt = "poisson", double errdef = 1, bool goodnessoffit = false);
 ~Likelihood();

  FitDataABC &    Observation (void);
  FitModelABC &   Expectation (void);
  FitParamList &  FitParams   (void);

  void UpdateParamList(FitParamList * fpar);

  double Statistic(Option_t * stat_opt = "likelihood-ratio", std::map<std::string,double> * likelihood_components = 0) const;

  // set the chisq definition 5 bins vs 73 bins
  void SetChisqDefinition (bool goodnessoffit) { fGoodnessOfFit = goodnessoffit; }
  bool GetChisqDefinition (void) { return fGoodnessOfFit; }

  // Turn the penalty term off or on (defaults to on)
  void IncludePenaltyHyp(bool use_penalty);
  void IncludePenaltySyst(bool use_penalty);

  // Set the chisquare type (e.g. poisson) to be used in the fit
  void SetChisquareFitOption(Option_t * opt) { fOpt = opt; }

  // Minuit2: Minimum chi2 fit (delta chi2 = 1 for 1 sigma error)
  double Up          (void) const    { return fErrDef;   }
  double ErrorDef    (void) const    { return Up();   }
  void   SetErrorDef (double errdef) { fErrDef = errdef; }

  // Minuit2: Returns chi^2
  double operator() (const std::vector<double>& x) const;

  // GSL
  // ROOT::Math::IBaseFunctionMultiDim * AsFunctor(void) { return new FitFunctor(this); }
  ROOT::Math::IMultiGenFunction * AsFunctor(void) { return new FitFunctor(this); }
  ROOT::Math::FitMethodFunction * AsFitMethodFunction(void) { return new FitMethodFunc(this->GetNDim(),this->GetNPoints(),this); }

  int GetNDim    (void) const {return fNDim;   }
  int GetNPoints (void) const {return fNPoints;}

private:

  FitDataABC *            fObs;
  mutable FitModelABC *   fExp;
  mutable FitParamList *  fFitParams;
  string                  fOpt;
  int                     fMode;
  double                  fErrDef;
  bool                    fGoodnessOfFit;
  int                     fNPoints;
  int                     fNDim;

  bool                    fIncludePenaltySyst;
  bool                    fIncludePenaltyHyp;

  ClassDef(Likelihood, 1)
};

//
// MINUIT, GSL fit functions
//

void FitFunc (Int_t &, Double_t *, Double_t &f, Double_t *par, Int_t iflag);

} // valor

#endif

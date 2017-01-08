//____________________________________________________________________________
/*!

\class     valor::FitInputs

\brief     Bundles fitter inputs into a single TObject-derived class so that 
           they can be passed to the minuit fitting function via 
           TMinuit::Set/GetObjectFit();

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   October 20, 2010

\copyright (c) 2010-2016, The VALOR group

*/
//____________________________________________________________________________

#ifndef _valor_fit_inputs_h_
#define _valor_fit_inputs_h_

#include <Minuit2/FumiliFCNBase.h>
#include <Math/IFunction.h>
#include <Math/FitMethodFunction.h>
#include <HFitInterface.h>

#include "valor/FitCore/FitParamList.h"
#include "valor/FitCore/LikelihoodABC.h"

namespace valor {
  
class FitDataABC;
class FitModelABC; 
class FitInputs;

//____________________________________________________________________________  
class FitFunctor: public ROOT::Math::IMultiGenFunction
{
public:
  FitFunctor(FitInputs * inputs);
  ~FitFunctor();
  double operator() (const double * x) const;
  double DoEval(const double * x) const;
  //ROOT::Math::IBaseFunctionMultiDim * Clone(void) const;
  ROOT::Math::IMultiGenFunction * Clone(void) const;
  unsigned int NDim(void) const;
 private:
  FitInputs * fFitInputs;
};
//____________________________________________________________________________
class FitMethodFunc : public ROOT::Math::FitMethodFunction 
{
 public:
 FitMethodFunc( int dim, int npoints,  FitInputs * inputs) :
  ROOT::Math::FitMethodFunction(dim,npoints),
    fFitInputs(inputs) {}
  ~FitMethodFunc();
  double operator() (const double * x) const;
  ROOT::Math::IMultiGenFunction * Clone(void) const{
    return new FitMethodFunc(*this);
  }
  
  double DataElement(const double* x, unsigned int i, double* g = 0) const;
  
  virtual  ROOT::Math::BasicFitMethodFunction<ROOT::Math::IBaseFunctionMultiDim>::Type_t Type(void) const ;
  
 private:
  mutable FitInputs * fFitInputs;
  virtual double DoEval(const double * x) const;
  
};
//____________________________________________________________________________
class FitInputs: public ROOT::Minuit2::FCNBase, public TObject
{
public :
  FitInputs(FitDataABC* o, FitModelABC* e, FitParamList* p,
        LikelihoodABC* l, string opt = "poisson", double errdef = 1,
        bool goodnessoffit = false);
  ~FitInputs();
  
  FitDataABC &      Observation  (void);
  FitModelABC &     Expectation  (void);
  FitParamList &    FitParams    (void);
  LikelihoodABC &   Likelihood   (void);

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
  
  int GetNDim(void) const {return fNDim;}
  int GetNPoints (void) const {return fNPoints;}

private:
  
  FitDataABC *            fObs;
  mutable FitModelABC *   fExp;
  mutable FitParamList *  fFitParams;
  LikelihoodABC *         fLikelihood;
  string                  fOpt;
  int                     fMode;
  double                  fErrDef;
  bool                    fGoodnessOfFit;
  int                     fNPoints;
  int                     fNDim;
  
  bool                    fIncludePenaltySyst;
  bool                    fIncludePenaltyHyp;
  
  ClassDef(FitInputs, 4)    
};
  
//
// MINUIT, GSL fit functions
//

void FitFunc (Int_t &, Double_t *, Double_t &f, Double_t *par, Int_t iflag);

} // valor

#endif


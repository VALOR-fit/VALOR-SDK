//____________________________________________________________________________
/*!

\class     valor::Likelihood

\brief     Basic likelihood object in VALOR.

           Single point of contact between statistical tools, and VALOR code
           for representing data and building the corresponding predictions.

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   October 20, 2010

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_likelihood_h_
#define _valor_likelihood_h_

#include "valor/FitCore/FitParamList.h"

namespace valor {

class FitDataABC;
class FitModelABC;

class Likelihood
{
public :
  Likelihood(FitDataABC * o, FitModelABC * e, FitParamList * p);
  ~Likelihood();

  FitDataABC &    Observation (void);
  FitModelABC &   Expectation (void);
  FitParamList &  FitParams   (void);

  void UpdateParamList(FitParamList * fpar);

  double Statistic(Option_t * stat_opt = "likelihood-ratio", std::map<std::string,double> * likelihood_components = 0) const;

  // Turn the penalty term off or on (defaults to on)
  void IncludePenaltyHyp (bool use_penalty);
  void IncludePenaltySyst(bool use_penalty);

  int GetNDim    (void) const {return fNDim;   }
  int GetNPoints (void) const {return fNPoints;}

private:

  FitDataABC *            fObs;
  mutable FitModelABC *   fExp;
  mutable FitParamList *  fFitParams;
  int                     fNPoints;
  int                     fNDim;
  bool                    fIncludePenaltySyst;
  bool                    fIncludePenaltyHyp;

  ClassDef(Likelihood, 1)
};

} // valor

#endif

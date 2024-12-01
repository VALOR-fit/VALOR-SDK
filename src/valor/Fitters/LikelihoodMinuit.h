//____________________________________________________________________________
/*!

\class     valor::Likelihood

\brief     VALOR likelihood wrapper object for Minuit

           Bundles the VALOR likelihood into a single TObject-derived class
           so that it can be passed to the Minuit fitting function via
           TMinuit::Set/GetObjectFit();

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   October 20, 2010

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_likelihood_minuit_h_
#define _valor_likelihood_minuit_h_

namespace valor {

class Likelihood;

class LikelihoodMinuit: public TObject
{
public :
  LikelihoodMinuit(Likelihood * likelihood);
 ~LikelihoodMinuit();

 // Return the core VALOR likelihood object
 Likelihood * Kernel(void) const { return fLikelihood; }

private:

  mutable Likelihood * fLikelihood;

ClassDef(LikelihoodMinuit, 1)
};

//
// helper MINUIT functions
//
void FitFunc (Int_t &, Double_t *, Double_t &f, Double_t *par, Int_t iflag);

} // valor

#endif

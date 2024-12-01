//____________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#include <cassert>
#include <cstdlib>

#include <TMinuit.h>

#include "valor/Messenger/Messenger.h"
#include "valor/Core/Likelihood.h"
#include "valor/Fitters/LikelihoodMinuit.h"

using namespace valor;
using namespace valor::conv;

ClassImp(LikelihoodMinuit)

//____________________________________________________________________________
LikelihoodMinuit::LikelihoodMinuit(Likelihood * likelihood) :
fLikelihood(likelihood)
{

}
//____________________________________________________________________________
LikelihoodMinuit::~LikelihoodMinuit()
{

}
//____________________________________________________________________________
void valor::FitFunc (
  Int_t &, Double_t *, Double_t &f, Double_t *par, Int_t /*iflag*/)
{
// This helper function can be used with a TVirtualFitter

  // Get VALOR likelihood object
  LikelihoodMinuit * lkhd_minuit = (LikelihoodMinuit *) gMinuit->GetObjectFit();
  Likelihood * lkhd = lkhd_minuit->Kernel();

  // Get fit params
  FitParamList & fpar = lkhd->FitParams();

  // Synchronize FitParamList with the paramerer array used by MINUIT
  fpar.Sync(par,true);

  // Calculate and set the likelihood value
  f = lkhd->Statistic();
  fpar.SetCurrStatistic(f);
}
//____________________________________________________________________________

//________________________________________________________________________________________
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <TMath.h>

#include "Examples/X01/X01Utils.h"
#include "Examples/X01/X01Const.h"

using namespace valor;

//____________________________________________________________________________
double util::example01::SigSpectrumUnosc(double E)
{
  if(E <= 0) return 0.;

  double fsignal = 
         conv::example01::kNorm * 
         TMath::Landau(E, conv::example01::kMPV, conv::example01::kSigma);
  return fsignal;
} 
//____________________________________________________________________________
double util::example01::BkgSpectrum(double E)
{
  if(E <= 0) return 0.;

  double fsig = util::example01::SigSpectrumUnosc(E);
  double frac = conv::example01::kBkgLevel0 * (1. + conv::example01::kBkgSlope*E);
  frac = TMath::Max(0.,frac);

  double fbkg = frac * fsig;
  return fbkg;
}
//____________________________________________________________________________

//____________________________________________________________________________
/*

Copyright (c) 2010-2018, The VALOR group. All rights reserved.

 */
//____________________________________________________________________________

#include <cstdlib>

#include <TMath.h>

#include "valor/Conventions/Opt.h"
#include "valor/Conventions/PhysHypothesisId.h"
#include "valor/Conventions/PhysParamId.h"
#include "valor/Conventions/Constants.h"
#include "valor/Utilities/UtlNumStat.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Oscillation/OscParams.h"
#include "valor/Oscillation/OscUtils.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

using namespace valor;
using namespace valor::conv;

//____________________________________________________________________________
double valor::util::RandomizeParameter(conv::PhysParamId_t p)
{
  switch (p) {
    case kPhypMixing23             : return util::RandomizeMixing23   ();
    case kPhypMixing13             : return util::RandomizeMixing13   ();
    case kPhypMixing12             : return util::RandomizeMixing12   ();
    case kPhypCPV                  : return util::RandomizeCPV        ();
    case kPhypDm23sq               : return util::RandomizeDm23sq     ();
    case kPhypDm12sq               : return util::RandomizeDm12sq     ();

    case kPhypMixing23Bar          : return util::RandomizeMixing23Bar();
    case kPhypMixing13Bar          : return util::RandomizeMixing13Bar();
    case kPhypMixing12Bar          : return util::RandomizeMixing12Bar();
    case kPhypCPVBar               : return util::RandomizeCPVBar     ();
    case kPhypDm23sqBar            : return util::RandomizeDm23sqBar  ();
    case kPhypDm12sqBar            : return util::RandomizeDm12sqBar  ();

      // NEW BETABAR
    case kPhypBetaBar              : return util::RandomizeBetaBar    ();
      //

    case kPhypAlphaDecay            :
    case kPhypSinsq2ThetaDecay      :
    case kPhypMusqDecoh             :
    case kPhypSinsq2ThetaDecoh      :
    case kPhypMixing14              :
    case kPhypMixing15              :
    case kPhypMixing24              :
    case kPhypMixing25              :
    case kPhypMixing34              :
    case kPhypMixing35              :
    case kPhypDm41sq                :
    case kPhypDm51sq                :
    case kPhypDelta12               :
    case kPhypDelta13               :
    case kPhypDelta15               :
    case kPhypDelta34               :
    case kPhypDelta35               :
    case kPhypNSIepsMuTau           :
#ifdef __show_parameter_randomization_warnings__
      std::cerr<<"util/RandomizeParameter > Not implemented for "<<util::RawParamAsString(p)<<" ("<<p<<")"<<std::endl;
#endif
      return -999;

    default:
      std::cerr<<"util/RandomizeParameter > Using default for "<<util::RawParamAsString(p)<<" ("<<p<<")"<<std::endl;
      return -999;
  }
#ifdef __show_parameter_randomization_warnings__
  std::cerr<<"util/RandomizeParameter > Unrecognized parameter "<<util::RawParamAsString(p)<<" ("<<p<<")"<<std::endl;
#endif
  return -999;
}
//____________________________________________________________________________
double valor::util::RandomizeMixing23(void)
{
  double mean  = Opt::Instance()->GetGlobalBestFit(kPhypMixing23);
  double sigma = mean * Opt::Instance()->GetGlobalFitError(kPhypMixing23);
  double mix   = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);
  /*
  double mix = 0;

  NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
  switch(omp) {
   case(kNuMixSinsq2Theta):
   {
     double mean  = conv::kGlobFitSinsq2Theta23;
     double sigma = conv::kGlobFitSinsq2Theta23 * conv::kGlobFitErrSinsq2Theta23;
     double sinsq2theta = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);
     mix = sinsq2theta;
     break;
   }
   case(kNuMixSinsqTheta):
   {
     double mean  = conv::kGlobFitSinsqTheta23;
     double sigma = conv::kGlobFitSinsqTheta23 * conv::kGlobFitErrSinsqTheta23;
     double sinsqtheta = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);
     mix = sinsqtheta;
     break;
   }
   default:
     util::Die(1);
  }*/

  // return if a physical value was generated
  if(mix >=0. && mix <= 1.) {
    return mix;
  }

  // or try again
  return util::RandomizeMixing23();
}
//____________________________________________________________________________
double valor::util::RandomizeMixing12(void)
{
  double mean  = Opt::Instance()->GetGlobalBestFit(kPhypMixing12);
  double sigma = mean * Opt::Instance()->GetGlobalFitError(kPhypMixing12);
  double mix   = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);
  /*
  double mix = 0;

  NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
  switch(omp) {
   case(kNuMixSinsq2Theta):
   {
     double mean  = conv::kGlobFitSinsq2Theta12;
     double sigma = conv::kGlobFitSinsq2Theta12 * conv::kGlobFitErrSinsq2Theta12;
     double sinsq2theta = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);
     mix = sinsq2theta;
     break;
   }
   case(kNuMixSinsqTheta):
   {
     double mean  = conv::kGlobFitSinsqTheta12;
     double sigma = conv::kGlobFitSinsqTheta12 * conv::kGlobFitErrSinsqTheta12;
     double sinsqtheta = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);
     mix = sinsqtheta;
     break;
   }
   default:
     util::Die(1);
  }
  * */

  // return if a physical value was generated
  if(mix >=0. && mix <= 1.) {
    return mix;
  }

  // or try again
  return util::RandomizeMixing12();
}
//____________________________________________________________________________
double valor::util::RandomizeMixing12Bar(void)
{
  double mean  = Opt::Instance()->GetGlobalBestFit(kPhypMixing12Bar);
  double sigma = mean * Opt::Instance()->GetGlobalFitError(kPhypMixing12Bar);
  double mix   = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);

  // return if a physical value was generated
  if(mix >=0. && mix <= 1.) {
    return mix;
  }

  // or try again
  return util::RandomizeMixing12Bar();
}
//____________________________________________________________________________
double valor::util::RandomizeMixing13(void)
{
  double mean  = Opt::Instance()->GetGlobalBestFit(kPhypMixing13);
  double sigma = mean * Opt::Instance()->GetGlobalFitError(kPhypMixing13);
  double mix   = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);
  /*
  double mix = 0;

  NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
  switch(omp) {
  case(kNuMixSinsq2Theta):
    {
      double mean  = conv::kGlobFitSinsq2Theta13;
      double sigma = conv::kGlobFitSinsq2Theta13 * conv::kGlobFitErrSinsq2Theta13;
      double sinsq2theta = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);
      mix = sinsq2theta;
      break;
    }
  case(kNuMixSinsqTheta):
    {
      double mean  = conv::kGlobFitSinsqTheta13;
      double sigma = conv::kGlobFitSinsqTheta13 * conv::kGlobFitErrSinsqTheta13;
      double sinsqtheta = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);
      mix = sinsqtheta;
      break;
    }
  default:
    util::Die(1);
  }*/

  // return if a physical value was generated
  if(mix >=0. && mix <= 1.) {
    return mix;
  }

  // or try again
  return util::RandomizeMixing13();
}
//____________________________________________________________________________
double valor::util::RandomizeCPV(void)
{
  NuOscDeltaCP_t cpv = Opt::Instance()->gOscDeltaCP;
  switch(cpv) {

    case (kNuDeltaCPPhase) :
    {
      double min = -conv::kpi;
      double max =  conv::kpi;
      return Rnd::Instance()->RndToyMCosc().Uniform(min,max);
    }
      break;

    case (kNuDeltaCPSin) :
    {
      double min = -1.;
      double max =  1.;
      return Rnd::Instance()->RndToyMCosc().Uniform(min,max);
    }
      break;

    default:
      return -99999;
  }
  return -99999;
}
//____________________________________________________________________________
double valor::util::RandomizeDm12sq(void)
{
  double mean  = Opt::Instance()->GetGlobalBestFit(kPhypDm12sq);
  double sigma = mean * Opt::Instance()->GetGlobalFitError(kPhypDm12sq);
  double dm    = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);
  // return if a physical value was generated
  if(dm >= 0.) {
    return dm;
  }
  //or try again
  return util::RandomizeDm12sq();
}
//____________________________________________________________________________
double valor::util::RandomizeDm23sq(void)
{
  /*
  double mean  = 0.0;
  NuOscDmsqScheme_t odmsq = Opt::Instance()->GetEffectiveDmsqScheme();
  switch (odmsq) {

  case(kNuDmsqFogliLisi):
    mean  =  kGlobFitDmsqFL;
    break;
  case(kNuDmsq12andAbs23):
    mean  = kGlobFitDm23sq;
    break;
  case(kNuDmsq12andAbs31):
    if(Opt::Instance()->gMassHierarchyAssumed == kNmhNormal)
      mean  = kGlobFitDm23sq + kGlobFitDm12sq;
    else if(Opt::Instance()->gMassHierarchyAssumed == kNmhInverted)
      mean  = kGlobFitDm23sq - kGlobFitDm12sq;
    break;
  default:
    mean  = kGlobFitDm23sq;
  }
  double sigma = mean * kGlobFitErrDm23sq;
  */
  double mean  = Opt::Instance()->GetGlobalBestFit(kPhypDm23sq);
  double sigma = mean * Opt::Instance()->GetGlobalFitError(kPhypDm23sq);
  double dm    = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);
  // return if a physical value was generated
  if(dm >= 0.) {
    return dm;
  }
  //or try again
  return util::RandomizeDm23sq();
}
//____________________________________________________________________________
double valor::util::RandomizeMixing23Bar(void)
{
  double mean  = Opt::Instance()->GetGlobalBestFit(kPhypMixing23Bar);
  double sigma = mean * Opt::Instance()->GetGlobalFitError(kPhypMixing23Bar);
  double mix   = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);

  // return if a physical value was generated
  if(mix >=0. && mix <= 1.) {
    return mix;
  }

  // or try again
  return util::RandomizeMixing23Bar();
}
//____________________________________________________________________________
double valor::util::RandomizeMixing13Bar(void)
{
  double mean  = Opt::Instance()->GetGlobalBestFit(kPhypMixing13Bar);
  double sigma = mean * Opt::Instance()->GetGlobalFitError(kPhypMixing13Bar);
  double mix   = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);

  // return if a physical value was generated
  if(mix >=0. && mix <= 1.) {
    return mix;
  }

  // or try again
  return util::RandomizeMixing13Bar();
}
//____________________________________________________________________________
double valor::util::RandomizeCPVBar(void)
{
  NuOscDeltaCP_t cpv = Opt::Instance()->gOscDeltaCP;
  switch(cpv) {

    case (kNuDeltaCPPhase) :
    {
      double min = -conv::kpi;
      double max =  conv::kpi;
      return Rnd::Instance()->RndToyMCosc().Uniform(min,max);
    }
      break;

    case (kNuDeltaCPSin) :
    {
      double min = -1.;
      double max =  1.;
      return Rnd::Instance()->RndToyMCosc().Uniform(min,max);
    }
      break;

    default:
      return -99999;
  }
  return -99999;
}
//____________________________________________________________________________
double valor::util::RandomizeDm12sqBar(void)
{
  double mean  = Opt::Instance()->GetGlobalBestFit(kPhypDm12sqBar);
  double sigma = mean * Opt::Instance()->GetGlobalFitError(kPhypDm12sqBar);
  double dm    = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);
  // return if a physical value was generated
  if(dm >= 0.) {
    return dm;
  }
  //or try again
  return util::RandomizeDm12sqBar();
}
//____________________________________________________________________________
double valor::util::RandomizeDm23sqBar(void)
{
  double mean  = Opt::Instance()->GetGlobalBestFit(kPhypDm23sqBar);
  double sigma = mean * Opt::Instance()->GetGlobalFitError(kPhypDm23sqBar);
  double dm    = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);
  // return if a physical value was generated
  if(dm >= 0.) {
    return dm;
  }
  //or try again
  return util::RandomizeDm23sqBar();
}

// NEW BETABAR
//____________________________________________________________________________
double valor::util::RandomizeBetaBar(void)
{
  double mean  = Opt::Instance()->GetGlobalBestFit(kPhypBetaBar);
  double sigma = mean * Opt::Instance()->GetGlobalFitError(kPhypBetaBar);
  double dm    = Rnd::Instance()->RndToyMCosc().Gaus(mean,sigma);
  // return if a physical value was generated
  if(dm >= 0.) {
    return dm;
  }
  //or try again
  return util::RandomizeBetaBar();
}
//____________________________________________________________________________
double valor::util::RandomizeUser(TH1D *hPrior)
{
  return hPrior->GetRandom();
}
//____________________________________________________________________________
int valor::util::RandomizeMH(const double probNH)
{

  const double min = 0.;
  const double max = 1.;
  double Pnh = probNH;
  double Pih = 1 - probNH;

  if(Pnh < 0. || Pnh > 1.){
    cerr << endl;
    cerr << "FitMarginalization::RandomizeMH" << endl;
    cerr << "The input probability for NH is not possible" << endl;
    cerr << "P(NH) = " << Pnh << endl;
    cerr << endl;
    util::Die(1);
  }
  if(Pih < 0. || Pih > 1.){
    cerr << endl;
    cerr << "FitMarginalization::RandomizeMH" << endl;
    cerr << "The input probability for IH is not possible" << endl;
    cerr << "P(IH) = " << Pih << endl;
    cerr << endl;
    util::Die(1);
  }

  double rndm = Rnd::Instance()->RndToyMCosc().Uniform(min,max);

  int MH = conv::kBadResult;
  if(rndm <= Pnh) MH = 0; // Normal Hierarchy
  else            MH = 1; // Inverted Hierarchy

  return MH;
}
//____________________________________________________________________________
// NEW OSC PRIOR
double valor::util::FromSinsq2thToSinsqth(const double sinsq2th){

  if(sinsq2th<0. || sinsq2th>1.){
    cerr << endl;
    cerr << "valor::util::FromSinsq2thToSinsqth" << endl;
    cerr << "Input parameter value of sinsq2th is not allowed" << endl;
    cerr << "sinsq2th = " << sinsq2th << endl;
    cerr << endl;
    util::Die(1);
  }

  double sinsqth = 0.5 * ( 1 - sqrt(1 - sinsq2th) );

  if(sinsqth<0. || sinsqth>1.){
    cerr << endl;
    cerr << "valor::util::FromSinsq2thToSinsqth" << endl;
    cerr << "Input parameter value of sinsqth is not allowed" << endl;
    cerr << "sinsqth = " << sinsqth << endl;
    cerr << endl;
    util::Die(1);
  }

  return sinsqth;
}
//____________________________________________________________________________
double valor::util::FromSinsqthToSinsq2th(const double sinsqth){

  if(sinsqth<0. || sinsqth>1.){
    cerr << endl;
    cerr << "valor::util::FromSinsqthToSinsq2th" << endl;
    cerr << "Input parameter value of sinsqth is not allowed" << endl;
    cerr << "sinsqth = " << sinsqth << endl;
    cerr << endl;
    util::Die(1);
  }

  double sinsq2th = 1 - (1 - 2*sinsqth)*(1 - 2*sinsqth);

  if(sinsq2th<0. || sinsq2th>1.){
    cerr << endl;
    cerr << "valor::util::FromSinsqthToSinsq2th" << endl;
    cerr << "Input parameter value of sinsq2th is not allowed" << endl;
    cerr << "sinsq2th = " << sinsq2th << endl;
    cerr << endl;
    util::Die(1);
  }

  return sinsq2th;
}
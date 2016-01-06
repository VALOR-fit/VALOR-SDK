//____________________________________________________________________________
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.

ChangeLog:

*/
//____________________________________________________________________________

//#define __show_cholesky_decomposition_mesg_debug__
//#define __show_cholesky_gen_corr_param_variations_mesg_debug__
//#define __show_xml_mesg_info__
//#define __show_xml_mesg_debug__
//#define __show_arr_idx_mesg_debug__
//#define __show_parameter_randomization_warnings__

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <TROOT.h>
#include <TMath.h>
#include <TStyle.h>
#include <TColor.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>
#include <TAxis.h>
#include <TVectorD.h>
#include <TMatrixD.h>
#include <TRandom3.h>
#include <TXMLEngine.h>
#include <TFile.h>
#include <TSystem.h>

#include "Conventions/Constants.h"
#include "Conventions/PhysHypothesisId.h"
#include "Conventions/Opt.h"
#include "Utilities/UtlNumStat.h"
#include "Utilities/Rnd.h"

#ifdef __VALOR_FMWK_GSL_ENABLED__
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_eigen.h>
#endif

using std::cout;
using std::cerr;
using std::endl;
using std::setw;
using std::setfill;
using std::fixed;
using std::scientific;
using std::setprecision;
using std::ofstream;
using std::ostringstream;

using namespace valor;
using namespace valor::conv;

//____________________________________________________________________________
double valor::util::DeltaChisquare(
  ConfLevel_t cl, int nfitparams) 
{
  switch(cl) {
      case (kCLUndefined) : 
      {
	return -99999;  
        break;
      }
      case (kCL68) :
      {
        if      (nfitparams == 1)  { return  1.00;  }
        else if (nfitparams == 2)  { return  2.30;  }
        else if (nfitparams == 3)  { return  3.53;  }
        else                       { return -99999; }
        break;
      }
      case (kCL90) :
      {
        if      (nfitparams == 1)  { return  2.71;  }
        else if (nfitparams == 2)  { return  4.61;  }
        else if (nfitparams == 3)  { return  6.25;  }
        else                       { return -99999; }
        break;
      }
      case (kCL95) :
      {
        if      (nfitparams == 1)  { return  3.84;  }
        else if (nfitparams == 2)  { return  5.99;  }
        else if (nfitparams == 3)  { return  7.82;  }
        else                       { return -99999; }
        break;
      }
      case (kCL99) :
      {
        if      (nfitparams == 1)  { return  6.63;  }
        else if (nfitparams == 2)  { return  9.21;  }
        else if (nfitparams == 3)  { return 11.34;  }
        else                       { return -99999; }
        break;
      }
      default:
        return -99999;     
    }
  return -99999;
} 
//____________________________________________________________________________
double valor::util::ChisqGauss(
  double ndata, double nmc)
{
  double chisq = 0.;
  if(nmc>0) {
    chisq = TMath::Power(ndata-nmc,2.)/nmc;
  }
  return chisq;
}
//____________________________________________________________________________
double valor::util::ChisqPoisson(double ndata, double nmc)
{
  if(nmc <= 0.)
    nmc = kSetNegEventsTo;
  if(ndata <= 0.)
    return 2.*(nmc-ndata);
  return 2. * (nmc-ndata) + 2. * ndata * TMath::Log(ndata/nmc);

  // if(nmc > 0. && ndata > 0.)
  //   return 2. * (nmc-ndata) + 2. * ndata * TMath::Log(ndata/nmc);
  // else 
  //   return 2. * (nmc-ndata);
}
//____________________________________________________________________________
TMatrixD valor::util::CholeskyDecomposition(const TMatrixD& cov_matrix)
{
// Perform a Cholesky decomposition of the input covariance matrix and
// return the lower triangular matrix
//
   int ncols = cov_matrix.GetNcols();
   int nrows = cov_matrix.GetNrows();

   assert(ncols==nrows);

   int n = nrows;

   TMatrixD L(n, n);

   for (int i = 0; i < n; ++i) {

     // calculate the diagonal term first
     L(i,i) = cov_matrix(i,i);
     for (int k = 0; k < i; ++k) {
        double tmp = L(k,i);
        L(i,i) -= tmp*tmp;
     }//k


     if(L(i,i) <= 0) {
       if( fabs(L(i,i))<conv::kASmallNum ){
	 L(i,i)=conv::kASmallNum;
	 cerr << "  , changed to "<<L(i,i)<<endl;
       }
       else{
	 cerr << "Decomposed covariance matrix not positive-definite" << endl;
	 cerr << "L(" << i << "," << i << ") = " << L(i,i) << endl;
	 exit(1);
       }
     }
     // if(L(i,i) <= 0) {
     //   cerr << "util/CholeskyDecomposition >> "<< "Value: "<<i<<","<<i<<" = "<<L(i,i);
     //   L(i,i)=conv::kASmallNum;
     //   cerr << "  , changed to "<<L(i,i)<<endl;
     //   //~ cerr << "Decomposed covariance matrix not positive-definite" << endl;
     //   //~ exit(1);
     // }



     L(i,i) = TMath::Sqrt(L(i,i));
     // then the off-diagonal terms
     for (int j = i+1; j < n; ++j) {
        L(i,j) = cov_matrix(i,j);
        for (int k = 0; k < i; ++k) {
           L(i,j) -= L(k,i)*L(k,j);
        }
        L(i,j) /= L(i,i);
     }//j
  }//i

  // create the transpose of L
  TMatrixD LT(TMatrixD::kTransposed,L);

#ifdef __show_cholesky_decomposition_mesg_debug__
  cout << "Cholesky matrix L^{T}" << endl;
  for(int ip=0; ip<n; ip++) {
    for(int jp=0; jp<n; jp++) {
      cout << setw(15) << setfill(' ') << setprecision(6) << LT[ip][jp] << " | ";
    }//jp
    cout << endl;
  }//ip
#endif

  return LT;
}
//____________________________________________________________________________
TVectorD valor::util::CholeskyGenerateCorrelatedParams (
    const TMatrixD& cholesky_triangular, TVectorD& mean_params) 
{
// Generate a vector of correlated params

  int ncols = cholesky_triangular.GetNcols();
  int nrows = cholesky_triangular.GetNrows();
  int npars = mean_params.GetNrows();

  if(ncols != nrows) {
    cerr << "util::CholeskyGenerateCorrelatedParams >> Number of columns " << ncols << " & rows " << nrows << " disagree. Exiting..." << endl;
    exit(1);
  }
  if(npars != nrows) {
    cerr << "util::CholeskyGenerateCorrelatedParams >> Number of parameters " << npars << " & rows " << nrows << " disagree. Exiting..." << endl;
    exit(1);
  }

  int n = nrows;

  // create a vector of unit Gaussian variables
  // and multiply by Lt to introduce the appropriate correlations
  TVectorD g(n);
  for (int k = 0; k < n; ++k) {
    g(k) = Rnd::Instance()->RndOther().Gaus();
  }
  g *= cholesky_triangular;

#ifdef __show_cholesky_gen_corr_param_variations_mesg_debug__
  ofstream foutp1("cholesky_uncorrelated_params.txt",fstream::app);
  for (int i = 0; i < n; ++i) {
    foutp1 << g[i] << "\t";
  }
  foutp1 << endl;
  foutp1.close();
#endif

  // add the mean value offsets and store the results
  TVectorD correlated_params(n);
  for (int i = 0; i < n; ++i) {
     double v = mean_params[i];
     v += g(i);
     correlated_params[i] = v;
  }

#ifdef __show_cholesky_gen_corr_param_variations_mesg_debug__
  ofstream foutp2("cholesky_correlated_params.txt",fstream::app);
  for (int i = 0; i < n; ++i) {
    foutp2 << correlated_params[i] << "\t";
  }
  foutp2 << endl;
  foutp2.close();
#endif

  return correlated_params;
}
//____________________________________________________________________________
TVectorD valor::util::CholeskyGenerateCorrelatedParams (
 const TMatrixD& cholesky_triangular, TVectorD& mean_params, TVectorD& g_uncorrelated) 
{
// Generate a vector of correlated params

  int ncols = cholesky_triangular.GetNcols();
  int nrows = cholesky_triangular.GetNrows();
  int npars = mean_params.GetNrows();
  int nunco = g_uncorrelated.GetNrows();

  if(ncols != nrows) {
    cerr << "util::CholeskyGenerateCorrelatedParams >> Number of columns " << ncols << " & rows " << nrows << " disagree. Exiting..." << endl;
    exit(1);
  }
  if(npars != nrows) {
    cerr << "util::CholeskyGenerateCorrelatedParams >> Number of parameters " << npars << " & rows " << nrows << " disagree. Exiting..." << endl;
    exit(1);
  }
  if(nunco != nrows) {
    cerr << "util::CholeskyGenerateCorrelatedParams >> Number of uncorrelated parameters " << nunco << " & rows " << nrows << " disagree. Exiting..." << endl;
    exit(1);
  }

  int n = nrows;

  // create a vector of unit Gaussian variables
  // and multiply by Lt to introduce the appropriate correlations
  g_uncorrelated *= cholesky_triangular;

#ifdef __show_cholesky_gen_corr_param_variations_mesg_debug__
  ofstream foutp1("cholesky_uncorrelated_params.txt",fstream::app);
  for (int i = 0; i < n; ++i) {
    foutp1 << g_uncorrelated[i] << "\t";
  }
  foutp1 << endl;
  foutp1.close();
#endif

  // add the mean value offsets and store the results
  TVectorD correlated_params(n);
  for (int i = 0; i < n; ++i) {
     double v = mean_params[i];
     v += g_uncorrelated(i);
     correlated_params[i] = v;
  }

#ifdef __show_cholesky_gen_corr_param_variations_mesg_debug__
  ofstream foutp2("cholesky_correlated_params.txt",fstream::app);
  for (int i = 0; i < n; ++i) {
    foutp2 << correlated_params[i] << "\t";
  }
  foutp2 << endl;
  foutp2.close();
#endif

  return correlated_params;
}
//____________________________________________________________________________
TVectorD valor::util::CholeskyGenerateCorrelatedParamVariations (
    const TMatrixD& cholesky_triangular) 
{
// Generate a vector of correlated params

  int ncols = cholesky_triangular.GetNcols();
  int nrows = cholesky_triangular.GetNrows();

  assert(ncols==nrows);

  int n = nrows;

  // create a vector of unit Gaussian variables
  // and multiply by Lt to introduce the appropriate correlations
  TVectorD g(n);
  for (int k = 0; k < n; ++k) {
    g(k) = Rnd::Instance()->RndOther().Gaus();
  }
  g *= cholesky_triangular;

#ifdef __show_cholesky_gen_corr_param_variations_mesg_debug__
  cout << "Util >> Tweak vector size = " << g.GetNrows() << endl;
#endif

  return g;
}
//____________________________________________________________________________
TVectorD valor::util::CholeskyCalculateCorrelatedParamVariations (
    const TMatrixD& cholesky_triangular, TVectorD & g_uncorrelated) 
{
// Generate a vector of correlated params

  int ncols = cholesky_triangular.GetNcols();
  int nrows = cholesky_triangular.GetNrows();
  int npars = g_uncorrelated.GetNrows();

  assert(ncols==nrows);
  assert(npars==nrows);

  // create a vector of unit Gaussian variables
  // and multiply by Lt to introduce the appropriate correlations
  TVectorD g(g_uncorrelated);
  g *= cholesky_triangular;

#ifdef __show_cholesky_gen_corr_param_variations_mesg_debug__
  cout << "Util >> Tweak vector size = " << g.GetNrows() << endl;
#endif

  return g;
}
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
     exit(1);
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
     exit(1);
  }*/

  // return if a physical value was generated
  if(mix >=0. && mix <= 1.) {
     return mix;
  }

  // or try again
  return util::RandomizeMixing23();
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
    exit(1);
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
    exit(1);
  }
  if(Pih < 0. || Pih > 1.){
    cerr << endl;
    cerr << "FitMarginalization::RandomizeMH" << endl;
    cerr << "The input probability for IH is not possible" << endl;
    cerr << "P(IH) = " << Pih << endl;
    cerr << endl;
    exit(1);
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
    exit(1);
  }

  double sinsqth = 0.5 * ( 1 - sqrt(1 - sinsq2th) );

  if(sinsqth<0. || sinsqth>1.){
    cerr << endl;
    cerr << "valor::util::FromSinsq2thToSinsqth" << endl;
    cerr << "Input parameter value of sinsqth is not allowed" << endl;
    cerr << "sinsqth = " << sinsqth << endl;
    cerr << endl;
    exit(1);
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
    exit(1);
  }

  double sinsq2th = 1 - (1 - 2*sinsqth)*(1 - 2*sinsqth);

  if(sinsq2th<0. || sinsq2th>1.){
    cerr << endl;
    cerr << "valor::util::FromSinsqthToSinsq2th" << endl;
    cerr << "Input parameter value of sinsq2th is not allowed" << endl;
    cerr << "sinsq2th = " << sinsq2th << endl;
    cerr << endl;
    exit(1);
  }
  
  return sinsq2th;
}
//





//____________________________________________________________________________
int valor::util::Sign(double tweaking_dial)
{
  if(fabs(tweaking_dial) < kASmallNum) return 0;
  if (tweaking_dial < 0) return -1;
  return +1;
}
//____________________________________________________________________________
int valor::util::DiagonaliseGSL(
#ifdef __VALOR_FMWK_GSL_ENABLED__
int ndim, double * matrix_in, double * eigenvectors_out, double * eigenvalues_out
#else
int, double *, double *, double *
#endif
)
{
#ifdef __VALOR_FMWK_GSL_ENABLED__
  // Initialise GSL memory structures
  gsl_matrix_complex * matr = gsl_matrix_complex_alloc(ndim,ndim);
  gsl_matrix_complex * eigenvectors = gsl_matrix_complex_alloc(ndim,ndim);
  gsl_vector * eigenvalues  = gsl_vector_alloc(ndim);
  gsl_eigen_hermv_workspace * workspace = gsl_eigen_hermv_alloc(ndim);
  
  // Put our matrix in the GSL matrix container
  memcpy( (double*)gsl_matrix_complex_ptr(matr,0,0), matrix_in, ndim*ndim*2*sizeof(double) );
  
  // Diagonalise
  int status=gsl_eigen_hermv(matr, eigenvalues, eigenvectors, workspace);
  
  // Copy the results to the output matrices
  memcpy( eigenvectors_out, (const double*)gsl_matrix_complex_const_ptr(eigenvectors,0,0) , ndim*ndim*2*sizeof(double));
  memcpy( eigenvalues_out , (const double*)gsl_vector_const_ptr        (eigenvalues ,0)   , ndim*sizeof(double));
  
  // Free GSL memory structures
  gsl_matrix_complex_free(matr);
  gsl_matrix_complex_free(eigenvectors);
  gsl_vector_free(eigenvalues);
  gsl_eigen_hermv_free(workspace);
  
  return (status != GSL_SUCCESS);
#else
  std::cerr<<"GSL Disabled"<<std::endl;
  exit(-1);
#endif
}
//____________________________________________________________________________

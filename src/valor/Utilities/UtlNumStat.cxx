//____________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

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
#include <TDecompSVD.h>

#include "valor/Conventions/Constants.h"
#include "valor/Conventions/PhysHypothesisId.h"
#include "valor/Conventions/Opt.h"
#include "valor/Utilities/UtlNumStat.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Messenger/Messenger.h"

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
  double chisq;
  if(nmc < conv::kSetNegEventsTo) {
    if (ndata>conv::kAVerySmallNum) {
      LOG("UtlNumStat",pWARN)<<"Zero MC events in bin with data events - chisquare should be infinite.";
    }
    nmc = conv::kSetNegEventsTo;
  }
  if(ndata <= conv::kAVerySmallNum) {
    chisq = 2.*(nmc-ndata);
  }
  else {
    chisq = 2. * (nmc-ndata) + 2. * ndata * TMath::Log(ndata/nmc);
  }
#ifdef VALOR_FMWK_LOW_LEVEL_MESG_ENABLED
  if (chisq<0) {
    LOG("UtlNumStat",pWARN)<<"Negative chisquare ("<<chisq<<"), from "<<ndata<<" events in data and "<<nmc<<" events in MC.";
  }
#endif
  return TMath::Max(0.,chisq);
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
	 LOG("UtlNumStat", pWARN) << "Element: " << i << "," << i << " = " << L(i,i);
	 L(i,i)=conv::kASmallNum;
	 LOG("UtlNumStat", pWARN) << "Changed to "<<L(i,i);
       }
       else{
	 LOG("UtlNumStat", pERROR) << "Decomposed covariance matrix not positive-definite"
				   << "L(" << i << "," << i << ") = " << L(i,i);
	 util::Die(1);
       }
     }
     // if(L(i,i) <= 0) {
     //   cerr << "util/CholeskyDecomposition >> "<< "Value: "<<i<<","<<i<<" = "<<L(i,i);
     //   L(i,i)=conv::kASmallNum;
     //   cerr << "  , changed to "<<L(i,i)<<endl;
     //   //~ cerr << "Decomposed covariance matrix not positive-definite" << endl;
     //   //~ util::Die(1);
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
    util::Die(1);
  }
  if(npars != nrows) {
    cerr << "util::CholeskyGenerateCorrelatedParams >> Number of parameters " << npars << " & rows " << nrows << " disagree. Exiting..." << endl;
    util::Die(1);
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
    util::Die(1);
  }
  if(npars != nrows) {
    cerr << "util::CholeskyGenerateCorrelatedParams >> Number of parameters " << npars << " & rows " << nrows << " disagree. Exiting..." << endl;
    util::Die(1);
  }
  if(nunco != nrows) {
    cerr << "util::CholeskyGenerateCorrelatedParams >> Number of uncorrelated parameters " << nunco << " & rows " << nrows << " disagree. Exiting..." << endl;
    util::Die(1);
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
  util::Die(-1);
#endif
}
//____________________________________________________________________________
TMatrixD valor::util::CovarianceMatrixToCorrelationMatrix(const TMatrixD & cov_m)
{
  int n = cov_m.GetNrows();
  TMatrixD corr_m(n,n);
  for (int ii = 0 ; ii < n; ii++) {
    for (int jj = 0 ; jj < n; jj++) {
      corr_m[ii][jj] = cov_m[ii][jj] / sqrt(cov_m[ii][ii] * cov_m[jj][jj]);
    }
  }
  return corr_m;
}
//____________________________________________________________________________
TMatrixD valor::util::MatrixPseudoInverted(const TMatrixD & m)
{
  TDecompSVD svd(m);
  TMatrixD output(m);
  //~ bool status =
  svd.Invert(output);
  return output;
}
//____________________________________________________________________________
TVectorD valor::util::ConcatenateTVectors (
  const vector< const TVectorD * > & tvecs )
{
  unsigned int vsize = tvecs.size();
  vector<int> edge_vec(vsize+1);
  edge_vec[0] = 0;
  for (unsigned int i = 0 ; i < vsize; i++) {
    edge_vec[i+1] = edge_vec[i] + tvecs.at(i)->GetNoElements();
  }
  
  TVectorD output(edge_vec[vsize]);
  
  for (unsigned int i = 0 ; i < vsize; i++) {
    output.SetSub(edge_vec[i],*tvecs.at(i));
  }
  
  return output;
}
//____________________________________________________________________________
TMatrixD valor::util::ConcatenateTMatrices (
  const std::vector<const TMatrixD *>  & tmats )
{
  unsigned int vsize = tmats.size();
  vector<int> edge_vec_x(vsize+1);
  edge_vec_x[0] = 0;
  vector<int> edge_vec_y(vsize+1);
  edge_vec_y[0] = 0;
  for (unsigned int i = 0 ; i < vsize; i++) {
    edge_vec_x[i+1] = edge_vec_x[i] + tmats.at(i)->GetNrows();
    edge_vec_y[i+1] = edge_vec_y[i] + tmats.at(i)->GetNcols();
  }
  
  TMatrixD output(edge_vec_x[vsize],edge_vec_y[vsize]);
  
  for (unsigned int i = 0 ; i < vsize; i++) {
    output.SetSub(edge_vec_x[i],edge_vec_y[i],*tmats.at(i));
  }
  
  return output;
}
//____________________________________________________________________________
double valor::util::SingleAngleToDoubleAngle(double sinsqtheta)
{
  double sinsq2theta = 1 - (1-2*sinsqtheta)*(1-2*sinsqtheta);
  return sinsq2theta;
}
//____________________________________________________________________________
double valor::util::DoubleAngleToSingleAngle(double sinsq2theta)
{
  double sinsqtheta = 0.5 * (1 - sqrt(1 - sinsq2theta));
  return sinsqtheta;
}
//____________________________________________________________________________
TAxis valor::util::EstimateOptimalBinning(int npoints, double * points)
{
  // Uses Freedman-Diaconis rule to guess a (fixed size) binning scheme as a starting point.
  
  //Get interquartile range
  double desired_quantiles[5] = {0,0.25,0.5,0.75,1.};
  double quantiles_out[5];
  TMath::Quantiles(npoints,5,points,quantiles_out,desired_quantiles,false);
  double iqr = quantiles_out[3]-quantiles_out[1];
  
  // Freedman-Diaconis
  double bin_size = 2 * iqr * pow(npoints,-.33333333333333);
  double xmax = quantiles_out[4];
  double xmin = quantiles_out[0];
  int nbins = TMath::Nint((xmax-xmin)/bin_size);
  return TAxis(nbins,xmin,xmax);
}
//____________________________________________________________________________
int valor::util::EstimateOptimalNumberOfBinsForGaussian(int npoints, double nsigma)
{
  // Uses Freedman-Diaconis rule
  
  static const double iqr = 27./20.; //assume sigma = 1 - we divide it out later
  
  // Freedman-Diaconis
  double bin_size = 2. * iqr * pow(npoints,-.33333333333333);
  int nbins = TMath::Nint(2*nsigma/bin_size);
  return nbins;
}
//____________________________________________________________________________
double valor::util::FractionOfBinContained(double min1, double max1, double min2, double max2)
{
  double range1 = max1-min1;
  double overlap;
  if     (min1 >= min2 && max1 <= max2) {overlap = max1 - min1;} // Entirely subsumed first bin
  else if(min2 >= min1 && max2 <= max1) {overlap = max2 - min2;} // Entirely subsumed second bin
  else if(max1 >  max2 && min1 >= min2) {overlap = max2 - min1;} // Second bin extends low
  else if(min1 <  min2 && max1 <= max2) {overlap = max1 - min2;} // Second bin extends high
  else {
    overlap = 0; // No crossover
  }
  return TMath::Max(overlap/range1,0.);
}
//____________________________________________________________________________

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
#include <cmath>
#include <algorithm>

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

#include "valor/Conventions/Build.h"
#include "valor/Conventions/Constants.h"
#include "valor/Conventions/PhysHypothesisId.h"
#include "valor/Conventions/Opt.h"
#include "valor/Utilities/UtlNumStat.h"
#include "valor/Utilities/UtlPrint.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Messenger/Messenger.h"

#ifdef __VALOR_FMWK_GSL_ENABLED__
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_errno.h>

#include "Math/Interpolator.h"
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
double valor::util::ChisqPoisson(double ndata_db, double nmc_db)
{
  double chisq;
  // Use long doubles internally - since DUNEND needs great precision for big numbers
  long double ndata = ndata_db;
  long double nmc   = nmc_db;
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
    chisq = 2. * ndata * (nmc/ndata+std::log(ndata/nmc)-1.l);
  }
#ifdef VALOR_FMWK_LOW_LEVEL_MESG_ENABLED
  if (chisq<0) {
    LOG("UtlNumStat",pWARN)<<"Negative chisquare ("<<chisq<<"), from "<<ndata<<" events in data and "<<nmc<<" events in MC.";
  }
#endif
  return TMath::Max(0.,chisq);
}
//____________________________________________________________________________
TMatrixD valor::util::CholeskyDecomposition(const TMatrixDSym& cov_matrix)
{
  return util::CholeskyDecomposition(TMatrixD(cov_matrix));
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
    const TMatrixD& cholesky_triangular, TVectorD& meanparams) 
{
// Generate a vector of correlated params

  int ncols = cholesky_triangular.GetNcols();
  int nrows = cholesky_triangular.GetNrows();
  int npars = meanparams.GetNrows();

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
     double v = meanparams[i];
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
 const TMatrixD& cholesky_triangular, TVectorD& meanparams, TVectorD& g_uncorrelated) 
{
// Generate a vector of correlated params

  int ncols = cholesky_triangular.GetNcols();
  int nrows = cholesky_triangular.GetNrows();
  int npars = meanparams.GetNrows();
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
     double v = meanparams[i];
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
TVectorD valor::util::CovarianceMatrixToErrorVector(const TMatrixD & cov_m)
{
  int n = cov_m.GetNrows();
  TVectorD vec(n);
  for (int ii = 0 ; ii < n; ii++) {
    vec[ii] = std::sqrt(cov_m[ii][ii]);
  }
  return vec;
}
//____________________________________________________________________________
TMatrixD valor::util::CorrelationAndErrorToCovarianceMatrix(const TMatrixD & corr, const TVectorD & one_sigma)
{
  int n = corr.GetNrows();
  if (n!=one_sigma.GetNrows()) {
    LOG("UtlNumStat",pERROR)<<"Matrix and vector dimensions do not match.";
    util::Die(-1);
    return TMatrixD();
  }
  TMatrixD cov_m(n,n);
  for (int ii = 0 ; ii < n; ii++) {
    for (int jj = 0 ; jj < n; jj++) {
      cov_m[ii][jj] = corr[ii][jj] * one_sigma[ii] * one_sigma[jj];
    }
  }
  return cov_m;
}
//____________________________________________________________________________
TMatrixDSym valor::util::MatrixToSymMatrix(const TMatrixD & m)
{
  int n = m.GetNrows();
  TMatrixDSym out (n);
  if (n!=m.GetNcols()) {
    LOG("UtlNumStat",pERROR)<<"Matrix is not square.";
    util::Die(-1);
    return out;
  }
  bool bad = false;
  for (int ii = 0 ; ii < n; ii++) {
    for (int jj = 0 ; jj <= ii; jj++) {
      double v1 = m[ii][jj];
      double v2 = m[jj][ii];
      out[ii][jj] = v1;
      out[jj][ii] = v2;
      if (std::fabs(v1) > conv::kAVerySmallNum && (std::fabs(v1/v2 - v2/v1) > conv::kASmallNum)) {
        bad = true;
      }
    }
  }
  if (bad) {
    LOG("UtlNumStat",pERROR)<<"Matrix is not symmetric.";
    util::Die(-1);
  }
  return out;
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
bool valor::util::SplineIsFlat(const TSpline * spl)
{
  bool flat = true;
  double x,y;
  for (int i = 0 ; i < spl->GetNp() ; i++) {
    spl->GetKnot(i,x,y);
    if (std::fabs(y-1) > conv::kASmallNum) {flat = false;}
  }
  return flat;
}
//____________________________________________________________________________
#ifdef __VALOR_FMWK_GSL_ENABLED__
bool valor::util::SplineIsFlat(const ROOT::Math::Interpolator * spl)
{
  bool flat = true;
  // Bit of a hack. No way to check limits/knots, so just evaluate at random values over wide range
  static const double x[6] = {-389931,-3E-2,0.,1.,7.,123210.};
  for (int i = 0 ; i < 6; i++) {
    double y = spl->Eval(x[i]);
    if (std::fabs(y-1) > conv::kASmallNum) {flat = false;}
  }
  return flat;
}
#else
bool valor::util::SplineIsFlat(const ROOT::Math::Interpolator * /*spl*/)
{
  return true;
}
#endif
//____________________________________________________________________________
#ifdef __VALOR_FMWK_GSL_ENABLED__
ROOT::Math::Interpolator * valor::util::ConvertTSplineToGSL(const TSpline & spl, Option_t * type)
{
  LOG("UtlNumStat",pINFO)<<"Converting spline to GSL: "<<spl.GetName();
  int n_knots = spl.GetNp();
  // Get spline type. Assume cubic if not specified
  TString type_ts(type);
  ROOT::Math::Interpolation::Type spl_type = ROOT::Math::Interpolation::kCSPLINE;
  if (type_ts.Contains("akima")) {
    spl_type = ROOT::Math::Interpolation::kAKIMA;
    assert(n_knots>=5);
  }
  vector<double> x(n_knots);
  vector<double> y(n_knots);
  for (int i = 0 ; i < n_knots; i++) {
    spl.GetKnot(i,x[i],y[i]);
  }
  ROOT::Math::Interpolator * out_interp = new ROOT::Math::Interpolator(n_knots,spl_type);
  out_interp->SetData(n_knots,x.data(),y.data());
  return out_interp;
}
//____________________________________________________________________________
ROOT::Math::Interpolator * valor::util::ConvertTSplineToGSL(const TSpline & spl,
  double xmin, double xmax, Option_t * type)
{
  LOG("UtlNumStat",pINFO)<<"Converting spline to GSL: "<<spl.GetName()<<" with padding knots.";
  int n_knots = spl.GetNp();
  // Get spline type. Assume cubic if not specified
  TString type_ts(type);
  ROOT::Math::Interpolation::Type spl_type = ROOT::Math::Interpolation::kCSPLINE;
  if (type_ts.Contains("akima")) {
    spl_type = ROOT::Math::Interpolation::kAKIMA;
    assert(n_knots>=3);
  }
  vector<double> x(n_knots+2);
  vector<double> y(n_knots+2);
  for (int i = 0 ; i < n_knots; i++) {
    spl.GetKnot(i,x[i+1],y[i+1]);
  }
  // Padding knots
  x.front() = xmin;
  y.front() = y[1];
  x.back()  = xmax;
  y.back()  = y[n_knots];
  
  ROOT::Math::Interpolator * out_interp = new ROOT::Math::Interpolator(n_knots,spl_type);
  out_interp->SetData(n_knots+2,x.data(),y.data());
  return out_interp;
}
#else
ROOT::Math::Interpolator * valor::util::ConvertTSplineToGSL(const TSpline & /*spl*/, Option_t * /*type*/)
{
  assert(!"ConvertTSplineToGSL not available without GSL!");
  return NULL;
}
ROOT::Math::Interpolator * valor::util::ConvertTSplineToGSL(const TSpline & /*spl*/, double, double, Option_t * /*type*/)
{
  assert(!"ConvertTSplineToGSL not available without GSL!");
  return NULL;
}
#endif
//____________________________________________________________________________
TMatrixDSym valor::util::HessianMatrix(MinuitFunction_t fitfunc, const double * point, int nparams, int max_iterations)
{
  // Uses iterative method from here: https://arxiv.org/abs/hep-ph/0008191
  // Based heavily off the FORTRAN here: http://www.pa.msu.edu/~pumplin/iterate/
  TMatrixDSym hessian_matrix(nparams);
  TMatrixDSym error_matrix  (nparams);
  double * eval = new double[nparams];
  int dummy=0, flag = 0; // Used in calling the function
  
  const double aimsag = 1;
  const int ncyc = 20; // max number of refinements to step size (per iteration)
  
  vector<double> x0best(nparams); // Location of the minimum
  double best_chisq;
  
  TMatrixD q   (nparams,nparams);
  TMatrixD v   (nparams,nparams);
  TMatrixD vold(nparams,nparams); // Last iteration's v
  TMatrixD u   (nparams,nparams); // Should converge to v
  vector<double> t (nparams); // Should converge to 1/sqrt(eigenvalues)
  vector<double> xi(nparams); // Should converge z, which sum squared to dchisq
  vector<double> y (nparams);
  
  vector<double> t1   (nparams);
  vector<double> dmove(nparams);
  
  vector<double> s (nparams); //
  vector<double> g11(nparams); // Estimate of first derivative
  vector<double> g22(nparams); // Estimate of second derivative
  vector<double> vh( (nparams*(nparams+1))/2 + 1);
  
  const double * x0 = point; // Alias for fortran
  
  // Initial values
  for (int i = 0 ; i < nparams; i++) {
    u[i][i] = 0.01;
    q[i][i] = 1;
    v[i][i] = 1;
    t[i] = 1.;
    x0best[i] = point[i];
    xi    [i] = point[i];
    eval  [i] = point[i];
  }
  
  // Get initial point chisq
  (*fitfunc)(dummy,0,best_chisq,eval,flag);
  double init_chisq = best_chisq;
  
  bool terminate = false; // second way out of this loop
  
  for (int iteration = 0 ; iteration < max_iterations && !terminate ; iteration++) {
    // Calculate t
    for (int i = 0 ; i < nparams; i++) {
      double sum = 0;
      for (int j = 0 ; j < nparams ; j++) {
        double elem = u(j,i);
        sum += elem*elem;
      }
      t[i] = sqrt(sum);
    }
    // Then v
    for (int i = 0 ; i < nparams; i++) {
      for (int j = 0 ; j < nparams ; j++) {
        v(i,j) = u(i,j) / t[j];
      }
    }
    // Check v orthogonal
    double errmax = 0;
    for (int i = 0 ; i < nparams; i++) {
      for (int j = 0 ; j < nparams ; j++) {
        double sum1 = 0;
        double sum2 = 0;
        for (int k = 0 ; k < nparams ; k++) {
          sum1 += v(k,i) * v(k,j);
          sum2 += v(i,k) * v(j,k);
        }
        double expected = (i==j);
        double err1 = std::fabs(sum1-expected);
        double err2 = std::fabs(sum2-expected);
        if (err1 > errmax) {errmax = err1;}
        if (err2 > errmax) {errmax = err2;}
      }
    }
    if (std::fabs(errmax) > 1E-5) {
      LOG("UtlNumStat",pERROR)<<"VTMatrix not orthogonal";
    }
    
    // Done with that bit - time to calculate the derivative
    // We won't do this on the first try
    if (iteration!=0) {
      
      double dstep = std::sqrt(aimsag);
      
      // Get diagonal second deriatives
      double fs1,fs2;
      for (int i = 0 ; i < nparams ; i++) {
        for (int j = 0 ; j < nparams ; j++) {
          eval[j] = x0[j] + u(j,i) * dstep;
        }
        (*fitfunc)(dummy,0,fs1,eval,flag);
        for (int j = 0 ; j < nparams ; j++) {
          eval[j] = x0[j] - u(j,i) * dstep;
        }
        (*fitfunc)(dummy,0,fs2,eval,flag);
        double sag = 0.5 * (fs1+fs2 - 2.*best_chisq);
        g22[i] = 2. * sag / (dstep * dstep);
        g11[i] = (fs1-fs2)/(2.*dstep);
        
        double sum = 0.;
        for (int j = 0 ; j < nparams ; j++) {
          double elem = u(j,i);
          sum += elem*elem;
        }
        //~ double step = dstep * sqrt(sum);
        
        int ndex = ((i+1)*(i+2))/2;
        vh[ndex] = g22[i];
      }
      
      double cormax = -9E99;
      double cormin =  9E99;
      
      double fs3,fs4;
      // Get off-diagonal second derivatives.
      for (int i = 1 ; i < nparams; i++) {
        for (int j = 0 ; j <= i-1; j++) {
// ******************************************************************************
// evaluate the fcn at 4 new points: (a,b), (-a,-b), (a, -b), (-a, b).
// Minuit's Hesse uses just one new point (a,b), and combines with the already
// known values at (0,0), (a,0), (0,b) to get an approximation that is accurate
// to first order.  If take instead two new points, e.g., (a,b) and (-a,-b),
// can combine with the known diagonal points to get a formula that is accurate
// to second order.  Using 4 points is still formally only accurate to second
// order, but the error contributions from the derivatives F13 and F31 are only
// half is big, and the error contribution from the derivative F22 is completely
// cancelled; so it is worth doing this even though it requires 4 function calls
// for each of the npar*(npar-1)/2 off-diagonal elements of the matrix.
// ******************************************************************************
//
// Steps for off-diagonal elements are taken to be smaller than
// the steps for diagonal elements by factor 1/sqrt(2), so that when
// convergence is achieved, they will correspond to raising the function
// by aimsag, just like the diagonal computations.

          double cmult = dstep/TMath::Sqrt2();
          // Evaluate at 4 corners of square around minimum
          for (int k = 0 ; k < nparams; k++) {
            eval[k]= x0[k] + cmult * (u(k,i) + u(k,j));
          }
          (*fitfunc)(dummy,0,fs1,eval,flag);
          for (int k = 0 ; k < nparams; k++) {
            eval[k]= x0[k] - cmult * (u(k,i) + u(k,j));
          }
          (*fitfunc)(dummy,0,fs2,eval,flag);
          for (int k = 0 ; k < nparams; k++) {
            eval[k]= x0[k] + cmult * (u(k,i) + u(k,j));
          }
          (*fitfunc)(dummy,0,fs3,eval,flag);
          for (int k = 0 ; k < nparams; k++) {
            eval[k]= x0[k] - cmult * (u(k,i) - u(k,j));
          }
          (*fitfunc)(dummy,0,fs4,eval,flag);
          
          double slow = (fs1+fs2+fs3+fs4)/(4.*cmult*cmult);
          double cmax = std::max(fs1-best_chisq,fs2-best_chisq);
          cmax = std::max(cmax,fs3-best_chisq);
          cmax = std::max(cmax,fs4-best_chisq);
          double cmin = std::min(fs1-best_chisq,fs2-best_chisq);
          cmin = std::min(cmin,fs3-best_chisq);
          cmin = std::min(cmin,fs4-best_chisq);
          
          int ndex =  ((i+1)*i)/2 + j;
          vh[ndex] = slow;
          
          cormax = std::max(cormax,cmax);
          cormin = std::min(cormin,cmin);
        }
      }
      
      // get qmat: hessian matrix in y
      for (int i = 0 ; i < nparams ; i++) {
        for (int j = 0 ; j <= i ; j++) {
          int ndex =  ((i+1)*i)/2 + j;
          q(i,j) = vh[ndex];
          q(j,i) = vh[ndex];
        }
      }
      
      // See how close q(i,j) is to the desired 2*(identity matrix)
      double diamin = q(0,0);
      double diamax = q(0,0);
      for (int i = 1 ; i < nparams ; i++) {
        diamin = std::min(q(i,i),diamin);
        diamax = std::max(q(i,i),diamax);
      }
      
      // Skip offdiag min/max checks here
      
      TMatrixD P(nparams,nparams);
      // get P = the hessian matrix in x
      for (int i = 0 ; i < nparams ; i++) {
        for (int j = 0 ; j <= i ; j++) {
          double sum = 0.;
          for (int m = 0 ; m < nparams ; m++) {
            for (int n = 0 ; n < i ; n++) {
              sum += q(m,n) * v(i,m) * v(j,n) / (t[m] * t[n]);
            }
          }
          P(i,j) = sum;
          P(j,i) = sum;
        }
      }
      
      TVectorD eigenvalues(nparams);
      TMatrixD eigenvectors = P.EigenVectors(eigenvalues);
      double min_eig = eigenvalues[0];
      double max_eig = eigenvalues[0];
      for (int i = 1 ; i < nparams ; i++) {
        min_eig = std::min(eigenvalues[i],min_eig);
        max_eig = std::max(eigenvalues[i],max_eig);
      }
      // if notion of hessian is not positive definite, add a constant to all
      // of the negative ones to fix it.
      if (min_eig <= 0.) {
        int nneg = 0;
        for (int i = 0 ; i < nparams ; i++) {
          if (eigenvalues[i] <= 0) {
            nneg++;
          }
          eigenvalues[i] -= 2*min_eig;
        }
      }
      // compute new estimate of location of the minimum...
      for (int i = 0 ; i < nparams ; i++) {
        double sum = 0.;
        for (int j = 0 ; j < nparams ; j++) {
          sum += v(i,j) * g11[j]/t[j];
        }
        s[i] = sum;
      }
      for (int i = 0 ; i < nparams ; i++) {
        double sum = 0.;
        for (int j = 0 ; j < nparams ; j++) {
          sum += P(j,i)*s[j];
        }
        t1[i] = -sum/eigenvalues[i];
      }
      for (int i = 0 ; i < nparams ; i++) {
        double sum = 0.;
        for (int j = 0 ; j < nparams ; j++) {
          sum += P(i,j) * t1[j];
        }
        dmove[i] = sum;
      }
      // find the displacement in new coordinates
      for (int i = 0 ; i < nparams ; i++) {
        double sum = 0.;
        for (int j = 0 ; j < nparams ; j++) {
          sum+= v(j,i)*dmove[j];
        }
        y[i] = sum/t[i];
      }
      
      
      // Improve minimum by steps
      int nsteps = 1;
      double stmax = 1.;
      // it may not be safe to move far from the region where the derivatives
      // have been estimated, so see how far that is
      double rmax = 0;
      for (int i = 0 ; i < nparams ; i++) {
        rmax = std::max(rmax,abs(y[i])/dstep);
      }
      static const double rmaxc = 1.; // threshold for testing by steps
      if (rmax > rmaxc) {
        nsteps = 50;
        stmax = 5.;
      }
      
      double fbest = best_chisq;
      for (int istep = 0 ; istep < nsteps; istep++) {
        double step = (stmax*(istep+1))/nsteps;
        for (int i = 0 ; i < nparams ; i++) {
          eval[i] = x0best[i] + step * dmove[i];
        }
        (*fitfunc)(dummy,0,fs1,eval,flag);
       // once we have stepped as far as the calculated position of minimum,
       // continue stepping only so long as each new step gives a new best value...
       
        if (step > 1. && fs1 > fbest) {break;}

        if (fs1 < fbest) {
          fbest = fs1;
          //~ stbest = step;
          for (int i = 0 ; i < nparams ; i++) {
            x0best[i] = eval[i];
            //~ x[i] = eval[i];
          }
          best_chisq = fbest;
        }
      } // step
      
      
      // get the new version of the transform
      // The fortran here has to reverse the eigenvectors, but ours are already in that order
      for (int i = 0 ; i < nparams ; i++) {
        t[i] = sqrt(2. / eigenvalues[i]);
      }
      
      // Save the old matrix
      vold = v;
      // And get the new one. The fortran here has to reverse the eigenvectors, but ours are already in that order
      v = eigenvectors;
        
      
      // To make the answer unique, multiply eigenvectors by -1 as
      // necessary to make the largest component of each vector positive
      for (int i = 0 ; i < nparams ; i++) {
        double tmp = v(0,i);
        for (int j = 0 ; j < nparams ; j++) {
          if (std::fabs(v(j,i)) > std::fabs(tmp)) {tmp = v(j,i);}
        }
        if (tmp < 0) {
          for (int j = 0 ; j < nparams ; j++) {
            v(j,i) *= -1;
          }
        }
      }
      for (int i = 0 ; i < nparams ; i++) {
        for (int j = 0 ; j < nparams ; j++) {
          u(i,j) = v(i,j) * t[j];
        }
      }
    }// not first iteration
    
    // adjust step sizes for each direction to make the increases in Chisqr 
    // to be approximately equal to the desired value
    for (int ipar = 0 ; ipar < nparams ; ipar++) {
      // For first few iterations, use faster, less accurate method
      if (iteration < 5) {
        double tmpst1 = 2.,tmpst2 = 2.;
        double fs1,fs2;
        for (int isize = 0 ; isize < ncyc ; isize++) {
          for (int j = 0 ; j < nparams ; j++) {
            eval[j] = x0[j] + u(j,ipar)*sqrt(aimsag);
          }
          (*fitfunc)(dummy,0,fs1,eval,flag);
          for (int j = 0 ; j < nparams ; j++) {
            eval[j] = x0[j] - u(j,ipar)*sqrt(aimsag);
          }
          (*fitfunc)(dummy,0,fs2,eval,flag);
          
          double rat1 = (std::max(fs1,fs2)-best_chisq)/aimsag;
          double rat2 = (std::min(fs1,fs2)-best_chisq)/aimsag;
          
          // ideally, rat1 and rat2 would both be 1.0; scale the step size to approximate that...
          // (in bizarre case that rat2 is negative, instead increase step size.)
          double tmp = sqrt(2./(rat1+rat2));
          if (rat2 < 0) {
            if (isize > 1) {
              if (tmpst1 > 1.) {tmp = std::min(tmp,tmpst1);}
              if (tmpst2 > 1.) {tmp = std::min(tmp,tmpst2);}
            }
          }
          else {
            tmp = sqrt(2./(rat1+rat2));
          }
          // force the scale change to lie between 0.5 and 2.0, so very large
          // changes are avoided...
          if (tmp > 1.5) {
            tmp = 2.-(0.75/tmp);
          }
          else if (tmp < 0.7) {
            tmp = 0.5 + (2./7.*tmp);
          }
          
          // terminate iterations if last few multipliers were close to 1...
          if(isize > 1) {
            double tmpmax = std::max(std::fabs(tmp   -1.),std::fabs(tmpst1-1.));
            if (std::max(tmpmax,std::fabs(tmpst2-1.)) < 0.1) {
              //~ LOG("UtlNumStat",pINFO) << "Last few multipliers close to one. Terminating iterations.";
              //~ terminate = true;
            }
          }
          
          // store the previous multipliers...
          if(isize > 0) {
            tmpst2 = tmpst1;
            tmpst1 = tmp;
          }
          for (int j = 0 ; j < nparams ; j++) {
             u(j,ipar) = tmp*u(j,ipar);
          }
        } // isize
      } // iterations < 4
      else {
        // Alternative procedure for higher iterations
        int igood = 0;
        double tbest = 0, dbest = 0;
        double xmult = 1;
        double tmvec[2] = {0.,0.};
        double tmult = sqrt(aimsag);
        for (int idir = 0; idir < 2; idir++) {
          tmult = sqrt(aimsag);
          if (idir == 1) {
            tmult *= -1;
          }
          // loop over iterations to find the displacement that 
          // gives the desired increase of aimsag in chisqr...
          // (This scheme avoids using derivatives -- it is 
          // intended to be robust in the face of discontinuities in 
          // FCN which can be present due to numerical integration noise.)
          int ntry = 50;
          double dbesth = -9E99;
          double dbestl =  9E99;
          double tbesth = 1.;
          double tbestl = 1.;
          bool converged = false;
          double fs1 = 0;
          for (int itry = 0 ; itry < ntry; itry++) {
            for (int j = 0 ; j < nparams ; j++) {
              eval[j] = x0[j] + u(j,ipar) * tmult;
            }
            (*fitfunc)(dummy,0,fs1,eval,flag);
            double del = fs1 - best_chisq;
            
            const double accu = 1E-3;
            // go if convergence good enough...
            // *** sets desired accuracy ***
            if (std::fabs(del-aimsag) > accu*aimsag) {
              igood = igood+idir+1; // Need a +1 here to match the fortran indices
              converged = true;
              tmvec[idir] = tmult;
              break;
            }
            if (itry == 0) {
              tbest = tmult;
              dbest = del;
            }
            //keep track of best above and best below...
            if (del >= aimsag) {
              if (std::fabs(del-aimsag) < std::fabs(dbesth-aimsag)) {
               tbesth = tmult;
               dbesth = del;
              }
            }
            if (del <= aimsag) {
              if (std::fabs(del-aimsag) < std::fabs(dbestl-aimsag)) {
                tbestl = tmult;
                dbestl = del;
              }
            }
            if (del <= 0.) {
              // del should always be positive, because amin should be the local minimum;
              // bail out if it isn't.
              LOG("UtlNumStat",pERROR) << "Function evaluation "<<fs1<<" below presumed minimum at "<<best_chisq;
              break;
            }
            // first time, calculate the multiplier by assuming chisqr is a quadratic 
            // function of the displacement, as it is expected to be near a local minimum...
            if (itry == 0 ) {
              xmult = sqrt(aimsag/del);
              // force the multiplier to be not too far from 1; but 
              // if it is already close to 1, leave it nearly as is...
              double ascale = 0.3;
              xmult = 1.+ ascale * TMath::ATan((xmult-1.)/ascale);
            }
            else {
              // keep track of best found so far...
              if (std::fabs(del-aimsag) < std::fabs(dbest-aimsag)) {
                tbest = tmult;
                dbest = del;
              }
              // reduce step size if have stepped to the other side of the desired value...
              if ( (xmult > 1. && del >= aimsag) ||
                   (xmult < 1. && del <= aimsag) ) {
                tmult = tbest;
                del = dbest;
                xmult = sqrt(xmult);//makes step factor closer to 1.0, i.e., smaller step size
              }
              if ( (xmult > 1. && del >= aimsag) ||
                   (xmult < 1. && del <= aimsag) ) {
                xmult = 1./xmult;
              }
            } // 850
            tmult *= xmult;
          } // itry
          
          // here it didn't converge.  This appears to happen only if the function is not 
          // monotonic.  In that case, go back to the best result found...
          if (!converged) {
            tmult = tbest;
            for (int j = 0 ; j < nparams ; j++) {
                eval[j] = x0[j] + u(j,ipar) * tmult;
            }
            (*fitfunc)(dummy,0,fs1,eval,flag);
            LOG("UtlNumStat",pERROR) << "Poor convergence - chisquare "<<fs1<<" with presumed minimum at "<<best_chisq;
          }
          tmvec[idir] = tmult;
        }//idir
        // get an "average" tmult, based on assuming a quadratic + cubic form...
        // NOTE, tmvec[1] is negative.
        if (igood == 3) {
          double tmp = 1./(tmvec[0]*tmvec[0]) + 1./(tmvec[1]*tmvec[1]) + 1./(tmvec[0]*tmvec[1]);
          tmult = 1./sqrt(tmp);
        }
        else if (igood == 1) {
          tmult = std::abs(tmvec[0]);
          LOG("UtlNumStat",pINFO) << "Only idir=0 converged, use tmult = "<<tmult;
        }
        else if (igood == 2) {
          tmult = std::abs(tmvec[1]);
          LOG("UtlNumStat",pINFO) << "Only idir=1 converged, use tmult = "<<tmult;
        }
        else {
          tmult = 1E-3;
          LOG("UtlNumStat",pINFO) << "Neither direction converged, use tmult = "<<tmult;
        }
        
        
        // update u...
        for (int j = 0 ; j < nparams ; j++) {
          u(j,ipar) = u(j,ipar)*tmult/sqrt(aimsag);
        }
      } // End of alterantive procedure
      
    } // end of loop over eigenvector directions ipar
    
    // See how much have the eigenvector directions have changed from the previous iteration...
    if (iteration >= 0) { // Copied from fortran - but I think this should probably be > rather than >=
      for (int i = 0 ; i < nparams ; i++) {
        double sum = 0.;
        for (int j = 0 ; j < nparams ; j++) {
          sum += v(j,i) * vold(j,i);
        }
      }
    }
    
  } // iterations
  
  // replace the location of the minimum by any accidentally discovered improvement...
  // VALOR - we're not doing this bit yet
  
  // calculate the final notion of the hessian and the error matrix...
  for (int i = 0 ; i < nparams ; i++) {
    for (int j = 0 ; j <= i ; j++) {
      double sum1 = 0.;
      double sum2 = 0.;
      for (int k = 0 ; k < nparams ; k++) {
         double tmp = t[k]*t[k] / 2.;
         sum1 = sum1 + v(i,k)*v(j,k)/tmp;
         sum2 = sum2 + v(i,k)*v(j,k)*tmp;
      } //k
    
      hessian_matrix(i,j) = sum1;
      hessian_matrix(j,i) = sum1;
      
      error_matrix(i,j) = sum2;
      error_matrix(j,i) = sum2;
    } // j
  } //i
    
      
  delete[] eval;
  return hessian_matrix;
}

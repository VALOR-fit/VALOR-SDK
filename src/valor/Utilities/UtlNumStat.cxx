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
#include <utility>

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
#include <TDecompChol.h>

#include "valor/Conventions/Build.h"
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
using std::vector;
using std::pair;

using namespace valor;
using namespace valor::conv;

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
      LOG("UtlNumStat",pWARN)<<"Zero MC events in bin with data events - chisquare should be infinite."
                             <<" MC: " << nmc_db
                             <<" Data:" << ndata_db;
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
   // Perform a Cholesky decomposition of the input covariance matrix
   int ncols = cov_matrix.GetNcols();
   int nrows = cov_matrix.GetNrows();

   assert(ncols==nrows);

   TDecompChol chol(cov_matrix);
   Bool_t succeeded = chol.Decompose();
   if(succeeded != kTRUE)
   {
     cerr << "util::CholeskyDecomposition >> decomposition failed" << endl;
     util::Die(1);
   }
    TMatrixD U = chol.GetU();
    TMatrixD UT(TMatrixD::kTransposed, U);
/*    cout << "ROOT Cholesky matrix U^{T}" << endl;
    for(int ip=0; ip<n; ip++)
    {
      for(int jp=0; jp<n; jp++)
      {
        cout << setw(15) << setfill(' ') << setprecision(6) << UT[ip][jp] << " | ";
      }
      cout << endl;
    }*/

   return UT;
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
TMatrixDSym valor::util::LowerTriangularMatrixToSymMatrix(const TMatrixD & m)
{
  int n = m.GetNrows();
  TMatrixDSym out (n);
  if (n!=m.GetNcols()) {
    LOG("UtlNumStat",pERROR)<<"Matrix is not square.";
    util::Die(-1);
    return out;
  }
  for (int ii = 0 ; ii < n; ii++) {
    for (int jj = 0 ; jj <= ii; jj++) {
      double v1 = m[ii][jj];
      out[ii][jj] = v1;
      out[jj][ii] = v1;
    }
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
  y.front() = spl.Eval(xmin);
  x.back()  = xmax;
  y.back()  = spl.Eval(xmax);
  
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
bool pair_compare_by_first(const pair<double,int> & lhs, const pair<double,int> & rhs)
  {return lhs.first < rhs.first;}
//____________________________________________________________________________
void valor::util::ReorderMatrixAndVector (TVectorD & sort_by, TMatrixD & matr, bool descending,
 bool reorder_rows, bool reorder_cols)
{
  if ((sort_by.GetNoElements() != matr.GetNrows()) &&
      (sort_by.GetNoElements() != matr.GetNcols()) ) {
    LOG("UtlNumStat",pWARN) << "Non-matching numbers of elements. Doing nothing.";
    return;
  }
  TMatrixD matcopy(matr);
  
  vector<pair<double,int> > vec_indices;
  for (int i = 0 ; i < sort_by.GetNoElements(); i++) {
    std::pair<double,int> p(sort_by[i],i);
    vec_indices.push_back(p);
  }
  if (descending) {
    std::sort(vec_indices.rbegin(),vec_indices.rend(),pair_compare_by_first);
  }
  else {
    std::sort(vec_indices.begin(),vec_indices.end(),pair_compare_by_first);
  }
  for (unsigned int i = 0 ; i < vec_indices.size() ; i++) {
    int orig_i = vec_indices[i].second;
    sort_by[i] = vec_indices[i].first;
    for (unsigned int j = 0 ; j < vec_indices.size() ; j++) {
      int orig_j = vec_indices[j].second;
      int ind_i = (reorder_rows ? orig_i : i);
      int ind_j = (reorder_cols ? orig_j : j);
      matr(i,j) = matcopy(ind_i,ind_j);
    }
  }
  return;
}
//____________________________________________________________________________
double valor::util::Chisq_Penalty_TH2D(TH2D* hobs, TH2D* hexp)
{
  int nbins_x = hobs->GetNbinsX();
  int nbins_y = hobs->GetNbinsY();
  
  double chisq = 0;
  
  for(int i=1;i<= nbins_x;i++)
  {
    for(int j=1;j<= nbins_y;j++)
    {
      double obs = hobs->GetBinContent(i,j);
      double exp = hexp->GetBinContent(i,j);
      
      chisq += valor::util::ChisqPoisson(obs,exp);
    }
  }
  
  return chisq;
}

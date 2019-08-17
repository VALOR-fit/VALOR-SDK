//____________________________________________________________________________
/*!

\class     valor::utils

\brief     Numerical and statistics utilities

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   Oct 20, 2010

\copyright (c) 2010-2016, The VALOR group.

*/
//____________________________________________________________________________

#ifndef _valor_utils_numstat_h_
#define _valor_utils_numstat_h_

#include <string>
#include <vector>
#include <complex>

#include <TXMLEngine.h>
#include <TFile.h>
#include <TTree.h>
#include <TVectorD.h>
#include <TMatrixD.h>
#include <TMatrixDSym.h>
#include <TH1D.h>
#include <TSpline.h>

class TH2D;


#include "valor/Conventions/Build.h"
#include "valor/Conventions/ConfLevel.h"
#include "valor/Conventions/PhysParamId.h"

using std::string;
using std::vector;

// mathmore forward decl
namespace ROOT{ namespace Math { class Interpolator;}}

namespace valor {
namespace util {

  //
  // Statistics
  //

  double DeltaChisquare     (conv::ConfLevel_t cl, int nfitparams);
  double ChisqGauss         (double ndata, double nmc);
  double ChisqPoisson       (double ndata, double nmc);
  double Chisq_Penalty_TH2D (TH2D* hobs, TH2D* hexp);
  
  double ChisqMeanLikelihood(std::vector<double> chisqs); // Performs a mean of the non-logged values, returns chisq

  TAxis EstimateOptimalBinning(int npoints, double * points);
  int EstimateOptimalNumberOfBinsForGaussian(int npoints, double nsigma);

  //
  // Cholesky decomposition
  //

  TMatrixD CholeskyDecomposition                     (const TMatrixD& cov);                 ///< Cholesky decomposition. Return lower triangular matrix.
  TMatrixD CholeskyDecomposition                     (const TMatrixDSym& cov);                 ///< calls above with explicit conversion
  TVectorD CholeskyGenerateCorrelatedParams          (const TMatrixD& Lch, TVectorD& mean); ///< Generate a vector of correlated params
  TVectorD CholeskyGenerateCorrelatedParams          (const TMatrixD& Lch, TVectorD& mean, TVectorD& g_uncorrelated); ///< Generate a vector of correlated params
  TVectorD CholeskyGenerateCorrelatedParamVariations (const TMatrixD& Lch);                 ///< Generate a vector of correlated param variations
  TVectorD CholeskyCalculateCorrelatedParamVariations(const TMatrixD& Lch, TVectorD& g);    ///<

  // Simple maths
  double SingleAngleToDoubleAngle(double sinsqtheta);
  double DoubleAngleToSingleAngle(double sinsq2theta);

  // Convert the sigma of a Gaussian from double angle to single angle and vice-versa
  double SingleAngleSigmaToDoubleAngleSigma(double sinsqtheta_mean , double sinsqtheta_sigma );
  double DoubleAngleSigmaToSingleAngleSigma(double sinsq2theta_mean, double sinsq2theta_sigma);

  // Convert the sigma of a Gaussian from phase to sin(phase) and vice-versa
  double PhaseSigmaToSinPhaseSigma(double phase_mean   , double phase_sigma   );
  double SinPhaseSigmaToPhaseSigma(double sinphase_mean, double sinphase_sigma);

  // Other Matrix Functions
  TMatrixD CovarianceMatrixToCorrelationMatrix(const TMatrixD & cov_m);
  TVectorD CovarianceMatrixToErrorVector(const TMatrixD & cov_m);
  TMatrixD CorrelationAndErrorToCovarianceMatrix(const TMatrixD & corr, const TVectorD & one_sigma);
  TMatrixD MatrixPseudoInverted(const TMatrixD & m);
  TMatrixDSym MatrixToSymMatrix(const TMatrixD & m);
  TMatrixDSym LowerTriangularMatrixToSymMatrix(const TMatrixD & m);

  TVectorD ConcatenateTVectors  (const std::vector<const TVectorD *>  & tvecs );
  TMatrixD ConcatenateTMatrices (const std::vector<const TMatrixD *>  & tmats );
  
  // Sorts both the vector and matr arguments in according to the vector's values in place
  void ReorderMatrixAndVector (TVectorD & sort_by, TMatrixD & matr, bool descending = false, bool reorder_rows = true, bool reorder_cols = true);

  //
  // Functions randomizing oscillation params (for use when params are marginalized)
  //
  
  double RandomizeParameter(conv::PhysParamId_t p);

  double RandomizeMixing23 (void); ///< pull from gaussian with mean & sigma from global fit (mixing param is either sinsq2theta or sinsqtheta depending on config)
  double RandomizeMixing12 (void); ///< pull from gaussian with mean & sigma from global fit ( -//- )
  double RandomizeMixing13 (void); ///< uniform from 0 up to ~ small_factor x CHOOZ limit    ( -//- )
  double RandomizeCPV      (void); ///< uniform from -pi to +pi (or -1 to +1)
  double RandomizeDm12sq   (void); ///< pull from gaussian with mean & sigma from global fit
  double RandomizeDm23sq   (void); ///< pull from gaussian with mean & sigma from global fit
  
  double RandomizeMixing23Bar (void); //
  double RandomizeMixing12Bar (void); //
  double RandomizeMixing13Bar (void); //
  double RandomizeCPVBar      (void); //  
  double RandomizeDm12sqBar   (void); //
  double RandomizeDm23sqBar   (void); //

  double RandomizeUser        (TH1D *hPrior); // Extract random from a prior 
  int    RandomizeMH          (const double probNH);
  
  // NEW BETABAR
  double RandomizeBetaBar   (void); //
  //

  // NEW OSC PRIOR 
  double FromSinsq2thToSinsqth(const double sinsq2th);
  double FromSinsqthToSinsq2th(const double sinsqth);
  //

  // Full matrix diagonalisation using GSL
  // matrix_in is a pointer to a block representing a n*n complex matrix, equivalent to a double[n][n][2]
  // eigenvectors_out is pointer to (assigned) memory in same format
  // eigenvalues_out is pointer to double[n]
  int DiagonaliseGSL(int ndim, double * matrix_in, double * eigenvectors_out, double * eigenvalues_out);
  
  //
  // Etc
  //

  int Sign(double x);
  
  double Logistic(double x);
  double Logit(double p);
  
  // Gives the fraction of bin 1 that lies inside bin 2
  double FractionOfBinContained(const double & min1, const double &  max1, const double &  min2, const double &  max2);
  
  // Check if a spline is flat
  bool SplineIsFlat(const TSpline * spl);
  bool SplineIsFlat(const ROOT::Math::Interpolator * spl);
  
  // Make a TSpline into a new GSL Spline
  ROOT::Math::Interpolator * ConvertTSplineToGSL(const TSpline & spl, Option_t * opt = "");
  ROOT::Math::Interpolator * ConvertTSplineToGSL(const TSpline & spl, double xmin, double xmax, Option_t * opt = "");
  
} // util
} // valor
  
#endif


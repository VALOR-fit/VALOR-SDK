//____________________________________________________________________________
/*!

\class     valor::utils

\brief     Fitter utilities

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   Oct 20, 2010

\copyright (c) 2010-2016, The VALOR group.

*/
//____________________________________________________________________________

#ifndef _valor_fitter_utilities_h_
#define _valor_fitter_utilities_h_

#include <vector>
#include <TMatrixD.h>
#include <TMatrixDSym.h>

#include "valor/FitCore/FitterABC.h"
#include "valor/FitCore/FitInputs.h"
#include "valor/FitCore/PhysParamList.h"

namespace valor {
namespace util {

// Method to return a VALOR fitter.
// Options:
//   - minuit
//   - minuit2
//   - SimAnneal
//   - LevMarq
//   - ConjugateFR
//   - ConjugatePR
//   - VectorBFGS
//   - SteepestDescent
//
FitterABC * GetFitter (FitInputs * inputs, Option_t * option);

  // Randomise the parameters in an FPL  using accept/reject with the chi-square probability
  double RandomiseParametersByAcceptance(FitInputs & fi, const TMatrixD * cholesky_for_throws = 0,
      int max_attempts = 100000, double scaling_factor = 1.);
  // WARNING - version with no cholesky matrix given is not properly validated
  // Also, scales very badly with num of parameters. Unusably slow with high dimensions
  // Should replace with eg MCMC
  
  // Tweak the parameters in an FPL  using MCMC with the chi-square probability
  bool AttemptMCMCStep(FitInputs & fitinputs, double & current_chisq, int & n_attempts_out,
    double step_size_factor, TMatrixD * cholesky_for_throws = NULL, int max_attempts = 1000);
  
  // Take a vector of parameter sets, and generate a covariance matrix
  TMatrixDSym CalculateCovariance(std::vector<valor::PhysParamList> param_sets,
      const CorrelatedSystematics & example_format);

} // util
} // valor

#endif


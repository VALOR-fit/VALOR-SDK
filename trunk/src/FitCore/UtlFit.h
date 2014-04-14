//____________________________________________________________________________
/*!

\class     t2kana::utils

\brief     Fitter utilities

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   Oct 20, 2010

\copyright (c) 2010-2014, The VALOR group.

*/
//____________________________________________________________________________

#ifndef _t2kana_fitter_utilities_h_
#define _t2kana_fitter_utilities_h_

#include "FitCore/FitterABC.h"
#include "FitCore/FitInputs.h"

namespace t2kana {
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

} // util
} // t2kana

#endif


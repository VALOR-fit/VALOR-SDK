//____________________________________________________________________________
/*!

\class     valor::conv::example01

\brief     Constants for example fit 01

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   Match 30, 2014

\copyright 2010-2014, The VALOR group.

*/
//____________________________________________________________________________

#ifndef _t2kana_const_example01_h_
#define _t2kana_const_example01_h_

namespace valor    {
namespace conv      {
namespace example01 {

static const int    kNEbins   = 100;
static const double kEmin     =  0.;
static const double kEmax     = 10.;

static const double kNorm      = 1000; // overall normalization factor
static const double kMPV       = 0.6;  // most probable value (GeV) for example signal Etrue spectrum (Landau distribution)
static const double kSigma     = 1.2;  // spread (GeV) of example signal Etrue spectrum (Landau distribution)
static const double kBkgLevel0 = 0.2;  // parameterizes a simple bkg, A 20% bkg at Etrue -> 0 which dies out linearly and is 0 at 10 GeV
static const double kBkgSlope  = 0.02; // parameterizes a simple bkg, A 20% bkg at Etrue -> 0 which dies out linearly and is 0 at 10 GeV

} // example01
} // conv
} // valor
  
#endif 


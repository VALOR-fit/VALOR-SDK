//____________________________________________________________________________
/*!

\class     valor::conv::

\brief     Beam configuration enumeration

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   April 15, 2014

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_expt_baseline_h_
#define _valor_expt_baseline_h_

#include "valor/Conventions/BeamConf.h"
#include "valor/Conventions/Detector.h"

namespace valor {
namespace conv {

//
// Baselines
//
static const double kJPARCTgt2SuperK =  295.000;  // km
static const double kJPARCTgt2ND280  =    0.280;  // km
static const double kLBNFTgt2DUNEFD  = 1300.000;  // km
static const double kLBNFTgt2DUNEND  =    0.547;  // km
static const double kBNBTgt2SBND     =    0.110;  // km
static const double kBNBTgt2uBooNE   =    0.470;  // km
static const double kBNBTgt2ICARUS   =    0.600;  // km
static const double kNuMITgt2NOvAFD  =  810.000;  // km
static const double kNuMITgt2NOvAND  =    1.000;  // km

} // conv

//
// Associated utilities
//
namespace util {

  double Baseline(conv::BeamConf_t b, conv::Detector_t d);

} // util

} // valor
  
#endif 


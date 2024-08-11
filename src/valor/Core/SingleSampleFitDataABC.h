//____________________________________________________________________________
/*!

\class     valor::SingleSampleFitDataABC

\brief     Abstract base class (pABC) for a single DUNE fit sample dataset

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   March 30, 2014

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_single_sample_fit_data_abc_h_
#define _valor_single_sample_fit_data_abc_h_

// fmwk
#include "valor/Core/FitDataABC.h"
#include "valor/Core/FitModelABC.h"
#include "valor/Conventions/KineSpace.h"
#include "valor/Conventions/BeamConf.h"
#include "valor/Conventions/Sample.h"
#include "valor/Conventions/Detector.h"

namespace valor {

class SingleSampleFitDataABC: public FitDataABC
{
public:
  virtual ~SingleSampleFitDataABC();

  void Configure(conv::Detector_t d, conv::BeamConf_t b, conv::Sample_t s, conv::KineSpace_t k); 

protected:
  SingleSampleFitDataABC();

  // Configuration
  conv::Detector_t  fDetector;
  conv::BeamConf_t  fBeam;
  conv::Sample_t    fSample;
  conv::KineSpace_t fKSp;
};

} // valor

#endif


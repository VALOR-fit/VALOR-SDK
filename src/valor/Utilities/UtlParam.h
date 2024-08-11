//____________________________________________________________________________
/*!

\file      UtlParam

\brief     Parameter related utilities

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           Steve Dennis <s.r.dennis \at liverpool.ac.uk>
           University of Liverpool

           Francis Bench <f.bench \at liverpool.ac.uk>
           University of Liverpool

\created   May 21, 2019

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_util_param_definition_h_
#define _valor_util_param_definition_h_

#include <string>
#include <utility>

//fmwk
#include "valor/Conventions/PhysParamId.h"
#include "valor/Conventions/Detector.h"
#include "valor/Conventions/BeamConf.h"
#include "valor/Conventions/FitParamApply.h"
#include "valor/Conventions/FitParamGroup.h"
#include "valor/Conventions/ReacMode.h"
#include "valor/Conventions/KineVar.h"
#include "valor/Core/CorrelatedSystematics.h"
#include "valor/Core/PhysHypothesis.h"

using std::string;

namespace valor {
namespace util {

  conv::FitParamGroup_t GetParameterGroup(conv::PhysParamId_t p);
  // Get the list of parameters associated with a set name (eg flux, int, ccqe, fsi, 3fosc)
  std::vector<conv::PhysParamId_t> GetParameterSet(std::string set_name, bool only_exact_match = false);
  // Same as above, but only get params that are compatible with an input apply mask
  std::vector<conv::PhysParamId_t> GetParameterSet(std::string set_name, conv::ApplyMask_t mask, bool only_exact_match = false);

  std::pair<double,double> GetKineRange(conv::PhysParamId_t p, conv::KineVar_t k);

  // Does the input parameter depend on any kine var?
  bool HasKinematicDependence(conv::PhysParamId_t p);

  // Get a vector of osc + syst params from the PhysHypothesis and ParameterPool
  std::vector<conv::PhysParamId_t> GetActiveParameters(const PhysHypothesis & ph);

} //util

} // valor

#endif


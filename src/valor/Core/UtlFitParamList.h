//____________________________________________________________________________
/*!

\file      UtlFitParamList

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

#ifndef _valor_dune_util_fit_param_list_definition_h_
#define _valor_dune_util_fit_param_list_definition_h_

//fmwk
#include "valor/Core/FitParamList.h"
#include "valor/Core/PhysHypothesis.h"
#include "valor/Core/PhysParamList.h"

namespace valor {
namespace util {
  // Constructs and returns a FitParamList from the ParameterPool
  FitParamList * GetFitParameterList (const PhysHypothesis& hypothesis , Option_t * param_opt = "");
  FitParamList * GetFitParameterList (std::string hypothesis_name      , Option_t * param_opt = "");

  // Aliases for the above, for python
  FitParamList * GetFitParameterListFromHypothesis    (const PhysHypothesis& hypothesis, Option_t * param_opt = "");
  FitParamList * GetFitParameterListFromHypothesisName(std::string hypothesis_name     , Option_t * param_opt = "");

  // Are the current values of all params in the fit param list (or phys param list) within their respective limits?
  bool AllWithinLimits(const FitParamList & fparm);
  bool AllWithinLimits(const PhysParamList & pparm);
} //util

} // valor

#endif


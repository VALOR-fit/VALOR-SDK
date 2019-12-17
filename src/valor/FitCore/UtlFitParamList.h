//____________________________________________________________________________
/*!

\file      UtlFitParamList

\brief     Parameter related utilities

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           Steve Dennis <s.r.dennis \at liverpool.ac.uk>
           University of Liverpool

           Francis Bench <f.bench \at liverpool.ac.uk>
           University of Liverpool

\created   May 21, 2019

\copyright (c) 2010-2019, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_dune_util_fit_param_list_definition_h_
#define _valor_dune_util_fit_param_list_definition_h_

//fmwk
#include "valor/FitCore/FitParamList.h"
#include "valor/FitCore/PhysHypothesis.h"

namespace valor {
namespace util {
  // Constructs and returns a FitParamList from the ParameterPool
  FitParamList * GetFitParameterList (const PhysHypothesis& hypothesis , Option_t * param_opt = "");
  FitParamList * GetFitParameterList (std::string hypothesis_name, Option_t * param_opt = "");
} //util

} // valor

#endif


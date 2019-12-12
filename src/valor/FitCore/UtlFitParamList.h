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

#ifndef _valor_dune_util_param_definition_h_
#define _valor_dune_util_param_definition_h_

#include <string>
#include <utility>

//fmwk
#include "valor/Utilities/UtlParam.h"
#include "valor/Conventions/PhysParamId.h"
#include "valor/Conventions/Detector.h"
#include "valor/Conventions/BeamConf.h"
#include "valor/Conventions/FitParamApply.h"
#include "valor/Conventions/FitParamGroup.h"
#include "valor/Conventions/ReacMode.h"
#include "valor/Conventions/KineVar.h"
#include "valor/FitCore/CorrelatedSystematics.h"
#include "valor/FitCore/FitParamList.h"
#include "valor/FitCore/PhysHypothesis.h"

namespace valor {
namespace util {
  // If you give a CorrelatedSystematics object, it'll ignore your list's standard systematics and use this set instead
  FitParamList * GetFitParameterList (Option_t * param_list_name = "nuosc3f", Option_t * param_opt = "", const CorrelatedSystematics * syst = 0);
} //util

} // valor

#endif


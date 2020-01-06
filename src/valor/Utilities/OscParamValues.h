//____________________________________________________________________________
/*!

\class     valor::conv::

\brief     Enumeration of default initial values for oscillation parameters

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           Lorena Escudero <escudero \at hep.phy.cam.ac.uk>
           University of Cambridge
          
\created   July, 2016

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_oscparams_h_
#define _valor_oscparams_h_

// fmwk
#include "valor/Conventions/PhysParamId.h"

namespace valor {

namespace util {

  // Given an osc param, p, and its value, val, which is given in a particular scheme: Convert the value from the
  // value's scheme to another scheme. If a scheme name is left blank, then the scheme defined in Opt::Instance is used.
  // e.g. Input a sinsq(2*theta) value and convert to a sinsq(theta) value.
  // NOTE: The conversion between Dmsq schemes is applied when calculating oscillation probabilities
  //       (see e.g. NuOsc3Flavour::SetOscParametersSinSqTAll), so this function does not convert Dmsq values.
  double ConvertOscParamToUsedScheme(conv::PhysParamId_t p, double val,
                                     std::string val_scheme_name = "", std::string target_scheme_name = "");

  double ConvertOscParamErrorToUsedScheme(conv::PhysParamId_t p, double central_value, double error,
                                     std::string val_scheme_name = "", std::string target_scheme_name = "");

} // util

} // valor
  
#endif 


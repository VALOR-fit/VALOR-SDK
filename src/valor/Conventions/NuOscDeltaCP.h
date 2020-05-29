//____________________________________________________________________________
/*!

\class     valor::conv

\brief     Choices of CP-violating parameter

\author    Lorena Escudero <loressa@ific.uv.es>
           IFIC Valencia
          
\created   Oct 20, 2010

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef cpv_parameter_h
#define cpv_parameter_h

#include <string>

using std::string;

namespace valor {

namespace conv {

typedef enum ENuOscDeltaCP
{
  kNuDeltaCPUndefined = -1,
  kNuDeltaCPPhase,  // use deltaCP 
  kNuDeltaCPSin     // use sin(deltaCP)
}
NuOscDeltaCP_t;
   
} // conv

namespace util {

  string AsString (conv::NuOscDeltaCP_t d);
  string CPVChoiceAsString (conv::NuOscDeltaCP_t d); //alias
  string CPVChoiceAsStringSimple (conv::NuOscDeltaCP_t d);

  conv::NuOscDeltaCP_t CPVChoiceFromString (string d);

} // util

} // valor
  
#endif 


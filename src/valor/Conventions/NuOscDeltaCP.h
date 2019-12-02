//____________________________________________________________________________
/*!

\class     valor::conv

\brief     Choices of CP-violating parameter

\author    Lorena Escudero <loressa@ific.uv.es>
           IFIC Valencia
          
\created   Oct 20, 2010

\copyright (c) 2010-2016, The VALOR group

*/
//____________________________________________________________________________

#ifndef cpv_parameter_h
#define cpv_parameter_h

#include <string>

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

  std::string AsString (conv::NuOscDeltaCP_t d);
  std::string CPVChoiceAsString (conv::NuOscDeltaCP_t d); //alias
  std::string CPVChoiceAsStringSimple (conv::NuOscDeltaCP_t d);

  conv::NuOscDeltaCP_t CPVChoiceFromString (std::string d);

} // util

} // valor
  
#endif 


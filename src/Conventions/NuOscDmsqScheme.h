//____________________________________________________________________________
/*!

\class     valor::conv::NuOscDmsqScheme_t

\brief     Choice of squared mass splitting parameter

\author    Lorena Escudero <loressa@ific.uv.es>
           IFIC Valencia
          
\created   Oct 20, 2010

\copyright (c) 2010-2014, The VALOR group
*/
//____________________________________________________________________________

#ifndef nuosc_dmsq_scheme_h
#define nuosc_dmsq_scheme_h

#include <string>

#include "Conventions/NuMassHierarchy.h"

using std::string;

namespace valor {

namespace conv {

typedef enum ENuOscDmsqScheme
{
  kNuDmsqUndefined = -1,
  kNuDmsq12andAbs23,  // use |Dm^2_{23}| and Dm^2_{12} as the 2 independent squared mass splittings                                                                            
  kNuDmsqFogliLisi,   // scheme used by Fogli and Lisi, dm^2 = Dm^2_{12} and Dm^2 = |m^2_{3} - (m^2_{1} + m^2_{1})/2|                                                            
  kNuDmsq12andAbs31,  // use |Dm^2_{31}| and Dm^2_{12} as the 2 independent squared mass splittings        
  kNuDmsqAuto         // Use kNuDmsq12andAbs23 if normal hierarchy, kNuDmsq12andAbs31 of inverted
}
NuOscDmsqScheme_t;
   
} // conv

namespace util {

  string AsString(conv::NuOscDmsqScheme_t dmsqs);
  string DmsqSchemeAsString(conv::NuOscDmsqScheme_t dmsqs); //alias of above

  conv::NuOscDmsqScheme_t DmsqSchemeFromString (string h);
  conv::NuOscDmsqScheme_t GetAutoDmsqSchemeFromHierarchy(conv::NuMassHierarchy_t hierarchy = conv::kNmhUndefined); // If given undefined, reads Opt class

} // util

} // valor
  
#endif 


//____________________________________________________________________________
/*!

\enum      valor::conv::NuMassHierarchy_t

\brief     Neutrino mass hierarchy enumeration

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   Oct 20, 2010

\copyright (c) 2010-2016, The VALOR group

*/
//____________________________________________________________________________

#ifndef nu_mass_hierarchy_h
#define nu_mass_hierarchy_h

#include <string>

namespace valor {

namespace conv {

typedef enum ENuMassHierarchy 
{   
   kNmhUndefined = -1, 
   kNmhNormal,
   kNmhInverted
} 
NuMassHierarchy_t;
   
} // conv

namespace util {

  std::string AsString (conv::NuMassHierarchy_t  m);
  std::string HierarchyAsString (conv::NuMassHierarchy_t  m); // alias of above for python

  conv::NuMassHierarchy_t HierarchyFromString (std::string h);

} // util

} // valor
  
#endif 


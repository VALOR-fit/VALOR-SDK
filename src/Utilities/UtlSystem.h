//____________________________________________________________________________
/*!

\class     valor::utils

\brief     System utilities

\author    Steve Dennis <stephen.dennis \t warwick.ac.uk
           University of Warwick & STFC Rutherford Appleton Laboratory
          
\created   Nov 16, 2015

\copyright (c) 2010-2015, The VALOR group

*/
//____________________________________________________________________________

#ifndef valor_system_utils_h
#define valor_system_utils_h

namespace valor {

namespace util {
  
  void Die(int status_code); // Call exit() unless Opt::DoNotDie is true.
  
} // util
} // valor

#endif

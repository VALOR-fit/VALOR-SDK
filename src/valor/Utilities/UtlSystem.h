//____________________________________________________________________________
/*!

\class     valor::utils

\brief     System utilities

\author    Steve Dennis <stephen.dennis \t warwick.ac.uk
           University of Warwick & STFC Rutherford Appleton Laboratory
          
\created   Nov 16, 2015

\copyright (c) 2010-2016, The VALOR group

*/
//____________________________________________________________________________

#ifndef valor_system_utils_h
#define valor_system_utils_h

#include <vector>
#include <string>

namespace valor {

namespace util {
  
  void Die(int status_code); // Call exit() unless Opt::DoNotDie is true.
  
  void ChangeInterpreterPrompt(std::string new_prompt = "VALOR: [%d] ");
  
  std::vector<std::string> GetLinkedLibraries(std::string executable_path, std::string pattern);
  
  std::string CompleteConfigPath(std::string config_file);
  
} // util
} // valor

#endif

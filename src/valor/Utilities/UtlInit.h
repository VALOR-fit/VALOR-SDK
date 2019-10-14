/*!

\brief     This namespace handles initialisation of the VALOR framework.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick
          
\created   March 16, 2017

\copyright (c) 2010-2017, The VALOR group

*/

#ifndef valor_util_utl_init
#define valor_util_utl_init

#include <string>

// valor
#include "valor/CmdLine/CmdLnArgList.h"

namespace valor
{
namespace util
{
namespace init
{
   /**
      Initialises the VALOR framwork.

      \param environment The name of the environment to instantiate.
      \param appArgs The path to the argument file for the application. This
         should be an absolute path, or a path relative to the config folder.
      \param seedArgs The path to the argument file for the seeds. This should
         be an absolute path, or a path relative to the config folder.
      \param argc The standard argument count from the command line.
      \param argv The standard argument array from the command line.
   */
   CmdLnArgList Init(const std::string& appArgs,
         const std::string& seedArgs, const int argc, const char* argv[]);

}
}
}

#endif


//____________________________________________________________________________
/*!

\brief     This namespace provides standard sets of unit tests for Dune.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick

\created   Mar 14, 2017

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved.

*/
//____________________________________________________________________________

#ifndef __CINT__

#ifndef valor_testing_standard_tests
#define valor_testing_standard_tests

// VALOR/SDK
#include "valor/CmdLine/CmdLnArgList.h"

namespace valor
{
namespace testing
{
   /**
      This method runs the standard set of batch tests for the overnight build.

      \param arg The command line arguments specified.
      \return True if the tests passed, false otherwise.
   */
   bool BatchTests(CmdLnArgList& arg);

}
}

#endif

#endif

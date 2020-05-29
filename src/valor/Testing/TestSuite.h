/*!

\class     valor::testing::TestSuite

\brief     This class imanages and runs unit tests.

\author    Andy Chappell <A.Chappell \t warwick.ac.uk
           University of Warwick
          
\created   Mar 02, 2017

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
#ifndef __CINT__

#ifndef valor_testing_test_suite
#define valor_testing_test_suite

#include <string>
#include <map>

// valor
#include "valor/Testing/Test.h"

namespace valor
{
namespace testing
{

class TestSuite
{
public:
   /**
      Constructor.
   */
   TestSuite(CmdLnArgList& arg): _arg{arg} { }

   /**
      Destructor.
   */
   ~TestSuite();

   /**
      Runs all the tests defined for this TestSuite.

      \param True if the tests ran successfully, false otherwise.
   */
   bool Run();

   /**
      This method adds a Test containing a set of unit tests to the suite.

      Note that the TestSuite will take ownership of the Test objects and will
      handle their deletion.

      \param name The name to uniquely identify the Test.
      \param test The Test to be added to suite.
   */
   void Add(std::string name, Test* test);

protected:
   CmdLnArgList& _arg;

private:
   std::map<std::string, Test*> _tests;
};

}
}

#endif

#endif


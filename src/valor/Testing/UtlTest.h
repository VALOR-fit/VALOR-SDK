//____________________________________________________________________________
/*!

\class     valor::testing::UtlTest

\brief     This class provides unit tests for Dune utilities.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick

\created   Mar 02, 2017

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved.

*/
//____________________________________________________________________________

#ifndef __CINT__

#ifndef valor_testing_utl_test
#define valor_testing_utl_test

// VALOR/SDK
#include "valor/Conventions/Build.h"
#include "valor/Testing/Test.h"
#if GENIE_ENABLED==1
#include "valor/GENIEUtilities/UtlEventRec.h"
#endif

namespace valor
{
namespace testing
{

class UtlTestCase : public TestCase
{
public:
   /**
      Constructor.

      \param test The associated Test object.
   */
   UtlTestCase(Test* test): TestCase(test){ }

   /**
      Check that true kinematics are retrieved correctly from a GENIE vertex.

      \return True if the test passed, false otherwise.
   */
   bool TestGetTrueKinematics();
};

class UtlTest : public Test
{
friend class UtlTestCase;
public:
   virtual ~UtlTest(){ }

   /**
      This method performs any pre-test initialisation.

      This method will be called before every test to ensure that each test is
      independent of other tests.
   */
   virtual void Setup();

   /**
      This method performs any post-test cleanup.

      This method will be called after every test to ensure that each test is
      independent of other tests.
   */
   virtual void Teardown();

   /**
      This method adds all of the unit tests to be run for this test set.
   */
   virtual void AddTests();

private:
   std::string _mcEventsFilename;
};

}
}

#endif

#endif

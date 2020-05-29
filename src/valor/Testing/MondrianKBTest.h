/*!

\class     valor::testing::MondrianKBTest

\brief     This class provides unit tests for MondrianKB.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick
          
\created   Mar 02, 2017

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
#ifndef __CINT__

#ifndef valor_testing_mondrian_kb_test
#define valor_testing_mondrian_kb_test

#include "valor/Testing/Test.h"

// valor
#include "valor/KineBinning/KBCubHcmbMondrian.h"

namespace valor
{
namespace testing
{

class MondrianKBTestCase : public TestCase
{
public:
   /**
      Constructor.

      \param test The associated Test object.
   */
   MondrianKBTestCase(Test* test): TestCase(test){ }

   /**
      Check that contraction of Mondrian binning works as expected.

      \return True if the test passed, false otherwise.
   */
   bool TestContract();
};

class MondrianKBTest : public Test
{
friend class MondrianKBTestCase;
public:
   virtual ~MondrianKBTest(){ }

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
   KBCubHcmbMondrian* _binning;
};

}
}

#endif

#endif


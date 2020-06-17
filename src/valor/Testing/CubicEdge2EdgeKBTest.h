//____________________________________________________________________________
/*!

\class     valor::testing::CubicEdge2EdgeKBTest

\brief     This class provides unit tests for CubicEdge2EdgeKB.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick

\created   Mar 02, 2017

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved.

*/
//____________________________________________________________________________

#ifndef __CINT__

#ifndef valor_testing_cubic_edge_2_edge_kb_test
#define valor_testing_cubic_edge_2_edge_kb_test

// VALOR/SDK
#include "valor/Testing/Test.h"
#include "valor/KineBinning/KBCubHcmbEdge2Edge.h"
#include "valor/Utilities/SingleSampleKineBinning.h"

namespace valor
{
namespace testing
{

class CubicEdge2EdgeKBTestCase : public TestCase
{
public:
   /**
      Constructor.

      \param test The associated Test object.
   */
   CubicEdge2EdgeKBTestCase(Test* test): TestCase(test){ }

   /**
      Checks that a known Edge2Edge binning of [Ereco,Yreco] contracted to
      [Ereco] and [Yreco] produces the expected results.

      \return True if the test passed, false otherwise.
   */
   bool TestContract();
};

class CubicEdge2EdgeKBTest : public Test
{
friend class CubicEdge2EdgeKBTestCase;
public:
   virtual ~CubicEdge2EdgeKBTest(){ }

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
   SingleSampleKineBinning* _binning;
};

}
}

#endif

#endif

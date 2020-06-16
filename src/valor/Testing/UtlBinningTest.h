//____________________________________________________________________________
/*!

\class     valor::testing::UtlBinningTest

\brief     This class provides unit tests for UtlBinning.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick

\created   Mar 02, 2017

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved.

*/
//____________________________________________________________________________

#ifndef __CINT__

#ifndef valor_testing_utl_binning_test
#define valor_testing_utl_binning_test

// VALOR/SDK
#include "valor/Testing/Test.h"
#include "valor/KineBinning/UtlBinning.h"

namespace valor
{
namespace testing
{

class UtlBinningTestCase : public TestCase
{
public:
   /**
      Constructor.

      \param test The associated Test object.
   */
   UtlBinningTestCase(Test* test): TestCase(test){ }

   /**
      Checks that a known optimised binning of [Ereco,Yreco,Etrue] produces an
      [Ereco,Yreco] ROOT compatible histogram.

      \return True if the test passed, false otherwise.
   */
   bool TestMake2DOptimisedHistogram();

   /**
      Checks that a known simple binning of [Ereco,Yreco, Etrue] produces an
      [Ereco,Yreco] ROOT compatible histogram.

      \return True if the tests pass, false otherwise.
   */
   bool TestMake2DSimpleHistogram();

   /**
      Checks that a known binning of [Ereco, Etrue] produces an [Ereco] ROOT
      compatible histogram.

      \return True if the tests pass, false otherwise.
   */
   bool TestMake1DHistogram();
};

class UtlBinningTest : public Test
{
friend class UtlBinningTestCase;
public:
   virtual ~UtlBinningTest(){ }

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
   VALORArray* _vOpt;
   VALORArray* _vSmp;
};

}
}

#endif

#endif

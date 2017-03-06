/*!

\class     valor::testing::PhysParamsTest

\brief     This class provides unit tests for PhysParams.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick
          
\created   Mar 01, 2017

\copyright (c) 2010-2017, The VALOR group

*/
#ifndef __CINT__

#ifndef valor_testing_phys_params_test
#define valor_testing_phys_params_test

#include "valor/Testing/Test.h"

// valor
#include "valor/FitCore/PhysParams.h"

namespace valor
{
namespace testing
{

class PhysParamsTestCase : public TestCase
{
public:
   /**
      Constructor.

      \param test The associated Test object.
   */
   PhysParamsTestCase(Test* test): TestCase(test){ }

   /**
      Check that best fit values are set correctly.

      \return True if the test passed, false otherwise.
   */
   bool TestBestfit();

   /**
      Check that intial values are set correctly.

      \return True if the test passed, false otherwise.
   */
   bool TestInitial();

   /**
      Check that minimum values are set correctly.

      \return True if the test passed, false otherwise.
   */
   bool TestMinimum();

   /**
      Check that maximum values are set correctly.

      \return True if the test passed, false otherwise.
   */
   bool TestMaximum();

   /**
      Check that step size values are set correctly.

      \return True if the test passed, false otherwise.
   */
   bool TestStepSize();

   /**
      Check that scaling values are set correctly.

      \return True if the test passed, false otherwise.
   */
   bool TestScaling();

   /**
      Check the number of parameters loaded is as expected.

      \return True if the test passed, false otherwise.
   */
   bool TestNParams();

   /**
      Check that a number of parameters expected to have been loaded exist and
      have the IDs expected.

      \return True if the test passed, false otherwise.
   */
   bool TestValidParams();

   /**
      Check that a parameter not expetced in the set is not not present.

      \return True if the test passed, false otherwise.
   */
   bool TestInvalidParams();
};

class PhysParamsTest : public Test
{
friend class PhysParamsTestCase;
public:
   virtual ~PhysParamsTest(){ }

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
   PhysParams* _params;
};

}
}

#endif

#endif


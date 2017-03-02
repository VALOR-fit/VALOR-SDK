/*!

\class     valor::testing::PhysParamsTest

\brief     This class provides unit tests for PhysParams.

\author    Andy Chappell <A.Chappell \t warwick.ac.uk
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
   PhysParamsTestCase(Test* test): TestCase(test){ }

   bool TestNParams();
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


/*!

\class     valor::testing::ConfigurationTest

\brief     This class provides unit tests for Configuration.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick
          
\created   Mar 14, 2017

\copyright (c) 2010-2017, The VALOR group

*/
#ifndef __CINT__

#ifndef valor_testing_configuration_test
#define valor_testing_configuration_test

#include "valor/Testing/Test.h"

// valor
#include "valor/Utilities/Configuration.h"

namespace valor
{
namespace testing
{

using util::Configuration;

class ConfigurationTestCase : public TestCase
{
public:
   /**
      Constructor.

      \param test The associated Test object.
   */
   ConfigurationTestCase(Test* test): TestCase(test){ }

   /**
      Check that instances are correctly created.

      \return True if the test passed, false otherwise.
   */
   bool TestInstance();

   /**
      Check that boolean values are correctly set.

      \return True if the test passed, false otherwise.
   */
   bool TestBool();

   /**
      Check that integer values are correctly set.

      \return True if the test passed, false otherwise.
   */
   bool TestInt();

   /**
      Check that double values are correctly set.

      \return True if the test passed, false otherwise.
   */
   bool TestDouble();

   /**
      Check that string values are correctly set.

      \return True if the test passed, false otherwise.
   */
   bool TestString();

   /**
      Check that histograms are correctly set.

      \return True if the test passed, false otherwise.
   */
   bool TestHistograms();
};

class ConfigurationTest : public Test
{
friend class ConfigurationTestCase;
public:
   virtual ~ConfigurationTest(){ }

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
   Configuration* _config;
};

}
}

#endif

#endif


/*!

\class     valor::testing::ConfigurationHandlerTest

\brief     This class provides unit tests for ConfigurationHandler.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick
          
\created   Mar 16, 2017

\copyright (c) 2010-2017, The VALOR group

*/
#ifndef __CINT__

#ifndef valor_testing_configuration_handler_test
#define valor_testing_configuration_handler_test

#include "valor/Testing/Test.h"

// valor
#include "valor/Utilities/ConfigurationHandler.h"

namespace valor
{
namespace testing
{

using util::ConfigurationHandler;

class ConfigurationHandlerTestCase : public TestCase
{
public:
   /**
      Constructor.

      \param test The associated Test object.
   */
   ConfigurationHandlerTestCase(Test* test): TestCase(test){ }

   /**
      Check that the top-level analysis configuration file is read correctly.

      \return True if the test passed, false otherwise.
   */
   bool TestReadAnalysis();

   /**
      Check that the samples configuration file is read correctly.

      \return True if the test passed, false otherwise.
   */
   bool TestReadSampleList();

   /**
      Check that the MC configuration file is read correctly.

      \return True if the test passed, false otherwise.
   */
   bool TestReadMC();

   /**
      Check that the binning configuration file is read correctly.

      \return True if the test passed, false otherwise.
   */
   bool TestReadBinning();

   /**
      Check that the optimised binning configuration file is read correctly.

      \return True if the test passed, false otherwise.
   */
   bool TestReadOptimisedBinning();

   /**
      Check that the simple binning configuration file is read correctly.

      \return True if the test passed, false otherwise.
   */
   bool TestReadSimpleBinning();

   /**
      Check that the parameter list configuration file is read correctly.

      \return True if the test passed, false otherwise.
   */
   bool TestReadParameterList();

   /**
      Check that the parameters configuration file is read correctly.

      \return True if the test passed, false otherwise.
   */
   bool TestReadParameters();

   /**
      Check that the splines configuration file is read correctly.

      \return True if the test passed, false otherwise.
   */
   bool TestReadSplines();

};

class ConfigurationHandlerTest : public Test
{
friend class ConfigurationHandlerTestCase;
public:
   virtual ~ConfigurationHandlerTest(){ }

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
   ConfigurationHandler* _handler;
};

}
}

#endif

#endif


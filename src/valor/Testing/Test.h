/*!

\class     valor::testing::Test

\brief     This class provides facilities for defining unit tests.

\author    Andy Chappell <A.Chappell \t warwick.ac.uk
           University of Warwick
          
\created   Mar 01, 2017

\copyright (c) 2010-2017, The VALOR group

*/
#ifndef __CINT__

#ifndef valor_testing_test
#define valor_testing_test

#include <functional>
#include <map>
#include <string>
#include <vector>

// valor
#include "valor/CmdLine/CmdLnArgList.h"

namespace valor
{
namespace testing
{

class Test;

class TestCase
{
public:
   TestCase(Test* test): _test{test} {}

protected:
   Test* _test;
};

/**
   Acts as a key for unit tests.
*/
class TestKey
{
public:
   /**
      Constructor.

      \param name The name of the test. This should be unique within the
         specified TestCase.
      \param context The TestCase against which unit tests are to be called.
   */
   TestKey(std::string name, TestCase* context):
      _name{name}, _context{context}
   {
   }

   TestKey(const TestKey& original);

   TestKey& operator=(const TestKey& original);

   ~TestKey(){ }

   /**
      Retrieves the name of the unit test.

      \return The name of the unit test.
   */
   const std::string& Name() const { return _name; }

   /**
      Retrieves the context of the unit test.

      \return The context of the unit test.
   */
   TestCase* Context(){ return _context; };

private:
   std::string _name;
   TestCase* _context;
};

typedef std::function<bool(void)> TestFunctionPtr;

class Test
{
public:
   /**
      Destructor.
   */
   virtual ~Test();

   /**
      Configure the test to allow reading from command line arguments.

      \param arg The command line arguments.
   */
   virtual void Configure(CmdLnArgList& arg){ _arg = &arg; }

   /**
      Runs the unit tests that have been defined.

      \param True if the tests ran successfully, false otherwise.
   */
   virtual bool Run();

   /**
      This method performs any pre-test initialisation.

      This method will be called before every test to ensure that each test is
      independent of other tests.
   */
   virtual void Setup() = 0;

   /**
      This method performs any post-test cleanup.

      This method will be called after every test to ensure that each test is
      independent of other tests.
   */
   virtual void Teardown() = 0;

   /**
      This method adds all of the unit tests to be run for this test set.
   */
   virtual void AddTests() = 0;

   /**
      This method returns the total number of tests.

      \return The number of tests.
   */
   virtual int NTests() const;

   /**
      This method returns the total number of failed tests.

      \return The number of failed tests.
   */
   virtual int NFailures() const;

   /**
      Tests the equality of double-precision floating point numbers within a given
      arithmetic tolerance.

      \param obs The observed value.
      \param b The expected value.
      \param tol The tolerance of the comparison. Default 1E-6.

      \return True if the numbers are equal within tolerance, false otherwise.
   */
   virtual bool Equal(double obs, double exp, double tol = 1E-6);

   /**
      Tests the equality of integer values.

      \param obs The observed value.
      \param exp The expected value.

      \return True if the numbers are equal.
   */
   virtual bool Equal(int obs, int exp);

protected:
   /**
      This method adds a unit test to the set of tests to be run.

      \param key The key uniquely identifying the test.
      \param test The method to be added to the set of tests.
   */
   virtual void Add(TestKey* key, TestFunctionPtr method);

protected:
   CmdLnArgList* _arg;

private:
   std::map<TestKey*, TestFunctionPtr> _tests;
   std::vector<TestKey*> _failures;
};

}
}

#endif

#endif


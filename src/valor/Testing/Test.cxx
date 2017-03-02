#include "valor/Testing/Test.h"

// valor
#include "valor/Messenger/Messenger.h"

namespace valor
{
namespace testing
{

Test::~Test()
{
   TestCase* context = nullptr;
   if(_tests.size() > 0)
   {  // If there are tests, get the context from the first test
      context = _tests.begin()->first->Context();
      // Clear the tests
      _tests.clear();
   }
   // Delete the TestKeys, noting this won't delete the TestCases within
   for(auto key : _failures)
   {
      delete key;
   }
   // Clear the failures
   _failures.clear();

   // One context is shared between all TestKeys associated with this Test, so
   // need to be careful deleting it when we're done
   delete context;
}

bool Test::Run()
{
   for(auto& kv : _tests)
   {
      TestFunctionPtr method = kv.second;
      Setup();
      bool passed = method();
      Teardown();
      if(not(passed))
      {
         _failures.push_back(kv.first);
      }
   }

   return (NFailures() == 0);
}

void Test::Add(TestKey* key, TestFunctionPtr method)
{
   _tests.insert(std::make_pair(key, method));
}

int Test::NTests() const
{
   return _tests.size();
}

int Test::NFailures() const
{
   return _failures.size();
}

TestKey::TestKey(const TestKey& original)
{
   _name = original._name;
   _context = original._context;
}

TestKey& TestKey::operator=(const TestKey& original)
{
   _name = original._name;
   _context = original._context;

   return (*this);
}

/**
   Tests the equality of double-precision floating point numbers within a given
   arithmetic tolerance.

   \param obs The observed value.
   \param b The expected value.
   \param tol The tolerance of the comparison. Default 1E-6.

   \return True if the numbers are equal within tolerance, false otherwise.
*/
bool Test::Equal(double obs, double exp, double tol)
{
   bool equal = (abs(obs - exp) < tol);
   if(!equal)
   {
      LOG("Test", pERROR) << "Test failed: Expected " << exp << ". Found " <<
         obs << ".";
   }
   return equal;
}

/**
   Tests the equality of integer values.

   \param obs The observed value.
   \param exp The expected value.

   \return True if the numbers are equal.
*/
bool Test::Equal(int obs, int exp)
{
   bool equal = obs == exp;
   if(!equal)
   {
      LOG("Test", pERROR) << "Test failed: Expected " << exp << ". Found " <<
         obs << ".";
   }
   return equal;
}

}
}


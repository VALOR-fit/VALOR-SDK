//____________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#include "valor/Testing/Test.h"

#include <cmath>

// VALOR/SDK
#include "valor/Messenger/Messenger.h"

namespace valor
{
namespace testing
{
//____________________________________________________________________________
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
//____________________________________________________________________________
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
//____________________________________________________________________________
void Test::Add(TestKey* key, TestFunctionPtr method)
{
   _tests.insert(std::make_pair(key, method));
}
//____________________________________________________________________________
int Test::NTests() const
{
   return _tests.size();
}
//____________________________________________________________________________
int Test::NFailures() const
{
   return _failures.size();
}
//____________________________________________________________________________
TestKey::TestKey(const TestKey& original)
{
   _name = original._name;
   _context = original._context;
}
//____________________________________________________________________________
TestKey& TestKey::operator=(const TestKey& original)
{
   _name = original._name;
   _context = original._context;

   return (*this);
}
//____________________________________________________________________________
bool Test::Equal(const double obs, const double exp, const double tol)
{
   bool equal = (std::abs(obs - exp) < tol);
   if(!equal)
   {
      LOG("Test", pERROR) << "Test failed: Expected " << exp << ". Found " <<
         obs << ".";
   }
   return equal;
}
//____________________________________________________________________________
bool Test::Equal(const int obs, const int exp)
{
   bool equal = obs == exp;
   if(!equal)
   {
      LOG("Test", pERROR) << "Test failed: Expected " << exp << ". Found " <<
         obs << ".";
   }
   return equal;
}
//____________________________________________________________________________
bool Test::Equal(const std::string& obs, const std::string& exp)
{
   bool equal = obs == exp;
   if(!equal)
   {
      LOG("Test", pERROR) << "Test failed: Expected \'" << exp << "\'. Found \'" <<
         obs << "\'.";
   }
   return equal;
}

bool Test::True(const bool obs)
{
   bool equal = obs == true;
   if(!equal)
   {
      LOG("Test", pERROR) << "Test failed: Expected true. Found false.";
   }
   return equal;
}
//____________________________________________________________________________
bool Test::False(const bool obs)
{
   bool equal = obs == false;
   if(!equal)
   {
      LOG("Test", pERROR) << "Test failed: Expected false. Found true.";
   }
   return equal;
}
//____________________________________________________________________________

}
}

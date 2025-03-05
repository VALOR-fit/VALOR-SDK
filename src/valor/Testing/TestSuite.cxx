//____________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

// VALOR/SDK
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Testing/TestSuite.h"

namespace valor
{
namespace testing
{
//____________________________________________________________________________
TestSuite::~TestSuite()
{
   for(auto kv : _tests)
   {
      delete kv.second;
   }
   _tests.clear();
}
//____________________________________________________________________________
bool TestSuite::Run()
{
   bool passed = true;
   for(auto kv : _tests)
   {
      std::string name = kv.first;
      Test* test = kv.second;
      test->Configure(_arg);
      test->AddTests();
      if(test->Run())
      {  // Test passed
         VLOG("Test", pINFO) << name << " passed.";
      }
      else
      {  // Test failed
         int nfailures = test->NFailures();
         int ntests = test->NTests();
         auto failures = test->Failures();
         VLOG("Test", pERROR) << name << " failed " << nfailures << " of " <<
            ntests << " tests:";
         for(auto failure : failures)
         {  // List the failed tests
            VLOG("Test", pERROR) << failure->Name() << " failed.";
         }
         passed = false;
      }
   }
   if(passed)
   {
      VLOG("Test", pINFO) << "All tests passed.";
   }

   return passed;
}
//____________________________________________________________________________
void TestSuite::Add(std::string name, Test* test)
{
   auto value = _tests.insert(std::make_pair(name, test));
   if(not(value.second))
   {  // Duplicate test name. Abort.
      VLOG("Test", pERROR) << "Non-unique test name: " << name << ". Aborting.";
      util::Die(-1);
   }
}
//____________________________________________________________________________

}
}

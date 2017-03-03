#include "valor/Testing/TestSuite.h"

// valor
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlSystem.h"

namespace valor
{
namespace testing
{

TestSuite::~TestSuite()
{
   for(auto kv : _tests)
   {
      delete kv.second;
   }
   _tests.clear();
}

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
         LOG("Test", pINFO) << name << " passed.";
      }
      else
      {  // Test failed
         int nfailures = test->NFailures();
         int ntests = test->NTests();
         auto failures = test->Failures();
         LOG("Test", pERROR) << name << " failed " << nfailures << " of " <<
            ntests << " tests:";
         for(auto failure : failures)
         {  // List the failed tests
            LOG("Test", pERROR) << failure->Name() << " failed.";
         }
         passed = false;
      }
   }
   if(passed)
   {
      LOG("Test", pINFO) << "All tests passed.";
   }

   return passed;
}

void TestSuite::Add(std::string name, Test* test)
{
   auto value = _tests.insert(std::make_pair(name, test));
   if(not(value.second))
   {  // Duplicate test name. Abort.
      LOG("Test", pERROR) << "Non-unique test name: " << name << ". Aborting.";
      util::Die(-1);
   }
}

}
}


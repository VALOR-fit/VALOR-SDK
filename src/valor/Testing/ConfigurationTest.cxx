#include "valor/Testing/ConfigurationTest.h"

// valor
#include "valor/Utilities/UtlFile.h"

namespace valor
{
namespace testing
{

bool ConfigurationTestCase::TestInstance()
{
   bool passed = true;

   Configuration& config1 = *(dynamic_cast<ConfigurationTest*>(_test)->_config);
   Configuration& config2 = *Configuration::Instance();

   // Check that both instances point to the same object
   passed = Test::True((&config1) == (&config2)) && passed;

   return passed;
}

void ConfigurationTest::Setup()
{
   _config = Configuration::Instance();
}

void ConfigurationTest::Teardown()
{
}

void ConfigurationTest::AddTests()
{
   ConfigurationTestCase* testCase = new ConfigurationTestCase(this);
   Add(new TestKey("TestInstance", testCase),
         [testCase](void) { return testCase->TestInstance(); });
}

}
}

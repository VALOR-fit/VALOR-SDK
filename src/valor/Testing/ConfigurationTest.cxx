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

bool ConfigurationTestCase::TestBool()
{
   bool passed = true;

   Configuration& config = *Configuration::Instance();

   config.Set("Test.True", true);
   config.Set("Test.False", false);

   // Check that boolean values are set correctly
   passed = Test::True(config.GetBool("Test.True")) && passed;
   passed = Test::False(config.GetBool("Test.False")) && passed;

   return passed;
}

bool ConfigurationTestCase::TestInt()
{
   bool passed = true;

   Configuration& config = *Configuration::Instance();

   config.Set("Test.Integer", 4);

   // Check that integer values are set correctly
   passed = Test::Equal(config.GetInt("Test.Integer"), 4) && passed;

   return passed;
}

bool ConfigurationTestCase::TestDouble()
{
   bool passed = true;

   Configuration& config = *Configuration::Instance();

   config.Set("Test.Double", 3.14159);

   // Check that double values are set correctly
   passed = Test::Equal(config.GetDouble("Test.Double"), 3.14159, 1E-5) && passed;

   return passed;
}

bool ConfigurationTestCase::TestString()
{
   bool passed = true;

   Configuration& config = *Configuration::Instance();

   config.Set("Test.String", "a string");

   // Check that string values are set correctly
   passed = Test::Equal(config.GetString("Test.String"), "a string") && passed;

   return passed;
}

bool ConfigurationTestCase::TestHistograms()
{
   bool passed = true;

   Configuration& config = *Configuration::Instance();

   TH1F* histo1D = new TH1F();
   config.Set("Test.Histogram1D", histo1D);

   // Check that 1D histograms are set correctly
   passed = Test::True(config.GetH1F("Test.Histogram1D") == histo1D) && passed;

   delete histo1D;

   TH2F* histo2D = new TH2F();
   config.Set("Test.Histogram2D", histo2D);

   // Check that 1D histograms are set correctly
   passed = Test::True(config.GetH2F("Test.Histogram2D") == histo2D) && passed;

   delete histo2D;

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
   Add(new TestKey("TestBool", testCase),
         [testCase](void) { return testCase->TestBool(); });
   Add(new TestKey("TestInt", testCase),
         [testCase](void) { return testCase->TestInt(); });
   Add(new TestKey("TestDouble", testCase),
         [testCase](void) { return testCase->TestDouble(); });
   Add(new TestKey("TestString", testCase),
         [testCase](void) { return testCase->TestString(); });
   Add(new TestKey("TestHistograms", testCase),
         [testCase](void) { return testCase->TestHistograms(); });
}

}
}

#include "valor/Testing/ConfigurationHandlerTest.h"

// valor
#include "valor/Utilities/Configuration.h"

namespace valor
{
namespace testing
{

bool ConfigurationHandlerTestCase::TestReadAnalysis()
{
   bool passed = true;

   ConfigurationHandlerTest& test = *(dynamic_cast<ConfigurationHandlerTest*>(
            _test));
   ConfigurationHandler& handler = *(test._handler);

   handler.ReadAnalysis(test._arg->GetString("test-config-analysis-file"));
   util::Configuration& config = (*util::Configuration::Instance());

   passed = Test::Equal(config.GetString("Analysis.samples"),
         "test/newxml/samples/samples.xml") && passed;
   passed = Test::Equal(config.GetString("Analysis.parameters"),
         "test/newxml/params/params.xml") && passed;
   passed = Test::Equal(config.GetString("Analysis.splines"),
         "test/newxml/splines/splines.xml") && passed;

   return passed;
}

void ConfigurationHandlerTest::Setup()
{
   _handler = new ConfigurationHandler();
}

void ConfigurationHandlerTest::Teardown()
{
  delete _handler;
}

void ConfigurationHandlerTest::AddTests()
{
   ConfigurationHandlerTestCase* testCase = new ConfigurationHandlerTestCase(this);
   Add(new TestKey("TestReadAnalysis", testCase),
         [testCase](void) { return testCase->TestReadAnalysis(); });
}

}
}


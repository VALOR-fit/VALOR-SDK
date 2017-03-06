#include "valor/Testing/PhysParamsTest.h"

// valor
#include "valor/Utilities/UtlFile.h"

namespace valor
{
namespace testing
{

bool PhysParamsTestCase::TestBestfit()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   passed = Test::Equal(params.Bestfit("kPhypMixing23"), 1.0) && passed;

   return passed;
}

bool PhysParamsTestCase::TestInitial()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   passed = Test::Equal(params.Initial("kPhypMixing23"), 1.0) && passed;

   return passed;
}

bool PhysParamsTestCase::TestMinimum()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   passed = Test::Equal(params.Minimum("kPhypMixing23"), 0.0) && passed;

   return passed;
}

bool PhysParamsTestCase::TestMaximum()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   passed = Test::Equal(params.Maximum("kPhypMixing23"), 2.0) && passed;

   return passed;
}

bool PhysParamsTestCase::TestStepSize()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   passed = Test::Equal(params.StepSize("kPhypMixing23"), 1.0e-4) && passed;

   return passed;
}

bool PhysParamsTestCase::TestScaling()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   passed = Test::Equal(params.Scaling("kPhypMixing23"), 1.0) && passed;

   return passed;
}

bool PhysParamsTestCase::TestNParams()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   // Check the number of parameters defined
   passed = Test::Equal(params.NParams(), 586) && passed;

   return passed;
}

bool PhysParamsTestCase::TestValidParams()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   // Check first parameter ID
   passed = Test::Equal(params["kPhypMixing23"], 0) && passed;
   // Check last parameter ID
   passed = Test::Equal(params["kNuisIntTarget"], 585) && passed;
   // Check intermediate parameter IDs
   passed = Test::Equal(params["kNuisFHCNuEFDFlux01"], 149) && passed;
   passed = Test::Equal(params["kNuisEffRHCnumuCC0pi1trk08"], 324) && passed;

   return passed;
}

bool PhysParamsTestCase::TestInvalidParams()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   // Check undefined parameter ID
   passed = Test::Equal(params["_kPhypNonExistent"], PhysParams::Undefined) && passed;

   return passed;
}

void PhysParamsTest::Setup()
{
   // Retrieve the location of the XML configuration file
   std::string selFilename = util::UtlFile::GetConfigurationFilename(
         _arg->GetString("params-sel-file"));
   std::string specFilename = util::UtlFile::GetConfigurationFilename(
         _arg->GetString("params-spec-file"));

   _params = PhysParams::Instance();
   _params->Read(specFilename);
//   _params->Select(selFilename);
}

void PhysParamsTest::Teardown()
{
   if(_params)
   {
      _params->Clear();
   }
}

void PhysParamsTest::AddTests()
{
   PhysParamsTestCase* testCase = new PhysParamsTestCase(this);
/*
   Add(new TestKey("TestNParams", testCase),
         [testCase](void) { return testCase->TestNParams(); });
   Add(new TestKey("TestValidParams", testCase),
         [testCase](void) { return testCase->TestValidParams(); });
   Add(new TestKey("TestInvalidParams", testCase),
         [testCase](void) { return testCase->TestInvalidParams(); });
*/
   Add(new TestKey("TestBesfit", testCase),
         [testCase](void) { return testCase->TestBestfit(); });
   Add(new TestKey("TestInitial", testCase),
         [testCase](void) { return testCase->TestInitial(); });
   Add(new TestKey("TestMinimum", testCase),
         [testCase](void) { return testCase->TestMinimum(); });
   Add(new TestKey("TestMaximum", testCase),
         [testCase](void) { return testCase->TestMaximum(); });
   Add(new TestKey("TestStepSize", testCase),
         [testCase](void) { return testCase->TestStepSize(); });
   Add(new TestKey("TestScaling", testCase),
         [testCase](void) { return testCase->TestScaling(); });
}

}
}

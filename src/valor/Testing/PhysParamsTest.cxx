#include "valor/Testing/PhysParamsTest.h"

// valor
#include "valor/Utilities/UtlFile.h"

namespace valor
{
namespace testing
{

bool PhysParamsTestCase::TestNParams()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   // Check the number of parameters defined
   passed = _test->Equal(params.NParams(), 586) && passed;
   // Check first parameter ID
   passed = _test->Equal(params["kPhypMixing23"], 0) && passed;
   // Check last parameter ID
   passed = _test->Equal(params["kNuisIntTarget"], 585) && passed;
   // Check intermediate parameter IDs
   passed = _test->Equal(params["kNuisFHCNuEFDFlux01"], 149) && passed;
   passed = _test->Equal(params["kNuisEffRHCnumuCC0pi1trk08"], 324) && passed;
   // Check undefined parameter ID
   passed = _test->Equal(params["_kPhypNonExistent"], PhysParams::Undefined) && passed;

   return passed;
}

void PhysParamsTest::Setup()
{
   // Retrieve the location of the XML configuration file
   std::string filename = _arg->GetString("params-file");
   std::string fullFilename = util::UtlFile::GetConfigurationFilename(filename);

   _params = PhysParams::Instance();
   _params->Read(fullFilename);
}

void PhysParamsTest::Teardown()
{
}

void PhysParamsTest::AddTests()
{
   PhysParamsTestCase* testCase = new PhysParamsTestCase(this);

   Add(new TestKey("SimpleTest", testCase),
         [testCase](void) { return testCase->TestNParams(); });
}

}
}

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

bool PhysParamsTestCase::TestIsMarginalised()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   passed = Test::False(params.IsMarginalised("kPhypMixing23")) && passed;

   return passed;
}

bool PhysParamsTestCase::TestGaussianPrior()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   passed = Test::Equal(params.GaussianPriorMean("kPhypMixing23"), 1.0) &&
      passed;
   passed = Test::Equal(params.GaussianPriorSigma("kPhypMixing23"), 0.2) &&
      passed;

   return passed;
}

bool PhysParamsTestCase::TestFlatPrior()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   passed = Test::Equal(params.FlatPriorMinimum("kPhypMixing23"), 0.0) &&
      passed;
   passed = Test::Equal(params.FlatPriorMaximum("kPhypMixing23"), 2.0) &&
      passed;

   return passed;
}

bool PhysParamsTestCase::TestUserPrior()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   passed = Test::Equal(params.UserPriorFilename("kPhypMixing23"), "") &&
      passed;

   return passed;
}

bool PhysParamsTestCase::TestIsFixed()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   passed = Test::False(params.IsFixed("kPhypMixing23")) && passed;

   return passed;
}

bool PhysParamsTestCase::TestIsNuisance()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   passed = Test::True(params.IsNuisance("kPhypMixing23")) && passed;

   return passed;
}

bool PhysParamsTestCase::TestMigradErrors()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   passed = Test::False(params.IsMigradErrorSet("kPhypMixing23")) && passed;
   passed = Test::Equal(params.MigradError("kPhypMixing23"), 0.0) && passed;

   return passed;
}

bool PhysParamsTestCase::TestHesseErrors()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   passed = Test::False(params.IsHesseErrorSet("kPhypMixing23")) && passed;
   passed = Test::Equal(params.HesseError("kPhypMixing23"), 0.0) && passed;

   return passed;
}

bool PhysParamsTestCase::TestMinosErrors()
{
   bool passed = true;

   PhysParams& params = *(dynamic_cast<PhysParamsTest*>(_test)->_params);

   passed = Test::False(params.IsMinosErrorSet("kPhypMixing23")) && passed;
   passed = Test::Equal(params.MinosLowError("kPhypMixing23"), 0.0) && passed;
   passed = Test::Equal(params.MinosHighError("kPhypMixing23"), 0.0) && passed;

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
   Add(new TestKey("TestIsMarginalised", testCase),
         [testCase](void) { return testCase->TestIsMarginalised(); });
   Add(new TestKey("TestGaussianPrior", testCase),
         [testCase](void) { return testCase->TestGaussianPrior(); });
   Add(new TestKey("TestFlatPrior", testCase),
         [testCase](void) { return testCase->TestFlatPrior(); });
   Add(new TestKey("TestUserPrior", testCase),
         [testCase](void) { return testCase->TestUserPrior(); });
   Add(new TestKey("TestIsFixed", testCase),
         [testCase](void) { return testCase->TestIsFixed(); });
   Add(new TestKey("TestIsNuisance", testCase),
         [testCase](void) { return testCase->TestIsNuisance(); });
   Add(new TestKey("TestMigradErrors", testCase),
         [testCase](void) { return testCase->TestMigradErrors(); });
   Add(new TestKey("TestHesseErrors", testCase),
         [testCase](void) { return testCase->TestHesseErrors(); });
   Add(new TestKey("TestMinosErrors", testCase),
         [testCase](void) { return testCase->TestMinosErrors(); });
}

}
}

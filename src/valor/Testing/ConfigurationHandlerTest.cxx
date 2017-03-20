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

bool ConfigurationHandlerTestCase::TestReadSampleList()
{
   bool passed = true;

   ConfigurationHandlerTest& test = *(dynamic_cast<ConfigurationHandlerTest*>(
            _test));
   ConfigurationHandler& handler = *(test._handler);

   handler.ReadAnalysis(test._arg->GetString("test-config-analysis-file"));
   util::Configuration& config = (*util::Configuration::Instance());

   passed = Test::Equal(config.GetString("sample_list.name"),
         "analysis_name") && passed;

   // First sample
   passed = Test::Equal(config.GetString("sample_list.sample_1.name"),
         "sample_name") && passed;
   passed = Test::Equal(config.GetString("sample_list.sample_1.detector"),
         "dune_nd") && passed;
   passed = Test::Equal(config.GetString("sample_list.sample_1.beam"),
         "lbnf_fhc") && passed;
   passed = Test::Equal(config.GetString("sample_list.sample_1.selection"),
         "numucc0pi1trk") && passed;
   passed = Test::Equal(config.GetString("sample_list.sample_1.data"),
         "observations.root") && passed;  
   passed = Test::Equal(config.GetString("sample_list.sample_1.mc"),
         "mc/dune_ndfit_2017a_FGT_pt4.xml") && passed; 
   passed = Test::Equal(config.GetString("sample_list.sample_1.data_class"),
         "valor::GenericSingleSampleFitData") && passed;  
   passed = Test::Equal(config.GetString("sample_list.sample_1.model_class"),
         "valor::GenericSingleSampleFitModel") && passed;

   return passed;
}

bool ConfigurationHandlerTestCase::TestReadParameterList()
{
   bool passed = true;

   ConfigurationHandlerTest& test = *(dynamic_cast<ConfigurationHandlerTest*>(
            _test));
   ConfigurationHandler& handler = *(test._handler);

   handler.ReadAnalysis(test._arg->GetString("test-config-analysis-file"));
   util::Configuration& config = (*util::Configuration::Instance());

   passed = Test::Equal(config.GetString("param_list.name"), "name") && passed;

   // Oscillation group
   passed = Test::Equal(config.GetString("param_list.osc.parameters"),
         "params/oscillation/osc_params.xml") && passed;
   passed = Test::Equal(config.GetString("param_list.osc.penalty.type"),
         "gaussian") && passed;
   passed = Test::Equal(config.GetString("param_list.osc.penalty.cov.filename"),
         "osc_covariance.root") && passed;
   passed = Test::Equal(config.GetString("param_list.osc.penalty.cov.objname"),
         "osc_covariance") && passed;
   passed = Test::Equal(config.GetString("param_list.osc.penalty.cov.objtype"),
         "TMatrixD") && passed;

   // Flux systematics group
   passed = Test::Equal(config.GetString("param_list.flux.parameters"),
         "params/systematic/flux/flux_params.xml") && passed;
   passed = Test::Equal(config.GetString("param_list.flux.penalty.type"),
         "gaussian") && passed;
   passed = Test::Equal(config.GetString("param_list.flux.penalty.cov.filename"),
         "flux_covariance.root") && passed;
   passed = Test::Equal(config.GetString("param_list.flux.penalty.cov.objname"),
         "flux_covariance") && passed;
   passed = Test::Equal(config.GetString("param_list.flux.penalty.cov.objtype"),
         "TMatrixD") && passed;
   passed = Test::Equal(config.GetString("param_list.flux.postfit.filename"),
         "flux_postfit.root") && passed;
   passed = Test::Equal(config.GetString("param_list.flux.postfit.objname"),
         "flux_postfit") && passed;
   passed = Test::Equal(config.GetString("param_list.flux.postfit.objtype"),
         "TVectorD") && passed;

   return passed;
}

void ConfigurationHandlerTest::Setup()
{
   _handler = new ConfigurationHandler();
   util::Configuration::Instance();
}

void ConfigurationHandlerTest::Teardown()
{
  delete _handler;
  util::Configuration::Instance()->Clear(true);;
}

void ConfigurationHandlerTest::AddTests()
{
   ConfigurationHandlerTestCase* testCase = new ConfigurationHandlerTestCase(this);
   Add(new TestKey("TestReadAnalysis", testCase),
         [testCase](void) { return testCase->TestReadAnalysis(); });
   Add(new TestKey("TestReadSampleList", testCase),
         [testCase](void) { return testCase->TestReadSampleList(); });
   Add(new TestKey("TestReadParameterList", testCase),
         [testCase](void) { return testCase->TestReadParameterList(); });
}

}
}


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
         "test/newxml/params/oscillation/osc_params.xml") && passed;
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
         "test/newxml/params/systematic/flux/flux_params.xml") && passed;
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

   // Interaction systematics group
   passed = Test::Equal(config.GetString("param_list.interaction.parameters"),
         "test/newxml/params/systematic/interaction/int_params.xml") && passed;
   passed = Test::Equal(config.GetString("param_list.interaction.penalty.type"),
         "gaussian") && passed;
   passed = Test::Equal(config.GetString(
            "param_list.interaction.penalty.cov.filename"),
            "int_covariance.root") && passed;
   passed = Test::Equal(config.GetString(
            "param_list.interaction.penalty.cov.objname"),
            "int_covariance") && passed;
   passed = Test::Equal(config.GetString(
            "param_list.interaction.penalty.cov.objtype"),
            "TMatrixD") && passed;
   passed = Test::Equal(config.GetString(
            "param_list.interaction.postfit.filename"),
            "int_postfit.root") && passed;
   passed = Test::Equal(config.GetString(
            "param_list.interaction.postfit.objname"),
            "int_postfit") && passed;
   passed = Test::Equal(config.GetString(
            "param_list.interaction.postfit.objtype"),
            "TVectorD") && passed;

   // Detector systematics group
   passed = Test::Equal(config.GetString("param_list.detector.parameters"),
         "test/newxml/params/systematic/detector/det_params.xml") && passed;
   passed = Test::Equal(config.GetString("param_list.detector.penalty.type"),
         "gaussian") && passed;
   passed = Test::Equal(config.GetString(
            "param_list.detector.penalty.cov.filename"),
            "det_covariance.root") && passed;
   passed = Test::Equal(config.GetString(
            "param_list.detector.penalty.cov.objname"),
            "det_covariance") && passed;
   passed = Test::Equal(config.GetString(
            "param_list.detector.penalty.cov.objtype"),
            "TMatrixD") && passed;
   passed = Test::Equal(config.GetString(
            "param_list.detector.postfit.filename"),
            "det_postfit.root") && passed;
   passed = Test::Equal(config.GetString(
            "param_list.detector.postfit.objname"),
            "det_postfit") && passed;
   passed = Test::Equal(config.GetString(
            "param_list.detector.postfit.objtype"),
            "TVectorD") && passed;

   return passed;
}

bool ConfigurationHandlerTestCase::TestReadParameters()
{
   bool passed = true;

   ConfigurationHandlerTest& test = *(dynamic_cast<ConfigurationHandlerTest*>(
            _test));
   ConfigurationHandler& handler = *(test._handler);

   handler.ReadAnalysis(test._arg->GetString("test-config-analysis-file"));
   util::Configuration& config = (*util::Configuration::Instance());

   passed = Test::Equal(config.GetString("param_list.name"), "name") && passed;

   // Defaults
   passed = Test::Equal(config.GetDouble("param_list.defaults.bestfit"), 1.0)
         && passed;
   passed = Test::Equal(config.GetDouble("param_list.defaults.init"), 1.0)
         && passed;
   passed = Test::Equal(config.GetDouble("param_list.defaults.minimum"), 0.0)
         && passed;
   passed = Test::Equal(config.GetDouble("param_list.defaults.maximum"), 2.0)
         && passed;  
   passed = Test::Equal(config.GetDouble("param_list.defaults.step_size"), 0.0001)
         && passed;  
   passed = Test::Equal(config.GetDouble("param_list.defaults.scaling"), 1.0)
         && passed;
   passed = Test::False(config.GetBool("param_list.defaults.fixed"))
         && passed;  
   passed = Test::True(config.GetBool("param_list.defaults.nuisance"))
         && passed;  
   passed = Test::False(config.GetBool("param_list.defaults.marginalise"))
         && passed; 

   // Parameters
   passed = Test::Equal(config.GetString("param_list.param_1.name"),
         "kPhypMixing13") && passed;
   passed = Test::Equal(config.GetDouble("param_list.param_1.bestfit"), 0.093)
         && passed;
   passed = Test::Equal(config.GetDouble("param_list.param_1.init"), 0.093)
         && passed;
   passed = Test::Equal(config.GetDouble("param_list.param_1.minimum"), 0.077)
         && passed;
   passed = Test::Equal(config.GetDouble("param_list.param_1.maximum"), 0.109)
         && passed;  
   passed = Test::Equal(config.GetDouble("param_list.param_1.step_size"), 0.0001)
         && passed;  
   passed = Test::Equal(config.GetDouble("param_list.param_1.scaling"), 1.0)
         && passed;
   passed = Test::False(config.GetBool("param_list.param_1.fixed"))
         && passed;  
   passed = Test::False(config.GetBool("param_list.param_1.nuisance"))
         && passed;  
   passed = Test::True(config.GetBool("param_list.param_1.marginalise"))
         && passed;
   passed = Test::Equal(config.GetDouble(
            "param_list.param_1.errors.migrad.error"), 0.6) && passed;

   // Selected systematic values
   passed = Test::Equal(config.GetString("param_list.param_2.name"),
         "kNuisFHCNuMuFlux00") && passed;
   passed = Test::Equal(config.GetDouble("param_list.param_2.maximum"), 2.0)
         && passed;  
   passed = Test::True(config.GetBool("param_list.param_2.marginalise"))
         && passed;
   passed = Test::Equal(config.GetString("param_list.param_3.name"),
         "kNuisIntNuCCQE1") && passed;
   passed = Test::Equal(config.GetDouble("param_list.param_3.init"), 1.0)
         && passed;
   passed = Test::Equal(config.GetString("param_list.param_4.name"),
         "kNuisEffNuMuCC0pi1trk") && passed;
   passed = Test::Equal(config.GetDouble("param_list.param_4.bestfit"), 1.0)
         && passed;

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
   Add(new TestKey("TestReadParameters", testCase),
         [testCase](void) { return testCase->TestReadParameters(); });
}

}
}


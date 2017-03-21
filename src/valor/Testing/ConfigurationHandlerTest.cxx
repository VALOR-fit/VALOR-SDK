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

   util::Configuration& config = (*util::Configuration::Instance());

   passed = Test::Equal(config.GetString("analysis.samples"),
         "test/newxml/samples/samples.xml") && passed;
   passed = Test::Equal(config.GetString("analysis.parameters"),
         "test/newxml/params/params.xml") && passed;
   passed = Test::Equal(config.GetString("analysis.splines"),
         "test/newxml/splines/splines.xml") && passed;

   return passed;
}

bool ConfigurationHandlerTestCase::TestReadSampleList()
{
   bool passed = true;

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
         "test/newxml/mc/mc.xml") && passed; 
   passed = Test::Equal(config.GetString("sample_list.sample_1.data_class"),
         "valor::GenericSingleSampleFitData") && passed;  
   passed = Test::Equal(config.GetString("sample_list.sample_1.model_class"),
         "valor::GenericSingleSampleFitModel") && passed;

   return passed;
}

bool ConfigurationHandlerTestCase::TestReadMC()
{
   bool passed = true;

   util::Configuration& config = (*util::Configuration::Instance());

   passed = Test::Equal(config.GetString("mc.name"), "mc_name") && passed;
   passed = Test::Equal(config.GetString("mc.version"), "ver") && passed;
   passed = Test::Equal(config.GetString("mc.detector"), "det") && passed;
   passed = Test::Equal(config.GetDouble("mc.exposure.fhc"), 5.1518E21) &&
      passed;
   passed = Test::Equal(config.GetDouble("mc.exposure.rhc"), 5.1518E21) &&
      passed;
   passed = Test::Equal(config.GetDouble("mc.fiducial_mass"), 35.09) &&
      passed;
   passed = Test::True(config.GetBool("mc.use_dst")) && passed;
   passed = Test::Equal(config.GetString("mc.dst.fhc.filename"),
         "FHC_NuE_high.root") && passed;
   passed = Test::Equal(config.GetString("mc.dst.fhc.objname"),
         "valor_dune_nd_mini_dst") && passed;
   passed = Test::Equal(config.GetString("mc.dst.fhc.objtype"),
         "TTree") && passed;
   passed = Test::Equal(config.GetString("mc.dst.rhc.filename"),
         "RHC_NuE_high.root") && passed;
   passed = Test::Equal(config.GetString("mc.dst.rhc.objname"),
         "valor_dune_nd_mini_dst") && passed;
   passed = Test::Equal(config.GetString("mc.dst.rhc.objtype"),
         "TTree") && passed;
   passed = Test::Equal(config.GetString("mc.template.filename"),
         "template.root") && passed;
   passed = Test::Equal(config.GetString("mc.template.tag"),
         "pass") && passed;
   passed = Test::Equal(config.GetDouble("mc.interaction_mass"), -999.0) &&
      passed;
   passed = Test::Equal(config.GetString("mc.default_binning"),
         "test/newxml/binning/fd_elike.xml") && passed;

   return passed;
}

bool ConfigurationHandlerTestCase::TestReadParameterList()
{
   bool passed = true;

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

bool ConfigurationHandlerTestCase::TestReadSplines()
{
   bool passed = true;

   util::Configuration& config = (*util::Configuration::Instance());

   passed = Test::Equal(config.GetString("splines.folder"),
         "dune/ndfit/2016b/fd/splines") && passed;
   passed = Test::Equal(config.GetString("splines.pattern"),
         "valor_splines_[BEAM]_[SELECTION]_[PARAM].root") && passed;

   return passed;
}

void ConfigurationHandlerTest::Setup()
{
   util::Configuration::Instance();
   _handler = new ConfigurationHandler();
   _handler->ReadAnalysis(_arg->GetString("test-config-analysis-file"));
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
   Add(new TestKey("TestReadSplines", testCase),
         [testCase](void) { return testCase->TestReadSplines(); });
   Add(new TestKey("TestReadMC", testCase),
         [testCase](void) { return testCase->TestReadMC(); });
}

}
}


//____________________________________________________________________________
/*!
 
\app       valor-run_fit

\brief     Generate and fit ensembles of toy-MC experiments.

\options   --tag                 : Tag for output file [def: "toymc"]
           --run                 : Run # [def: 0]
           --ntoymc              : Number of toy-MC experiments [def: 1]
           --physics-hypothesis  : Physics hypothesis (see UtlParam.cxx)
                                   [def: "3f_osc"]
           --fit-param-opt       : Options used when setting up the FitParamList
                                   [def: "NONE"]
           --param-values-option : Decide how to set the values of physics
                                   params used for toy-MC generation
                                  [def:"randomize-all;prefit-errors"]
           --fitter              : Fitter option [def: "minuit"]
           --config              : Analysis configuration file for datasetA [mandatory]
           --config-datasetB     : Analysis configuration file for datasetB (for chi² comparison) [mandatory]
           --mesg-thresholds     : Message thresholds [def: "verbosity.xml"]
          [--with-stat-fluct |     : Include statistical fluctuation in toy-MC generation? [def: with-stat-fluct]
           --without-stat-fluct]
          [--store-spectra   |     : Store toy-MC/best-fit spectra? [def: discard-spectra]
           --discard-spectra]
           --start_at_random_points : Should we start at a random point? If so, how many [-1, start at default]
           --input-mixing12,etc  : Option to give input values to osc params (in .arg)
           --fix-mixing12,etc    : Option to fix osc params (in .arg)
           --fix-at-mixing12,etc : Option to fix osc params to a different value than true/central for the fit
           --load-prior-from-file : Option for file to give constrained prior matrix [ use default prior ]
           --prior-fit-number     : Option to specify the entry in the tree for above file [ 0 ]
           --tweak-parameter-set  : Option to tweak a set of parameters [ default: none ]
           --force-tweak-size     : Tweak the parameters to a given +n-sigma. If given 0, randomize [ randomize ]
           --flip-true-octant     : Option to flip the true octant [default: 0]
           --read-data, generate-data : Read or Generate data (default: generate-data)
           --nthreads            : Number of CPU threads to use [default: std::thread::hardware_concurrency]

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   March 27, 2014

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

// ROOT headers
#include "TFile.h"
#include "TSystem.h"
#include "TObjArray.h"
#include "TString.h"
#include "TVirtualFitter.h"
#include "TPRegexp.h"
#include <time.h>

// VALOR/SDK headers
#include "valor/Conventions/Constants.h"
#include "valor/Conventions/Opt.h"
#include "valor/Conventions/ParameterPool.h"
#include "valor/CmdLine/CmdLnArgList.h"
#include "valor/FitCore/CorrelatedSystematics.h"
#include "valor/FitCore/PhysParamList.h"
#include "valor/FitCore/FitParamList.h"
#include "valor/FitCore/FitModelABC.h"
#include "valor/FitCore/FitDataABC.h"
#include "valor/FitCore/Likelihood.h"
#include "valor/FitCore/FitterABC.h"
#include "valor/FitCore/CompoundFitElements.h"
#include "valor/FitCore/UtlFit.h"
#include "valor/FitCore/UtlOscillation.h"
#include "valor/FitCore/UtlFitParamList.h"
#include "valor/Oscillation/SimpleOsc.h"
#include "valor/Utilities/OscParamValues.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/UtlStyle.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/Env.h"
#include "valor/Utilities/VThreadPool.h"
#include "valor/Utilities/UtlParam.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Ntuple/StdFitNtp.h"

using namespace std;
using namespace valor;
using namespace valor::conv;

const char * fit_result_tree_name = "valor_fit_ntp";

int main(int argc, char ** argv)
{
  // Record start time
  time_t startrawtime;
  struct tm * starttimeinfo;
  time(&startrawtime);
  starttimeinfo = localtime(&startrawtime);
  std::cout << "-----------------------------------------------------------" << std::endl;
  std::cout << " Start local time and date:  " << asctime(starttimeinfo);
  std::cout << "-----------------------------------------------------------" << std::endl;

  // Print banner
  valor::util::PrintBanner(Env::Instance()->Banner(), 500);

  // Initialize command-line arguments from XML files and CLI
  CmdLnArgList arg;
  arg.Read(argc, argv, "$VALOR_FMWK/config/apps/run_fit.arg:$VALOR_FMWK/config/apps/seeds.arg:$VALOR_FMWK/config/apps/oscparams.arg");
  LOG("valor", pNOTICE) << arg;

  // Retrieve various command-line options
  TString param_values_ts(arg.GetString("param-values-option"));
  TString fit_options_ts(arg.GetString("fitter"));
  bool with_stat_fluct    = arg.GetBool("with-stat-fluct");
  bool start_at_true      = arg.GetBool("start-at-true");
  bool rethrow_unphysical = arg.GetBool("rethrow-unphysical");
  int ntoymc              = arg.GetInt("ntoymc");
  int n_threads           = arg.GetInt("nthreads");
  string hypname          = arg.GetString("physics-hypothesis");
  string fplopt           = arg.GetString("fit-param-opt");
  string fittername       = arg.GetString("fitter");
  string true_values_file = arg.GetString("true-from-filename");
  string true_values_obj  = arg.GetString("true-from-objname"); // A PhysParamList object
  bool flip_23_octant     = arg.GetBool("flip-true-octant");
  bool read_data          = arg.GetBool("read-data");
  string read_prior_from_file = arg.GetString("load-prior-from-file");
  int prior_fit_number    = arg.GetInt("prior-fit-number");
  string tweak_parameter_set = arg.GetString("tweak-parameter-set");
  double force_tweak_size = arg.GetDouble("force-tweak-size");

  // Override POT if specified
  double pot_override = arg.GetDouble("pot-override");
  if (pot_override > 0) {
    Opt::Instance()->MutableGlobalRegistry().Set("pot_override", pot_override);
  }

  std::string mesg_thresholds = util::CompleteConfigPath(arg.GetString("mesg-thresholds"));
  Messenger::Instance()->Configure(mesg_thresholds);

  // Set up thread pool
  if(n_threads > 0) {
    util::VThreadPool::Instance()->SetNThreads(n_threads);
  }

  // Set random seeds
  Rnd::Instance()->SetSeeds(arg);
  LOG("valor", pNOTICE) << *Rnd::Instance();

  // Read true values from file if provided
  PhysParamList * true_values_from_file_ptr = 0;
  if (true_values_file.size() && util::ToLowerCase(true_values_file) != "none") {
    TFile tvf(true_values_file.c_str());
    if (!tvf.IsOpen()) {
      LOG("valor", pERROR) << "Could not read true values file [" << true_values_file << "]";
      util::Die(-1);
    }
    true_values_from_file_ptr = dynamic_cast<PhysParamList*>(tvf.Get(true_values_obj.c_str()));
    if (!true_values_from_file_ptr) {
      LOG("valor", pERROR) << "Could not read true values object " << true_values_obj 
                           << " from file " << true_values_file;
      util::Die(-1);
    }
  }

  // Create output ROOT file and tree
  TString outfilename = TString::Format("%s_%d.root", arg.GetString("tag").c_str(), arg.GetInt("run"));
  StdFitNtp ntp;
  ntp.Create(outfilename.Data(), fit_result_tree_name, "VALOR fit n-tuple");

  //=======================================================================
  // Modified Section: Load two configuration files for two separate datasets.
  // - DatasetA: Used for generating toy‐MC (modelA and dataA)
  // - DatasetB: Used for chi² comparison (modelB)
  //=======================================================================
  // Load datasetA (toy‐MC generation) configuration from --config
  CompoundFitElements * elementsA = new CompoundFitElements();
  std::string config_path_A = util::CompleteConfigPath(arg.GetString("config"));
  elementsA->Read(config_path_A);
  FitModelABC * modelA = elementsA->FitModel();
  FitDataABC * dataA = elementsA->FitData();

  // Load datasetB (chi² comparison) configuration from --config-datasetB
  CompoundFitElements * elementsB = new CompoundFitElements();
  std::string config_path_B = util::CompleteConfigPath(arg.GetString("config-datasetB"));
  elementsB->Read(config_path_B);
  // Here, we only need the model from datasetB (modelB)
  FitModelABC * modelB = elementsB->FitModel();
  // Optionally, check whether modelB is valid

  const util::NameRegistry * nr = util::NameRegistry::Instance();
  // Use systematics from modelA (for toy‐MC generation)
  CorrelatedSystematics * syst = modelA->GetSystematics();

  // If necessary, update the systematics prior from a previous fit
  if (read_prior_from_file != "-" && read_prior_from_file != "none") {
    TFile prior_file(read_prior_from_file.c_str());
    TTree * prior_tree = dynamic_cast<TTree*>(prior_file.Get(fit_result_tree_name));
    assert(prior_tree);
    FitParamList * prior_fpl = NULL;
    prior_tree->SetBranchAddress("best_fit_params", &prior_fpl);
    prior_tree->GetEntry(prior_fit_number);
    syst->UpdateCentralValuesAndCovarianceFromFPL(*prior_fpl);
    delete prior_tree;
    prior_file.Close();
  }
  //=======================================================================
  // End of configuration separation modifications.
  //=======================================================================

  // Use ND model to generate toy-MC experiments & fit
  for (int itoymc = 0; itoymc < ntoymc; itoymc++) {
    LOG("valor", pNOTICE) << "\n\n *** Toy-MC experiment #" << itoymc;
    // Reset output tree variables
    ntp.Reset();

    // Get the specified fit parameter list (specifies which parameters are floated/fixed,
    // parameter limits, initial values etc).
    FitParamList * fp = util::GetFitParameterList(hypname.c_str(), fplopt.c_str());

    if(fp == 0){ // Make sure that worked
      VLOG("valor",pFATAL) << "FitParamList not filled";
      util::Die(-1);
    }

    // Reset systematics using modelA
    modelA->ResetSystematics(*fp, "all");

    // If required, randomize systematics
    if (param_values_ts.Contains("randomize")) {
      modelA->RandomizeSystematics(*fp, param_values_ts.Data());
    }

    // Process tweak parameters if specified
    if (tweak_parameter_set != "none" && tweak_parameter_set != "-") {
      vector<PhysParamId_t> param_set = util::GetParameterSet(tweak_parameter_set);
      assert(param_set.size());
      if (std::fabs(force_tweak_size) > conv::kAVerySmallNum) {
        for (unsigned int ip = 0; ip < param_set.size(); ip++) {
          PhysParamId_t p = param_set[ip];
          if (!syst->Contains(p)) continue;
          double central = syst->InitialValue(p);
          double onesig = syst->OneSigmaError(p);
          double tweaked = central + force_tweak_size * onesig;
          syst->SetCurrentValue(p, tweaked);
        }
      } else {
        std::map<PhysParamId_t, bool> param_map;
        for (unsigned int ip = 0; ip < param_set.size(); ip++) {
          PhysParamId_t p = param_set[ip];
          param_map[p] = true;
        }
        modelA->RandomizeSystematics(param_map, *fp);
      }
    }

    PhysParamList tweaks = modelA->Tweaks();
    fp->FromPhysParamList(tweaks, "current");
    if (fp == 0) {
      LOG("valor", pFATAL) << "FitParamList not filled after tweaks applied";
      util::Die(-1);
    }

    // Check parameter name, maybe rethrow
    if (!util::AllWithinLimits(*fp)) {
      VLOG("valor", pNOTICE) << "At least one parameter thrown outside expected limits.";
      if (rethrow_unphysical) {
        LOG("valor", pNOTICE) << "Regenerating...";
        itoymc--;
        continue;
      }
    }

    // If true values were read from file, override the current parameters
    if (true_values_from_file_ptr)
      fp->FromPhysParamList(*true_values_from_file_ptr, "current");

    // Set global oscillation options based on command-line flags
    Opt::Instance()->gMassHierarchyAssumed = ((arg.GetBool("inverted")) ? conv::kNmhInverted : conv::kNmhNormal);
    Opt::Instance()->gOscMixingParamChoice = ((arg.GetBool("sinsq2theta")) ? conv::kNuMixSinsq2Theta : conv::kNuMixSinsqTheta);
    Opt::Instance()->gOscOctantChoice = ((arg.GetBool("second-octant")) ? conv::kNuOctantTwo : conv::kNuOctantOne);

    conv::NuOscDmsqScheme_t dmsq_scheme = util::DmsqSchemeFromString(arg.GetString("dmsq-scheme"));
    if (dmsq_scheme == kNuDmsqUndefined) {
      LOG("valor", pFATAL) << "Could not parse the input Dmsq scheme: [" << arg.GetString("dmsq-scheme") << "]";
      util::Die(-1);
    }
    Opt::Instance()->gOscDmsqScheme = dmsq_scheme;

    util::oscillation::SetParameterPriors(arg, *fp);

    // Process input parameters
    RgKeyList input_args = arg.FindKeys("input-");
    TPRegexp input_match("input-(\\w*)");
    for (RgKey argname : input_args) {
      TObjArray * matches = input_match.MatchS(argname.c_str());
      if (matches->GetEntries() != 2) {
        LOG("valor", pFATAL) << "Cannot parse argument " << argname << " to get parameter name";
        util::Die(-1);
      }
      TObjString * tos_param_name = dynamic_cast<TObjString*>(matches->At(1));
      std::string param_name(tos_param_name->String().Data());
      delete matches;
      conv::PhysParamId_t param = nr->PhysParamFromString(param_name);
      if (param == conv::kPhypUndefined) {
        LOG("valor", pFATAL) << "Cannot find parameter to fix from argument string " << argname;
        util::Die(-1);
      }
      double param_val = arg.GetDouble(argname);
      if (std::fabs(param_val + 999.) < conv::kASmallNum)
        param_val = Opt::Instance()->GetGlobalBestFit(param);
      if (param == kPhypMixing23 && flip_23_octant)
        param_val = util::FlipOctantAroundMaxDisap(param_val, arg.GetDouble("input-mixing13"));
      if (fp->Exists(param)) {
        fp->Update(param, param_val);
        fp->SetInitValue(param, param_val);
      }
    }

    // Process fixed parameters
    RgKeyList fix_args = arg.FindKeys("fix-");
    TPRegexp fix_match("fix-(\\w*)");
    for (RgKey argname : fix_args) {
      TObjArray * matches = fix_match.MatchS(argname.c_str());
      if (matches->GetEntries() != 2) {
        LOG("valor", pFATAL) << "Cannot parse argument " << argname << " to get parameter name";
        util::Die(-1);
      }
      TObjString * tos_param_name = dynamic_cast<TObjString*>(matches->At(1));
      std::string param_name(tos_param_name->String().Data());
      delete matches;
      if (param_name == "at")
        continue;
      conv::PhysParamId_t param = nr->PhysParamFromString(param_name);
      if (param == conv::kPhypUndefined) {
        LOG("valor", pFATAL) << "Cannot find parameter to fix from argument string " << argname;
        util::Die(-1);
      }
      if (fp->Exists(param)) {
        if (arg.GetBool(argname))
          fp->FixParameter(param);
        else if (param_values_ts.Contains("randomize-all")) {
          double rnd_value = util::oscillation::GetRandomizedParam(param, *fp);
          fp->Update(param, rnd_value);
          fp->SetInitValue(param, rnd_value);
        }
      } else {
        LOG("valor", pINFO) << "Not setting parameter not included in FPL: " << param_name;
      }
    }

    PhysParamList true_params = fp->AsPhysParamList("current");

    //=======================================================================
    // Modification: Use datasetA (modelA and dataA) to generate toy-MC data.
    //=======================================================================
    FitDataABC * data = dataA; // Use datasetA's data object
    if (!read_data) {
      if (!elementsA->DataGiven())
        LOG("valor", pNOTICE) << "No data given to Read, Generating the toy-MC instead";
      else
        LOG("valor", pNOTICE) << "Data given but will be overwritten by Generate";
      LOG("valor", pNOTICE) << "Generating toy-MC data using datasetA";
      LOG("valor", pNOTICE) << "Toy-MC generation uses true parameters: " << true_params;
      data->Generate(modelA, *fp, with_stat_fluct);
    } else {
      if (!elementsA->DataGiven()) {
        LOG("valor", pFATAL) << "Told to Read data but no data file was given ";
        util::Die(-1);
      }
      LOG("valor", pNOTICE) << "Reading data from file, not Generating the toy-MC";
    }

    // Save the true parameter values for output
    ntp.TrueParamArr()->Append(true_params);

    if (start_at_true) {
      // Reset fixed parameters to initial values
      for (int ip = 0; ip < fp->GetNumberParameters(); ip++) {
        if (fp->IsFixedByIndex(ip)) {
          fp->SetCurrentValueByIndex(ip, fp->InitValueByIndex(ip));
        }
      }
      LOG("valor", pNOTICE) << "Copying current values to initial values to help the fitter.";
      fp->CopyCurrToInitValues();
    }

    // Process "fix-at-" arguments (fix parameter at a specified value)
    RgKeyList fix_at_args = arg.FindKeys("fix-at-");
    TPRegexp fix_at_match("fix-at-(\\w*)");
    for (RgKey argname : fix_at_args) {
      TObjArray * matches = fix_at_match.MatchS(argname.c_str());
      if (matches->GetEntries() != 2) {
        LOG("valor", pFATAL) << "Cannot parse argument " << argname << " to get parameter name";
        util::Die(-1);
      }
      TObjString * tos_param_name = dynamic_cast<TObjString*>(matches->At(1));
      std::string param_name(tos_param_name->String().Data());
      delete matches;
      conv::PhysParamId_t param = nr->PhysParamFromString(param_name);
      if (param == conv::kPhypUndefined) {
        LOG("valor", pFATAL) << "Cannot find parameter to fix from argument string " << argname;
        util::Die(-1);
      }
      if (fp->Exists(param) && fp->IsFixed(param)) {
        double fix_param_at = arg.GetDouble(argname);
        if (std::fabs(fix_param_at + 999.) < conv::kASmallNum)
          continue;
        if (!util::IsSystematic(param)) {
          if (!ParameterPool::Instance()->IsParamInPool(param)) {
            LOG("UtlOscillation", pFATAL) << "The osc param '" << nr->ParamAsString(param)
                                          << "' was in the FPL but not the ParameterPool";
            util::Die(-1);
          }
          const Parameter & parameter = ParameterPool::Instance()->GetParam(param);
          double min = parameter.GetLimits().first;
          double max = parameter.GetLimits().second;
          if ((fix_param_at < min) || (fix_param_at > max)) {
            LOG("valor", pFATAL) << "Attempting to fix parameter " << nr->ParamAsString(param)
                                  << " outside limits at " << fix_param_at << " - limits are " << min << " to " << max;
            util::Die(-1);
          }
        }
        fp->Update(param, fix_param_at);
        fp->SetInitValue(param, fix_param_at);
      }
    }
    fp->ResetCurrToInitValues();

    //=======================================================================
    // Modification: Create the fitter using datasetA's toy-MC data and modelB for chi² comparison.
    // Although toy-MC is generated from modelA (dataA), the Likelihood is built using modelB.
    //=======================================================================
    LOG("valor", pNOTICE) << "Preparing the fitter: comparing modelB with toy-MC generated by modelA";
    Likelihood * likelihood = new Likelihood(data, modelB, fp);
    FitterABC * fitter = util::GetFitter(likelihood, fittername.c_str());
    LOG("valor", pNOTICE) << "Running the fitter";
    LOG("valor", pNOTICE) << "Fit starts with the following fit parameter list: " << *fp;
    // divide by 100
    LOG("valor", pNOTICE) << "Fit starts with statistic: " << likelihood->Statistic() / 100.0;
    fitter->DoTheFit();
    LOG("valor", pNOTICE) << "Fit ends with statistic: " << likelihood->Statistic() / 100.0;

    ntp.BestFitParamArr()->Copy(*fp);

    if (arg.GetBool("store-spectra")) {
      fp->CopyBestFitToCurrValues();
      modelA->Generate(*fp, "+SB");  // Optionally regenerate using modelA for spectra
      TObjArray * best_fit_model_array = ntp.BestFitModelArr();
      modelA->Write(best_fit_model_array, "best_fit", "pdfc_rf,pdft_rf,pdfts_rf,pdfcs_rf +SB");
      TObjArray * data_array = ntp.DataArr();
      data->Write(data_array, "toymc", "all");
    }
    ntp.Fill();

    delete fp;
    delete likelihood;
    delete fitter;
  } // end toy-MC loop

  ntp.Save();
  std::cout << "Done." << std::endl << " GO VALOR!!!" << std::endl;

  time_t endrawtime;
  struct tm * endtimeinfo;
  time(&endrawtime);
  endtimeinfo = localtime(&endrawtime);
  time_t total_time = endrawtime - startrawtime;
  std::cout << "-----------------------------------------------------------" << std::endl;
  std::cout << " End local time and date  :  " << asctime(endtimeinfo);
  std::cout << " Total time taken         :  " << total_time / static_cast<double>(60) << " minutes" << std::endl;
  std::cout << "-----------------------------------------------------------" << std::endl;

  return 0;
}

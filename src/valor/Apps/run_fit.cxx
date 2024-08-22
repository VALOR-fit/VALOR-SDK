//____________________________________________________________________________
/*!

\app       valor-run_fit

\brief     Generate and fit ensembles of toy-MC experiments.

\options     --tag                 : Tag for output file [def: "toymc"]
             --run                 : Run # [def: 0]
             --ntoymc              : Number of toy-MC experiments [def: 1]
             --physics-hypothesis  : Physics hypothesis (see UtlParam.cxx)
                                     [def: "3f_osc"]
             --fit-param-opt       : Options used when setting up the FitParamList
                                     [def: "NONE"]
             --param-values-option : Decide how to set the values of physics
                                     params used for toy-MC generation
                                    [def:"randomize-all;prefit-errors"]
             --fit-opt             : Fitter option [def: "minuit"]
             --config              : Analysis configuration file [required]
             --mesg-thresholds     : Message thresholds [def: "verbosity.xml"]
            [--with-stat-fluct |   : Include statistical fluctuation in toy
                                     MC generation? [def: yes]
             --without-stat-fluct]
            [--store-spectra   |   : Store toy-MC/best-fit spectra? [def: no]
             --discard-spectra   ]

             --start_at_random_points : Should we start at a random point? If so, how many [-1, start at default]

             --input-mixing12,etc  : Option to give input values to osc params (in .arg)
             --fix-mixing12,etc    : Option to fix osc params (in .arg)
             --fix-at-mixing12,etc : Option to fix osc params to a different value than true/central for the fit

             --load-prior-from-file : Option for file to give constrained prior matrix [ use default prior ]
             --prior-fit-number     : Option to specify the entry in the tree for above file [ 0 ]

             --tweak-parameter-set : Option to tweak a set of parameters [ default: none ]
             --force-tweak-size    : Tweak the parameters to a given +n-sigma. If given 0, randomize [ randomize ]
             [ --read-data,
             --generate-data ]     : Generate data or Read data given to the configuration file [default: --generate-data]

             [--nthreads ]         : Number of CPU threads to use [ std::thread::hardware_concurrency ]
             *

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   March 27, 2014

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

// ROOT
#include "TFile.h"
#include "TSystem.h"
#include "TObjArray.h"
#include "TString.h"
#include "TVirtualFitter.h"
#include "TPRegexp.h"
#include <time.h>
// VALOR/SDK
#include "valor/Conventions/Constants.h"
#include "valor/Conventions/Opt.h"
#include "valor/Conventions/ParameterPool.h"
#include "valor/CmdLine/CmdLnArgList.h"
#include "valor/Core/CorrelatedSystematics.h"
#include "valor/Core/PhysParamList.h"
#include "valor/Core/FitParamList.h"
#include "valor/Core/FitModelABC.h"
#include "valor/Core/FitDataABC.h"
#include "valor/Core/Likelihood.h"
#include "valor/Core/FitterABC.h"
#include "valor/Core/CompoundFitElements.h"
#include "valor/Fitters/UtlFit.h"
#include "valor/Core/UtlOscillation.h"
//#include "valor/Core/UtlFitParamList.h"
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

  time_t startrawtime;
  struct tm * starttimeinfo;
  time (&startrawtime);
  starttimeinfo = localtime (&startrawtime);
  std::cout << "-----------------------------------------------------------" << std::endl;
  std::cout << " Start local time and date:  " << asctime(starttimeinfo);
  std::cout << "-----------------------------------------------------------" << std::endl;

  valor::util::PrintBanner(Env::Instance()->Banner(),500);

  //  util::InitializeOAPriors();

  CmdLnArgList arg;
  arg.Read(argc,argv,"$VALOR_FMWK/config/apps/run_fit.arg:$VALOR_FMWK/config/apps/seeds.arg:$VALOR_FMWK/config/apps/oscparams.arg");
  VLOG("valor", pNOTICE) << arg;

  TString param_values_ts(arg.GetString("param-values-option"));
  TString fit_options_ts (arg.GetString("fitter"));

  bool with_stat_fluct    = arg.GetBool("with-stat-fluct");
  bool start_at_true      = arg.GetBool("start-at-true");
  bool rethrow_unphysical = arg.GetBool("rethrow-unphysical");
  int  ntoymc             = arg.GetInt("ntoymc");
  int n_threads           = arg.GetInt("nthreads");
  string hypname          = arg.GetString("physics-hypothesis");
  string fplopt           = arg.GetString("fit-param-opt");
  string fittername       = arg.GetString("fitter");
  string true_values_file = arg.GetString("true-from-filename");
  string true_values_obj  = arg.GetString("true-from-objname"); // a physparamlist
  bool flip_23_octant     = arg.GetBool("flip-true-octant");
  bool read_data           = arg.GetBool("read-data"); // if read-data-from-file then do not Generate the Mock Data

  string read_prior_from_file = arg.GetString("load-prior-from-file");
  int prior_fit_number = arg.GetInt("prior-fit-number");

  string tweak_parameter_set = arg.GetString("tweak-parameter-set");
  double force_tweak_size    = arg.GetDouble("force-tweak-size");

  // If pot_override is set to a positive number, every sample will be initialised with the same POT - this number
  double pot_override     = arg.GetDouble("pot-override");
  if (pot_override > 0 ) {
    Opt::Instance()->MutableGlobalRegistry().Set("pot_override",pot_override);
  }

  std::string mesg_thresholds = util::CompleteConfigPath(arg.GetString("mesg-thresholds"));
  valor::Messenger::Instance()->Configure(mesg_thresholds);

  // Set up the thread pool
  if(n_threads>0) {//without n-cores specified, use as many as the host has
    util::VThreadPool::Instance()->SetNThreads(n_threads);
  }

  // Set the random seeds
  Rnd::Instance()->SetSeeds(arg);

  VLOG("valor", pNOTICE) << *Rnd::Instance();

  // Check if we have a PParm to read from file containing true values
  PhysParamList * true_values_from_file = 0;
  if (true_values_file.size() && util::ToLowerCase(true_values_file)!="none") {
    TFile tvf(true_values_file.c_str());
    if (!tvf.IsOpen()) {
      VLOG("valor", pERROR)<<"Could not read true values file ["<<true_values_file<<"]";
      util::Die(-1);
    }
    true_values_from_file = dynamic_cast<PhysParamList*>(tvf.Get(true_values_obj.c_str()));
    if (!true_values_from_file) {
      VLOG("valor", pERROR)<<"Could not read true values object "<<true_values_obj<<" from file "<<true_values_file;
      util::Die(-1);
    }
  }
  
  // Create file and tree to store outputs
  TString outfilename = TString::Format("%s_%d.root", arg.GetString("tag").c_str(), arg.GetInt("run"));
  StdFitNtp ntp;
  ntp.Create(outfilename.Data(), fit_result_tree_name, "VALOR fit n-tuple");

  // Get ND model & configure
  VLOG("valor", pNOTICE) << "Getting and configuring the fit elements";
  CompoundFitElements * elements = new CompoundFitElements();
  std::string config_path = util::CompleteConfigPath(arg.GetString("config"));
  elements->Read(config_path);
  FitModelABC * model = elements->FitModel();
  const util::NameRegistry * nr = util::NameRegistry::Instance();

  // Hack until we move GetSystematics() into FitModelABC
  CorrelatedSystematics * syst = model->GetSystematics();

  // If necessary, update the systematics prior from a previous fit
  // TODO: This should be handled by the XML prior file, but priors trees are not yet supported
  if (read_prior_from_file!="-" && read_prior_from_file!="none") {
    TFile prior_file(read_prior_from_file.c_str());
    TTree * prior_tree = dynamic_cast<TTree*>(prior_file.Get(fit_result_tree_name));
    assert(prior_tree);
    FitParamList * prior_fpl = NULL;
    prior_tree->SetBranchAddress("best_fit_params",&prior_fpl);
    prior_tree->GetEntry(prior_fit_number);
    syst->UpdateCentralValuesAndCovarianceFromFPL(*prior_fpl);
    delete prior_tree;
    prior_file.Close();
  }

  // Use ND model to generate toy-MC experiments & fit
  for(int itoymc = 0; itoymc < ntoymc; itoymc++) {
    VLOG("valor", pNOTICE) << "\n\n *** Toy-MC experiment #" << itoymc;

    // Reset output tree variables
    ntp.Reset();

    // Get the specified fit parameter list (specifies which parameters are floated/fixed,
    // parameter limits, initial values etc).
    //FitParamList * fp = util::GetFitParameterList(hypname.c_str(), fplopt.c_str());
    FitParamList * fp = new FitParamList(hypname, fplopt.c_str());

    if(fp == 0){ // Make sure that worked
      VLOG("valor",pFATAL) << "FitParamList not filled";
      util::Die(-1);
    }

    model->ResetSystematics(*fp, "all");
    // If required, randomise the systematics in the model, and copy them to the FPL
    if (param_values_ts.Contains("randomize")) {
      model->RandomizeSystematics(*fp, param_values_ts.Data());
    }

    if (tweak_parameter_set != "none" && tweak_parameter_set != "-") {
      vector<PhysParamId_t> param_set = util::GetParameterSet(tweak_parameter_set);
      assert(param_set.size());
      // Force tweaks to specific value
      if (std::fabs(force_tweak_size)>conv::kAVerySmallNum) {
        for (unsigned int ip = 0 ; ip < param_set.size(); ip++) {
          PhysParamId_t p = param_set[ip];
          if (!syst->Contains(p)) {continue;}
          double central = syst->InitialValue(p);
          double onesig  = syst->OneSigmaError(p);
          double tweaked = central + force_tweak_size * onesig;
          syst->SetCurrentValue(p,tweaked);
        }
      }
      // or just randomise according to prior
      else {
        std::map<PhysParamId_t,bool> param_map;
        for (unsigned int ip = 0 ; ip < param_set.size() ; ip++) {
          PhysParamId_t p = param_set[ip];
          param_map[p] = true;
        }
        model->RandomizeSystematics(param_map, *fp);
      }
    }

    PhysParamList tweaks = model->Tweaks();

    //~ // Don't randomise fixed parameters
    //~ for (int ip = 0; ip < conv::knppmax ; ip++) {
    //~ PhysParamId_t p = (PhysParamId_t)ip;
    //~ if (tweaks.Contains(p) and fp->IsFixed(p) ) {
    //~ double init = fp->InitValue(p);
    //~ tweaks.SetValue(p,init);
    //~ }
    //~ }

    fp->FromPhysParamList(tweaks,"current");

    if(fp == 0){ // Make sure that worked
      VLOG("valor",pFATAL) << "FitParamList not filled after tweaks applied";
      util::Die(-1);
    }

    // Check parameter name, maybe rethrow
    //if (!util::AllWithinLimits(*fp)) {
    if (!fp->AllWithinLimits()) {
      VLOG("valor", pNOTICE) << "At least one parameter thrown outside expected limits.";
      if (rethrow_unphysical) {
        VLOG("valor", pNOTICE) << "Regenerating...";
        itoymc--;
        continue;
      }
    }

    // If we have a set of true values read from file, use those.
    if (true_values_from_file) {
      fp->FromPhysParamList(*true_values_from_file,"current");
    }

    //change if something different to default (i.e. normal, sinsqtheta, first octant and dmsq12abs23 (NH) / dmsq12abs31 (IH))
    Opt::Instance()->gMassHierarchyAssumed = ( (arg.GetBool("inverted"))     ? conv::kNmhInverted      : conv::kNmhNormal);
    Opt::Instance()->gOscMixingParamChoice = ( (arg.GetBool("sinsq2theta"))  ? conv::kNuMixSinsq2Theta : conv::kNuMixSinsqTheta);
    Opt::Instance()->gOscOctantChoice      = ( (arg.GetBool("second-octant"))? conv::kNuOctantTwo      : conv::kNuOctantOne);

    conv::NuOscDmsqScheme_t dmsq_scheme = util::DmsqSchemeFromString(arg.GetString("dmsq-scheme"));
    if(dmsq_scheme == kNuDmsqUndefined) {
      VLOG("valor", pFATAL) << "Could not parse the input Dmsq scheme: [" << arg.GetString("dmsq-scheme") << "]";
      util::Die(-1);
    }
    Opt::Instance()->gOscDmsqScheme = dmsq_scheme;

    util::oscillation::SetParameterPriors(arg, *fp);

    // Set input parameters
    RgKeyList input_args = arg.FindKeys("input-");
    TPRegexp input_match("input-(\\w*)");
    for (RgKey argname : input_args) { // RgKey is just a typename for std::string right now
      TObjArray * matches = input_match.MatchS(argname.c_str());

      if (matches->GetEntries() != 2) {
        VLOG("valor",pFATAL) << "Cannot parse argument "<<argname<<" to get parameter name";
        util::Die(-1);
      }
      TObjString * tos_param_name = dynamic_cast<TObjString*>(matches->At(1));
      std::string param_name(tos_param_name->String().Data());
      delete matches;

      conv::PhysParamId_t param = nr->PhysParamFromString(param_name);
      if (param == conv::kPhypUndefined) {
        VLOG("valor",pFATAL) << "Cannot find parameter to fix from argument string "<<argname;
        util::Die(-1);
      }

      double param_val = arg.GetDouble(argname);
      if ( std::fabs(param_val + 999. ) < conv::kASmallNum ) {
        param_val = Opt::Instance()->GetGlobalBestFit(param);
      }
      if (param==kPhypMixing23 && flip_23_octant) {
        param_val = util::FlipOctantAroundMaxDisap(param_val,arg.GetDouble("input-mixing13"));
      }
      if(fp->Exists(param)) {
        fp->Update      (param, param_val);
        fp->SetInitValue(param, param_val);
      }
    }

    // fix them? randomize them?
    RgKeyList fix_args = arg.FindKeys("fix-");
    TPRegexp fix_match("fix-(\\w*)");
    for (RgKey argname : fix_args) { // RgKey is just a typename for std::string right now
      TObjArray * matches = fix_match.MatchS(argname.c_str());

      if (matches->GetEntries() != 2) {
        VLOG("valor",pFATAL) << "Cannot parse argument "<<argname<<" to get parameter name";
        util::Die(-1);
      }
      TObjString * tos_param_name = dynamic_cast<TObjString*>(matches->At(1));
      std::string param_name(tos_param_name->String().Data());
      delete matches;

      if (param_name=="at") {continue;} // We have lots of arguments called fix-at instead of just fix-[param]

      conv::PhysParamId_t param = nr->PhysParamFromString(param_name);
      if (param == conv::kPhypUndefined) {
        VLOG("valor",pFATAL) << "Cannot find parameter to fix from argument string "<<argname;
        util::Die(-1);
      }

      if (fp->Exists(param)) {
        if(arg.GetBool(argname)){
          fp->FixParameter(param);
        }
        else if(param_values_ts.Contains("randomize-all")) {
          double rnd_value = util::oscillation::GetRandomizedParam(param,*fp);
          fp->Update      (param, rnd_value);
          fp->SetInitValue(param, rnd_value);
        }
      } //Exists(param)
      else {
        VLOG("valor",pINFO)<<"Not setting parameter not included in FPL: "<<param_name;
      }
    } //argname

    PhysParamList true_params = fp->AsPhysParamList("current");

    // Generate a toy-MC experiment
    FitDataABC * data = elements->FitData();
    if(!read_data){ // If we do not get told to read the data, Generate the mock data from the MC
      if(!elements->DataGiven())
        VLOG("valor", pNOTICE) << "No data given to Read, Generating the toy-MC instead";
      else
        VLOG("valor", pNOTICE) << "Data given but will be overwritten by Generate";

      VLOG("valor", pNOTICE) << "Generating toy-MC data";
      VLOG("valor", pNOTICE) << "Toy-MC generation uses true parameters: " << true_params;
      data->Generate(model,*fp,with_stat_fluct);
    }
    else{
      if(!elements->DataGiven()){
        VLOG("valor",pFATAL) << "Told to Read data but no data file was given ";
        util::Die(-1);
      }
      VLOG("valor", pNOTICE) << "Reading data from file, not Generating the toy-MC";
    }

    // The toy-MC was generated using the `current' values of the input parameter list.
    // These values will be overwritten during the fit.
    // Copy those values now so as to save them later in the output tree.
    ntp.TrueParamArr()->Append(true_params);

    if (start_at_true) {
      // Before we do our start-at-true, we'll reset our fixed parameters
      for (int ip = 0 ; ip < fp->GetNumberParameters() ; ip++) {
        if (fp->IsFixedByIndex(ip)) {
          fp->SetCurrentValueByIndex(ip,fp->InitValueByIndex(ip));
        }
      }
      VLOG("valor", pNOTICE) << "Copying current values to initial values to give the fitter some help.";
      fp->CopyCurrToInitValues();
    }


    //before reset for the fit, fix to a different value?
    RgKeyList fix_at_args = arg.FindKeys("fix-at-");
    TPRegexp fix_at_match("fix-at-(\\w*)");
    for (RgKey argname : fix_at_args) { // RgKey is just a typename for std::string right now
      TObjArray * matches = fix_at_match.MatchS(argname.c_str());

      if (matches->GetEntries() != 2) {
        VLOG("valor",pFATAL) << "Cannot parse argument "<<argname<<" to get parameter name";
        util::Die(-1);
      }
      TObjString * tos_param_name = dynamic_cast<TObjString*>(matches->At(1));
      std::string param_name(tos_param_name->String().Data());
      delete matches;

      conv::PhysParamId_t param = nr->PhysParamFromString(param_name);
      if (param == conv::kPhypUndefined) {
        VLOG("valor",pFATAL) << "Cannot find parameter to fix from argument string "<<argname;
        util::Die(-1);
      }

      if (fp->Exists(param) && fp->IsFixed(param)) {
        double fix_param_at = arg.GetDouble(argname);
        if ( std::fabs(fix_param_at + 999. ) < conv::kASmallNum ) {
          continue;
        }
        if (!util::IsSystematic(param)) {
          if(!ParameterPool::Instance()->IsParamInPool(param)) {
            VLOG("UtlOscillation", pFATAL) << "The osc param \'" << util::NameRegistry::Instance()->ParamAsString(param)
              << "\' was in the FPL but not the ParameterPool";
            util::Die(-1);
          }
          const Parameter & parameter = ParameterPool::Instance()->GetParam(param);
          double min = parameter.GetLimits().first;
          double max = parameter.GetLimits().second;
          if((fix_param_at < min) ||
              (fix_param_at > max)) {
            VLOG("valor",pFATAL) << "Attempting to fix parameter "
              <<nr->ParamAsString(param)<<" outside limits at "<<fix_param_at
              <<" - limits are "<<min<< " to "<<max;
            util::Die(-1);
          }
        } // is osc param
        fp->Update(param, fix_param_at);
        fp->SetInitValue(param, fix_param_at);
      } // param exists in FPL
    } // argname

    // Set our current values back to the fitter start point.
    // This will be central values unless start-at-true is defined.
    fp->ResetCurrToInitValues();

    // Get fitter
    VLOG("valor", pNOTICE) << "Preparing the fitter";
    Likelihood * likelihood = new Likelihood(data,model,fp);
    FitterABC * fitter = util::GetFitter(likelihood, fittername.c_str());

    // Do the fit
    VLOG("valor", pNOTICE) << "Running the fitter";
    VLOG("valor", pNOTICE) << "Fit starts with the following fit parameter list: " << *fp;
    VLOG("valor", pNOTICE) << "Fit starts with statistic: " << likelihood->Statistic();

    fitter->DoTheFit();
    VLOG("valor", pNOTICE) << "Fit ends with statistic: " << likelihood->Statistic();

    //  migradmin = fitter->FitStatusCode("migrad_min");
    //  migraderr = fitter->FitStatusCode("migrad_err");

    // Copy best-fit params
    ntp.BestFitParamArr()->Copy(*fp);

    if(arg.GetBool("store-spectra")) {
      // Copy best-fit to current values and re-generate the best-fit prediction
      // with special option (not used in the fit) to generate versions of the fit model
      // using a fine binning scheme (for plots).
      // Then, write-out the best-fit predictions
      fp->CopyBestFitToCurrValues();
      model->Generate(*fp,"+SB");
      TObjArray * best_fit_model_array = ntp.BestFitModelArr();
      model->Write(best_fit_model_array,"best_fit","pdfc_rf,pdft_rf,pdfts_rf,pdfcs_rf +SB");

      // Write-out the toy-MC
      TObjArray * data_array = ntp.DataArr();
      data->Write(data_array,"toymc","all");
    }

    // Write results to the output tree
    ntp.Fill();

    delete fp;
    delete likelihood;
    delete fitter;

  }//itoymc

  ntp.Save();

  //~ // Write a PPL with the prior uncertainties used
  //~ SDModel * ndm =  dynamic_cast<SDModel*>(model);
  //~ PhysParamList ppl = ndm->OneSigmaErrors();
  //~ ppl.Write("initial_errors");

  std::cout<<"Done."<< std::endl << " GO VALOR!!!" << std::endl;

  time_t endrawtime;
  struct tm * endtimeinfo;
  time (&endrawtime);
  endtimeinfo = localtime (&endrawtime);
  time_t total_time = endrawtime-startrawtime;
  std::cout << "-----------------------------------------------------------" << std::endl;
  std::cout << " End local time and date  :  " << asctime(endtimeinfo);
  std::cout << " Total time taken         :  " << total_time/static_cast<double>(60) << " minutes" << std::endl;
  std::cout << "-----------------------------------------------------------" << std::endl;

  return 0;
}

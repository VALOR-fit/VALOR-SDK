//
// ------------------------------------------------------------------------------------------------
// Generic macro for toy-MC studies.
// Runs oscillation fits to toy MC experiments generated on-the-fly. 
// Store the toy MC experiment, the best-fit parameter values, the best-fit spectra and the true 
// input parameter values.
//
// Costas Andreopoulos
// ------------------------------------------------------------------------------------------------
//
	
void run_osc_fit
(
  const char * config_file             = "lbne_34kt_def.xml",                 /* analysis configuration file */
  const char * tag                     = "toyexptfits",                       /* tag for output file */
  int          run                     = 0,                                   /* run number */
  int          nexperiments            = 1,                                   /* number of toy MC experiment fits */
  bool         stat_fluctuations       = true,                                /* include statistical fluctuation in toy MC expts? */
  int          tweak_systematics       = 1,                                   /* generate toy MC with randomized systematics? (0: none, 1: all)*/
  const char * fit_param_list          = "nuosc3f",                           /* fit parameter list for the given physics hypothesis (see FitParamList.h) */
  const char * fit_option              = "minuit",                            /* fitter option */
  double       input_mixing12          = 0.306,                               /* 12-mixing, sinq2theta or sinsqtheta depending on user choice, if -999 then randomize */
  double       input_mixing13          = 0.0251,                              /* 13-mixing, sinq2theta or sinsqtheta depending on user choice, if -999 then randomize */
  double       input_mixing23          = 0.50,                                /* 23-mixing, sinq2theta or sinsqtheta depending on user choice */
  double       input_cpv               = 0.00,                                /* cpv param (deltacp or sin(deltacp)), if -999 then randomize */
  double       input_dm12sq            = 7.58E-5,                             /* Dm^2_{12}, if -999 then randomize */
  double       input_dm23sq            = 2.32E-3,                             /* |Dm^2_{23}| */
  const char * hierarchy               = "normal",                            /* normal or inverted */
  const char * mixparamchoice          = "sinsqtheta",                        /* sinsq2theta or sinsqtheta */
  const char * mixparamoctant          = "one",                               /* one or two */
  const char * dmsqscheme              = "abs23",                             /* abs23==kNuDmsq12andAbs23 abs31==kNuDmsq12andAbs31 fogli==kNuDmsqFogliLisi */
  const char * cpvchoice               = "delta",                             /* delta or sindelta */
  bool         store_toy_expts         = true,                                /* disable this if you don't want to store the toy MC expts     */
  bool         store_bestfit_spectrum  = true                                 /* disable this if you don't want to store the best-fit spectra */
)

{
  using namespace std;
  using namespace t2kana;
  using namespace t2kana::conv;

//Messenger * msg = Messenger::Instance();

  // Analysis configuration
//TString config = Form("%s/config/%s", gSystem->Getenv("VALOR_LBNX"), config_file);

  // Global options
  Opt * opt = Opt::Instance();
  opt->gMassHierarchyAssumed = util::HierarchyFromString(hierarchy);
  opt->gOscMixingParamChoice = util::MixingTypeFromString(mixparamchoice);
  opt->gOscOctantChoice      = util::OctantFromString(mixparamoctant);
  opt->gOscDmsqScheme        = util::DmsqSchemeFromString(dmsqscheme);
  opt->gOscDeltaCP           = util::CPVChoiceFromString(cpvchoice);
  if(opt->Undefined()) {
    cerr << "Global options not defined correctly. Exiting..." << endl;
    cerr << *opt << endl;
    exit(1);
  }

  // Check whether some osc. params need to be randomized
  bool randomize_mixing12 = (TMath::Abs(input_mixing12 + 999) < kASmallNum);
  bool randomize_mixing13 = (TMath::Abs(input_mixing13 + 999) < kASmallNum);
  bool randomize_cpv      = (TMath::Abs(input_cpv      + 999) < kASmallNum);
  bool randomize_dm12sq   = (TMath::Abs(input_dm12sq   + 999) < kASmallNum);

  // Print settings
  cout << "\n[Job inputs]" << endl;
  cout << "- Job: " << tag << "/" << run << endl;
//cout << "- Reading analysis configuration from: " << config << endl;
  cout << "- Toy-MC generation" << endl;
  cout << "   - Number of toy experiments: " << nexperiments << endl; 
  cout << "   - Randomize systematics : "      << ((tweak_systematics) ?      "YES " : "NO") << endl;
  cout << "   - Statistical fluctuations: "    << ((stat_fluctuations) ?      "YES " : "NO") << endl;
  cout << "   - True oscillation parameters: " << endl;
  cout << "       > " << util::AsString(kPhypMixing12) << " = " << ((randomize_mixing12) ? "* randomized *" : input_mixing12) << endl;
  cout << "       > " << util::AsString(kPhypMixing13) << " = " << ((randomize_mixing13) ? "* randomized *" : input_mixing13) << endl;
  cout << "       > " << util::AsString(kPhypMixing23) << " = " << input_mixing23 << endl;
  cout << "       > " << util::AsString(kPhypCPV)      << " = " << ((randomize_cpv)     ? "* randomized *"  : input_cpv) << endl;
  cout << "       > " << util::AsString(kPhypDm12sq)   << " = " << ((randomize_dm12sq)  ? "* randomized *"  : input_dm12sq ) << " eV^2/c^4" << endl;
  cout << "       > " << util::AsString(kPhypDm23sq)   << " = " << input_dm23sq << " eV^2/c^4" << endl;
  cout << "- Fit" << endl;
  cout << "   - Fit Parameter List: " << fit_param_list << endl;
  cout << "   - Fitter option: " << fit_option << endl;
  cout << "- Outputs" << endl;
  cout << "     - Storing toy MC experiments:  " << ((store_toy_expts) ?        "YES " : "NO") << endl;
  cout << "     - Storing best-fit spectra: "    << ((store_bestfit_spectrum) ? "YES " : "NO") << endl;
  cout << endl;
  cout << *Opt::Instance() << endl;
  cout << *Rnd::Instance() << endl;

  // Fit parameter list
  FitParamList * fparm  = FitParamList::NamedParameterList(fit_param_list); 
  PhysHypothesis_t hyp = fparm->PhysicsHypothesis();
  cout << "- The following physics hypothesis is used in the fit: " << util::AsString(hyp) << endl;

  FitParamList * gfparm = new FitParamList; // placeholder for global min

  // Get fit model & configure
  FitModelABC * model = new X01Model;
  //model->Read(config.Data());

  // Object for storing input oscillation and nuisance params in each toy-MC
  PhysParamList * iparm = new PhysParamList;

  // Arrays for storing all pieces of fit data and the best-fit model (and its components) for each toy-MC.
  TObjArray * spectra_array = new TObjArray; // array to keep input data and best-fit spectra
  spectra_array->SetOwner(false);
  TObjArray * bfp_array = new TObjArray; // array to keep best-fit parameters from (potentially) multiple fits to input data
  bfp_array->SetOwner(false);

  // Create file to store results
  string oscstr = util::AsString(hyp,input_mixing12,input_mixing13,input_mixing23,input_cpv,input_dm12sq,input_dm23sq);
  TFile f(Form("%s-run_%d-%s.root", tag, run, oscstr.c_str()), "recreate");

  // Create tree to store outputs
  TTree * fitnt = new TTree("fitnt","Results of VALOR data and/or toy-MC experiment fits");
  fitnt->Branch("input_params",           "t2kana::PhysParamList", &iparm,         1280000,0);
  fitnt->Branch("global_best_fit_params", "t2kana::FitParamList",  &gfparm,        1280000,0);
  fitnt->Branch("best_fit_params",        "TObjArray",             &bfp_array,     1280000,0);
  fitnt->Branch("spectra",                "TObjArray",             &spectra_array, 1280000,0);

  // Loop over toy-MC experiments
  for(int iexperiment = 0; iexperiment < nexperiments; iexperiment++) {

     cout << "\n\n *** Pseudo-experiment = " << iexperiment << endl;

     // randomize specified osc params for current toy MC expt
     if (randomize_mixing12) { input_mixing12 = util::RandomizeMixing12(); }
     if (randomize_mixing13) { input_mixing13 = util::RandomizeMixing13(); }
     if (randomize_cpv     ) { input_cpv      = util::RandomizeCPV();      }
     if (randomize_dm12sq  ) { input_dm12sq   = util::RandomizeDm12sq();   }

     // set oscillation parameter inputs as necessary (used to generate toy-MC expt)
     if(hyp == kHypOsc3F) {
       fparm->Update(kPhypMixing12, input_mixing12);
       fparm->Update(kPhypMixing13, input_mixing13);
       fparm->Update(kPhypMixing23, input_mixing23);
       fparm->Update(kPhypCPV,      input_cpv);
       fparm->Update(kPhypDm12sq,   input_dm12sq);
       fparm->Update(kPhypDm23sq,   input_dm23sq);
     }
//     else
//     if(hyp == kHypNuMuDisapp2F) {
//       fparm->Update(kPhypMixing23, input_mixing23);
//       fparm->Update(kPhypDm23sq,   input_dm23sq);
//     }

     // Reset all output spectra and best-fit params array
     spectra_array->Clear();    
     bfp_array->Clear()

     // Reset input true params and add true oscillation params
     iparm->Clear();
     PhysParamList true_oscillation_params = fparm->AsPhysParamList("current");
     iparm->Append(true_oscillation_params);

     //
     // Get a toy MC with/without systematics
     //
     FitDataABC * data = new X01Data;
     bool ok = false;
     while(!ok) {
       if(tweak_systematics) {
	 // tweak all sources of systematic error
	 if(tweak_systematics == 1) {
	   cout << "- Randomizing all systematic params in model..." << endl;
	   model->RandomizeSystematics(); 
	 }
	 model->PrintSystematics();
	 // generate mock data using the current model (incl. systematic variations) 
	 // + statistical fluctuations if requested (Poisson fluctuation in each bin)
	 cout << "- Generating toy-MC..." << endl;
	 //note that fparm does not contain the tweaks, so use only the PhysHyp here
	 ok = data->Generate(model, PhysHypothesis(*fparm), stat_fluctuations);
	 if(ok) {
	   // keep track of all nuisance params set so as to add in output tree
	   PhysParamList true_nuisance_params = model->Tweaks();
	   iparm->Append(true_nuisance_params);
	 }
	 // reset all systematics so that the model used in the fitter is the nominal one
	 cout << "- Reseting all systematic params in model..." << endl;
	 model->ResetSystematics();
	 model->PrintSystematics();
       }
       else {
	 //make sure systematics are reset
	 model->ResetSystematics();
	 // generate mock data using the current model (without systematic variations) 
	 // with statistical fluctuations if requested (Poisson fluctuation in each bin)
	 cout << "- Generating toy-MC..." << endl;
	 ok = data->Generate(model, PhysHypothesis(*fparm), stat_fluctuations); 
       }
     }//ok


     // Get fitter
     FitInputs * fit_inputs = new FitInputs(data,model,fparm);
     FitterABC * fitter = util::GetFitter(fit_inputs,fit_option);

     // Likelihood ratio for true (input) params
     double truelr = data->Statistic(model,"likelihood-ratio");
     fparm->ShowOnlyCurrValuesInNextPrint();
     cout << "- True oscillation params (below) yield a true likelihood ratio = " << truelr << *fparm;

     //
     // Do the fit
     //

     cout << "- Running the fitter" << endl;     

     fparm->ResetCurrToInitValues(); // reset osc params
     fitter->DoTheFit();
     int fitstatus = fitter->FitStatusCode();

     fparm->CopyBestFitToCurrValues();

     double bestfitlr = fparm->GetBestFitStatistic();
     // Save spectra (data and best-fit model)
     FitParamList * fparm1 = new FitParamList(*fparm);
     fparm1->SetName("best_fit_params");
     bfp_array->Add(fparm1);
     if(store_toy_expts) {
       data->Write(spectra_array, "data");
     }
     if(store_bestfit_spectrum) {
       model->Write(spectra_array, "best_fit_model");
     }

     // Set as global best fit
     gfparm->Copy(*fparm);

     //
     // Do the fit again at the mirror theta23 point
     //
     if(opt->gOscMixingParamChoice == conv::kNuMixSinsqTheta) {
       cout << "- Running the fitter again with a seed in mirror theta23 point" << endl;     
       //get the correct intial values (best fit points from first fit)
       FitParamList * fparm2 = FitParamList::NamedParameterList(fit_param_list);
       fparm2->CopyBestFitToInitValues(*fparm);
       //set the mixing23 seed to be the mirror point of the initial best fit
       double sinsqtheta23_bestfit = fparm->BestFitValue(conv::kPhypMixing23);
       double sinsqtheta13_bestfit = fparm->BestFitValue(conv::kPhypMixing13);
       double centre = 1 / (2 * (1 - sinsqtheta13_bestfit));
       double reseed = sinsqtheta23_bestfit;
       if (reseed < centre) {
	 reseed = centre + TMath::Abs(centre - sinsqtheta23_bestfit);
       } else 
       if(reseed > centre)
       {
	 reseed = centre - TMath::Abs(centre - sinsqtheta23_bestfit);
       }
       fparm2->SetInitValue(conv::kPhypMixing23, reseed);
       // Get fitter
       FitInputs * fit_inputs2 = new FitInputs(data,model,fparm2);
       FitterABC * fitter2 = util::GetFitter(fit_inputs2,fit_option);
       fparm2->ResetCurrToInitValues(); // reset osc params
       fitter2->DoTheFit();
       // Replace global best fit params?
       double bestfitlt2 = fparm2->GetBestFitStatistic();
       if(bestfitlr < bestfitlr)
       {
         gfparm->Copy(*fparm2);
       }
       // Save best fit params
       fparm2->SetName("best_fit_params_mirror_octant");
       bfp_array->Add(fparm2);
       // Save spectra (best-fit model)
       if(store_bestfit_spectrum) {
	 model->Write(spectra_array, "best_fit_model_mirror_octant");
       }
       delete fit_inputs2;
       delete fitter2;
     }

     // Name global best fit params
     gfparm->SetName("global_best_fit_params");
     cout << "Global best-fit params: " << gfparm;
    
     // Add all results in the tree
     fitnt->Fill();

     // Clean-up
     delete data;
     delete fit_inputs;
     delete fitter;
  }

  // Extract Opt state to a lightweight persistable object
  Opt::Persistable optp; 
  opt->WriteTo(optp);

  // Store ntuple with toy-MC expts & best-fit results
  f.cd();
  optp.Write("options");
  fitnt->Write();
  f.Close();
}

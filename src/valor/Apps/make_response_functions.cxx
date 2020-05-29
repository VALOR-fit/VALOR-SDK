//____________________________________________________________________________
/*!

  \app       valor-make_response_functions

  \brief

  \syntax    valor-make_response_functions
               --input  input_analysis_file.root
               --output output_response_function_file.root
               --format format_of_the_input_file <hpgartpc, lartpc, fgt>
               --beam   beam_name
               --sample sample_name
               --systematic genie_systematic_name

  \author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
             University of Liverpool & STFC Rutherford Appleton Laboratory

  \created   April 20, 2016

  \copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

// WARNING - this probably still assumes that binning is 2D even though it's moved to the ND interface

// System
#include <string>
// ROOT
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TSpline.h>
#include <TString.h>
#include <TSystem.h>
#include <TMath.h>
// VALOR/SDK
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/Env.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/VArray/VALORArray.h"
#include "valor/CmdLine/CmdLnArgList.h"
#include "valor/Conventions/BeamConf.h"
#include "valor/Conventions/Sample.h"
#include "valor/Conventions/Detector.h"
#include "valor/Conventions/ParameterPool.h"
#include "valor/Conventions/Parameter.h"
#include "valor/Conventions/ModeList.h"
#include "valor/KineBinning/VALORBinning.h"
#include "valor/Conventions/Parameter.h"
#include "valor/Conventions/ReacMode.h"
#include "valor/Conventions/ParameterPool.h"
#include "valor/GENIEUtilities/GENIEIncludes.h"
#include "valor/GENIEUtilities/UtlEventRec.h"
#include "valor/Utilities/SingleSampleKineBinning.h"
#include "valor/Utilities/RFL.h"
#include "valor/Utilities/KinematicBin.h"
#include "valor/FitCore/CompoundFitElements.h"
#include "valor/FitElements/GenericSingleSampleFitModel.h"
#include "valor/FitCore/UtlFitParamList.h"

using std::string;
using genie::EventRecord;
using namespace valor;
using namespace valor::conv;

//___________________________________________________________________
int main(int argc, char ** argv)
{
  // Make GENIE quiet - intranuke spams with default messenger
  TString gmesg_path = "$GENIE/config/Messenger_laconic.xml";
  gSystem->ExpandPathName(gmesg_path);
  genie::utils::app_init::MesgThresholds(gmesg_path.Data());

  // Read command line arguments
  CmdLnArgList arg;
  arg.Read(argc,argv,"$VALOR_FMWK/config/apps/make_response_functions.arg");
  VLOG("valor", pNOTICE) << arg;

  string arg_beam        = arg.GetString("beam");
  string arg_sample      = arg.GetString("sample");
  string arg_detector    = arg.GetString("detector");
  string arg_systematic  = arg.GetString("systematic");
  string arg_format      = arg.GetString("format");
  string arg_inp_file    = arg.GetString("input");
  string arg_out_file    = arg.GetString("output");
  string arg_config_file = arg.GetString("config");
  string modelist_file   = arg.GetString("modes");
  double max_ereco       = arg.GetDouble("max-ereco");
  double max_yreco       = arg.GetDouble("max-yreco");

  bool include_sb       = arg.GetBool("include-simple-binning");
  int  max_events       = arg.GetInt("max-events");

  BeamConf_t beam      = util::BeamConfFromString (arg_beam      );
  Sample_t   sample    = util::SampleFromString   (arg_sample    );
  Detector_t detector  = util::DetectorFromString (arg_detector  );
  GSyst_t    syst      = GSyst::FromString        (arg_systematic);

  // Read the config to get the relevant binning and the parameters
  CompoundFitElements * elements = new CompoundFitElements();
  std::string config_path = util::CompleteConfigPath(arg.GetString("config"));
  elements->Read(config_path);

  Parameter p = ParameterPool::Instance()->ParamFromString(arg_systematic);
  PhysParamId_t fit_param = p.GetID();
  // TODO: Now that FitParam has been deleted, all of these PhysParamId_t enumerations are gone.
  // TODO: Here I've assumed that arg_systematic can be used with both GSyst::FromString and
  // TODO: ParameterPool::ParamFromString (I don't have a GENIE installation to test this).
  // TODO: If this is not the case, then hard-code special cases below:
//  PhysParamId_t fit_param = kPhypUndefined;
//  if      (syst == kINukeTwkDial_MFP_pi     ) fit_param = kNuisIntPiMeanFreePath;
//  else if (syst == kINukeTwkDial_MFP_N      ) fit_param = kNuisIntNucMeanFreePath;
//  else if (syst == kINukeTwkDial_FrCEx_pi   ) fit_param = kNuisIntFracPiChargeExchange;
//  else if (syst == kINukeTwkDial_FrInel_pi  ) fit_param = kNuisIntFracPiInelastic;
//  else if (syst == kINukeTwkDial_FrAbs_pi   ) fit_param = kNuisIntFracPiAbsorb;
//  else if (syst == kINukeTwkDial_FrPiProd_pi) fit_param = kNuisIntFracPiProduction;
//  else if (syst == kINukeTwkDial_FrCEx_N    ) fit_param = kNuisIntFracNucChargeExchange;
//  else if (syst == kINukeTwkDial_FrInel_N   ) fit_param = kNuisIntFracNucInelastic;
//  else if (syst == kINukeTwkDial_FrAbs_N    ) fit_param = kNuisIntFracNucAbsorb;
//  else if (syst == kINukeTwkDial_FrPiProd_N ) fit_param = kNuisIntFracNucProduction;

  if (beam==kbUndefined || sample == ksUndefined || fit_param == kPhypUndefined) {
    std::cerr<<"Bad configuration."<<std::endl;
    std::cerr<<"Beam:       "<<arg_beam       <<"\t => "<< util::AsString(beam)    <<std::endl;
    std::cerr<<"Sample:     "<<arg_sample     <<"\t => "<< util::AsString(sample)  <<std::endl;
    std::cerr<<"Detector:   "<<arg_detector   <<"\t => "<< util::AsString(detector)<<std::endl;
    std::cerr<<"GENIE Syst: "<<arg_systematic <<"\t => "<<GSyst::AsString(syst)    <<std::endl;
    std::cerr<<" to VALOR   "<< "\t => "<<util::NameRegistry::Instance()->AsString(fit_param)<<std::endl;
  }
  assert(beam      != kbUndefined   );
  assert(sample    != ksUndefined   );
  assert(detector  != kdUndefined   );
  assert(fit_param != kPhypUndefined);

  CompoundFitModel * compound_model = dynamic_cast<CompoundFitModel*>(elements->FitModel());
  assert(compound_model);
  GenericSingleSampleFitModel * fit_model = 0;
  bool got_appropriate_model = false;
  for (unsigned int ism = 0 ; ism < compound_model->GetNSubModels() ; ism++) {
    fit_model = dynamic_cast<GenericSingleSampleFitModel*>(compound_model->GetSubModel(ism));
    if  (fit_model->GetDetector() == detector &&
         fit_model->GetSample()   == sample   &&
         fit_model->GetBeam()     == beam )
      { got_appropriate_model = true; break; }
  }
  if (!got_appropriate_model) {
    VLOG("make_response_func", pFATAL) << "Failed to identify appropriate fit model to get binning.";
  }

  const util::RFL & rfl                   = *(fit_model->GetRFL());
  const SingleSampleKineBinning & binning = *(fit_model->GetBinning());

  // Make it an option here to use the Xsec binning instead
  const VALORBinning * reco_ob = (binning.RecoOptBinning());
  const VALORBinning * reco_sb = (binning.RecoSmpBinning());
  const int n_reco_bins_ob  = reco_ob->NBins();
  const int n_reco_bins_sb  = reco_sb->NBins();


  // Number of tweaks for the specified systematic parameter
  const int   inom          = 5;
  const int   n_twks        = 11;
  const float twk_dial_min  = -5.0;
  const float twk_dial_max  =  5.0;
  const float twk_dial_step = (twk_dial_max - twk_dial_min) / (n_twks-1);

  ModeList ml = valor::ModeList(util::CompleteConfigPath(modelist_file));
  const std::vector<conv::ReacMode_t> & read_modes = ml.Modes();

  // Arrays to store the number of events in each MC template bin
  // for each systematic parameter tweak
  VALORArray * mc_templ_sb [n_twks] [read_modes.size()];
  VALORArray * mc_templ_ob [n_twks] [read_modes.size()];

  for(int itwk = 0; itwk < n_twks; itwk++) {
    unsigned int imode = 0;
    for(ReacMode_t m : read_modes) {
      const VALORBinning * true_sb = binning.TrueSmpBinning(m);
      const int n_true_bins_sb  = true_sb->NBins();
      if (include_sb) {
        mc_templ_sb [itwk][imode] =
          new VALORArray(n_reco_bins_sb, n_true_bins_sb);
        mc_templ_sb [itwk][imode] -> SetAll(0.);
      }
      mc_templ_ob [itwk][imode] =
        new VALORArray(n_reco_bins_ob, n_true_bins_sb);
      mc_templ_ob [itwk][imode] -> SetAll(0.);
      imode++;
    }
  }

  // Open input file and get the HPGArTPC, LArTPC or FGT event tree
  TFile fin(arg_inp_file.c_str(),"READ");
  if (!fin.IsOpen()) {
    VLOG("make_response_func", pFATAL)
      << "Failed to open file "<<arg_inp_file.c_str();
    exit(1);
  }
  string tree_name = "";
  if(arg_format == "hpgartpc") {
    //~ tree_name = "GasTPCTree";
    tree_name = "NdtfDst";
  }
  else
    if(arg_format == "lartpc" || arg_format == "fgt") {
      tree_name = "dune_dst";
    }
    else {
      VLOG("make_response_func", pFATAL)
        << "Unknown input file format. Please specify one of hpgartpc, lartpc or fgt";
      exit(1);
    }
  TTree * analysis_tree = dynamic_cast <TTree *> (fin.Get(tree_name.c_str()));
  if(!analysis_tree) {
    VLOG("make_response_func", pFATAL)
      << "No event tree " << tree_name
      << " in " << arg_format << "-format input file: "
      << arg.GetString("input");
    exit(1);
  }
  NtpMCEventRecord * curr_mcrec = 0;
  Sample_t curr_sample          = ksUndefined;
  int      curr_sample_int      = -1;
  double   curr_enu_reco        =  -999;
  double   curr_y_reco          =  -999;
  float    curr_enu_reco_float  =  -999;
  float    curr_y_reco_float    =  -999;
  if(arg_format == "hpgartpc") {
    analysis_tree->SetBranchAddress("gmcrec",          &curr_mcrec);
    analysis_tree->SetBranchAddress("Sample",          &curr_sample_int);
    analysis_tree->SetBranchAddress("Ev_reco",         &curr_enu_reco);
    analysis_tree->SetBranchAddress("Y",          &curr_y_reco);
  }
  else if(arg_format == "lartpc" || arg_format == "fgt") {
    analysis_tree->SetBranchAddress("genie_mc_truth",  &curr_mcrec);
    analysis_tree->SetBranchAddress("sample_reco",     &curr_sample_int);
    analysis_tree->SetBranchAddress("Ev_reco",         &curr_enu_reco_float);
    analysis_tree->SetBranchAddress("y_reco",          &curr_y_reco_float);
  }
  else {
    VLOG("make_response_func",pFATAL)
      << "No recognised tree format.";
    util::Die(-1); return(-1);
  }

  // GENIE event reweighting interface
  GReWeight rw;
  rw.AdoptWghtCalc( "hadro_intranuke", new GReWeightINuke );
  GSystSet & syst_list = rw.Systematics();

  // I think we need to add all fates except the ones we want to act as cushion terms
  // So I add all mfp terms and all fates (except elastic) with nominal value
  // Only the input one will be tweaked later on.
  //syst_list.Init(syst);
  syst_list.Init(kINukeTwkDial_MFP_pi,      0., twk_dial_min, twk_dial_max);
  syst_list.Init(kINukeTwkDial_MFP_N,       0., twk_dial_min, twk_dial_max);
  syst_list.Init(kINukeTwkDial_FrCEx_pi,    0., twk_dial_min, twk_dial_max);
  syst_list.Init(kINukeTwkDial_FrInel_pi,   0., twk_dial_min, twk_dial_max);
  syst_list.Init(kINukeTwkDial_FrAbs_pi,    0., twk_dial_min, twk_dial_max);
  syst_list.Init(kINukeTwkDial_FrPiProd_pi, 0., twk_dial_min, twk_dial_max);
  syst_list.Init(kINukeTwkDial_FrCEx_N,     0., twk_dial_min, twk_dial_max);
  syst_list.Init(kINukeTwkDial_FrInel_N,    0., twk_dial_min, twk_dial_max);
  syst_list.Init(kINukeTwkDial_FrAbs_N,     0., twk_dial_min, twk_dial_max);
  syst_list.Init(kINukeTwkDial_FrPiProd_N,  0., twk_dial_min, twk_dial_max);

  // Systematic tweaks
  for(int itwk = 0; itwk < n_twks; itwk++) {

    double twk_dial = twk_dial_min + itwk * twk_dial_step;
    VLOG("make_response_func", pNOTICE) << "Doing tweak: " << twk_dial << " ("<<itwk+1 << " of "<<n_twks<<")";
    syst_list.Set(syst, twk_dial);
    rw.Reconfigure();

    // Event loop
    Long64_t nev = analysis_tree->GetEntries();
    Long64_t events_to_read = (max_events > 0 ? max_events : nev);
    for(Long64_t iev = 0; iev < events_to_read; iev++) {
      if (iev%1000==0) {
        VLOG("make_response_func", pNOTICE) << "** Reading event: " << iev;
      }
      analysis_tree->GetEntry(iev);

      // Consider only input sample
      curr_sample = util::SampleFromInt(curr_sample_int);
      if(curr_sample != sample) {
        curr_mcrec->Clear();
        continue;
      }

      // Retrieve GHEP event record and print it out.
      NtpMCRecHeader rec_header = curr_mcrec->hdr;
      EventRecord &  event = *(curr_mcrec->event);
      VLOG("make_response_func", pINFO)
        << "GENIE MC truth info: " << event;

      // hpgartpc output in MeV, convert to GeV as in lartpc and fgt outputs
      if(arg_format == "hpgartpc") {
        curr_enu_reco = curr_enu_reco/1000.;
      } // and the other two detectors are floats, not doubles
      else if (arg_format == "lartpc" || arg_format == "fgt") {
        curr_enu_reco = curr_enu_reco_float;
        curr_y_reco = curr_y_reco_float;
      }

      if(curr_enu_reco <= 0) {
        VLOG("make_minidst", pNOTICE) << "- Rejecting event [Ev (reco) <= 0]";
        curr_mcrec->Clear();
        continue;
      }
      if(curr_y_reco < 0) {
        VLOG("make_minidst", pNOTICE) << "- Rejecting event [y (reco) < 0]";
        curr_mcrec->Clear();
        continue;
      }

      // Get reaction mode
      ReacMode_t curr_mode = valor::util::DetermineReactionMode(&event, false);
      if(curr_mode == kmUndefined) {
        VLOG("make_minidst", pNOTICE) << "- Rejecting event [Undefined mode]";
        curr_mcrec->Clear();
        continue;
      }
      unsigned int imode = 0;
      for(ReacMode_t m : read_modes) {
        if(m == curr_mode) break;
        imode++;
      }

      // Get required true kinematical variables
      double curr_enu_true = event.Particle(0)->E();
      double curr_q2_true  = event.Summary()->KinePtr()->Q2(true);

      // Calculate event weight
      double wght = rw.CalcWeight(event);
      //if(wght <    0) wght = 1; // needed?
      //if(wght > 9999) wght = 1; // needed?

      // Protect against vars at upper limit (eg y_reco = 1, set by hand for NC events) as
      // the binary search below will give an index out of limits
      const double epsilon = 1E-10;
      if(curr_enu_reco >= max_ereco) { curr_enu_reco = curr_enu_reco - epsilon; }
      if(curr_y_reco   >= max_yreco) { curr_y_reco   = curr_y_reco   - epsilon; }

      // MUST BE REPLACED WITH APPROPRIATE KINESPACE STUFF
      std::vector<double> truevec;
      truevec.push_back(curr_enu_true);
      truevec.push_back(curr_q2_true );

      std::vector<double> recovec;
      recovec.push_back(curr_enu_reco);
      recovec.push_back(curr_y_reco);

      const VALORBinning * true_sb = binning.TrueSmpBinning(curr_mode);
      const int itrue_sb = true_sb->FindBin(truevec);
      const int ireco_ob = reco_ob->FindBin(recovec);
      const int ireco_sb = reco_sb->FindBin(recovec);

      if (include_sb) {
        mc_templ_sb [itwk] [imode]      -> AddToEntry(wght, ireco_sb, itrue_sb);
      }
      mc_templ_ob [itwk] [imode] -> AddToEntry(wght, ireco_ob, itrue_sb);
      curr_mcrec->Clear();
    }// event loop
  }//tweaks

  VLOG("make_response_func", pNOTICE) << "Closing input file";

  fin.Close();

  // Output file - first make the substitutions for sample/parameter
  TString ts_outfile = arg_out_file.c_str();
  ts_outfile = ts_outfile.ReplaceAll("[BEAM]"   , util::                          AsString(beam     ).c_str());
  ts_outfile = ts_outfile.ReplaceAll("[SAMPLE]" , util::                          AsString(sample   ).c_str());
  ts_outfile = ts_outfile.ReplaceAll("[PARAM]"  , util::NameRegistry::Instance()->AsString(fit_param).c_str());
  VLOG("make_response_func", pNOTICE)
    << "Saving VALOR response functions to: "<<ts_outfile;
  TFile fout(ts_outfile.Data(),"recreate");

  int n_splines_written = 0;

  unsigned int imode = 0;
  for(ReacMode_t m : read_modes) {
    const VALORBinning * true_sb = binning.TrueSmpBinning(m);
    const int n_true_bins_sb = true_sb->NBins();

    if (include_sb) {
      for(int ireco = 0; ireco < n_reco_bins_sb; ireco++) {
        for(int itrue = 0; itrue < n_true_bins_sb; itrue++) {
          double y[n_twks] = { 0 };
          double x[n_twks] = { 0 };
          for(int itwk = 0; itwk < n_twks; itwk++) {
            double twk_dial = twk_dial_min + itwk * twk_dial_step;
            double nevents  = mc_templ_sb [itwk][imode] -> GetEntry(ireco,itrue);
            double nevents0 = mc_templ_sb [inom][imode] -> GetEntry(ireco,itrue);
            y[itwk] = (nevents0 > 0) ? nevents/nevents0 : 1;
            x[itwk] = 1+(twk_dial * GSystUncertainty::Instance()->OneSigmaErr(syst));
          }//itwk
          TString name  = rfl.BuildSplineName(m,fit_param,itrue,ireco);
          TString title = name; // Make this contain real kinematic ranges
          //~ TSpline3 spl(title.Data(), x, y, n_twks, "0"); // Define each x knot individually
          TSpline3 spl(title.Data(), x[0], x[n_twks-1], y, n_twks, "0"); // Define only first and last knots - faster interpolation.
          spl.SetName(name.Data());
          spl.Write();
          n_splines_written++;
          if (n_splines_written%1000==0) {
            VLOG("make_response_func",pNOTICE)<<"Written "<<n_splines_written<<" splines so far.";
          }
        }//itrue
      }//ireco
    }//if include sb
    for(int ireco = 0; ireco < n_reco_bins_ob; ireco++) {
      for(int itrue = 0; itrue < n_true_bins_sb; itrue++) {
        double y[n_twks] = { 0 };
        double x[n_twks] = { 0 };
        for(int itwk = 0; itwk < n_twks; itwk++) {
          double twk_dial = twk_dial_min + itwk * twk_dial_step;
          double nevents  = mc_templ_ob [itwk][imode] -> GetEntry(ireco, itrue);
          double nevents0 = mc_templ_ob [inom][imode] -> GetEntry(ireco, itrue);
          y[itwk] = (nevents0 > 0) ? nevents/nevents0 : 1;
          x[itwk] = 1+(twk_dial * GSystUncertainty::Instance()->OneSigmaErr(syst));
        }//itwk
        TString name  = rfl.BuildSplineName(m,fit_param,itrue,ireco);
        TString title = name; // Make this contain real kinematic ranges
        //~ TSpline3 spl(title.Data(), x, y, n_twks, "0"); // Define each x knot individually
        TSpline3 spl(title.Data(), x[0],x[n_twks-1], y, n_twks, "0"); // Define only first and last knots - faster interpolation.
        spl.SetName(name.Data());
        spl.Write();
        n_splines_written++;
        if (n_splines_written%1000==0) {
          VLOG("make_response_func",pNOTICE)<<"Written "<<n_splines_written<<" splines so far.";
        }
      }//itrue
    }//ireco
    imode++;
  }//imode

  fout.Close();

  VLOG("make_response_func", pNOTICE)  << "Done! Wrote "<<n_splines_written<<" splines.";

  return 0;
}
//___________________________________________________________________

//________________________________________________________________________________________
/*

   Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

//#define _apply_mc_stat_fluctuation_

#include <cassert>
#include <ostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <algorithm>

#include <TXMLEngine.h>
#include <TFile.h>
#include <TObjArray.h>
#include <TMath.h>
#include <TSystem.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TPRegexp.h>

//fmwk
#include "valor/FitElements/GenericSingleSampleFitModel.h"

#include "valor/Conventions/Build.h"
#include "valor/Conventions/ExperimentSetup.h"

#include "valor/Conventions/Constants.h"
#include "valor/Conventions/Names.h"
#include "valor/Conventions/NuFlavour.h"
#include "valor/Conventions/NuOscMixParam.h"
#include "valor/Conventions/NuOscDmsqScheme.h"
#include "valor/Conventions/Opt.h"
#include "valor/Conventions/PDG.h"
#include "valor/Conventions/PhysHypothesisId.h"
#include "valor/Conventions/PhysParamId.h"
#include "valor/Utilities/Interpolator.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/Env.h"
#include "valor/Utilities/KinematicPoint.h"
#include "valor/Utilities/UtlNumStat.h"
#include "valor/Utilities/UtlParam.h"
#include "valor/Utilities/UtlStyle.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlXML.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/KineBinning/UtlBinning.h"
#include "valor/Oscillation/NuOsc3Flavour.h"
#include "valor/Oscillation/SimpleOsc.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Conventions/ReacMode.h"
#include "valor/Conventions/ParameterPool.h"
#include "valor/Conventions/Baseline.h"
#include "valor/Conventions/BeamConf.h"
#include "valor/Conventions/Detector.h"
#include "valor/Conventions/SystType.h"
#include "valor/Conventions/FitParamApply.h"

using std::endl;

using namespace valor;
using namespace valor::conv;

//#define SPLINE_WEIGHTS_ARE_ONE
//#define VALOR_EXPT_LOW_LEVEL_MESG_ENABLED

#define APPLY_ALL_AUTOWGHT_BRANCHES

//#define DEBUG_SINGLE_BIN

static const ExperimentSetup & es = *ExperimentSetup::Instance();

//________________________________________________________________________________________
GenericSingleSampleFitModel::GenericSingleSampleFitModel() :
  SingleSampleFitModelABC(),
  fBinning(NULL),
  fReactionModes(),
  fRFL(NULL),
  fSystematics(NULL),
  fOwnerOfSystematics(false),
  fApplyUniverseVariations(false),
  fFirstUniverseToSave(0),
  fLastUniverseToSave (0),
  fNonEmptyBinsOb(),
  fNonEmptyBinsSb(),
  fNumOfNonEmptyBinsOb(),
  fNumOfNonEmptyBinsSb(),
  fUnscaledBinEdgesOb(),
  fScaledBinEdgesOb(),
  fIsUnscaledBinOverflowOb(),
  fScaledBinToGlobalBinOb(),
  fUnscaledBinToGlobalBinOb(),
  fSkipBinOb(),
  fNOverflowBinsOb(),
  fUnscaledBinEdgesSb(),
  fScaledBinEdgesSb(),
  fIsUnscaledBinOverflowSb(),
  fScaledBinToGlobalBinSb(),
  fUnscaledBinToGlobalBinSb(),
  fSkipBinSb(),
  fNOverflowBinsSb(),
  fDefVALORArrayFormat("pdft_vf")
{
  this->Init();
}
//________________________________________________________________________________________
GenericSingleSampleFitModel::~GenericSingleSampleFitModel() 
{
  this->Clean();
}
//________________________________________________________________________________________
//
//
// FitModelABC interface implementation:
//
//________________________________________________________________________________________
string GenericSingleSampleFitModel::Name(void) const
{
  return fName;
}
//________________________________________________________________________________________
bool GenericSingleSampleFitModel::Read(string file, Option_t * opt)
{
  if (fDetector == conv::kdUndefined ) { LOG("SingleSampleFitModelABC",pFATAL) << "Bad Detector"  << fDetector ; util::Die(-1); }
  if (fBeam     == conv::kbUndefined ) { LOG("SingleSampleFitModelABC",pFATAL) << "Bad Beam"      << fBeam     ; util::Die(-1); }
  if (fSample   == conv::ksUndefined ) { LOG("SingleSampleFitModelABC",pFATAL) << "Bad Sample"    << fSample   ; util::Die(-1); }
  if (fKSp      == conv::kspUndefined) { LOG("SingleSampleFitModelABC",pFATAL) << "Bad KineSpace" << fKSp      ; util::Die(-1); }
  
  int ibeam   = es.BeamConfIndex(fBeam);
  int isample = es.SampleIndex(fBeam,fSample);
  if (isample == -1) {
    LOG("GenericSingleSampleFitModel",pFATAL)
      << "Sample not included in analysis build: "<<util::AsString(fBeam)<<" "<<util::AsString(fSample);
    util::Die(-1);
  }

  fReactionModes = es.ReactionModes()[ibeam][isample];

  fName = util::nm::MdlName(fDetector, fBeam, fSample, fKSp);
  
  TString ts_opt(opt);
  bool force_use_dst    = ts_opt.Contains("forcedst");
  bool skip_spline_read = ts_opt.Contains("nosplines");
  bool skip_mc_read     = ts_opt.Contains("nomc");
#ifdef SPLINE_WEIGHTS_ARE_ONE
  skip_spline_read = true;
#endif
  
  // Set up the initial XML reader
  LOG("GenericSingleSampleFitModel", pNOTICE) 
    << "Reading configuration from XML file [" << file << "]";

  fConfigFileRead = util::CompleteConfigPath(file);
  TString ts_file = fConfigFileRead.c_str();

  // Parse the input XML configuration file
  TXMLEngine* xml = new TXMLEngine;   
  XMLDocPointer_t xmldoc = xml->ParseFile(ts_file.Data());
  if (xmldoc==0) {
    LOG("GenericSingleSampleFitModel",pERROR) << "Can not parse input XML file [" << ts_file << "]";
    delete xml;
    util::Die(1); return false;
  }
  XMLNodePointer_t root_node = xml->DocGetRootElement(xmldoc);
  
  
  // Get binning file name, potentially from option string
  string binning_file;
  TPRegexp binning_re("(?<=binning\\=)[^ ,]+");
  TObjArray * matches = binning_re.MatchS(opt);
  if(matches->GetEntries()==1) {
    TObjString * binning_objstring = dynamic_cast<TObjString*>(matches->At(0));
    if (binning_objstring) {
      binning_file = binning_objstring->String().Data();
    }
    LOG("GenericSingleSampleFitModel",pNOTICE) << fName<< ": Reading binning from "<<binning_file;
  }
  else {
    binning_file = util::XML2String(xml,root_node,"default_binning");
    LOG("GenericSingleSampleFitModel",pNOTICE) << fName<< ": No sample-specific binning in options \""<<opt<<"\" , falling back to default: "<<binning_file;
  }
  delete matches;
  
  if (fBinning) {delete fBinning;}
  fBinning = new SingleSampleKineBinning(fDetector,fBeam,fSample);
  fBinning->Read(binning_file);

  this->Book();
  this->Zero();

  //------------------------
  // TODO
  // fSystematics = new CompoundCorrelatedSystematics; 
  // fSystematics->Read(xml,root_node);
  //------------------------

  // Get the detector fiducial mass
  string name = "fiducial_mass";
  fFiducialMass = util::XML2Dbl(xml,root_node,name);
  LOG("GenericSingleSampleFitModel",pINFO)
    << "Fiducial mass = " << fFiducialMass*1000 << " tonnes";

  // Check whether or not we should apply the osc prob to ALL detectors (i.e. skipping the util::OscProbIsAppliedTo check)
  name = "force_apply_osc_prob_to_all_detectors";
  if(util::NodeExists(xml,root_node,name)) {
    fSkipOscProbApplyCheck = util::XML2Bool(xml,root_node,name);
    LOG("GenericSingleSampleFitModel",pINFO)
      << "Apply osc prob to all detectors (regardless of util::OscProbIsAppliedTo)? " << fSkipOscProbApplyCheck << " tonnes";
  }
  // Get assumed exposure for each beam configuration
  {
    // Optionally over-ride the POT
    if (Opt::Instance()->GlobalRegistry().Exists("pot_override")) {
      fPOT = Opt::Instance()->GlobalRegistry().GetDouble("pot_override");
    }
    else {
      string node_name = "data/exposure/" + util::AsString(fBeam, "lowercase");
      fPOT = util::XML2Dbl(xml,root_node,node_name);
    }
    LOG("GenericSingleSampleFitModel",pNOTICE) 
      << "Beam config: " << util::AsString(fBeam) 
      << " -> Exposure = " << fPOT << " POT.";
  }

  // Read in any external tuning hists that we might have
  this->ReadExtTuningHistsAll(xml, root_node);

  //
  // Using input in VALOR DST format?
  // If true, read the ROOT files and fill-in the MC templates.
  // If false, just read in the MC templates directly. 
  // In the later case, the templates are pre-binned so the binning
  // can not change. Make sure the templates have the expected binning.
  //
  fUseMCDST = util::XML2Bool(xml,root_node,"mc/using_dst_format") || force_use_dst;
  if (skip_mc_read) {
    LOG("GenericSingleSampleFitModel",pWARN) << "Skipping MC read.";
  }
  else if(fUseMCDST) {
    LOG("GenericSingleSampleFitModel",pINFO) << "Filling in VALOR MC templates using VALOR DSTs";
    string node_name = "mc/dst/" + util::AsString(fBeam, "lowercase");
    TTree * mc_dst = util::XML2TTree (xml,root_node,node_name);
    if (!mc_dst) {
      LOG("GenericSingleSampleFitModel",pFATAL) << "Failed to read DST tree";
      util::Die(-1); return false;
    }
    this->FillFromDST(mc_dst);
    delete mc_dst;
  } 
  else {
    LOG("GenericSingleSampleFitModel",pINFO) << "Using stored VALOR MC templates";
    string template_tag = util::XML2String(xml,root_node,"mc/template/tag");
    TFile * template_file = util::XML2TFile(xml,root_node,"mc/template");

    this->FillFromTemplateFile(template_file,template_tag,fPOT);
    template_file->Close();
    delete template_file;
  }
  
  double n_events = 0.;
  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
    ReacMode_t m = fReactionModes[imode];
    if(m == kmUndefined) continue;
    n_events+=fMCTNomSb[imode]->Integral();
  }
  LOG("GenericSingleSampleFitModel",pNOTICE) << "Untuned template contains "<<n_events<<" events.";

  // At the point the nominal MC templates are filled-in.
  // Apply any fixed tuning here, not on an event by event basis.
  // It is the tuned templates that be the starting point in the Generate() method.
  this->ApplyTuning();

  // Print the tuned MC templates
  LOG("GenericSingleSampleFitModel",pDEBUG)
    << "\n\nAfter applying bin-by-bin tuning, the fMCTTunOb integrals for each mode are:";
  double fMCTTunOb_tot_integral = 0;
  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
    ReacMode_t m = fReactionModes[imode];
    if(m == kmUndefined) continue;
    fMCTTunOb_tot_integral += fMCTTunOb[imode]->Integral();
    LOG("GenericSingleSampleFitModel",pDEBUG)
      << "fMCTTunOb[" << util::AsString(m) << "] integral = " << fMCTTunOb[imode]->Integral();
  }
  LOG("GenericSingleSampleFitModel",pDEBUG) << "fMCTTunOb integral summed over all modes = " << fMCTTunOb_tot_integral;

  // Release memory
  xml->FreeDoc(xmldoc);
  delete xml;

  bool any_splines_needed = false;
  // Check there are actually any splines needed for this sample/parameterisation
  for (int im = 0 ; im < conv::kNReacModes ; im++) {
    if (fSplineParamsAppliedTo[im].size()) {
      any_splines_needed = true;
      break;
    }
  }

  if (!any_splines_needed) {
    skip_spline_read = true;
    LOG("GenericSingleSampleFitModel", pINFO)
      << "Could not find any splines, so skipping spline read";
  }
  
  if (!skip_spline_read) {
    if (! this->FillSplines(file,opt)) { return false; }
  }

  this->FillNonEmptyBinVectors();
  this->PreComputeLateralSystVectors(false);
  this->PreComputeLateralSystVectors(true);


  LOG("GenericSingleSampleFitModel",pNOTICE) << "Done configuring DUNE/ND model... \n\n";

  return true;
}
//________________________________________________________________________________________
bool GenericSingleSampleFitModel::Read(util::ToyStream* stream)
{
   LOG("GenericSingleSampleFitModel",pDEBUG) << "Reading model from stream";

   Option_t* read_opt = stream->GetVALORArrayFormat();
   string opt = (read_opt) ? read_opt : fDefVALORArrayFormat;

   // Read in the MC templates
   if(opt.find("mct") != string::npos)
   {
      for(size_t imode = 0; imode < fReactionModes.size(); imode++)
      {
         ReacMode_t m = fReactionModes[imode];
         if(m == kmUndefined || fMCTNullOb[imode])
         {
            continue;
         }
         stream->Read(fMCTNomOb[imode]);
         stream->Read(fMCTTunOb[imode]);
         stream->Read(fMCTSysOb[imode]);
         stream->Read(fMCTFinOb[imode]);

         if(opt.find("+SB") != string::npos)
         {
            if(fMCTNullSb[imode])
            {
               continue;
            }
            stream->Read(fMCTNomSb[imode]);
            stream->Read(fMCTTunSb[imode]);
            stream->Read(fMCTSysSb[imode]);
            stream->Read(fMCTFinSb[imode]);
         }
      }
   }

   // Write out the PDF components
   if(opt.find("pdfc") != string::npos)
   {
      for(size_t imode = 0; imode < fReactionModes.size(); imode++)
      {
         ReacMode_t m = fReactionModes[imode];
         if(m == kmUndefined || fMCTNullOb[imode])
         {
            continue;
         }
         stream->Read(fPDFCompOb[imode]);

         if(opt.find("+SB") != string::npos)
         {
            if(fMCTNullSb[imode])
            {
               continue;
            }
            stream->Read(fPDFCompSb[imode]);
         }
      }
   }

   // Write out the total PDF
   if(opt.find("pdft") != string::npos)
   {
      stream->Read(fPDFTotOb);

      if(opt.find("+SB") != string::npos)
      {
        stream->Read(fPDFTotSb);
      }
   }

   return true;

}
//________________________________________________________________________________________
bool GenericSingleSampleFitModel::FillSplines(string file, Option_t * opt)
{
  LOG("GenericSingleSampleFitModel",pINFO) << "Configuring the Response Function Library... \n\n";
  // And now get the RFL
  fRFL = new util::RFL(fBinning, fSplineParams);
  fRFL->Read(file, opt);
  
  bool null_warning = false;

  const VALORBinning * reco_ob = fBinning->RecoOptBinning();
  const int n_reco_bins_ob = reco_ob->NBins();

  // no SB splines for now
  //~ const VALORBinning * reco_sb = fBinning->RecoSmpBinning();
  //~ const int n_reco_bins_sb = reco_sb->NBins();
  
  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
    ReacMode_t m = fReactionModes[imode];
    if(m == kmUndefined) continue;


    const VALORBinning * true_sb = fBinning->TrueSmpBinning(m);
    const int n_true_bins_sb = true_sb->NBins();
    
    // Optimised binning
    util::KinematicPoint kp;
    for (int itrue = 0 ; itrue < n_true_bins_sb ; itrue++) {
      std::vector<double> true_centers = true_sb->BinCentre(itrue);
      if (true_centers.size() > 0) { kp.true1 = true_centers[0]; }
      if (true_centers.size() > 1) { kp.true2 = true_centers[1]; }
      if (true_centers.size() > 2) { kp.true3 = true_centers[2]; }
      for (int ireco = 0 ; ireco < n_reco_bins_ob ; ireco++) {
        std::vector<double> reco_centers = reco_ob->BinCentre(ireco);
        kp.reco1 = reco_centers[0];
        kp.reco2 = reco_centers[1];
        std::pair<int,int> binid = fRFL->GetBin(kp,m);
        int ispltrue = binid.first;
        int isplreco = binid.second;
        for (unsigned int ip = 0 ; ip < fSplineParamsAppliedTo[imode].size(); ip++) {
          PhysParamId_t p = fSplineParamsAppliedTo[imode][ip];
          Long64_t spl_as_int = reinterpret_cast<Long64_t>(util::RFL::kNoSplineRequired);
          const Parameter & param = ParameterPool::Instance()->GetParam(p);
          if (param.GetType() == conv::kSystResponse) {
            const util::Interpolator * spl = fRFL->GetSpline(m,p,ispltrue,isplreco);
            if (spl == NULL) {
              LOG("GenericSingleSampleFitModel",pINFO) << "Got null spline.\n";
              null_warning = true;
            }
            spl_as_int = reinterpret_cast<Long64_t>(spl);
          } // param has splines
          fRFPtr[imode]->SetEntry(spl_as_int,ireco,itrue,ip);
        }//ip
      }//ireco
    }//itrue
  }//imode
  if (null_warning) {
    LOG("GenericSingleSampleFitModel",pFATAL) << "Null splines found - giving up.";
    util::Die(-1);return false;
  }
  LOG("GenericSingleSampleFitModel",pINFO) << "Spline array filled... \n\n";
  return true;
}
//________________________________________________________________________________________
bool GenericSingleSampleFitModel::Generate(const PhysHypothesis & ph, Option_t * opt)
{
  // input opt: 
  // - Leave null for the fit
  // - Use "+SB" to generate finely-binned predictions too (for plotting or toy-MC generation)

  //  LOG("GenericSingleSampleFitModel",pDEBUG)  
  //     << "Input physics hypothesis before copying to private data member: " << ph;

  fPhysHypothesis.Copy(ph);
  //PhysHypothesis_t hyp = fPhysHypothesis.Hypothesis();

  LOG("GenericSingleSampleFitModel",pINFO)  
    << "Generating model predictions for the input hypothesis: " 
    << fPhysHypothesis;

#ifndef __VALOR_FMWK_PENALTY_ONLY__
  this->FluctuateMCStat(opt);   // `tun' MC templates --> `tun' MC templates (NOTE: if important effect, will need to add a new type of templates
  //this->ApplySystematics(opt);  // `tun' MC templates --> `sys' MC templates
  this->ApplyPhysics(opt);      // `sys' MC templates --> `fin' MC templates

  // Print the final MC templates before oscillations
  LOG("GenericSingleSampleFitModel",pDEBUG)
    << "\n\nAfter applying systematics, the fMCTFinOb integrals for sample "<<util::AsString(fBeam)<<" "<<util::AsString(fSample)<<" and each mode are:";
  double fMCTFinOb_tot_integral = 0;
  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
    ReacMode_t m = fReactionModes[imode];
    if(m == kmUndefined) continue;
    fMCTFinOb_tot_integral += fMCTFinOb[imode]->Integral();
    LOG("GenericSingleSampleFitModel",pDEBUG)
      << "fMCTFinOb[" << util::AsString(m) << "] integral = " << fMCTFinOb[imode]->Integral();
  }
  LOG("GenericSingleSampleFitModel",pDEBUG) << "fMCTFinOb integral summed over all modes = " << fMCTFinOb_tot_integral;

  this->ApplyOscProb(opt);      // `fin' MC templates --> `fin' MC templates
  this->UpdatePDFs(opt);        // `fin' MC templates --> p.d.f
#endif //__VALOR_FMWK_PENALTY_ONLY__

  // Print the final MC templates
  LOG("GenericSingleSampleFitModel",pDEBUG)
    << "\n\nAt end of GSSFM::Generate, the fMCTFinOb integrals for sample "<<util::AsString(fBeam)<<" "<<util::AsString(fSample)<<" and each mode are:";
  fMCTFinOb_tot_integral = 0;
  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
    ReacMode_t m = fReactionModes[imode];
    if(m == kmUndefined) continue;
    fMCTFinOb_tot_integral += fMCTFinOb[imode]->Integral();
    LOG("GenericSingleSampleFitModel",pDEBUG)
      << "fMCTFinOb[" << util::AsString(m) << "] integral = " << fMCTFinOb[imode]->Integral();
  }
  LOG("GenericSingleSampleFitModel",pDEBUG) << "fMCTFinOb integral summed over all modes = " << fMCTFinOb_tot_integral;

  LOG("GenericSingleSampleFitModel",pDEBUG) << "Done generating predictions";

  return true;
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::RandomizeSystematics(Option_t * options)
{
  fSystematics->Tweak(false,NULL,options);
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::RandomizeSystematics(bool (* should_randomize)(conv::PhysParamId_t), Option_t * options)
{
  fSystematics->Tweak(false,0,should_randomize,options);
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::RandomizeSystematics(std::map<valor::conv::PhysParamId_t, bool> should_randomize, Option_t * options)
{
  fSystematics->Tweak(false,0,should_randomize,options);
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::TweakSystematic(
    string syst_name, double twkdial, Option_t * opt)
{
  PhysParamId_t p = util::NameRegistry::Instance()->PhysParamFromString(syst_name);
  this->TweakSystematic(p,twkdial,opt);
  return;
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::TweakSystematic(
    PhysParamId_t syst, double twkdial, Option_t * /*opt*/)
{
  if (fSystematics) {
    fSystematics->SetCurrentValue(syst,twkdial);
  }
  else {
    LOG("GenericSingleSampleFitModel",pWARN) << "No systematic model - not tweaking requested parameter: "
                                             << util::NameRegistry::Instance()->AsString(syst);
  }
}
//________________________________________________________________________________________
PhysParamList GenericSingleSampleFitModel::Tweaks(Option_t * /*opt*/) const
{
  PhysParamList list;

  fSystematics -> CopyTweaksTo(list);

  return list;
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::ResetSystematics(Option_t * /*opt*/)
{
  fSystematics -> Tweak(true, 0);
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::PrintSystematics(ostream & stream, Option_t * /*opt*/) const
{
  stream << endl;
}
//________________________________________________________________________________________
bool GenericSingleSampleFitModel::Generate (const FitParamList & fp, Option_t * opt)
{
  //  LOG("GenericSingleSampleFitModel",pDEBUG)  
  //     << "Input fit parameter list before converting to a physics hypothesis: " << fp;

  if (fSystematics) {
    fSystematics ->UpdateFrom(fp);
  }

  // extract the values of params driving the oscillation, decoherence or decay model
  PhysHypothesis ph(fp);

  // call the main Generate() method
  return this->Generate(ph, opt);
}
//________________________________________________________________________________________
bool GenericSingleSampleFitModel::Write(TObjArray * array, const char * tag, Option_t * write_opt)
{
  //
  // write_opt:
  // - if null, store only what is needed for the fit (equiv. with write_opt = "pdft_vf")
  // - if "all", store everything listed below
  // - include the string "mct_vf" to write-out the MC templates
  //   (functions of beam,sample,mode,zone,Ereco,yreco,Etrue - in VALORArray format)
  // - include the string "pdfc_vf" to write-out the PDF components 
  //   (functions of beam,sample,mode,zone,Ereco,yreco - in VALORArray format)
  // - include the string "pdfc_rf" to write out the above in bare ROOT format (TH1D, TH2D,....)
  // - include the string "pdft_vf" to write-out the total PDF 
  //   (functions of beam,sample,zone,Ereco,yreco - in VALORArray format)
  // - include the string "pdft_rf" to write-out the above in bare ROOT format
  // - include the string "pdfcs_rf" to write-out 1-D slices of the PDF components in bare ROOT format
  // - include the string "pdfts_rf" to write-out 1-D slices of the total PDF in bare ROOT format
  // - include the string "+SB" to write-out versions of the above in simpler binning
  //
  // Key:
  //  mct   -> MC template                    CC: f(reco,true;s,m)
  //  pdft  -> PDF total                      CC: f(reco,true;s)
  //  pdfc  -> PDF component                  CC: f(reco,true;s,m)
  //  pdfts -> 1-D slice of PDF total         CC: f(Ereco;s)|other_reco,true
  //  pdfcs -> 1-D slice of PDF component     CC: f(Ereco;s,m)|other_reco,true
  //  vf    -> VALOR format
  //  rf    -> ROOT format (TH1 or TH2)
  //  rpf   -> ROOT format (TH2Poly for 2-d histograms)
  //  ob    -> Optimized binning (for fit)
  //  sb    -> Simple binning (for plots)


  LOG("GenericSingleSampleFitModel", pINFO)  << "Writing results to input object array...";
  LOG("GenericSingleSampleFitModel", pINFO) << "tag = " << tag << ", write option = " << write_opt;
  LOG("GenericSingleSampleFitModel", pINFO)  << "sample = " << util::AsString(fSample) << ", detector = "
                                             << util::AsString(fDetector) << ", beam = " << util::AsString(fBeam);

  string opt = (write_opt) ? write_opt : fDefVALORArrayFormat;

  bool write_all = opt.find("all") != string::npos;
  bool write_sb  = opt.find("+SB") != string::npos;

  bool write_mct_vf   = (opt.find("mct_vf"  ) != string::npos) || write_all;
  bool write_pdft_vf  = (opt.find("pdft_vf" ) != string::npos) || write_all;
  bool write_pdft_rf  = (opt.find("pdft_rf" ) != string::npos) || write_all;
  bool write_pdfc_vf  = (opt.find("pdfc_vf" ) != string::npos) || write_all;
  bool write_pdfc_rf  = (opt.find("pdfc_rf" ) != string::npos) || write_all;
  //~ bool write_pdfts_vf = (opt.find("pdfts_vf") != string::npos) || write_all;
  bool write_pdfts_rf = (opt.find("pdfts_rf") != string::npos) || write_all;
  //~ bool write_pdfcs_vf = (opt.find("pdfcs_vf") != string::npos) || write_all;
  bool write_pdfcs_rf = (opt.find("pdfcs_rf") != string::npos) || write_all;

  if(write_pdfc_rf) {
    LOG("GenericSingleSampleFitModel",pWARN) << "The option pdfc_rf is not yet implemented.";
  }

  if(!array) {
    LOG("GenericSingleSampleFitModel",pERROR) << "Null input object array.";
    return false;
  }

  const SingleSampleKineBinning & binning = *fBinning;
  const VALORBinning & reco_smp = *(binning.RecoSmpBinning());
  const VALORBinning & reco_opt = *(binning.RecoOptBinning());

  //
  // Write out MC templates (VALORArray format)
  //

  if(write_mct_vf) {

    for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
      ReacMode_t m = fReactionModes[imode];
      if(m == kmUndefined) continue;

      if(fMCTNullOb[imode]) continue;
      fMCTNomOb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mctnom_ob_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
      fMCTTunOb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mcttun_ob_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
      fMCTSysOb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mctsys_ob_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
      fMCTFinOb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mctfin_ob_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
      LOG("GenericSingleSampleFitModel", pDEBUG) << "Adding: " << fMCTNomOb [imode]->GetName();
      LOG("GenericSingleSampleFitModel", pDEBUG) << "Adding: " << fMCTTunOb [imode]->GetName();
      LOG("GenericSingleSampleFitModel", pDEBUG) << "Adding: " << fMCTSysOb [imode]->GetName();
      LOG("GenericSingleSampleFitModel", pDEBUG) << "Adding: " << fMCTFinOb [imode]->GetName();
      array -> Add ( fMCTNomOb [imode] ); 
      array -> Add ( fMCTTunOb [imode] ); 
      array -> Add ( fMCTSysOb [imode] ); 
      array -> Add ( fMCTFinOb [imode] );

      if(write_sb) {
        if(fMCTNullSb[imode]) continue;
        fMCTNomSb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mctnom_sb_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
        fMCTTunSb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mcttun_sb_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
        fMCTSysSb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mctsys_sb_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
        fMCTFinSb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mctfin_sb_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
        LOG("GenericSingleSampleFitModel", pDEBUG) << "Adding: " << fMCTNomSb [imode]->GetName();
        LOG("GenericSingleSampleFitModel", pDEBUG) << "Adding: " << fMCTTunSb [imode]->GetName();
        LOG("GenericSingleSampleFitModel", pDEBUG) << "Adding: " << fMCTSysSb [imode]->GetName();
        LOG("GenericSingleSampleFitModel", pDEBUG) << "Adding: " << fMCTFinSb [imode]->GetName();
        array -> Add ( fMCTNomSb [imode] ); 
        array -> Add ( fMCTTunSb [imode] ); 
        array -> Add ( fMCTSysSb [imode] ); 
        array -> Add ( fMCTFinSb [imode] );
      }//+SB?
    }//imode
  }//mct_vf


  //
  // Write out the PDF components (VALORArray or bare ROOT format)
  //

  if(write_pdfc_vf || write_pdfc_rf) {

    for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
      ReacMode_t m = fReactionModes[imode];
      if(m == kmUndefined) continue;

      if(fMCTNullOb[imode]) continue;
      
      // in VALORArray format
      if (write_pdfc_vf) {
        fPDFCompOb [imode]->SetName(TString::Format("%s_%s_%s_%s",tag,"pdfc_ob_vf",fName.c_str(),util::AsString(m).c_str()).Data());
        LOG("GenericSingleSampleFitModel", pDEBUG) << "Adding: " << fPDFCompOb[imode]->GetName();
        array->Add(fPDFCompOb[imode]);
      }

      if(write_sb) {
        if(fMCTNullSb[imode]) continue;
        // in VALORArray format
        if (write_pdfc_vf) {
          fPDFCompSb[imode]->SetName(TString::Format("%s_%s_%s_%s",tag,"pdfc_sb_vf",fName.c_str(),util::AsString(m).c_str()));
          LOG("GenericSingleSampleFitModel", pDEBUG) << "Adding: " << fPDFCompSb[imode]->GetName();
          array->Add(fPDFCompSb[imode]);
        }
      }//+SB?          

    }//imode
  }//pdfc_vf,pdfc_rf


  //
  // Write out the total PDF (VALORArray or bare ROOT format)
  //

  if(write_pdft_vf || write_pdft_rf) {
      // in VALORArray format
      if (write_pdft_vf) {
        fPDFTotOb->SetName(TString::Format("%s_%s_%s",tag,"pdft_ob_vf",fName.c_str()).Data());
        LOG("GenericSingleSampleFitModel", pDEBUG) << "Adding: " << fPDFTotOb->GetName();
        array->Add(fPDFTotOb);
      }

      if (write_pdft_rf) {
        const std::vector<KineVar_t>  & reco_vars = reco_opt.KineVars();
        if(reco_vars.size() == 1) {
          conv::KineSpace_t ks(util::KineSpaceFromKineVar(reco_vars[0]));
          TH1 * hob = util::binning::Make1DHistogram(*fPDFTotOb,binning,ks,false);
          if(hob) {
            TString hobname = TString::Format("%s_%s_%s_%s",tag,"pdft_ob_rf",fName.c_str(),util::AsString(reco_vars[0]).c_str());
            hob->SetName(hobname.Data());
            array->Add(hob);
          }
        }
        else if(reco_vars.size() == 2) {
          conv::KineSpace_t ks = util::KineSpaceFromKineVars(reco_vars);
          if(ks != conv::kspUndefined) {
            TH2 *hob = util::binning::Make2DHistogram(*fPDFTotOb,&reco_opt,reco_vars);
            if(hob) {
              TString hobname = TString::Format("%s_%s_%s_%s",tag,"pdft_ob_rf",fName.c_str(),util::AsString(ks).c_str());
              hob->SetName(hobname.Data());
              array->Add(hob);
            }
          }
        }
        else {
          LOG("GenericSingleSampleFitModel", pWARN)
            << "Tried writing PDFT to ROOT format, but number of kine vars != 1 or 2 (" << fPDFTotOb->GetName() << ")";
        }
      }

    if(write_sb) {
      // in VALORArray format
      if (write_pdft_vf) {
        fPDFTotSb->SetName(TString::Format("%s_%s_%s",tag,"pdft_sb_vf",fName.c_str()));
        LOG("GenericSingleSampleFitModel", pDEBUG) << "Adding: " << fPDFTotSb->GetName();
        array->Add(fPDFTotSb);
      }

      if (write_pdft_rf) {
        const std::vector<KineVar_t>  & reco_vars = reco_smp.KineVars();
        if(reco_vars.size() == 1) {
          conv::KineSpace_t ks(util::KineSpaceFromKineVar(reco_vars[0]));
          TH1 * hob = util::binning::Make1DHistogram(*fPDFTotSb,binning,ks,true);
          if(hob) {
            TString hobname = TString::Format("%s_%s_%s_%s",tag,"pdft_sb_rf",fName.c_str(),util::AsString(reco_vars[0]).c_str());
            hob->SetName(hobname.Data());
            array->Add(hob);
          }
        }
        else if(reco_vars.size() == 2) {
          conv::KineSpace_t ks = util::KineSpaceFromKineVars(reco_vars);
          if(ks != conv::kspUndefined) {
            TH2 *hob = util::binning::Make2DHistogram(*fPDFTotSb,&reco_smp,reco_vars);
            if(hob) {
              TString hobname = TString::Format("%s_%s_%s_%s",tag,"pdft_sb_rf",fName.c_str(),util::AsString(ks).c_str());
              hob->SetName(hobname.Data());
              array->Add(hob);
            }
          }
        }
        else {
          LOG("GenericSingleSampleFitModel", pWARN)
            << "Tried writing PDFT to ROOT format, but number of kine vars != 1 or 2 (" << fPDFTotOb->GetName() << ")";
        }
      }
    }//+SB?

  }//pdft_vf,pdft_rf

  // Write 1d slices for the total in reco only
  if (write_pdfts_rf) {
    const std::vector<KineVar_t>  & reco_vars = reco_opt.KineVars();
    for (const conv::KineVar_t kv : reco_vars) {
      conv::KineSpace_t ks(util::KineSpaceFromKineVar(kv));
      if (ks == conv::kspUndefined) {continue;}
      TH1 * hob = util::binning::Make1DHistogram(*fPDFTotOb,binning,ks,false);
      if(hob) {
        TString hobname = TString::Format("%s_%s_%s_%s",tag,"pdfts_ob_rf",fName.c_str(),util::AsString(kv).c_str());
        hob->SetName(hobname.Data());
        array->Add(hob);
      }
      if (write_sb) {
        TH1 * hsb = util::binning::Make1DHistogram(*fPDFTotSb,binning,ks,true);
        if(hsb) {
          TString hsbname = TString::Format("%s_%s_%s_%s",tag,"pdfts_sb_rf",fName.c_str(),util::AsString(kv).c_str());
          hsb->SetName(hsbname.Data());
          array->Add(hsb);
        }
      }
    }
  }

  // Write 1d slices for the components in both reco and true

  if (write_pdfcs_rf) {
    const std::vector<KineVar_t>  & reco_vars = reco_opt.KineVars();

    for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
      ReacMode_t m = fReactionModes[imode];
      if(m == kmUndefined) continue;
      
      const std::vector<KineVar_t> & true_vars = binning.TrueSmpBinning(imode)->KineVars();

      // Write reco, using the PDF comp objects (which have lateral systematics applied)
      for (const conv::KineVar_t kv : reco_vars) {
        conv::KineSpace_t ks(util::KineSpaceFromKineVar(kv));
        if (ks == conv::kspUndefined) {continue;}
        TH1 * hob = util::binning::Make1DHistogram(*fPDFCompOb[imode],binning,ks,false);
        if(hob) {
          TString hobname = TString::Format("%s_%s_%s_%s_%s",tag,"pdfcs_ob_rf",
            fName.c_str(),util::AsString(kv).c_str(),util::AsString(m).c_str());
          hob->SetName(hobname.Data());
          array->Add(hob);
        }

        if (write_sb) {
          TH1 * hsb = util::binning::Make1DHistogram(*fPDFCompSb[imode],binning,ks,true);
          if(hsb) {
            TString hsbname = TString::Format("%s_%s_%s_%s_%s",tag,"pdfcs_sb_rf",
              fName.c_str(),util::AsString(kv).c_str(),util::AsString(m).c_str());
            hsb->SetName(hsbname.Data());
            array->Add(hsb);
          }
        }
      }
      for (const conv::KineVar_t kv : true_vars) {
        conv::KineSpace_t ks(util::KineSpaceFromKineVar(kv));
        if (ks == conv::kspUndefined) {continue;}
        TH1 * hob = util::binning::Make1DHistogram(*fMCTFinOb[imode],binning,ks,false, m);
        if(hob) {
          TString hobname = TString::Format("%s_%s_%s_%s_%s",tag,"pdfcs_ob_rf",
            fName.c_str(),util::AsString(kv).c_str(),util::AsString(m).c_str());
          hob->SetName(hobname.Data());
          array->Add(hob);
        }

        if (write_sb) {
          TH1 * hsb = util::binning::Make1DHistogram(*fMCTFinSb[imode],binning,ks,true, m);
          if(hsb) {
            TString hsbname = TString::Format("%s_%s_%s_%s_%s",tag,"pdfcs_sb_rf",
              fName.c_str(),util::AsString(kv).c_str(),util::AsString(m).c_str());
            hsb->SetName(hsbname.Data());
            array->Add(hsb);
          }
        }
      }
    }
  }

  // Warn the user if SB is combined with universes
  if (write_sb && fApplyUniverseVariations) {
    LOG("GenericSingleSampleFitModel", pWARN)
      << "Wrote simple binning with universe variations on. Be aware these are applied only to OB";
  }

  return true;
}
//________________________________________________________________________________________
bool GenericSingleSampleFitModel::Write(
    const char * filename, const char * tag, Option_t * write_opt, Option_t * file_opt)
{
  LOG("GenericSingleSampleFitModel",pNOTICE) << "Writing results to file: [" << filename << "]";

  TObjArray * array = new TObjArray();
  this->Write(array,tag,write_opt);

  Option_t * used_file_opt = (file_opt == 0) ? "recreate" : file_opt;
  TFile f(filename, used_file_opt);
  array->Write();
  f.Close();
  delete array;

  return true;
}
//________________________________________________________________________________________
bool GenericSingleSampleFitModel::Write(util::ToyStream* stream)
{
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
   LOG("GenericSingleSampleFitModel",pINFO) << "Writing results to stream";
#endif

   Option_t* write_opt = stream->GetVALORArrayFormat();
   string opt = (write_opt) ? write_opt : fDefVALORArrayFormat;

   // Write out the MC templates
   if(opt.find("mct") != string::npos)
   {
      for(size_t imode = 0; imode < fReactionModes.size(); imode++)
      {
         ReacMode_t m = fReactionModes[imode];
         if(m == kmUndefined || fMCTNullOb[imode])
         {
            continue;
         }
         stream->Write(fMCTNomOb[imode]);
         stream->Write(fMCTTunOb[imode]);
         stream->Write(fMCTSysOb[imode]);
         stream->Write(fMCTFinOb[imode]);

         if(opt.find("+SB") != string::npos)
         {
            if(fMCTNullSb[imode])
            {
               continue;
            }
            stream->Write(fMCTNomSb[imode]);
            stream->Write(fMCTTunSb[imode]);
            stream->Write(fMCTSysSb[imode]);
            stream->Write(fMCTFinSb[imode]);
         }
      }
   }

   // Write out the PDF components
   if(opt.find("pdfc") != string::npos)
   {
      for(size_t imode = 0; imode < fReactionModes.size(); imode++)
      {
         ReacMode_t m = fReactionModes[imode];
         if(m == kmUndefined || fMCTNullOb[imode])
         {
            continue;
         }
         stream->Write(fPDFCompOb[imode]);

         if(opt.find("+SB") != string::npos)
         {
            if(fMCTNullSb[imode])
            {
               continue;
            }
            stream->Write(fPDFCompSb[imode]);
         }
      }
   }

   // Write out the total PDF
   if(opt.find("pdft") != string::npos)
   {
      stream->Write(fPDFTotOb);
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
      LOG("GenericSingleSampleFitModel", pINFO) << "Sample integral: " <<
         fPDFTotOb->Integral();
#endif

      if(opt.find("+SB") != string::npos)
      {
        stream->Write(fPDFTotSb);
      }
   }

   return true;
}
//________________________________________________________________________________________
long GenericSingleSampleFitModel::StreamSize (Option_t* opt_in) const
{
  string opt = (opt_in) ? opt_in : fDefVALORArrayFormat;
  long size = 0;

  if(opt.find("mct") != string::npos)
  {
    for(size_t imode = 0; imode < fReactionModes.size(); imode++)
    {
      ReacMode_t m = fReactionModes[imode];
      if(m == kmUndefined || fMCTNullOb[imode])
      {
        continue;
      }
      size += fMCTNomOb[imode]->StreamSize();
      size += fMCTTunOb[imode]->StreamSize();
      size += fMCTSysOb[imode]->StreamSize();
      size += fMCTFinOb[imode]->StreamSize();

      if(opt.find("+SB") != string::npos)
      {
        if(fMCTNullSb[imode])
        {
          continue;
        }
        size += fMCTNomSb[imode]->StreamSize();
        size += fMCTTunSb[imode]->StreamSize();
        size += fMCTSysSb[imode]->StreamSize();
        size += fMCTFinSb[imode]->StreamSize();
      }
    }
  }

  if(opt.find("pdfc") != string::npos)
  {
    for(size_t imode = 0; imode < fReactionModes.size(); imode++)
    {
      ReacMode_t m = fReactionModes[imode];
      if(m == kmUndefined || fMCTNullOb[imode])
      {
        continue;
      }
      size += fPDFCompOb[imode]->StreamSize();

      if(opt.find("+SB") != string::npos)
      {
        if(fMCTNullSb[imode])
        {
          continue;
        }
        size += fPDFCompSb[imode]->StreamSize();
      }
    }
  }

  if(opt.find("pdft") != string::npos)
  {
    size += fPDFTotOb->StreamSize();
    if(opt.find("+SB") != string::npos)
    {
      size += fPDFTotSb->StreamSize();
    }
  }

  return size;
}
//________________________________________________________________________________________
double GenericSingleSampleFitModel::GetFitStatisticPenalty (Option_t * /*opt*/) const
{
  double Sp = (fSystematics-> ChisqPenalty());
  return Sp;
}
//________________________________________________________________________________________
//
//
// Private methods:
//
//________________________________________________________________________________________

void GenericSingleSampleFitModel::Init(void)
{
  LOG("GenericSingleSampleFitModel",pDEBUG) << "Initializing...";

  fName         = "none";
  fFiducialMass = 0;
  fPOT          = 0;
  fUseMCDST     = false;
  fRFL          = 0;

  fPDFTotOb      = 0;
  fPDFTotNullOb  = true;

  fPDFTotSb     = 0;
  fPDFTotNullSb = true;

  fSkipOscProbApplyCheck = false;

  for(size_t imode = 0; imode < conv::kNReacModes; imode++) {

    // MC templates (simple binning)
    fMCTNullSb[imode] = true;
    fMCTNomSb [imode] = 0;
    fMCTTunSb [imode] = 0;
    fMCTSysSb [imode] = 0;
    fMCTFinSb [imode] = 0;
    fMCStatSb [imode] = 0;
    // PDF (simple binning)
    fPDFCompSb[imode] = 0;

    // MC templates
    fMCTNullOb [imode] = true;
    fMCTNomOb  [imode] = 0;
    fMCTTunOb  [imode] = 0;
    fMCTSysOb  [imode] = 0;
    fMCTFinOb  [imode] = 0;
    fMCStatOb  [imode] = 0;
    // Splines
    fRFPtr     [imode] = 0;
    // PDF
    fPDFCompOb [imode] = 0;
    
  }//imode

  fSystematics = 0;
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::Book(void)
{
  LOG("GenericSingleSampleFitModel",pDEBUG) << "Booking "<<util::AsString(fBeam)<<" "<<util::AsString(fSample);
  
  const VALORBinning  * reco_ob = fBinning->RecoOptBinning();
  const VALORBinning  * reco_sb = fBinning->RecoSmpBinning();

  const char * tag = "init"; // initial name for the array on booking

  // Modes should eventually move to being fully runtime configurable
  // Which means that this will end up going in read
  // And we may have to completely merge Book/Read
  int ibeam   = es.BeamConfIndex(fBeam);
  int isample = es.SampleIndex(fBeam,fSample);
  fReactionModes = es.ReactionModes()[ibeam][isample];

  fNonEmptyBinsOb = std::vector<std::vector<int>>(fReactionModes.size());
  fNonEmptyBinsSb = std::vector<std::vector<int>>(fReactionModes.size());
  fNumOfNonEmptyBinsOb = std::vector<int>(fReactionModes.size());
  fNumOfNonEmptyBinsSb = std::vector<int>(fReactionModes.size());

  this->BuildParamLists();

  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
    ReacMode_t m = fReactionModes[imode];
    if(m == kmUndefined) continue;
    
    const VALORBinning * true_sb = fBinning->TrueSmpBinning(m);

    const int n_reco_bins_ob = reco_ob->NBins();
    const int n_reco_bins_sb = reco_sb->NBins();
    const int n_true_bins    = true_sb->NBins();

    fNonEmptyBinsOb[imode] = std::vector<int>(n_reco_bins_ob * n_true_bins);
    fNonEmptyBinsSb[imode] = std::vector<int>(n_reco_bins_sb * n_true_bins);

    LOG("GenericSingleSampleFitModel",pDEBUG) 
      << "Booking MC template (simple binning) for " 
      << "beam: "     << util::AsString(fBeam) 
      << ", sample: " << util::AsString(fSample)
      << ", mode: "   << util::AsString(m)
      << ", with "<<n_reco_bins_sb<<" reco bins and "<<n_true_bins<<" true bins"; 

    // >> MC templates (simple binning)
    // (Note order of variables: reco first, true later)
    fMCTNullSb[imode] = false;
    fMCTNomSb [imode] = new VALORArray(n_reco_bins_sb, n_true_bins);
    fMCTTunSb [imode] = new VALORArray(n_reco_bins_sb, n_true_bins);
    fMCTSysSb [imode] = new VALORArray(n_reco_bins_sb, n_true_bins);
    fMCTFinSb [imode] = new VALORArray(n_reco_bins_sb, n_true_bins);
    fMCStatSb [imode] = new VALORArray(n_reco_bins_sb, n_true_bins);
    
    fMCTNomSb [imode]->SetDimensionName(0,"reco");
    fMCTTunSb [imode]->SetDimensionName(0,"reco");
    fMCTSysSb [imode]->SetDimensionName(0,"reco");
    fMCTFinSb [imode]->SetDimensionName(0,"reco");
    fMCStatSb [imode]->SetDimensionName(0,"reco");

    fMCTNomSb [imode]->SetDimensionName(1,"true");
    fMCTTunSb [imode]->SetDimensionName(1,"true");
    fMCTSysSb [imode]->SetDimensionName(1,"true");
    fMCTFinSb [imode]->SetDimensionName(1,"true");
    fMCStatSb [imode]->SetDimensionName(1,"true");

    fMCTNomSb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mctnom_sb_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
    fMCTTunSb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mcttun_sb_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
    fMCTSysSb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mctsys_sb_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
    fMCTFinSb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mctfin_sb_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
    fMCStatSb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mcstat_sb_vf", fName.c_str(),util::AsString(m).c_str()).Data() );

    // >> PDF component
    fPDFCompSb[imode] = 
      new VALORArray(n_reco_bins_sb);
    fPDFCompSb[imode]->SetName(TString::Format("%s_%s_%s_%s",tag,"pdfc_sb_vf",fName.c_str(),util::AsString(m).c_str()));
    fPDFCompSb[imode]->SetDimensionName(0,"reco");
    
    // >> PDF total
    // should get here for at least one imode value
    if(fPDFTotNullSb) {              
      LOG("GenericSingleSampleFitModel",pDEBUG) 
        << "Booking PDF (total / simple binning) for " 
        << "beam: "     << util::AsString(fBeam) 
        << ", sample: " << util::AsString(fSample);
      fPDFTotNullSb = false;
      fPDFTotSb     = new VALORArray(n_reco_bins_sb); 
      fPDFTotSb->SetName(TString::Format("%s_%s_%s",tag,"pdft_sb_vf",fName.c_str()).Data());
      fPDFTotSb->SetDimensionName(0,"reco");
    }
    
    // >> MC templates 
    // (Note order of variables: reco first, true later)
    LOG("GenericSingleSampleFitModel",pDEBUG) 
      << "Booking MC template (opt binning) for " 
      << "beam: "     << util::AsString(fBeam) 
      << ", sample: " << util::AsString(fSample)
      << ", mode: "   << util::AsString(m)
      << ", with "<<n_reco_bins_ob<<" reco bins and "<<n_true_bins<<" true bins"; 
      
    fMCTNullOb[imode] = false;
    fMCTNomOb [imode] = new VALORArray(n_reco_bins_ob, n_true_bins);
    fMCTTunOb [imode] = new VALORArray(n_reco_bins_ob, n_true_bins);
    fMCTSysOb [imode] = new VALORArray(n_reco_bins_ob, n_true_bins);
    fMCTFinOb [imode] = new VALORArray(n_reco_bins_ob, n_true_bins);
    fMCStatOb [imode] = new VALORArray(n_reco_bins_ob, n_true_bins);
    
    fMCTNomOb [imode]->SetDimensionName(0,"reco");
    fMCTTunOb [imode]->SetDimensionName(0,"reco");
    fMCTSysOb [imode]->SetDimensionName(0,"reco");
    fMCTFinOb [imode]->SetDimensionName(0,"reco");
    fMCStatOb [imode]->SetDimensionName(0,"reco");

    fMCTNomOb [imode]->SetDimensionName(1,"true");
    fMCTTunOb [imode]->SetDimensionName(1,"true");
    fMCTSysOb [imode]->SetDimensionName(1,"true");
    fMCTFinOb [imode]->SetDimensionName(1,"true");
    fMCStatOb [imode]->SetDimensionName(1,"true");


    fMCTNomOb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mctnom_ob_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
    fMCTTunOb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mcttun_ob_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
    fMCTSysOb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mctsys_ob_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
    fMCTFinOb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mctfin_ob_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
    fMCStatOb [imode] -> SetName( TString::Format("%s_%s_%s_%s", tag, "mcstat_ob_vf", fName.c_str(),util::AsString(m).c_str()).Data() );
      
    // Spline library
    fRFPtr [imode] = 
      new VALORArrayL(n_reco_bins_ob, n_true_bins, fSplineParams.size());
    fRFPtr [imode]->SetName(TString::Format("%s_%s_%s", "rfl_vf", fName.c_str(),util::AsString(m).c_str()).Data());

    // >> PDF component
    fPDFCompOb[imode] = 
      new VALORArray(n_reco_bins_ob);
    fPDFCompOb[imode]->SetName(TString::Format("%s_%s_%s_%s",tag,"pdfc_ob_vf",fName.c_str(),util::AsString(m).c_str()));
    fPDFCompOb[imode]->SetDimensionName(0,"reco");
    fPDFCompOb[imode]->SetDimensionName(1,"true");
    // >> PDF total
    // should get here for at least one imode value
    if(fPDFTotNullOb) {              
      LOG("GenericSingleSampleFitModel",pDEBUG) 
        << "Booking PDF (total) for " 
        << "beam: "     << util::AsString(fBeam) 
        << ", sample: " << util::AsString(fSample);

      fPDFTotNullOb  = false;
      fPDFTotOb      = new VALORArray(n_reco_bins_ob);
      fPDFTotOb->SetName(TString::Format("%s_%s_%s",tag,"pdft_ob_vf",fName.c_str()).Data());
      fPDFTotOb->SetDimensionName(0,"reco");
      fPDFTotOb->SetDimensionName(1,"true");

    }

    // If we need to save universe variations, make arrays for that
    size_t n_universes = fLastUniverseToSave-fFirstUniverseToSave+1;
    for (const std::string & param : fUniverseParametersToSave) {
      std::vector<VALORArrayF> & vec = fUniverseVariations[param];
      vec.resize(imode); // dumb but easier than making it reserve earlier
      assert(fLastUniverseToSave>=fFirstUniverseToSave);
      auto it = vec.begin();
      vec.insert(it+imode,VALORArrayF(n_reco_bins_ob,n_true_bins,n_universes));
      vec[imode].SetDimensionName(0,"reco");
      vec[imode].SetDimensionName(1,"true");
      vec[imode].SetDimensionName(2,"universe");
      vec[imode].SetAll(0.0f);
    }

  }//imode

  std::vector<conv::KineVar_t> kvs_already_considered;
  for(size_t ip = 0; ip < fLateralPostOscParams.size(); ip++) {
    PhysParamId_t p = fLateralPostOscParams[ip];
    if(p == kPhypUndefined) { continue; }

    const Parameter & param = ParameterPool::Instance()->GetParam(p);
    const std::string & param_name = param.GetName();
    if(!param.HasKinematicRange()) {
      LOG("Lateral",pFATAL) << "Lateral systematic parameter " << param_name
                            << " has no kinematic range. Cannot apply";
      util::Die(-1);
    }

    const std::vector<conv::KineVar_t> & kvs = param.GetKineVars();
    if(kvs.size() != 1) {
      LOG("Lateral",pFATAL) << "Lateral systematic parameter " << param_name
                            << " is applied to " << kvs.size() << " kinematic variables. Cannot apply";
      util::Die(-1);
    }
    const conv::KineVar_t & kv = kvs[0];
    if(std::find(kvs_already_considered.begin(), kvs_already_considered.end(), kv) != kvs_already_considered.end()) { continue; }
    kvs_already_considered.push_back(kv);

    fUnscaledBinEdgesOb      [kv] = std::vector<double>           ();
    fScaledBinEdgesOb        [kv] = std::vector<double>           ();
    fIsUnscaledBinOverflowOb [kv] = std::vector<bool>             ();
    fScaledBinToGlobalBinOb  [kv] = std::vector<unsigned int>     ();
    fUnscaledBinToGlobalBinOb[kv] = std::vector<unsigned int>     ();
    fSkipBinOb               [kv] = std::vector<std::vector<bool>>();
    fNOverflowBinsOb         [kv] = 0;
    fUnscaledBinEdgesSb      [kv] = std::vector<double>           ();
    fScaledBinEdgesSb        [kv] = std::vector<double>           ();
    fIsUnscaledBinOverflowSb [kv] = std::vector<bool>             ();
    fScaledBinToGlobalBinSb  [kv] = std::vector<unsigned int>     ();
    fUnscaledBinToGlobalBinSb[kv] = std::vector<unsigned int>     ();
    fSkipBinSb               [kv] = std::vector<std::vector<bool>>();
    fNOverflowBinsSb         [kv] = 0;

  } // ip: lateral post-oscillation systematics
  
  // Write will initialise the names of the arrays - useful for debugging in read
  TObjArray tmp_array;
  this->Write(&tmp_array,"valor","mct_vf+SB");
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::BuildParamLists()
{
  // Use a set to keep track of all of them
  std::set<PhysParamId_t> norm_params_set;
  std::set<PhysParamId_t> spline_params_set;
  std::set<PhysParamId_t> lateral_post_osc_params_set;
  const std::vector<Parameter> & params = ParameterPool::Instance()->GetParams();
  for (const Parameter & param : params) {
    if(!util::IsSystematic(param.GetID())) { continue; } // Osc params are handled in ApplyOscProb()
    for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
      bool is_applied = false;
      ReacMode_t m = fReactionModes[imode];
      if(m == kmUndefined) continue;

      for (size_t itarget = 0; itarget < kNNuclearTarget ; itarget++ ) {
        NuclearTarget_t nt = static_cast<NuclearTarget_t>(itarget);
        is_applied = is_applied || util::IsAppliedTo(param.GetApplyMask(),fBeam,fSample,fDetector,m,nt);
      } // itarget

      // Add them to our lists. This will be need to be updated if new parameter types are added
      if (is_applied) {
        PhysParamId_t p = param.GetID();
        SystType_t type = param.GetType();

        std::vector<KineVar_t> binning_kv = fBinning->RecoSmpBinning()->KineVars();
        const std::vector<KineVar_t> & true_kv = fBinning->TrueSmpBinning(m)->KineVars();

        switch (type) {

          case kSystNorm    :
            fNormParamsAppliedTo[imode].push_back(p);
            norm_params_set.insert(p);
            LOG("GenericSingleSampleFitModel",pINFO) << "Will apply norm param ["<<param.GetName()<<"] to mode ["<<util::AsString(m)<<"]";
            // Check if the normalisations only depend on parameters we're actually binned in

            binning_kv.insert(binning_kv.end(),true_kv.begin(),true_kv.end());

            for (const KineVar_t & kv : param.GetKineVars() ) {
              if (std::find(binning_kv.begin(),binning_kv.end(),kv) == binning_kv.end()) {
                LOG("GenericSingleSampleFitModel",pFATAL) << "Parameter "<<param.GetName()
                  <<" is binned in "<<util::AsString(kv)<<" << but model "<<fName<<" is not";
                util::Die(-1);
              }
            }
            break;
          case kSystResponse:
            fSplineParamsAppliedTo[imode].push_back(p);
            spline_params_set.insert(p);
            LOG("GenericSingleSampleFitModel",pINFO) << "Will apply spline param ["<<param.GetName()<<"] to mode ["<<util::AsString(m)<<"]";
            break;
          case kSystLateralPostOsc:
            // Handle post-oscillation lateral systematics after all templates have been filled
            lateral_post_osc_params_set.insert(p);
            LOG("GenericSingleSampleFitModel",pINFO) << "Will apply lateral (post-osc) param ["<<param.GetName()<<"] to mode ["<<util::AsString(m)<<"]";
            break;
          case kSystUndefined:
          case kSystLateral:
            LOG("GenericSingleSampleFitModel",pFATAL) << "Cannot handle parameter "<<param.GetName()<<" ("<<fName<<")";
            util::Die(-1); return;
          case kSystSpecial:
            LOG("GenericSingleSampleFitModel",pFATAL) << "Cannot handle parameter "<<param.GetName()<<" ("<<fName<<")";
            util::Die(-1); return;
          case kSystDisabled:
            break;
        } // switch type
      } // is applied
      else {
        if(param.GetType() == conv::kSystLateralPostOsc) {
          LOG("GenericSingleSampleFitModel",pFATAL)
            << "Lateral post-oscillation parameters must be applied to all modes and targets. "
            <<param.GetName()<<" ("<<fName<<") is not applied to mode " << util::AsString(m);
          util::Die(-1); return;
        }
      }

    } // imode

  } // parameters

  // And just the total list
  fNormParams = std::vector<PhysParamId_t>(norm_params_set.begin(),norm_params_set.end());
  std::sort(fNormParams.begin(),fNormParams.end());
  fSplineParams = std::vector<PhysParamId_t>(spline_params_set.begin(),spline_params_set.end());
  std::sort(fSplineParams.begin(),fSplineParams.end());
  fLateralPostOscParams = std::vector<PhysParamId_t>(lateral_post_osc_params_set.begin(),lateral_post_osc_params_set.end());
  std::sort(fLateralPostOscParams.begin(),fLateralPostOscParams.end());

}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::Clean(void)
{
  LOG("GenericSingleSampleFitModel",pDEBUG) << "Cleaning-up...";

  for(size_t imode = 0; imode < conv::kNReacModes; imode++) {

    if(!fMCTNullSb[imode]) {
      fMCTNullSb[imode] = true;
      // MC templates (simple binning)
      if ( fMCTNomSb [imode] ) delete fMCTNomSb [imode];
      if ( fMCTTunSb [imode] ) delete fMCTTunSb [imode];
      if ( fMCTSysSb [imode] ) delete fMCTSysSb [imode];
      if ( fMCTFinSb [imode] ) delete fMCTFinSb [imode];
      if ( fMCStatSb [imode] ) delete fMCStatSb [imode];
      fMCTNomSb[imode] = 0;
      fMCTTunSb[imode] = 0;
      fMCTSysSb[imode] = 0;
      fMCTFinSb[imode] = 0;
      fMCStatSb[imode] = 0;
      // PDF components
      if(fPDFCompSb[imode]) delete fPDFCompSb[imode];
      fPDFCompSb[imode] = 0;
    }
    if(!fPDFTotNullSb) {
      fPDFTotNullSb = true;
      if(fPDFTotSb) delete fPDFTotSb;
      fPDFTotSb = 0;
    }

    if(!fMCTNullOb[imode]) {
      fMCTNullOb[imode] = true;
      // MC templates
      if ( fMCTNomOb [imode] ) { delete fMCTNomOb [imode]; fMCTNomOb [imode] = 0; }
      if ( fMCTTunOb [imode] ) { delete fMCTTunOb [imode]; fMCTTunOb [imode] = 0; }
      if ( fMCTSysOb [imode] ) { delete fMCTSysOb [imode]; fMCTSysOb [imode] = 0; }
      if ( fMCTFinOb [imode] ) { delete fMCTFinOb [imode]; fMCTFinOb [imode] = 0; }
      if ( fMCStatOb [imode] ) { delete fMCStatOb [imode]; fMCStatOb [imode] = 0; }
      if ( fRFPtr    [imode] ) { delete fRFPtr    [imode]; fRFPtr    [imode] = 0; }
      
      // PDF components
      if(fPDFCompOb[imode]) { delete fPDFCompOb[imode]; fPDFCompOb[imode] = 0; }
    }
    if(!fPDFTotNullOb) {
      fPDFTotNullOb = true;
      if(fPDFTotOb) { delete fPDFTotOb; fPDFTotOb = 0; }
    }

  }//imode
  
  if (fRFL)  { delete fRFL ; fRFL  = 0; }
  if (fBinning) { delete fBinning ; fBinning = 0; }
  
  if (fSystematics && fOwnerOfSystematics) { delete fSystematics; fSystematics = 0; }

  for(TuningInfo pairpair : fEventByEventTuningHists) {
    delete pairpair.second;
  }
  for(TuningInfo pairpair : fBinByBinTuningHists) {
    delete pairpair.second;
  }

  this->ResetSystematicVariationIDs();
  this->ResetUniversesToRead();

  fConfigFileRead = "";
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::Zero(void)
{
  LOG("GenericSingleSampleFitModel",pDEBUG) << "Zeroing MC templates and/or PDFs...";

  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {

    if(!fMCTNullSb[imode]) {
      fMCTNomSb [imode] -> SetAll(0.);
      fMCTTunSb [imode] -> SetAll(0.);
      fMCTSysSb [imode] -> SetAll(0.);
      fMCTFinSb [imode] -> SetAll(0.);
      fMCStatSb [imode] -> SetAll(0.);
      fPDFCompSb[imode] -> SetAll(0.);
    }
    if(!fPDFTotNullSb) {
      fPDFTotSb-> SetAll(0.);
    }

    if(!fMCTNullOb[imode]) {
      fMCTNomOb [imode] -> SetAll(0.);
      fMCTTunOb [imode] -> SetAll(0.); 
      fMCTSysOb [imode] -> SetAll(0.);
      fMCTFinOb [imode] -> SetAll(0.);
      fMCStatOb [imode] -> SetAll(0.);
      fPDFCompOb[imode] -> SetAll(0.);
    }
    if(!fPDFTotNullOb) {
      fPDFTotOb -> SetAll(0.);
    }

  }//imode
}
//________________________________________________________________________________________
//________________________________________________________________________________________
void GenericSingleSampleFitModel::FillFromDST(TTree * t)
{
  
#ifdef __VALOR_FMWK_PENALTY_ONLY__
  return;
#endif

  // Fill MC templates from the input VALOR/DUNE DST TTree
  // We use a single tree for all samples for a single beam configuration.

  LOG("GenericSingleSampleFitModel",pDEBUG) 
    << "Filling-in MC templates for beam configuration: " << util::AsString(fBeam);

  if(!t) return;
  
  // Get a list of all the kinematic variables we care about
  const std::vector<conv::KineVar_t> & kinevars = fBinning->KineVars();

  int    sample        = -1;
  int    beam          = -1;
  int    reac_mode     = -1;
  double wght          = 1 ; //conv::kBadResult;
  double wght_pot      = conv::kBadResult;

  // Turn off all the branches by default
  t->SetBranchStatus("*",0);

  std::map<KineVar_t,double> kinematics;

  // Set up a list of the branches associated with each kinematic variable
  std::map<KineVar_t,std::string> kinematic_branch_names;
  for (const KineVar_t & kv : kinevars) {
    kinematic_branch_names[kv] = util::DSTBranchName(kv);
  }

  int bstat_sample     = 0;
  int bstat_beam       = 0;
  int bstat_reac_mode  = 0;
  std::map<KineVar_t,int> bstat_kinematics;

  // And turn all the base branches on
  t->SetBranchStatus("sample"   , 1);
  t->SetBranchStatus("beam"     , 1);
  t->SetBranchStatus("reac_mode", 1);
  t->SetBranchStatus("wght"     , 1);
  t->SetBranchStatus("wght_pot" , 1);

  bstat_sample    = t->SetBranchAddress("sample",    &sample);
  bstat_sample    = t->SetBranchAddress("beam"  ,    &beam);
  bstat_reac_mode = t->SetBranchAddress("reac_mode", &reac_mode);
  
  // Weight is not required - use one if failed
  t->SetBranchAddress("wght"       , &wght);
  t->SetBranchAddress("wght_pot"   , &wght_pot);

  bool any_bad_status = (bstat_sample || bstat_beam || bstat_reac_mode);

  for(const conv::KineVar_t & kv : kinevars) {
    const char * branchname = kinematic_branch_names[kv].c_str();
    t->SetBranchStatus(branchname,1);
    bool bstat = t->SetBranchAddress(branchname,&kinematics[kv]);
    any_bad_status = any_bad_status || bstat;
  }

  if (any_bad_status) {
    LOG("GenericSingleSampleFitModel",pWARN)<<"MiniDST branches not found - giving up";
    util::Die(-1);
  }
  
  // Also read any other branches named "wght_"
  TPRegexp wght_branch_pattern("autowght_\\w+");
  std::map<std::string,double> wght_branches;
  TObjArray * branch_list = t->GetListOfBranches();
  for (int ib = 0 ; ib < branch_list->GetEntries(); ib++) {
    const char * branchname = branch_list->At(ib)->GetName();
    t->SetBranchStatus(branchname,1);
    TObjArray * matches = wght_branch_pattern.MatchS(branchname);
    if (matches->GetEntries()==1) {
      wght_branches[branchname] = -1.;
      t->SetBranchAddress(branchname,&(wght_branches[branchname]));
    }
    delete matches;
  }

  // Let's work out what universe branches we need
  //~ bool apply_universe_weights = fSystUniverseToApply.size();
  const size_t max_n_universes = 10000;
  std::map<std::string,int> branch_n_universes;
  std::map<std::string,std::vector<float>> branch_universes;
  for (const auto & variation_name : fUniverseParametersToSave) {
    branch_universes.emplace(variation_name,std::vector<float>(max_n_universes));
    std::string branch_name   = Form("variation_%s",variation_name.c_str());
    std::string branch_name_n = Form("variation_n_%s",variation_name.c_str());
    t->SetBranchStatus (branch_name_n.c_str(),1);
    t->SetBranchStatus (branch_name  .c_str(),1);
    bool bstat_univ_n = t->SetBranchAddress(branch_name_n.c_str(),&branch_n_universes[variation_name]);
    bool bstat_univ   = t->SetBranchAddress(branch_name.  c_str(),branch_universes  [variation_name].data());
    if (bstat_univ_n || bstat_univ) {
      LOG("GenericSingleSampleModel",pFATAL) << fName<<" Failed to load needed universe branch.";
      util::Die(-1) ; return;
    }
  }

  Long_t nev = t->GetEntries();
  LOG("GenericSingleSampleFitModel",pINFO) << "Input VALOR DST has " << nev << " entries";
  
  double tree_weight = t->GetWeight();
  
  LOG("GenericSingleSampleFitModel",pNOTICE) << "Input VALOR DST has weight "<<tree_weight;
  
  const VALORBinning * reco_ob = fBinning->RecoOptBinning();
  const VALORBinning * reco_sb = fBinning->RecoSmpBinning();
  
  for(Long_t iev = 0; iev < nev; iev++) 
  {
    t->GetEntry(iev);

    if(iev && iev%(nev/1000)==0) {
      double pct = (100 * (double)(iev+1)) / (double)nev;
      LOG("GenericSingleSampleFitModel",pINFO) << "Reading "<< fName <<" MC DST: "<< TMath::Nint(pct) << "% done";
    }

    // Get sample index
    Sample_t s = util::SampleFromInt(sample);
    if(s == ksUndefined) {
      LOG("GenericSingleSampleFitModel",pWARN) 
        << " ** Undefined sample ID used in MC event: " << iev
        << " (value stored in DST: " << sample << ")";
      //util::Die(-1);return;
    }

    //  ================ NOTE - HACKS ================
    if(s == ksNuMuCC2piC) {
      LOG("HACK",pNOTICE) 
        << "MC event: " << iev << " belongs to sample: " << util::AsString(s) 
        << " not considered separately. Bundle in sample: " << util::AsString(ksNuMuCCoth);
      s = ksNuMuCCoth;
    }
    
#ifdef FORCE_NC_INCLUSIVE
    if(util::IsNC(s)) {
      LOG("HACK",pNOTICE) 
        << "MC event: " << iev << " belongs to NC sample: " << util::AsString(s) 
        << " Bundling it in sample: " << util::AsString(ksNC);
      s = ksNC;
    }
#endif

    //  ==============================================
    if(s != fSample) {
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
      LOG("GenericSingleSampleFitModel",pDEBUG) 
        << "Ignoring MC event: " << iev 
        << " (sample ID: " << s << " "<< util::AsString(s) 
        << " not matching one used for configuring model: " 
        << fSample <<" "<<util::AsString(fSample) << ")";
#endif
      continue;  
    }
    
    BeamConf_t b = util::BeamConfFromInt(beam);
    if(b != fBeam) {
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
      LOG("GenericSingleSampleFitModel",pDEBUG) 
        << "Ignoring MC event: " << iev 
        << " (beam ID: " << b << " "<< util::AsString(b) 
        << " not matching one used for configuring model: " 
        << fBeam <<" "<<util::AsString(fBeam) << ")";
#endif
      continue;  
    }

    // Get reaction mode index
    ReacMode_t m = util::ReacModeFromInt(reac_mode);
 
    if(m == kmUndefined) {
      LOG("GenericSingleSampleFitModel",pWARN) 
        << " ** Undefined reaction mode ID used in MC event: " << iev
        << " (value stored in DST: " << reac_mode << "). Ignoring event.";
      continue;
    }
    int imode = es.ReacModeIndex(b, s, m);
    if(imode < 0) {
      // For a given sample we should consider a complete list of
      // true reaction modes (otherwise there are true contributions to the
      // event sample which are not taken into account).
      LOG("GenericSingleSampleFitModel",pFATAL) 
        << " ** Ignoring MC event: " << iev 
        << " (ignoring contribution to sample: " << util::AsString(s) << " from mode: " << util::AsString(m) << " ?)";
      util::Die(-1);return;
    }

    bool is_null = fMCTNullOb[imode];
    if(is_null) {
      LOG("GenericSingleSampleFitModel",pFATAL) 
        << "** Null MC template for sample: " << util::AsString(s) 
        << ", mode: " << util::AsString(m);
      util::Die(-1);return;
    }
    bool is_null_sb = fMCTNullSb[imode];
    if(is_null_sb) {
      LOG("GenericSingleSampleFitModel",pFATAL) 
        << "** Null MC template (simple binning) for sample: " << util::AsString(s) 
        << ", mode: " << util::AsString(m) << " ??";
      util::Die(-1);return;
    }
    
    // Get the mode appropriate binning
    const VALORBinning * true_sb = fBinning->TrueSmpBinning(m);

    // check kinematic values against built-in limits and make vectors
    std::vector<double> true_vars;
    std::vector<double> reco_vars;
    bool out_of_range = false;
    // in true
    for (conv::KineVar_t kv : true_sb->KineVars()) {
      double val = kinematics.at(kv);
      if (val < true_sb->Minimum(kv) || val > true_sb->Maximum(kv)) {
        LOG("GenericSingleSampleFitModel",pWARN) 
          << "MC event: " << iev << " has true kinematic value outside range: "<<util::AsString(kv) << " = "<<val
          << ", range used: [" << true_sb->Minimum(kv) << ", " << true_sb->Maximum(kv) << "]";
        out_of_range = true;
      }
      true_vars.push_back(val);
    }
    // and in reco
    for (conv::KineVar_t kv : reco_ob->KineVars()) {
      double val = kinematics.at(kv);
      if (val < reco_ob->Minimum(kv) || val > reco_ob->Maximum(kv)) {
        if(es.ExperimentName() == "T2K"){
          // Loads of T2K events have Ereco values outside of the binning range. This is fine because studies have shown
          // that they're all intrinsic nue so we don't care about them
          SLOG("GenericSingleSampleFitModel",pDEBUG)
            << fName<<" > MC event: " << iev << " has reco kinematic value outside range: "<<util::AsString(kv) << " = "<<val
            << ", range used: [" << reco_ob->Minimum(kv) << ", " << reco_ob->Maximum(kv) << "]";
        }
        else {
          SLOG("GenericSingleSampleFitModel", pWARN)
            << fName << " > MC event: " << iev << " has reco kinematic value outside range: " << util::AsString(kv)
            << " = " << val
            << ", range used: [" << reco_ob->Minimum(kv) << ", " << reco_ob->Maximum(kv) << "]";
        }
        out_of_range = true;
      }
      reco_vars.push_back(val);
    }
    if (out_of_range) {continue;}
    
    // Exposure for assumed data - Weight MC templates to that exposure
    double data_pot = fPOT;

    // Calculate POT weight (assumed data exposure) / (generated MC exposure)
    double pot_wght = wght_pot * data_pot * tree_weight;
    
    if(fFiducialMass > 0) {
      pot_wght = pot_wght*fFiducialMass;
    }

    // Now check for weights from external tuning files
    // TODO: Currently we don't consider the nuclear target in FillFromDST, but if that changes the final argument in GetExternalWeight must be modified accordingly
    double ext_wght = GetExternalWeight(kinematics, m, conv::kntInclusive);

    // Find the appropriate bins
    
    int true_bin    = true_sb->FindBin(true_vars);
    int reco_bin_sb = reco_sb->FindBin(reco_vars);
    int reco_bin_ob = reco_ob->FindBin(reco_vars);

    LOG("GenericSingleSampleFitModel",pDEBUG) 
      << "Inserting to MC teplates --> MC event #" << iev 
      << "\n sample      = " << s        << " (" << util::AsString(s) << ")"
      << "\n beam        = " << beam     << " (" << util::AsString(b) << ")"
      << "\n imode       = " << imode    << " (" << util::AsString(m) << ")"
      << "\n wght        = " << wght 
      << "\n pot_wght    = " << pot_wght
      << "\n ext_wght    = " << ext_wght
      << "\n true_bin    = " << true_bin
      << "\n reco_bin_ob = " << reco_bin_ob
      << "\n reco_bin_sb = " << reco_bin_sb
      << "\n  True: "<<true_vars[0]<<" , "<<true_vars[1]
      << "\n  Reco: "<<reco_vars[0]<<" , "<<reco_vars[1];
      
    double this_entry = wght;
    
    this_entry *= pot_wght * ext_wght;
    
#ifdef APPLY_ALL_AUTOWGHT_BRANCHES
    // Apply the "autowght" variables
    for (const auto & namewght : wght_branches) {
      this_entry *= namewght.second;
    }
#endif

    // If necessary, get the weight from the systematic variations
    for (const std::string & name : fUniverseParametersToSave) {
      for (size_t universe_id = fFirstUniverseToSave ; universe_id <= fLastUniverseToSave ; universe_id++) {
        size_t universe_offset = universe_id - fFirstUniverseToSave;
        const std::vector<float> & universes = branch_universes.at(name);
        // Right now we'll sum up the weighted bin totals
        float universe_entry = this_entry*universes.at(universe_id);
        fUniverseVariations[name][imode].AddToEntry(
          universe_entry,
          reco_bin_ob, true_bin, universe_offset
        );
      }
    }

    // Add `nom' array entry (and init `tun', `sys' and `fin' arrays = `nom')
    double curr_wght = fMCTNomOb[imode]->GetEntry(reco_bin_ob, true_bin);
    curr_wght += this_entry;
    fMCTNomOb [imode]->SetEntry(curr_wght, reco_bin_ob, true_bin);
    fMCTTunOb [imode]->SetEntry(curr_wght, reco_bin_ob, true_bin);
    fMCTSysOb [imode]->SetEntry(curr_wght, reco_bin_ob, true_bin);
    fMCTFinOb [imode]->SetEntry(curr_wght, reco_bin_ob, true_bin);     
    fMCStatOb [imode]->SetEntry(        1, reco_bin_ob, true_bin);

    double curr_wght_sb = fMCTNomSb[imode]->GetEntry(reco_bin_sb, true_bin);
    curr_wght_sb += this_entry;
    fMCTNomSb [imode]->SetEntry(curr_wght_sb, reco_bin_sb, true_bin);
    fMCTTunSb [imode]->SetEntry(curr_wght_sb, reco_bin_sb, true_bin);
    fMCTSysSb [imode]->SetEntry(curr_wght_sb, reco_bin_sb, true_bin);
    fMCTFinSb [imode]->SetEntry(curr_wght_sb, reco_bin_sb, true_bin);
    fMCStatSb [imode]->SetEntry(           1, reco_bin_sb, true_bin);
  }//iev

  // If we're storing the weights from the universes, here we should
  // divide out the unweighted total to get back to a simple weight
  size_t n_universes_to_save = fLastUniverseToSave - fFirstUniverseToSave + 1;
  int n_reco_bins_ob = reco_ob->NBins();
  for (const std::string & name : fUniverseParametersToSave) {
    for (size_t universe_offset = 0 ; universe_offset < n_universes_to_save ; universe_offset++) {
      for(size_t imode = 0; imode < fReactionModes.size(); imode++){
        ReacMode_t m = fReactionModes[imode];
        if(m == kmUndefined) continue;
        const VALORBinning * true_sb = fBinning->TrueSmpBinning(m);
        const int n_true_bins = true_sb->NBins();

        for(int reco_bin_ob = 0 ; reco_bin_ob < n_reco_bins_ob ; reco_bin_ob++) {
          for (int true_bin = 0 ; true_bin < n_true_bins ; true_bin++) {
            float varied_entry = fUniverseVariations[name][imode].GetEntry(reco_bin_ob,true_bin,universe_offset);
            const float & orig_entry = fMCTFinOb [imode]->GetEntryRef(reco_bin_ob,true_bin);
            float universe_weight = 1.;
            if (std::abs(orig_entry) > conv::kASmallNum) {
              universe_weight = varied_entry/orig_entry;
            }
            fUniverseVariations[name][imode].SetEntry(universe_weight, reco_bin_ob,true_bin,universe_offset);
          }//true bins
        }//reco bins
      }//modes
    }//universe
  }//param

  LOG("GenericSingleSampleFitModel",pDEBUG)
    << "\n\nAfter filling MC templates from the input MC tree, the fMCTNomOb integrals for each mode are:";
  double fMCTNomOb_tot_integral = 0;
  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
    ReacMode_t m = fReactionModes[imode];
    if(m == kmUndefined) continue;
    fMCTNomOb_tot_integral += fMCTNomOb[imode]->Integral();
    LOG("GenericSingleSampleFitModel",pDEBUG)
      << "fMCTFinOb[" << util::AsString(m) << "] integral = " << fMCTNomOb[imode]->Integral();
  }
  LOG("GenericSingleSampleFitModel",pDEBUG) << "fMCTNomOb integral summed over all modes = " << fMCTNomOb_tot_integral;
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::ReadTune(
    TXMLEngine* xmleng,
    XMLNodePointer_t top_node,
    std::vector<TuningInfo> * hist_vec,
    Option_t * /*opt*/)
{
  LOG("GenericSingleSampleFitModel", pNOTICE)
    << "Will apply tune with name [" << xmleng->GetAttr(top_node,"name") << "]";

  conv::ApplyMask_t tuning_apply_mask = util::ReadApplyMaskAll(xmleng, top_node);
  std::vector<conv::KineVar_t> kvs;
  TH1* tuning_hist = nullptr;

  std::vector<string> result;
  string filename      = "";
  string objname       = "";
  string objtype       = "";
  string kinevars_str  = "";

  XMLNodePointer_t curr_node = xmleng->GetChild(top_node);

  while(curr_node) {
      string curr_node_name = xmleng->GetNodeName(curr_node);
      if(xmleng->GetNodeContent(curr_node)) {
        string content = util::RemoveAllWhitespace(xmleng->GetNodeContent(curr_node));
        LOG("GenericSingleSampleFitModel", pINFO) << curr_node_name << " --> " << content;
        if(curr_node_name == "filename"      ) { filename      = content; }
        if(curr_node_name == "objname"       ) { objname       = content; }
        if(curr_node_name == "objtype"       ) { objtype       = content; }
        if(curr_node_name == "kinevars"      ) { kinevars_str  = content; }

      }
      curr_node = xmleng->GetNext(curr_node);
  }

  // Turn the kine var string into a vector of kine vars
  std::vector<string> kinevars_str_vec = util::Tokens(kinevars_str, ",");
  for(string kv_string : kinevars_str_vec) {
    conv::KineVar_t kv = util::KineVarFromString(kv_string);
    if(kv == conv::kvUndefined) {
      LOG("GenericSingleSampleFitModel", pFATAL) << "Could not convert input string [" << kv_string << "] to a kine var";
      util::Die(-1);
    }
    kvs.push_back(kv);
  }

  // Get the tuning hist
    if(util::ToLowerCase(objtype) == "th1d") { tuning_hist = util::XML2TH1D(xmleng, top_node, ""); }
    if(util::ToLowerCase(objtype) == "th2d") { tuning_hist = util::XML2TH2D(xmleng, top_node, ""); }
    if(util::ToLowerCase(objtype) == "th3d") { tuning_hist = util::XML2TH3D(xmleng, top_node, ""); }
    if(util::ToLowerCase(objtype) == "th1f") { tuning_hist = util::XML2TH1F(xmleng, top_node, ""); }

  if(!tuning_hist) {
    LOG("GenericSingleSampleFitModel", pFATAL)
      << "Failed to read tuning hist in node named [" << xmleng->GetAttr(top_node,"name") << "]";
    util::Die(-1);
  }
  LOG("GenericSingleSampleFitModel", pINFO)
    << "Found tuning hist named [" << tuning_hist->GetName() << "]";

  // Add the tuning hist, apply mask and kine vars to the vector
  std::pair<conv::ApplyMask_t, std::vector<conv::KineVar_t>> pair = std::make_pair(tuning_apply_mask, kvs);
  std::pair<std::pair<conv::ApplyMask_t, std::vector<conv::KineVar_t>>, TH1*> pairpair = std::make_pair(pair, tuning_hist);
  hist_vec->push_back(pairpair);
}
//________________________________________________________________________________________
double GenericSingleSampleFitModel::GetExternalWeight(std::map<KineVar_t,double> kinematics, conv::ReacMode_t m, conv::NuclearTarget_t nt)
{
  double wght_external = 1.;
  if(fEventByEventTuningHists.size() < 1) { return wght_external; }

  for(TuningInfo pairpair : fEventByEventTuningHists) {
    std::pair<conv::ApplyMask_t, std::vector<conv::KineVar_t> > apply_pair = pairpair.first;
    TH1* tuning_hist = pairpair.second;
    if(!tuning_hist) { LOG("GenericSingleSampleFitModel", pERROR) << "Tuning apply hist not found!"; util::Die(-1); }

    conv::ApplyMask_t            tuning_apply_mask = apply_pair.first;
    std::vector<conv::KineVar_t> tuning_kine_vars  = apply_pair.second;
    if(tuning_kine_vars.size() == 0) { LOG("GenericSingleSampleFitModel", pERROR) << "Tuning kine vars not found!" ; util::Die(-1); }
    if((int)tuning_kine_vars.size() != tuning_hist->GetDimension()) {
      LOG("GenericSingleSampleFitModel", pERROR) << "Tuning kine vars size not equal to the number of tuning hist dimensions!";
      util::Die(-1);
    }

    // Does this tuning hist apply to this event?
    bool do_apply = util::IsAppliedTo(tuning_apply_mask, fBeam, fSample, fDetector, m, nt);
    LOG("GenericSingleSampleFitModel", pDEBUG) << "Apply tuning hist to event with: " << util::AsString(fBeam) << ", "
                                               << util::AsString(fSample) << ", " << util::AsString(m) << ", "
                                               << util::AsString(nt) << "? " << do_apply;
    if(!do_apply) { continue; }

    // Does this tuning hist apply to the kinematics considered?
    bool kinevar_mismatch = false;
    for(conv::KineVar_t kv : tuning_kine_vars) {
      if(kinematics.find(kv) == kinematics.end()) { kinevar_mismatch = true; }
    }
    if(kinevar_mismatch) { continue; }

    // Get the weight from the tuning hist
    // Assume that the order of kine vars in tuning_kine_vars corresponds to the relevant dimension of the tuning hist
    int global_bin = 0;
    if     (tuning_kine_vars.size() == 1) { global_bin = tuning_hist->FindBin(kinematics.at(tuning_kine_vars[0])); }
    else if(tuning_kine_vars.size() == 2) { global_bin = tuning_hist->FindBin(kinematics.at(tuning_kine_vars[0]),
                                                                              kinematics.at(tuning_kine_vars[1])); }
    else if(tuning_kine_vars.size() == 3) { global_bin = tuning_hist->FindBin(kinematics.at(tuning_kine_vars[0]),
                                                                              kinematics.at(tuning_kine_vars[1]),
                                                                              kinematics.at(tuning_kine_vars[2])); }
    if(tuning_hist->IsBinOverflow(global_bin) || tuning_hist->IsBinUnderflow(global_bin)) { continue; }
    LOG("GenericSingleSampleFitModel", pDEBUG) << "Getting weight from tuning hist bin " << global_bin
                                               << " with content " << tuning_hist->GetBinContent(global_bin);
    wght_external *= tuning_hist->GetBinContent(global_bin);
  }

  return wght_external;
}

//________________________________________________________________________________________

bool GenericSingleSampleFitModel::FillFromTemplateFile (TFile * f, string tag, double desired_POT)
{
  bool success = true;

  //~ BeamConf_t b = fBeam;
  //~ Sample_t   s = fSample;
  
  if (desired_POT > 0) {
    LOG("GenericSingleSampleFitModel",pNOTICE) << "Scaling POT template by "<<desired_POT * 1E-21 << " to get template for "<<desired_POT<<" POT.";
  }

  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
    ReacMode_t m = fReactionModes[imode];
    if(m == kmUndefined) continue;
    TString obj_name_ob = TString::Format("%s_%s_%s", tag.c_str(), "mctnom_ob_vf", fName.c_str());
    VALORArray * arr_ob = dynamic_cast<VALORArray*>(f->Get(obj_name_ob.Data()));
    if (arr_ob && arr_ob->GetSize()) {
      assert(arr_ob->GetDimensionSizes() == fMCTNomOb[imode]->GetDimensionSizes());
      assert(arr_ob->GetDimensionSizes() == fMCTTunOb[imode]->GetDimensionSizes());
      assert(arr_ob->GetDimensionSizes() == fMCTSysOb[imode]->GetDimensionSizes());
      assert(arr_ob->GetDimensionSizes() == fMCTFinOb[imode]->GetDimensionSizes());
      assert(arr_ob->GetDimensionSizes() == fMCStatOb[imode]->GetDimensionSizes());
      if (desired_POT > 0) {
        arr_ob->Scale(desired_POT * 1E-21);
      }
      fMCTNomOb [imode]->CopyFrom(*arr_ob);
      fMCTTunOb [imode]->CopyFrom(*arr_ob);
      fMCTSysOb [imode]->CopyFrom(*arr_ob);
      fMCTFinOb [imode]->CopyFrom(*arr_ob);
      fMCStatOb [imode]->CopyFrom(*arr_ob);
      fMCStatOb [imode]->SetAll(1.);  
      delete arr_ob;
    }
    else {
      success = false;
      LOG("GenericSingleSampleFitModel",pERROR) << "Failed to read template "<<obj_name_ob;
    }
    
    TString obj_name_sb = TString::Format("%s_%s_%s", tag.c_str(), "mctnom_sb_vf", fName.c_str());
    VALORArray * arr_sb = dynamic_cast<VALORArray*>(f->Get(obj_name_sb.Data()));
    if (arr_sb && arr_sb->GetSize()) {
      assert(arr_sb->GetDimensionSizes() == fMCTNomSb[imode]->GetDimensionSizes());
      assert(arr_sb->GetDimensionSizes() == fMCTTunSb[imode]->GetDimensionSizes());
      assert(arr_sb->GetDimensionSizes() == fMCTSysSb[imode]->GetDimensionSizes());
      assert(arr_sb->GetDimensionSizes() == fMCTFinSb[imode]->GetDimensionSizes());
      assert(arr_sb->GetDimensionSizes() == fMCStatSb[imode]->GetDimensionSizes());
      if (desired_POT > 0) {
          arr_sb->Scale(desired_POT * 1E-21);
      }
      fMCTNomSb [imode]->CopyFrom(*arr_sb);
      fMCTTunSb [imode]->CopyFrom(*arr_sb);
      fMCTSysSb [imode]->CopyFrom(*arr_sb);
      fMCTFinSb [imode]->CopyFrom(*arr_sb);
      fMCStatSb [imode]->CopyFrom(*arr_sb);
      fMCStatSb [imode]->SetAll(1.);
      delete arr_sb;
    }
    else {
      success = false;
      LOG("GenericSingleSampleFitModel",pERROR) << "Failed to read template "<<obj_name_sb;
    }
  } //imode

  if (!success) {
    util::Die(-1);
  }
  return success;
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::ApplyTuning(Option_t * /*opt*/)
{
  // This method maps nom -> tun MC templates
  // This is applied once when the model is constructed, not everytime Generate() is called.

  // No bin-by-bin tuning is supported yet (but event-by-event tuning is supported in FillFromDST).
  // If bin-by-bin tuning is needed, slight modification to ApplyBinnedRatioTuningToTemplates (currently unused) is
  // needed (see the notes within that function).

  if(fBinByBinTuningHists.size() > 0) {
    LOG("GenericSingleSampleFitModel",pFATAL) << "Bin-by-bin tuning is not yet implemented.";
    util::Die(-1);
  }

}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::ApplyBinnedRatioTuningToTemplates (std::vector<TuningInfo> & numerator_vec,
    std::vector<TuningInfo> & denominator_vec, std::vector<std::string> templates_to_update)
{
  // TODO: This needs slightly modifying (and renaming) so that bin-by-bin tuning can be applied. Currently this
  // TODO: function assumes that you've got 2 input vector<TuningInfo> and then their ratio is taken as weights. Instead
  // TODO: just take in a single vector<TuningInfo>, and put in an operation type (multiply or divide) into the XML files

  if(numerator_vec.size() != denominator_vec.size()) {
    LOG("GenericSingleSampleFitModel",pFATAL)
      << "Different number of tuning hists in the numerator and denominator vectors. This should not happen!";
    util::Die(-1);
  }

  for (size_t itune = 0; itune < numerator_vec.size(); itune++) {
    // Unpack the tuning information
    TuningInfo & tune_numerator   = numerator_vec  [itune];
    TuningInfo & tune_denominator = denominator_vec[itune];
    std::pair<conv::ApplyMask_t, std::vector<conv::KineVar_t> > pair_numerator   = tune_numerator  .first;
    std::pair<conv::ApplyMask_t, std::vector<conv::KineVar_t> > pair_denominator = tune_denominator.first;
    TH1* hist_numerator   = tune_numerator  .second;
    TH1* hist_denominator = tune_denominator.second;
    conv::ApplyMask_t mask_numerator   = pair_numerator  .first;
    conv::ApplyMask_t mask_denominator = pair_denominator.first;

    // Get the apply mask for this tune to see whether we should apply it here
    std::map<conv::ReacMode_t, bool> apply_to_mode;
    bool apply_this_tune = false;
    for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
      ReacMode_t m = fReactionModes[imode];
      if(m == kmUndefined) continue;
      bool apply_numerator   = util::IsAppliedTo(mask_numerator  , fBeam, fSample, fDetector, m, conv::kntInclusive);   // TODO: Add support for different nuclear targets?
      bool apply_denominator = util::IsAppliedTo(mask_denominator, fBeam, fSample, fDetector, m, conv::kntInclusive);
      bool apply = false;
      if ((apply_numerator == apply_denominator) && apply_numerator) { apply = true; }
      apply_to_mode[m] = apply;
      apply_this_tune = apply_this_tune || apply;
    }
    if(!apply_this_tune) { continue; }

    // Get the kine vars that this tune applies to to see whether we should apply it here
    std::vector<conv::KineVar_t> kvs_numerator   = pair_numerator  .second;
    std::vector<conv::KineVar_t> kvs_denominator = pair_denominator.second;
    const std::vector<conv::KineVar_t> & binning_kvs = fBinning->KineVars();
    bool kinevar_mismatch = false;
    for(conv::KineVar_t kv : kvs_numerator) {
      if(std::find(binning_kvs.begin(), binning_kvs.end(), kv) == binning_kvs.end()) { kinevar_mismatch = true; }
    }
    if(kvs_numerator.size() != kvs_denominator.size()) { kinevar_mismatch = true; }
    for(conv::KineVar_t kv_numerator : kvs_numerator) {
      if(std::find(kvs_denominator.begin(), kvs_denominator.end(), kv_numerator) == kvs_denominator.end()) { kinevar_mismatch = true; }
    }
    if(kinevar_mismatch) { continue; }

    if(kvs_numerator.size() == 0) { LOG("GenericSingleSampleFitModel", pERROR) << "Tuning kine vars not found!" ; util::Die(-1); }
    if((int)kvs_numerator.size() != hist_numerator->GetDimension()) {
      LOG("GenericSingleSampleFitModel", pERROR) << "Tuning kine vars size not equal to the number of tuning hist dimensions!";
      util::Die(-1);
    }
    if(hist_denominator->GetDimension() != hist_numerator->GetDimension()) {
      LOG("GenericSingleSampleFitModel", pERROR) << "Numerator and denominator hists should have the same number of dimensions!";
      util::Die(-1);
    }

    // Apply the bin-by-bin tune
    LOG("GenericSingleSampleFitModel",pDEBUG)
      << "About to apply bin-by-bin tuning from hist " << hist_numerator->GetName() << ", and apply mask "
      << util::AsString(mask_numerator) << " to modes:";
    for(std::map<conv::ReacMode_t, bool>::iterator iter = apply_to_mode.begin(); iter != apply_to_mode.end(); ++iter) {
      conv::ReacMode_t map_mode = iter->first;
      bool map_apply = iter->second;
      LOG("GenericSingleSampleFitModel", pDEBUG) << util::AsString(map_mode) << " : " << map_apply;
    }
    this->ApplyBinnedRatioTuningToTemplates(hist_numerator, hist_denominator, apply_to_mode, kvs_numerator, templates_to_update, false);
    this->ApplyBinnedRatioTuningToTemplates(hist_numerator, hist_denominator, apply_to_mode, kvs_numerator, templates_to_update, true);
  }

}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::ApplyBinnedRatioTuningToTemplates (TH1* numerator_hist, TH1* denominator_hist,
    std::map<conv::ReacMode_t, bool> apply_to_mode, std::vector<conv::KineVar_t> tuning_kine_vars,
    std::vector<std::string> templates_to_update, bool use_simple_binning)
{
  // TODO: This needs slightly modifying (and renaming) so that bin-by-bin tuning can be applied. Currently this
  // TODO: function assumes that you've got 2 input hists and then their ratio is taken as weights. Instead
  // TODO: just take in a single hist, and put in an operation type (multiply or divide) into the XML files

  // TODO: This function needs some efficiency improvements, as it's too slow right now. Things to try:
  // TODO: 1) Weights hist -> VALOR array
  // TODO: 2) Just after (or during) filling nominal MC templates, store whether or not the MCT bin is empty in a VALOR array
  // TODO: 3) If the weights hist only depends on true kvs (as in T2K), then read the weight outside of the reco loop

  // Create a single weights hist from numerator_hist and denominator_hist
  std::string new_name = (std::string) numerator_hist->GetName() + "_ratio_wght_hist";
  TH1* wght_hist = (TH1*) numerator_hist->Clone(new_name.c_str());
  wght_hist->Divide(denominator_hist);

  const VALORBinning * reco_binning = use_simple_binning ? fBinning->RecoSmpBinning() : fBinning->RecoOptBinning();
  const int n_reco_bins  = reco_binning->NBins();

  // Loop over all true and reco bins. Calculate and apply the weight to each bin.
  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
    ReacMode_t m = fReactionModes[imode];
    if(m == kmUndefined) continue;
    if(!apply_to_mode.at(m)) continue;
    bool is_null = fMCTNullOb[imode];
    if(is_null) continue;

    const VALORBinning * true_binning = fBinning->TrueSmpBinning(m);
    const int n_true_bins  = true_binning->NBins();
    for(int itrue = 0; itrue < n_true_bins; itrue++) {
      for(int ireco = 0; ireco < n_reco_bins; ireco++) {

        // Get the MC template bin center at each kv
        std::vector<double> mct_bin_centers;
        for (conv::KineVar_t kv : tuning_kine_vars) {
          double bin_center = 0.;
          if(util::IsTrue(kv)) { bin_center = true_binning->BinCentre(itrue, kv); }
          else                 { bin_center = reco_binning->BinCentre(ireco, kv); }
          mct_bin_centers.push_back(bin_center);
        }

        // Get the weight from the ratio hist
        // Assume that the order of kine vars in tuning_kine_vars corresponds to the relevant dimension of the tuning hist
        int global_bin = 0;
        if     (tuning_kine_vars.size() == 1) { global_bin = wght_hist->FindBin(mct_bin_centers[0]); }
        else if(tuning_kine_vars.size() == 2) { global_bin = wght_hist->FindBin(mct_bin_centers[0],
                                                                                mct_bin_centers[1]); }
        else if(tuning_kine_vars.size() == 3) { global_bin = wght_hist->FindBin(mct_bin_centers[0],
                                                                                mct_bin_centers[1],
                                                                                mct_bin_centers[2]); }
        if(wght_hist->IsBinOverflow(global_bin) || wght_hist->IsBinUnderflow(global_bin)) { continue; }
        double wght = wght_hist->GetBinContent(global_bin);
        if(wght < 0) continue;
        if(wght < conv::kASmallNum) wght = 1.;
        LOG("GenericSingleSampleFitModel", pDEBUG) << "Getting ratio weight from tuning hist bin " << global_bin
                                                   << " with content " << wght
                                                   << "; Applying to MCT bin ("<<ireco<<", "<<itrue<<") with old content = "
                                                   << fMCTTunSb[imode]->GetEntry(ireco, itrue) << ", new content = "
                                                   << fMCTTunSb[imode]->GetEntry(ireco, itrue) * wght;

        // Apply the weight to all requested templates
        for(std::string template_name : templates_to_update) {
          if(use_simple_binning){
            if     (template_name == "Tun") { fMCTTunSb[imode]->GetEntryRef(ireco, itrue) *= wght; }
            else if(template_name == "Sys") { fMCTSysSb[imode]->GetEntryRef(ireco, itrue) *= wght; }
            else if(template_name == "Fin") { fMCTFinSb[imode]->GetEntryRef(ireco, itrue) *= wght; }
          }
          else {
            if     (template_name == "Tun") { fMCTTunOb[imode]->GetEntryRef(ireco, itrue) *= wght; }
            else if(template_name == "Sys") { fMCTSysOb[imode]->GetEntryRef(ireco, itrue) *= wght; }
            else if(template_name == "Fin") { fMCTFinOb[imode]->GetEntryRef(ireco, itrue) *= wght; }
          }
        }//template_name: name of the MC template to update
      }//ireco: reco bins
    }//itrue: true bins
  }//imode: reac mode
}
//________________________________________________________________________________________

void GenericSingleSampleFitModel::FluctuateMCStat(Option_t * option)
{
  // Take into account the effect of MC statistics on the tuned MC templates.
  // Note that this is the error due to the finite MC statistics used to construct the
  // model and not the statistical error for the assumed (or actual) experimental exposure
  // which is also taken account when generating toy-MC experiments.
  // NOTE: For the time-being the effect applied on the tuned templates and stored on the 
  // tuned templates, so we lose the tuned templates as they were before applying the MC
  // statistical error. If this is important, we ought to create a new set of templates
  // to hold this intermediate step going from the `tun' to the `sys' templates.

  LOG("GenericSingleSampleFitModel",pINFO) 
    << "Applying MC statistic error (option: " 
    << ((option) ? option : "")
    << ")";

#ifdef _apply_mc_stat_fluctuation_

  bool update_mct_sb = false;
  if(option) {
    string opt = option;
    if(opt.find("+SB") != string::npos) {
      update_mct_sb = true;
    }
  }
  
  //Rnd * rnd = Rnd::Instance()->RndToyMCstat().Poisson(nmc) : nmc;
  Rnd * rnd = Rnd::Instance();
  
  const VALORBinning * reco_ob = fBinning->RecoOptBinning();
  const VALORBinning * reco_sb = fBinning->RecoSmpBinning();

  const int n_reco_bins_ob  = reco_ob->NBins();
  const int n_reco_bins_sb  = reco_sb->NBins();
  
  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {

    ReacMode_t m = fReactionModes[imode];
    if(m == kmUndefined) continue;
    const VALORBinning * true_sb = fBinning->TrueSmpBinning(m);
    
    const int n_true_bins  = true_sb->NBins();

    // Updating only the templates using the optimized binning
    if(!update_mct_sb) {
      bool is_null = fMCTNullOb[imode];
      if(is_null) continue;
      for(int itrue = 0; itrue < n_true_bins; itrue++) {
        for(int ireco = 0; ier < n_reco_bins_ob; ier++) {
          double val  = fMCTTunOb[imode]->GetEntry(ireco,itrue);
          double nmc  = fMCStatOb[imode]->GetEntry(ireco,itrue);
          // Fluctuate MC statistics and get the tweak factor
          double nmcf = rnd->RndToyMCstat().Poisson(nmc); 
          //double twk  = (nmc > 0) ? nmcf/nmc ? 1;
          double twk  = (nmc > 0) ? nmcf/nmc : 1;
          // Apply the tweak factor in the template, obviously a small effect for 
          // large MC statistics. Again noting that this is not the statistical error
          // for the current exposure. That one will be applied later in toy-MC generation.
          double valf = val * twk; 
          fMCTTunOb[imode]->SetEntry(valf,ireco,itrue);
        }//ireco: reco bins
      }//itrue: true bins
    }

    // Updating both the templates in optimized and simple binning schemes.
    // They can not be fluctuated independently because the represent the exact same quantity
    // in two binning schemes. The sum of bin contents (for specific bin ranges) of templates
    // in the simple scheme should equal the contents of specific bins of templates in the optimized scheme...
    else {
      
      // Reset
      fMCTTunOb[imode]->setAll(0.);

      // Start by fluctuation the templates using the simple binning.
      // Then map each simple bin to an optimized bin and add to its contents. 
      for(int itrue = 0; itrue < n_true_bins; itrue++) {
        for(int ireco = 0; ireco < n_reco_bins; ireco++) {
          double val  = fMCTTunSb[imode]->GetEntry(ireco,itrue);
          double nmc  = fMCStatSb[imode]->GetEntry(ireco,itrue);
          double nmcf = rnd->RndToyMCstat().Poisson(nmc); 
          //double twk  = (nmc > 0) ? nmcf/nmc ? 1;
          double twk  = (nmc > 0) ? nmcf/nmc : 1;
          double valf = val * twk; 
          fMCTTunSb[imode]->SetEntry(valf,ireco,itrue);
          
          std::vector<double> reco_vars = reco_sb->BinCentre(ireco);
          int ireco_ob = reco_ob->FindBin(reco_vars);
          double valf_ob = fMCTTunOb[imode]->GetEntry(ireco_ob,itrue);
          valf_ob += valf;
          fMCTTunOb[imode]->SetEntry(valf_ob,ireco_ob,itrue);
        }//ireco: Ereco bins
      }//itrue: Etrue bins

    }//update_mct_sb

  }//imode

#else

  LOG("GenericSingleSampleFitModel",pINFO) << "MC statistic error *not* applied";

#endif

}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::ApplySystematics(Option_t * /*opt*/)
{
  // This method maps tun -> sys MC templates.
  // Applying detector systematics and uncorrelated flux/cross-section systematics.

  // NOTE: For the time-being, everything (flux, cross-sections, detector systematics)
  // are applied together at the next method, ApplyPhysics()
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::ApplyPhysics(Option_t * option)
{
  // This method maps sys -> fin MC templates.
  // Applying correlated flux and cross-section systematics

  LOG("GenericSingleSampleFitModel",pINFO) 
    << fName
    << " - Applying physics params (option: " 
    << ((option) ? option : "")
    << ")";
    
  bool update_mct_sb = false;
  if(option) {
    string opt = option;
    if(opt.find("+SB") != string::npos) {
      update_mct_sb = true;
    }
  }

#ifdef DEBUG_SINGLE_BIN
  double              debug_true0  = 1.101;   // For T2K this is Etrue      // Set to -999 to look at all bins for this kine var
  double              debug_reco0  = 1.101;   // For T2K this is Ereco
  double              debug_reco1  = 90.0;    // For T2K this is theta reco
  conv::Sample_t      debug_sample = conv::ks1Rmu;
  conv::BeamConf_t    debug_beam   = conv::kbJPARC_FHC;
  conv::ReacMode_t    debug_mode   = conv::kmNuMuCCQE;
  conv::PhysParamId_t debug_param  = util::NameRegistry::Instance()->PhysParamFromString("skdetfsi002");  // Or leave as conv::kPhypUndefined to look at all applied params
  if(fSample == debug_sample && fBeam == debug_beam) {
    LOG("GenericSingleSampleFitModel",pNOTICE)
      << "Will print weights info for true var 0 = " << debug_true0 << ", reco var 0 = " << debug_reco0
      << ", reco var 1 = " << debug_reco1 << ", beam = " << util::AsString(debug_beam)
      << ", sample = " << util::AsString(debug_sample) << ", mode = " << util::AsString(debug_mode);
  }
#endif

  const VALORBinning * reco_ob = fBinning->RecoOptBinning();
  const VALORBinning * reco_sb = fBinning->RecoSmpBinning();
  
  const int n_reco_bins_sb  = reco_sb->NBins();
  const int n_reco_bins_ob  = reco_ob->NBins();
  const KineSpace_t reco_ks = reco_ob->KineSpace();
  const std::vector<conv::KineVar_t> reco_kinevars = util::KineVarReco(reco_ks);

  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
    ReacMode_t m = fReactionModes[imode];
    if(m == kmUndefined) continue;
    
    const VALORBinning * true_sb = fBinning->TrueSmpBinning(m);
    
    const int n_true_bins_sb = true_sb->NBins();
    const KineSpace_t true_ks = true_sb->KineSpace();
    const std::vector<conv::KineVar_t> true_kinevars = util::KineVarTrue(true_ks);

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
    LOG("GenericSingleSampleFitModel",pDEBUG)
      << "Handing templates for: "
      << util::AsString(fBeam) << ", " << util::AsString(fSample) << ", " << util::AsString(m);

    LOG("GenericSingleSampleFitModel",pDEBUG) 
      << "Initializing `fin' templates to the `sys' ones";
#endif
    bool is_null = fMCTNullOb[imode];
    if(is_null) {continue;}
    
    // copy sys -> fin MC templates
    fMCTFinOb[imode]->CopyDataOnlyFrom(*fMCTSysOb[imode]);

    // Recast VALORArray blocks so they can be used addressed like normal arrays (fast).
    // Hard to read, so should only be used in code for which speed is essential.
    typedef double (templtypeOb)[n_reco_bins_ob][n_true_bins_sb];
    templtypeOb & templFinOb = *reinterpret_cast<templtypeOb *>(&(fMCTFinOb[imode]->GetEntryRef(0,0)));

#ifdef DEBUG_SINGLE_BIN
    unsigned int debug_ireco = 999;
    unsigned int debug_itrue = 999;
    if(fSample == debug_sample && fBeam == debug_beam && m == debug_mode) {
      std::vector<double> debug_reco_vals;
      std::vector<double> debug_xsec_reco_vals;
      std::vector<double> debug_true_vals;
      debug_reco_vals.push_back(debug_reco0);
      if(util::NDimReco(reco_ks) > 1) { debug_reco_vals.push_back(debug_reco1); }
      debug_true_vals.push_back(debug_true0);
      debug_xsec_reco_vals.push_back(debug_reco0);
      debug_xsec_reco_vals.push_back(debug_reco1);
      debug_ireco = reco_ob->FindBin(debug_reco_vals);
      if (debug_true0 != -999) {
        debug_itrue = true_sb->FindBin(debug_true_vals);
      }
      else {
        debug_itrue = 0;
      }
      const VALORBinning * debug_xsec_true_binning = fBinning->XsecTrueBinning(m);
      const VALORBinning * debug_xsec_reco_binning = fBinning->XsecRecoBinning();
      double debug_xsec_ireco = debug_xsec_reco_binning->FindBin(debug_xsec_reco_vals);
      double debug_xsec_itrue = debug_xsec_true_binning->FindBin(debug_true_vals);
      if (debug_true0 != -999) {
        LOG("GenericSingleSampleFitModel", pNOTICE)
          << "At the start of apply physics, bin content = "
          << templFinOb[debug_ireco][debug_itrue] << ", debug_itrue = " << debug_itrue
          << ", debug_ireco = " << debug_ireco << ", debug_xsec_itrue = " << debug_xsec_itrue
          << ", debug_xsec_ireco = " << debug_xsec_ireco;
      }
      else {
        LOG("GenericSingleSampleFitModel", pNOTICE)
          << "At the start of apply physics, bin content (with true bin 0) = "
          << templFinOb[debug_ireco][0]
          << ", debug_ireco = " << debug_ireco
          << ", debug_xsec_ireco = " << debug_xsec_ireco;
      }
      LOG("GenericSingleSampleFitModel",pNOTICE) << "list of norm params applied to this mode:";
      for(size_t ip = 0; ip < fNormParamsAppliedTo[imode].size(); ip++) {
        PhysParamId_t p = fNormParamsAppliedTo[imode][ip];
        if (p == kPhypUndefined          ) {continue;}
        LOG("GenericSingleSampleFitModel",pNOTICE) << "  " << util::NameRegistry::Instance()->ParamAsString(p);
      }
      LOG("GenericSingleSampleFitModel",pNOTICE) << "list of spline params applied to this mode:";
      for(size_t ip = 0; ip < fSplineParamsAppliedTo[imode].size(); ip++) {
        PhysParamId_t p = fSplineParamsAppliedTo[imode][ip];
        if (p == kPhypUndefined          ) {continue;}
        LOG("GenericSingleSampleFitModel",pNOTICE) << "  " << util::NameRegistry::Instance()->ParamAsString(p);
      }
    }
#endif

    // First do the normalisation parameters
    for(size_t ip = 0; ip < fNormParamsAppliedTo[imode].size(); ip++) {
      PhysParamId_t p = fNormParamsAppliedTo[imode][ip];
      if (p == kPhypUndefined          ) {continue;}
      assert(util::IsSystematic(p));
      const Parameter & param = ParameterPool::Instance()->GetParam(p);
      const std::string & param_name = param.GetName();

      if (!fPhysHypothesis.Contains(p)) {
        LOG("GenericSingleSampleFitModel",pINFO) << "Skipping parameter not defined in PhysHypothesis "<<param_name;
        continue;
      }
      
      bool has_kinematic_dependence = param.HasKinematicRange();
         
      double f = fPhysHypothesis.CurrValue(p);
      
      if (has_kinematic_dependence) {
#ifndef __LET_ME_DO_STUPID_STUFF_WITH_MY_NORMALISATIONS__
        f = TMath::Max(0.,f);
#else
        if(es.ExperimentName() == "T2K"){
          if(f < 0) {
            if(param.SearchCategories("int")) { f = fRFL->GetForceNegWghtTo(); }
          }
        }
        if(f < 0) {
          std::cerr << "Applying a negative normalisation for parameter "
                    << param_name << std::endl;
        }
#endif
        bool tweaked = TMath::Abs(f-1) > kASmallNum;    // TODO: put in the untuned value instead of 1
#ifdef DEBUG_SINGLE_BIN
        if(fSample == debug_sample && fBeam == debug_beam && m == debug_mode && !tweaked && (debug_param == conv::kPhypUndefined || p == debug_param)) {
          LOG("GenericSingleSampleFitModel",pNOTICE)
            << "Skipping untweaked param [" << param_name
            << "] with hypothesis curr val = " << f;
        }
#endif
        if(!tweaked) continue;
        
        double true1_min  = 0.;
        double true1_max  = 0.;
        double true2_min  = 0.;
        double true2_max  = 0.;
        double reco1_min  = 0.;
        double reco1_max  = 0.;
        double reco2_min  = 0.;
        double reco2_max  = 0.;

        // Work out whether the parameter is applied to true, reco or both
        unsigned int param_true_kv_size = 0;
        unsigned int param_reco_kv_size = 0;

        // Syst params
        const std::vector<conv::KineVar_t> & kvs = param.GetKineVars();
        for (conv::KineVar_t kv : kvs) {
          if (util::IsTrue(kv)) param_true_kv_size++;
          else                  param_reco_kv_size++;
        }

        if(param_true_kv_size > true_kinevars.size()) {
          LOG("GenericSingleSampleFitModel",pWARN) << "Parameter is applied to more true kvs than is in the true binning";
          util::Die(-1); break;
        }
        if(param_reco_kv_size > reco_kinevars.size()) {
          LOG("GenericSingleSampleFitModel",pWARN) << "Parameter is applied to more true kvs than is in the true binning";
          util::Die(-1); break;
        }

        // Work out how many true and reco dimensions to apply this param to
        unsigned int apply_true_ndims = std::min(param_true_kv_size, (unsigned int)true_kinevars.size());
        unsigned int apply_reco_ndims = std::min(param_reco_kv_size, (unsigned int)reco_kinevars.size());

        // Get the true kine var ranges
        if (apply_true_ndims == 1) {
          const std::pair<double,double> & pair1 = param.GetKineRange(true_kinevars[0]);
          true1_min = pair1.first;
          true1_max = pair1.second;
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
          LOG("GenericSingleSampleFitModel",pINFO)
            << fName << ", " << util::AsString(m) << ": "
            << "Applying parameter: " << param_name << " with value = " << f
            << " in "  << util::AsString(true_kinevars[0]) << " bin range: [" << true1_min << ", " << true1_max << "]";
#endif
        }
        else if (apply_true_ndims == 2) {
          const std::pair<double,double> & pair1 = param.GetKineRange(true_kinevars[0]);
          const std::pair<double,double> & pair2 = param.GetKineRange(true_kinevars[1]);
          true1_min = pair1.first;
          true1_max = pair1.second;
          true2_min = pair2.first;
          true2_max = pair2.second;
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
          LOG("GenericSingleSampleFitModel",pINFO)
            << fName << ", " << util::AsString(m) << ": "
            << "Applying parameter: " << param_name << " with value = " << f
            << " in "  << util::AsString(true_kinevars[0]) << " bin range: [" << true1_min << ", " << true1_max << "]"
            << " and " << util::AsString(true_kinevars[1]) << " bin range: [" << true2_min << ", " << true2_max << "]";
#endif
        }
        else if (apply_true_ndims > 2) {
          LOG("GenericSingleSampleFitModel",pWARN) << "Unsupported true kinespace "<<util::AsString(true_ks);
          util::Die(-1); break;
        }

        // Get the reco kine var ranges
        if (apply_reco_ndims == 1) {
          const std::pair<double,double> & pair1 = param.GetKineRange(reco_kinevars[0]);
          reco1_min = pair1.first;
          reco1_max = pair1.second;
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
          LOG("GenericSingleSampleFitModel",pINFO)
            << fName << ", " << util::AsString(m) << ": "
            << "Applying parameter: " << param_name << " with value = " << f
            << " in "  << util::AsString(reco_kinevars[0]) << " bin range: [" << reco1_min << ", " << reco1_max << "]";
#endif
        }
        else if (apply_reco_ndims == 2) {
          const std::pair<double,double> & pair1 = param.GetKineRange(reco_kinevars[0]);
          const std::pair<double,double> & pair2 = param.GetKineRange(reco_kinevars[1]);
          reco1_min = pair1.first;
          reco1_max = pair1.second;
          reco2_min = pair2.first;
          reco2_max = pair2.second;
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
          LOG("GenericSingleSampleFitModel",pINFO)
            << fName << ", " << util::AsString(m) << ": "
            << "Applying parameter: " << param_name << " with value = " << f
            << " in "  << util::AsString(reco_kinevars[0]) << " bin range: [" << reco1_min << ", " << reco1_max << "]"
            << " and " << util::AsString(reco_kinevars[1]) << " bin range: [" << reco2_min << ", " << reco2_max << "]";
#endif
        }
        else if (apply_reco_ndims > 2) {
          LOG("GenericSingleSampleFitModel",pWARN) << "Unsupported true kinespace "<<util::AsString(true_ks);
          util::Die(-1); break;
        }

        int last_itrue = -1;
        double frac_true = 1.;
        for(int i_non_empty = 0; i_non_empty < fNumOfNonEmptyBinsOb[imode]; i_non_empty++) {
          // Only loop over non-empty MC template bins
          int itrue =  fNonEmptyBinsOb[imode][i_non_empty] % n_true_bins_sb;
          int ireco = (fNonEmptyBinsOb[imode][i_non_empty] - itrue) / n_true_bins_sb;

          // Determine how much the systematic bin and truth bin overlap so we know how 'much' of the parameter to apply
          if(itrue != last_itrue) {
            // Only calculate the frac_true when entering a new true bin
            frac_true = 1.;
            if (apply_true_ndims == 1 || apply_true_ndims == 2) {
              const vector<double> & true1_range (true_sb->BinRange(itrue,true_kinevars[0]));
              frac_true *= util::FractionOfBinContained(true1_range[0],true1_range[1],true1_min,true1_max);
            }
            if (apply_true_ndims == 2) {
              const vector<double> & true2_range (true_sb->BinRange(itrue,true_kinevars[1]));
              frac_true *= util::FractionOfBinContained(true2_range[0],true2_range[1],true2_min,true2_max);
            }
          }
          double frac_reco = 1.;
          if (apply_reco_ndims == 1 || apply_reco_ndims == 2) {
            const vector<double> & reco1_range (reco_ob->BinRange(ireco,reco_kinevars[0]));
            frac_reco *= util::FractionOfBinContained(reco1_range[0],reco1_range[1],reco1_min,reco1_max);
          }
          if (apply_reco_ndims == 2) {
            const vector<double> & reco2_range (reco_ob->BinRange(ireco,reco_kinevars[1]));
            frac_reco *= util::FractionOfBinContained(reco2_range[0],reco2_range[1],reco2_min,reco2_max);
          }

          double effective_tweak = frac_true * frac_reco * (f-1.) + 1;    // TODO: put in the untuned value here instead of 1? (in both occurrences)
          if (std::fabs(effective_tweak-1.) < conv::kASmallNum) {
#ifdef DEBUG_SINGLE_BIN
            if(fSample == debug_sample && fBeam == debug_beam && m == debug_mode && ireco == debug_ireco && itrue == debug_itrue  && (debug_param == conv::kPhypUndefined || p == debug_param)) {
                unsigned int use_debug_itrue = debug_itrue;
                if(debug_true0 == -999) { use_debug_itrue = itrue; }
                LOG("GenericSingleSampleFitModel",pNOTICE)
                  << "Skipping untweaked param [" << param_name
                  << "] with curr val = " << f << ", effective tweak = " << effective_tweak
                  << "(itrue = " << use_debug_itrue << ", ireco = " << ireco << ")";
              }
#endif
            continue;
          }

            double & val = templFinOb[ireco][itrue];
            val *= effective_tweak;
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
            LOG("GenericSingleSampleFitModel",pDEBUG)
              << "MCT[" << imode << "] = "
              << val/f << " --> " << val;
#endif
#ifdef DEBUG_SINGLE_BIN
          if(fSample == debug_sample && fBeam == debug_beam && m == debug_mode && ireco == debug_ireco && (itrue == debug_itrue || debug_true0 == -999) && (debug_param == conv::kPhypUndefined || p == debug_param)) {
              unsigned int use_debug_itrue = debug_itrue;
              if(debug_true0 == -999) { use_debug_itrue = itrue; }
              LOG("GenericSingleSampleFitModel",pNOTICE)
                << "Applying norm param with kinematic dependence [" << param_name
                << "] with curr val = " << f << ", effective tweak = " << effective_tweak
                << "; new bin content = " << templFinOb[debug_ireco][use_debug_itrue];
            }
#endif
          last_itrue = itrue;
        } // i non-empty bin
      }// has kinematic dependence

      // then apply the non-Etrue-dependent factors (use scale - not loop)
      else {
        double noEt = 1 ;
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
        LOG("GenericSingleSampleFitModel",pINFO)
          << fName <<": "
          << "Applying parameter: " << param_name << " with value = " << f << " to whole mode "<<util::AsString(m);
#endif
        noEt *= f;
#ifndef __LET_ME_DO_STUPID_STUFF_WITH_MY_NORMALISATIONS__
        noEt = TMath::Max(0.,noEt);
#else
        if(es.ExperimentName() == "T2K"){
          if(noEt < 0) {
            if(param.SearchCategories("int")) { noEt = fRFL->GetForceNegWghtTo(); }
          }
        }
        else {
          if(noEt < 0) {
          std::cerr << "Applying a negative normalisation for parameter "
                    << param_name << std::endl;
          }
        }

#endif
        for(int i_non_empty = 0; i_non_empty < fNumOfNonEmptyBinsOb[imode]; i_non_empty++) {
          // Only loop over non-empty MC template bins
          int itrue =  fNonEmptyBinsOb[imode][i_non_empty] % n_true_bins_sb;
          int ireco = (fNonEmptyBinsOb[imode][i_non_empty] - itrue) / n_true_bins_sb;
          templFinOb[ireco][itrue] *= noEt;
        }
#ifdef DEBUG_SINGLE_BIN
        if(fSample == debug_sample && fBeam == debug_beam && m == debug_mode && (debug_param == conv::kPhypUndefined || p == debug_param)) {
          LOG("GenericSingleSampleFitModel",pNOTICE)
            << "Applying norm param with NO kinematic dependence [" << param_name
            << "] with curr val = " << noEt
            << "; new bin content = " << templFinOb[debug_ireco][debug_itrue];
        }
#endif
      } // No spline, no kin dependence
    } //iparam (no splines)
    
    // Parameters with splines
#ifndef SPLINE_WEIGHTS_ARE_ONE
    for(unsigned int ip = 0; ip < fSplineParamsAppliedTo[imode].size(); ip++) {
      PhysParamId_t p = fSplineParamsAppliedTo[imode][ip];
      if (p == kPhypUndefined          ) {continue;}
      const Parameter & param = ParameterPool::Instance()->GetParam(p);
      const std::string & param_name = param.GetName();

      if (!fPhysHypothesis.Contains(p)) {
        LOG("GenericSingleSampleFitModel",pINFO) << "Skipping parameter not defined in PhysHypothesis "<<param_name;
        continue;
      }

      bool spline_is_applied = util::IsAppliedTo(param.GetApplyMask(),fBeam,fSample,fDetector,m,conv::kntInclusive); // TODO: Put in handling for nuclear targets
      if(!spline_is_applied) { continue; }

      double f = fPhysHypothesis.CurrValue(p);
      
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
        LOG("GenericSingleSampleFitModel",pDEBUG) 
          << "Applying parameter: " << param_name << " with value = " << f
          << " using splines.";
#endif
      double untuned = fSystematics->UntunedValue(p);
      const conv::SplEvalType_t & spline_eval_type = fRFL->GetSplineEvalType();
      if(spline_eval_type == conv::kSplEvalUndefined) {
        LOG("GenericSingleSampleFitModel",pFATAL) << "Spline evaluation type not defined for model " << this->Name();
        util::Die(-1);
      }
      VALORArrayBin splbin;
      splbin.bin3 = ip;

      int last_itrue = -1;
      for(int i_non_empty = 0; i_non_empty < fNumOfNonEmptyBinsOb[imode]; i_non_empty++) {
        // Only loop over non-empty MC template bins
        int itrue =  fNonEmptyBinsOb[imode][i_non_empty] % n_true_bins_sb;
        int ireco = (fNonEmptyBinsOb[imode][i_non_empty] - itrue) / n_true_bins_sb;

        if(itrue != last_itrue) { splbin.bin2 = itrue; }
        splbin.bin1 = ireco;

        double response = 1;
        ULong64_t spl_addr = fRFPtr[imode]->GetEntry(splbin);
        const util::Interpolator * spl = reinterpret_cast<util::Interpolator*>(spl_addr);
        if (spl==util::RFL::kNoSplineRequired) {
          LOG("GenericSingleSampleFitModel",pERROR)
            <<"Got no spline required for bin expecting spline."
            <<param_name<<", "<<util::AsString(fBeam)<<", "
            <<util::AsString(fSample)<<","<<util::AsString(m);
        }
        if (spl!=util::RFL::kFlatSpline) {
          if (spline_eval_type == conv::kSplEvalCurrTweak) {
            // Apply weights based on the spline evaluated at the current - untuned values
            response = spl->Eval(f - untuned);
          }
          else {
            // Default spline weighting method: evaluate splines at the current value
            response = spl->Eval(f);
          }

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
          LOG("GenericSingleSampleFitModel", pDEBUG)
              << "Got response = " << response << " for param = " << param_name
              << ", itrue = " << itrue << ", ireco = " << ireco << ", untuned = " << untuned
              << ", current = " << f << ", from spline with name [" << spl->GetName()
              << "] and title [" << spl->GetTitle() << "]";
#endif

          if (response<0) {
            LOG("GenericSingleSampleFitModel",pINFO)
              <<"Negative response from spline (OB) "//<<spl->GetName()
              <<" evaluated at "<<f<<" with result "<<response
              <<". Using " << fRFL->GetForceNegWghtTo() << " instead.";
            response = fRFL->GetForceNegWghtTo();
          }
        }
        double & val = templFinOb[ireco][itrue];
        val *= response;
#ifdef DEBUG_SINGLE_BIN
        if(fSample == debug_sample && fBeam == debug_beam && m == debug_mode && ireco == debug_ireco && (itrue == debug_itrue || debug_true0 == -999) && (debug_param == conv::kPhypUndefined || p == debug_param)) {
            unsigned int use_debug_itrue = debug_itrue;
            if(debug_true0 == -999) {
              use_debug_itrue = itrue;
              LOG("GenericSingleSampleFitModel",pNOTICE) << "True kinevar 0 = " << true_sb->BinCentre(use_debug_itrue, 0);
            }
            LOG("GenericSingleSampleFitModel",pNOTICE)
              << "Applying spline param [" << param_name
              << "] with curr val = " << f << ", untuned val = " << untuned
              << ", weight = " << response
              << "; new bin content = " << templFinOb[debug_ireco][use_debug_itrue]
              << ", from spline with name [" << spl->GetName() << "] and title [" << spl->GetTitle() << "]";
          }
#endif
        last_itrue = itrue;
      } // i non-empty bin
    } // iparams (splines)
#endif //SPLINE_WEIGHTS_ARE_ONE
    
// Same again but for simple binning
    if(update_mct_sb) {
      if (!imode) {
        LOG("GenericSingleSampleFitModel",pNOTICE) << "Updating simple binning templates for "<<fName<<".";
      }
      
      if(fMCTNullSb[imode]) {continue;}
      fMCTFinSb[imode]->CopyDataOnlyFrom(*(fMCTSysSb[imode]));

      // Recast VALORArray blocks so they can be used addressed like normal arrays (fast).
      // Hard to read, so should only be used in code for which speed is essential.
      typedef double (templtypeSb)[n_reco_bins_sb][n_true_bins_sb];
      templtypeSb & templFinSb = *reinterpret_cast<templtypeSb *>(&(fMCTFinSb[imode]->GetEntryRef(0,0)));

      // First do the normalisation parameters
      for(size_t ip = 0; ip < fNormParamsAppliedTo[imode].size(); ip++) {
        PhysParamId_t p = fNormParamsAppliedTo[imode][ip];
        if (p == kPhypUndefined          ) {continue;}
        assert(util::IsSystematic(p));
        const Parameter & param = ParameterPool::Instance()->GetParam(p);
        const std::string & param_name = param.GetName();

        if (!fPhysHypothesis.Contains(p)) {
          LOG("GenericSingleSampleFitModel",pINFO) << "Skipping parameter not defined in PhysHypothesis "<<param_name;
          continue;
        }

        assert(util::IsSystematic(p));

        bool has_kinematic_dependence = param.HasKinematicRange();
         
        double f = fPhysHypothesis.CurrValue(p);
        
        if (has_kinematic_dependence) {
#ifndef __LET_ME_DO_STUPID_STUFF_WITH_MY_NORMALISATIONS__
          f = TMath::Max(0.,f);
#else
          if(es.ExperimentName() == "T2K"){
            if(f < 0) {
              if(param.SearchCategories("int")) { f = fRFL->GetForceNegWghtTo(); }
            }
          }
          else {
            if(f < 0) {
              std::cerr << "Applying a negative normalisation for parameter "
                        << param_name << std::endl;
            }
          }
#endif
          bool tweaked = TMath::Abs(f-1) > kASmallNum;
          if(!tweaked) continue;
          
          double true1_min  = 0.;
          double true1_max  = 0.;
          double true2_min  = 0.;
          double true2_max  = 0.;
          double reco1_min  = 0.;
          double reco1_max  = 0.;
          double reco2_min  = 0.;
          double reco2_max  = 0.;

          // Work out whether the parameter is applied to true, reco or both
          unsigned int param_true_kv_size = 0;
          unsigned int param_reco_kv_size = 0;
          
          // Syst params
          const std::vector<conv::KineVar_t> & kvs = param.GetKineVars();
          for (conv::KineVar_t kv : kvs) {
            if (util::IsTrue(kv)) param_true_kv_size++;
            else                  param_reco_kv_size++;
          }

          if(param_true_kv_size > true_kinevars.size()) {
            LOG("GenericSingleSampleFitModel",pWARN) << "Parameter is applied to more true kvs than is in the true binning";
            util::Die(-1); break;
          }
          if(param_reco_kv_size > reco_kinevars.size()) {
            LOG("GenericSingleSampleFitModel",pWARN) << "Parameter is applied to more true kvs than is in the true binning";
            util::Die(-1); break;
          }

          // Work out how many true and reco dimensions to apply this param to
          unsigned int apply_true_ndims = std::min(param_true_kv_size, (unsigned int)true_kinevars.size());
          unsigned int apply_reco_ndims = std::min(param_reco_kv_size, (unsigned int)reco_kinevars.size());

          // Get the true kine var ranges
          if (apply_true_ndims == 1) {
            const std::pair<double,double> & pair1 = param.GetKineRange(true_kinevars[0]);
            true1_min = pair1.first;
            true1_max = pair1.second;
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
            LOG("GenericSingleSampleFitModel",pINFO)
              << fName << ", " << util::AsString(m) << ": "
              << "Applying parameter: " << param_name << " with value = " << f
              << " in "  << util::AsString(true_kinevars[0]) << " bin range: [" << true1_min << ", " << true1_max << "]";
#endif
          }
          else if (apply_true_ndims == 2) {
            const std::pair<double,double> & pair1 = param.GetKineRange(true_kinevars[0]);
            const std::pair<double,double> & pair2 = param.GetKineRange(true_kinevars[1]);
            true1_min = pair1.first;
            true1_max = pair1.second;
            true2_min = pair2.first;
            true2_max = pair2.second;
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
            LOG("GenericSingleSampleFitModel",pINFO)
              << fName << ", " << util::AsString(m) << ": "
              << "Applying parameter: " << param_name << " with value = " << f
              << " in "  << util::AsString(true_kinevars[0]) << " bin range: [" << true1_min << ", " << true1_max << "]"
              << " and " << util::AsString(true_kinevars[1]) << " bin range: [" << true2_min << ", " << true2_max << "]";
#endif
          }
          else if (apply_true_ndims > 2) {
            LOG("GenericSingleSampleFitModel",pWARN) << "Unsupported true kinespace "<<util::AsString(true_ks);
            util::Die(-1); break;
          }

          // Get the reco kine var ranges
          if (apply_reco_ndims == 1) {
            const std::pair<double,double> & pair1 = param.GetKineRange(reco_kinevars[0]);
            reco1_min = pair1.first;
            reco1_max = pair1.second;
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
            LOG("GenericSingleSampleFitModel",pINFO)
              << fName << ", " << util::AsString(m) << ": "
              << "Applying parameter: " << param_name << " with value = " << f
              << " in "  << util::AsString(reco_kinevars[0]) << " bin range: [" << reco1_min << ", " << reco1_max << "]";
#endif
          }
          else if (apply_reco_ndims == 2) {
            const std::pair<double,double> & pair1 = param.GetKineRange(reco_kinevars[0]);
            const std::pair<double,double> & pair2 = param.GetKineRange(reco_kinevars[1]);
            reco1_min = pair1.first;
            reco1_max = pair1.second;
            reco2_min = pair2.first;
            reco2_max = pair2.second;
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
            LOG("GenericSingleSampleFitModel",pINFO)
              << fName << ", " << util::AsString(m) << ": "
              << "Applying parameter: " << param_name << " with value = " << f
              << " in "  << util::AsString(reco_kinevars[0]) << " bin range: [" << reco1_min << ", " << reco1_max << "]"
              << " and " << util::AsString(reco_kinevars[1]) << " bin range: [" << reco2_min << ", " << reco2_max << "]";
#endif
          }
          else if (apply_reco_ndims > 2) {
            LOG("GenericSingleSampleFitModel",pWARN) << "Unsupported true kinespace "<<util::AsString(true_ks);
            util::Die(-1); break;
          }

          int last_itrue = -1;
          double frac_true = 1.;
          for(int i_non_empty = 0; i_non_empty < fNumOfNonEmptyBinsSb[imode]; i_non_empty++) {
            // Only loop over non-empty MC template bins
            int itrue =  fNonEmptyBinsSb[imode][i_non_empty] % n_true_bins_sb;
            int ireco = (fNonEmptyBinsSb[imode][i_non_empty] - itrue) / n_true_bins_sb;

            // Determine how much the systematic bin and truth bin overlap so we know how 'much' of the parameter to apply
            if(itrue != last_itrue) {
              // Only calculate the frac_true when entering a new true bin
              frac_true = 1.;
              if (apply_true_ndims == 1 || apply_true_ndims == 2) {
                const vector<double> & true1_range (true_sb->BinRange(itrue,true_kinevars[0]));
                frac_true *= util::FractionOfBinContained(true1_range[0],true1_range[1],true1_min,true1_max);
              }
              if (apply_true_ndims == 2) {
                const vector<double> & true2_range (true_sb->BinRange(itrue,true_kinevars[1]));
                frac_true *= util::FractionOfBinContained(true2_range[0],true2_range[1],true2_min,true2_max);
              }
            }
            double frac_reco = 1.;
            if (apply_reco_ndims == 1 || apply_reco_ndims == 2) {
              const vector<double> & reco1_range (reco_sb->BinRange(ireco,reco_kinevars[0]));
              frac_reco *= util::FractionOfBinContained(reco1_range[0],reco1_range[1],reco1_min,reco1_max);
            }
            if (apply_reco_ndims == 2) {
              const vector<double> & reco2_range (reco_sb->BinRange(ireco,reco_kinevars[1]));
              frac_reco *= util::FractionOfBinContained(reco2_range[0],reco2_range[1],reco2_min,reco2_max);
            }

            double effective_tweak = frac_true * frac_reco * (f-1.) + 1;    // TODO: put in the untuned value here instead of 1? (in both occurrences)
            if (std::fabs(effective_tweak-1.) < conv::kASmallNum) {
              continue;
            }

              double & val = templFinSb[ireco][itrue];
              val *= effective_tweak;
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
            LOG("GenericSingleSampleFitModel",pDEBUG)
              << "MCT[" << imode << "] = "
              << val/f << " --> " << val;
#endif
            last_itrue = itrue;
          } // i non-empty bin
        }// has kinematic dependence
        else {
          // then apply the non-Etrue-dependent factors (bundle them together to loop over the
          // Ereco,yreco,Etrue bins just once)
          double fnoEt = 1.;

#ifndef __LET_ME_DO_STUPID_STUFF_WITH_MY_NORMALISATIONS__
          f = TMath::Max(0.,f);
#else

          if(es.ExperimentName() == "T2K"){
            if(f < 0) {
              if(param.SearchCategories("int")) { f = fRFL->GetForceNegWghtTo(); }
            }
          }
          else {
            if(f < 0) {
              std::cerr << "Applying a negative normalisation for parameter "
                        << param_name << std::endl;
            }
          }
#endif

          bool tweaked = TMath::Abs(f-1) > kASmallNum;
          if(!tweaked) continue;
          fnoEt *= f;
#ifndef __LET_ME_DO_STUPID_STUFF_WITH_MY_NORMALISATIONS__
          fnoEt = TMath::Max(0.,fnoEt);
#else

          if(es.ExperimentName() == "T2K"){
            if(fnoEt < 0) {
              if(param.SearchCategories("int")) { fnoEt = fRFL->GetForceNegWghtTo(); }
            }
          }
          else {
            if(fnoEt < 0) {
              std::cerr << "Applying a negative normalisation for parameter "
                        << param_name << std::endl;
            }
          }
#endif

          for(int i_non_empty = 0; i_non_empty < fNumOfNonEmptyBinsSb[imode]; i_non_empty++) {
            // Only loop over non-empty MC template bins
            int itrue =  fNonEmptyBinsSb[imode][i_non_empty] % n_true_bins_sb;
            int ireco = (fNonEmptyBinsSb[imode][i_non_empty] - itrue) / n_true_bins_sb;
            templFinSb[ireco][itrue] *= fnoEt;
          }
        }// (no energy dependence)
      }//ip (normalisations)

#ifndef SPLINE_WEIGHTS_ARE_ONE
      for(unsigned int ip = 0; ip < fSplineParamsAppliedTo[imode].size(); ip++) {
        PhysParamId_t p = fSplineParamsAppliedTo[imode][ip];
        if (p == kPhypUndefined          ) {continue;}
        const Parameter & param = ParameterPool::Instance()->GetParam(p);
        const std::string & param_name = param.GetName();

        if (!fPhysHypothesis.Contains(p)) {
          LOG("GenericSingleSampleFitModel",pINFO) << "Skipping parameter not defined in PhysHypothesis "<<param_name;
          continue;
        }

        bool spline_is_applied = util::IsAppliedTo(param.GetApplyMask(),fBeam,fSample,fDetector,m,conv::kntInclusive); // TODO: Put in handling for nuclear targets
        if(!spline_is_applied) { continue; }

        double f = fPhysHypothesis.CurrValue(p);

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
        LOG("GenericSingleSampleFitModel",pDEBUG)
          << "Applying parameter: " << param_name << " with value = " << f
          << " using splines.";
#endif
        double untuned = fSystematics->UntunedValue(p);
        const conv::SplEvalType_t & spline_eval_type = fRFL->GetSplineEvalType();
        if(spline_eval_type == conv::kSplEvalUndefined) {
          LOG("GenericSingleSampleFitModel",pFATAL) << "Spline evaluation type not defined for model " << this->Name();
          util::Die(-1);
        }
        VALORArrayBin splbin;
        splbin.bin3 = ip;

        int last_itrue = -1;
        for(int i_non_empty = 0; i_non_empty < fNumOfNonEmptyBinsSb[imode]; i_non_empty++) {
          // Only loop over non-empty MC template bins
          int itrue =  fNonEmptyBinsSb[imode][i_non_empty] % n_true_bins_sb;
          int ireco = (fNonEmptyBinsSb[imode][i_non_empty] - itrue) / n_true_bins_sb;

          if(itrue != last_itrue) { splbin.bin2 = itrue; }
          splbin.bin1 = ireco;

          double response = 1;
          ULong64_t spl_addr = fRFPtr[imode]->GetEntry(splbin);
          const util::Interpolator * spl = reinterpret_cast<util::Interpolator*>(spl_addr);
          if (spl==util::RFL::kNoSplineRequired) {
            LOG("GenericSingleSampleFitModel",pERROR)
              <<"Got no spline required for bin expecting spline."
              <<param_name<<", "<<util::AsString(fBeam)<<", "
              <<util::AsString(fSample)<<","<<util::AsString(m);
          }
          if (spl!=util::RFL::kFlatSpline) {
            if (spline_eval_type == conv::kSplEvalCurrTweak) {
              // Apply weights based on the spline evaluated at the current - untuned values
              response = spl->Eval(f - untuned);
            }
            else {
              // Default spline weighting method: evaluate splines at the current value
              response = spl->Eval(f);
            }

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
            LOG("GenericSingleSampleFitModel", pDEBUG)
              << "Got response = " << response << " for param = " << param_name
              << ", itrue = " << itrue << ", ireco = " << ireco << ", untuned = " << untuned
              << ", current = " << f << ", from spline with name [" << spl->GetName()
              << "] and title [" << spl->GetTitle() << "]";
#endif

            if (response<0) {
              LOG("GenericSingleSampleFitModel",pINFO)
                <<"Negative response from spline (SB) "//<<spl->GetName()
                <<" evaluated at "<<f<<" with result "<<response
                <<". Using " << fRFL->GetForceNegWghtTo() << " instead.";
              response = fRFL->GetForceNegWghtTo();
            }
          }
          double & val = templFinSb[ireco][itrue];
          val *= response;
          last_itrue = itrue;
        } // i non-empty bin
      } //iparam (with splines)
#endif
        
    }//update_mct_sb?



    // If we need to apply universes, do so now (only to OB)
    if (fApplyUniverseVariations) {
      for (const auto & variation_name_and_id : fSystUniverseToApply) {
        const std::string & name = variation_name_and_id.first;
        const int & universe_id  = variation_name_and_id.second;
        size_t universe_offset = universe_id - fFirstUniverseToSave;
        for (int ireco = 0 ; ireco < n_reco_bins_ob ; ireco++) {
          for (int itrue = 0 ; itrue < n_true_bins_sb ; itrue++) {
            float & wght = fUniverseVariations[name][imode].GetEntryRef(ireco,itrue,universe_offset);
            double & val = templFinOb[ireco][itrue];
            val *= wght;
          } // true
        } //reco
      } // parameters to apply
    }
  }//imode
  if (update_mct_sb) {
    LOG("GenericSingleSampleFitModel",pINFO) << "Simple binning update completed.";
  }
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::ApplyOscProb(Option_t * option)
{
  // This method applies oscillation probabilities
  // from fin to fin templates                       
  // a la SuperKMC
  
  LOG("GenericSingleSampleFitModel",pINFO)
    << "Applying oscillation probabilities (option: "
    << ((option) ? option : "")
    << ")";
  
  bool update_mct_sb = false;
  if(option) {
    string opt = option;
    if(opt.find("+SB") != string::npos) {
      update_mct_sb = true;
    }
  }

  //first, check that there are actually osc params in the fit params list 
  bool apply_osc_prob = false;
  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
    for(int ip = 0; ip < conv::kNuis000; ip++) {
      PhysParamId_t p = (PhysParamId_t) ip;
      if(fPhysHypothesis.Contains(p)) {
        apply_osc_prob = true;
      }
    }
  }

  // If not, just set the oscillated modes to zero and move on
  if (!apply_osc_prob) {
    for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
      ReacMode_t m = fReactionModes[imode];
      if(m == kmUndefined) {continue;}
      conv::NuFlavour_t init_flav   = util::ReacModeToInitFlavor(m);
      conv::NuFlavour_t final_flav  = util::ReacModeToFinalFlavor(m);
      if (init_flav != final_flav) {
        fMCTFinOb[imode]->SetAll(0.);
        if (update_mct_sb) {fMCTFinSb[imode]->SetAll(0.);}
      }
    }
    return;
  }
  
  PhysHypothesis_t hypid = fPhysHypothesis.Hypothesis();
  // Variables for the sterile oscillations if we need them, since they don't come in an object
  double sinsq2thmue = -1, sinsq2thmumu = -1, dm41sq = -1;
  
  // Set 3 flavour osc parameters
  if (hypid == kHypOsc3F) {
    
    // Otherwise actually do the oscillation calculation
    //first find the baseline to be applied
    double baseline = util::Baseline(fBeam,fDetector);
    fNuOsc3Flavour.SetBaseline(baseline);
    
    // set oscillation parameters to the oscillation probability calculation code
    double mixing12 = fPhysHypothesis.CurrValue(kPhypMixing12);
    double mixing13 = fPhysHypothesis.CurrValue(kPhypMixing13);
    double mixing23 = fPhysHypothesis.CurrValue(kPhypMixing23);
    double dmsq     = fPhysHypothesis.CurrValue(kPhypDm12sq);
    double Dmsq     = fPhysHypothesis.CurrValue(kPhypDm23sq);
    double delta_cp = fPhysHypothesis.CurrValue(kPhypCPV    );

    NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
    NuOscDmsqScheme_t  dmsqscheme = Opt::Instance()->GetEffectiveDmsqScheme();

    // mixing params are: sin^2(2*theta)                                                                                        
    // independent squared mass splittings are: dm^2 = Dm^2_{12}, Dm^2 = |Dm^2_{23}|                                            
    if(dmsqscheme == kNuDmsq12andAbs23 && omp == kNuMixSinsq2Theta)
    {
      fNuOsc3Flavour.SetOscParameters(mixing12,mixing13,mixing23,dmsq,Dmsq,delta_cp);
    }
    // mixing params are: sin^2(theta)                                                                                     
    // independent squared mass splittings are: dm^2 = Dm^2_{12}, Dm^2 = |Dm^2_{23}|                                          
    else if(dmsqscheme == kNuDmsq12andAbs23 && omp == kNuMixSinsqTheta)
    {
      fNuOsc3Flavour.SetOscParametersSinSqTAll(mixing12,mixing13,mixing23,dmsq,Dmsq,delta_cp);
    }
    // mixing params are: sin^2(2*theta)                                                                                      
    // independent squared mass splittings are: dm^2 = Dm^2_{12}, Dm^2 = |m^2_{3} - (m^2_{1} + m^2_{1})/2|                   
    else if(dmsqscheme == kNuDmsqFogliLisi && omp == kNuMixSinsq2Theta)
    {
      fNuOsc3Flavour.SetOscParametersSSq2TDmAtmSol(mixing12,mixing13,mixing23,dmsq,Dmsq,delta_cp);
    }
    // mixing params are: sin^2(theta)                                                                                        
    // independent squared mass splittings are: dm^2 = Dm^2_{12}, Dm^2 = |m^2_{3} - (m^2_{1} + m^2_{1})/2|                    
    else if(dmsqscheme == kNuDmsqFogliLisi && omp == kNuMixSinsqTheta)
    {
      fNuOsc3Flavour.SetOscParametersDmsqAtmSol(mixing12,mixing13,mixing23,dmsq,Dmsq,delta_cp);
    }
    // mixing paras are: sin^2(2*theta)                                                                                     
    // independent squared mass splittings are: dm^2 = Dm^2_{12}, Dm^2 = |Dm^2_{31}|                                       
    else if(dmsqscheme == kNuDmsq12andAbs31 && omp == kNuMixSinsq2Theta)
    {
      fNuOsc3Flavour.SetOscParametersSSq2TDmsq13(mixing12,mixing13,mixing23,dmsq,Dmsq,delta_cp);
    }
    // mixing paras are: sin^2(theta)                                                                                        
    // independent squared mass splittings are: dm^2 = Dm^2_{12}, Dm^2 = |Dm^2_{31}|                                           
    else if(dmsqscheme == kNuDmsq12andAbs31 && omp == kNuMixSinsqTheta)
    {
      fNuOsc3Flavour.SetOscParametersDmsq13(mixing12,mixing13,mixing23,dmsq,Dmsq,delta_cp);
    }
    //unknown                                                                                                                    
    else {
      LOG("GenericSingleSampleFitModel",pFATAL) << "**  GenericSingleSampleFitModel >> Can't handle choice of "
           << "\n oscillation mixing params: " << util::AsString(omp)
           << "\n and independent squared mass-splittings: " << util::AsString(dmsqscheme);
      util::Die(-1);return;
    }
  }
  else if (hypid == kHypOscSterileEff) {
    sinsq2thmue  = fPhysHypothesis.CurrValue(kPhypMixingMuE);
    sinsq2thmumu = fPhysHypothesis.CurrValue(kPhypMixingMuMu);
    dm41sq       = fPhysHypothesis.CurrValue(kPhypDm41sq);
  }
    
  
  double p_numu_numu       = -1;
  double p_numu_nue        = -1;
  double p_numu_nutau      = -1;
  double p_numubar_numubar = -1;
  double p_numubar_nuebar  = -1;
  double p_numubar_nutaubar= -1;
  double p_nue_nue         = -1;
  double p_nue_numu        = -1;
  double p_nue_nutau       = -1;
  double p_nuebar_nuebar   = -1;
  double p_nuebar_numubar  = -1;
  double p_nuebar_nutaubar = -1;
  
  const VALORBinning * reco_ob = fBinning->RecoOptBinning();
  const VALORBinning * reco_sb = fBinning->RecoSmpBinning();
  
  const int n_reco_bins_ob = reco_ob->NBins();
  const int n_reco_bins_sb = reco_sb->NBins();

  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
    ReacMode_t m = fReactionModes[imode];
    if(m         == kmUndefined) continue;
    if(fDetector == kdUndefined) continue;
    if(fBeam     == kbUndefined) continue;
    if(fSample   == ksUndefined) continue;

    //check if oscillation probabilities should be applied to this combination of phys hypothesis and detector
    if(!fSkipOscProbApplyCheck) {
     if(!util::OscProbIsAppliedTo(hypid, fDetector)) {continue; }
    }

    const VALORBinning * true_sb = fBinning->TrueSmpBinning(m);
    const int n_true_bins_sb = true_sb->NBins();
    
    // Recast our VALORarrays as simple C-arrays to speed up the many rapid accesses this function needs
    typedef double (templtypeOb)[n_reco_bins_ob][n_true_bins_sb];
    templtypeOb & templFinOb = *reinterpret_cast<templtypeOb *>(&(fMCTFinOb[imode]->GetEntryRef(0,0)));
    typedef double (templtypeSb)[n_reco_bins_sb][n_true_bins_sb];
    templtypeSb & templFinSb = *reinterpret_cast<templtypeSb *>(&(fMCTFinSb[imode]->GetEntryRef(0,0)));

    for(int itrue = 0; itrue < n_true_bins_sb; itrue++) {
      //get the true energy to calculate osc prob 
      double Etrue = true_sb->BinCentre(itrue,conv::kvEtrue);
      
      if (hypid == kHypOsc3F) {
        double p_osc    [3][3];
        double p_osc_bar[3][3];
        fNuOsc3Flavour.CalcMatterOscProbAll(false, Etrue, p_osc    );
        fNuOsc3Flavour.CalcMatterOscProbAll(true , Etrue, p_osc_bar);
        p_numu_numu       = p_osc    [1][1];
        p_numu_nue        = p_osc    [1][0];
        p_numu_nutau      = p_osc    [1][2];
        p_numubar_numubar = p_osc_bar[1][1];
        p_numubar_nuebar  = p_osc_bar[1][0];
        p_numubar_nutaubar= p_osc_bar[1][2];
        p_nue_nue         = p_osc    [0][0];
        p_nue_numu        = p_osc    [0][1];
        p_nue_nutau       = p_osc    [0][2];
        p_nuebar_nuebar   = p_osc_bar[0][0];
        p_nuebar_numubar  = p_osc_bar[0][1];
        p_nuebar_nutaubar = p_osc_bar[0][2];
      
      }
      else if (hypid == kHypOscSterileEff) {
        double baseline = util::Baseline(fBeam,fDetector);
        double lOverE = baseline / Etrue;
        p_numu_numu       = util::Calculate2FSterileOscMuE(conv::kPdgNuMu   , conv::kPdgNuMu   , lOverE, sinsq2thmue, sinsq2thmumu, dm41sq);
        p_numu_nue        = util::Calculate2FSterileOscMuE(conv::kPdgNuMu   , conv::kPdgNuE    , lOverE, sinsq2thmue, sinsq2thmumu, dm41sq);
        p_numu_nutau      = 0.;
        p_numubar_numubar = util::Calculate2FSterileOscMuE(conv::kPdgNuMuBar, conv::kPdgNuMuBar, lOverE, sinsq2thmue, sinsq2thmumu, dm41sq);
        p_numubar_nuebar  = util::Calculate2FSterileOscMuE(conv::kPdgNuMuBar, conv::kPdgNuEBar , lOverE, sinsq2thmue, sinsq2thmumu, dm41sq);
        p_numubar_nutaubar= 0.;
        p_nue_nue         = util::Calculate2FSterileOscMuE(conv::kPdgNuE    , conv::kPdgNuE    , lOverE, sinsq2thmue, sinsq2thmumu, dm41sq);
        p_nue_numu        = util::Calculate2FSterileOscMuE(conv::kPdgNuE    , conv::kPdgNuMu   , lOverE, sinsq2thmue, sinsq2thmumu, dm41sq);
        p_nue_nutau       = 0;
        p_nuebar_nuebar   = util::Calculate2FSterileOscMuE(conv::kPdgNuEBar , conv::kPdgNuEBar , lOverE, sinsq2thmue, sinsq2thmumu, dm41sq);
        p_nuebar_numubar  = util::Calculate2FSterileOscMuE(conv::kPdgNuEBar , conv::kPdgNuMuBar, lOverE, sinsq2thmue, sinsq2thmumu, dm41sq);
        p_nuebar_nutaubar = 0;
      }
        

      double posc = 1.;
      if(util::IsCC(m))
      {
        if     ( util::IsNuMu(m)       ) { posc = p_numu_numu;       }
        else if( util::IsNuMuBar(m)    ) { posc = p_numubar_numubar; }
        else if( util::IsNuE(m)        ) { posc = p_nue_nue;         }
        else if( util::IsNuEBar(m)     ) { posc = p_nuebar_nuebar;   }

        else if( util::IsNuMuToNuE(m)  ) { posc = p_numu_nue;        }
        else if( util::IsNuMuToNuTau(m)) { posc = p_numu_nutau;      }
        else if( util::IsNuEToNuMu(m)  ) { posc = p_nue_numu;        }
        else if( util::IsNuEToNuTau(m) ) { posc = p_nue_nutau;       }

        else if( util::IsNuMuBarToNuEBar(m)  ) { posc = p_numubar_nuebar;        }
        else if( util::IsNuMuBarToNuTauBar(m)) { posc = p_numubar_nutaubar;      }
        else if( util::IsNuEBarToNuMuBar(m)  ) { posc = p_nuebar_numubar;        }
        else if( util::IsNuEBarToNuTauBar(m) ) { posc = p_nuebar_nutaubar;       }
      }
      
      if(fMCTNullOb[imode]) {continue;}
      
      for(int ireco_ob = 0; ireco_ob < n_reco_bins_ob; ireco_ob++) {
        templFinOb[ireco_ob][itrue] *= posc;
      }//ireco
        
      if(update_mct_sb) {
        if(fMCTNullSb[imode]) {continue;}
        for(int ireco_sb = 0; ireco_sb < n_reco_bins_sb; ireco_sb++) {
          templFinSb[ireco_sb][itrue] *= posc;
        }//ireco_sb
      }//update_mct_sb
    }//itrue
  }//imode

}
//________________________________________________________________________________________

void GenericSingleSampleFitModel::UpdatePDFs(Option_t * option)
{
  bool update_mct_sb = false;
  if(option) {
    string opt = option;
    if(opt.find("+SB") != string::npos) {
      update_mct_sb = true;
    }
  }

  //
  // Use the `fin' MC templates to fill in the PDF components
  // Integrate over true kinematical quantities (eg Etrue), but do not integrate
  // over true reaction modes just yet
  //

  const VALORBinning  * reco_ob = fBinning->RecoOptBinning();
  const VALORBinning * reco_sb = fBinning->RecoSmpBinning();

  const int n_reco_bins_ob  = reco_ob->NBins();
  const int n_reco_bins_sb  = reco_sb->NBins();

  // Recast VALORArray blocks so they can be used addressed like normal arrays (fast).
  // Hard to read, so should only be used in code for which speed is essential.
  typedef double (pdfTypeOb)[n_reco_bins_ob];
  typedef double (pdfTypeSb)[n_reco_bins_sb];
  pdfTypeOb & pdfTotObArr  = *reinterpret_cast<pdfTypeOb *>(&(fPDFTotOb->GetEntryRef(0)));
  pdfTypeSb & pdfTotSbArr  = *reinterpret_cast<pdfTypeSb *>(&(fPDFTotSb->GetEntryRef(0)));

  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
    ReacMode_t m = fReactionModes[imode];
    if(m == kmUndefined) continue;
    
    const VALORBinning * true_sb = fBinning->TrueSmpBinning(m);
    const int n_true_bins_sb = true_sb->NBins();

    // Recast VALORArray blocks so they can be used addressed like normal arrays (fast).
    // Hard to read, so should only be used in code for which speed is essential.
    typedef double (mctTypeOb)[n_reco_bins_ob][n_true_bins_sb];
    typedef double (mctTypeSb)[n_reco_bins_sb][n_true_bins_sb];
    pdfTypeOb & pdfCompObArr = *reinterpret_cast<pdfTypeOb *>(&(fPDFCompOb[imode]->GetEntryRef(0)));
    pdfTypeSb & pdfCompSbArr = *reinterpret_cast<pdfTypeSb *>(&(fPDFCompSb[imode]->GetEntryRef(0)));
    mctTypeOb & mctFinObArr  = *reinterpret_cast<mctTypeOb *>(&(fMCTFinOb [imode]->GetEntryRef(0,0)));
    mctTypeSb & mctFinSbArr  = *reinterpret_cast<mctTypeSb *>(&(fMCTFinSb [imode]->GetEntryRef(0,0)));

    if(fMCTNullOb[imode]) continue;
    
    for(int ireco=0; ireco < n_reco_bins_ob; ireco++) {
      double sum = 0.;
      for(int itrue=0; itrue < n_true_bins_sb; itrue++) {
        sum += mctFinObArr[ireco][itrue];
      }//itrue
      pdfCompObArr[ireco] = sum;
    }//ireco

    if(update_mct_sb) {
      bool is_null = fMCTNullSb[imode];
      if(is_null) continue;
      for(int ireco=0; ireco < n_reco_bins_sb; ireco++) {
        double sum = 0.;
        for(int itrue=0; itrue < n_true_bins_sb; itrue++) {
          sum += mctFinSbArr[ireco][itrue];
        }//itrue
        pdfCompSbArr[ireco] = sum;
      }//ireco
    }//update_mct_sb?

  }//imode

  //
  // Now integrate the above-computed PDF components over the true reaction mode
  // and calculate the total PDF
  //
    for(int ireco=0; ireco < n_reco_bins_ob; ireco++) {
      double sum = 0.;
      for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
        ReacMode_t m = fReactionModes[imode];
        if(m == kmUndefined) continue;
        if(fMCTNullOb[imode]) continue;
        pdfTypeOb & pdfCompObArr = *reinterpret_cast<pdfTypeOb *>(&(fPDFCompOb[imode]->GetEntryRef(0)));
        sum += pdfCompObArr[ireco];
      }//imode
      pdfTotObArr[ireco] = sum;
    }//ireco

  if(update_mct_sb) {
    for(int ireco=0; ireco < n_reco_bins_sb; ireco++) {
      double sum = 0.;
      for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
        ReacMode_t m = fReactionModes[imode];
        if(m == kmUndefined) continue;
        if(fMCTNullOb[imode]) continue;
        pdfTypeSb & pdfCompSbArr = *reinterpret_cast<pdfTypeSb *>(&(fPDFCompSb[imode]->GetEntryRef(0)));
        sum += pdfCompSbArr[ireco];
      }//imode
      pdfTotSbArr[ireco] = sum;
    }//ireco
  }//update_mct_sb?

  // Apply post-oscillation lateral systematics to each PDF (e.g. the energy scale systematic)
  this->ApplyPostOscLateralSystematics(fPDFTotOb, false);
  if(update_mct_sb) {
    this->ApplyPostOscLateralSystematics(fPDFTotSb, true);
  }
  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
    ReacMode_t m = fReactionModes[imode];
    if(m == kmUndefined) continue;
    if(fMCTNullOb[imode]) continue;
    this->ApplyPostOscLateralSystematics(fPDFCompOb[imode], false);
    if(update_mct_sb) {
      this->ApplyPostOscLateralSystematics(fPDFCompSb[imode], true);
    }
  }
}
//________________________________________________________________________________________
valor::PhysParamList valor::GenericSingleSampleFitModel::OneSigmaErrors(Option_t * /*opt*/)
{
  PhysParamList ppl;

  // -----------------------------------  
  // TODO
  // remove
  //ppl.SetName("NDModel_Errors");
  //fNuFlux->CopyOneSigmaErrorsTo(ppl);
  //fNuDet ->CopyOneSigmaErrorsTo(ppl);
  //fNuInt ->CopyOneSigmaErrorsTo(ppl);
  //--------------------------------------- 

  ppl.SetName("PreFitErrors");
  fSystematics->CopyOneSigmaErrorsTo(ppl);
  return ppl;
}
//________________________________________________________________________________________
void valor::GenericSingleSampleFitModel::UpdateObservableBinningFromSimple(Option_t * /*opt*/)
{
  LOG("GenericSingleSampleFitModel",pNOTICE)<<"Updating observable binning from simple: "<<fName;

  const VALORBinning * reco_ob = fBinning->RecoOptBinning();
  const VALORBinning * reco_sb = fBinning->RecoSmpBinning();

  const int n_reco_bins_ob  = reco_ob->NBins();
  const int n_reco_bins_sb  = reco_sb->NBins();
  
  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
    
    ReacMode_t m = fReactionModes[imode];
    if(m == kmUndefined) continue;
    
    const VALORBinning * true_sb = fBinning->TrueSmpBinning(m);
    const int n_true_bins_sb = true_sb->NBins();
    
    // Zero it out, since we'll just be adding to elements
    fMCTFinOb[imode]->SetAll(0.);
    
    // We'll use two nested sets of loops - first over the simple bins, then over the optimised bins to get their content
    for(int ireco_sb = 0; ireco_sb < n_reco_bins_sb; ireco_sb++) {
      // And optimised reco bins
      for(int ireco_ob = 0; ireco_ob < n_reco_bins_ob; ireco_ob++) {
        // Now loop over the truth bins
        for(int itrue_sb = 0; itrue_sb < n_true_bins_sb; itrue_sb++) {
          // Fraction of the observable bin held in the simple bin.
          const std::vector<double> & frac_sb_range1 = reco_sb->BinRange(ireco_sb,0);
          const std::vector<double> & frac_ob_range1 = reco_ob->BinRange(ireco_ob,0);
          double frac_sb_1 = util::FractionOfBinContained(frac_sb_range1[0],frac_sb_range1[1], frac_ob_range1[0],frac_ob_range1[1]);
          
          double frac_sb_2 = 1;
          if (reco_sb->NDimensions() > 1) {
            const std::vector<double> & frac_sb_range2 = reco_sb->BinRange(ireco_sb,1);
            const std::vector<double> & frac_ob_range2 = reco_ob->BinRange(ireco_ob,1);
            frac_sb_1 = util::FractionOfBinContained(frac_sb_range2[0],frac_sb_range2[1],frac_ob_range2[0],frac_ob_range2[1]);
          }
          
          double frac_sb = frac_sb_1 * frac_sb_2;
          if (frac_sb < conv::kASmallNum) {
            continue;
          }
          double sb_content = fMCTFinSb[imode]->GetEntry(ireco_sb,itrue_sb);
          double content_to_add = frac_sb * sb_content;
          fMCTFinOb[imode]->AddToEntry(content_to_add,ireco_ob,itrue_sb);
        }//itrue_sb
      }//ireco_ob
    }//ireco_sb
  } //imode
  return;
}
//________________________________________________________________________________________
CorrelatedSystematics * GenericSingleSampleFitModel::GetSystematics(Option_t * /*opt*/)
{
  return fSystematics;
}
//________________________________________________________________________________________
const CorrelatedSystematics * GenericSingleSampleFitModel::GetSystematics(Option_t * /*opt*/) const
{
  return fSystematics;
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::SetSystConf(CorrelatedSystematics * syst, Option_t * opt)
{
  TString ts(opt);
  if (ts.Contains("copy") ) {
    fSystematics = dynamic_cast<CorrelatedSystematics *>(syst->Clone());
    fOwnerOfSystematics = true;
  }
  else {
    fSystematics = syst;
    fOwnerOfSystematics = ts.Contains("adopt");
  }

  return;
}
//________________________________________________________________________________________
const util::RFL * GenericSingleSampleFitModel::GetRFL() const
{
  return fRFL;
}
//________________________________________________________________________________________
const SingleSampleKineBinning * GenericSingleSampleFitModel::GetBinning() const
{
  return fBinning;
}
//________________________________________________________________________________________
const std::vector<conv::ReacMode_t> GenericSingleSampleFitModel::GetReactionModes(Option_t * /*opt*/) const
{
  return fReactionModes;
}
//________________________________________________________________________________________
const double &  GenericSingleSampleFitModel::GetPOT() const
{
  return fPOT;
}
//________________________________________________________________________________________
const double & GenericSingleSampleFitModel::GetFiducialMass() const
{
  return fFiducialMass;
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::ApplyPostOscLateralSystematics(VALORArray * pdf, bool simple_binning)
{
  for(size_t ip = 0; ip < fLateralPostOscParams.size(); ip++) {
    PhysParamId_t p = fLateralPostOscParams[ip];
    if(p == kPhypUndefined) { continue; }
    this->ApplyLateralSystematic(pdf, p, simple_binning);
  }
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::ApplyLateralSystematic(VALORArray * pdf, conv::PhysParamId_t p, bool simple_binning)
{
  double orig_pdf_integral = pdf->Integral();
  if(orig_pdf_integral < conv::kASmallNum) {
    LOG("Lateral",pINFO) << "No need to apply lateral parameter to empty PDF " << pdf->GetName();
    return;
  }

  if (!fPhysHypothesis.Contains(p)) {
    LOG("Lateral",pINFO) << "Skipping lateral parameter not defined in PhysHypothesis: "
                         << util::NameRegistry::Instance()->ParamAsString(p);
    return;
  }

  const Parameter & param = ParameterPool::Instance()->GetParam(p);
  const std::string & param_name = param.GetName();
  if(!param.HasKinematicRange()) {
    LOG("Lateral",pFATAL) << "Lateral systematic parameter " << param_name
                          << " has no kinematic range. Cannot apply";
    util::Die(-1);
  }

  const std::vector<conv::KineVar_t> & kvs = param.GetKineVars();
  if(kvs.size() != 1) {
    LOG("Lateral",pFATAL) << "Lateral systematic parameter " << param_name
                          << " is applied to " << kvs.size() << " kinematic variables. Cannot apply";
    util::Die(-1);
  }
  const conv::KineVar_t & kv = kvs[0];
  if(!util::IsReco(kv)) {
    LOG("Lateral",pFATAL) << "Lateral systematic parameter " << param_name
                          << " is applied to " << util::AsString(kv)
                          << ", but only reco kinevars can be applied";
    util::Die(-1);
  }

  const VALORBinning * recoBinning = simple_binning ? fBinning->RecoSmpBinning() : fBinning->RecoOptBinning();
  if(!recoBinning->Contains(kv)) {
    LOG("Lateral",pFATAL) << "Lateral systematic parameter " << param_name
                          << " is applied to " << util::AsString(kv)
                          << ", but this kine var is not in the binning scheme";
    util::Die(-1);
  }

  // Get a vector of kine vars that are present in the binning scheme, but without the kine var we're
  // applying the lateral syst to
  std::vector<conv::KineVar_t> unapplied_kvs;
  for(conv::KineVar_t binning_kv : recoBinning->KineVars()) {
    if(binning_kv != kv) { unapplied_kvs.push_back(binning_kv); }
  }

  double scale = fPhysHypothesis.CurrValue(p);
  if(param.HasLimits()) {
    scale = param.GetNearestPhysicalValue(scale);
  }
  bool tweaked = TMath::Abs(scale-1) > kASmallNum;
  if(!tweaked) return;

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
  LOG("Lateral",pDEBUG) << "Applying lateral systematic parameter: " << param_name
                       << "to PDF [" << pdf->GetName() << "] "
                       << " with kine var " << util::AsString(kv) << " and scale = " << scale;
#endif

  unsigned int nbins = recoBinning->NBins(); // number of reco bins across all reco kinevars in the binning scheme

  std::vector<double>            * unscaled_bin_edges         = & fUnscaledBinEdgesOb      .at(kv);
  std::vector<double>            * bin_edges_to_scale         = & fScaledBinEdgesOb        .at(kv);
  std::vector<bool>              * is_unscaled_bin_overflow   = & fIsUnscaledBinOverflowOb .at(kv);
  std::vector<unsigned int>      * scaled_bin_to_global_bin   = & fScaledBinToGlobalBinOb  .at(kv);
  std::vector<unsigned int>      * unscaled_bin_to_global_bin = & fUnscaledBinToGlobalBinOb.at(kv);
  std::vector<std::vector<bool>> * bins_to_skip               = & fSkipBinOb               .at(kv);
  unsigned int                   * n_overflow_bins            = & fNOverflowBinsOb         .at(kv);
  if(simple_binning) {
    unscaled_bin_edges         = & fUnscaledBinEdgesSb      .at(kv);
    bin_edges_to_scale         = & fScaledBinEdgesSb        .at(kv);
    is_unscaled_bin_overflow   = & fIsUnscaledBinOverflowSb .at(kv);
    scaled_bin_to_global_bin   = & fScaledBinToGlobalBinSb  .at(kv);
    unscaled_bin_to_global_bin = & fUnscaledBinToGlobalBinSb.at(kv);
    bins_to_skip               = & fSkipBinSb               .at(kv);
    n_overflow_bins            = & fNOverflowBinsSb         .at(kv);
  }
  std::vector<double> scaled_bin_edges = *bin_edges_to_scale;

  // Recast VALORArray blocks so they can be used addressed like normal arrays (fast).
  // Hard to read, so should only be used in code for which speed is essential.
  typedef double (pdftype)[nbins];
  pdftype & pdfArr = *reinterpret_cast<pdftype *>(&(pdf->GetEntryRef(0)));

  // Scale the bin edges for this kine var. Assuming uniform distribution of events within each bin.
  for(double & edge : scaled_bin_edges) {
    edge *= scale;
  }

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
  // Copy the un-modified PDF contents into a vector, so we can print comparisons later
  std::vector<double> old_events_per_bin (nbins, 0.0);
  for(unsigned int ireco = 0; ireco < nbins; ireco++) {
    old_events_per_bin[ireco] = pdfArr[ireco];
  }
#endif

  // Calculate the new number of events per bin by seeing how much overlap there is between scaled and unscaled bins.
  // Note: We have underflow and overflow bins here. This is because as we scale the bin edges, events can drop off
  // either end of the scaled binning. These under/overflow bins catch these events, which are then added back on to
  // the first or last bin at the end, respectively.
  std::vector<double> new_events_per_bin (nbins + *n_overflow_bins, 0.0);
  double factor = 0.0;
  int ibin_s = -1;
  for (size_t iedge_s = 0 ; iedge_s < scaled_bin_edges.size() - 1 ; iedge_s++) { // Loop over scaled edges
    const double & bin_min_s = scaled_bin_edges[iedge_s]  ;
    const double & bin_max_s = scaled_bin_edges[iedge_s+1];

    // If the binning scheme is > 1D, looping over the edges of the scaled and unscaled bins will give a number of
    // nonsense 'bins', where the 'lower' edge will be greater than the 'upper' edge, so account for that here when
    // calculating the bin number (as we don't assign these nonsense bins a bin number, we skip them at a later check).
    if(bin_min_s < bin_max_s) { ibin_s++; }

    int ibin_u = -1;
    for (size_t iedge_u = 0 ; iedge_u < (*unscaled_bin_edges).size() - 1 ; iedge_u++) {  // Loop over unscaled bins
      const double & bin_min_u = (*unscaled_bin_edges)[iedge_u]  ;
      const double & bin_max_u = (*unscaled_bin_edges)[iedge_u+1];
      if(bin_min_u < bin_max_u) { ibin_u++; }

      // Only consider unscaled bins for the same kine var values as the scaled bin
      // (for the kine vars that we're not applying the lateral syst to).
      // Note: We end up with bins that go from the upper edge of an overflow to the lower edge of next
      // set of bins. This is fine, as this check automatically skips these bins
      if((*is_unscaled_bin_overflow)[ibin_u] && (*bins_to_skip)[ibin_s][ibin_u - 1]) { continue; }  // Set the overflow bin kine var value to that of the last bin
      else if((*bins_to_skip)[ibin_s][ibin_u]) { continue; }

      if(bin_min_u > bin_max_s)
      {  // Unscaled bin entirely larger than scaled bin, stop checking higher unscaled
        break;
      }
      else if(bin_min_s > bin_max_u)
      {  // Scaled bin entirely larger than unscaled bin, skip to next unscaled bin
        continue;
      }
      else if(bin_min_u >= bin_min_s && bin_max_u <= bin_max_s)
      {  // Unscaled bin subset of scaled bin
        factor = (bin_max_u - bin_min_u) / (bin_max_s - bin_min_s);
      }
      else if(bin_min_s >= bin_min_u && bin_max_s <= bin_max_u)
      {  // Scaled bin subset of unscaled bin
        // Take the contents of this bin unchanged, there could be additional scaled bins contributing
        factor = 1.0;
      }
      else if(bin_max_u > bin_max_s && bin_min_u >= bin_min_s)
      {  // Scaled bin overlaps low part of unscaled bin
        factor = (bin_max_s - bin_min_u) / (bin_max_s - bin_min_s);
      }
      else if(bin_min_u <  bin_min_s && bin_max_u <= bin_max_s)
      {  // Scaled bin overlaps high part of unscaled bin
        factor = (bin_max_u - bin_min_s) / (bin_max_s - bin_min_s);
      }
      else
      {  // Shouldn't get here
        factor = 0.0;
      }

      if(factor <= 0.0) {
        LOG("Lateral",pWARN) << "Got a factor <= 0 for ibin_s = " << ibin_s << ", ibin_u = "
                             << ibin_u << ". This shouldn't happen!";
        continue;
      }

      const unsigned int & glob_bin_s = (*scaled_bin_to_global_bin)[ibin_s];
      new_events_per_bin[ibin_u] += pdfArr[glob_bin_s] * factor;

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
      LOG("Lateral", pDEBUG)
        << "scaled bin "    << ibin_s << " [" << bin_min_s << ", " << bin_max_s << "]; "
        << "un-scaled bin " << ibin_u << " [" << bin_min_u << ", " << bin_max_u << "]; "
        << "factor = " << factor << "; old Nevents = " << pdfArr[glob_bin_s] << "; new Nevents = " << new_events;
#endif

    }
  }

  // Modify the PDF and print debug info
  for (unsigned int ibin = 0 ; ibin < nbins ; ibin++) {  // Loop over global bins
    pdfArr[ibin] = 0.;
  }

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
  LOG("Lateral", pDEBUG) << "PDF contents before and after applying lateral systematic:";
#endif

  int ibin_u = -1;
  for (size_t iedge_u = 0 ; iedge_u < (*unscaled_bin_edges).size() - 1 ; iedge_u++) {  // Loop over unscaled bins
    const double & bin_min_u = (*unscaled_bin_edges)[iedge_u]  ;
    const double & bin_max_u = (*unscaled_bin_edges)[iedge_u+1];
    if(bin_min_u < bin_max_u) { ibin_u++; }
    else                      { continue; }
    const unsigned int & ibin = (*unscaled_bin_to_global_bin)[ibin_u];
    pdfArr[ibin] += new_events_per_bin[ibin_u];

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
    LOG("Lateral", pDEBUG)
      << "bin " << ibin << " [" << recoBinning->BinMinimum(ibin, kv) << ", " << recoBinning->BinMaximum(ibin, kv) << "]"
      << "; old content = " << old_events_per_bin[ibin] << "; new content = " << pdfArr[ibin];
#endif

  }

  // Lateral systematics should just migrate events between bins, while keeping the total number of events constant.
  // Check that we haven't changed the total number of events
  if ( fabs((pdf->Integral()/orig_pdf_integral)-1) > conv::kASmallNum ) {
    LOG("Lateral", pFATAL) << std::fixed << std::setprecision(9)
                           << "Failed normalisation check after applying lateral systematic for PDF"
                           << pdf->GetName() << ". Original integral = " << orig_pdf_integral
                           << ", new integral = " << pdf->Integral();
    util::Die(-1);
  }
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::ReadExtTuningHistsAll(TXMLEngine* xmleng_mc, XMLNodePointer_t top_node_mc)
{
  if(util::NodeExists(xmleng_mc, top_node_mc, "tuning")) {
    XMLNodePointer_t tuning_node = util::FindNode(xmleng_mc, top_node_mc, "tuning");  // <tuning>
    XMLNodePointer_t tuning_group_node = xmleng_mc->GetChild(tuning_node);            // <tuning_group>
    // Loop over all tuning group nodes
    while(tuning_group_node) {
      LOG("GenericSingleSampleFitModel", pINFO)
        << "Reading tuning group named [" << xmleng_mc->GetAttr(tuning_group_node, "name") << "]";

      // Work out the type of tuning to apply and therefore how to treat the input tuning hists
      std::string tuning_type = util::XML2String(xmleng_mc, tuning_group_node, "type"); // <type>
      tuning_type = util::ToLowerCase(util::RemoveAllWhitespace(tuning_type));
      std::vector<TuningInfo> * hist_vec = nullptr;
      std::string param_set_str = "";
      if(tuning_type == "event-by-event") {
        hist_vec = (std::vector<TuningInfo> *) &fEventByEventTuningHists;
      }
      else if(tuning_type == "bin-by-bin") {
        hist_vec = (std::vector<TuningInfo> *) &fBinByBinTuningHists;
        param_set_str = util::XML2String(xmleng_mc, tuning_group_node, "param_set"); // <param_set>
        param_set_str = util::ToLowerCase(util::RemoveAllWhitespace(param_set_str));
      }
      else {
        LOG("GenericSingleSampleFitModel", pFATAL)
          << "Can not parse input tuning type [" << tuning_type << "] for tuning group ["
          << xmleng_mc->GetAttr(tuning_group_node, "name") << "]";
        util::Die(-1);
      }

      // The actual tuning hist info lives in a separate config file, the name of which is specified in the node named "file"
      std::string file = util::RemoveAllWhitespace(util::XML2String(xmleng_mc, tuning_group_node, "file"));
      TString ts_file = util::CompleteConfigPath(file).c_str();

      // Parse the input tuning XML configuration file
      TXMLEngine* xmleng = new TXMLEngine;
      XMLDocPointer_t xmldoc = xmleng->ParseFile(ts_file.Data());
      if (xmldoc==0) {
        LOG("GenericSingleSampleFitModel", pERROR)
          << "Can not parse input tuning XML file [" << file << "]";
        util::Die(1);
      }
      XMLNodePointer_t root_node = xmleng->DocGetRootElement(xmldoc);   // <tuning>
      XMLNodePointer_t curr_node = xmleng->GetChild(root_node);         // <tune>
      // Loop over all tuning nodes
      while(curr_node) {
        this->ReadTune(xmleng, curr_node, hist_vec);
        curr_node = xmleng->GetNext(curr_node);
      } // Loop over tune nodes in the separate tuning file

      tuning_group_node = xmleng->GetNext(tuning_group_node);
    } // Loop over tuning group nodes
  }
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::ReweightHistUsingParamSet(TH1* hist_to_reweight, std::vector<conv::KineVar_t> kvs,
    CorrelatedSystematics* syst, std::string param_set_str, conv::ApplyMask_t mask, bool use_current_vals)
{
  int hndim = hist_to_reweight->GetDimension();
  if(hndim != (int)kvs.size()) {
    LOG("FitParams",pERROR)
      << "Tried to reweight a hist with " << hist_to_reweight->GetDimension() << " dimensions, but "
      << kvs.size() << " were input.";
    util::Die(-1);
  }

  std::vector<PhysParamId_t> param_set = util::GetParameterSet(param_set_str, mask);
  std::string param_set_output_str = "";
  for (PhysParamId_t p : param_set) {
    param_set_output_str += "\n  " + util::NameRegistry::Instance()->ParamAsString(p);
  }
  LOG("FitParams",pDEBUG) << "Reweighting hist " << hist_to_reweight->GetName()
                          << " with the curr or init vals of the following params: " << param_set_output_str;
  for (PhysParamId_t p : param_set) {
    if(!util::IsSystematic(p)) { continue; }
    if(!ParameterPool::Instance()->IsParamInPool(p)) {
      LOG("FitParams",pWARN)
        << "Skipping parameter " << util::NameRegistry::Instance()->ParamAsString(p)
        << ", as it was not in the ParameterPool";
      continue;
    }
    Parameter param = ParameterPool::Instance()->GetParam(p);

    // Check that this parameter has matching kinevars
    if(!param.HasKinematicRange()) {
      LOG("FitParams",pWARN)
        << "Skipping parameter " << util::NameRegistry::Instance()->ParamAsString(p)
        << ", as it has no kinematic dependence";
      continue;
    }
    for (const conv::KineVar_t param_kv : param.GetKineVars()) {
      bool found = false;
      for (conv::KineVar_t input_kv : kvs) {
        if(param_kv == input_kv) {
          found = true;
          break;
        }
      }
      if(!found) {
        LOG("FitParams",pERROR)
          << "Parameter " << util::NameRegistry::Instance()->ParamAsString(p)
          << " could not be applied, as all of its applied kine vars don't match the input kinevars";
        util::Die(-1);
      }
    }

    // Get the weight
    double weight = 1.;
    if(use_current_vals) {
      weight = syst->CurrentValue(p);
    }
    else {
      weight = syst->InitialValue(p);
    }

    // Get the bins this parameter applies to. Here I've assumed that the order of kinevars in kvs matches the order of
    // dimensions in the input hist
    int bin_low_glob  = hist_to_reweight->FindBin(param.GetKineRange(kvs[0]).first  + conv::kASmallNum);  // + kASmallNum because searching for the right bin can go wrong if we search using the bin edges
    int bin_high_glob = hist_to_reweight->FindBin(param.GetKineRange(kvs[0]).second - conv::kASmallNum);
    int bin_low_x  = -1;
    int bin_high_x = -1;
    int dummy = -1;
    hist_to_reweight->GetBinXYZ(bin_low_glob , bin_low_x , dummy, dummy);
    hist_to_reweight->GetBinXYZ(bin_high_glob, bin_high_x, dummy, dummy);
    for (int ibinx = bin_low_x; ibinx <= bin_high_x; ibinx++) {
      if(kvs.size() > 1) {
        bin_low_glob  = hist_to_reweight->FindBin(param.GetKineRange(kvs[0]).first,  param.GetKineRange(kvs[1]).first);
        bin_high_glob = hist_to_reweight->FindBin(param.GetKineRange(kvs[0]).second, param.GetKineRange(kvs[1]).second);
        int bin_low_y  = -1;
        int bin_high_y = -1;
        hist_to_reweight->GetBinXYZ(bin_low_glob , dummy, bin_low_y,  dummy);
        hist_to_reweight->GetBinXYZ(bin_high_glob, dummy, bin_high_y, dummy);
        for (int ibiny = bin_low_y; ibiny <= bin_high_y; ibiny++) {
          if(kvs.size() > 2) {
            // TODO: Put in 3D hist reweighting if needed
            LOG("FitParams",pFATAL)
              << "Reweighting a 3D hist is not yet supported.";
            util::Die(-1);
          }
          else {
            double current_content = hist_to_reweight->GetBinContent(ibinx, ibiny);
            hist_to_reweight->SetBinContent(ibinx, ibiny, current_content * weight);
          }
        }
      }
      else {
        double current_content = hist_to_reweight->GetBinContent(ibinx);
        hist_to_reweight->SetBinContent(ibinx, current_content * weight);
      }
    }
  }
}
//________________________________________________________________________________________
std::string GenericSingleSampleFitModel::GetPDFIntegralAsString(Option_t * opt_in)
{
  double integral = -999;

  std::string opt = (opt_in) ? opt_in : "";
  if(opt.find("SB") != string::npos) {
    integral = fPDFTotSb->Integral();
  }
  else {
    integral = fPDFTotOb->Integral();
  }

  TString temp = TString::Format("%s total model PDF integral = %.5lf", this->Name().c_str(), integral);
  std::string to_print = temp.Data();

  return to_print;
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::FillNonEmptyBinVectors()
{
  const VALORBinning  * reco_ob = fBinning->RecoOptBinning();
  const VALORBinning  * reco_sb = fBinning->RecoSmpBinning();
  const int n_reco_bins_ob = reco_ob->NBins();
  const int n_reco_bins_sb = reco_sb->NBins();
  for(size_t imode = 0; imode < fReactionModes.size(); imode++) {
    ReacMode_t m = fReactionModes[imode];
    if(m == kmUndefined) continue;

    const VALORBinning *true_sb = fBinning->TrueSmpBinning(m);
    const int n_true_bins = true_sb->NBins();

    unsigned int i_nonempty_bin_ob = 0;
    unsigned int i_nonempty_bin_sb = 0;
    for(int itrue = 0; itrue < n_true_bins; itrue++) {
      for(int ireco = 0; ireco < n_reco_bins_ob; ireco++) {
        if(fMCTFinOb[imode]->GetEntryRef(ireco,itrue) > conv::kASmallNum) {
          fNonEmptyBinsOb[imode][i_nonempty_bin_ob++] = n_true_bins * ireco + itrue;
        }
      }
      for(int ireco = 0; ireco < n_reco_bins_sb; ireco++) {
        if(fMCTFinSb[imode]->GetEntryRef(ireco,itrue) > conv::kASmallNum) {
          fNonEmptyBinsSb[imode][i_nonempty_bin_sb++] = n_true_bins * ireco + itrue;
        }
      }
    }

    fNumOfNonEmptyBinsOb[imode] = i_nonempty_bin_ob;
    fNumOfNonEmptyBinsSb[imode] = i_nonempty_bin_sb;
  }
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::PreComputeLateralSystVectors(bool use_simple_binning)
{
  if(fLateralPostOscParams.size() == 0) { return; }

  const VALORBinning * recoBinning = use_simple_binning ? fBinning->RecoSmpBinning() : fBinning->RecoOptBinning();
  unsigned int nbins = recoBinning->NBins(); // number of reco bins across all reco kinevars in the binning scheme

  std::vector<conv::KineVar_t> kvs_already_considered;
  for(size_t ip = 0; ip < fLateralPostOscParams.size(); ip++) {
    PhysParamId_t p = fLateralPostOscParams[ip];
    if(p == kPhypUndefined) { continue; }

    const Parameter & param = ParameterPool::Instance()->GetParam(p);
    const std::string & param_name = param.GetName();
    if(!param.HasKinematicRange()) {
      LOG("Lateral",pFATAL) << "Lateral systematic parameter " << param_name
                            << " has no kinematic range. Cannot apply";
      util::Die(-1);
    }

    const std::vector<conv::KineVar_t> & kvs = param.GetKineVars();
    if(kvs.size() != 1) {
      LOG("Lateral",pFATAL) << "Lateral systematic parameter " << param_name
                            << " is applied to " << kvs.size() << " kinematic variables. Cannot apply";
      util::Die(-1);
    }
    const conv::KineVar_t & kv = kvs[0];
    if(std::find(kvs_already_considered.begin(), kvs_already_considered.end(), kv) != kvs_already_considered.end()) { continue; }
    kvs_already_considered.push_back(kv);


    if(!util::IsReco(kv)) {
      LOG("Lateral",pFATAL) << "Lateral systematic parameter " << param_name
                            << " is applied to " << util::AsString(kv)
                            << ", but only reco kinevars can be applied";
      util::Die(-1);
    }

    if(!recoBinning->Contains(kv)) {
      LOG("Lateral",pFATAL) << "Lateral systematic parameter " << param_name
                            << " is applied to " << util::AsString(kv)
                            << ", but this kine var is not in the binning scheme";
      util::Die(-1);
    }

    // Get a vector of kine vars that are present in the binning scheme, but without the kine var we're
    // applying the lateral syst to
    std::vector<conv::KineVar_t> unapplied_kvs;
    for(conv::KineVar_t binning_kv : recoBinning->KineVars()) {
      if(binning_kv != kv) { unapplied_kvs.push_back(binning_kv); }
    }

    // Calculating the bin centres is computationally very expensive, so pre-calculate them here
    std::vector<std::map<conv::KineVar_t, double>> unapplied_kv_bin_centres(nbins);
    for(unsigned int ireco = 0 ; ireco < nbins ; ireco++) {
      for(conv::KineVar_t unapplied_binning_kv : unapplied_kvs) {
        unapplied_kv_bin_centres[ireco][unapplied_binning_kv] = recoBinning->BinCentre(ireco, unapplied_binning_kv);
      }
    }

    std::vector<double>            * unscaled_bin_edges         = & fUnscaledBinEdgesOb      .at(kv);
    std::vector<double>            * scaled_bin_edges           = & fScaledBinEdgesOb        .at(kv);
    std::vector<bool>              * is_unscaled_bin_overflow   = & fIsUnscaledBinOverflowOb .at(kv);
    std::vector<unsigned int>      * scaled_bin_to_global_bin   = & fScaledBinToGlobalBinOb  .at(kv);
    std::vector<unsigned int>      * unscaled_bin_to_global_bin = & fUnscaledBinToGlobalBinOb.at(kv);
    std::vector<std::vector<bool>> * bins_to_skip               = & fSkipBinOb               .at(kv);
    unsigned int                   * n_overflow_bins            = & fNOverflowBinsOb         .at(kv);
    if(use_simple_binning) {
      unscaled_bin_edges         = & fUnscaledBinEdgesSb      .at(kv);
      scaled_bin_edges           = & fScaledBinEdgesSb        .at(kv);
      is_unscaled_bin_overflow   = & fIsUnscaledBinOverflowSb .at(kv);
      scaled_bin_to_global_bin   = & fScaledBinToGlobalBinSb  .at(kv);
      unscaled_bin_to_global_bin = & fUnscaledBinToGlobalBinSb.at(kv);
      bins_to_skip               = & fSkipBinSb               .at(kv);
      n_overflow_bins            = & fNOverflowBinsSb         .at(kv);
    }

    // Fill the vector of indices corresponding to end bins. Also get the number of overflow bins expected (1 for each time the binning loops)
    // TODO: Put in an underflow bin
    std::vector<unsigned int> end_bins;
    double last_bin_min = recoBinning->BinMinimum(0, kv);
    for(unsigned int ireco = 0 ; ireco < nbins ; ireco++) {
      double bin_min = recoBinning->BinMinimum(ireco, kv);
      if(bin_min < last_bin_min) {
        // When this happens in a > 1D binning scheme, we've reached the end of this kine var's binning and started
        // back at the beginning, so we'll need to add the high edge of the last bin and then an overflow bin
        end_bins.push_back(ireco-1);
      }
      last_bin_min = bin_min;
    }
    end_bins.push_back(nbins-1);  // The above loop won't catch the very last bin in the entire binning scheme, so add it here
    *n_overflow_bins           = end_bins.size();
    unsigned int n_upper_edges = end_bins.size();

    unsigned int bad_bin_number = 99999999;

    *unscaled_bin_edges         = std::vector<double>            (nbins + n_upper_edges + *n_overflow_bins, conv::kBadResult);
    *scaled_bin_edges           = std::vector<double>            (nbins + n_upper_edges                   , conv::kBadResult);
    *is_unscaled_bin_overflow   = std::vector<bool>              (nbins + *n_overflow_bins                , false           );
    *scaled_bin_to_global_bin   = std::vector<unsigned int>      (nbins                                   , bad_bin_number);
    *unscaled_bin_to_global_bin = std::vector<unsigned int>      (nbins + *n_overflow_bins                , bad_bin_number);
    *bins_to_skip               = std::vector<std::vector<bool>> (nbins, std::vector<bool>(nbins + *n_overflow_bins, false));

    // Fill the unscaled bin edge vector. For the scaled bin edge vector, fill with unscaled edges for now,
    // and scale in ApplyLateralSystematic
    unsigned int n_up_edges_added = 0;
    {
      vector<unsigned int>::iterator end_bins_it;
      end_bins_it = end_bins.begin();
      unsigned int iedge_u = 0;
      unsigned int iedge_s = 0;
      for(unsigned int ireco = 0 ; ireco < nbins ; ireco++) {
        // Add the low edge
        double bin_min = recoBinning->BinMinimum(ireco, kv);
        (*unscaled_bin_edges)[iedge_u] = bin_min; iedge_u++;
        (*scaled_bin_edges  )[iedge_s] = bin_min; iedge_s++;
        (*scaled_bin_to_global_bin  )[iedge_s-1 - n_up_edges_added] = ireco;
        (*unscaled_bin_to_global_bin)[iedge_u-1 - n_up_edges_added] = ireco;

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
        LOG("Lateral", pDEBUG) << "ireco = " << ireco << "; scaled bin = " << iedge_s-1 - n_up_edges_added
                             << "; un-scaled bin = " << iedge_u-1 - n_up_edges_added
                             << "; iedge_u = " << iedge_u-1 << "; iedge_s = " << iedge_s-1
                             << "; un-scaled edge = " << (*unscaled_bin_edges([iedge_u-1]
                             << "; scaled edge = "    << (*scaled_bin_edges)[iedge_s-1]
                             << "; next end bin index = " << *end_bins_it;
#endif

        // Add the high edge of the last bin, as well as an overflow bin
        if(ireco == *end_bins_it) {
          double bin_max = recoBinning->BinMaximum(ireco, kv);
          (*unscaled_bin_edges)[iedge_u] = bin_max;          iedge_u++;         // PDF bin maximum
          (*unscaled_bin_edges)[iedge_u] = conv::kALargeNum; iedge_u++;         // Overflow bin maximum
          (*scaled_bin_edges  )[iedge_s] = bin_max;          iedge_s++;         // PDF bin maximum
          (*unscaled_bin_to_global_bin)[iedge_u-2 - n_up_edges_added] = ireco;  // Overflow bin
          (*is_unscaled_bin_overflow  )[iedge_u-2 - n_up_edges_added] = true;
          n_up_edges_added++;
          end_bins_it++;

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
          LOG("Lateral", pDEBUG) << "un-scaled bin = " << iedge_u-2 - (n_up_edges_added-1)
                               << "; iedge_u = " << iedge_u-2 << "; iedge_s = " << iedge_s-1
                               << "; un-scaled edge = " << (*unscaled_bin_edges)[iedge_u-2]
                               << "; scaled edge = "    << (*scaled_bin_edges)[iedge_s-1]
                               << "; with overflow un-scaled edge = " << (*unscaled_bin_edges)[iedge_u-1];
#endif
        } // if is an end bin
      } // ireco
    }

    // Check edges
    for(unsigned int iedge = 0 ; iedge < (*unscaled_bin_edges).size() ; iedge++) {
      if(iedge == 0 && fabs((*unscaled_bin_edges)[0]) > conv::kASmallNum) {
        LOG("Lateral", pFATAL)
          << "Tried to apply a lateral systematic when the first reco bin edge != 0. Currently there's no underflow bin to support this, it needs putting in.";
        util::Die(-1);
      }
      if((*unscaled_bin_edges)[iedge] == conv::kBadResult) {
        LOG("Lateral", pFATAL) << "Un-scaled bin edge for index " << iedge << " not set.";
        util::Die(-1);
      }
    }
    for(unsigned int iedge = 0 ; iedge < (*scaled_bin_edges).size() ; iedge++) {
      if((*scaled_bin_edges)[iedge] == conv::kBadResult) {
        LOG("Lateral", pFATAL) << "Scaled bin edge for index " << iedge << " not set.";
        util::Die(-1);
      }
    }

    // Check bins
    for(unsigned int ibin = 0 ; ibin < nbins ; ibin++) {
      if((*scaled_bin_to_global_bin)[ibin] == bad_bin_number) {
        LOG("Lateral", pFATAL) << "Scaled bin to global bin vector: index " << ibin << " not set.";
        util::Die(-1);
      }
    }
    for(unsigned int ibin = 0 ; ibin < nbins + *n_overflow_bins ; ibin++) {
      if((*unscaled_bin_to_global_bin)[ibin] == bad_bin_number) {
        LOG("Lateral", pFATAL) << "Un-scaled bin to global bin vector: index " << ibin << " not set.";
        util::Die(-1);
      }
    }

    // Work out which combinations of scaled and un-scaled bins we need to skip.
    // It's fine that the bin edges haven't actually been scaled yet - the relative values of the edges is what
    // matters here, and they're all scaled by the same amount.
    int ibin_s = -1;
    for (unsigned int iedge_s = 0 ; iedge_s < (*scaled_bin_edges).size() - 1 ; iedge_s++) { // Loop over scaled edges
      const double & bin_min_s = (*scaled_bin_edges)[iedge_s]  ;
      const double & bin_max_s = (*scaled_bin_edges)[iedge_s+1];

      // If the binning scheme is > 1D, looping over the edges of the scaled and unscaled bins will give a number of
      // nonsense 'bins', where the 'lower' edge will be greater than the 'upper' edge, so account for that here when
      // calculating the bin number (as we don't assign these nonsense bins a bin number, we skip them at a later check).
      if(bin_min_s < bin_max_s) { ibin_s++; }

      int ibin_u = -1;
      for (size_t iedge_u = 0 ; iedge_u < (*unscaled_bin_edges).size() - 1 ; iedge_u++) {  // Loop over unscaled bins
        const double & bin_min_u = (*unscaled_bin_edges)[iedge_u]  ;
        const double & bin_max_u = (*unscaled_bin_edges)[iedge_u+1];
        if(bin_min_u < bin_max_u) { ibin_u++; }

        // Only consider unscaled bins for the same kine var values as the scaled bin
        // (for the kine vars that we're not applying the lateral syst to).
        // Note: We end up with bins that go from the upper edge of an overflow to the lower edge of next
        // set of bins. This is fine, as this check automatically skips these bins
        unsigned int check_ibin_u = ibin_u;
        if((*is_unscaled_bin_overflow)[ibin_u]) {
          // Set the overflow bin kine var value to that of the last bin
          check_ibin_u = ibin_u - 1;
        }
        for(const conv::KineVar_t & unapplied_binning_kv : unapplied_kvs) {
          const double & unapplied_kv_val_s = unapplied_kv_bin_centres[  (*scaled_bin_to_global_bin)[ibin_s]].at(unapplied_binning_kv);
          const double & unapplied_kv_val_u = unapplied_kv_bin_centres[(*unscaled_bin_to_global_bin)[check_ibin_u]].at(unapplied_binning_kv);
          if(fabs(unapplied_kv_val_s - unapplied_kv_val_u) > conv::kASmallNum) {
            (*bins_to_skip)[ibin_s][check_ibin_u] = true;
            break;
          }

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
          LOG("Lateral", pDEBUG)
             << "scaled bin "    << ibin_s << " [" << bin_min_s << ", " << bin_max_s << "]; "
             << "un-scaled bin " << ibin_u << " [" << bin_min_u << ", " << bin_max_u << "]; "
             << "For unapplied kv ["<<util::AsString(unapplied_binning_kv)<<"], val s = "
             <<unapplied_kv_val_s<<", val u = "<<unapplied_kv_val_u<<", skip? "<<skip;
#endif

        } // loop over unapplied kine vars
      } // iedge_u
    } // iedge_s
  } // ip: loop over lateral post-osc params
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::ResetSystematicVariationIDs()
{
  fSystUniverseToApply.clear();
  fApplyUniverseVariations = false;
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::SetSystematicVariationIDToApply(const std::string & param_name,size_t id)
{
  assert(id >= fFirstUniverseToSave && id <=fLastUniverseToSave);
  fSystUniverseToApply[param_name] = id;
  fApplyUniverseVariations = true;
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::SetUniverseRangeToRead(size_t first, size_t last)
{
  fFirstUniverseToSave = first;
  fLastUniverseToSave = last;
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::SetUniverseParameterToRead(const std::string & param_name)
{
  fUniverseParametersToSave.push_back(param_name);
}
//________________________________________________________________________________________
void GenericSingleSampleFitModel::ResetUniversesToRead()
{
  fFirstUniverseToSave = 0;
  fLastUniverseToSave = 0;
  fUniverseParametersToSave.clear();
}
//________________________________________________________________________________________
const std::string & GenericSingleSampleFitModel::GetLastConfigFileRead() const
{
  return fConfigFileRead;
}
//________________________________________________________________________________________
util::ToyStream* GenericSingleSampleFitModel::InitToyStream(const string file_name, const string read_or_write, string valor_array_format)
{
  valor_array_format = util::RemoveAllWhitespace(valor_array_format);
  if(valor_array_format.empty()) {
    valor_array_format = fDefVALORArrayFormat;
  }

  return new util::ToyStream(file_name, read_or_write, typeid(this), this->Name(),
                             valor_array_format, this->StreamSize());
}
//________________________________________________________________________________________

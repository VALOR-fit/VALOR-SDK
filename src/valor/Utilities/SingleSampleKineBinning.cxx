//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#include <functional>
#include <set>
#include <iterator>
#include <algorithm>

#include <TSystem.h>

#include "valor/Conventions/Constants.h"
#include "valor/Conventions/ExperimentSetup.h"
#include "valor/Utilities/SingleSampleKineBinning.h"
#include "valor/Utilities/UtlXML.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/Env.h"
#include "valor/Messenger/Messenger.h"
#include "valor/KineBinning/VALORBinning.h"

using namespace valor;
using std::string;
using std::vector;

// Keep a static copy of the experimental setup
static const ExperimentSetup & es = *ExperimentSetup::Instance();

//________________________________________________________________________________________
SingleSampleKineBinning::SingleSampleKineBinning(conv::Detector_t d, conv::BeamConf_t b, conv::Sample_t s)
{
  this->Init();

  fDetector = d;
  fBeamConf = b;
  fSample   = s;
}
//________________________________________________________________________________________
SingleSampleKineBinning::SingleSampleKineBinning(UniqueSampleID usid)
{
  this->Init();

  fDetector = usid.detector;
  fBeamConf = usid.beamconf;
  fSample   = usid.sample;
}
//________________________________________________________________________________________
SingleSampleKineBinning::~SingleSampleKineBinning(void)
{
  this->CleanUp();
}
//________________________________________________________________________________________
bool SingleSampleKineBinning::Read(string file)
{
  if(fBeamConf == conv::kbUndefined) {
    LOG("SampleBinning", pERROR) << "Can not read binning for undefined beam";
    util::Die(1); 
    return false;
  }
  if(fSample == conv::ksUndefined) {
    LOG("SampleBinning", pERROR) << "Can not read binning for undefined sample";
    util::Die(1); 
    return false;
  }
  
  std::set<conv::KineVar_t> kinevars;

  this->CleanUp();
  
  // Get relevant modes
  int ibeam   = es.BeamConfIndex(fBeamConf);
  int isample = es.SampleIndex(fBeamConf,fSample);
  fReactionModes = es.ReactionModes()[ibeam][isample];

  TString ts_file = file.c_str();
  gSystem->ExpandPathName(ts_file);

  // See if it works as a normal path
  FileStat_t dummy;
  if (gSystem->GetPathInfo(ts_file.Data(), dummy))
  {
    // If not, try the config folder
    ts_file = TString::Format("%s/%s",
       Env::Instance()->TopLevelConfDir().c_str(),ts_file.Data());
    if (gSystem->GetPathInfo(ts_file.Data(), dummy)) {
      LOG("SampleBinning", pERROR) << "Binning file not found: " << file;
      util::Die(1); 
      return false;
    }
  }

  // Parse the input binning configuration XML file
  TXMLEngine* xml = new TXMLEngine;
  XMLDocPointer_t xmldoc = xml->ParseFile(ts_file.Data());
  if (xmldoc==0) {
    LOG("SampleBinning",pERROR)
        << "Can not parse input XML file [" << ts_file << "]";
    delete xml;
    util::Die(1); 
    return false;
  }
  XMLNodePointer_t root_node = xml->DocGetRootElement(xmldoc);

  //
  // Reco/Optimized
  //

  if(! util::NodeExists(xml,root_node,"reco_optimized")) {
     LOG("SampleBinning", pERROR)
        << "<reco_optimized> node not found in file: " << file;
     return false;
  }
  string reco_opt_binning_file = util::XML2String(xml,root_node,"reco_optimized");
  LOG("SampleBinning", pNOTICE)
    << "\n ** Reading optimized reco binning ** " 
    << "\n    For beam: " << util::AsString(fBeamConf) << " / sample: " << util::AsString(fSample)
    << "\n    From file: " << reco_opt_binning_file;

  fRecoOptimizedBinning = VALORBinning::BinningFromFile(reco_opt_binning_file,false);
  bool read_ok = fRecoOptimizedBinning->Read(reco_opt_binning_file);
  if(!read_ok) {
     LOG("SampleBinning", pERROR)
       << "Could not read optimized reco binning from: " << reco_opt_binning_file;
     return false;
  }
  
  const std::vector<conv::KineVar_t> & reco_opt_kvs = fRecoOptimizedBinning->KineVars();
  std::copy(reco_opt_kvs.begin(),reco_opt_kvs.end(),std::inserter( kinevars, kinevars.end()));

  //
  // Reco/Simple
  //

  if(! util::NodeExists(xml,root_node,"reco_simple")) {
     LOG("SampleBinning", pERROR)
        << "<reco_simple> node not found in file: " << file;
     return false;
  }
  string reco_smp_binning_file = util::XML2String(xml,root_node,"reco_simple");
  LOG("SampleBinning", pINFO)
    << "\n ** Reading simple reco binning ** " 
    << "\n    For beam: " << util::AsString(fBeamConf) << " / sample: " << util::AsString(fSample)
    << "\n    From file: " << reco_smp_binning_file;

  fRecoSimpleBinning = VALORBinning::BinningFromFile(reco_smp_binning_file,false);;
  fRecoSimpleBinning->SetName("reco_simple");
  read_ok = fRecoSimpleBinning->Read(reco_smp_binning_file);
  if(!read_ok) {
     LOG("SampleBinning", pINFO)
       << "Could not read simple reco binning from: " << reco_smp_binning_file;
     return false;
  }
  
  const std::vector<conv::KineVar_t> & reco_smp_kvs = fRecoSimpleBinning->KineVars();
  std::copy(reco_smp_kvs.begin(),reco_smp_kvs.end(),std::inserter( kinevars, kinevars.end()));
  
  // Check reco and true have the dimensions and limits
  if (fRecoSimpleBinning->KineSpace() != fRecoOptimizedBinning->KineSpace()) {
    LOG("SampleBinning",pERROR)<<"Simple and Optimized binnings do not have the kinematic dimensions.";
    return false;
  }
  for (unsigned int idim = 0 ; idim < fRecoSimpleBinning->NDimensions(); idim++) {
    if (std::fabs(fRecoSimpleBinning->Maximum(idim)-fRecoOptimizedBinning->Maximum(idim)) > conv::kASmallNum || 
        std::fabs(fRecoSimpleBinning->Minimum(idim)-fRecoOptimizedBinning->Minimum(idim)) > conv::kASmallNum ) {
      LOG("SampleBinning",pERROR)<<"Simple and Optimized binnings do not have the same total range.";
      return false;
    }
  }


  //
  // True/Simple [default]
  //

  bool def_true_smp_exists = util::NodeExists(xml,root_node,"true_simple/default");
  if(! def_true_smp_exists) {
     LOG("SampleBinning", pWARN)
        << "<true_simple><default> node not found in file: " << file << "\n"
        << " ** Binning must be specified for every single reaction mode";
  }
  if(def_true_smp_exists) {
     string def_true_smp_binning_file = util::XML2String(xml,root_node,"true_simple/default");
     LOG("SampleBinning", pINFO)
        << "\n ** Reading `default' simple true binning ** " 
        << "\n    For beam: " << util::AsString(fBeamConf) << " / sample: " << util::AsString(fSample)
        << "\n    From file: " << def_true_smp_binning_file;
     VALORBinning * def_true_smp_binning = VALORBinning::BinningFromFile(def_true_smp_binning_file,false);
     def_true_smp_binning->SetName("true_simple (default)");
     read_ok = def_true_smp_binning->Read(def_true_smp_binning_file);
     if(!read_ok) {
        LOG("SampleBinning", pINFO)
          << "Could not read default simple true binning from: " << def_true_smp_binning_file;
        return false;
     }
     fTrueSimpleBinningUnique.push_back(def_true_smp_binning);
     for(size_t im = 0; im < fReactionModes.size(); im++) {
        assert(im==fTrueSimpleBinning.size());
        fTrueSimpleBinning.push_back(def_true_smp_binning);
     }
     
     const std::vector<conv::KineVar_t> & true_smp_kvs = def_true_smp_binning->KineVars();
     std::copy(true_smp_kvs.begin(),true_smp_kvs.end(),std::inserter( kinevars, kinevars.end()));
  }

  //
  // True/Simple [per mode]
  //

  int is = es.SampleIndex(fBeamConf,fSample);
  LOG("SampleBinning", pDEBUG) << "Sample idx = " << is;

  for(size_t im = 0; im < fReactionModes.size(); im++) {
    conv::ReacMode_t m = fReactionModes[im];
    if(m == conv::kmUndefined) continue;
    string node_path = "true_simple/" + util::AsString(m);
    bool mode_true_smp_exists = util::NodeExists(xml,root_node,node_path);
    if(mode_true_smp_exists) {
      string mode_true_smp_binning_file = util::XML2String(xml,root_node,node_path);
      LOG("SampleBinning", pINFO)
        << "\n ** Overriding default / Reading simple true binning ** " 
        << "\n    For beam: " << util::AsString(fBeamConf) << " / sample: " << util::AsString(fSample) << " / mode: " <<  util::AsString(m)
        << "\n    From file: " << mode_true_smp_binning_file;
      VALORBinning * mode_true_smp_binning = VALORBinning::BinningFromFile(mode_true_smp_binning_file,false);;
      read_ok = mode_true_smp_binning->Read(mode_true_smp_binning_file);
      if(!read_ok) {
         LOG("SampleBinning", pINFO)
            << "Could not read simple true binning from: " << mode_true_smp_binning_file;
         return false;
      }
      fTrueSimpleBinningUnique.push_back(mode_true_smp_binning);
      fTrueSimpleBinning [im] = mode_true_smp_binning;
      
      const std::vector<conv::KineVar_t> & true_smp_kvs = mode_true_smp_binning->KineVars();
      std::copy(true_smp_kvs.begin(),true_smp_kvs.end(),std::inserter( kinevars, kinevars.end()));
     
    }    
  }

  //
  // XSec reco 
  //

  if(! util::NodeExists(xml,root_node,"xsec_reco")) {
     LOG("SampleBinning", pERROR)
        << "<xsec_reco> node not found in file: " << file;
     return false;
  }
  string xsec_reco_binning_file = util::XML2String(xml,root_node,"xsec_reco");
  LOG("SampleBinning", pINFO)
    << "\n ** Reading xsec reconstructed binning ** " 
    << "\n    For beam: " << util::AsString(fBeamConf) << " / sample: " << util::AsString(fSample)
    << "\n    From file: " << xsec_reco_binning_file;

  fXsecRecoBinning = VALORBinning::BinningFromFile(xsec_reco_binning_file,false);
  read_ok = fXsecRecoBinning->Read(xsec_reco_binning_file);
  fXsecRecoBinning->SetName("xsec_reco");
  if(!read_ok) {
     LOG("SampleBinning", pNOTICE)
       << "Could not read xsec reco binning from: " << xsec_reco_binning_file<<", falling back on optimized.";
       
     // FALL BACK ON OPTIMIZED BINNING
     delete fXsecRecoBinning;
     fXsecRecoBinning = fRecoOptimizedBinning;
  }
  
  //
  // xsec true [default]
  //
  bool xsec_true_binning_exists = util::NodeExists(xml,root_node,"xsec_true");
  if (xsec_true_binning_exists) {
    bool def_xsec_true_exists = util::NodeExists(xml,root_node,"xsec_true/default");
    if(! def_xsec_true_exists) {
       LOG("SampleBinning", pWARN)
          << "<xsec_true><default> node not found in file: " << file << "\n"
          << " ** Binning must be specified for every single reaction mode";
    }
    if(def_xsec_true_exists) {
       string def_xsec_true_binning_file = util::XML2String(xml,root_node,"xsec_true/default");
       LOG("SampleBinning", pINFO)
          << "\n ** Reading `default' xsec true binning ** " 
          << "\n    For beam: " << util::AsString(fBeamConf) << " / sample: " << util::AsString(fSample)
          << "\n    From file: " << def_xsec_true_binning_file;
       VALORBinning * def_xsec_true_binning = VALORBinning::BinningFromFile(def_xsec_true_binning_file,false);
       def_xsec_true_binning->SetName("xsec_true (default)");
       read_ok = def_xsec_true_binning->Read(def_xsec_true_binning_file);
       if(!read_ok) {
          LOG("SampleBinning", pINFO)
            << "Could not read default xsec true binning from: " << def_xsec_true_binning_file;
          return false;
       }
       fXsecTrueBinningUnique.push_back(def_xsec_true_binning);
       for(size_t im = 0; im < fReactionModes.size(); im++) {
          assert(fXsecTrueBinning.size()==im);
          fXsecTrueBinning.push_back(def_xsec_true_binning);
       }
    }

    //
    // True/Simple [per mode]
    //
    for(size_t im = 0; im < fReactionModes.size(); im++) {
      conv::ReacMode_t m = fReactionModes[im];
      if(m == conv::kmUndefined) continue;
      string mode_xsec_true_binning_file = "";
      string node_path = "xsec_true/" + util::AsString(m);
      bool mode_xsec_true_exists = false;
      if(util::NodeExists(xml,root_node,node_path)) {
        mode_xsec_true_exists = true;
        mode_xsec_true_binning_file = util::XML2String(xml, root_node, node_path);
      }
      else {
        // Read binning that depends on the coarse reac mode, if specified
        XMLNodePointer_t xsec_true_node = util::FindNode(xml, root_node, "xsec_true");
        vector<XMLNodePointer_t> xsec_true_binning_vec = util::FindNodes(xml, xsec_true_node, "element");
        if(xsec_true_binning_vec.size() != 0) {
          for(XMLNodePointer_t element : xsec_true_binning_vec) {
            if(util::NodeExists(xml, element, "coarse_modes") && util::NodeExists(xml, element, "binning")) {
              string coarse_modes_str_all = util::XML2String(xml, element, "coarse_modes");
              vector<string> coarse_modes_str_vec = util::Tokens(util::RemoveAllWhitespace(coarse_modes_str_all), ",");
              for(string coarse_mode_str : coarse_modes_str_vec) {
                if(util::ModeWithinCoarse(m, util::CoarseModeFromString(coarse_mode_str))) {
                  mode_xsec_true_exists = true;
                  mode_xsec_true_binning_file = util::XML2String(xml, element, "binning");
                  break;
                }
              }
            }
            if(mode_xsec_true_exists) {
              break;
            }
          }
        }
      }
      if(mode_xsec_true_exists) {
        LOG("SampleBinning", pINFO)
          << "\n ** Overriding default / Reading xsec true binning ** " 
          << "\n    For beam: " << util::AsString(fBeamConf) << " / sample: " << util::AsString(fSample) << " / mode: " <<  util::AsString(m)
          << "\n    From file: " << mode_xsec_true_binning_file;
        VALORBinning * mode_xsec_true_binning = VALORBinning::BinningFromFile(mode_xsec_true_binning_file,false);
        read_ok = mode_xsec_true_binning->Read(mode_xsec_true_binning_file);
        if(!read_ok) {
           LOG("SampleBinning", pINFO)
              << "Could not read xsec true binning from: " << mode_xsec_true_binning_file;
           return false;
        }
        fXsecTrueBinningUnique.push_back(mode_xsec_true_binning);
        fXsecTrueBinning [im] = mode_xsec_true_binning;
      }    
    }
  } // xsec binning specified independently
  else {
     LOG("SampleBinning", pNOTICE)
       << "No xsec true binning specified. Using default true binning for xsec model.";
    fXsecTrueBinningUnique = fTrueSimpleBinningUnique;
    for (size_t im = 0 ; im <  fReactionModes.size(); im++) {
      fXsecTrueBinning[im] = fTrueSimpleBinning [im];
    }
  }

  // Check that optimised bin edges each lie on a simple bin edge.
  CheckBinEdges(fRecoOptimizedBinning, fRecoSimpleBinning);
  CheckBinEdges(fXsecRecoBinning     , fRecoSimpleBinning);

  // Check that there're more bins in the simple binning scheme than the optimised
  if (fRecoOptimizedBinning->NBins() > fRecoSimpleBinning->NBins()) {
    LOG("SampleBinning",pFATAL) << "Fewer simple reco bins ("<<fRecoSimpleBinning->NBins()
      <<") than optimised bins ("<<fRecoOptimizedBinning->NBins()<<"). You did something wrong.";
    util::Die(-1); return false;
  }
  if (fRecoOptimizedBinning->NBins() > fRecoSimpleBinning->NBins()) {
    LOG("SampleBinning",pFATAL) << "Fewer simple reco bins ("<<fRecoSimpleBinning->NBins()
      <<") than xsec bins ("<<fXsecRecoBinning->NBins()<<"). You did something wrong.";
    util::Die(-1); return false;
  }
  
  // Copy our set of kinevars into the stored vector and then sort it
  fKineVars.insert(fKineVars.end(),kinevars.begin(),kinevars.end());
  std::sort(fKineVars.begin(),fKineVars.end());
  
  xml->FreeDoc(xmldoc);
  delete xml;
  
  return true;
}
//________________________________________________________________________________________
const VALORBinning * SingleSampleKineBinning::TrueSmpBinning(conv::ReacMode_t m) const
{
  if(m == conv::kmUndefined) return 0;
  int im = es.ReacModeIndex(fBeamConf,fSample,m);
  return this->TrueSmpBinning(im);
}
//________________________________________________________________________________________
const VALORBinning * SingleSampleKineBinning::XsecTrueBinning(conv::ReacMode_t m) const
{
  if(m == conv::kmUndefined) return 0;
  int im = es.ReacModeIndex(fBeamConf,fSample,m);
  return this->XsecTrueBinning(im);
}
//________________________________________________________________________________________
void SingleSampleKineBinning::Init(void)
{
  fDetector = conv::kdUndefined; 
  fBeamConf = conv::kbUndefined; 
  fSample   = conv::ksUndefined;

  fRecoOptimizedBinning = nullptr;
  fRecoSimpleBinning    = nullptr;
  fXsecRecoBinning      = nullptr;

  fTrueSimpleBinningUnique.clear();
  fXsecTrueBinningUnique  .clear();
}
//________________________________________________________________________________________
void SingleSampleKineBinning::CleanUp(void)
{
  std::vector<VALORBinning*> deleted_binnings;
  
  std::function<void(VALORBinning*)> delete_binning = [&](VALORBinning* b) {
    // Check we haven't deleted it already
    if (b && std::find(deleted_binnings.begin(),deleted_binnings.end(),b) == deleted_binnings.end()) {
      deleted_binnings.push_back(b);
      delete b;
    }
    b = nullptr;
  };
  
  delete_binning(fRecoOptimizedBinning);  
  delete_binning(fRecoSimpleBinning);  
  delete_binning(fXsecRecoBinning);
  
  for(size_t im = 0; im < fReactionModes.size(); im++) {
    delete_binning(fTrueSimpleBinning [im]);
    delete_binning(fXsecTrueBinning   [im]);
  }
  
  for (auto & b : fTrueSimpleBinningUnique) {delete_binning(b);}
  for (auto & b : fXsecTrueBinningUnique  ) {delete_binning(b);}
  
  fKineVars.clear();
  
  return;
}
//________________________________________________________________________________________
unsigned int SingleSampleKineBinning::GetRecoOptBin(unsigned int reco_smp_bin) const
{
  int ndim = fRecoOptimizedBinning->NDimensions();
  std::vector<double> bin_centres;
  for (int idim = 0 ; idim < ndim ; idim++) {
    double smp_centre = fRecoSimpleBinning->BinCentre(reco_smp_bin,idim);
    bin_centres.push_back(smp_centre);
  }
  return fRecoOptimizedBinning->FindBin(bin_centres);
}
//________________________________________________________________________________________
const std::vector<conv::KineVar_t> & SingleSampleKineBinning::KineVars() const
{
  return fKineVars;
}
//________________________________________________________________________________________
void SingleSampleKineBinning::CheckBinEdges(VALORBinning *opt_binning, VALORBinning *smp_binning) const
{
  for(auto kv_opt : opt_binning->KineVars()) {
    if(!smp_binning->Contains(kv_opt)) { continue; }
    for(unsigned int ibin_opt = 0; ibin_opt < opt_binning->NBins(); ++ibin_opt) {
      // Check if the bin lower edge matches
      bool min_matches = false;
      double bin_min_opt = opt_binning->BinMinimum(ibin_opt, kv_opt);
      for(unsigned int ibin_smp = 0; ibin_smp < smp_binning->NBins(); ++ibin_smp) {
        double bin_min_smp = smp_binning->BinMinimum(ibin_smp, kv_opt);
        double bin_max_smp = smp_binning->BinMaximum(ibin_smp, kv_opt);
        if((bin_min_smp - bin_min_opt < conv::kASmallNum) || (bin_max_smp - bin_min_opt < conv::kASmallNum)) {
          min_matches = true;
          break;
        }
      }
      if(!min_matches) {
        LOG("SampleBinning",pFATAL) << "The optimised lower bin edge " << bin_min_opt
                                    << " for kinevar = " << util::AsString(kv_opt)
                                    << " does not lie on a simple bin edge for binning = " << opt_binning->GetName();
        util::Die(-1);
      }

      // Check if the bin upper edge matches
      bool max_matches = false;
      double bin_max_opt = opt_binning->BinMaximum(ibin_opt, kv_opt);
      for(unsigned int ibin_smp = 0; ibin_smp < smp_binning->NBins(); ++ibin_smp) {
        double bin_min_smp = smp_binning->BinMinimum(ibin_smp, kv_opt);
        double bin_max_smp = smp_binning->BinMaximum(ibin_smp, kv_opt);
        if((bin_min_smp - bin_max_opt < conv::kASmallNum) || (bin_max_smp - bin_max_opt < conv::kASmallNum)) {
          max_matches = true;
          break;
        }
      }
      if(!max_matches) {
        LOG("SampleBinning",pFATAL) << "The optimised upper bin edge " << bin_min_opt
                                    << " for kinevar = " << util::AsString(kv_opt)
                                    << " does not lie on a simple bin edge for binning = " << opt_binning->GetName();
        util::Die(-1);
      }
    }
  }
}
//________________________________________________________________________________________

//________________________________________________________________________________________
/*

   Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//________________________________________________________________________________________

#include <cassert>

#include <TFile.h>
#include <TObjArray.h>
#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TSystem.h>
#include <TPRegexp.h>

// fmwk
#include "valor/Conventions/Opt.h"
#include "valor/Conventions/Names.h"
#include "valor/FitElements/GenericSingleSampleFitData.h"
#include "valor/FitElements/GenericSingleSampleFitModel.h"
#include "valor/FitCore/FitModelABC.h"
#include "valor/FitCore/FitParamList.h"
#include "valor/FitCore/PhysHypothesis.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/UtlStyle.h"
#include "valor/Utilities/UtlNumStat.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Messenger/Messenger.h"
#include "valor/KineBinning/UtlBinning.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
GenericSingleSampleFitData::GenericSingleSampleFitData() :
  SingleSampleFitDataABC(),
  fBinning(nullptr),
  fFiducialMass(-99999.),
  fPOT(-99999.),
  fGSSFM(nullptr),
  fDefVALORArrayFormat("data_vf")
{
  this->Init();
}
//________________________________________________________________________________________
GenericSingleSampleFitData::~GenericSingleSampleFitData() 
{
  this->Clean();
}
//________________________________________________________________________________________
string GenericSingleSampleFitData::Name(void) const
{  
  return fName;
}
//___l_____________________________________________________________________________________
bool GenericSingleSampleFitData::Read(string file, Option_t * /*opt*/)
{

  // Set various parameters from the associated GSSFM
  if(!fGSSFM){
    VLOG("GenericSingleSampleFitData",pNOTICE)
      << "The associated model has not been set prior to calling Read, why is this?";
  }

  // This requires the binning, however we end up getting it
  this->Book();

  // Set up the initial XML reader
  VLOG("GenericSingleSampleFitData", pNOTICE)
    << "Reading configuration from XML file [" << file << "]";

  // string file_fullpath = util::CompleteConfigPath(file);
  TString ts_file = util::CompleteConfigPath(file).c_str();

  // Parse the input XML configuration file
  TXMLEngine* xml = new TXMLEngine;
  XMLDocPointer_t xmldoc = xml->ParseFile(ts_file.Data());
  if (xmldoc==0) {
    VLOG("GenericSingleSampleFitData",pERROR)
      << "Can not parse input XML file [" << ts_file << "]";
    delete xml;
    util::Die(1); return false;
  }
  XMLNodePointer_t root_node = xml->DocGetRootElement(xmldoc);

  // Using input in VALOR DST format?
  // If use_dst=true, read the ROOT TTree and fill-in the VALOR data arrays.
  bool use_dst = util::XML2Bool(xml,root_node,"data/using_dst_format");
  bool use_varray = util::XML2Bool(xml,root_node,"data/using_varray_format");
  bool use_hist = util::XML2Bool(xml,root_node,"data/using_hist_format");
  if(use_dst) {
    VLOG("GenericSingleSampleFitData",pINFO)
      << "Populating VALOR data arrays from VALOR DSTs";
    string node_name = "data/dst/" + util::AsString(fBeam, "lowercase");
    TTree * data_dst = util::XML2TTree (xml,root_node,node_name);
    if (!data_dst) {
      VLOG("GenericSingleSampleFitData",pFATAL) << "Failed to read DST tree";
      util::Die(-1); return false;
    }
    this->FillFromDST(data_dst);
    delete data_dst;
  }
  // If use_dst=false, read in the VALOR data arrays directly.
  // In this case, the data arrays are pre-binned so the binning can not change.
  // Make sure the arrays you read in correspond to the expected binning.
  //editsstart
  else if(use_hist){
    VLOG("GenericSingleSampleFitData",pNOTICE) << "Reading data stored in histograms";
    // -------------------------------------------------
    //convert to varray then fill from array
    string node_name = "data/hist/" + util::AsString(fBeam, "lowercase"); //need to be careful that config/data xmls are set up right for each different type
    string node_binning = "data/default_binning/file";
    string node_binning_scheme = "data/default_binning/scheme";
    TH1D * data_hist = util::XML2TH1D (xml,root_node,node_name);
    if (!data_hist){
      VLOG("GenericSingleSampleFitData",pFATAL) << "Failed to read histogram";
      util::Die(-1); return false;
    }
    //check binning:
    vector<double> config_low_edges = util::XML2BinLowEdges(xml,root_node,node_binning,node_binning_scheme);
    if(config_low_edges.size() != data_hist->GetXaxis()->GetNbins()){
      VLOG("GenericSingleSampleFitData",pFATAL) << "Histogram and config binning do not match (size)";
      util::Die(-1);
    }
    for(int i = 0; i < config_low_edges.size(); i++){
      if(config_low_edges[i] != data_hist->GetXaxis()->GetBinLowEdge(i+1)){
        VLOG("GenericSingleSampleFitData",pFATAL) << "Histogram and config binning do not match (edges)";
        util::Die(-1);
      }
    }
    
    VALORArray * data_hist2varray = new VALORArray(data_hist->GetXaxis()->GetNbins());
    for(int i=1; i<=data_hist->GetXaxis()->GetNbins();i++){
      data_hist2varray->SetEntry(data_hist->GetBinContent(i),i-1);
    }
    this->FillFromVArray(data_hist2varray);
    delete data_hist2varray;
    delete data_hist;
  } 
  else if(use_varray){
    VLOG("GenericSingleSampleFitData",pNOTICE) << "Reading data stored VALOR data arrays (check your own binning schemes)";
    // -------------------------------------------------
    string node_name = "data/varray/" + util::AsString(fBeam, "lowercase");
    VALORArray * data_varray = new VALORArray(util::XML2VArray (xml,root_node,node_name));  //to make XML2VArray in UtlXMl
    if (!data_varray){
      VLOG("GenericSingleSampleFitData",pFATAL) << "Failed to read VALOR Array";
      util::Die(-1); return false;
    }
    this->FillFromVArray(data_varray);
    delete data_varray;
  } else{
    VLOG("GenericSingleSampleFitData",pFATAL) << "Data format not accepted (yet)";
    util::Die(-1); return false;
    // -------------------------------------------------
  }
  // Release memory
  xml->FreeDoc(xmldoc);
  delete xml;

  VLOG("GenericSingleSampleFitData",pNOTICE) << "Done reading data";

  return true;
}
//________________________________________________________________________________________
bool GenericSingleSampleFitData::Write(TObjArray * array, const char * tag, Option_t * write_opt)
{
  VLOG("GenericSingleSampleFitData", pINFO)  
    << "Writing results to input object array...";
  VLOG("GenericSingleSampleFitData", pDEBUG) 
    << "tag = " << tag << ", write option = " << write_opt;

  string opt = (write_opt) ? write_opt : fDefVALORArrayFormat;

  if(opt.find("all") != string::npos) {
    opt = "data_vf,data_rf +slices +SB";
    VLOG("GenericSingleSampleFitData", pDEBUG) << "Write option replaced with = " << opt;
  }

  if(!array) {
    VLOG("GenericSingleSampleFitData", pERROR) << "Null input object array.";
    return false;
  }

  const SingleSampleKineBinning & binning = *fBinning;
  const VALORBinning & reco_smp = *(binning.RecoSmpBinning());
  const VALORBinning & reco_opt = *(binning.RecoOptBinning());
  
  //
  // Write out data distributions (VALORArray or bare ROOT format)
  //

  if(opt.find("data_vf") != string::npos || opt.find("data_rf") != string::npos) {

    // binning optimised for fit CPU performance
    if(fDataObNull)  {
      VLOG("GenericSingleSampleFitData",pFATAL) << "Trying to write null data.";
      util::Die(-1) ; return false;
    }
    // Add data in VALOR array format
    if(opt.find("data_vf") != string::npos) {
      assert(fDataOb);
      fDataOb->SetName(TString::Format("%s_%s_%s",tag,"data_ob_vf",fName.c_str()).Data());
      VLOG("GenericSingleSampleFitData", pDEBUG) << "Adding: " << fDataOb->GetName();
      array->Add(fDataOb);
    }

    // Add data in ROOT histogram format
    if(opt.find("data_rf") != string::npos) {
      assert(fDataOb);
      const std::vector<KineVar_t>  & reco_vars = reco_opt.KineVars();
      if(reco_vars.size() == 1) {
        conv::KineSpace_t ks(util::KineSpaceFromKineVar(reco_vars[0]));
        TH1 * hob = util::binning::Make1DHistogram(*fDataOb,binning,ks,false);
        if(hob) {
          TString hobname = TString::Format("%s_%s_%s",tag,"data_ob_rf",fName.c_str());
          hob->SetName(hobname.Data());
          array->Add(hob);
        }
      }
      else if(reco_vars.size() == 2) {
        conv::KineSpace_t ks = util::KineSpaceFromKineVars(reco_vars);
        if(ks != conv::kspUndefined) {
          TH2 *hob = util::binning::Make2DHistogram(*fDataOb,&reco_opt,reco_vars);
          if(hob) {
            TString hobname = TString::Format("%s_%s_%s",tag,"data_ob_rf",fName.c_str());
            hob->SetName(hobname.Data());
            array->Add(hob);
          }
        }
      }
      else {
        VLOG("GenericSingleSampleFitData", pWARN)
          << "Tried writing data to ROOT format, but number of kine vars != 1 or 2 (" << fDataOb->GetName() << ")";
      }

      // Write 1d slices for the total in reco only
      if (opt.find("slices") != string::npos) {
        for (const conv::KineVar_t kv : reco_vars) {
          conv::KineSpace_t ks(util::KineSpaceFromKineVar(kv));
          if (ks == conv::kspUndefined) {continue;}
          TH1 * hob = util::binning::Make1DHistogram(*fDataOb,binning,ks,false);
          if(hob) {
            TString hobname = TString::Format("%s_%s_%s_%s",tag,"data_ob_rf_slice",fName.c_str(),util::AsString(kv).c_str());
            hob->SetName(hobname.Data());
            array->Add(hob);
          }
        }
      }

    }

    // regular/fine binning for plotting
    if(opt.find("+SB") != string::npos) {
      assert(!fDataSbNull);

      // Add data in VALOR array format
      if(opt.find("data_vf") != string::npos) {
        assert(fDataSb);
        fDataSb->SetName(TString::Format("%s_%s_%s",tag,"data_sb_vf",fName.c_str()));
        VLOG("GenericSingleSampleFitData", pDEBUG) << "Adding: " << fDataSb->GetName();
        array->Add(fDataSb);
      }//data_vf

      // Add data in ROOT histogram format
      if(opt.find("data_rf") != string::npos) {
        assert(fDataSb);
        const std::vector<KineVar_t>  & reco_vars = reco_smp.KineVars();
        if(reco_vars.size() == 1) {
          conv::KineSpace_t ks(util::KineSpaceFromKineVar(reco_vars[0]));
          TH1 * hsb = util::binning::Make1DHistogram(*fDataSb,binning,ks,false);
          if(hsb) {
            TString hsbname = TString::Format("%s_%s_%s",tag,"data_sb_rf",fName.c_str());
            hsb->SetName(hsbname.Data());
            array->Add(hsb);
          }
        }
        else if(reco_vars.size() == 2) {
          conv::KineSpace_t ks = util::KineSpaceFromKineVars(reco_vars);
          if(ks != conv::kspUndefined) {
            TH2 *hsb = util::binning::Make2DHistogram(*fDataSb,&reco_smp,reco_vars);
            if(hsb) {
              TString hsbname = TString::Format("%s_%s_%s",tag,"data_sb_rf",fName.c_str());
              hsb->SetName(hsbname.Data());
              array->Add(hsb);
            }
          }
        }
        else {
          VLOG("GenericSingleSampleFitData", pWARN)
            << "Tried writing data to ROOT format, but number of kine vars != 1 or 2 (" << fDataSb->GetName() << ")";
        }

        // Write 1d slices for the total in reco only
        if (opt.find("slices") != string::npos) {
          for (const conv::KineVar_t kv : reco_vars) {
            conv::KineSpace_t ks(util::KineSpaceFromKineVar(kv));
            if (ks == conv::kspUndefined) {continue;}

            TH1 * hsb = util::binning::Make1DHistogram(*fDataSb,binning,ks,true);
            if(hsb) {
              TString hsbname = TString::Format("%s_%s_%s_%s",tag,"data_sb_rf_slice",fName.c_str(),util::AsString(kv).c_str());
              hsb->SetName(hsbname.Data());
              array->Add(hsb);
            }
          }
        }

      }

    }//+SB
  }//data_vf, data_rf

  VLOG("GenericSingleSampleFitData", pINFO)  << "Done!";

  return true;
}
//________________________________________________________________________________________
bool GenericSingleSampleFitData::Write(
    const char * filename, const char * tag, const char * write_opt, Option_t * file_opt)
{
  VLOG("GenericSingleSampleFitData",pNOTICE) 
    << "Writing results to file: [" << filename << "]";

  TObjArray * array = new TObjArray();
  this->Write(array,tag,write_opt);

  Option_t * used_file_opt = (file_opt == 0) ? "recreate" : file_opt;
  TFile f(filename, used_file_opt);
  array->Write();
  f.Close();

  return true;
}
//________________________________________________________________________________________
bool GenericSingleSampleFitData::Write(util::ToyStream* stream)
{
   Option_t* write_opt = stream->GetVALORArrayFormat();
   string opt = (write_opt) ? write_opt : fDefVALORArrayFormat;
   assert(fDataOb);
   fDataOb->SetName(TString::Format("%s_%s", "data_ob_vf", fName.c_str()));
   stream->Write(fDataOb);

   if(opt.find("+SB") != string::npos)
   {
      assert(fDataSb);
      fDataSb->SetName(TString::Format("%s_%s", "data_sb_vf", fName.c_str()));
      stream->Write(fDataSb);
   } //+SB

   return true;
}
//________________________________________________________________________________________
bool GenericSingleSampleFitData::Read(util::ToyStream* stream)
{
  Option_t* read_opt = stream->GetVALORArrayFormat();
  string opt = (read_opt) ? read_opt : fDefVALORArrayFormat;
  assert(fDataOb);
  fDataOb->SetName(TString::Format("%s_%s", "data_ob_vf", fName.c_str()));
  stream->Read(fDataOb);
  fDataObNull = false;

  if(opt.find("+SB") != string::npos)
  {
    assert(fDataSb);
    fDataSb->SetName(TString::Format("%s_%s", "data_sb_vf", fName.c_str()));
    stream->Read(fDataSb);
    fDataSbNull = false;
  } //+SB

  return true;
}
//________________________________________________________________________________________
long GenericSingleSampleFitData::StreamSize(Option_t* opt_in) const
{
  if (!fBinning) {
    VLOG("GenericSingleSampleFitData", pERROR) << "Must call SetAssociatedModel() and SetFromModel() with FitModel "
                                              << "before attempting to determine the toy size.";
    return false;
  }

  string opt = (opt_in) ? opt_in : fDefVALORArrayFormat;
  long size = 0;

  assert(fDataOb);
  size += fDataOb->StreamSize();

  if(opt.find("+SB") != string::npos)
  {
    assert(fDataSb);
    size += fDataSb->StreamSize();
  } //+SB

  return size;
}
//________________________________________________________________________________________
bool GenericSingleSampleFitData::Generate(
    FitModelABC * model_abc, const FitParamList & fp, bool fluctuate)
{
  VLOG("GenericSingleSampleFitData",pINFO) 
    << "Generating a toy-MC expt with the following params: " << fp;
  VLOG("GenericSingleSampleFitData",pINFO)
    << "Stat. fluctuations are turned-" << ((fluctuate)? "ON" : "OFF");
  
  bool ok = model_abc->Generate(fp,"+SB");
  if(!ok) {
    VLOG("GenericSingleSampleFitData",pERROR) 
      << "Could not generate predictions with model: " << model_abc->Name()
      << " and input params: " << fp;
    return false;
  }
  
  return this->Generate(model_abc,fluctuate);
}
//________________________________________________________________________________________
bool GenericSingleSampleFitData::Generate(
    FitModelABC * model_abc, const PhysHypothesis & ph, bool fluctuate)
{
  VLOG("GenericSingleSampleFitData",pINFO) 
    << "Generating a toy-MC expt with the following params: " << ph;
  VLOG("GenericSingleSampleFitData",pINFO)
    << "Stat. fluctuations are turned-" << ((fluctuate)? "ON" : "OFF");

  bool ok = model_abc->Generate(ph,"+SB");
  if(!ok) {
    VLOG("GenericSingleSampleFitData",pERROR) 
      << "Could not generate predictions with model: " << model_abc->Name()
      << " with systematic tweaks: " << model_abc->Tweaks()
      << " and input params: " << ph;
    return false;
  }

  return this->Generate(model_abc,fluctuate);
}
//________________________________________________________________________________________
bool GenericSingleSampleFitData::Generate(FitModelABC * model_abc, bool fluctuate, Option_t * option)
{

  fIsFakeData = true;

  // Set various parameters from the associated GSSFM
  if(!fGSSFM){
    VLOG("GenericSingleSampleFitData",pNOTICE)
      << "The associated model has not been set prior to calling Generate, why is this?";
    this->SetAssociatedModel(model_abc);
    this->SetFromModel();
  }

  // Book the information from the model for binning etc
  this->Book();

  bool update_sb = false;
  if(option) {
    string opt = option;
    if(opt.find("+SB") != string::npos) {
      update_sb = true;
    }
  }
  Option_t * write_opt = "pdft_vf";
  if(update_sb) { write_opt = "pdft_vf +SB"; }

  TObjArray array;
  model_abc->Write(&array, "curr", write_opt);

  this->Zero("all");
#ifndef __VALOR_FMWK_PENALTY_ONLY__
  VLOG("GenericSingleSampleFitData",pINFO) 
    << "Generating toy-MC for: " 
    << fName;

  if(!update_sb) {
    // Updating only the templates using the optimized binning
    TString ob_name = TString::Format("curr_pdft_ob_vf_%s",fModelName.c_str());
    VALORArray * pdf_ob = (VALORArray*) array.FindObject(ob_name.Data());
    assert(pdf_ob);
    const int nbins_ob = pdf_ob->GetSize();
    assert(nbins_ob==fDataOb->GetSize());

    for(int ibin_ob = 0 ; ibin_ob < nbins_ob ; ibin_ob ++) {
      double nmc   = pdf_ob->GetArray()[ibin_ob];
      double ndata = (fluctuate) ? Rnd::Instance()->RndToyMCstat().Poisson(nmc) : nmc;
      fDataOb->GetArray()[ibin_ob] = ndata;

      VLOG("GenericSingleSampleFitData",pDEBUG)
        << "(reco_ob_idx = "<<ibin_ob
        << "): N_{MC} = " << nmc << ", N_{Data} = " << ndata;
    }//ibin_ob
  }
  else {
    // Updating the templates using both the simple and optimised binning
    TString sb_name = TString::Format("curr_pdft_sb_vf_%s",fModelName.c_str());
    VALORArray * pdf_sb = (VALORArray*) array.FindObject(sb_name.Data());
    assert(pdf_sb);
    const int nbins_sb = pdf_sb->GetSize();
    assert(nbins_sb==fDataSb->GetSize());

    for(int ibin_sb = 0 ; ibin_sb < nbins_sb ; ibin_sb ++) {
      double nmc   = pdf_sb->GetArray()[ibin_sb];
      double ndata = (fluctuate) ? Rnd::Instance()->RndToyMCstat().Poisson(nmc) : nmc;
      fDataSb->GetArray()[ibin_sb] = ndata;

      VLOG("GenericSingleSampleFitData",pDEBUG)
        << "(reco_sb_idx = "<<ibin_sb
        << "): N_{MC} = " << nmc << ", N_{Data} = " << ndata;

      // Map to distributions with optimized binning
      //
      unsigned int ibin_ob = fBinning->GetRecoOptBin(ibin_sb);
      fDataOb->GetArray()[ibin_ob] += ndata;
    }//ibin_sb
  }
#endif //__VALOR_FMWK_PENALTY_ONLY__
 
   VLOG("GenericSingleSampleFitData",pINFO) << fName << ": N_{Data} SB = " <<
      fDataSb->Integral() << " ; N_{Data} OB = " << fDataOb->Integral();

  return true;
}
//________________________________________________________________________________________
int GenericSingleSampleFitData::NumOfBins(Option_t * /*opt*/) const
{
  const VALORBinning * reco_ob = fBinning->RecoOptBinning();
  return reco_ob->NBins();
}
//________________________________________________________________________________________
double GenericSingleSampleFitData::GetFitStatistic(
    FitModelABC* model_abc, const FitParamList & fp, Option_t * opt, std::map<std::string,double> * likelihood_components)
{
  bool ok = model_abc->Generate(fp);
  assert(ok);

  return this->GetFitStatistic(model_abc,opt, likelihood_components);
}
//________________________________________________________________________________________
double GenericSingleSampleFitData::GetFitStatistic(
    FitModelABC * model_abc, const PhysHypothesis & ph, Option_t * opt, std::map<std::string,double> * likelihood_components)
{
  bool ok = model_abc->Generate(ph);
  assert(ok);

  return this->GetFitStatistic(model_abc,opt, likelihood_components);  
}
//________________________________________________________________________________________
double GenericSingleSampleFitData::GetFitStatistic(
    FitModelABC * model_abc, Option_t * /*opt*/, std::map<std::string,double> * likelihood_components) 
{
  TObjArray * array = new TObjArray();
  model_abc->Write(array, "curr", "pdft_vf");

  // Calculate statistic
  double S = 0;
  
  // Counters for total number of events to we can do rate-only
  double tot_nmc   = 0.;
  double tot_ndata = 0.;
  
  TString name = TString::Format("curr_pdft_ob_vf_%s",fModelName.c_str());
  VALORArray * pdf = (VALORArray*) array->FindObject(name.Data());
  assert(pdf);
  assert(pdf->GetSize() == fDataOb->GetSize());
  for(int ibin = 0; ibin < pdf->GetSize(); ibin++) {
    double nmc   = pdf    ->GetArray()[ibin];
    double ndata = fDataOb->GetArray()[ibin];
    tot_nmc   += nmc;
    tot_ndata += ndata;
    double dS = util::ChisqPoisson(ndata, nmc);
    S += dS;
  }//ier
  S = TMath::Max(0.,S);
  
  double S_rate_only = TMath::Max(0.,util::ChisqPoisson(tot_ndata, tot_nmc));

  VLOG("GenericSingleSampleFitData",pINFO)
    << fName<<" - Current fit statistic (rate+shape) = " << S;
  VLOG("GenericSingleSampleFitData",pINFO)
    << fName<<" - Current fit statistic (rate only) = " << S_rate_only
    << ", from N data = " << tot_ndata << ", N MC = " << tot_nmc;
  
  // Write our contribution to the likelihood
  if (likelihood_components) {
    TString likelihood_name = TString::Format("sample_%s",fName.c_str());
    TString likelihood_name_rate_only = TString::Format("sample_rate_only_%s",fName.c_str());
    (*likelihood_components)[likelihood_name.Data()] = S;
    (*likelihood_components)[likelihood_name_rate_only.Data()] = S_rate_only;
  }
    
  delete array;

  return S;
}
//________________________________________________________________________________________
void GenericSingleSampleFitData::Init(void)
{
  VLOG("GenericSingleSampleFitData",pDEBUG) << "Initializing...";

  fName         = "none";
  fModelName    = "none";

  fDetector     = kdUndefined;
  fBeam         = kbUndefined;
  fSample       = ksUndefined;

  fDataObNull   = true;
  fDataOb       = 0;
  fDataSbNull   = true;
  fDataSb       = 0;

  fIsFakeData   = false;
  fGSSFM        = nullptr;
  fBinning      = 0;
  fPOT          = 0;
  fFiducialMass = 0;

  fDefVALORArrayFormat = "data_vf";
}
//________________________________________________________________________________________
void GenericSingleSampleFitData::SetAssociatedModel(FitModelABC *model_abc)
{
  fGSSFM = dynamic_cast<GenericSingleSampleFitModel*>(model_abc);
  VLOG("GenericSingleSampleFitData",pNOTICE) << " Set the model with name: " << fGSSFM->Name();
}
//________________________________________________________________________________________
void GenericSingleSampleFitData::SetFromModel(void)
{
  assert(fGSSFM);
  fBinning      = fGSSFM->GetBinning();
  fPOT          = fGSSFM->GetPOT();
  fFiducialMass = fGSSFM->GetFiducialMass();

  VLOG("GenericSingleSampleFitData",pNOTICE) << " POT set from model : " << fPOT;
}
//________________________________________________________________________________________
void GenericSingleSampleFitData::Book(void)
{
  VLOG("GenericSingleSampleFitData",pDEBUG) << "Booking...";
  assert(fBinning);
  assert(fGSSFM  );

  assert(fDetector != kdUndefined);
  assert(fBeam     != kbUndefined);
  assert(fSample   != ksUndefined);
  assert(fKSp      != kspUndefined);

  fName      = util::nm::DSetName(fDetector, fBeam, fSample, fKSp);
  fModelName = util::nm::MdlName (fDetector, fBeam, fSample, fKSp);
  
  const VALORBinning * reco_ob = fBinning->RecoOptBinning();
  const VALORBinning * reco_sb = fBinning->RecoSmpBinning();

  fDataObNull = false;
  fDataOb     = new VALORArray(reco_ob->NBins());
  fDataSbNull = false;
  fDataSb     = new VALORArray(reco_sb->NBins());
}
//________________________________________________________________________________________
void GenericSingleSampleFitData::Clean(void)
{
  VLOG("GenericSingleSampleFitData",pDEBUG) << "Cleaning-up...";

  if (!fDataObNull) {
    fDataObNull = true;
    if (fDataOb) {delete fDataOb;}
    fDataOb = 0;
  }
  if (!fDataSbNull) {
    fDataSbNull = true;
    if(fDataSb) {delete fDataSb;}
    fDataSb = 0;
  }
}
//________________________________________________________________________________________
void GenericSingleSampleFitData::Zero(Option_t * opt)
{
  VLOG("GenericSingleSampleFitData",pDEBUG) << "Zeroing-out (option: " << opt << ") ...";

  string option = (opt) ? opt : "all";

  if (!fDataObNull) { fDataOb->SetAll(0.); }
  if (!fDataSbNull) { fDataSb->SetAll(0.); }
}
//________________________________________________________________________________________
//editsstart
void GenericSingleSampleFitData::FillFromVArray (VALORArray * a)
{
  if(!a) return;
  VALORArray * array = new VALORArray(*a);
  fDataSb = array;
  fDataOb = array;
  fDataObNull = false;
  fDataSbNull = false;
}
//editsend
//________________________________________________________________________________________
void GenericSingleSampleFitData::FillFromDST (TTree * t)
{
  if(!t) return;

  // Get a list of all the kinematic variables we care about
  const std::vector<conv::KineVar_t> & kinevars = fBinning->RecoKineVars();

  int    sample        = 0;
  int    beam          = 0;
  double wght          = 1.;
  double wght_pot      = 1.;

  std::map<KineVar_t,double> kinematics;

  // Set up a list of the branches associated with each kinematic variable
  std::map<KineVar_t,std::string> kinematic_branch_names;
  for (const KineVar_t & kv : kinevars) {
    kinematic_branch_names[kv] = util::DSTBranchName(kv);
  }

  int bstat_sample     = TTree::kNoCheck;
  int bstat_beam       = TTree::kNoCheck;
  int bstat_wght       = TTree::kNoCheck;
  int bstat_wght_pot   = TTree::kNoCheck;
  std::map<KineVar_t,int> bstat_kinematics;

  bstat_sample = t->SetBranchAddress("sample",   &sample);
  bstat_beam   = t->SetBranchAddress("beam",     &beam);
  bstat_wght   = t->SetBranchAddress("wght",     &wght);

  // Note: Could have called t->SetBranchAddress() directly, but this avoids a non-fatal error being
  //       thrown if the wght_pot branch doesn't exist (which it won't when reading real data)
  bool br_wght_pot_exists = (t->GetListOfBranches()->FindObject("wght_pot") != nullptr);
  if(br_wght_pot_exists){
    bstat_wght_pot = t->SetBranchAddress("wght_pot", &wght_pot);
  }

  fIsFakeData = (bstat_wght_pot == TTree::kMatch);

  bool any_bad_status = (bstat_sample || bstat_beam || bstat_wght);

  for(const conv::KineVar_t & kv : kinevars) {
    bool bstat = t->SetBranchAddress(kinematic_branch_names[kv].c_str(),&kinematics[kv]);
    any_bad_status = any_bad_status || bstat;
  }

  if (any_bad_status) {
    VLOG("GenericSingleSampleFitData", pWARN)
      << "MiniDST branches not found";
    util::Die(-1);
  }

  if(bstat_wght) {
    VLOG("GenericSingleSampleFitData",pINFO)
      << fName<<" - No wght branch found.";
  }

  Long_t nev = t->GetEntries();
  VLOG("GenericSingleSampleFitData",pINFO)
    << "Input VALOR DST has " << nev << " entries";

  double tree_weight = t->GetWeight();

  VLOG("GenericSingleSampleFitData",pNOTICE)
    << "Input VALOR DST has weight "<<tree_weight;

  const VALORBinning * reco_ob = fBinning->RecoOptBinning();
  const VALORBinning * reco_sb = fBinning->RecoSmpBinning();

  for(Long_t iev = 0; iev < nev; iev++)
  {
    t->GetEntry(iev);

    if(iev%(nev/20)==0) {
      double pct = (100 * (double)(iev+1)) / (double)nev;
      VLOG("GenericSingleSampleFitData",pINFO)
        << "Reading "<< util::AsString(fBeam)
        <<" data DST: "<< TMath::Nint(pct) << "% done";
    }

    // Get sample index
    Sample_t s = util::SampleFromInt(sample);
    if(s == ksUndefined) {
      VLOG("GenericSingleSampleFitData",pWARN)
        << " ** Undefined sample ID used in data event: " << iev
        << " (value stored in DST: " << sample << ")";
      //util::Die(-1);return;
    }
    //LOG("GenericSingleSampleFitData",pDEBUG) << "Sample = " << sample;
    if(s != fSample) {
      VLOG("GenericSingleSampleFitData",pDEBUG) << "Sample = " << s << ", fSample = " << fSample;
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
      VLOG("GenericSingleSampleFitData",pDEBUG)
        << "Ignoring data event: " << iev
        << " (sample ID: " << s << " "<< util::AsString(s)
        << " not matching one used for configuring this dataset: "
        << fSample <<" "<<util::AsString(fSample) << ")";
#endif
      continue;
    }
    //LOG("GenericSingleSampleFitData",pDEBUG) << "Sample = " << sample;
    BeamConf_t b = util::BeamConfFromInt(beam);
    VLOG("GenericSingleSampleFitData",pDEBUG) << "Beam = " << util::AsString(b);
    if(b != fBeam) {
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
      VLOG("GenericSingleSampleFitData",pDEBUG)
        << "Ignoring data event: " << iev
        << " (beam ID: " << b << " "<< util::AsString(b)
        << " not matching one used for configuring this dataset: "
        << fBeam <<" "<<util::AsString(fBeam) << ")";
#endif
      continue;
    }
    VLOG("GenericSingleSampleFitData",pDEBUG)
      << "Sample and beam passed for event: " << iev;

    // check kinematic values against built-in limits and make vectors
    std::vector<double> reco_vars;
    bool out_of_range = false;
    for (conv::KineVar_t kv : reco_ob->KineVars()) {
      double val = kinematics.at(kv);
      if (val < reco_ob->Minimum(kv) || val > reco_ob->Maximum(kv)) {
        VLOG("GenericSingleSampleFitData",pWARN)
          << "MC event: " << iev << " has reco kinematic value outside range: "
          << util::AsString(kv) << " = " << val
          << ", range used: [" << reco_ob->Minimum(kv) << ", " << reco_ob->Maximum(kv) << "]";
        out_of_range = true;
      }
      reco_vars.push_back(val);
    }
    if (out_of_range) {continue;}
    VLOG("GenericSingleSampleFitData",pDEBUG)
      << "Kinematic values passed for event: " << iev;

    // weight for this entry
    double this_entry = wght;
    if(fIsFakeData) {
      // Exposure for assumed data - Weight MC templates to that exposure
      double data_pot = fPOT;
      // Calculate POT weight (assumed data exposure) / (generated MC exposure)
      double pot_wght = wght_pot * data_pot * tree_weight;
      if(fFiducialMass > 0) {
        pot_wght = pot_wght*fFiducialMass;
      }
      this_entry *= pot_wght;
    }

    // Find the appropriate reconstructed kinematical variable bins
    int reco_bin_sb = reco_sb->FindBin(reco_vars);
    int reco_bin_ob = reco_ob->FindBin(reco_vars);

    VLOG("GenericSingleSampleFitData",pDEBUG)
      << "Inserting to MC teplates --> MC event #" << iev
      << "\n sample       = " << s        << " (" << util::AsString(s) << ")"
      << "\n wght         = " << wght
      << "\n overall wght = " << this_entry
      << "\n reco_bin_ob  = " << reco_bin_ob
      << "\n reco_bin_sb  = " << reco_bin_sb
      << "\n  Reco: " << reco_vars[0] <<" , " <<reco_vars[1];

    double curr_wght_ob = fDataOb->GetEntry(reco_bin_ob);
    curr_wght_ob += this_entry;
    fDataOb->SetEntry(curr_wght_ob, reco_bin_ob);

    double curr_wght_sb = fDataSb->GetEntry(reco_bin_sb);
    curr_wght_sb += this_entry;
    fDataSb->SetEntry(curr_wght_sb, reco_bin_sb);
  }//iev

  fDataObNull = false;
  fDataSbNull = false;
}
//________________________________________________________________________________________
std::string GenericSingleSampleFitData::GetPDFIntegralAsString(Option_t * opt_in)
{
  double integral = -999;

  std::string opt = (opt_in) ? opt_in : "";
  if(opt.find("SB") != string::npos) {
    integral = fDataSb->Integral();
  }
  else {
    integral = fDataOb->Integral();
  }

  TString temp = TString::Format("%s total data PDF integral = %.5lf", this->Name().c_str(), integral);
  std::string to_print = temp.Data();

  return to_print;
}
//________________________________________________________________________________________
util::ToyStream* GenericSingleSampleFitData::InitToyStream(const string file_name, const string read_or_write, string valor_array_format)
{
  valor_array_format = util::RemoveAllWhitespace(valor_array_format);
  if(valor_array_format.empty()) {
    valor_array_format = fDefVALORArrayFormat;
  }

  return new util::ToyStream(file_name, read_or_write, typeid(this), this->Name(),
                             valor_array_format, this->StreamSize());
}
//________________________________________________________________________________________
bool GenericSingleSampleFitData::DataExists(Option_t * opt_in) const
{
  bool exists = (fDataObNull == false);

  std::string opt = (opt_in) ? opt_in : "";
  if(opt.find("SB") != string::npos) {
    exists = exists && (fDataSbNull == false);
  }

  return exists;
}
//________________________________________________________________________________________
void GenericSingleSampleFitData::OverwriteData(VALORArray* replacement_data_ob, VALORArray* replacement_data_sb)
{
  if(replacement_data_ob) {
    if(fDataObNull) {
      VLOG("GenericSingleSampleFitData",pFATAL) << "Trying to overwrite null Ob data arrays.";
      util::Die(-1);
    }
    if(fDataOb->GetSize() != replacement_data_ob->GetSize()) {
      VLOG("GenericSingleSampleFitData",pFATAL) << "Trying to overwrite Ob data array with an array of a different size.";
    }
    if(fDataOb->GetSize() != replacement_data_ob->GetSize()) {
      VLOG("GenericSingleSampleFitData",pFATAL) << "Trying to overwrite Ob data array with an array of a different size.";
      util::Die(-1);
    }
    fDataOb = new VALORArray(*replacement_data_ob);
    if(!fDataOb) {
      VLOG("GenericSingleSampleFitData",pFATAL) << "Could not overwrite Ob data array.";
      util::Die(-1);
    }
  }

  if(replacement_data_sb) {
    if(fDataSbNull) {
      VLOG("GenericSingleSampleFitData",pFATAL) << "Trying to overwrite null Sb data arrays.";
      util::Die(-1);
    }
    if(fDataSb->GetSize() != replacement_data_sb->GetSize()) {
      VLOG("GenericSingleSampleFitData",pFATAL) << "Trying to overwrite Sb data array with an array of a different size.";
      util::Die(-1);
    }
    fDataSb = new VALORArray(*replacement_data_sb);
    if(!fDataSb) {
      VLOG("GenericSingleSampleFitData",pFATAL) << "Could not overwrite Sb data array.";
      util::Die(-1);
    }
  }
}

//________________________________________________________________________________________

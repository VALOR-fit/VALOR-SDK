//________________________________________________________________________________________
/*

   Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#include <cassert>

#include <TFile.h>
#include <TObjArray.h>
#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TSystem.h>

// fmwk
#include "valor/FitCore/GenericSingleSampleFitData.h"

#include "valor/Conventions/Names.h"
#include "valor/FitCore/GenericSingleSampleFitModel.h"
#include "valor/FitCore/FitModelABC.h"
#include "valor/FitCore/FitParamList.h"
#include "valor/FitCore/PhysHypothesis.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/UtlStyle.h"
#include "valor/Utilities/UtlNumStat.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Messenger/Messenger.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
GenericSingleSampleFitData::GenericSingleSampleFitData() :
  SingleSampleFitDataABC(),
  fBinning(nullptr)
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
//________________________________________________________________________________________
bool GenericSingleSampleFitData::Read(string /*file*/, Option_t * /*opt*/)
{
  
  if (!fBinning) {
    LOG("GenericSingleSampleFitData", pERROR) << "Must call Generate() with FitModel before attempting to read file.";
    return false;
  }

  return true;
}
//________________________________________________________________________________________
bool GenericSingleSampleFitData::Write(TObjArray * array, const char * tag, Option_t * write_opt)
{
  LOG("GenericSingleSampleFitData", pINFO)  
    << "Writing results to input object array...";
  LOG("GenericSingleSampleFitData", pDEBUG) 
    << "tag = " << tag << ", write option = " << write_opt;

  string opt = (write_opt) ? write_opt : "data_vf";

  if(opt.find("all") != string::npos) {
    opt = "data_vf,data_rf +slices +SB";
    LOG("GenericSingleSampleFitData", pDEBUG) << "Write option replaced with = " << opt;
  }

  if(!array) {
    LOG("GenericSingleSampleFitData", pERROR) << "Null input object array.";
    return false;
  }
  
  //
  // Write out data distributions (VALORArray or bare ROOT format)
  //

  if(opt.find("data_vf") != string::npos || opt.find("data_rf") != string::npos) {

    // binning optimised for fit CPU performance
    if(fDataObNull)  {
      LOG("GenericSingleSampleFitData",pFATAL) << "Trying to write null data.";
      util::Die(-1) ; return false;
    }
    // Add data in VALOR array format
    if(opt.find("data_vf") != string::npos) {
      assert(fDataOb);
      fDataOb->SetName(TString::Format("%s_%s_%s",tag,"data_ob_vf",fName.c_str()).Data());
      LOG("GenericSingleSampleFitData", pDEBUG) << "Adding: " << fDataOb->GetName();
      array->Add(fDataOb);
    }

    // regular/fine binning for plotting
    if(opt.find("+SB") != string::npos) {
      assert(!fDataSbNull);
      // Add data in VALOR array format
      if(opt.find("data_vf") != string::npos) {
        assert(fDataSb);
        fDataSb->SetName(TString::Format("%s_%s_%s",tag,"data_sb_vf",fName.c_str()));
        LOG("GenericSingleSampleFitData", pDEBUG) << "Adding: " << fDataSb->GetName();
        array->Add(fDataSb);
      }//data_vf
    }//+SB
  }//data_vf, data_rf

  LOG("GenericSingleSampleFitData", pINFO)  << "Done!";

  return true;
}
//________________________________________________________________________________________
bool GenericSingleSampleFitData::Write(
    const char * filename, const char * tag, const char * write_opt, Option_t * file_opt)
{
  LOG("GenericSingleSampleFitData",pNOTICE) 
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
bool GenericSingleSampleFitData::Write(std::ofstream& stream, Option_t * write_opt)
{
   string opt = (write_opt) ? write_opt : "";
   assert(fDataOb);
   fDataOb->SetName(TString::Format("%s_%s", "data_ob_vf", fName.c_str()));
   fDataOb->WriteStream(stream);

   if(opt.find("+SB") != string::npos)
   {
      assert(fDataSb);
      fDataSb->SetName(TString::Format("%s_%s", "data_sb_vf", fName.c_str()));
      fDataSb->WriteStream(stream);
   } //+SB

   return true;
}
//________________________________________________________________________________________
bool GenericSingleSampleFitData::Generate(
    FitModelABC * model_abc, const FitParamList & fp, bool fluctuate)
{
  LOG("GenericSingleSampleFitData",pINFO) 
    << "Generating a toy-MC expt with the following params: " << fp;
  LOG("GenericSingleSampleFitData",pINFO)
    << "Stat. fluctuations are turned-" << ((fluctuate)? "ON" : "OFF");
  
  bool ok = model_abc->Generate(fp,"+SB");
  if(!ok) {
    LOG("GenericSingleSampleFitData",pERROR) 
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
  LOG("GenericSingleSampleFitData",pINFO) 
    << "Generating a toy-MC expt with the following params: " << ph;
  LOG("GenericSingleSampleFitData",pINFO)
    << "Stat. fluctuations are turned-" << ((fluctuate)? "ON" : "OFF");

  bool ok = model_abc->Generate(ph,"+SB");
  if(!ok) {
    LOG("GenericSingleSampleFitData",pERROR) 
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
  
  GenericSingleSampleFitModel * model = dynamic_cast<GenericSingleSampleFitModel*>(model_abc);
  
  if(fBinning!=model->GetBinning()){
    this->Clean();
    fBinning = model->GetBinning();
    this->Book();
  }

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
  LOG("GenericSingleSampleFitData",pINFO) 
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

      LOG("GenericSingleSampleFitData",pDEBUG)
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

      LOG("GenericSingleSampleFitData",pDEBUG)
        << "(reco_sb_idx = "<<ibin_sb
        << "): N_{MC} = " << nmc << ", N_{Data} = " << ndata;

      // Map to distributions with optimized binning
      //
      unsigned int ibin_ob = fBinning->GetRecoOptBin(ibin_sb);
      fDataOb->GetArray()[ibin_ob] += ndata;
    }//ibin_sb
  }
#endif //__VALOR_FMWK_PENALTY_ONLY__
 
   LOG("GenericSingleSampleFitData",pINFO) << fName << ": N_{Data} SB = " <<
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

  LOG("GenericSingleSampleFitData",pINFO)
    << fName<<" - Current fit statistic (rate+shape) = " << S;
  LOG("GenericSingleSampleFitData",pINFO)
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
  LOG("GenericSingleSampleFitData",pDEBUG) << "Initializing...";

  fName = "none";

  fDetector = kdUndefined;
  fBeam     = kbUndefined;
  fSample   = ksUndefined;

  fDataObNull= true;
  fDataOb    = 0;
  fDataSbNull = true;
  fDataSb     = 0;

  fBinning = 0;
}
//________________________________________________________________________________________
void GenericSingleSampleFitData::Book(void)
{
  LOG("GenericSingleSampleFitData",pDEBUG) << "Booking...";
  assert(fBinning);
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
  LOG("GenericSingleSampleFitData",pDEBUG) << "Cleaning-up...";

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
  LOG("GenericSingleSampleFitData",pDEBUG) << "Zeroing-out (option: " << opt << ") ...";

  string option = (opt) ? opt : "all";

  if (!fDataObNull) { fDataOb->SetAll(0.); }
  if (!fDataSbNull) { fDataSb->SetAll(0.); }
}
//________________________________________________________________________________________


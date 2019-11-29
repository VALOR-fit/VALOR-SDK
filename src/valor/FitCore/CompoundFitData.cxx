//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <iostream>

#include "TClass.h"
#include "TFile.h"
#include "TObjArray.h"
#include "TSystem.h"

#include "valor/Messenger/Messenger.h"
#include "valor/Conventions/Constants.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlXML.h"
#include "valor/Utilities/VThreadPool.h"
#include "valor/VArray/VALORArray.h"
#include "valor/FitCore/CompoundFitData.h"
#include "valor/FitCore/CompoundFitModel.h"

using namespace valor;
using namespace valor::conv;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

//________________________________________________________________________________________
CompoundFitData::CompoundFitData() :
  fDefVALORArrayFormat("data_vf")
{
  fName = "UninitialisedCompoundFitData";
}
//________________________________________________________________________________________
CompoundFitData::~CompoundFitData() 
{
  for (FitDataABC *& d : fSubData) {
    delete d; d = 0;
  }
}
//________________________________________________________________________________________
std::string CompoundFitData::CompoundName() const
{
  std::string name = "--";
  for(std::vector<FitDataABC*>::const_iterator it = fSubData.begin(); it != fSubData.end(); ++it) {
    name += (*it)->Name() + "--";
  }
  return name;
}
//________________________________________________________________________________________
long CompoundFitData::StreamSize(Option_t* opt) const
{
  long size = 0;
  for(std::vector<FitDataABC*>::const_iterator it = fSubData.begin(); it != fSubData.end(); ++it)
  {
    size += (*it)->StreamSize(opt);
  }

  return size;
}
//________________________________________________________________________________________
std::string CompoundFitData::Name() const
{
  return fName;
}
//________________________________________________________________________________________
bool CompoundFitData::Read(string file, Option_t * /*opt*/)
{
  bool status = true;
  
  // Parse the input XML configuration file
  TXMLEngine* xml = new TXMLEngine;   
  XMLDocPointer_t xmldoc = xml->ParseFile(file.c_str());
  if (xmldoc==0) {
    LOG("CompoundFitData",pERROR) << "Can not parse input XML file [" << file << "]";
    delete xml;
    util::Die(1); return false;
  }
  XMLNodePointer_t root_node = xml->DocGetRootElement(xmldoc);
  
  for ( XMLNodePointer_t curr_node = util::FindNode(xml,root_node,"data");
        (!(xml->IsEmptyNode(curr_node)));
        curr_node = xml->GetNext(curr_node) )
  {
    string data_name = util::XML2String(xml,curr_node,"name");
    string class_name = util::XML2String(xml,curr_node,"class");
    TString config_path = util::XML2String(xml,curr_node,"config");
    gSystem->ExpandPathName(config_path);
    
    // Use factory class to get proper data
    TClass * curr_class = TClass::GetClass(class_name.c_str());
    if (!curr_class) {
      LOG("CompoundFitData",pERROR) << "Cannot find class "<<class_name;
      status = false;
      continue;
    }
    void * curr_obj = curr_class->New();
    FitDataABC * curr_data = static_cast<FitDataABC*>(curr_obj);
    
    if (!curr_data) {
      LOG("CompoundFitData",pERROR) << "Failed to properly turn class "<<class_name;
      status = false;
      continue;
    }
    
    status = status && curr_data->Read(config_path.Data());
    
    fSubData.push_back(curr_data);
    
  }
  
  if (not status) {
    LOG("CompoundFitData",pFATAL) << "Failed to properly read all data.";
    util::Die(-1);
  }
  
  return status;
}
//________________________________________________________________________________________
bool CompoundFitData::Read(util::ToyStream* stream)
{

  bool status = true;
  for (std::vector<FitDataABC*>::iterator it = fSubData.begin(); it != fSubData.end(); ++it)
  {
    status = status && (*it)->Read(stream);
  }

  return status;
}
//________________________________________________________________________________________
bool CompoundFitData::Generate (FitModelABC * model, const PhysHypothesis & ph, bool stat_fluct)
{
  CompoundFitModel * cfm = dynamic_cast<CompoundFitModel*>(model);
#ifdef __VALOR_FMWK_PARALLEL__
  std::atomic_bool status(true);
#else
  bool status = true;
#endif
  int ientry = 0;
  for (std::vector<FitDataABC*>::iterator it = fSubData.begin(); it != fSubData.end(); ++it,++ientry) {
    FitDataABC  & d = **it;
    FitModelABC & m = *(cfm ? cfm->GetSubModel(ientry) : model);
#ifdef __VALOR_FMWK_PARALLEL__
    std::function<void(void)> f = [&]{status = status && d.Generate(&m, ph, stat_fluct);};
    util::VThreadPool::Instance()->AddTask(f);
#else
    status = status && d.Generate(&m, ph, stat_fluct);
#endif
  }
#ifdef __VALOR_FMWK_PARALLEL__
  util::VThreadPool::Instance()->Wait();
#endif
  
  return status;
}
//________________________________________________________________________________________
bool CompoundFitData::Generate (FitModelABC * model, const FitParamList & fp, bool stat_fluct)
{
  CompoundFitModel * cfm = dynamic_cast<CompoundFitModel*>(model);
#ifdef __VALOR_FMWK_PARALLEL__
  std::atomic_bool status(true);
#else
  bool status = true;
#endif
  int ientry = 0;
  for (std::vector<FitDataABC*>::iterator it = fSubData.begin(); it != fSubData.end(); ++it,++ientry) {
    FitDataABC  & d = **it;
    FitModelABC & m = *(cfm ? cfm->GetSubModel(ientry) : model);
#ifdef __VALOR_FMWK_PARALLEL__
    std::function<void(void)> f = [&]{status = status && d.Generate(&m, fp, stat_fluct);};
    util::VThreadPool::Instance()->AddTask(f);
#else
    status = status && d.Generate(&m, fp, stat_fluct);
#endif
  }
#ifdef __VALOR_FMWK_PARALLEL__
  util::VThreadPool::Instance()->Wait();
#endif
  
  return status;
}
//________________________________________________________________________________________
bool CompoundFitData::Generate (FitModelABC * model, bool stat_fluct, Option_t* read_opt)
{
  CompoundFitModel * cfm = dynamic_cast<CompoundFitModel*>(model);
#ifdef __VALOR_FMWK_PARALLEL__
  std::atomic_bool status(true);
#else
  bool status = true;
#endif
  int ientry = 0;
  for (std::vector<FitDataABC*>::iterator it = fSubData.begin(); it != fSubData.end(); ++it,++ientry) {
    FitDataABC  & d = **it;
    FitModelABC & m = *(cfm ? cfm->GetSubModel(ientry) : model);
#ifdef __VALOR_FMWK_PARALLEL__
    std::function<void(void)> f = [&]{status = status && d.Generate(&m, stat_fluct);};
    util::VThreadPool::Instance()->AddTask(f);
#else
    status = status && d.Generate(&m, stat_fluct, read_opt);
#endif
  }
#ifdef __VALOR_FMWK_PARALLEL__
  util::VThreadPool::Instance()->Wait();
#endif
  
  return status;
}
//________________________________________________________________________________________
bool CompoundFitData::Write (TObjArray* collection, const char * tag, Option_t * write_opt)
{
  bool status = true;
  for (std::vector<FitDataABC*>::iterator it = fSubData.begin(); it != fSubData.end(); ++it) {
    status = status && (*it)->Write(collection,tag,write_opt);
  }
  return status;
}
//________________________________________________________________________________________
bool CompoundFitData::Write(util::ToyStream* stream)
{

   bool status = true;
   for (std::vector<FitDataABC*>::iterator it = fSubData.begin(); it != fSubData.end(); ++it)
   {
      status = status && (*it)->Write(stream);
   }
   
   return status;
}
//________________________________________________________________________________________
double CompoundFitData::GetFitStatistic (FitModelABC * model, const PhysHypothesis & ph, Option_t * opt, std::map<std::string,double> * likelihood_components)
{
  double statistic = 0;
  CompoundFitModel * cfm = dynamic_cast<CompoundFitModel*>(model);
  if (!cfm) {util::Die(-1);return conv::kBadResult;}
  model->Generate(ph,opt);
  for (unsigned int imodel = 0; imodel < fSubData.size() ; imodel++) {
    statistic += fSubData[imodel]->GetFitStatistic(cfm->GetSubModel(imodel), opt, likelihood_components);
  }
  LOG("CompoundFitData",pINFO) << fName<<" - statistic (ph) = "<<statistic;
  return statistic;
}
//________________________________________________________________________________________
double CompoundFitData::GetFitStatistic (FitModelABC * model, const FitParamList & fp, Option_t * opt, std::map<std::string,double> * likelihood_components )
{
  double statistic = 0;
  CompoundFitModel * cfm = dynamic_cast<CompoundFitModel*>(model);
  if (!cfm) {util::Die(-1);return conv::kBadResult;}
  model->Generate(fp,opt);
  for (unsigned int imodel = 0; imodel < fSubData.size() ; imodel++) {
    statistic += fSubData[imodel]->GetFitStatistic(cfm->GetSubModel(imodel), opt, likelihood_components);
  }
  LOG("CompoundFitData",pINFO) << fName<<" - statistic (fp) = "<<statistic;
  return statistic;
}
//________________________________________________________________________________________
double CompoundFitData::GetFitStatistic (FitModelABC * model, Option_t * opt, std::map<std::string,double> * likelihood_components )
{
  double statistic = 0;
  CompoundFitModel * cfm = dynamic_cast<CompoundFitModel*>(model);
  if (!cfm) {util::Die(-1);return conv::kBadResult;}
  for (unsigned int imodel = 0; imodel < fSubData.size() ; imodel++) {
    statistic += fSubData[imodel]->GetFitStatistic(cfm->GetSubModel(imodel), opt, likelihood_components);
  }
  LOG("CompoundFitData",pINFO) << fName<<" - statistic = "<<statistic;
  return statistic;
}
//________________________________________________________________________________________
int CompoundFitData::NumOfBins(Option_t * /*opt*/) const
{
  int nbins = 0;
  for (std::vector<FitDataABC*>::const_iterator it = fSubData.begin(); it != fSubData.end(); ++it) {
    nbins += (*it)->NumOfBins();
  }
  LOG("CompoundFitData",pDEBUG) << fName<<" - number of bins = "<<nbins;
  return nbins;
}
//________________________________________________________________________________________
bool CompoundFitData::Write (const char * filename, const char * tag, Option_t * write_opt, Option_t * file_opt)
{
  LOG("CompoundFitData",pNOTICE) << "Writing results to file: [" << filename << "]";

  TObjArray * array = new TObjArray();
  bool status = this->Write(array,tag,write_opt);

  Option_t * used_file_opt = (file_opt == 0) ? "recreate" : file_opt;
  TFile f(filename, used_file_opt);
  array->Write();
  f.Close();

  return status;
}
//________________________________________________________________________________________
void CompoundFitData::AddSubData(FitDataABC * data, Option_t * /*opt*/)
{
  fSubData.push_back(data);
  return;
}
//________________________________________________________________________________________
const FitDataABC * CompoundFitData::GetSubData(int data_idx, Option_t * /*opt*/) const
{
  return fSubData.at(data_idx);
}
//________________________________________________________________________________________
FitDataABC * CompoundFitData::GetSubData(int data_idx, Option_t * /*opt*/)
{
  return fSubData.at(data_idx);
}
//________________________________________________________________________________________
unsigned int CompoundFitData::GetNSubData(Option_t * /*opt*/) const
{
  return fSubData.size();
}
//________________________________________________________________________________________
std::string CompoundFitData::GetPDFIntegralAsString(Option_t * opt)
{
  std::string to_print = "";

  for (std::vector<FitDataABC*>::iterator it = fSubData.begin(); it != fSubData.end(); ++it) {
    to_print += (*it)->GetPDFIntegralAsString(opt) + "\n";
  }

  return to_print;
}
//________________________________________________________________________________________
void CompoundFitData::SetName(std::string name)
{
  fName = name;
}
//________________________________________________________________________________________
void CompoundFitData::SetAssociatedModel(FitModelABC *model_abc)
{
  fCFM = dynamic_cast<CompoundFitModel*>(model_abc);
}
//________________________________________________________________________________________
void CompoundFitData::SetFromModel(void)
{
  for (unsigned int isubdata = 0; isubdata < fSubData.size() ; isubdata++) {
    fSubData[isubdata]->SetFromModel();
  }
}
//________________________________________________________________________________________
util::ToyStream* CompoundFitData::InitToyStream(const string file_name, const string read_or_write, string valor_array_format)
{
  valor_array_format = util::RemoveAllWhitespace(valor_array_format);
  if(valor_array_format.empty()) {
    valor_array_format = fDefVALORArrayFormat;
  }

  return new util::ToyStream(file_name, read_or_write, typeid(this), this->CompoundName(),
                             valor_array_format, this->StreamSize());
}
//________________________________________________________________________________________
bool CompoundFitData::DataExists(Option_t * opt) const
{
  bool exists = true;
  for (unsigned int isubdata = 0; isubdata < fSubData.size() ; isubdata++) {
    exists = exists && fSubData[isubdata]->DataExists(opt);
  }
  return exists;
}
//________________________________________________________________________________________

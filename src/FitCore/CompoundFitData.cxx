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

#include "Messenger/Messenger.h"
#include "Conventions/Constants.h"
#include "Utilities/UtlSystem.h"
#include "Utilities/UtlXML.h"
#include "FitCore/CompoundFitData.h"

using namespace valor;
using namespace valor::conv;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

//________________________________________________________________________________________
CompoundFitData::CompoundFitData()
{
  fName = "UninitialisedCompoundFitData";
}
//________________________________________________________________________________________
CompoundFitData::~CompoundFitData() 
{
  
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
    
    status = curr_data->Read(config_path.Data());
    
    fSubData.push_back(curr_data);
    
  }
  
  if (not status) { util::Die(-1); }
  
  return status;
}
//________________________________________________________________________________________
bool CompoundFitData::Generate (FitModelABC * model, const PhysHypothesis & ph, bool stat_fluct)
{
  bool status = true;
  for (std::vector<FitDataABC*>::iterator it = fSubData.begin(); it != fSubData.end(); ++it) {
    status = status & (*it)->Generate(model, ph, stat_fluct);
  }
  return status;
}
//________________________________________________________________________________________
bool CompoundFitData::Generate (FitModelABC * model, const FitParamList & fp, bool stat_fluct)
{
  bool status = true;
  for (std::vector<FitDataABC*>::iterator it = fSubData.begin(); it != fSubData.end(); ++it) {
    status = status && (*it)->Generate(model, fp, stat_fluct);
  }
  return status;
}
//________________________________________________________________________________________
bool CompoundFitData::Generate (FitModelABC * model, bool stat_fluct)
{
  bool status = true;
  for (std::vector<FitDataABC*>::iterator it = fSubData.begin(); it != fSubData.end(); ++it) {
    status = status && (*it)->Generate(model, stat_fluct);
  }
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
double CompoundFitData::Statistic (FitModelABC * model, const PhysHypothesis & ph, Option_t * opt)
{
  double statistic = 0;
  for (std::vector<FitDataABC*>::iterator it = fSubData.begin(); it != fSubData.end(); ++it) {
    statistic += (*it)->Statistic(model, ph, opt);
  }
  LOG("CompoundFitData",pINFO) << fName<<" - statistic = "<<statistic;
  return statistic;
}
//________________________________________________________________________________________
double CompoundFitData::Statistic (FitModelABC * model, const FitParamList & fp, Option_t * opt)
{
  double statistic = 0;
  for (std::vector<FitDataABC*>::iterator it = fSubData.begin(); it != fSubData.end(); ++it) {
    statistic += (*it)->Statistic(model, fp, opt);
  }
  LOG("CompoundFitData",pINFO) << fName<<" - statistic = "<<statistic;
  return statistic;
}
//________________________________________________________________________________________
double CompoundFitData::Statistic (FitModelABC * model, Option_t * opt)
{
  double statistic = 0;
  for (std::vector<FitDataABC*>::iterator it = fSubData.begin(); it != fSubData.end(); ++it) {
    statistic += (*it)->Statistic(model, opt);
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
void CompoundFitData::SetName(std::string name)
{
  fName = name;
}
//________________________________________________________________________________________

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
#include "Utilities/UtlSystem.h"
#include "Utilities/UtlXML.h"
#include "FitCore/CompoundFitModel.h"

using namespace valor;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

//________________________________________________________________________________________
CompoundFitModel::CompoundFitModel() 
{
  fName = "UninitialisedCompoundFitModel";
}
//________________________________________________________________________________________
CompoundFitModel::~CompoundFitModel() 
{

}
//________________________________________________________________________________________
std::string CompoundFitModel::Name() const
{
  return fName;
}
//________________________________________________________________________________________
bool CompoundFitModel::Read(string file, Option_t * /*opt*/)
{
  bool status = true;
  
  // Parse the input XML configuration file
  TXMLEngine* xml = new TXMLEngine;   
  XMLDocPointer_t xmldoc = xml->ParseFile(file.c_str());
  if (xmldoc==0) {
    LOG("NDModel",pERROR) << "Can not parse input XML file [" << file << "]";
    delete xml;
    util::Die(1); return false;
  }
  XMLNodePointer_t root_node = xml->DocGetRootElement(xmldoc);
  
  for ( XMLNodePointer_t curr_node = util::FindNode(xml,root_node,"model");
        (!(xml->IsEmptyNode(curr_node)));
        curr_node = xml->GetNext(curr_node) )
  {
    string model_name = util::XML2String(xml,curr_node,"name");
    string class_name = util::XML2String(xml,curr_node,"class");
    TString config_path = util::XML2String(xml,curr_node,"config");
    gSystem->ExpandPathName(config_path);
    
    // Use factory class to get proper model
    TClass * curr_class = TClass::GetClass(class_name.c_str());
    if (!curr_class) {
      LOG("NDModel",pERROR) << "Cannot find class "<<class_name;
      status = false;
      continue;
    }
    void * curr_obj = curr_class->New();
    FitModelABC * curr_model = static_cast<FitModelABC*>(curr_obj);
    
    if (!curr_model) {
      LOG("NDModel",pERROR) << "Failed to properly turn class "<<class_name;
      status = false;
      continue;
    }
    
    status = curr_model->Read(config_path.Data());
    
    fSubModels.push_back(curr_model);
    
  }
  
  if (not status) { util::Die(-1); }
  
  return status;
}
//________________________________________________________________________________________
bool CompoundFitModel::Generate (const PhysHypothesis & ph, Option_t * opt)
{
  bool status = true;
  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    status = status & (*it)->Generate(ph,opt);
  }
  return status;
}
//________________________________________________________________________________________
bool CompoundFitModel::Generate (const FitParamList & fp, Option_t * opt)
{
  bool status = true;
  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    status = status & (*it)->Generate(fp,opt);
  }
  return status;
}
//________________________________________________________________________________________
void CompoundFitModel::RandomizeSystematics (Option_t * opt)
{
  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    (*it)->RandomizeSystematics(opt);
  }
}
//________________________________________________________________________________________
void CompoundFitModel::TweakSystematic (string syst_name, double twkdial, Option_t * opt)
{
  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    (*it)->TweakSystematic(syst_name, twkdial, opt);
  }
}
//________________________________________________________________________________________
void CompoundFitModel::TweakSystematic (conv::PhysParamId_t syst, double twkdial, Option_t * opt)
{
  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    (*it)->TweakSystematic(syst, twkdial, opt);
  }
}
//________________________________________________________________________________________
void CompoundFitModel::ResetSystematics (Option_t * opt){
  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    (*it)->ResetSystematics(opt);
  }
}
//________________________________________________________________________________________
void CompoundFitModel::PrintSystematics (std::ostream & stream, Option_t * opt) const
{
  for (std::vector<FitModelABC*>::const_iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    (*it)->PrintSystematics(stream,opt);
  }
}
//________________________________________________________________________________________
PhysParamList CompoundFitModel::Tweaks(Option_t * opt) const
{
  PhysParamList ppl;
  for (std::vector<FitModelABC*>::const_iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    PhysParamList ppl2 = (*it)->Tweaks();
    ppl.MergeFrom(ppl2,opt);
  }
  return ppl;
}
//________________________________________________________________________________________
bool CompoundFitModel::Write (TObjArray* collection, const char * tag, Option_t * write_opt)
{
  bool status = true;
  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    status = status && (*it)->Write(collection,tag,write_opt);
  }
  return status;
}
//________________________________________________________________________________________
bool CompoundFitModel::Write (const char * filename, const char * tag, Option_t * write_opt, Option_t * file_opt)
{
  LOG("CompoundFitModel",pNOTICE) << "Writing results to file: [" << filename << "]";

  TObjArray * array = new TObjArray();
  bool status = this->Write(array,tag,write_opt);

  Option_t * used_file_opt = (file_opt == 0) ? "recreate" : file_opt;
  TFile f(filename, used_file_opt);
  array->Write();
  f.Close();

  return status;
}
//________________________________________________________________________________________
double CompoundFitModel::GetFitStatisticPenalty(Option_t * opt) const
{
  double fitstat = 0;
  for (std::vector<FitModelABC*>::const_iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    fitstat+= (*it)->GetFitStatisticPenalty(opt);
  }
  return fitstat;
}
//________________________________________________________________________________________
const FitModelABC * CompoundFitModel::GetSubModel(int model_idx, Option_t * /*opt*/) const
{
  return fSubModels.at(model_idx);
}
//________________________________________________________________________________________
FitModelABC * CompoundFitModel::GetSubModel(int model_idx, Option_t * /*opt*/)
{
  return fSubModels.at(model_idx);
}
//________________________________________________________________________________________

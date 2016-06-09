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

#include "valor/Conventions/Build.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlXML.h"
#include "valor/Utilities/VThreadPool.h"
#include "valor/FitCore/CompoundFitModel.h"

using namespace valor;
using namespace valor::conv;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

//________________________________________________________________________________________
CompoundFitModel::CompoundFitModel() :
 fSystematics(0),
 fOwnerOfSystematics(false)
{
  fName = "UninitialisedCompoundFitModel";
}
//________________________________________________________________________________________
CompoundFitModel::~CompoundFitModel() 
{
  if (fSystematics && fOwnerOfSystematics) { delete fSystematics; }
}
//________________________________________________________________________________________
std::string CompoundFitModel::Name() const
{
  return fName;
}
//________________________________________________________________________________________
bool CompoundFitModel::Read(string file, Option_t * opt)
{
  bool status = true;
  
  // Parse the input XML configuration file
  TXMLEngine* xml = new TXMLEngine;   
  XMLDocPointer_t xmldoc = xml->ParseFile(file.c_str());
  if (xmldoc==0) {
    LOG("CompoundFitModel",pERROR) << "Can not parse input XML file [" << file << "]";
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
      LOG("CompoundFitModel",pERROR) << "Cannot find class "<<class_name;
      status = false;
      continue;
    }
    void * curr_obj = curr_class->New();
    FitModelABC * curr_model = static_cast<FitModelABC*>(curr_obj);
    
    if (!curr_model) {
      LOG("CompoundFitModel",pERROR) << "Failed to properly turn class "<<class_name;
      status = false;
      continue;
    }
    
    status = curr_model->Read(config_path.Data(), opt);
    
    fSubModels.push_back(curr_model);
    
  }
  
  if (not status) { util::Die(-1); }
  
  return status;
}
//________________________________________________________________________________________
bool CompoundFitModel::Generate (const PhysHypothesis & ph, Option_t * opt)
{
  bool status = true;
#ifdef __VALOR_FMWK_PARALLEL__
  std::atomic_bool astat(status);
  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    FitModelABC & m = **it;
    std::function<void(void)> f = [&]{astat = astat && m.Generate(ph,opt);};
    util::VThreadPool::Instance()->AddTask(f);
  }
  util::VThreadPool::Instance()->Wait();
  status = astat;
#else
  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    status = status && (*it)->Generate(ph,opt);
  }
#endif //__VALOR_FMWK_PARALLEL__
  return status;
}
//________________________________________________________________________________________
bool CompoundFitModel::Generate (const FitParamList & fp, Option_t * opt)
{
  bool status = true;
  if (fSystematics) {
    fSystematics->UpdateFrom(fp);
    status = this->Generate(PhysHypothesis(fp),opt);
  }
  else {
#ifdef __VALOR_FMWK_PARALLEL__
    std::atomic_bool astat(status);
    for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
      FitModelABC & m = **it;
      std::function<void(void)> f = [&]{astat = astat && m.Generate(fp,opt);};
      util::VThreadPool::Instance()->AddTask(f);
    }
    util::VThreadPool::Instance()->Wait();
    status = astat;
#else
    for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
      status = status && (*it)->Generate(fp,opt);
    }
#endif //__VALOR_FMWK_PARALLEL__
  }
  return status;
}
//________________________________________________________________________________________
void CompoundFitModel::RandomizeSystematics (Option_t * opt)
{
  if (fSystematics) {  // If we have our own systematic model, tweak that
    fSystematics->Tweak(false, NULL,opt);
  }
  else {// otherwise loop over all submodels.
    for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
      (*it)->RandomizeSystematics(opt);
    }
  }
}
//________________________________________________________________________________________
void CompoundFitModel::TweakSystematic (string syst_name, double twkdial, Option_t * opt)
{
  assert(!"Not implemented");
  (void)syst_name;
  PhysParamId_t syst = conv::kPhypUndefined; // util::PhysParamFromString(syst_name);
  this->TweakSystematic(syst,twkdial,opt);
}
//________________________________________________________________________________________
void CompoundFitModel::TweakSystematic (conv::PhysParamId_t syst, double twkdial, Option_t * opt)
{
  // If we have our own systematic model, tweak that
  if (fSystematics) {
    fSystematics->SetCurrentValue(syst, twkdial);
  }
  else { // otherwise loop over all submodels.
    for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
      (*it)->TweakSystematic(syst, twkdial, opt);
    }
  }
}
//________________________________________________________________________________________
void CompoundFitModel::ResetSystematics (Option_t * opt) {
  if (fSystematics) {
    fSystematics->Tweak(true,NULL);
  }
  else {
    for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
      (*it)->ResetSystematics(opt);
    }
  }
  return;
}
//________________________________________________________________________________________
void CompoundFitModel::PrintSystematics (std::ostream & stream, Option_t * opt) const
{
  if (fSystematics) {
    fSystematics->Print(opt);
  }
  else {
    for (std::vector<FitModelABC*>::const_iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
      (*it)->PrintSystematics(stream,opt);
    }
  }
  return;
}
//________________________________________________________________________________________
PhysParamList CompoundFitModel::Tweaks(Option_t * opt) const
{
  PhysParamList ppl;
  if (fSystematics) {
    fSystematics->CopyTweaksTo(ppl);
  }
  else {
    for (std::vector<FitModelABC*>::const_iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
      PhysParamList ppl2 = (*it)->Tweaks();
      ppl.MergeFrom(ppl2,opt);
    }
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
  if (fSystematics) {
    return fSystematics->FitStatisticPenalty(opt);
  }
  LOG("CompoundFitModel",pNOTICE) << "No global covariance defined. Using submodel covariances for fit statistic penalty - may be wrong.";
  double fitstat = 0;
  for (std::vector<FitModelABC*>::const_iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    fitstat+= (*it)->GetFitStatisticPenalty(opt);
  }
  return fitstat;
}
//________________________________________________________________________________________
void CompoundFitModel::AddSubModel(FitModelABC * model, Option_t * /*opt*/)
{
  fSubModels.push_back(model);
  return;
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
unsigned int CompoundFitModel::GetNSubModels(Option_t * /*opt*/) const
{
  return fSubModels.size();
}
//________________________________________________________________________________________
CorrelatedSystematics * CompoundFitModel::GetSystematics(Option_t * /*opt*/)
{
  return fSystematics;
}
//________________________________________________________________________________________
const CorrelatedSystematics * CompoundFitModel::GetSystematics(Option_t * /*opt*/) const
{
  return fSystematics;
}
//________________________________________________________________________________________
void CompoundFitModel::SetSystConf(CorrelatedSystematics * syst, Option_t * opt)
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
  
  //~ // Set the same matrix for the submodels
  //~ for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    //~ (*it)->SetSystConf(syst,opt);
  //~ }
  
  return;
}
//________________________________________________________________________________________
void CompoundFitModel::SetName(std::string name)
{
  fName = name;
}
//________________________________________________________________________________________

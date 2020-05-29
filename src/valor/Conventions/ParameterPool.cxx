//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//________________________________________________________________________________________

#include <cstring>
#include <utility>
#include <map>

#include "TXMLEngine.h"

#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Conventions/ParameterPool.h"
#include "valor/Conventions/SystType.h"
#include "valor/Conventions/NuOscDeltaCP.h"
#include "valor/Conventions/NuOscDmsqScheme.h"
#include "valor/Conventions/NuOscMixParam.h"
#include "valor/Conventions/Opt.h"

using namespace valor;


valor::conv::PhysParamId_t gFirstParam = conv::kNuis000;

//________________________________________________________________________________________
valor::ParameterPool * valor::ParameterPool::fInstance = 0;
//________________________________________________________________________________________
valor::ParameterPool::ParameterPool()
{
  fInstance = 0;
}
//________________________________________________________________________________________
void ParameterPool::ReadXML(std::string xml_path)
{
  // Analysis configuration
  std::string param_file_full = util::CompleteConfigPath(xml_path);

  // Parse the input XML param file
  TXMLEngine* xmleng = new TXMLEngine;
  XMLDocPointer_t xmldoc = xmleng->ParseFile(param_file_full.c_str());
  if (xmldoc==0) {
    LOG("ParameterPool",pFATAL)<< "Can not parse input XML file [" << param_file_full << "]";
    util::Die(1);
  }
  XMLNodePointer_t root_node = xmleng->DocGetRootElement(xmldoc);
  
  std::string root_node_name = xmleng->GetNodeName(root_node);
  if (strcmp(root_node_name.c_str(),"param_list")!=0) {
    LOG("ParameterPool",pFATAL)<< "root node in XML file "<<param_file_full<<" is "<<root_node_name<<", expected param_list";
    util::Die(-1);
  }

  XMLNodePointer_t curr_node = xmleng->GetChild(root_node);
  while(curr_node) {
    string curr_node_name = xmleng->GetNodeName(curr_node);
    if(curr_node_name == "param") {
      // Peek at the parameter name to see whether we should assign this parameter
      // to a nuisance parameter ID or to a specific oscillation parameter ID
      std::string simple_name = util::ReadSimpleName(xmleng, curr_node);
      conv::PhysParamId_t p = util::PhysParamIdFromRawString(simple_name, true);
      if(p == conv::kPhypUndefined) {
        // The parameter's simple name did not match any predefined parameter,
        // so assign it a generic nuisance parameter ID
        p = (conv::PhysParamId_t)((size_t)gFirstParam + (size_t)(this->CountNonSpecialParams()));
      }
      else {
        // The parameter's simple name matched a predefined parameter,
        // so do nothing here
      }

      // Does the parameter already exist in the pool? If so, update it. If not, add it.
      if(this->IsParamInPool(p)) {
        Parameter update_from(p, xmleng, curr_node);
        this->UpdateParam(this->GetMutableParam(p), update_from);
      }
      else {
        fParams.emplace_back(p, xmleng, curr_node);
        fParamIdx[p] = fParams.size() - 1;
      }
    }
    else if(curr_node_name == "schemes") {
      this->CheckSchemes(xmleng, curr_node);
    }
    else {
      LOG("ParameterPool",pWARN)<< "Found a node, " << curr_node_name << ", which does not correspond to a parameter";
    }

    curr_node = xmleng->GetNext(curr_node);
  }

  xmleng->FreeDoc(xmldoc);
  delete xmleng;

  this->PopulateNameRegistry();
}
//________________________________________________________________________________________
void ParameterPool::CheckSchemes(TXMLEngine* xmleng, XMLNodePointer_t root_node)
{
  bool checked_mixing = false;
  bool checked_Dmsq   = false;
  bool checked_phase  = false;
  XMLNodePointer_t scheme_node = xmleng->GetChild(root_node);
  while(scheme_node) {
    string scheme_node_name = xmleng->GetNodeName(scheme_node);
    if(scheme_node_name == "mixing") {
      std::string scheme_name = util::XML2String(xmleng, root_node, scheme_node_name);
      conv::NuOscMixParam_t scheme = util::MixingTypeFromString(scheme_name);
      if(scheme != Opt::Instance()->gOscMixingParamChoice) {
        LOG("ParameterPool",pFATAL) << "An input parameter list was specified with mixing angle scheme \'"
                                    << util::AsString(scheme)
                                    << "\', but this did not match the scheme set in Opt::Instance \'"
                                    << util::AsString(Opt::Instance()->gOscMixingParamChoice) << "\'";
        exit(-1);
      }
      else {
        checked_mixing = true;
      }
    }
    else if(scheme_node_name == "Dmsq") {
      std::string scheme_name = util::XML2String(xmleng, root_node, scheme_node_name);
      conv::NuOscDmsqScheme_t scheme = util::DmsqSchemeFromString(scheme_name);
      conv::NuOscDmsqScheme_t opt_scheme = Opt::Instance()->gOscDmsqScheme;
      //if(opt_scheme == conv::kNuDmsqAuto) { opt_scheme = util::GetAutoDmsqSchemeFromHierarchy(); }
      if(opt_scheme == conv::kNuDmsqAuto && scheme == conv::kNuDmsq12andAbs23) {
        // The conversion between abs23 and abs31 is done when applying osc prob, so regardless of
        // the mass hierarchy, Dmsq values should be specified with the abs23 scheme
        opt_scheme = conv::kNuDmsq12andAbs23;
      }
      if(scheme != opt_scheme) {
        LOG("ParameterPool",pFATAL) << "An input parameter list was specified with Dmsq scheme \'"
                                    << util::AsString(scheme)
                                    << "\', but this did not match the scheme set in Opt::Instance \'"
                                    << util::AsString(Opt::Instance()->gOscDmsqScheme) << "\'";
        exit(-1);
      }
      else {
        checked_Dmsq = true;
      }
    }
    else if(scheme_node_name == "phase") {
      std::string scheme_name = util::XML2String(xmleng, root_node, scheme_node_name);
      conv::NuOscDeltaCP_t scheme = util::CPVChoiceFromString(scheme_name);
      if(scheme != Opt::Instance()->gOscDeltaCP) {
        LOG("ParameterPool",pFATAL) << "An input parameter list was specified with phase scheme \'"
                                    << util::AsString(scheme)
                                    << "\', but this did not match the scheme set in Opt::Instance \'"
                                    << util::AsString(Opt::Instance()->gOscDeltaCP) << "\'";
        exit(-1);
      }
      else {
        checked_phase = true;
      }
    }
    else {
      LOG("ParameterPool",pFATAL) << "Did not recognise scheme node named \'" << scheme_node_name << "\'";
      exit(-1);
    }
    scheme_node = xmleng->GetNext(scheme_node);
  }

  if(!(checked_mixing && checked_Dmsq && checked_phase)) {
    LOG("ParameterPool",pFATAL)
      << "The mixing angle, Dmsq, and phase schemes must all be specified in the parameter config file";
    exit(-1);
  }
}
//________________________________________________________________________________________
const std::vector<Parameter> & ParameterPool::GetParams() const
{
  return fParams;
}
//________________________________________________________________________________________
const valor::ParameterPool * valor::ParameterPool::Instance()
{
  return MutableInstance();
}
//________________________________________________________________________________________
valor::ParameterPool * valor::ParameterPool::MutableInstance()
{
  if(fInstance == 0) {
    static valor::ParameterPool::Cleaner cleaner;
    cleaner.DummyMethodAndSilentCompiler();
    fInstance = new valor::ParameterPool;
  }
  return fInstance;
}
//________________________________________________________________________________________
void valor::ParameterPool::Print(std::ostream & o) const
{
  for (const auto & p : fParams) {
    p.Print(o);
  }
}
//________________________________________________________________________________________
void valor::ParameterPool::CheckForCollisions() const
{
  std::map<std::string,conv::PhysParamId_t> names;
  for (const Parameter & p : fParams) {
    std::string name = p.GetName();
    if (names.count(name)) {
      LOG("ParameterPool",pFATAL) << "Duplicated parameter name: "<<name;
      exit(-1);
    }
    conv::PhysParamId_t param_id = p.GetID();
    names[name] = param_id;
  }
  return;
}
//________________________________________________________________________________________
const Parameter & ParameterPool::GetParam(conv::PhysParamId_t p) const
{
  unsigned int new_id = fParamIdx.at(p);
  return fParams[new_id];
}
//________________________________________________________________________________________
Parameter & ParameterPool::GetMutableParam (conv::PhysParamId_t p)
{
  unsigned int new_id = fParamIdx.at(p);
  return fParams[new_id];
}
//________________________________________________________________________________________
const Parameter & ParameterPool::ParamFromString(std::string str) const
{
  this->CheckForCollisions();
  std::string str_search = util::ToLowerCase(util::RemoveAllWhitespace(str));
  for (const Parameter & p : fParams) {
    if (p.GetName() == str_search) {
      return p;
    }
  }
  LOG("ParameterPool",pFATAL) << "No parameter named [" << str << "] in the ParameterPool";
  exit(-1);
}
//________________________________________________________________________________________
void ParameterPool::PopulateNameRegistry(void)
{
  util::NameRegistry * nr = util::NameRegistry::MutableInstance();

  for (const Parameter & param : fParams) {
    conv::PhysParamId_t p = param.GetID();
    nr->fParamNames          [p] = param.GetName      (); // TODO: Put in the long name from the t2k_migration branch
    nr->fParamNamesSimple    [p] = param.GetName      (); // If we start using proper names, will change this.
    nr->fParamNamesVerySimple[p] = param.GetName      (); // If we start using proper names, will change this.
    nr->fParamNamesTex       [p] = param.GetTeXName   (); // If we start using proper names, will change this.
    nr->fParamNamesRooTex    [p] = param.GetRooTeXName(); // If we start using proper names, will change this.
  }
  return;
}
//________________________________________________________________________________________
bool ParameterPool::IsParamInPool (conv::PhysParamId_t p_search) const
{
  for (const Parameter & param : fParams) {
    conv::PhysParamId_t p = param.GetID();
    if(p_search == p) {
      return true;
    }
  }
  return false;
}
//________________________________________________________________________________________
unsigned int ParameterPool::CountNonSpecialParams(void) const
{
  unsigned int counter = 0;
  for(const auto param : fParams) {
    if(param.GetType() != conv::kSystSpecial) { counter++; }
  }
  return counter;
}
//________________________________________________________________________________________
void ParameterPool::UpdateParam(Parameter& to_update, const Parameter& update_from)
{
  if(to_update.GetID() != update_from.GetID()) {
    LOG("ParameterPool",pFATAL) << "Tried to update parameter [" << to_update.GetName() << "] from parameter ["
                                << update_from.GetName() << "], but the PhysParamId's did not match";
    exit(-1);
  }
  if(to_update.GetName() != update_from.GetName()) {
    LOG("ParameterPool",pFATAL) << "Tried to update parameter [" << to_update.GetName() << "] from parameter ["
                                << update_from.GetName() << "], but the simple names did not match";
    exit(-1);
  }
  if(to_update.GetType() != update_from.GetType()) {
    LOG("ParameterPool",pFATAL) << "Tried to update parameter [" << to_update.GetName() << "] from parameter ["
                                << update_from.GetName() << "], but the types did not match";
    exit(-1);
  }

  if(update_from.NameLongWasSpecified()      ) { to_update.SetNameLong      (update_from.GetNameLong()      ); }
  if(update_from.RooTeXNameWasSpecified()    ) { to_update.SetRooTeXName    (update_from.GetRooTeXName()    ); }
  if(update_from.TeXNameWasSpecified()       ) { to_update.SetTeXName       (update_from.GetTeXName()       ); }
  if(update_from.CategoriesWasSpecified()    ) { to_update.SetCategories    (update_from.GetCategories()    ); }
  if(update_from.HasLimitsWasSpecified()     ) { to_update.SetHasLimits     (update_from.HasLimits()        ); }
  if(update_from.PhysicalLimitsWasSpecified()) { to_update.SetPhysicalLimits(update_from.GetPhysicalLimits()); }
  if(update_from.InitValueWasSpecified()     ) { to_update.SetInitValue     (update_from.GetInitValue()     ); }
  if(update_from.ErrorWasSpecified()         ) { to_update.SetError         (update_from.GetError()         ); }
  if(update_from.ThrowRangeModeWasSpecified()) { to_update.SetThrowRangeMode(update_from.GetThrowRangeMode()); }
  if(update_from.ThrowRangeWasSpecified()    ) { to_update.SetThrowRange    (update_from.GetThrowRange()    ); }
  if(update_from.StepSizeModeWasSpecified()  ) { to_update.SetStepSizeMode  (update_from.GetStepSizeMode()  ); }
  if(update_from.StepSizeWasSpecified()      ) { to_update.SetStepSize      (update_from.GetStepSize()      ); }
  if(update_from.IsFixedWasSpecified()       ) { to_update.SetIsFixed       (update_from.IsFixed()          ); }
  if(update_from.IsNuisanceWasSpecified()    ) { to_update.SetIsNuisance    (update_from.IsNuisance()       ); }
  if(update_from.DoMarginaliseWasSpecified() ) { to_update.SetDoMarginalise (update_from.DoMarginalise()    ); }
}
//________________________________________________________________________________________
size_t ParameterPool::GetNumOscParams(void) const
{
  size_t counter = 0;
  for(const auto param : fParams) {
    if(!util::IsSystematic(param.GetID())) { counter++; }
  }
  return counter;
}
//________________________________________________________________________________________
size_t ParameterPool::GetNumSystParams(void) const
{
  return fParams.size() - this->GetNumOscParams();
}
//________________________________________________________________________________________

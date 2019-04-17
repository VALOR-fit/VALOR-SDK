//________________________________________________________________________________________
/*

Copyright (c) 2010-2019, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#include <cstring>
#include <utility>
#include <map>

#include "TXMLEngine.h"

#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Conventions/ParameterPool.h"

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
      conv::PhysParamId_t p = (conv::PhysParamId_t)((size_t)gFirstParam+fParams.size());
      fParams.emplace_back(p, xmleng, curr_node);
    }
    else {
      LOG("ParameterPool",pWARN)<< "Found a node, " << curr_node_name << ", which does not correspond to a parameter";
    }

    curr_node = xmleng->GetNext(curr_node);
  }

  xmleng->FreeDoc(xmldoc);
  delete xmleng;
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
  int new_id = p - gFirstParam;
  // Should definitely do a bounds check here if it doesn't slow us down too much
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
  NameRegistry * nr = util::NameRegistry::MutableInstance();

  for (const Parameter & param : fParams) {
    conv::PhysParamId_t p = param.GetID();
    nr->fParamNames      [p] = param.GetName(); // TODO: Put in the long name from the t2k_migration branch
    nr->fParamNamesSimple[p] = param.GetName(); // If we start using proper names, will change this.
  }
  return;
}
//________________________________________________________________________________________

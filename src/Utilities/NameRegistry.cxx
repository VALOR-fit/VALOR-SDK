//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <cassert>
#include "Utilities/NameRegistry.h"

using namespace valor;
using namespace valor::conv;

using std::map;
using std::string;

//___________________________________________________________________________
valor::util::NameRegistry * valor::util::NameRegistry::fInstance = 0;
//____________________________________________________________________________
valor::util::NameRegistry::NameRegistry()
{
  this->Init();
  fInstance = 0;
}
//____________________________________________________________________________
const valor::util::NameRegistry * valor::util::NameRegistry::Instance() {return MutableInstance();}
//____________________________________________________________________________
valor::util::NameRegistry * valor::util::NameRegistry::MutableInstance() 
{
  if(fInstance == 0) {
    static valor::util::NameRegistry::Cleaner cleaner;
    cleaner.DummyMethodAndSilentCompiler();
    fInstance = new valor::util::NameRegistry;
  }
  return fInstance;
}
//____________________________________________________________________________
void valor::util::NameRegistry::Init(void)
{
  for (int ip = -1 ; ip < conv::kLastNuisParam; ip++) {
    conv::PhysParamId_t p = (conv::PhysParamId_t)ip;
    fParamNames      .insert(map<conv::PhysParamId_t, std::string>::value_type(p,util::RawParamAsString   (p)));
    fParamNamesSimple.insert(map<conv::PhysParamId_t, std::string>::value_type(p,util::RawParamAsStringRaw(p)));
  }
  return;
}
//________________________________________________________________________________________
std::string valor::util::NameRegistry::ParamAsString(PhysParamId_t p ) const
{
  map<PhysParamId_t, string>::const_iterator iter = fParamNames.find(p);
  assert(iter != fParamNames.end());
  return iter->second;
}
//________________________________________________________________________________________
std::string valor::util::NameRegistry::ParamAsStringSimple(PhysParamId_t p ) const
{
  map<PhysParamId_t, string>::const_iterator iter = fParamNamesSimple.find(p);
  assert(iter != fParamNamesSimple.end());
  return iter->second;
}
//________________________________________________________________________________________
std::string valor::util::NameRegistry::AsString      (PhysParamId_t p) const { return this->ParamAsString      (p); }
std::string valor::util::NameRegistry::AsStringSimple(PhysParamId_t p) const { return this->ParamAsStringSimple(p); }
//________________________________________________________________________________________

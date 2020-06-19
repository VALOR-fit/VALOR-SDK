//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

ChangeLog:

*/
//________________________________________________________________________________________

#include <cassert>
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlPrint.h"

using namespace valor;
using namespace valor::conv;

using std::map;
using std::string;

//___________________________________________________________________________
util::NameRegistry * util::NameRegistry::fInstance = 0;
//____________________________________________________________________________
util::NameRegistry::NameRegistry()
{
  this->Init();
}
//____________________________________________________________________________
const util::NameRegistry * util::NameRegistry::Instance() {return MutableInstance();}
//____________________________________________________________________________
util::NameRegistry * util::NameRegistry::MutableInstance() 
{
  if(fInstance == 0) {
    static util::NameRegistry::Cleaner cleaner;
    cleaner.DummyMethodAndSilentCompiler();
    fInstance = new util::NameRegistry;
  }
  return fInstance;
}
//____________________________________________________________________________
void util::NameRegistry::Init(void)
{
  for (int ip = -1 ; ip < conv::kLastNuisParam; ip++) {
    conv::PhysParamId_t p = (conv::PhysParamId_t)ip;
    fParamNames          .insert(map<conv::PhysParamId_t, std::string>::value_type(p,util::RawParamAsString      (p)));
    fParamNamesSimple    .insert(map<conv::PhysParamId_t, std::string>::value_type(p,util::RawParamAsStringRaw   (p)));
    fParamNamesVerySimple.insert(map<conv::PhysParamId_t, std::string>::value_type(p,util::RawParamAsStringSimple(p)));
    fParamNamesTex       .insert(map<conv::PhysParamId_t, std::string>::value_type(p,util::RawParamAsStringTex   (p)));
    fParamNamesRooTex    .insert(map<conv::PhysParamId_t, std::string>::value_type(p,util::RawParamAsStringRooTex(p)));
  }
  for (int im = 0 ; im < kNReacModes       ; im++) {fModeNames    .push_back(util::ModeAsString    ((ReacMode_t)im));}
  for (int is = 0 ; is < kMaxNSamples      ; is++) {fSampleNames  .push_back(util::SampleAsString  ((Sample_t)  is));}
  for (int id = 0 ; id < kNDetectorsDefined; id++) {fDetectorNames.push_back(util::DetectorAsString((Detector_t)id));}
  for (int ib = 0 ; ib < kNBeamsDefined    ; ib++) {fBeamConfNames.push_back(util::BeamConfAsString((BeamConf_t)ib));}
  for (int it = 0 ; it < kNNuclearTarget   ; it++) {fNuclearTargetNames.push_back(util::NuclearTargetAsString((NuclearTarget_t)it));}
  return;
}
//________________________________________________________________________________________
std::string util::NameRegistry::ParamAsString(PhysParamId_t p ) const
{
  map<PhysParamId_t, string>::const_iterator iter = fParamNames.find(p);
  assert(iter != fParamNames.end());
  return iter->second;
}
//________________________________________________________________________________________
std::string util::NameRegistry::ParamAsStringSimple(PhysParamId_t p ) const
{
  map<PhysParamId_t, string>::const_iterator iter = fParamNamesSimple.find(p);
  assert(iter != fParamNamesSimple.end());
  return iter->second;
}
//________________________________________________________________________________________
std::string util::NameRegistry::ParamAsStringVerySimple(PhysParamId_t p ) const
{
  map<PhysParamId_t, string>::const_iterator iter = fParamNamesVerySimple.find(p);
  assert(iter != fParamNamesVerySimple.end());
  return iter->second;
}
//________________________________________________________________________________________
std::string util::NameRegistry::ParamAsStringTex(PhysParamId_t p ) const
{
  map<PhysParamId_t, string>::const_iterator iter = fParamNamesTex.find(p);
  assert(iter != fParamNamesTex.end());
  return iter->second;
}
//________________________________________________________________________________________
std::string util::NameRegistry::ParamAsStringRooTex(PhysParamId_t p ) const
{
  map<PhysParamId_t, string>::const_iterator iter = fParamNamesRooTex.find(p);
  assert(iter != fParamNamesRooTex.end());
  return iter->second;
}
//________________________________________________________________________________________
std::string util::NameRegistry::ModeAsString(ReacMode_t m) const
{
  if(m>=0 && m < (ReacMode_t) fModeNames.size()) {
    return fModeNames.at((size_t)m);
  }
  else {
    LOG("NameRegistry",pWARN)
      << "Unable to find name in current object for mode "
      <<m<<", compiled in as "<<util::AsString(m);
  }
  return "Mode not found";
}
//________________________________________________________________________________________
std::string util::NameRegistry::SampleAsString(Sample_t s) const
{
  if(s>=0 && s < (Sample_t) fSampleNames.size()) {
    return fSampleNames.at((size_t)s);
  }
  else {
    LOG("NameRegistry",pWARN)
      << "Unable to find name in current object for sample "
      <<s<<", compiled in as "<<util::AsString(s);
  }
  return "Sample not found";
}
//________________________________________________________________________________________
std::string util::NameRegistry::DetectorAsString(Detector_t d) const
{
  if(d>=0 && d < (Detector_t) fDetectorNames.size()) {
    return fDetectorNames.at((size_t)d);
  }
  else {
    LOG("NameRegistry",pWARN)
      << "Unable to find name in current object for detector "
      <<d<<", compiled in as "<<util::AsString(d);
  }
  return "Detector not found";
}
//________________________________________________________________________________________
std::string util::NameRegistry::BeamConfAsString(BeamConf_t b) const
{
  if(b>=0 && b < (BeamConf_t) fBeamConfNames.size()) {
    return fBeamConfNames.at((size_t)b);
  }
  else {
    LOG("NameRegistry",pWARN)
      << "Unable to find name in current object for beam "
      <<b<<", compiled in as "<<util::AsString(b);
  }
  return "Beam not found";
}
//________________________________________________________________________________________
std::string util::NameRegistry::NuclearTargetAsString(NuclearTarget_t nt) const
{
  if(nt>=0 && nt < (NuclearTarget_t) fNuclearTargetNames.size()) {
    return fNuclearTargetNames.at((size_t)nt);
  }
  else {
    LOG("NameRegistry",pWARN)
      << "Unable to find name in current object for nuclear target "
      <<nt<<", compiled in as "<<util::AsString(nt);
  }
  return "Nuclear target not found";
}
//________________________________________________________________________________________
std::string util::NameRegistry::AsString (ReacMode_t m      ) const { return this->ModeAsString          (m );}
std::string util::NameRegistry::AsString (Sample_t   s      ) const { return this->SampleAsString        (s );}
std::string util::NameRegistry::AsString (Detector_t d      ) const { return this->DetectorAsString      (d );}
std::string util::NameRegistry::AsString (BeamConf_t b      ) const { return this->BeamConfAsString      (b );}
std::string util::NameRegistry::AsString (NuclearTarget_t nt) const { return this->NuclearTargetAsString (nt);}
std::string util::NameRegistry::AsString          (PhysParamId_t p) const { return this->ParamAsString          (p );}
std::string util::NameRegistry::AsStringSimple    (PhysParamId_t p) const { return this->ParamAsStringSimple    (p );}
std::string util::NameRegistry::AsStringVerySimple(PhysParamId_t p) const { return this->ParamAsStringVerySimple(p );}
std::string util::NameRegistry::AsStringTex       (PhysParamId_t p) const { return this->ParamAsStringTex       (p );}
std::string util::NameRegistry::AsStringRooTex    (PhysParamId_t p) const { return this->ParamAsStringRooTex    (p );}
//________________________________________________________________________________________
PhysParamId_t util::NameRegistry::PhysParamFromString(const std::string & inp_name) const
{
  std::string inp_name_lc = util::ToLowerCase(inp_name);
  for(int ip = 0; ip < kLastNuisParam; ip++) {
    PhysParamId_t p = conv::PhysParamId_t(ip);
    string name             = util::ToLowerCase(this->AsString(p));
    string name_simple      = util::ToLowerCase(this->AsStringSimple(p));
    string name_very_simple = util::ToLowerCase(this->AsStringVerySimple(p));
    if((name == inp_name_lc) || (name_simple == inp_name_lc) || (name_very_simple == inp_name_lc)) {
      return p;
    }
  }
  return kPhypUndefined;
}
//________________________________________________________________________________________
ReacMode_t util::NameRegistry::ModeFromString(const std::string & name)
{
  for (size_t i = 0 ; i < fModeNames.size() ; i++) {
    if (name == fModeNames.at(i)) {
      return (ReacMode_t) i;
    }
  }
  LOG("NameRegistry",pERROR)<<"Unable to match mode name: "<<name;
  return kmUndefined;
}
//________________________________________________________________________________________
Sample_t util::NameRegistry::SampleFromString(const std::string & name)
{
  for (size_t i = 0 ; i < fSampleNames.size() ; i++) {
    if (name == fSampleNames.at(i)) {
      return (Sample_t) i;
    }
  }
  LOG("NameRegistry",pERROR)<<"Unable to match sample name: "<<name;
  return ksUndefined;
}
//________________________________________________________________________________________
Detector_t util::NameRegistry::DetectorFromString(const std::string & name)
{
  for (size_t i = 0 ; i < fDetectorNames.size() ; i++) {
    if (name == fDetectorNames.at(i)) {
      return (Detector_t) i;
    }
  }
  LOG("NameRegistry",pERROR)<<"Unable to match detector name: "<<name;
  return kdUndefined;
}
//________________________________________________________________________________________
BeamConf_t util::NameRegistry::BeamConfFromString(const std::string & name)
{
  for (size_t i = 0 ; i < fBeamConfNames.size() ; i++) {
    if (name == fBeamConfNames.at(i)) {
      return (BeamConf_t) i;
    }
  }
  LOG("NameRegistry",pERROR)<<"Unable to match beam name: "<<name;
  return kbUndefined;
}
//________________________________________________________________________________________
NuclearTarget_t util::NameRegistry::NuclearTargetFromString(const std::string & name)
{
  for (size_t i = 0 ; i < fNuclearTargetNames.size() ; i++) {
    if (name == fNuclearTargetNames.at(i)) {
      return (NuclearTarget_t) i;
    }
  }
  LOG("NameRegistry",pERROR)<<"Unable to match nuclear target name: "<<name;
  return kntUndefined;
}
//________________________________________________________________________________________
bool util::NameRegistry::HardcodedEnumsMatch(const NameRegistry & other) const
{
  bool success = true;
  if(this->fModeNames != other.fModeNames) {
    success = false;
    LOG("NameRegistry",pERROR) << "Modes do not match.";
    util::PrintVectorComparison(this->fModeNames,other.fModeNames,std::cerr);
  }
  if(this->fSampleNames != other.fSampleNames) {
    success = false;
    LOG("NameRegistry",pERROR) << "Samples do not match.";
    util::PrintVectorComparison(this->fSampleNames,other.fSampleNames,std::cerr);
  }
  if(this->fDetectorNames != other.fDetectorNames) {
    success = false;
    LOG("NameRegistry",pERROR) << "Detectors do not match.";
    util::PrintVectorComparison(this->fDetectorNames,other.fDetectorNames,std::cerr);
  }
  if(this->fBeamConfNames != other.fBeamConfNames) {
    success = false;
    LOG("NameRegistry",pERROR) << "Beam configurations do not match.";
    util::PrintVectorComparison(this->fBeamConfNames,other.fBeamConfNames,std::cerr);
  }
  if(this->fNuclearTargetNames != other.fNuclearTargetNames) {
    success = false;
    LOG("NameRegistry",pERROR) << "Nuclear targets do not match.";
    util::PrintVectorComparison(this->fNuclearTargetNames,other.fNuclearTargetNames,std::cerr);
  }
  
  return success;
}
//________________________________________________________________________________________
bool util::NameRegistry::ParametersMatch(const NameRegistry & /*other*/) const
{
  assert(!"To be implemented when I've sorted out the parameter container format - SD"); return false;
}
//________________________________________________________________________________________

//________________________________________________________________________________________
/*

Copyright (c) 2010-2018, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#include <string>
#include <algorithm>
#include <cctype>

#include <TXMLEngine.h>

#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlXML.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Conventions/Constants.h"
#include "valor/Conventions/Opt.h"
#include "valor/Conventions/Parameter.h"

using std::string;
using std::vector;
using std::map;
using std::pair;

using namespace valor;
using namespace valor::conv;

// Just so we don't have to keep making it later
std::pair<double,double> max_limits = std::make_pair<double,double>(
  -std::numeric_limits<double>::max(), std::numeric_limits<double>::max());

//________________________________________________________________________________________
Parameter::Parameter(conv::PhysParamId_t param_id, TXMLEngine *xml, XMLNodePointer_t param_node)
{
  Init(param_id,xml, param_node);
  ReadNames();
  ReadRanges();
  ReadType();
  ReadCategories();
  ReadApplyMaskAll();
  ReadLimits();
  LOG("FitInputs", pINFO) << "Successfully read parameter \'" << GetName() << "\'";
}
//________________________________________________________________________________________
Parameter::Parameter()
{
  this->Init(conv::kPhypUndefined,nullptr,nullptr);
}
//________________________________________________________________________________________
Parameter::~Parameter()
{
}
//________________________________________________________________________________________
void Parameter::Init(conv::PhysParamId_t param_id, TXMLEngine *xml, XMLNodePointer_t param_node)
{
  fParam      = param_id;
  fName       = "undefined";
  fRooTeXName = "undefined";
  fTeXName    = "undefined";
  fType       = kSystUndefined;
  fHasLimits  = false;

  fxml = xml;
  fParamNode = param_node;
}
//________________________________________________________________________________________
void Parameter::ReadNames()
{
  if(IsNodeSpecified("name/simple")) {
    fName = util::XML2String(fxml, fParamNode, "name/simple");
  }
  else {
    LOG("FitInputs", pFATAL) << "No simple name specified for a parameter";
    util::Die(1);
  }

  if(IsNodeSpecified("name/rootex")) {
    fRooTeXName = util::XML2String(fxml, fParamNode, "name/rootex");
  }
  else {
    LOG("FitInputs", pNOTICE) << "No rootex name specified for parameter \'"
                              << fName << "\', instead using the simple name";
    fRooTeXName = fName;
  }
  
  if (!IsNodeSpecified("name/tex")) {
    fTeXName = fRooTeXName;
    replace( fTeXName.begin(), fTeXName.end(), '\\', '#' ); // Make slashes hashes for RooTeX
    // Get rid of hashes preceding sin
    size_t ind = fTeXName.find("#sin",0);
    if (ind !=  string::npos) (fTeXName.replace(ind,4," sin"));
    fTeXName = util::TrimSpaces(fTeXName);
  }
}
//________________________________________________________________________________________
void Parameter::ReadRanges()
{
  if(util::NodeExists(fxml, fParamNode, "apply_to_range")) {
    vector <XMLNodePointer_t> range_nodes = util::FindAllNodes(fxml,
                                                               util::FindNode(fxml, fParamNode, "apply_to_range"));
    if(range_nodes.size() == 0) {
      LOG("FitInputs", pFATAL) << "No parameter apply ranges specified for parameter \'" << fName << "\'";
      util::Die(1);
    }
    for(XMLNodePointer_t range_node : range_nodes) {
      KineVar_t kv = kvUndefined;
      // Get the kinematic variable for this range
      string node_name = fxml->GetNodeName(range_node);
      for(int kvi = kvUndefined + 1; kvi != kNKineVarMax; kvi++) {
        KineVar_t kv_try = static_cast<KineVar_t>(kvi);
        if(util::ToLowerCase(node_name) == util::ToLowerCase(util::AsString(kv_try))) {
          kv = kv_try;
          break;
        }
        if(kv_try == kNKineVarMax) {
          LOG("FitInputs", pFATAL) << "Unrecognised kinematic variable \'" << node_name << "\'";
          util::Die(1);
        }
      }
      if(std::find(fKinematicVariables.begin(),fKinematicVariables.end(),kv)!=fKinematicVariables.end()) {
        LOG("FitInputs", pFATAL) << "For parameter " << fName << " and kinematic variable " << node_name
                                 << ", there is already a range defined for this kinematic variable, cannot add another";
        util::Die(1);
      }
      if(kv == kvUndefined) {
        // This should not happen
        LOG("FitInputs", pFATAL) << "For parameter " << fName << " and kinematic variable " << node_name
                                 << ", the kinematic variable could not be set";
        util::Die(1);
      }
      // Get the range
      string path = "apply_to_range/" + node_name;
      fKinematicVariables.push_back(kv);
      fKinematicRange.push_back(util::XML2DblDblPair(fxml, fParamNode, path));
      if(fKinematicRange.back().second < fKinematicRange.back().first) {
        LOG("FitInputs", pFATAL) << "For parameter " << fName << " and kinematic variable " << node_name
                                 << ", the value specified for the upper limit of the range is smaller than the value specified for the lower limit";
        util::Die(1);
      }
    }
  }
}
//________________________________________________________________________________________
void Parameter::ReadType()
{
  if(IsNodeSpecified("type")) {
    std::string type_str = util::XML2String(fxml, fParamNode, "type");
    SystType_t type = util::SystTypeFromString(type_str);
    if(type == kSystUndefined) {
      // The user has tried to set a type for this param but it was not recognised, so die
      LOG("FitInputs", pFATAL) << "For parameter " << fName << ", systematic type \'" << type_str << "\' not recognised";
      util::Die(1);
    }
    fType = type;
  }
  else {
    // The user hasn't tried to set any type for this param, so give up
    LOG("FitInputs", pNOTICE) << "For parameter " << fName << ", systematic type not set.";
    util::Die(-1);
  }
}
//________________________________________________________________________________________
void Parameter::ReadCategories()
{
  XMLNodePointer_t cats_node = util::FindNode(fxml, fParamNode, "categories");
  vector<XMLNodePointer_t> cat_nodes = util::FindNodes(fxml, cats_node, "category");
  if(cat_nodes.size() == 0) {
    LOG("FitInputs", pFATAL) << "No category nodes specified for parameter \'" << fName <<"\'";
    util::Die(1);
  }
  for(XMLNodePointer_t cat_node : cat_nodes) {
    string cat = util::TrimSpaces(fxml->GetNodeContent(cat_node));
    if(cat == "") {
      LOG("FitInputs", pFATAL) << "Empty category node for parameter \'" << fName <<"\'";
      util::Die(1);
    }
    fCategories.push_back(cat);
  }
}
//________________________________________________________________________________________
void Parameter::ReadApplyMaskAll()
{
  DetectorApplyMask_t      detector = fDefApplyDetector;
  BeamApplyMask_t          beam     = fDefApplyBeam;
  FlavApplyMask_t          flav     = fDefApplyFlav;
  SelecApplyMask_t         selec    = fDefApplySelec;
  ReacTypeApplyMask_t      reactype = fDefApplyReacType;
  NuclearTargetApplyMask_t target   = fDefApplyNuclearTarget;
  if(util::NodeExists(fxml, fParamNode, "apply_to")) {
    detector = ReadDetectorApplyMask     ("detector");
    beam     = ReadBeamApplyMask         ("beam");
    flav     = ReadFlavApplyMask         ("flav");
    selec    = ReadSelecApplyMask        ("selection");
    reactype = ReadReacTypeApplyMask     ("reac_type");
    target   = ReadNuclearTargetApplyMask("target");
  }
  else {
    LOG("FitInputs", pNOTICE) << "For parameter " << fName << ", no apply masks have been set. Applying to all.";
    // No need to do anything here, as the the variables were initialised with the default values and have not been changed
  }
  fApply = {detector, beam, flav, selec, reactype, target};
}
//________________________________________________________________________________________
DetectorApplyMask_t Parameter::ReadDetectorApplyMask(string node_name) const
{
  DetectorApplyMask_t mask = fDefApplyDetector;
  string node_path = "apply_to/" + node_name;
  if(IsNodeSpecified(node_path)) {
    string content = util::XML2String(fxml, fParamNode, node_path);
    std::string trimmed_content = util::ToLowerCase(util::RemoveAllWhitespace(content));
    try {
      mask = util::DetectorMaskFromString(util::ToLowerCase(trimmed_content));
    }
    catch (const std::out_of_range& oor) {
      LOG("FitInputs", pFATAL) << "For parameter " << fName << ", " << node_name << " apply mask name \'" << trimmed_content
                               << "\' (from user input \'" << content << "\') was not recognised - threw an out of range error: " << oor.what();
      util::Die(1);
    }
  }
  else {
    // The user hasn't tried to set any mask of this type for this param, so keep at the default mask
    // No need to do anything here, as the the variables were initialised with the default values and have not been changed
    LOG("FitInputs", pNOTICE) << "For parameter " << fName << ", " << node_name << " apply mask not set. Setting to default (" << util::DetectorMaskAsString(mask) << ")";
  }
  return mask;
}
//________________________________________________________________________________________
BeamApplyMask_t Parameter::ReadBeamApplyMask(string node_name) const
{
  BeamApplyMask_t mask = fDefApplyBeam;
  string node_path = "apply_to/" + node_name;
  if(IsNodeSpecified(node_path)) {
    string content = util::XML2String(fxml, fParamNode, node_path);
    std::string trimmed_content = util::ToLowerCase(util::RemoveAllWhitespace(content));
    try {
      mask = util::BeamMaskFromString(util::ToLowerCase(trimmed_content));
    }
    catch (const std::out_of_range& oor) {
      LOG("FitInputs", pFATAL) << "For parameter " << fName << ", " << node_name << " apply mask name \'" << trimmed_content
                               << "\' (from user input \'" << content << "\') was not recognised - threw an out of range error: " << oor.what();
      util::Die(1);
    }
  }
  else {
    // The user hasn't tried to set any mask of this type for this param, so keep at the default mask
    // No need to do anything here, as the the variables were initialised with the default values and have not been changed
    LOG("FitInputs", pNOTICE) << "For parameter " << fName << ", " << node_name << " apply mask not set. Setting to default (" << util::BeamMaskAsString(mask) << ")";
  }
  return mask;
}
//________________________________________________________________________________________
FlavApplyMask_t Parameter::ReadFlavApplyMask(string node_name) const
{
  FlavApplyMask_t mask = fDefApplyFlav;
  string node_path = "apply_to/" + node_name;
  if(IsNodeSpecified(node_path)) {
    string content = util::XML2String(fxml, fParamNode, node_path);
    std::string trimmed_content = util::ToLowerCase(util::RemoveAllWhitespace(content));
    try {
      mask = util::FlavMaskFromString(trimmed_content);
    }
    catch (const std::out_of_range& oor) {
      LOG("FitInputs", pFATAL) << "For parameter " << fName << ", " << node_name << " apply mask name \'" << trimmed_content
                               << "\' (from user input \'" << content << "\') was not recognised - threw an out of Range error: " << oor.what();
      util::Die(1);
    }
  }
  else {
    // The user hasn't tried to set any mask of this type for this param, so keep at the default mask
    // No need to do anything here, as the the variables were initialised with the default values and have not been changed
    LOG("FitInputs", pNOTICE) << "For parameter " << fName << ", " << node_name << " apply mask not set. Setting to default (" << util::FlavMaskAsString(mask) << ")";
  }
  return mask;
}
//________________________________________________________________________________________
SelecApplyMask_t Parameter::ReadSelecApplyMask(string node_name) const
{
  SelecApplyMask_t mask = fDefApplySelec;
  string node_path = "apply_to/" + node_name;
  if(IsNodeSpecified(node_path)) {
    string content = util::XML2String(fxml, fParamNode, node_path);
    std::string trimmed_content = util::ToLowerCase(util::RemoveAllWhitespace(content));
    try {
      mask = util::SelecMaskFromString(util::ToLowerCase(trimmed_content));
    }
    catch (const std::out_of_range& oor) {
      LOG("FitInputs", pFATAL) << "For parameter " << fName << ", " << node_name << " apply mask name \'" << trimmed_content
                               << "\' (from user input \'" << content << "\') was not recognised - threw an out of range error: " << oor.what();
      util::Die(1);
    }
  }
  else {
    // The user hasn't tried to set any mask of this type for this param, so keep at the default mask
    // No need to do anything here, as the the variables were initialised with the default values and have not been changed
    LOG("FitInputs", pNOTICE) << "For parameter " << fName << ", " << node_name << " apply mask not set. Setting to default (" << util::SelecMaskAsString(mask) << ")";
  }
  return mask;
}
//________________________________________________________________________________________
ReacTypeApplyMask_t Parameter::ReadReacTypeApplyMask(string node_name) const
{
  ReacTypeApplyMask_t mask = fDefApplyReacType;
  string node_path = "apply_to/" + node_name;
  if(IsNodeSpecified(node_path)) {
    string content = util::XML2String(fxml, fParamNode, node_path);
    std::string trimmed_content = util::ToLowerCase(util::RemoveAllWhitespace(content));
    try {
      mask = util::ReacTypeMaskFromString(util::ToLowerCase(trimmed_content));
    }
    catch (const std::out_of_range& oor) {
      LOG("FitInputs", pFATAL) << "For parameter " << fName << ", " << node_name << " apply mask name \'" << trimmed_content
                               << "\' (from user input \'" << content << "\') was not recognised - threw an out of range error: " << oor.what();
      util::Die(1);
    }
  }
  else {
    // The user hasn't tried to set any mask of this type for this param, so keep at the default mask
    // No need to do anything here, as the the variables were initialised with the default values and have not been changed
    LOG("FitInputs", pNOTICE) << "For parameter " << fName << ", " << node_name << " apply mask not set. Setting to default (" << util::ReacTypeMaskAsString(mask) << ")";
  }
  return mask;
}
//________________________________________________________________________________________
NuclearTargetApplyMask_t Parameter::ReadNuclearTargetApplyMask(string node_name) const
{
  NuclearTargetApplyMask_t mask = fDefApplyNuclearTarget;
  string node_path = "apply_to/" + node_name;
  if(IsNodeSpecified(node_path)) {
    string content = util::XML2String(fxml, fParamNode, node_path);
    std::string trimmed_content = util::ToLowerCase(util::RemoveAllWhitespace(content));
    try {
      mask = util::NuclearTargetMaskFromString(util::ToLowerCase(trimmed_content));
    }
    catch (const std::out_of_range& oor) {
      LOG("FitInputs", pFATAL) << "For parameter " << fName << ", " << node_name << " apply mask name \'" << trimmed_content
                               << "\' (from user input \'" << content << "\') was not recognised - threw an out of range error: " << oor.what();
      util::Die(1);
    }
  }
  else {
    // The user hasn't tried to set any mask of this type for this param, so keep at the default mask
    // No need to do anything here, as the the variables were initialised with the default values and have not been changed
    LOG("FitInputs", pNOTICE) << "For parameter " << fName << ", " << node_name << " apply mask not set. Setting to default (" << util::NuclearTargetMaskAsString(mask) << ")";
  }
  return mask;
}
//________________________________________________________________________________________
void Parameter::ReadLimits()
{
  if(IsNodeSpecified("limits")) {
    fLimits = util::XML2DblDblPair(fxml, fParamNode, "limits");
  }
  else {
    fHasLimits = false;
    fLimits = max_limits;
  }
}
//________________________________________________________________________________________
bool Parameter::IsNodeSpecified(const string node_path) const
{
  bool specified = true;
  if(util::NodeExists(fxml, fParamNode, node_path)) {
    std::string content = util::XML2String(fxml, fParamNode, node_path);
    if(content == "") {
      specified = false;
    }
  }
  else {
    specified = false;
  }
  return specified;
}
//________________________________________________________________________________________
void Parameter::Print(std::ostream & stream) const
{
  vector<string> cats = GetCategories();
  vector<KineVar_t> kinevars = GetKineVars();
  std::pair<double,double> limits = GetLimits();
  stream << "Parameter info:\n"
         << "    Current numerical ID: "<<fParam<<"\n"
         << "    Name (simple) = " << GetName()       << "\n"
         << "    Name (RooTeX) = " << GetRooTeXName() << "\n"
         << "    Type = " << util::AsString(GetType()) << "\n"
         << "    Categories:\n";
  for(string cat : cats) {
    stream << "        " << cat << "\n";
  }
  stream << "    Ranges:\n";
  for(KineVar_t kv : kinevars) {
    const pair<double,double> & range = this->GetKineRange(kv);
    stream << "        For " << util::AsString(kv) << ", range = [" << range.first << ", " << range.second << "]\n";
  }
  stream << "    Apply parameter to:\n";
  stream << "        Detector = "        << util::DetectorMaskAsString(fApply.detector) << "\n";
  stream << "        Beam = "            << util::BeamMaskAsString(fApply.beam)     << "\n";
  stream << "        Selection = "       << util::SelecMaskAsString(fApply.selec)    << "\n";
  stream << "        Reaction type = "   << util::ReacTypeMaskAsString(fApply.reactype) << "\n";
  stream << "        True nu flavour = " << util::FlavMaskAsString(fApply.flav)     << "\n";
  stream << "        Nuclear target = "  << util::NuclearTargetMaskAsString(fApply.target)   << "\n";
  stream << "    Physical limits = [" << limits.first << ", " << limits.second << "]" << "\n";
  stream << std::endl;
}
//________________________________________________________________________________________
bool Parameter::SearchCategories(string to_find, bool one_category_only) const
{
  if(one_category_only) {
    if(fCategories.size() != 1) {
      return false;
    }
  }
  to_find = util::ToLowerCase(util::TrimSpaces(to_find));
  if(to_find != "") {
    for(string cat : fCategories) {
      cat = util::ToLowerCase(cat);
      if(cat == to_find) {
        return true;
      }
    }
  }
  return false;
}
//________________________________________________________________________________________
bool Parameter::IsAppliedTo(BeamConf_t b, Sample_t s, Detector_t d, ReacMode_t m, NuclearTarget_t nt) const
{
  if(m == kmUndefined) { return false; }

  NuFlavour_t f = kReacModeToInitFlavour[m];
  ReacType_t t = kReacModeToType[m];

  return IsAppliedTo(b, s, d, f, t, nt);
}
//________________________________________________________________________________________
bool IsAppliedTo(BeamConf_t b, Sample_t s, Detector_t d, NuFlavour_t f, ReacType_t t, NuclearTarget_t nt) const
{
  if(b == kbUndefined)         { return false; }
  if(s == ksUndefined)         { return false; }
  if(d == kdUndefined)         { return false; }
  if(nt == kntUndefined)       { return false; }
  if(f == kNuFlavourUndefined) { return false; }
  if(t == kmtUndefined)        { return false; }

  // Check whether or not to apply the param to this
  bool apply_to_beam     = (kApplyToBeamMode     [b]  & fApply.beam).any();
  bool apply_to_sample   = (kApplyToSample       [s]  & fApply.selec).any();
  bool apply_to_flavour  = (kApplyToFlavour      [f]  & fApply.flav).any();
  bool apply_to_type     = (kApplyToReacType     [t]  & fApply.reactype).any();
  bool apply_to_detector = (kApplyToDetector     [d]  & fApply.detector).any();
  bool apply_to_target   = (kApplyToNuclearTarget[nt] & fApply.target).any();

  bool apply = (apply_to_beam && apply_to_sample && apply_to_flavour && apply_to_type && apply_to_detector && apply_to_target);

  return apply;
}
//________________________________________________________________________________________
const std::pair<double,double> & Parameter::GetKineRange(conv::KineVar_t kv) const
{
  for (size_t it = 0 ; it < fKinematicVariables.size(); it++) {
    conv::KineVar_t kvi = fKinematicVariables[it];
    if (kvi == kv) { return fKinematicRange[it]; }
  }
  LOG("Parameter",pFATAL) << "Param "<<fName<<" does not have requested dependence on "<<util::AsString(kv);
  util::Die(-1);
  return max_limits;
}
//________________________________________________________________________________________
double Parameter::IsWithinLimits(double val) const
{
  if (v >= fLimits.first && v <= fLimits.second) {
    return true;
  }
  return false;
}
//________________________________________________________________________________________
double Parameter::GetNearestPhysicalValue(double curr_val) const
{
  if (this->IsWithinLimits(curr_val)) {return curr_val;}
  if (curr_val < fLimits.first)       {return fLimits.first;}
  return fLimits.second;
}
//________________________________________________________________________________________

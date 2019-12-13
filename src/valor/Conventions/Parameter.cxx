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
  fApply = util::ReadApplyMaskAll(xml, fParamNode);
  ReadLimits();
  ReadFitInfo();
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
  fParam             = param_id;
  fName              = "undefined";
  fRooTeXName        = "undefined";
  fTeXName           = "undefined";
  fType              = kSystUndefined;
  fHasLimits         = false;
  fHasKinematicRange = false;
  fInit              = 1.;
  fError             = 1.;
  fRangeMode         = "sigma";
  fStepSize          = 1.;
  fIsFixed           = false;
  fIsNuisance        = true;
  fDoMarginalise     = true;

  fxml = xml;
  fParamNode = param_node;
}
//________________________________________________________________________________________
void Parameter::ReadNames()
{
  if(util::IsNodeSpecified(fxml, fParamNode, "name/simple")) {
    fName = util::XML2String(fxml, fParamNode, "name/simple");
  }
  else {
    LOG("FitInputs", pFATAL) << "No simple name specified for a parameter";
    util::Die(1);
  }

  if(util::IsNodeSpecified(fxml, fParamNode, "name/long")) {
    fNameLong = util::XML2String(fxml, fParamNode, "name/long");
  }
  else {
    LOG("FitInputs", pNOTICE) << "No long name specified for parameter \'"
                              << fName << "\', instead using the simple name";
    fNameLong = fName;
  }

  if(util::IsNodeSpecified(fxml, fParamNode, "name/rootex")) {
    fRooTeXName = util::XML2String(fxml, fParamNode, "name/rootex");
  }
  else {
    LOG("FitInputs", pNOTICE) << "No rootex name specified for parameter \'"
                              << fName << "\', instead using the simple name";
    // with underscores escaped
    fRooTeXName = util::ReplaceAll(fName,"_","#_");
  }

  if(util::IsNodeSpecified(fxml, fParamNode, "name/tex")) {
    fTeXName = util::XML2String(fxml, fParamNode, "name/tex");
  }
  else {
    LOG("FitInputs", pNOTICE) << "No Tex name specified for parameter \'"
                              << fName << "\', instead using the RooTex name with \'#\'replaced with \'\\\\\'";
    fTeXName = fRooTeXName;
    fTeXName = util::ReplaceAll(fTeXName,"#","\\"); // Make hashes slahes from RooTeX
    // Get rid of hashes preceding sin
    fTeXName = util::ReplaceAll(fTeXName,"#sin","sin");
    fTeXName = util::TrimSpaces(fTeXName);
  }
}
//________________________________________________________________________________________
void Parameter::ReadRanges()
{
  if(util::NodeExists(fxml, fParamNode, "apply_to_range")) {
    vector <XMLNodePointer_t> range_nodes = util::FindAllNodes(fxml,util::FindNode(fxml, fParamNode, "apply_to_range"));
    if(range_nodes.size() == 0) {
      LOG("FitInputs", pINFO) << "No parameter apply ranges specified for parameter \'" << fName << "\'";
      fHasKinematicRange = false;
      return;
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
      fHasKinematicRange = true;
      if(fKinematicRange.back().second < fKinematicRange.back().first) {
        LOG("FitInputs", pFATAL) << "For parameter " << fName << " and kinematic variable " << node_name
                                 << ", the value specified for the upper limit of the range is smaller than the value specified for the lower limit";
        util::Die(1);
      }
    }
  }
  fHasKinematicRange = (bool)fKinematicVariables.size();
}
//________________________________________________________________________________________
void Parameter::ReadType()
{
  if(util::IsNodeSpecified(fxml, fParamNode, "type")) {
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
void Parameter::ReadLimits()
{
  if(util::IsNodeSpecified(fxml, fParamNode, "limits")) {
    fHasLimits = true;
    fLimits = util::XML2DblDblPair(fxml, fParamNode, "limits");
  }
  else {
    fHasLimits = false;
    fLimits = max_limits;
  }
}
//________________________________________________________________________________________
void Parameter::ReadFitInfo()
{
  // Initial value
  if(util::IsNodeSpecified(fxml, fParamNode, "init")) {
    fInit = util::XML2Dbl(fxml, fParamNode, "init");
  }
  else {
    // Set to a flag that can be recoginised by e.g. CorrelatedSystematics
    // that indicates this value can be overwritten
    fInit = fOverwriteFlag;
  }

  // Error value
  if(util::IsNodeSpecified(fxml, fParamNode, "error")) {
    fError = util::XML2Dbl(fxml, fParamNode, "error");
  }
  else {
    // Set to a flag that can be recoginised by e.g. CorrelatedSystematics
    // that indicates this value can be overwritten
    fError = fOverwriteFlag;
  }

  // Range mode
  if(util::IsNodeSpecified(fxml, fParamNode, "range_mode")) {
    fRangeMode = util::XML2String(fxml, fParamNode, "range_mode");
  }
  else {
    fRangeMode = fDefRangeMode;
    LOG("FitInputs", pINFO) << "No fit info 'range' node was found, setting to \'" << fRangeMode <<"\'";
  }

  // Range
  std::pair<double,double> temp_pair;
  if(util::IsNodeSpecified(fxml, fParamNode, "range")) {
    fRange = util::XML2DblDblPair(fxml, fParamNode, "range");
  }
  else {
    // Set to a flag that can be recoginised by e.g. CorrelatedSystematics
    // that indicates this value can be overwritten
    fRange = std::make_pair(fOverwriteFlag, fOverwriteFlag);
  }

  // Step size
  if(util::IsNodeSpecified(fxml, fParamNode, "step")) {
    fStepSize = util::XML2Dbl(fxml, fParamNode, "step");
  }
  else {
    // Set to a flag that can be recoginised by e.g. CorrelatedSystematics
    // that indicates this value can be overwritten
    fStepSize = fOverwriteFlag;
  }

  // Is fixed?
  if(util::IsNodeSpecified(fxml, fParamNode, "fixed")) {
    fIsFixed = util::XML2Bool(fxml, fParamNode, "fixed");
  }
  else {
    fIsFixed = fDefIsFixed;
    LOG("FitInputs", pINFO) << "No fit info 'fixed' node was found, setting to \'" << fIsFixed <<"\'";
  }

  // Is nuisance?
  if(util::IsNodeSpecified(fxml, fParamNode, "nuisance")) {
    fIsNuisance = util::XML2Bool(fxml, fParamNode, "nuisance");
  }
  else {
    fIsNuisance = fDefIsNuisance;
    LOG("FitInputs", pINFO) << "No fit info 'nuisance' node was found, setting to \'" << fIsNuisance <<"\'";
  }

  // Marginalise?
  if(util::IsNodeSpecified(fxml, fParamNode, "marginalise")) {
    fDoMarginalise = util::XML2Bool(fxml, fParamNode, "marginalise");
  }
  else {
    fDoMarginalise = fDefDoMarg;
    LOG("FitInputs", pINFO) << "No fit info 'marginalise' node was found, setting to \'" << fDoMarginalise <<"\'";
  }

}
//________________________________________________________________________________________
void Parameter::Print(std::ostream & stream) const
{
  const vector<string>    & cats     = GetCategories();
  const vector<KineVar_t> & kinevars = GetKineVars();
  std::pair<double,double> limits = GetLimits();
  stream << "Parameter info:\n"
         << "    Current numerical ID: "<<fParam<<"\n"
         << "    Name (simple) = " << GetName()       << "\n"
         << "    Name (long) = "   << GetNameLong()   << "\n"
         << "    Name (RooTeX) = " << GetRooTeXName() << "\n"
         << "    Name (TeX) = "    << GetTeXName()    << "\n"
         << "    Type = " << util::AsString(GetType()) << "\n"
         << "    Categories:\n";
  for(string cat : cats) {
    stream << "        " << cat << "\n";
  }
  stream << "    Ranges:\n";
  if (fHasKinematicRange) {
    for(KineVar_t kv : kinevars) {
      const pair<double, double> &range = this->GetKineRange(kv);
      stream << "        For " << util::AsString(kv) << ", range = [" << range.first << ", " << range.second << "]\n";
    }
  }
  else {
    stream << "      Universal\n";
  }
  stream << "    Apply parameter to:\n";
  stream << "        Detector = "              << util::DetectorMaskAsString     (fApply.detector)   << "\n";
  stream << "        Beam = "                  << util::BeamMaskAsString         (fApply.beam)       << "\n";
  stream << "        Selection = "             << util::SelecMaskAsString        (fApply.selec)      << "\n";
  stream << "        Reaction type = "         << util::ReacTypeMaskAsString     (fApply.reactype)   << "\n";
  stream << "        True nu init flavour = "  << util::FlavMaskAsString         (fApply.flav_init)  << "\n";
  stream << "        True nu final flavour = " << util::FlavMaskAsString         (fApply.flav_final) << "\n";
  stream << "        Nuclear target = "        << util::NuclearTargetMaskAsString(fApply.target)     << "\n";
  stream << "    Physical limits = ["          << limits.first << ", " << limits.second << "]"       << "\n";
  stream << "    Fit info:\n";
  stream << "        Initial value = " << fInit                                 << "\n";
  stream << "        error = "         << fError                                << "\n";
  stream << "        range mode = "    << fRangeMode                            << "\n";
  stream << "        range = ["        << fRange.first << ", " << fRange.second << "]\n";
  stream << "        step size = "     << fStepSize                             << "\n";
  stream << "        fixed? "          << fIsFixed                              << "\n";
  stream << "        nuisance? "       << fIsNuisance                           << "\n";
  stream << "        marginalise? "    << fDoMarginalise                        << "\n";
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
bool Parameter::IsWithinLimits(double val) const
{
  if (!fHasLimits) {
    return true;
  }
  if (val >= fLimits.first && val <= fLimits.second) {
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
void Parameter::SetDefaultInitValue(void)
{
  fInit = fDefInit;
  LOG("FitInputs", pWARN) << "Setting initial value to default (" << fDefInit
                          << ") for parameter \'" << fName << "\'";
}
//________________________________________________________________________________________

void Parameter::SetDefaultError(void)
{
  fError = fDefError;
  LOG("FitInputs", pWARN) << "Setting initial value to default (" << fDefError
                          << ") for parameter \'" << fName << "\'";
}
//________________________________________________________________________________________

void Parameter::SetDefaultThrowRange(void)
{
  fRange.first  = fDefInit - fDefSymmetricRange * fDefError;
  fRange.second = fDefInit + fDefSymmetricRange * fDefError;
  LOG("FitInputs", pWARN) << "Setting range to default [" << fRange.first << ", "
                          << fRange.second << "] for parameter \'" << fName << "\'";
}
//________________________________________________________________________________________

void Parameter::SetDefaultStepSize(void)
{
  fStepSize = fDefStepSizeFactor * (fRange.second - fRange.first);
  LOG("FitInputs", pWARN) << "Setting step size to default (" << fStepSize
                          << ") for parameter \'" << fName << "\'";
}
//________________________________________________________________________________________

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
  Init(param_id, xml, param_node);
  ReadType();
  ReadNames();
  ReadRanges();
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
  fParam                      = param_id;
  fName                       = "undefined";
  fRooTeXName                 = "undefined";
  fTeXName                    = "undefined";
  fType                       = kSystUndefined;
  fHasLimits                  = false;
  fHasKinematicRange          = false;
  fInit                       = 1.;
  fError                      = 1.;
  fRangeMode                  = "sigma";
  fStepSize                   = 1.;
  fIsFixed                    = false;
  fIsNuisance                 = true;
  fDoMarginalise              = true;
  fUpdateFitInfo              = true;
  fNameLongWasSpecified       = false;
  fRooTeXNameWasSpecified     = false;
  fTeXNameWasSpecified        = false;
  fCategoriesWasSpecified     = false;
  fHasLimitsWasSpecified      = false;
  fLimitsWasSpecified         = false;
  fInitValueWasSpecified      = false;
  fErrorWasSpecified          = false;
  fThrowRangeModeWasSpecified = false;
  fThrowRangeWasSpecified     = false;
  fStepSizeWasSpecified       = false;
  fIsFixedWasSpecified        = false;
  fIsNuisanceWasSpecified     = false;
  fDoMarginaliseWasSpecified  = false;

  fxml = xml;
  fParamNode = param_node;
}
//________________________________________________________________________________________
void Parameter::ReadNames()
{
  // Simple name
  fName = util::ReadSimpleName(fxml, fParamNode);

  // Longer, more descriptive name
  if(util::IsNodeSpecified(fxml, fParamNode, "name/long")) {
    fNameLong = util::XML2String(fxml, fParamNode, "name/long");
    fNameLongWasSpecified = true;
  }
  else {
    if(fType != conv::kSystSpecial) {
      LOG("FitInputs", pNOTICE) << "No long name specified for parameter \'"
                                << fName << "\', instead using the simple name";
      fNameLong = fName;
    }
    else {
      // Special cases
      if(util::IsOscParam(fParam)) {
        fNameLong = util::RawParamAsString(fParam);
      }
      else {
        LOG("FitInputs", pFATAL) << "Unsupported special case";
        util::Die(1);
      }
    }
  }

  // RooTeX name
  if(util::IsNodeSpecified(fxml, fParamNode, "name/rootex")) {
    fRooTeXName = util::XML2String(fxml, fParamNode, "name/rootex");
    fRooTeXNameWasSpecified = true;
  }
  else {
    if(fType != conv::kSystSpecial) {
      LOG("FitInputs", pNOTICE) << "No rootex name specified for parameter \'"
                                << fName << "\', instead using the simple name";
      // with underscores escaped
      fRooTeXName = util::ReplaceAll(fName,"_","#_");
    }
    else {
      // Special cases
      if(util::IsOscParam(fParam)) {
        fRooTeXName = util::RawParamAsStringRooTex(fParam);
      }
      else {
        LOG("FitInputs", pFATAL) << "Unsupported special case";
        util::Die(1);
      }
    }
  }

  // TeX name
  if(util::IsNodeSpecified(fxml, fParamNode, "name/tex")) {
    fTeXName = util::XML2String(fxml, fParamNode, "name/tex");
    fTeXNameWasSpecified = true;
  }
  else {
    if(fType != conv::kSystSpecial) {
      LOG("FitInputs", pNOTICE) << "No Tex name specified for parameter \'"
                                << fName << "\', instead using the RooTex name with \'#\'replaced with \'\\\\\'";
      fTeXName = fRooTeXName;
      fTeXName = util::ReplaceAll(fTeXName, "#", "\\"); // Make hashes slahes from RooTeX
      // Get rid of hashes preceding sin
      fTeXName = util::ReplaceAll(fTeXName, "#sin", "sin");
      fTeXName = util::TrimSpaces(fTeXName);
    }
    else {
      // Special cases
      if(util::IsOscParam(fParam)) {
        fTeXName = util::RawParamAsStringTex(fParam);
      }
      else {
        LOG("FitInputs", pFATAL) << "Unsupported special case";
        util::Die(1);
      }
    }
  }
}
//________________________________________________________________________________________
void Parameter::ReadRanges()
{
  if(fType != conv::kSystSpecial) {
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
  } // normal case
  else {
    if(util::IsOscParam(fParam)) {
      fHasKinematicRange = true;
      fKinematicVariables.push_back(conv::kvEtrue);
      fKinematicRange.push_back(std::make_pair(0, max_limits.second));
      if(util::NodeExists(fxml, fParamNode, "apply_to_range")) {
        LOG("FitInputs", pWARN) << "For parameter " << fName
                                << ", an 'apply_to_range' node was found. This will be ignored and the parameter will be applied to [0, inf] in Etrue";
      }
    }
    else {
      LOG("FitInputs", pFATAL) << "Unsupported special case";
      util::Die(1);
    }
  } // special case
}
//________________________________________________________________________________________
void Parameter::ReadType()
{
  if(util::IsNodeSpecified(fxml, fParamNode, "type")) {
    std::string type_str = util::XML2String(fxml, fParamNode, "type");
    SystType_t type = util::SystTypeFromString(type_str);
    if(type == kSystUndefined) {
      // The user has tried to set a type for this param but it was not recognised, so die
      LOG("FitInputs", pFATAL) << "An input parameter's systematic type \'" << type_str << "\' not recognised";
      util::Die(1);
    }
    if(util::IsOscParam(fParam) && type != conv::kSystSpecial) {
      LOG("FitInputs", pFATAL) << "Parameter " << util::RawParamAsStringSimple(fParam)
                               << " is an oscillation parameter, but the input systematic type \'"
                               << type_str <<  "\' was not set to 'special'.";
      util::Die(-1);
    }
    fType = type;
  }
  else {
    // The user hasn't tried to set any type for this param, so give up
    LOG("FitInputs", pFATAL) << "An input parameter's systematic type was not set.";
    util::Die(-1);
  }
}
//________________________________________________________________________________________
void Parameter::ReadCategories()
{
  XMLNodePointer_t cats_node = util::FindNode(fxml, fParamNode, "categories");
  vector<XMLNodePointer_t> cat_nodes = util::FindNodes(fxml, cats_node, "category");
  if(fType != conv::kSystSpecial) {
    if(cat_nodes.size() == 0) {
      LOG("FitInputs", pFATAL) << "No category nodes specified for parameter \'" << fName <<"\'";
      util::Die(1);
    }
  }
  else {
    if(util::IsOscParam(fParam) && cat_nodes.size() == 0) {
      fCategories.push_back("oscillation");
    }
  }

  for(XMLNodePointer_t cat_node : cat_nodes) {
    string cat = util::TrimSpaces(fxml->GetNodeContent(cat_node));
    if(cat == "") {
      LOG("FitInputs", pFATAL) << "Empty category node for parameter \'" << fName <<"\'";
      util::Die(1);
    }
    fCategories.push_back(cat);
    fCategoriesWasSpecified = true;
  }
}
//________________________________________________________________________________________
void Parameter::ReadLimits()
{
  if(util::IsNodeSpecified(fxml, fParamNode, "limits")) {
    fHasLimits = true;
    fLimits = util::XML2DblDblPair(fxml, fParamNode, "limits");
    fHasLimitsWasSpecified = true;
    fLimitsWasSpecified = true;
  }
  else {
    fHasLimits = false;
    fLimits = max_limits;
  }
}
//________________________________________________________________________________________
void Parameter::ReadFitInfo()
{
  // Update the init value (and range if the range mode is 'sigma') from the covariance matrix file?
  if(util::IsNodeSpecified(fxml, fParamNode, "update_from_cov_matrix")) {
    fUpdateFitInfo = util::XML2Bool(fxml, fParamNode, "update_from_cov_matrix");
    if(fType == conv::kSystSpecial) {
      if(util::IsOscParam(fParam)) {
        if(fUpdateFitInfo) {
          LOG("FitInputs", pNOTICE)
            << "For osc param \'" << fName
            << "\', 'update_from_cov_matrix' was set to true, but this option does not apply to osc params, so will be forced to false";
        }
        fUpdateFitInfo = false;
      }
      else {
        LOG("FitInputs", pFATAL) << "Unsupported special case";
        util::Die(1);
      }
    }
  }
  else {
    if(fType != conv::kSystSpecial) {
      fUpdateFitInfo = fDefUpdateFitInfo;
    }
    else {
      if(util::IsOscParam(fParam)) {
        fUpdateFitInfo = false;
      }
      else {
        LOG("FitInputs", pFATAL) << "Unsupported special case";
        util::Die(1);
      }
    }
  }

  // Initial value
  if(util::IsNodeSpecified(fxml, fParamNode, "init")) {
    fInit = util::XML2Dbl(fxml, fParamNode, "init");
    fInitValueWasSpecified = true;
  }
  else {
    // Set to a flag that can be recoginised by e.g. CorrelatedSystematics
    // that indicates this value can be overwritten
    fInit = fDefInit;
  }

  // Error value
  if(util::IsNodeSpecified(fxml, fParamNode, "error")) {
    fError = util::XML2Dbl(fxml, fParamNode, "error");
    fErrorWasSpecified = true;
  }
  else {
    // Set to a flag that can be recoginised by e.g. CorrelatedSystematics
    // that indicates this value can be overwritten
    fError = fDefError;
  }

  // Range mode
  if(util::IsNodeSpecified(fxml, fParamNode, "range_mode")) {
    fRangeMode = util::XML2String(fxml, fParamNode, "range_mode");
    fThrowRangeModeWasSpecified = true;
  }
  else {
    fRangeMode = fDefRangeMode;
    LOG("FitInputs", pINFO) << "No fit info 'range' node was found for \'" << fName << "\', setting to \'" << fRangeMode <<"\'";
  }

  // Range
  if(util::IsNodeSpecified(fxml, fParamNode, "range")) {
    fRange = util::XML2DblDblPair(fxml, fParamNode, "range");
    fThrowRangeWasSpecified = true;
  }
  else {
    // Set to a flag that can be recoginised by e.g. CorrelatedSystematics
    // that indicates this value can be overwritten
    fRange = std::make_pair(-fDefSymmetricRange, fDefSymmetricRange);
  }

  // Step size
  if(util::IsNodeSpecified(fxml, fParamNode, "step")) {
    fStepSize = util::XML2Dbl(fxml, fParamNode, "step");
    fStepSizeWasSpecified = true;
  }
  else {
    // Set to a flag that can be recoginised by e.g. CorrelatedSystematics
    // that indicates this value can be overwritten
    // TODO: put in a better default for the step size
    fStepSize = fDefStepSizeFactor;
  }

  // Is fixed?
  if(util::IsNodeSpecified(fxml, fParamNode, "fixed")) {
    fIsFixed = util::XML2Bool(fxml, fParamNode, "fixed");
    fIsFixedWasSpecified = true;
  }
  else {
    fIsFixed = fDefIsFixed;
    LOG("FitInputs", pINFO) << "No fit info 'fixed' node was found for \'" << fName
                            << "\', setting to \'" << fIsFixed <<"\'";
  }

  // Is nuisance?
  if(util::IsNodeSpecified(fxml, fParamNode, "nuisance")) {
    fIsNuisance = util::XML2Bool(fxml, fParamNode, "nuisance");
    fIsNuisanceWasSpecified = true;
  }
  else {
    if(fType != conv::kSystSpecial) {
      fIsNuisance = fDefIsNuisance;
    }
    else {
      if(util::IsOscParam(fParam)) {
        fIsNuisance = false;
      }
      else {
        LOG("FitInputs", pFATAL) << "Unsupported special case";
        util::Die(1);
      }
    }
    LOG("FitInputs", pINFO) << "No fit info 'nuisance' node was found for \'" << fName
                            << "\', setting to \'" << fIsNuisance <<"\'";
  }

  // Marginalise?
  if(util::IsNodeSpecified(fxml, fParamNode, "marginalise")) {
    fDoMarginalise = util::XML2Bool(fxml, fParamNode, "marginalise");
    fDoMarginaliseWasSpecified = true;
  }
  else {
    if(fType != conv::kSystSpecial) {
      fDoMarginalise = fDefDoMarg;
    }
    else {
      if(util::IsOscParam(fParam)) {
        fDoMarginalise = false;
      }
      else {
        LOG("FitInputs", pFATAL) << "Unsupported special case";
        util::Die(1);
      }
    }
    LOG("FitInputs", pINFO) << "No fit info 'marginalise' node was found for \'" << fName
                            << "\', setting to \'" << fDoMarginalise <<"\'";
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
  stream << "        Error = "         << fError                                << "\n";
  stream << "        Range mode = "    << fRangeMode                            << "\n";
  stream << "        Range = ["        << fRange.first << ", " << fRange.second << "]\n";
  stream << "        Step size = "     << fStepSize                             << "\n";
  stream << "        Fixed? "          << fIsFixed                              << "\n";
  stream << "        Nuisance? "       << fIsNuisance                           << "\n";
  stream << "        Marginalise? "    << fDoMarginalise                        << "\n";
  stream << "        Will be updated by CorrelatedSystematics? " << fUpdateFitInfo << "\n";
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
const std::string util::ReadSimpleName(TXMLEngine* xml, const XMLNodePointer_t& param_node)
{
  std::string name = "";
  if(util::IsNodeSpecified (xml, param_node, "name/simple")) {
    name = util::XML2String(xml, param_node, "name/simple");
  }
  else {
    LOG("FitInputs", pFATAL) << "No simple name specified for a parameter";
    util::Die(1);
  }
  return name;
}
//________________________________________________________________________________________

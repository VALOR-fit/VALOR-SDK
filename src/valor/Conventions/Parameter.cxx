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
  fStepSizeMode               = "frac";
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
  fPhysicalLimitsWasSpecified = false;
  fInitValueWasSpecified      = false;
  fErrorWasSpecified          = false;
  fThrowRangeModeWasSpecified = false;
  fThrowRangeWasSpecified     = false;
  fStepSizeModeWasSpecified   = false;
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
      if(!util::IsSystematic(fParam)) {
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
      if(!util::IsSystematic(fParam)) {
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
      if(!util::IsSystematic(fParam)) {
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
    if(!util::IsSystematic(fParam)) {
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
    if(!util::IsSystematic(fParam) && type != conv::kSystSpecial) {
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
    if(!util::IsSystematic(fParam) && cat_nodes.size() == 0) {
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
    fPhysicalLimits = util::XML2DblDblPair(fxml, fParamNode, "limits");
    fHasLimitsWasSpecified = true;
    fPhysicalLimitsWasSpecified = true;
  }
  else {
    fHasLimits = false;
    fPhysicalLimits = max_limits;
  }
}
//________________________________________________________________________________________
void Parameter::ReadFitInfo()
{
  // Update the init value (and range if the range mode is 'sigma') from the covariance matrix file?
  if(util::IsNodeSpecified(fxml, fParamNode, "update_from_cov_matrix")) {
    fUpdateFitInfo = util::XML2Bool(fxml, fParamNode, "update_from_cov_matrix");
    if(fType == conv::kSystSpecial) {
      if(!util::IsSystematic(fParam)) {
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
      if(!util::IsSystematic(fParam)) {
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
    fInit = fDefInit;
  }

  // Error value
  if(util::IsNodeSpecified(fxml, fParamNode, "error")) {
    fError = util::XML2Dbl(fxml, fParamNode, "error");
    fErrorWasSpecified = true;
  }
  else {
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
    fHasLimits = true;
  }
  else {
    if(fType != conv::kSystSpecial) {
      fRange = std::make_pair(-fDefSymmetricRange, fDefSymmetricRange);
      fHasLimits = true;
    }
    else {
      if(!util::IsSystematic(fParam)) {
        fRange = max_limits;
        fHasLimits = false;
      }
      else {
        LOG("FitInputs", pFATAL) << "Unsupported special case";
        util::Die(1);
      }
    }
  }

  // Either the range_mode and the range must be specified together, or neither must be specified
  if(!( ( fThrowRangeModeWasSpecified &&  fThrowRangeWasSpecified ) ||
        (!fThrowRangeModeWasSpecified && !fThrowRangeWasSpecified ) )
    ) {
    LOG("FitInputs", pFATAL)
      << "Either the 'range_mode' and the 'range' nodes must be specified together, or neither must be specified";
    util::Die(1);
  }

  // If we're not going to update this range later and the range is specified in units of sigmas,
  // convert it to an absolute range here. But don't do this for disabled params
  if(fType != kSystDisabled) {
    if(fHasLimits && !fUpdateFitInfo && fRangeMode == "sigma") {
      if(fInitValueWasSpecified && fErrorWasSpecified) {
        fRange.first  = fInit + fRange.first  * fError;
        fRange.second = fInit + fRange.second * fError;
        fRangeMode == "value";
      }
      else {
        LOG("FitInputs", pFATAL) << "For parameter \'" << fName
                                 << "\', the throw range is specified in units of sigmas, "
                                 << "but both the initial value and the 1 sigma error must be specified";
        util::Die(1);
      }
    }
  }
  else {
    fRange = std::make_pair(-fDefSymmetricRange, fDefSymmetricRange);
  }

  // Step size mode
  if(util::IsNodeSpecified(fxml, fParamNode, "step_mode")) {
    fStepSizeMode = util::XML2String(fxml, fParamNode, "step_mode");
    fStepSizeModeWasSpecified = true;
  }
  else {
    fStepSizeMode = fDefStepSizeMode;
    if(fStepSizeMode == "sigma" && !fErrorWasSpecified) {
      // If we want the step size to be in units of N sigma but don't have sigma,
      // fall back to stepping a fraction of the range
      fStepSizeMode = "range";
    }
  }

  // Step size
  if(util::IsNodeSpecified(fxml, fParamNode, "step")) {
    fStepSize = util::XML2Dbl(fxml, fParamNode, "step");
    fStepSizeWasSpecified = true;
  }
  else {
    if(fStepSizeMode == "sigma") { fStepSize = fDefStepSizeNSigma; }
    else                         { fStepSize = fDefStepSizeFactor; }
  }

  // Either the step_mode and the step size must be specified together, or neither must be specified
  if(!( ( fStepSizeModeWasSpecified &&  fStepSizeWasSpecified ) ||
        (!fStepSizeModeWasSpecified && !fStepSizeWasSpecified ) )
    ) {
    LOG("FitInputs", pFATAL)
      << "Either the 'step_mode' and the 'step' nodes must be specified together, or neither must be specified";
    util::Die(1);
  }

  // If the step size is specified as a fraction, make sure it's between 0 and 1
  if(fStepSizeMode == "frac" && (fStepSize < 0 || fStepSize > 1)) {
    LOG("FitInputs", pFATAL) << "The step size was specified to be a fraction of the range, but its value \'"
                             << fStepSize <<"\' is outside of the range [0,1]";
    util::Die(1);
  }

  // If we're not going to update the step size later and it's is specified as a fractional range
  // or a number of sigmas, convert it to an absolute step size here
  if(!fUpdateFitInfo && fStepSizeMode == "frac") {
    if(fHasLimits) {
      fStepSize = (this->GetLimits().second - this->GetLimits().first) * fStepSize;
      fStepSizeMode == "value";
    }
    else {
      LOG("FitInputs", pFATAL) << "For parameter \'" << fName
                               << "\', the step size is specified as a fraction of the range of the limits, "
                               << "but no limits were specified";
      util::Die(1);
    }
  }
  if(!fUpdateFitInfo && fStepSizeMode == "sigma") {
    if(fErrorWasSpecified) {
      fStepSize *= fError;
      fDefStepSizeMode == "value";
    }
    else {
      LOG("FitInputs", pFATAL) << "For parameter \'" << fName
                               << "\', the step size is specified in units of sigmas, "
                               << "but the 1 sigma error was not specified";
      util::Die(1);
    }
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
      if(!util::IsSystematic(fParam)) {
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
      if(!util::IsSystematic(fParam)) {
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
  std::pair<double,double> physical_limits = GetPhysicalLimits();
  std::pair<double,double> combined_limits = GetLimits();
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
  stream << "    Physical limits = ["          << physical_limits.first << ", " << physical_limits.second << "]" << "\n";
  stream << "    Combined limits (physical and throw range) = [" << combined_limits.first << ", " << combined_limits.second << "]" << "\n";
  stream << "    Fit info:\n";
  stream << "        Initial value = " << fInit                                    << "\n";
  stream << "        Error = "         << fError                                   << "\n";
  stream << "        Range mode = "    << fRangeMode                               << "\n";
  stream << "        Range = ["        << fRange.first << ", " << fRange.second    << "]\n";
  stream << "        Step size = "     << fStepSize                                << "\n";
  stream << "        Fixed? "          << fIsFixed                                 << "\n";
  stream << "        Nuisance? "       << fIsNuisance                              << "\n";
  stream << "        Marginalise? "    << fDoMarginalise                           << "\n";
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
std::pair<double,double> Parameter::GetLimits (void) const
{
  std::pair<double,double> limits;
  double min1 = this->GetPhysicalLimits().first;
  double min2 = this->GetThrowRange()    .first;
  double max1 = this->GetPhysicalLimits().second;
  double max2 = this->GetThrowRange()    .second;

  // If the throw range is not 'value', convert it
  if(this->GetThrowRangeMode() == "sigma") {
    // Then min2 and max2 are in units of sigmas from the initial value
    min2 = this->GetInitValue() + min2 * this->GetError();
    max2 = this->GetInitValue() + max2 * this->GetError();
  }

  // Return the tighter set of limits
  limits.first  = std::max(min1, min2);
  limits.second = std::min(max1, max2);
  return limits;
}
//________________________________________________________________________________________
bool Parameter::IsWithinLimits(double val) const
{
  if (!fHasLimits) {
    return true;
  }
  const std::pair<double,double> limits = this->GetLimits();
  if (val >= limits.first && val <= limits.second) {
    return true;
  }
  return false;
}
//________________________________________________________________________________________
double Parameter::GetNearestPhysicalValue(double curr_val) const
{
  if (this->IsWithinLimits(curr_val)) {return curr_val;}
  const std::pair<double,double> limits = this->GetLimits();
  if (curr_val < limits.first) {return limits.first;}
  return limits.second;
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

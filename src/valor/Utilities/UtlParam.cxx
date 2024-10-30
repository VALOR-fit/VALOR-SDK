//________________________________________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________________________________________________

#include <algorithm>

// fmwk
#include "valor/Utilities/UtlParam.h"

#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Conventions/Constants.h"
#include "valor/Conventions/ParameterPool.h"
#include "valor/Conventions/FitParamApply.h"
#include "valor/Conventions/SystType.h"
#include "valor/Utilities/UtlSystem.h"

using namespace valor;
using namespace valor::conv;

//____________________________________________________________________________
conv::FitParamGroup_t util::GetParameterGroup(conv::PhysParamId_t p)
{
  const ParameterPool * pool = ParameterPool::Instance();
  if (p==conv::kPhypUndefined) {return conv::kpgUndefined;}
  else if(util::IsOscParam(p)) {return conv::kpgOscillations;}
  else if(pool->IsParamInPool(p)) {

    const Parameter & param = pool->GetParam(p);
    conv::ApplyMask_t mask = param.GetApplyMask();

    bool apply_numu_only =    (
                                (conv::kfApplyToNuMu    & mask.flav_init) .any() &&
                               !(conv::kfApplyToNuMuBar & mask.flav_init) .any() &&
                               !(conv::kfApplyToNuE     & mask.flav_init) .any() &&
                               !(conv::kfApplyToNuEBar  & mask.flav_init) .any()
                              ) ||
                              (
                                (conv::kfApplyToNuMu    & mask.flav_final).any() &&
                               !(conv::kfApplyToNuMuBar & mask.flav_final).any() &&
                               !(conv::kfApplyToNuE     & mask.flav_final).any() &&
                               !(conv::kfApplyToNuEBar  & mask.flav_final).any()
                              );

    bool apply_numubar_only = (
                                !(conv::kfApplyToNuMu    & mask.flav_init) .any() &&
                                 (conv::kfApplyToNuMuBar & mask.flav_init) .any() &&
                                !(conv::kfApplyToNuE     & mask.flav_init) .any() &&
                                !(conv::kfApplyToNuEBar  & mask.flav_init) .any()
                              ) ||
                              (
                                !(conv::kfApplyToNuMu    & mask.flav_final).any() &&
                                 (conv::kfApplyToNuMuBar & mask.flav_final).any() &&
                                !(conv::kfApplyToNuE     & mask.flav_final).any() &&
                                !(conv::kfApplyToNuEBar  & mask.flav_final).any()
                              );

    bool apply_nue_only =     (
                                !(conv::kfApplyToNuMu    & mask.flav_init) .any() &&
                                !(conv::kfApplyToNuMuBar & mask.flav_init) .any() &&
                                 (conv::kfApplyToNuE     & mask.flav_init) .any() &&
                                !(conv::kfApplyToNuEBar  & mask.flav_init) .any()
                              ) ||
                              (
                                !(conv::kfApplyToNuMu    & mask.flav_final).any() &&
                                !(conv::kfApplyToNuMuBar & mask.flav_final).any() &&
                                 (conv::kfApplyToNuE     & mask.flav_final).any() &&
                                !(conv::kfApplyToNuEBar  & mask.flav_final).any()
                              );

    bool apply_nuebar_only =  (
                                !(conv::kfApplyToNuMu    & mask.flav_init) .any() &&
                                !(conv::kfApplyToNuMuBar & mask.flav_init) .any() &&
                                !(conv::kfApplyToNuE     & mask.flav_init) .any() &&
                                 (conv::kfApplyToNuEBar  & mask.flav_init) .any()
                              ) ||
                              (
                                !(conv::kfApplyToNuMu    & mask.flav_final).any() &&
                                !(conv::kfApplyToNuMuBar & mask.flav_final).any() &&
                                !(conv::kfApplyToNuE     & mask.flav_final).any() &&
                                 (conv::kfApplyToNuEBar  & mask.flav_final).any()
                              );

    bool apply_fhc_only = (conv::kbApplyToFHC & mask.beam).any() &&
                         !(conv::kbApplyToRHC & mask.beam).any();

    bool apply_rhc_only = !(conv::kbApplyToFHC & mask.beam).any() &&
                           (conv::kbApplyToRHC & mask.beam).any();

    if(param.SearchCategories("flux") && apply_fhc_only && apply_numu_only)    {return conv::kpgFHCNuMuFlux;}
    if(param.SearchCategories("flux") && apply_fhc_only && apply_numubar_only) {return conv::kpgFHCNuMuBarFlux;}
    if(param.SearchCategories("flux") && apply_fhc_only && apply_nue_only)     {return conv::kpgFHCNuEFlux;}
    if(param.SearchCategories("flux") && apply_fhc_only && apply_nuebar_only)  {return conv::kpgFHCNuEBarFlux;}
    if(param.SearchCategories("flux") && apply_rhc_only && apply_numu_only)    {return conv::kpgRHCNuMuFlux;}
    if(param.SearchCategories("flux") && apply_rhc_only && apply_numubar_only) {return conv::kpgRHCNuMuBarFlux;}
    if(param.SearchCategories("flux") && apply_rhc_only && apply_nue_only)     {return conv::kpgRHCNuEFlux;}
    if(param.SearchCategories("flux") && apply_rhc_only && apply_nuebar_only)  {return conv::kpgRHCNuEBarFlux;}
    if(param.SearchCategories("xsec"))                                         {return conv::kpgXSec;}
    if(param.SearchCategories("had"))                                          {return conv::kpgHadronization;}
    if(param.SearchCategories("fsi"))                                          {return conv::kpgFSI;}
    if(param.SearchCategories("det"))                                          {return conv::kpgDetector;}
  }
  return conv::kpgUndefined;
}
//____________________________________________________________________________
std::pair<double,double> util::GetKineRange(conv::PhysParamId_t p, conv::KineVar_t k)
{
  // Init with the ranges used for osc params
  const Parameter & param = ParameterPool::Instance()->GetParam(p);
  if(!param.HasKinematicRange()) {
    VLOG("FitParams",pERROR)<<"Parameter " << param.GetName() << " has no kinematic range";
    util::Die(-1);
  }
  std::pair<double,double> range = param.GetKineRange(k);
  return range;
}
//____________________________________________________________________________
bool util::HasKinematicDependence(conv::PhysParamId_t p)
{
  const Parameter & param = ParameterPool::Instance()->GetParam(p);
  if(!param.HasKinematicRange()) { return false; }
  return true;
}
//____________________________________________________________________________
std::vector<PhysParamId_t> util::GetParameterSet(std::string set_name_in, bool only_exact_match)
{
  // Set the default apply mask (apply to all)
  conv::DetectorApplyMask_t      detector   = conv::kdApplyToAll;
  conv::BeamApplyMask_t          beam       = conv::kbApplyToAll;
  conv::FlavApplyMask_t          flav_init  = conv::kfApplyToAll;
  conv::FlavApplyMask_t          flav_final = conv::kfApplyToAll;
  conv::SelecApplyMask_t         selec      = conv::ksApplyToAll;
  conv::ReacTypeApplyMask_t      reactype   = conv::ktApplyToAll;
  conv::NuclearTargetApplyMask_t target     = conv::kntApplyToAll;
  conv::ApplyMask_t apply_mask = {detector, beam, flav_init, flav_final, selec, reactype, target};

  return util::GetParameterSet(set_name_in, apply_mask, only_exact_match);
}
//____________________________________________________________________________
std::vector<PhysParamId_t> util::GetParameterSet(std::string set_name_in, conv::ApplyMask_t mask, bool only_exact_match)
{
  vector<PhysParamId_t> vec;
  std::string set_names = util::RemoveAllWhitespace(util::ToLowerCase(set_name_in));
  std::vector<string> set_names_vec = util::Tokens(set_names, ",");

  for(std::string set_name : set_names_vec) {
    vector<PhysParamId_t> vec_to_append;
    if(set_name != "all" && set_name != "3fosc") {
      const std::vector<Parameter> & params = ParameterPool::Instance()->GetParams();
      for (const Parameter & param : params) {
        // Search both the param name and categories
        if((!only_exact_match && param.GetName().find(set_name) != string::npos) ||                    // Search param name (search for sub-string, not exact match)
           param.SearchCategories(set_name) ||                                                         // Search param category for a direct match
           // Special cases:
           (set_name == "ndflux"         && param.SearchCategories("nd") && param.SearchCategories("flux") )  ||  // ND flux
           (set_name == "ndfluxnumu"     && param.SearchCategories("sbnd") && param.SearchCategories("flux")  && param.SearchCategories("numu"))  ||  // ND flux
           (set_name == "ndfluxnue"     && param.SearchCategories("sbnd") && param.SearchCategories("flux")  && param.SearchCategories("nue"))  ||  // ND flux
           (set_name == "fdflux"         && param.SearchCategories("fd") && param.SearchCategories("flux") )  ||  // FD flux
           (set_name == "nddet"          && param.SearchCategories("nd") && param.SearchCategories("det")  )  ||  // ND Detector
           (set_name == "fddet"          && param.SearchCategories("fd") && param.SearchCategories("det")  )  ||  // FD Detector
           (set_name == "allxsecsplines" && param.SearchCategories("int") && param.SearchCategories("splines")  )  ||  // FD Detector
           (set_name == "allsyst"        && util::IsSystematic(param.GetID()))                                  // All systematic params
          ) {
          vec_to_append.push_back(param.GetID());
        }
      }
    }
    else if(set_name == "all") {
      const std::vector<Parameter> & params = ParameterPool::Instance()->GetParams();
      for (const Parameter & param : params) {
        vec_to_append.push_back(param.GetID());
      }
    }

    // OA parameters
    if (set_name == "all" || set_name == "3fosc") {
      vec_to_append.push_back(kPhypMixing23);
      vec_to_append.push_back(kPhypMixing13);
      vec_to_append.push_back(kPhypMixing12);
      vec_to_append.push_back(kPhypCPV     );
      vec_to_append.push_back(kPhypDm23sq  );
      vec_to_append.push_back(kPhypDm12sq  );
    }

    // If we've failed to identify a set of parameters, try a single parameter. Look for an exact match
    if (vec_to_append.size()==0) {
      PhysParamId_t try_param = util::NameRegistry::Instance()->PhysParamFromString(set_name);
      if(try_param != kPhypUndefined) { vec_to_append.push_back(try_param); }
      else { VLOG("FitParams",pWARN)<<"No parameters added to set: "<<set_name; }
    }

    // Append the vector for this param set to the main vector
    vec.insert(vec.end(), vec_to_append.begin(), vec_to_append.end());
  }

  // Loop over the vector and remove any params that aren't compatible with the input apply mask
  for(size_t ip = 0; ip < vec.size(); ip++) {
    PhysParamId_t p = vec[ip];
    Parameter param = ParameterPool::Instance()->GetParam(p);
    if(!util::IsAppliedTo(param.GetApplyMask(), mask)) {
      vec.erase(vec.begin()+ip);
      ip--;
    }
  }

  return vec;
}
//____________________________________________________________________________
std::vector<conv::PhysParamId_t> util::GetActiveParameters(const PhysHypothesis & ph)
{
  if(ph.GetParams().size() == 0) {
    VLOG("FitParams",pWARN) << "No parameters in the PhysHypothesis";
  }
  if(ParameterPool::Instance()->GetParams().size() == 0) {
    VLOG("FitParams",pWARN)
      << "No parameters in the ParameterPool. If this is unintended then read in a config file to populate it";
  }

  std::vector<conv::PhysParamId_t> vec;
  for(const Parameter& param : ParameterPool::Instance()->GetParams()) {
    conv::PhysParamId_t p = param.GetID();
    if(p == conv::kPhypUndefined) { continue; }
    if(!util::IsSystematic(p)) {
      if(ph.Contains(p)) {
        vec.push_back(p);
      }
    }
    else {
      vec.push_back(p);
    }
  }
  return vec;
}
//____________________________________________________________________________

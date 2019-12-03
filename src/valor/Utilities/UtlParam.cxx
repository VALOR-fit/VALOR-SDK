//________________________________________________________________________________________________________________________
/*

Copyright (c) 2010-2019, The VALOR Fitting Group, All Rights Reserved.

options for the fit param list (must match the options in FitParamList::NamedParameterList):
        - nuosc3f:  standard PMNS osc params
        - sterile:  effective sterile mixing params
        etc.
        In addition to the above osc params, sytematic params are taken from the instance of ParameterPool

additional options
        - no-int-systematics: skipping neutrino interaction systematics in the list construction
        - no-flux-systematics, no-nd-flux-systematics, no-fd-flux-systematics: as above for flux
        - no-detector-systematics, no-nd-detector-systematics, no-fd-detector-systematics: as above for efficiencies
        - no-osc-params: skipping oscillation parameters in the list construction
        - only-nd: skipping far detector parameters in the list construction
        - five-sigma-range: Set the parameter min/max value to using the 5 sigma error (depends on the syst type). Defaults to 3 sigma limits

options after list construction
        - no-osc-params     : 
        - fix-osc-params    : fix all oscillation parameters
        - cons-osc-params   : set all osc params as nuisance - Note: to constrain only one (i.e. theta13, usual reactor)
                              use this one and fix other osc params independently (see run_fit options)
        - cons-atm-params   : set the 23 parameters as nuisance
        - cons-solar-params : set the 12 parameters as nuisance
        - fix-det-syst      : fix all detector params included in the list
        - fix-nd-det-syst   : 
        - fix-fd-det-syst   : 
        - fix-flux-syst     : fix all flux params included in the list
        - fix-nd-flux-syst  : 
        - fix-fd-flux-syst  : 
        - fix-int-syst      : fix all neutrino interaction systematic parameters
        - fix-mec-syst      : fix mec params
        - fix-fsi-syst      : fix the FSI params
        - fix-osc-param: 
        - cons_osc_params: 
        - cons_atm_params:     
        - cons_solar_params:   
        - fix_detector_params: 
        - fix_flux_params:     
        - fix_int_params:      
        - fix_mec_params:      
        - fix_fsi_params:      
*/
//____________________________________________________________________________________________________________________

#include <algorithm>

// fmwk
#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Conventions/ParameterPool.h"
#include "valor/Conventions/FitParamApply.h"
#include "valor/Conventions/SystType.h"
#include "valor/Utilities/UtlSystem.h"
// dune
#include "dune/Conventions/OscParamValues.h"
#include "dune/Utilities/UtlParam.h"
#include "dune/Conventions/PhysicalParameterLimits.h"

using namespace valor;
using namespace valor::conv;
//________________________________________________________________________________________
FitParamList * util::GetFitParameterList(Option_t * param_list_name, Option_t * param_opt , const CorrelatedSystematics * syst)
{
// Create a FitParamList object, adding all physics (from FitParamList::NamedParameterList) + nuisance (from the ParameterPool) params.
// The input option specifies both the set of parameters (eg "standard")


  string param_list_name_str = ((param_list_name) ? param_list_name : "nuosc3f");
  string param_list_opt      = ((param_opt)       ? param_opt       : "NONE");

  LOG("FitParams", pDEBUG)
    << "Instantiating a fit parameter list object with named parameter list: " << param_list_name_str
    << "\n and option: " << param_list_opt;

  //get the extra options here:
  bool no_osc                 = (param_list_opt.find("no-osc-params"    ) != string::npos);
  bool fix_osc_params         = (param_list_opt.find("fix-osc-params"   ) != string::npos);
  bool cons_osc_params        = (param_list_opt.find("cons-osc-params"  ) != string::npos);
  bool cons_atm_params        = (param_list_opt.find("cons-atm-params"  ) != string::npos);
  bool cons_solar_params      = (param_list_opt.find("cons-solar-params") != string::npos);
  bool cons_theta13           = (param_list_opt.find("cons-theta13"     ) != string::npos);
  bool fix_all                = (param_list_opt.find("fix-all"          ) != string::npos);
  bool fix_detector_params    = (param_list_opt.find("fix-det-syst"     ) != string::npos);
  bool fix_nd_detector_params = (param_list_opt.find("fix-nd-det-syst"  ) != string::npos);
  bool fix_fd_detector_params = (param_list_opt.find("fix-fd-det-syst"  ) != string::npos);
  bool fix_flux_params        = (param_list_opt.find("fix-flux-syst"    ) != string::npos);
  bool fix_nd_flux_params     = (param_list_opt.find("fix-nd-flux-syst" ) != string::npos);
  bool fix_fd_flux_params     = (param_list_opt.find("fix-fd-flux-syst" ) != string::npos);
  bool fix_int_params         = (param_list_opt.find("fix-int-syst"     ) != string::npos);
  bool fix_mec_params         = (param_list_opt.find("fix-mec-syst"     ) != string::npos);
  bool fix_fsi_params         = (param_list_opt.find("fix-fsi-syst"     ) != string::npos);
  bool only_nd                = (param_list_opt.find("only-nd"          ) != string::npos);
  bool use_five_sigma_range   = (param_list_opt.find("five-sigma-range" ) != string::npos);

  const std::vector<Parameter> & pool_params = ParameterPool::Instance()->GetParams();
  FitParamList * fparm = 0;

  // Set the number of syst parameters
  int nparams_syst = 0;
  if (syst) {
    nparams_syst = syst->Size();
  }
  else{
    nparams_syst = pool_params.size();
  }
  int nparams_osc = 0;

  // Get a FitParamList containing only the osc params and having empty slots for the syst params
  bool is_sterile = param_list_name_str.find("sterile") != string::npos;
  if(!no_osc && (is_sterile || !only_nd)) { // Make sure steriles are in the FPL even if we're only considering near detectors
    fparm = FitParamList::NamedParameterList(param_list_name_str, nparams_syst);
    if(!fparm) {
      LOG("FitParams", pERROR) << "Could not get a named parameter list using input name = " << param_list_name_str;
      util::Die(-1);
    }
    //if osc params constrained, set priors
    if(cons_osc_params) {
      util::InitializeOAPriors();   // TODO: This is DUNE specific and needs moving to a config file, but can be left in for now
    }
    nparams_osc = FitParamList::NamedParameterList(param_list_name_str, 0)->Size();
  }
  else {
    fparm = new FitParamList(nparams_syst);
  }

  int nparams = nparams_syst + nparams_osc;

  // If necessary, add the systematics from the model
  if (syst) {
    syst->AppendParamsTo(*fparm);
    // And update their limits`
    for (unsigned int ip = 0 ; ip < syst->Size(); ip++) {
      conv::PhysParamId_t p = syst->WhichParam(ip);
      // If we don't know what type of syst param this is, use these default limits
      double min_val = 0.;  // TODO: Do we want these hard-coded values in a config file?
      double max_val = 2.;
      // If the parameter is also present in the ParameterPool, set the limits based on what type of syst it is
      if(ParameterPool::Instance()->IsParamInPool(p)) {
        const Parameter & param = ParameterPool::Instance()->GetParam(p);
        double sigma_limit = use_five_sigma_range ? 5. : 3.;  // TODO: Do we want these hard-coded values in a config file?
        double central_val = syst->InitialValue(p);
        bool min_set = false;
        bool max_set = false;
        if(param.HasLimits()) {
          min_val = param.GetLimits().first;
          max_val = param.GetLimits().second;
          // The physical limits for a param can be e.g. +/- inf (stored as kALargeNum),
          // but we don't actually want the param to vary beyond 3 or 5 sigma away from nominal (unless a specific
          // non-infinite range is specified in the param config file)
          if(min_val != -conv::kALargeNum) { min_set = true; }
          if(max_val !=  conv::kALargeNum) { max_set = true; }
        }
        if(!min_set) { min_val = central_val - sigma_limit * syst->OneSigmaError(p); min_set = true; }
        if(!max_set) { max_val = central_val + sigma_limit * syst->OneSigmaError(p); max_set = true; }
        // Nomalisation and lateral params can't go negative
        if(min_val < 0. &&
          (param.GetType() == conv::kSystNorm ||
           param.GetType() == conv::kSystLateral ||
           param.GetType() == conv::kSystLateralPostOsc)) {
          min_val = 0.;
        }
      }
      fparm->SetMinValue(p, PhysicalParameterLimits::Instance()->GetNearestPhysicalValue(p, min_val));
      fparm->SetMaxValue(p, PhysicalParameterLimits::Instance()->GetNearestPhysicalValue(p, max_val));
    }
  }
  // Or add parameters from the ParameterPool
  else {
    int ipc = nparams_osc; //counter with no gaps, in case of `kPhypUndefined' params
    for (const Parameter & param : pool_params) {
      PhysParamId_t p = param.GetID();
      if(p == kPhypUndefined) {
        LOG("FitParams", pFATAL) << "Found an undefined parameter in the ParameterPool";
        util::Die(-1);
      }

      if(only_nd && !param.SearchCategories("ND")) { continue; }

      // set parameter range, current and init value, etc
      // TODO: It would be better to have the option of getting non-default values of the following params from a config file, but this is fine for now
      double init = 1.0;
      double curr = 1.0;
      double min  = 0.0;
      double max  = 2.0;
      double step = 1E-4;
      bool   fix  = false;
      bool   nuis = true;

      // Check we're not going behind physical region
      min = param.GetNearestPhysicalValue(min);
      max = param.GetNearestPhysicalValue(min);

      fparm->AddParam(ipc, p, init, min, max, step, fix, nuis);
      ipc++;
      fparm->Update(p, curr);
    } // Params in ParameterPool
  }

  // Now that we have all the params we want in the FitParamList, fix/set parameters as necessary
  for(int ip = 0; ip < nparams; ip++) {
    conv::PhysParamId_t p = fparm->WhichParam(ip);

    // Special handling for the osc params, as we might want to set different fit info to those set in FitParamList::NamedParameterList
    if(util::IsOscParam(p)) {
      if (no_osc) {continue;}
      // set parameter range, current and init value, etc
      double init = util::OscParamInitValue(p);  // TODO: This is DUNE specific and needs moving to a config file
      double curr = util::OscParamInitValue(p);  // TODO: This is DUNE specific and needs moving to a config file
      double min  = util::OscParamMinValue(p);   // TODO: This is DUNE specific and needs moving to a config file
      double max  = util::OscParamMaxValue(p);   // TODO: This is DUNE specific and needs moving to a config file
      double step = util::OscParamStepSize(p);   // TODO: This is DUNE specific and needs moving to a config file
      bool nuis = false;
      if(init != -1) { fparm->SetInitValue(p, init); }
      if(curr != -1) { fparm->Update(p, curr); }
      if(step != -1) { fparm->SetStepSize(p, step); }
      fparm->SetIsNuisance(p, nuis);
      // Check we're not going behind physical region
      if(min != -1) {
        min = PhysicalParameterLimits::Instance()->GetNearestPhysicalValue(p,min);
        fparm->SetMinValue(p, min);
      }
      if(max != -1) {
        max = PhysicalParameterLimits::Instance()->GetNearestPhysicalValue(p,max);
        fparm->SetMaxValue(p, max);
      }

      // toggle oscillation params on/off
      if(fix_osc_params || fix_all){
        // TODO: update these with T2K values!
        double init_value = util::OscParamInitValue(p);   // TODO: This is DUNE specific and needs moving to a config file
        if(init != -1) {
          fparm->Update(p, init_value);
          fparm->SetInitValue(p, init_value);
          fparm->FixParameter(p);
        }
      }

      //constrain osc params
      if(cons_osc_params){
        fparm->SetNuisance(p,true);
      }
    }
    else {
      if(!ParameterPool::Instance()->IsParamInPool(p)) {
        LOG("FitParams",pERROR)<<"When attempting to fix/set systematic parameters, parameter \'"
                               << NameRegistry::Instance()->AsString(p)
                               << "\' was not found in the ParameterPool so could not be categorised.";
        util::Die(-1); return fparm;
      }
      const Parameter & param = ParameterPool::Instance()->GetParam(p);

      // Set all systematic parameters to be marginalised. This is fine even if the FPL is not used in marginalisation,
      // as in that case the flag is not used
      fparm->SetDoMarg(p, true);

      double init = 1.;
      if (syst) {
        init = syst->InitialValue(p);
      }

      // Fix all systematics
      if(fix_all) {
        fparm->Update(p, init);
        fparm->SetInitValue(p, init);
        fparm->FixParameter(p);
      }
      // toggle detector related params on/off
      if(fix_detector_params && param.SearchCategories("det")) {
        fparm->Update(p, init);
        fparm->SetInitValue(p, init);
        fparm->FixParameter(p);
      }
      if(fix_nd_detector_params && param.SearchCategories("det") && param.SearchCategories("ND")) {
        fparm->Update(p, init);
        fparm->SetInitValue(p, init);
        fparm->FixParameter(p);
      }
      if(fix_fd_detector_params && param.SearchCategories("det") && param.SearchCategories("FD")) {
        fparm->Update(p, init);
        fparm->SetInitValue(p, init);
        fparm->FixParameter(p);
      }
      // toggle flux related params on/off
      if(fix_flux_params && param.SearchCategories("flux")) {
        fparm->Update(p, init);
        fparm->SetInitValue(p, init);
        fparm->FixParameter(p);
      }
      // toggle nd flux related params on/off
      if(fix_nd_flux_params && param.SearchCategories("ND") && param.SearchCategories("flux")){
        fparm->Update(p, init);
        fparm->SetInitValue(p, init);
        fparm->FixParameter(p);
      }
      // toggle fd flux related params on/off
      if(fix_fd_flux_params && param.SearchCategories("FD") && param.SearchCategories("flux")){
        fparm->Update(p, init);
        fparm->SetInitValue(p, init);
        fparm->FixParameter(p);
      }
      // toggle int related params on/off
      if(fix_int_params && param.SearchCategories("int")) {
        fparm->Update(p, init);
        fparm->SetInitValue(p, init);
        fparm->FixParameter(p);
      }
      // toggle mec related params on/off
      if(fix_mec_params && (util::ToLowerCase(param.GetName()).find("mec")  != std::string::npos ||
                            util::ToLowerCase(param.GetName()).find("2p2h") != std::string::npos)) {
        fparm->Update(p, init);
        fparm->SetInitValue(p, init);
        fparm->FixParameter(p);
      }

      // toggle FSI related params on/off
      if(fix_fsi_params && param.SearchCategories("FSI")) {
        fparm->Update(p, init);
        fparm->SetInitValue(p, init);
        fparm->FixParameter(p);
      }
    }
  } // ip

  //constrain solar params
  if(cons_solar_params){
    fparm->SetNuisance(conv::kPhypMixing12,true);
    fparm->SetNuisance(conv::kPhypDm12sq  ,true);
  }

  //constrain atmospheric params
  if(cons_atm_params){
    fparm->SetNuisance(conv::kPhypMixing23,true);
    fparm->SetNuisance(conv::kPhypDm23sq  ,true);
  }

  if (cons_theta13) {
    fparm->SetNuisance(conv::kPhypMixing13,true);
  }

  if(!fparm->IsComplete()) {
    LOG("FitParams",pERROR)<<"Fit parameter list is not complete";
    fparm->Print();
    util::Die(-1); return fparm;
  }

  return fparm;
}
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
  std::pair<double,double> range = util::PhysicalLimits(k);
  // If it's not an osc param, get the kine range from the parameter pool
  if(!util::IsOscParam(p)) {
    const Parameter & param = ParameterPool::Instance()->GetParam(p);
    if(!param.HasKinematicRange()) {
      LOG("FitParams",pERROR)<<"Parameter " << param.GetName() << " has no kinematic range";
      util::Die(-1);
    }
    range = param.GetKineRange(k);
  }
  return range;
}
//____________________________________________________________________________
bool util::HasKinematicDependence(conv::PhysParamId_t p)
{
  if(util::IsOscParam(p)) { return true; }
  else {
    const Parameter & param = ParameterPool::Instance()->GetParam(p);
    if(!param.HasKinematicRange()) { return false; }
    return true;
  }
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
           set_name == "ndflux"  && param.SearchCategories("nd") && param.SearchCategories("flux") ||  // ND flux
           set_name == "fdflux"  && param.SearchCategories("fd") && param.SearchCategories("flux") ||  // FD flux
           set_name == "nddet"   && param.SearchCategories("nd") && param.SearchCategories("det")  ||  // ND Detector
           set_name == "fddet"   && param.SearchCategories("fd") && param.SearchCategories("det")  ||  // FD Detector
           set_name == "allsyst"                                                                       // All systematic params
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
      else { LOG("FitParams",pWARN)<<"No parameters added to set: "<<set_name; }
    }

    // Append the vector for this param set to the main vector
    vec.insert(vec.end(), vec_to_append.begin(), vec_to_append.end());
  }

  // Loop over the vector and remove any params that aren't compatible with the input apply mask
  for(int ip = 0; ip < vec.size(); ip++) {
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
  // Get the osc params
  std::vector<conv::PhysParamId_t> vec = ph.GetParams();
  if(vec.size() == 0) {
    LOG("FitParams",pWARN) << "No parameters in the PhysHypothesis";
  }
  // Get the syst params
  const std::vector<Parameter> & syst_params = ParameterPool::Instance()->GetParams();
  if(syst_params.size() == 0) {
    LOG("FitParams",pWARN)
      << "No parameters in the ParameterPool. If this is unintended then read in a config file to populate it";
  }
  for (const Parameter & param : syst_params) {
    if(param.GetID() != conv::kPhypUndefined) { vec.push_back(param.GetID()); }
  }
  return vec;
}
//____________________________________________________________________________

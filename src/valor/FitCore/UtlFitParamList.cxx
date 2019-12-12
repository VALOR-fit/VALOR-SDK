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
#include "valor/FitCore/UtlFitParamList.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Conventions/Constants.h"
#include "valor/Conventions/ParameterPool.h"
#include "valor/Conventions/FitParamApply.h"
#include "valor/Conventions/SystType.h"
#include "valor/FitCore/PhysicalParameterLimits.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/OscParamValues.h"


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

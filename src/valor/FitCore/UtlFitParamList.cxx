//________________________________________________________________________________________________________________________
/*

Copyright (c) 2010-2019, The VALOR Fitting Group, All Rights Reserved.

additional options
        - no-int-systematics: skipping neutrino interaction systematics in the list construction  TODO: not yet implemented
        - no-flux-systematics, no-nd-flux-systematics, no-fd-flux-systematics: as above for flux  TODO: not yet implemented
        - no-detector-systematics, no-nd-detector-systematics, no-fd-detector-systematics: as above for efficiencies  TODO: not yet implemented
        - no-osc-params: skipping oscillation parameters in the list construction
        - only-nd: skipping far detector parameters in the list construction

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
#include <string>
#include <utility>

// fmwk
#include "valor/FitCore/UtlFitParamList.h"
#include "valor/FitCore/PhysParamList.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Conventions/ParameterPool.h"


using namespace valor;
using namespace valor::conv;
//________________________________________________________________________________________
FitParamList * util::GetFitParameterList(std::string hypothesis_name, Option_t * param_opt)
{
  PhysHypothesis hyp(hypothesis_name);
  return util::GetFitParameterList(hyp, param_opt);
}
//________________________________________________________________________________________
FitParamList * util::GetFitParameterList(const PhysHypothesis& hyp, Option_t * param_opt)
{
  // Create a FitParamList object, adding all physics + nuisance params from the instance of ParameterPool.
  string param_list_opt = ((param_opt) ? param_opt : "NONE");

  LOG("FitParams", pDEBUG)
    << "Instantiating a fit parameter list object from the ParameterPool with option(s): " << param_list_opt;

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

  const std::vector<Parameter> & pool_params = ParameterPool::Instance()->GetParams();

  // Set the number of parameters based on the parameters and the above options
  size_t nparams_syst = ParameterPool::Instance()->GetNumSystParams();
  size_t nparams_osc  = ParameterPool::Instance()->GetNumOscParams ();
  for(const Parameter& param : pool_params) {
    if(!util::IsSystematic(param.GetID()) && ( !hyp.Contains(param.GetID()) || no_osc )) { nparams_osc--;  }
    if(only_nd && !param.SearchCategories("ND"))   { nparams_syst--; }
  }
  size_t nparams = nparams_syst + nparams_osc;

  FitParamList* fparm = new FitParamList((int)nparams);

  // Add parameters from the pool
  int ipc = 0; //counter with no gaps, in case of `kPhypUndefined' params
  for(const Parameter& param : pool_params) {
    PhysParamId_t p = param.GetID();
    if(p == kPhypUndefined) {
      LOG("FitParams", pFATAL) << "Found an undefined parameter in the ParameterPool";
      util::Die(-1);
    }

    // Skip parameters if needed
    if(!hyp.Contains(p) && !util::IsSystematic(p)) { continue; }  // The pool could contain many osc params taken from a defaults file, but we might not want them all in the FPL
    if(no_osc  && !util::IsSystematic(p))          { continue; }
    if(only_nd && !param.SearchCategories("ND"))   { continue; }

    double      init = param.GetInitValue();
    double      min  = param.GetLimits().first;
    double      max  = param.GetLimits().second;
    double      step = param.GetStepSize();
    bool        fix  = param.IsFixed();
    bool        nuis = param.IsNuisance();
    bool        marg = param.DoMarginalise();
    double      curr = init;
    std::string name = "";
    if(!util::IsSystematic(p)) { name = param.GetNameLong(); }
    else                       { name = param.GetName();     }

    // Fix parameters as necessary
    if(( fix_all                                                                                          ) ||
       ( fix_osc_params         && !util::IsSystematic(p)                                                 ) ||
       ( fix_detector_params    && param.SearchCategories("det")                                          ) ||
       ( fix_nd_detector_params && param.SearchCategories("det") && param.SearchCategories("ND")          ) ||
       ( fix_fd_detector_params && param.SearchCategories("det") && param.SearchCategories("FD")          ) ||
       ( fix_flux_params        && param.SearchCategories("flux")                                         ) ||
       ( fix_nd_flux_params     && param.SearchCategories("ND") && param.SearchCategories("flux")         ) ||
       ( fix_fd_flux_params     && param.SearchCategories("FD") && param.SearchCategories("flux")         ) ||
       ( fix_int_params         && param.SearchCategories("int")                                          ) ||
       ( fix_mec_params         && (util::ToLowerCase(param.GetName()).find("mec")  != std::string::npos ||
                                    util::ToLowerCase(param.GetName()).find("2p2h") != std::string::npos) ) ||
       ( fix_fsi_params         && param.SearchCategories("FSI")                                          )
      ) {
      fix = true;
    }

    // Constrain osc params as necessary
    if(( cons_osc_params   && !util::IsSystematic(p)   ) ||
       ( cons_solar_params && p == conv::kPhypMixing12 ) ||
       ( cons_solar_params && p == conv::kPhypDm12sq   ) ||
       ( cons_atm_params   && p == conv::kPhypMixing23 ) ||
       ( cons_atm_params   && p == conv::kPhypDm23sq   ) ||
       ( cons_theta13      && p == conv::kPhypMixing13 )
      ) {
      nuis = true;
    }

    // Check we're not going behind physical region
    min = param.GetNearestPhysicalValue(min);
    max = param.GetNearestPhysicalValue(max);

    fparm->AddParam(ipc, p, init, min, max, step, fix, nuis);
    fparm->Update(p, curr);
    fparm->SetDoMarg(p, marg);
    ipc++;
  } // loop over Parameters

  if(!fparm->IsComplete()) {
    LOG("FitParams",pERROR)<<"Fit parameter list is not complete";
    fparm->Print();
    util::Die(-1); return fparm;
  }

  // Finally, check that all of the params in the PhysHypothesis are in the FPL
  for(conv::PhysParamId_t hyp_par : hyp.GetParams()) {
    if(!fparm->Exists(hyp_par)) {
      LOG("FitParams",pERROR)<<"Parameter \'" << NameRegistry::Instance()->AsString(hyp_par)
                             << "\' from the PhysHypothesis was not found in the FitParamList";
      fparm->Print();
      util::Die(-1); return fparm;
    }
  }

  return fparm;
}
//____________________________________________________________________________
bool util::AllWithinLimits(const FitParamList & fparm)
{
  for (int iparam = 0 ; iparam < fparm.Size(); iparam++) {
    PhysParamId_t p = fparm.WhichParam(iparam);
    if(!ParameterPool::Instance()->IsParamInPool(p)) {
      LOG("FitParams", pFATAL) << "The parameter \'" << util::NameRegistry::Instance()->ParamAsString(p)
                               << "\' was in the FPL but not the ParameterPool";
      util::Die(-1);
    }
    const Parameter & param = ParameterPool::Instance()->GetParam(p);

    double value = fparm.CurrValueByIndex(iparam);
    if (not param.IsWithinLimits(value)) {
      return false;
    }
  }
  return true;
}
//____________________________________________________________________________
bool util::AllWithinLimits(const valor::PhysParamList & pparm)
{
  for (int iparam = 0 ; iparam < conv::knppmax; iparam++) {
    PhysParamId_t p = PhysParamId_t(iparam);
    if (not pparm.Contains(p)) continue;
    if(!ParameterPool::Instance()->IsParamInPool(p)) {
      LOG("FitParams", pFATAL) << "The parameter \'" << util::NameRegistry::Instance()->ParamAsString(p)
                               << "\' was in the PhysParamList but not the ParameterPool";
      util::Die(-1);
    }
    const Parameter & param = ParameterPool::Instance()->GetParam(p);
    double value = pparm.Value(p);
    if (not param.IsWithinLimits(value)) {
      return false;
    }
  }
  return true;
}
//____________________________________________________________________________

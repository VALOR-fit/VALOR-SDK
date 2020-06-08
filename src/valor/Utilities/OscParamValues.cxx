//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//________________________________________________________________________________________

//fmw
#include "valor/Utilities/OscParamValues.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Conventions/NuOscMixParam.h"
#include "valor/Conventions/NuOscDmsqScheme.h"
#include "valor/Conventions/Opt.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlNumStat.h"
#include "valor/Utilities/NameRegistry.h"

//ROOT
#include <TMath.h>

using namespace valor;
using namespace valor::conv;

using std::string;

//________________________________________________________________________________________
double util::ConvertOscParamToUsedScheme(PhysParamId_t p, double val, std::string val_scheme_name, std::string target_scheme_name)
{
  double converted_val;
  val_scheme_name    = util::RemoveAllWhitespace(val_scheme_name);
  target_scheme_name = util::RemoveAllWhitespace(target_scheme_name);

  if(util::IsMixingAngle(p)) {
    NuOscMixParam_t input_scheme  = kNuMixUndefined;
    NuOscMixParam_t output_scheme = kNuMixUndefined;

    // If the scheme names for the input value or output value are empty, use the schemes defined in the global options
    if(!val_scheme_name.empty()) { input_scheme = util::MixingTypeFromString(val_scheme_name); }
    else { input_scheme = Opt::Instance()->gOscMixingParamChoice; }
    if(input_scheme == kNuMixUndefined) {
      LOG("Parameter", pERROR) << "Could not parse the input mixing scheme \'" << val_scheme_name << "\'";
      exit(1);
    }

    if(!target_scheme_name.empty()) { output_scheme = util::MixingTypeFromString(target_scheme_name); }
    else { output_scheme = Opt::Instance()->gOscMixingParamChoice; }
    if(output_scheme == kNuMixUndefined) {
      LOG("Parameter", pERROR) << "Could not parse the output mixing scheme \'" << val_scheme_name << "\'";
      exit(1);
    }

    // No need to convert the value if input scheme == output scheme
    if(input_scheme == output_scheme) { converted_val = val; }
    else {
      if     (input_scheme == kNuMixSinsqTheta  && output_scheme == kNuMixSinsq2Theta) { converted_val = util::SingleAngleToDoubleAngle(val); }
      else if(input_scheme == kNuMixSinsq2Theta && output_scheme == kNuMixSinsqTheta ) { converted_val = util::DoubleAngleToSingleAngle(val); }
      else {
        LOG("Parameter", pERROR) << "Unsupported mixing scheme conversion: " << val_scheme_name << " --> " << target_scheme_name;
        exit(1);
      }
    }
  }
  else if(util::IsMassSplitting(p)) {
    // The conversion between Dmsq schemes is applied when calculating oscillation probabilities
    // (see e.g. NuOsc3Flavour::SetOscParametersSinSqTAll), so there's no need to do anything here
    converted_val = val;
  }
  else if(util::IsPhase(p)) {
    NuOscDeltaCP_t input_scheme  = kNuDeltaCPUndefined;
    NuOscDeltaCP_t output_scheme = kNuDeltaCPUndefined;

    // If the scheme names for the input value or output value are empty, use the schemes defined in the global options
    if(!val_scheme_name.empty()) { input_scheme = util::CPVChoiceFromString(val_scheme_name); }
    else { input_scheme = Opt::Instance()->gOscDeltaCP; }
    if(input_scheme == kNuDeltaCPUndefined) {
      LOG("Parameter", pERROR) << "Could not parse the input CPV scheme \'" << val_scheme_name << "\'";
      exit(1);
    }

    if(!target_scheme_name.empty()) { output_scheme = util::CPVChoiceFromString(target_scheme_name); }
    else { output_scheme = Opt::Instance()->gOscDeltaCP; }
    if(output_scheme == kNuDeltaCPUndefined) {
      LOG("Parameter", pERROR) << "Could not parse the output CPV scheme \'" << val_scheme_name << "\'";
      exit(1);
    }

    // No need to convert the value if input scheme == output scheme
    if(input_scheme == output_scheme) { converted_val = val; }
    else {
      if     (input_scheme == kNuDeltaCPPhase && output_scheme == kNuDeltaCPSin  ) { converted_val = TMath::Sin(val);  }
      else if(input_scheme == kNuDeltaCPSin   && output_scheme == kNuDeltaCPPhase) { converted_val = TMath::ASin(val); }
      else {
        LOG("Parameter", pERROR) << "Unsupported CPV scheme conversion: " << val_scheme_name << " --> " << target_scheme_name;
        exit(1);
      }
    }
  }
  else {
    LOG("Parameter", pERROR) << "Could not parse osc param \'"
                             << util::NameRegistry::Instance()->ParamAsStringVerySimple(p) << "\'";
    exit(1);
  }

  return converted_val;
}
//________________________________________________________________________________________
double util::ConvertOscParamErrorToUsedScheme(conv::PhysParamId_t p, double central_value, double error, std::string val_scheme_name, std::string target_scheme_name)
{
  double err_up   = central_value + error;
  double err_down = central_value - error;
  double converted_err_up   = util::ConvertOscParamToUsedScheme(p, err_up  , val_scheme_name, target_scheme_name);
  double converted_err_down = util::ConvertOscParamToUsedScheme(p, err_down, val_scheme_name, target_scheme_name);
  return 0.5 * (converted_err_up - converted_err_down);
}
//________________________________________________________________________________________

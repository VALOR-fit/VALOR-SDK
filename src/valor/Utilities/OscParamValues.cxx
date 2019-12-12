//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

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
double util::OscParamInitValue(conv::PhysParamId_t p, std::string target_scheme_name)
{
  double val = -1;

  NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
  NuOscDmsqScheme_t  dmsqscheme = Opt::Instance()->GetEffectiveDmsqScheme();

  switch (p) 
  {
    case(kPhypMixing23):
      val = ( (omp==kNuMixSinsq2Theta) ? valor::conv::duneosc::kInitSinsq2Theta23 : valor::conv::duneosc::kInitSinsqTheta23); break;
    case(kPhypMixing13):
      val = ( (omp==kNuMixSinsq2Theta) ? valor::conv::duneosc::kInitSinsq2Theta13 : valor::conv::duneosc::kInitSinsqTheta13); break;
    case(kPhypMixing12):
      val = ( (omp==kNuMixSinsq2Theta) ? valor::conv::duneosc::kInitSinsq2Theta12 : valor::conv::duneosc::kInitSinsqTheta12); break;

    case(kPhypDm23sq):
      val = ( (dmsqscheme==kNuDmsqFogliLisi) ? valor::conv::duneosc::kInitDmFL : valor::conv::duneosc::kInitDm23sq); break;

    case(kPhypDm12sq):
      val = valor::conv::duneosc::kInitDm12sq; break;

    case (kPhypCPV):
      val = valor::conv::duneosc::kInitDeltaCP; break;
      
      
    case (kPhypDm41sq) :
      val = 0.; break;
    case (kPhypMixingMuE):
      val = 0.; break;
    case (kPhypMixingMuMu):
      val = 0.; break;
    case (kPhypMixingEE):
      val = 0.; break;
      
    default: 
      return -1; 

  }

  // The hard-coded values above all correspond the mixing/dmsq/phase schemes indicated below.
  // Convert the value to the target scheme (if needed) and return
  std::string val_scheme_name = "";
  if(util::IsPhase(p)) {
    val_scheme_name = util::CPVChoiceAsStringSimple(conv::kNuDeltaCPPhase);
    return util::ConvertOscParamToUsedScheme(p, val, val_scheme_name, target_scheme_name);
  }
  else if(util::IsMixingAngle(p) || util::IsMassSplitting(p)) {
    // Choice of scheme already handled above
    return val;
  }
  else {
    LOG("Parameter", pERROR) << "Could not parse osc param \'"
                             << util::NameRegistry::Instance()->ParamAsStringVerySimple(p) << "\'";
    exit(1);
  }

  return -1;
}
//________________________________________________________________________________________
double util::OscParamInitError(conv::PhysParamId_t p, std::string target_scheme_name)
{
  double val = -1;

  NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;

  switch (p) 
  {
    case(kPhypMixing23):
      val = ( (omp==kNuMixSinsq2Theta) ? valor::conv::duneosc::kInitErrSinsq2Theta23 : valor::conv::duneosc::kInitErrSinsqTheta23); break;
    case(kPhypMixing13):
      val = ( (omp==kNuMixSinsq2Theta) ? valor::conv::duneosc::kInitErrSinsq2Theta13 : valor::conv::duneosc::kInitErrSinsqTheta13); break;
    case(kPhypMixing12):
      val = ( (omp==kNuMixSinsq2Theta) ? valor::conv::duneosc::kInitErrSinsq2Theta12 : valor::conv::duneosc::kInitErrSinsqTheta12); break;

    case(kPhypDm23sq):
      val = valor::conv::duneosc::kInitErrDm23sq; break;

    case(kPhypDm12sq):
      val = valor::conv::duneosc::kInitErrDm12sq; break;

    case (kPhypCPV):
      val = valor::conv::duneosc::kInitErrDeltaCP; break;
      
    case (kPhypDm41sq) :
      val = 10.; break;
    case (kPhypMixingMuE):
      val = 1.; break;
    case (kPhypMixingMuMu):
      val = 1.; break;
    case (kPhypMixingEE):
      val = 1.; break;

    default: 
      return -1; 

  }

  // The hard-coded values above all correspond the mixing/dmsq/phase schemes indicated below.
  // Convert the value to the target scheme (if needed) and return
  std::string val_scheme_name = "";
  if(util::IsPhase(p)) {
    val_scheme_name = util::CPVChoiceAsStringSimple(conv::kNuDeltaCPPhase);
  }
  else if(util::IsMassSplitting(p)) {
    val_scheme_name = util::DmsqSchemeAsStringSimple(conv::kNuDmsq12andAbs23);
  }
  else if(util::IsMixingAngle(p)) {
    // Choice of scheme already handled above
    return val;
  }
  else {
    LOG("Parameter", pERROR) << "Could not parse osc param \'"
                             << util::NameRegistry::Instance()->ParamAsStringVerySimple(p) << "\'";
    exit(1);
  }

  double init_unconverted = util::OscParamInitValue(p, val_scheme_name);
  double init_converted   = util::OscParamInitValue(p, target_scheme_name);
  return (1./init_converted) * util::ConvertOscParamToUsedScheme(p, val * init_unconverted,
                                                                 val_scheme_name, target_scheme_name);
}
//________________________________________________________________________________________
double util::OscParamMaxValue(conv::PhysParamId_t p, std::string target_scheme_name)
{
  double val = -1;

  switch (p) 
  {
    case(kPhypMixing23):
      val = 1.0; break;
    case(kPhypMixing13):
      val = 1.0; break;
    case(kPhypMixing12):
      val = 1.0; break;

    case(kPhypDm23sq):
      val = valor::conv::duneosc::kMaxDm23sq; break;
    case(kPhypDm12sq):
      val = valor::conv::duneosc::kMaxDm12sq; break;

    case (kPhypCPV):
      val = 3.1416; break;
      
    case (kPhypDm41sq) :
      val = 1E10; break;
    case (kPhypMixingMuE):
      val = 1.; break;
    case (kPhypMixingMuMu):
      val = 1.; break;
    case (kPhypMixingEE):
      val = 1.; break;

    default: 
      return -1;
  }

  // The hard-coded values above all correspond the mixing/dmsq/phase schemes indicated below.
  // Convert the value to the target scheme (if needed) and return
  std::string val_scheme_name = "";
  if     (util::IsMixingAngle  (p)) { return val; }  // The physical max values of the mixing params are the same whether you use sinsq(theta) or sinsq(2*theta)
  else if(util::IsMassSplitting(p)) { val_scheme_name = util::DmsqSchemeAsStringSimple(conv::kNuDmsq12andAbs23); }
  else if(util::IsPhase        (p)) { val_scheme_name = util::CPVChoiceAsStringSimple (conv::kNuDeltaCPPhase  ); }
  else {
    LOG("Parameter", pERROR) << "Could not parse osc param \'"
                             << util::NameRegistry::Instance()->ParamAsStringVerySimple(p) << "\'";
    exit(1);
  }

  return util::ConvertOscParamToUsedScheme(p, val, val_scheme_name, target_scheme_name);
}
//________________________________________________________________________________________
double util::OscParamMinValue(conv::PhysParamId_t p, std::string target_scheme_name)
{
  double val = -1;

  switch (p) 
  {
    case(kPhypMixing23):
      val = 0.0; break;
    case(kPhypMixing13):
      val = 0.0; break;
    case(kPhypMixing12):
      val = 0.0; break;

    case(kPhypDm23sq):
      val = valor::conv::duneosc::kMinDm23sq; break;
    case(kPhypDm12sq):
      val = valor::conv::duneosc::kMinDm12sq; break;

    case (kPhypCPV):
      val = -3.1416; break;

    case (kPhypDm41sq) :
      val = 0; break;
    case (kPhypMixingMuE):
      val = 0.; break;
    case (kPhypMixingMuMu):
      val = 0.; break;
    case (kPhypMixingEE):
      val = 0.; break;

    default: 
      return -1; 
  }

  // The hard-coded values above all correspond the mixing/dmsq/phase schemes indicated below.
  // Convert the value to the target scheme (if needed) and return
  std::string val_scheme_name = "";
  if     (util::IsMixingAngle  (p)) { return val; }  // The physical min values of the mixing params are the same whether you use sinsq(theta) or sinsq(2*theta)
  else if(util::IsMassSplitting(p)) { val_scheme_name = util::DmsqSchemeAsStringSimple(conv::kNuDmsq12andAbs23); }
  else if(util::IsPhase        (p)) { val_scheme_name = util::CPVChoiceAsStringSimple (conv::kNuDeltaCPPhase  ); }
  else {
    LOG("Parameter", pERROR) << "Could not parse osc param \'"
                             << util::NameRegistry::Instance()->ParamAsStringVerySimple(p) << "\'";
    exit(1);
  }

  return util::ConvertOscParamToUsedScheme(p, val, val_scheme_name, target_scheme_name);
}
//________________________________________________________________________________________
double util::OscParamStepSize (conv::PhysParamId_t p, std::string target_scheme_name)
{
  double val = -1;

  NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
  switch (p) 
  {
    case(kPhypMixing23):
      val = ( (omp==kNuMixSinsq2Theta) ? valor::conv::duneosc::kStepSinsq2Theta23 : valor::conv::duneosc::kStepSinsqTheta23); break;
    case(kPhypMixing13):
      val = ( (omp==kNuMixSinsq2Theta) ? valor::conv::duneosc::kStepSinsq2Theta13 : valor::conv::duneosc::kStepSinsqTheta13); break;
    case(kPhypMixing12):
      val = ( (omp==kNuMixSinsq2Theta) ? valor::conv::duneosc::kStepSinsq2Theta12 : valor::conv::duneosc::kStepSinsqTheta12); break;

    case(kPhypDm23sq):
      val = valor::conv::duneosc::kStepDm23sq; break;

    case(kPhypDm12sq):
      val = valor::conv::duneosc::kStepDm12sq; break;

    case (kPhypCPV):
      val = valor::conv::duneosc::kStepDeltaCP; break;
      
    case (kPhypDm41sq) :
      val = 0.05; break;
    case (kPhypMixingMuE):
      val = 0.05; break;
    case (kPhypMixingMuMu):
      val = 0.05; break;
    case (kPhypMixingEE):
      val = 0.05; break;

    default:
      return -1;
  }

  // The hard-coded values above all correspond the mixing/dmsq/phase schemes indicated below.
  // Convert the value to the target scheme (if needed) and return
  std::string val_scheme_name = "";
  if(util::IsPhase(p)) {
    val_scheme_name = util::CPVChoiceAsStringSimple(conv::kNuDeltaCPPhase);
  }
  else if(util::IsMassSplitting(p)) {
    val_scheme_name = util::DmsqSchemeAsStringSimple(conv::kNuDmsq12andAbs23);
  }
  else if(util::IsMixingAngle(p)) {
    // Choice of scheme already handled above
    return val;
  }
  else {
    LOG("Parameter", pERROR) << "Could not parse osc param \'"
                             << util::NameRegistry::Instance()->ParamAsStringVerySimple(p) << "\'";
    exit(1);
  }

  double max_unconverted = util::OscParamMaxValue(p, val_scheme_name);
  double max_converted   = util::OscParamMaxValue(p, target_scheme_name);
  double min_unconverted = util::OscParamMinValue(p, val_scheme_name);
  double min_converted   = util::OscParamMinValue(p, target_scheme_name);

  // Keep the step size the same fraction of the range as it was before converting
  return val * (max_converted - min_converted) / (max_unconverted - min_unconverted);
}
//________________________________________________________________________________________
void util::InitializeOAPriors()
{
  NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
  NuOscDmsqScheme_t  dmsqscheme = Opt::Instance()->GetEffectiveDmsqScheme();
  
  for (int ip = 0 ; ip < conv::kNuis000 ; ip++) {
    PhysParamId_t p = (PhysParamId_t) ip;
    Opt::Instance()->SetGlobalBestFit (p,util::OscParamInitValue(p));
    Opt::Instance()->SetGlobalFitError(p,util::OscParamInitError(p));
  }
    
  Opt::Instance()->SetGlobalBestFit (kPhypDm12sq,valor::conv::duneosc::kInitDm12sq);
  Opt::Instance()->SetGlobalFitError(kPhypDm12sq,valor::conv::duneosc::kInitErrDm12sq);
  Opt::Instance()->SetGlobalBestFit (kPhypCPV,valor::conv::duneosc::kInitDeltaCP);
  Opt::Instance()->SetGlobalFitError(kPhypCPV,valor::conv::duneosc::kInitErrDeltaCP);
  if(omp==kNuMixSinsq2Theta){
    Opt::Instance()->SetGlobalBestFit (kPhypMixing23,valor::conv::duneosc::kInitSinsq2Theta23);
    Opt::Instance()->SetGlobalFitError(kPhypMixing23,valor::conv::duneosc::kInitErrSinsq2Theta23);

    Opt::Instance()->SetGlobalBestFit (kPhypMixing12,valor::conv::duneosc::kInitSinsq2Theta12);
    Opt::Instance()->SetGlobalFitError(kPhypMixing12,valor::conv::duneosc::kInitErrSinsq2Theta12);

    Opt::Instance()->SetGlobalBestFit (kPhypMixing13,valor::conv::duneosc::kInitSinsq2Theta13);
    Opt::Instance()->SetGlobalFitError(kPhypMixing13,valor::conv::duneosc::kInitErrSinsq2Theta13);
  }
  else{
    Opt::Instance()->SetGlobalBestFit (kPhypMixing23,valor::conv::duneosc::kInitSinsqTheta23);
    Opt::Instance()->SetGlobalFitError(kPhypMixing23,valor::conv::duneosc::kInitErrSinsqTheta23);

    Opt::Instance()->SetGlobalBestFit (kPhypMixing12,valor::conv::duneosc::kInitSinsqTheta12);
    Opt::Instance()->SetGlobalFitError(kPhypMixing12,valor::conv::duneosc::kInitErrSinsqTheta12);

    Opt::Instance()->SetGlobalBestFit (kPhypMixing13,valor::conv::duneosc::kInitSinsqTheta13);
    Opt::Instance()->SetGlobalFitError(kPhypMixing13,valor::conv::duneosc::kInitErrSinsqTheta13);
  }

  if(dmsqscheme==kNuDmsqFogliLisi)
    Opt::Instance()->SetGlobalBestFit (kPhypDm23sq,valor::conv::duneosc::kInitDmFL);
  else
    Opt::Instance()->SetGlobalBestFit (kPhypDm23sq,valor::conv::duneosc::kInitDm23sq);

  Opt::Instance()->SetGlobalFitError(kPhypDm23sq,valor::conv::duneosc::kInitErrDm23sq);

}
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

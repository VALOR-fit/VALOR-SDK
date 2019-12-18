//____________________________________________________________________________
/*!

\class     valor::conv::

\brief     Enumeration of default initial values for oscillation parameters

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           Lorena Escudero <escudero \at hep.phy.cam.ac.uk>
           University of Cambridge
          
\created   July, 2016

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_oscparams_h_
#define _valor_oscparams_h_

// fmwk
#include "valor/Conventions/PhysParamId.h"

namespace valor {
namespace conv {
namespace duneosc{
//
// Default values of oscillation parameters - updated from NuFit 2018, using 3sigma range/6 for relative error
// From normal
// WITHOUT SK ATMOSPHERIC
// http://www.nu-fit.org/?q=node/177

// Dm^2 global fit results                                               
  static const double kInitDm12sq    = 7.39E-5; // (NuFit 4.0 2018)
  static const double kMinDm12sq     = 1.0E-5;
  static const double kMaxDm12sq     = 15.0E-5;
  static const double kStepDm12sq    = 2.0E-6;
  static const double kInitErrDm12sq = 0.028; // 2.8% (NuFit 4.0 2018)
  
  static const double kInitSinsqTheta12        = 0.310; // (NuFit 4.0 2018)
  static const double kInitSinsq2Theta12       = 0.846; // (PDG 2015)
  static const double kInitErrSinsq2Theta12    = 0.025; // ~2.5% (PDG 2015)
  static const double kInitErrSinsqTheta12     = 0.040; // 4% (NuFit 4.0 2018)
  static const double kStepSinsqTheta12        = 1.0E-2;
  static const double kStepSinsq2Theta12       = 1.0E-2;


  static const double kInitDm23sq    = 2.525E-3; // (NuFit 4.0 2018)
  static const double kMinDm23sq     = 1.0E-3;
  static const double kMaxDm23sq     = 5.0E-3;
  static const double kInitErrDm23sq = 0.017; // ~1.3% (NuFit 4.0 2018)
  static const double kInitDmFL      = 2.48E-3; // (PDG 2015)  
  static const double kStepDm23sq    = 1.0E-4;

  static const double kInitSinsqTheta23     = 0.580; // (NuFit 4.0 2018)
  static const double kInitSinsq2Theta23    = 0.999; 
  static const double kInitErrSinsq2Theta23 = 0.018; // (PDG 2015, NH) 
  static const double kInitErrSinsqTheta23  = 0.060; // 6% (NuFit 4.0 2018)
  static const double kStepSinsqTheta23     = 5.0E-3;
  static const double kStepSinsq2Theta23    = 5.0E-3;

  static const double kInitSinsqTheta13     = 0.02241; // (NuFit 4.0 2018)
  static const double kInitSinsq2Theta13    = 0.085;  // (PDG 2015) 
  static const double kInitErrSinsqTheta13  = 0.029;  // 2.9% (NuFit 4.0 2018)
  static const double kInitErrSinsq2Theta13 = 0.059;  // (PDG 2015)
  static const double kStepSinsqTheta13     = 1.0E-3;
  static const double kStepSinsq2Theta13    = 1.0E-3;

  static const double kInitDeltaCP = -1.55; // T2K BF NH with reactor
  //  static const double kInitDeltaCP = 0.0;
    static const double kInitErrDeltaCP = 0.10; // ~10%  
  static const double kStepDeltaCP = 1.0E-2;

} // duneosc
} // conv

//
// Associated utilities
//
namespace util {

  double OscParamInitValue(conv::PhysParamId_t p, std::string scheme_name = "");
  double OscParamInitError(conv::PhysParamId_t p, std::string scheme_name = "");
  double OscParamMaxValue (conv::PhysParamId_t p, std::string scheme_name = "");
  double OscParamMinValue (conv::PhysParamId_t p, std::string scheme_name = "");
  double OscParamStepSize (conv::PhysParamId_t p, std::string scheme_name = "");

  // Given an osc param, p, and its value, val, which is given in a particular scheme: Convert the value from the
  // value's scheme to another scheme. If a scheme name is left blank, then the scheme defined in Opt::Instance is used.
  // e.g. Input a sinsq(2*theta) value and convert to a sinsq(theta) value.
  // NOTE: The conversion between Dmsq schemes is applied when calculating oscillation probabilities
  //       (see e.g. NuOsc3Flavour::SetOscParametersSinSqTAll), so this function does not convert Dmsq values.
  double ConvertOscParamToUsedScheme(conv::PhysParamId_t p, double val,
                                     std::string val_scheme_name = "", std::string target_scheme_name = "");

  double ConvertOscParamErrorToUsedScheme(conv::PhysParamId_t p, double central_value, double error,
                                     std::string val_scheme_name = "", std::string target_scheme_name = "");

} // util

} // valor
  
#endif 


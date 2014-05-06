//____________________________________________________________________________
/*!

\class     valor::conv::PhysParamId_t;

\brief     Conventions

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   Oct 20, 2010

\copyright (c) 2010-2014, The VALOR group

*/
//____________________________________________________________________________

#ifndef _phys_param_id_h_
#define _phys_param_id_h_

#include <string>

using std::string;

namespace valor {

namespace conv {

static const int kNumOfNuisanceParams = 0; 

// Max number of fit  parameters listed below (for booking large enough param arrays)
static const int knppmax = 100; 

typedef enum EPhysParamId 
{   
   kPhypUndefined = -1, 

   /*
    * List of oscillation parameters (various schemes)
    * ------------------------------------------------
   */

   kPhypMixing23,              // 23-mixing parameter (either sin^2(2*\theta_{23}) or sin^2(\theta_{23}) depending on config choices)
   kPhypMixing13,              // 13-mixing parameter (either sin^2(2*\theta_{13}) or sin^2(\theta_{13}) depending on config choices)
   kPhypMixing12,              // 12-mixing parameter (either sin^2(2*\theta_{12}) or sin^2(\theta_{12}) depending on config choices)
   kPhypCPV,                   // CP-invariance violating param (either deltacp, or sin(deltacp) depending on config choices)
   kPhypDm23sq,                // modulus of large squared-mass splitting (Dm^2_32, Dm^2_31 or Fogli-Lisi Dm^2 depending on config choices)
   kPhypDm12sq,                // small squared-mass splitting (Dm^2_21 or Fogli-Lisi dm^2 depending on config choices)
   kPhypAlphaDecay,            //
   kPhypSinsq2ThetaDecay,      // 
   kPhypMusqDecoh,             //
   kPhypSinsq2ThetaDecoh,      //
   kPhypMixing14,              // Sterile Mixings
   kPhypMixing15,              // Sterile Mixings
   kPhypMixing24,              // Sterile Mixings
   kPhypMixing25,              // Sterile Mixings
   kPhypMixing34,              // Sterile Mixings
   kPhypMixing35,              // Sterile Mixings
   kPhypDm41sq,                // Sterile mass splitting
   kPhypDm51sq,                // Sterile mass splitting
   kPhypDelta12,               // Sterile phase
   kPhypDelta13,               // Sterile phase - same as kPhypDeltaCP, but separate for constistency.
   kPhypDelta15,               // Sterile phase
   kPhypDelta34,               // Sterile phase
   kPhypDelta35,               // Sterile phase
   kPhypNSIepsMuTau,           // Nonstandard matter interactions

   /*
    * List of nuisance parameters 
    * -----------------------------------------
   */

   /*
    * (flux and cross-section) correlated parameters used in the BANFF fit
   */   
   kNuisBANFF00,               // 
   kNuisBANFF01,               // 
   kNuisBANFF02,               // 
   kNuisBANFF03,               // 
   kNuisBANFF04,               // 
   kNuisBANFF05,               // 
   kNuisBANFF06,               // 
   kNuisBANFF07,               // 
   kNuisBANFF08,               // 
   kNuisBANFF09,               // 
   kNuisBANFF10                // 
}
PhysParamId_t;

} // conv

namespace util {

  string AsString       (conv::PhysParamId_t p);
  string AsStringTex    (conv::PhysParamId_t p);
  string AsStringRooTex (conv::PhysParamId_t p);

  bool IsSystematic  (conv::PhysParamId_t p);
  bool IsMixingAngle (conv::PhysParamId_t p);


} // util

} // valor
  
#endif // _phys_param_id_h_


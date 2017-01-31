//____________________________________________________________________________
/*!

\class     valor::conv::KineVar_t

\brief     Kinematic variable enumeration

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   April 15, 2014

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_fmwk_kine_var_h_
#define _valor_fmwk_kine_var_h_

#include <TROOT.h>

#include <string>

using std::string;

namespace valor {
namespace conv {

typedef enum EKineVar
{
   kvUndefined = -1, 
   kvEtrue,           // true neutrino energy
   kvQ2true,          // true momentum transfer
   kvEreco,           // reconstructed neutrino energy
   kvEvis,            // visible neutrino energy
   kvYreco,           // reconstructed inelasticity (y)
   kvThetareco,       // reconstructed lepton angle
   kvq0reco,          // reconstructed 1st component of the 4-momentum    TODO: Isn't this just the same as Ereco?
   kvq3reco,          // reconstructed hadronic energy along the z-direction
   kvmodqreco		  // reconstructed magnitude of the 3-momentum
} 
KineVar_t; 

} // conv


//
// Associated utilities
//
namespace util {

  string  AsString     (conv::KineVar_t kv);
  string  AsStringTeX  (conv::KineVar_t kv, bool root);

  bool IsReco (conv::KineVar_t kv);
  bool IsTrue (conv::KineVar_t kv);

} // util

} // valor
  
#endif 


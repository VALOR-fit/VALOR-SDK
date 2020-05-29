//____________________________________________________________________________
/*!

\class     valor::conv::KineVar_t

\brief     Kinematic variable enumeration

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   April 15, 2014

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_fmwk_kine_var_h_
#define _valor_fmwk_kine_var_h_

#include <TROOT.h>

#include <string>
#include <utility>

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
   kvq0reco,          // reconstructed 1st component of the 4-momentum
   kvmodqreco         // reconstructed magnitude of the 3-momentum
} 
KineVar_t;

const int kNKineVarMax = conv::kvmodqreco+1;

} // conv


//
// Associated utilities
//
namespace util {

  string  AsString         (conv::KineVar_t kv);
  string  AsStringTeX      (conv::KineVar_t kv, bool root);
  std::string DSTBranchName(conv::KineVar_t kv);
  
  string  KineVarAsString     (conv::KineVar_t kv);
  string  KineVarAsStringTeX  (conv::KineVar_t kv, bool root);

  conv::KineVar_t KineVarFromString (std::string s);

  bool IsReco (conv::KineVar_t kv);
  bool IsTrue (conv::KineVar_t kv);
  // aliases
  bool KineVarIsReco (conv::KineVar_t kv);
  bool KineVarIsTrue (conv::KineVar_t kv);

  conv::KineVar_t KineVarFromInt(int k);

  std::pair<double,double> PhysicalLimits(conv::KineVar_t k);

} // util

} // valor
  
#endif 


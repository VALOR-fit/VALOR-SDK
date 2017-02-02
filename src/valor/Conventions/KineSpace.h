//____________________________________________________________________________
/*!

\class     valor::conv::KineSpace_t

\brief     Kinematic variable space enumeration (set of KineVar_t)

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   April 08, 2016

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_fmwk_kine_space_h_
#define _valor_fmwk_kine_space_h_

#include <string>
#include <vector>

#include <TROOT.h>

#include "valor/Conventions/KineVar.h"


namespace valor {
namespace conv {

static const int kMaxKineSpaces = 11;

typedef enum EKineSpace
{
   kspUndefined = -1, 
   kspEtrue,           // 1D: true neutrino energy
   kspEreco,           // 1D: reconstructed neutrino energy
   kspEvis,            // 1D: visible neutrino energy
   kspYreco,           // 1D: reconstructed inelasticity (y)
   kspThetareco,       // 1D: reconstructed lepton angle 
   kspErecoYreco,      // 2D: reconstructed energy, inelasticity
   kspEtrueQ2true,     // 2D: true neutrino energy, true momentum transfer
   kspErecoThetareco,  // 2D: reconstructed neutrino energy, reconstructed lepton angle
   kspq0recoq3reco,    // 2D: reconstructed 1st component of the 4-momentum, hadronic momentum along z-direction
   kspq0recomodqreco,  // 2D: reconstructed 1st component of the 4-momentum, reconstructed magnitude of the 3-momentum
   kspErecoEvis        // 2D: reconstructed neutrino energy, visible neutrino energy
   // ...
   // ...
   // ...
} 
KineSpace_t; 

} // conv


//
// Associated utilities
//
namespace util {

  std::string                         AsString            (conv::KineSpace_t ks);
  std::string                         AsStringTeX         (conv::KineSpace_t ks, bool root);
  bool                                IsPureReco          (conv::KineSpace_t ks);
  bool                                IsPureTrue          (conv::KineSpace_t ks);
  bool                                IsMixedRecoTrue     (conv::KineSpace_t ks);
  int                                 NDim                (conv::KineSpace_t ks);
  int                                 NDimReco            (conv::KineSpace_t ks);
  int                                 NDimTrue            (conv::KineSpace_t ks);
  conv::KineSpace_t                   SubSpaceReco        (conv::KineSpace_t ks);
  conv::KineSpace_t                   SubSpaceTrue        (conv::KineSpace_t ks);
  std::vector<valor::conv::KineVar_t> KineVar             (conv::KineSpace_t ks);
  std::vector<valor::conv::KineVar_t> KineVarReco         (conv::KineSpace_t ks);
  std::vector<valor::conv::KineVar_t> KineVarTrue         (conv::KineSpace_t ks);
  conv::KineSpace_t                   KineSpaceFromInt    (int is);
  conv::KineSpace_t                   KineSpaceFromString (string s);
  
  // Aliases to prevent overloading ambiguity
  string                  KineSpaceAsString    (conv::KineSpace_t ks);
  string                  KineSpaceAsStringTeX (conv::KineSpace_t ks, bool root);


} // util

} // valor
  
#endif 


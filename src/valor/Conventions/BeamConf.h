//____________________________________________________________________________
/*!

\class     valor::conv::BeamConf_t

\brief     Beam configuration enumeration

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   April 15, 2014

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_dune_beam_configuration_h_
#define _valor_dune_beam_configuration_h_

#include <TROOT.h>

#include <string>

#include "valor/Conventions/Build.h"

using std::string;

namespace valor {
namespace conv {

typedef enum EBeamConf
{
   kbUndefined = -1, 
   //
   // "Generic" FHC and RHC beam configurations
   // More detailed descriptions can be added if multiple FHC or RHC
   // beam configurations for the same facility need to used concurrently
   //
   kbLBNF_FHC,             // forward horn current beam @ FNAL/LBNF
   kbLBNF_RHC,             // reverse horn current beam @ FNAL/LBNF
   kbJPARC_FHC,            // forward horn current beam @ JPARC
   kbJPARC_RHC,            // reverse horn current beam @ JPARC 
   kbNuMI_FHC,             // forward horn current beam @ FNAL/NuMI 
   kbNuMI_RHC,             // reverse horn current beam @ FNAL/NuMI 
   kbBNB_FHC,              // forward horn current beam @ FNAL/Booster
   kbBNB_RHC               // reverse horn current beam @ FNAL/Booster
} 
BeamConf_t;

static const int kNBeamsDefined = 8;

} // conv

//
// Associated utilities
//
namespace util {

  string           AsString           (conv::BeamConf_t b, Option_t * opt=0);
  conv::BeamConf_t BeamConfFromInt    (int b);
  conv::BeamConf_t BeamConfFromString (std::string b);
  
  // Alias for above
  string BeamConfAsString(conv::BeamConf_t b, Option_t * opt=0);

} // util

} // valor
  
#endif 


//____________________________________________________________________________
/*!

\class     valor::conv

\brief     Physics hypotheses

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   Oct 20, 2010

\copyright (c) 2010-2014, The VALOR group

*/
//____________________________________________________________________________

#ifndef physics_hypotheses_h
#define physics_hypotheses_h

#include <string>

using std::string;

namespace valor {

namespace conv {

typedef enum EPhysHypothesis 
{   
   kHypUndefined = -1,
   kHypDecay,             // neutrino decay 
   kHypDecoh,             // neutrino decoherence 
   kHypNuMuDisapp2F,      // muon-neutrino disappearance, simple 2-flavour oscillation framework 
   kHypOsc3F,             // neutrino oscillations, 3-flavour framework
   kHypOscSterile4F,      // neutrino oscillations with steriles, 3+1
   kHypOscSterile5F,      // neutrino oscillations with steriles, 3+2
   kHypNSI3F,             // non-standard matter interactions
   kHypVoid               // nothing - used for ND systematic constraint fits
} 
PhysHypothesis_t;
   
} // conv

namespace util {

  string AsString (conv::PhysHypothesis_t h);
  string PhysHypAsString (conv::PhysHypothesis_t h); //alias
  string AsString (conv::PhysHypothesis_t hyp, double mix12, double mix13, double mix23, double deltacp, double dm12sq, double dm23sq);

  conv::PhysHypothesis_t PhysHypFromString (string h);

} // util

} // valor
  
#endif 


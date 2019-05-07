//____________________________________________________________________________
/*!

\class     valor::conv::NuFlavour_t

\brief     Specify the neutrino flavours

\author    Steve Dennis, <s.r.dennis \at liverpool.ac.uk
          
\created   Mar 24, 2016

\copyright (c) 2010-2016, The VALOR group

*/
//____________________________________________________________________________

#ifndef nuflav_h
#define nuflav_h

#include <string>

namespace valor {

namespace conv {

typedef enum ENuFlavour
{   
   kNuFlavourUndefined = -1,
   kNuE,
   kNuMu,
   kNuTau,
   kNuEBar,
   kNuMuBar,
   kNuTauBar
} 
NuFlavour_t;

static const int kNNuFlavour = 6;

} // conv

namespace util {

  std::string AsString(conv::NuFlavour_t p);
  std::string AsStringTeX(conv::NuFlavour_t p);
  std::string FlavourAsString(conv::NuFlavour_t p); //alias for python
  std::string FlavourAsStringTeX(conv::NuFlavour_t p); //alias for python

  conv::NuFlavour_t FlavourFromString(std::string h);
  conv::NuFlavour_t FlavourFromPDG(int nu_pdg);

} // util

} // valor
  
#endif 


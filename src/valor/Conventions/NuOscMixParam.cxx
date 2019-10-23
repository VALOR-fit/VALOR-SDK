//____________________________________________________________________________
/*

\copyright (c) 2010-2013, VaLOR group.

*/
//____________________________________________________________________________

#include "valor/Conventions/NuOscMixParam.h"

using namespace valor;
using namespace valor::conv;

//____________________________________________________________________________
string util::AsString(conv::NuOscMixParam_t p)
{
  switch(p) {
   case (kNuMixSinsq2Theta) :
     return "sin^2(2*theta_{ij})";
     break;
   case (kNuMixSinsqTheta) :
     return "sin^2(theta_{ij})";
     break;
   default :
     return "Undefined";
     break;
  }
  return "";
}
//____________________________________________________________________________
string util::MixingTypeAsString(conv::NuOscMixParam_t p) { return AsString(p);}
//____________________________________________________________________________
NuOscMixParam_t util::MixingTypeFromString(string h)
{
  if(h == "sinsq2theta") return kNuMixSinsq2Theta;
  if(h == "sinsqtheta" ) return kNuMixSinsqTheta;

  return kNuMixUndefined;
}
//____________________________________________________________________________


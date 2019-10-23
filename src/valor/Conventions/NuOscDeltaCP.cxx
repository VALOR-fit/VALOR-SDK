//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include "valor/Conventions/NuOscDeltaCP.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
string util::AsString(NuOscDeltaCP_t d)
{
  switch(d) {
  case(kNuDeltaCPPhase) :
    return "d_{CP}";
    break;
  case(kNuDeltaCPSin) :
    return "sin(d_{CP})";
    break;
  default :
    return "Undefined";
    break;
  }
}
//________________________________________________________________________________________
string util::CPVChoiceAsString(NuOscDeltaCP_t d) { return AsString(d); }
//________________________________________________________________________________________
string util::CPVChoiceAsStringSimple(NuOscDeltaCP_t d)
{
  switch(d) {
    case(kNuDeltaCPPhase) :
      return "delta";
      break;
    case(kNuDeltaCPSin) :
      return "sindelta";
      break;
    default :
      return "Undefined";
      break;
  }
}
//________________________________________________________________________________________
NuOscDeltaCP_t util::CPVChoiceFromString (string d)
{
  if(d == "sindelta" ) return kNuDeltaCPSin;
  if(d == "delta"    ) return kNuDeltaCPPhase;

  return kNuDeltaCPUndefined;
}
//________________________________________________________________________________________


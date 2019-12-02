//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include "valor/Conventions/NuOscOctant.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
std::string util::AsString(NuOscOctant_t o)
{
  switch(o) {
   case (kNuOctantOne) :
     return "sin^2(theta_{23}) <= 0.5";
     break;
   case (kNuOctantTwo) :
     return "sin^2(theta_{23}) >= 0.5";
     break;
   default :
     return "Undefined";
     break;
  }
  return "";
}
//________________________________________________________________________________________
std::string util::OctantAsString(NuOscOctant_t o) { return AsString(o);}
//________________________________________________________________________________________
NuOscOctant_t util::OctantFromString(std::string h)
{
  if(h == "one") return kNuOctantOne;
  if(h == "two") return kNuOctantTwo;

  return kNuOctantUndefined;
}
//________________________________________________________________________________________


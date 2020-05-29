//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

ChangeLog:

*/
//________________________________________________________________________________________

#include "valor/Conventions/NuMassHierarchy.h"
#include "valor/Utilities/UtlStr.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
string util::AsString(NuMassHierarchy_t m)
{
  switch(m) {
   case (kNmhNormal) :
     return "Normal";
     break;
   case (kNmhInverted) :
     return "Inverted";
     break;
   default :
     return "Undefined"; 
     break;
  }
  return "";
}
//________________________________________________________________________________________
string util::HierarchyAsString(NuMassHierarchy_t m) { return AsString(m); }
//________________________________________________________________________________________
NuMassHierarchy_t util::HierarchyFromString(string h)
{
  string lh = util::ToLowerCase(h);
  if(lh == "normal"   ) return kNmhNormal;
  if(lh == "inverted" ) return kNmhInverted;

  return kNmhUndefined;
}
//________________________________________________________________________________________


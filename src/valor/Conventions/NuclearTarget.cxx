//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

ChangeLog:

*/
//________________________________________________________________________________________

#include <algorithm>

//fmwk
#include "valor/Utilities/UtlStr.h"
#include "valor/Messenger/Messenger.h"
//dune
#include "valor/Conventions/NuclearTarget.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
string util::AsString(NuclearTarget_t nt)
{
  string name = "";
  switch(nt) {
    case (kntInclusive) : name = "Inclusive"; break;
    case (kntHydrogen ) : name = "Hydrogen";  break;
    case (kntCarbon   ) : name = "Carbon";    break;
    case (kntOxygen   ) : name = "Oxygen";    break;
    case (kntArgon    ) : name = "Argon";     break;
    case (kntCalcium  ) : name = "Calcium";   break;
    case (kntOther    ) : name = "Other";     break;      
    default : 
     VLOG("NuclearTarget",pERROR) 
        << "Unrecognized nuclear target " << nt;
  }
  return name;
}
//________________________________________________________________________________________
string util::AsStringTeX(NuclearTarget_t nt, bool root)
{
  string name = "";
  switch(nt) { // Maybe change these to the isotope notation
    case (kntInclusive) : name = "Inclusive"; break;
    case (kntHydrogen ) : name = "Hydrogen";  break;
    case (kntCarbon   ) : name = "Carbon";    break;
    case (kntOxygen   ) : name = "Oxygen";    break;
    case (kntArgon    ) : name = "Argon";     break;
    case (kntCalcium  ) : name = "Calcium";   break;
    case (kntOther    ) : name = "Other";     break;      
    default : 
     VLOG("NuclearTarget",pERROR) 
        << "Unrecognized nuclear target " << nt;
  }

  if(root) {
    replace( name.begin(), name.end(), '\\', '#' ); // Make slashes hashes for RooTeX
    // Get rid of hashes preceding sin
    size_t ind = name.find("#sin",0);
    if (ind !=  string::npos) (name.replace(ind,4," sin"));
    return util::TrimSpaces(name);
  }

  return name;
}
//________________________________________________________________________________________
NuclearTarget_t util::NuclearTargetFromString(string inp_name)
{
  for(int it = 0; it < kNNuclearTarget; it++) {
     NuclearTarget_t nuc = (NuclearTarget_t)(it);
     string name = util::AsString(nuc);
     if(name == inp_name) {
       return nuc;
     }
  }
  return kntUndefined;
}
//________________________________________________________________________________________

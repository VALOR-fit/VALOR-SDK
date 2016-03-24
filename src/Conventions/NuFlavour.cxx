//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include "Conventions/NuFlavour.h"

#include <algorithm>

using namespace valor;
using namespace valor::conv;

using std::string;

//________________________________________________________________________________________
string util::AsString(NuFlavour_t f)
{
  switch(f) {
   case (kNuE     ) : return "NuE" ;
   case (kNuMu    ) : return "NuMu";
   case (kNuTau   ) : return "NuTau";
   case (kNuEBar  ) : return "NuEBar" ;
   case (kNuMuBar ) : return "NuMuBar";
   case (kNuTauBar) : return "NuTauBar";
   default          : return "Undefined";
  }
}
//________________________________________________________________________________________
string util::FlavourAsString(NuFlavour_t f) { return AsString(f);}
//________________________________________________________________________________________
NuFlavour_t util::FlavourFromString(string flavname)
{
  std::transform(flavname.begin(), flavname.end(), flavname.begin(), ::tolower);
  for (int i = 0 ; i < kNNuFlavour ; i++ ) {
    NuFlavour_t f = (NuFlavour_t)i;
    string flavname2 = util::FlavourAsString(f);
    std::transform(flavname2.begin(), flavname2.end(), flavname2.begin(), ::tolower);
    if(flavname==flavname2 && flavname2!="undefined") {
      return f;
    }
  }
    
  return kNuFlavourUndefined;
}
//________________________________________________________________________________________


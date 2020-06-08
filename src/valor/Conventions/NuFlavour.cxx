//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

ChangeLog:

*/
//________________________________________________________________________________________

#include "valor/Conventions/NuFlavour.h"
#include "valor/Conventions/PDG.h"

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
string util::AsStringTeX(NuFlavour_t f)
{
  switch(f) {
   case (kNuE     ) : return "\\nu_{e}" ;
   case (kNuMu    ) : return "\\nu_{\\mu}";
   case (kNuTau   ) : return "\\nu_{\\tau}";
   case (kNuEBar  ) : return "\\bar{\\nu}_{e}" ;
   case (kNuMuBar ) : return "\\bar{\\nu}_{\\mu}";
   case (kNuTauBar) : return "\\bar{\\nu}_{\\tau}";
   default          : return "Undefined";
  }
}
//________________________________________________________________________________________
string util::FlavourAsString   (NuFlavour_t f) { return AsString(f);}
string util::FlavourAsStringTeX(NuFlavour_t f) { return AsStringTeX(f);}
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
NuFlavour_t util::FlavourFromPDG(int nu_pdg)
{
  switch (nu_pdg) {
   case (conv::kPdgNuE     ) : return kNuE     ;
   case (conv::kPdgNuMu    ) : return kNuMu    ;
   case (conv::kPdgNuTau   ) : return kNuTau   ;
   case (conv::kPdgNuEBar  ) : return kNuEBar  ;
   case (conv::kPdgNuMuBar ) : return kNuMuBar ;
   case (conv::kPdgNuTauBar) : return kNuTauBar;
   
   default : return kNuFlavourUndefined;
  }
  return kNuFlavourUndefined;
}
//________________________________________________________________________________________


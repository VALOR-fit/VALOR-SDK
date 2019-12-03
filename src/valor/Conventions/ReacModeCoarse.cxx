//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include "dune/Conventions/ReacModeCoarse.h"
#include "valor/Utilities/UtlStr.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
string util::AsString(ReacModeCoarse_t m)
{
  switch(m) {
    case (kmcNuMuCC         ) : return "numuCC";          break;
    case (kmcNuMuCCQEMEC    ) : return "numuCCQEMEC";     break;
    case (kmcNuMuCCQE       ) : return "numuCCQE";        break;
    case (kmcNuMuCCMEC      ) : return "numuCCMEC";       break;
    case (kmcNuMuCC1piC     ) : return "numuCC1piC";      break;
    case (kmcNuMuCC1pi0     ) : return "numuCC1pi0";      break;
    case (kmcNuMuCC1pi      ) : return "numuCC1pi";       break;
    case (kmcNuMuCC2pi      ) : return "numuCC2pi";       break;
    case (kmcNuMuCCIMD      ) : return "numuCCIMD";       break;
    case (kmcNuMuCCElScat   ) : return "numuCCElScat";    break;
    case (kmcNuMuCCoth      ) : return "numuCCoth";       break;
    case (kmcNuMuNC         ) : return "numuNC";          break;
    case (kmcNuMuBarCC      ) : return "numubarCC";       break;
    case (kmcNuMuBarCCQEMEC ) : return "numubarCCQEMEC";  break;
    case (kmcNuMuBarCCQE    ) : return "numubarCCQE";     break;
    case (kmcNuMuBarCCMEC   ) : return "numubarCCMEC";    break;
    case (kmcNuMuBarCC1piC  ) : return "numubarCC1piC";   break;
    case (kmcNuMuBarCC1pi0  ) : return "numubarCC1pi0";   break;
    case (kmcNuMuBarCC1pi   ) : return "numubarCC1pi";    break;
    case (kmcNuMuBarCC2pi   ) : return "numubarCC2pi";    break;
    case (kmcNuMuBarCCIMD   ) : return "numubarIMD";      break;
    case (kmcNuMuBarCCElScat) : return "numubarElScat";   break;
    case (kmcNuMuBarCCoth   ) : return "numubarCCoth";    break;
    case (kmcNuMuBarNC      ) : return "numubarNC";       break;
    case (kmcNuENuEBarCC    ) : return "nuenuebarCC";     break;
    case (kmcNuECC          ) : return "nueCC";           break;
    case (kmcNuECCQEMEC     ) : return "nueCCQEMEC";      break;
    case (kmcNuECCQE        ) : return "nueCCQE";         break;
    case (kmcNuECCMEC       ) : return "nueCCMEC";        break;
    case (kmcNuECC1pi       ) : return "nueCC1pi";        break;
    case (kmcNuECC2pi       ) : return "nueCC2pi";        break;
    case (kmcNuECCoth       ) : return "nueCCoth";        break;
    case (kmcNuEElScat      ) : return "nueElScat";       break;
    case (kmcNuENC          ) : return "nueNC";           break;
    case (kmcNuEBarCC       ) : return "nuebarCC";        break;
    case (kmcNuEBarCCQEMEC  ) : return "nuebarCCQEMEC";   break;
    case (kmcNuEBarCCQE     ) : return "nuebarCCQE";      break;
    case (kmcNuEBarCCMEC    ) : return "nuebarCCMEC";     break;
    case (kmcNuEBarCC1pi    ) : return "nuebarCC1pi";     break;
    case (kmcNuEBarCC2pi    ) : return "nuebarCC2pi";     break;
    case (kmcNuEBarCCoth    ) : return "nuebarCCoth";     break;
    case (kmcNuEBarElScat   ) : return "nuebarElScat";    break;
    case (kmcNuEBarNC       ) : return "nuebarNC";        break;
    case (kmcNuTauNuTauBarCC) : return "nutaunutaubarCC"; break;
    case (kmcNC             ) : return "NC";              break;
    case (kmcNC1piC         ) : return "NC1piC";          break;
    case (kmcNC1pi0         ) : return "NC1pi0";          break;
    case (kmcNCoth          ) : return "NCoth";           break;
    case (kmcOscNuMuCC      ) : return "oscnumuCC";       break;
    case (kmcOscNuMuBarCC   ) : return "oscnumubarCC";    break;
    case (kmcOscNuECC       ) : return "oscnueCC";        break;
    case (kmcUnOscNuECC     ) : return "unoscnueCC";      break;
    case (kmcOscNuEBarCC    ) : return "oscnuebarCC";     break;
    case (kmcUnOscNuEBarCC  ) : return "unoscnuebarCC";   break;
    case (kmcNuTauCC        ) : return "nutauCC";         break;
    case (kmcNuTauBarCC     ) : return "nutaubarCC";      break;
    default                   : return "--";           
  }
  return "";
}
//________________________________________________________________________________________
string util::AsStringTex(ReacModeCoarse_t m, bool root)
{
  string name = "";

  switch(m) { 
    case (kmcNuMuCC         ) : name = "#nu_{#mu} CC";                break;
    case (kmcNuMuCCQEMEC    ) : name = "#nu_{#mu} CC QE+MEC";         break;
    case (kmcNuMuCCQE       ) : name = "#nu_{#mu} CC QE";             break;
    case (kmcNuMuCCMEC      ) : name = "#nu_{#mu} CC MEC";            break;
    case (kmcNuMuCC1pi      ) : name = "#nu_{#mu} CC 1#pi";           break;
    case (kmcNuMuCC2pi      ) : name = "#nu_{#mu} CC 2#pi";           break;
    case (kmcNuMuCCoth      ) : name = "#nu_{#mu} CC other";          break;
    case (kmcNuMuCCElScat   ) : name = "#nu_{#mu} CC elastic";        break;
    case (kmcNuMuNC         ) : name = "#nu_{#mu} NC";                break;
    case (kmcNuMuBarCC      ) : name = "#bar{#nu}_{#mu} CC";          break;
    case (kmcNuMuBarCCQEMEC ) : name = "#bar{#nu}_{#mu} CC QE+MEC";   break;
    case (kmcNuMuBarCCQE    ) : name = "#bar{#nu}_{#mu} CC QE";       break;
    case (kmcNuMuBarCCMEC   ) : name = "#bar{#nu}_{#mu} CC MEC";      break;
    case (kmcNuMuBarCC1pi   ) : name = "#bar{#nu}_{#mu} CC 1#pi";     break;
    case (kmcNuMuBarCC2pi   ) : name = "#bar{#nu}_{#mu} CC 2#pi";     break;
    case (kmcNuMuBarCCoth   ) : name = "#bar{#nu}_{#mu} CC other";    break;
    case (kmcNuMuBarCCElScat) : name = "#bar{#nu}_{#mu} CC elastic";  break;
    case (kmcNuMuBarNC      ) : name = "#bar{#nu}_{#mu} NC";          break;
    case (kmcNuENuEBarCC    ) : name = "#nu_{e}/#bar{#nu}_{e} CC";    break;
    case (kmcNuECC          ) : name = "#nu_{e} CC";                  break;
    case (kmcNuECCQEMEC     ) : name = "#nu_{e} CC QE+MEC";           break;
    case (kmcNuECCQE        ) : name = "#nu_{e} CC QE";               break;
    case (kmcNuECCMEC       ) : name = "#nu_{e} CC MEC";              break;
    case (kmcNuECC1pi       ) : name = "#nu_{e} CC 1#pi";             break;
    case (kmcNuECC2pi       ) : name = "#nu_{e} CC 2#pi";             break;
    case (kmcNuECCoth       ) : name = "#nu_{e} CC other";            break;
    case (kmcNuEElScat      ) : name = "#nu_{e}-e elastic";           break;
    case (kmcNuENC          ) : name = "#nu_{e} NC";                  break;
    case (kmcNuEBarCC       ) : name = "#bar{#nu}_{e} CC";            break;
    case (kmcNuEBarCCQEMEC  ) : name = "#bar{#nu}_{e} CC QE+MEC";     break;
    case (kmcNuEBarCCQE     ) : name = "#bar{#nu}_{e} CC QE";         break;
    case (kmcNuEBarCCMEC    ) : name = "#bar{#nu}_{e} CC MEC";        break;
    case (kmcNuEBarCC1pi    ) : name = "#bar{#nu}_{e} CC 1#pi";       break;
    case (kmcNuEBarCC2pi    ) : name = "#bar{#nu}_{e} CC 2#pi";       break;
    case (kmcNuEBarCCoth    ) : name = "#bar{#nu}_{e} CC other";      break;
    case (kmcNuEBarElScat   ) : name = "#bar{nu}_{e}-e elastic";      break;
    case (kmcNuEBarNC       ) : name = "#bar{#nu}_{e} NC";            break;
    case (kmcNuTauNuTauBarCC) : name = "#nu_{#tau}/#bar{#nu}_{#tau} CC";  break;
    case (kmcNC             ) : name = "NC";                          break;
    case (kmcNC1piC         ) : name = "NC 1#pi^{#pm}";               break;
    case (kmcNC1pi0         ) : name = "NC 1#pi^{0}";                 break;
    case (kmcNCoth          ) : name = "NC other";                    break;
    case (kmcOscNuMuCC      ) : name = "osc #nu_{#mu} CC";            break;
    case (kmcOscNuMuBarCC   ) : name = "osc #bar{#nu}_{#mu} CC";      break;
    case (kmcOscNuECC       ) : name = "osc #nu_{e} CC";              break;
    case (kmcUnOscNuECC     ) : name = "unosc #nu_{e} CC";            break;
    case (kmcOscNuEBarCC    ) : name = "osc #bar{#nu}_{e} CC";        break;
    case (kmcUnOscNuEBarCC  ) : name = "unosc #bar{#nu}_{e} CC";      break;
    case (kmcNuTauCC        ) : name = "#nu_{#tau} CC";               break;
    case (kmcNuTauBarCC     ) : name = "#bar{#nu}_{#tau} CC";         break;
    default                   : name = "--"; 
  }

  if(root) return name;

  // change `#' with `\' (later)

  return name;
}
//________________________________________________________________________________________
conv::ReacModeCoarse_t util::CoarseModeFromString(string coarse)
{
  string lc_coarse = util::ToLowerCase(coarse);
  for (int i = 0 ; i < (int)conv::kmcNuTauBarCC ; i++ ) {
    conv::ReacModeCoarse_t c = (conv::ReacModeCoarse_t)i;
    string this_coarse = util::ToLowerCase(util::AsString(c));
    if(lc_coarse==this_coarse && this_coarse!="--") {
      return c;
    }
  }

  return conv::kmcUndefined;
}
//________________________________________________________________________________________
bool util::ModeWithinCoarse(conv::ReacMode_t m, conv::ReacModeCoarse_t c)
{
  for (int icoarse = 0 ; icoarse < conv::kMaxCoarseReacOptions ; icoarse++ ) {
    conv::ReacModeCoarse_t coarse = conv::kCoarseReacPriority[(int)m][icoarse];
    if(c == coarse) {
      return true;
    }
  }
  return false;
}
//________________________________________________________________________________________

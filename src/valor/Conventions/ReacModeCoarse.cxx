//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

ChangeLog:

*/
//________________________________________________________________________________________

#include "valor/Conventions/ReacModeCoarse.h"
#include "valor/Utilities/UtlStr.h"

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
string util::AsString(ReacModeCoarse_t m)
{
  switch(m) {
  case (kmcNuMuCCQE    ) : return "numuccqe";         break;
  case (kmcNuMuCCRES   ) : return "numuccres";        break;
  case (kmcNuMuCCDIS   ) : return "numuccdis";        break;
  case (kmcNuMuCCCOH   ) : return "numucccoh";        break;
  case (kmcNuMuCCMEC   ) : return "numuccmec";        break;
  case (kmcNuMuNCQE    ) : return "numuncqe";         break;
  case (kmcNuMuNCRES   ) : return "numuncres";        break;
  case (kmcNuMuNCDIS   ) : return "numuncdis";        break;
  case (kmcNuMuNCCOH   ) : return "numunccoh";        break;
  case (kmcNuMuNCMEC   ) : return "numuncmec";        break;
  case (kmcNuMuBarCCQE ) : return "numubarccqe";      break;
  case (kmcNuMuBarCCRES) : return "numubarccres";     break;
  case (kmcNuMuBarCCDIS) : return "numubarccdis";     break;
  case (kmcNuMuBarCCCOH) : return "numubarcccoh";     break;
  case (kmcNuMuBarCCMEC) : return "numubarccmec";     break;
  case (kmcNuMuBarNCQE ) : return "numubarncqe";      break;
  case (kmcNuMuBarNCRES) : return "numubarncres";     break;
  case (kmcNuMuBarNCDIS) : return "numubarncdis";     break;
  case (kmcNuMuBarNCCOH) : return "numubarnccoh";     break;
  case (kmcNuMuBarNCMEC) : return "numubarncmec";     break;
  case (kmcNuECCQE     ) : return "nueccqe";          break;
  case (kmcNuECCRES    ) : return "nueccres";         break;
  case (kmcNuECCDIS    ) : return "nueccdis";         break;
  case (kmcNuECCCOH    ) : return "nuecccoh";         break;
  case (kmcNuECCMEC    ) : return "nueccmec";         break;
  case (kmcNuENCQE     ) : return "nuencqe";          break;
  case (kmcNuENCRES    ) : return "nuencres";         break;
  case (kmcNuENCDIS    ) : return "nuencdis";         break;
  case (kmcNuENCCOH    ) : return "nuenccoh";         break;
  case (kmcNuENCMEC    ) : return "nuencmec";         break;
  case (kmcNuEBarCCQE  ) : return "nuebarccqe";       break;
  case (kmcNuEBarCCRES ) : return "nuebarccres";      break;
  case (kmcNuEBarCCDIS ) : return "nuebarccdis";      break;
  case (kmcNuEBarCCCOH ) : return "nuebarcccoh";      break;
  case (kmcNuEBarCCMEC ) : return "nuebarccmec";      break;
  case (kmcNuEBarNCQE  ) : return "nuebarncqe";       break;
  case (kmcNuEBarNCRES ) : return "nuebarncres";      break;
  case (kmcNuEBarNCDIS ) : return "nuebarncdis";      break;
  case (kmcNuEBarNCCOH ) : return "nuebarnccoh";      break;
  case (kmcNuEBarNCMEC ) : return "nuebarncmec";      break;
  case (kmcCohElastic  ) : return "cohelastic";       break;
  case (kmcElScat      ) : return "electronscatter";  break;
  case (kmcIMDAnh      ) : return "imd-annihilation"; break;
  case (kmcIBD         ) : return "ibd";              break;
  case (kmcGlashow     ) : return "glashow";          break;
  case (kmcAMNuGamma   ) : return "anomalynugamma";   break;
  case (kmcDiffractive ) : return "diffractive";      break;
  case (kmcEM          ) : return "em";               break;
  case (kmcWeakMix     ) : return "weakmix";          break;
  case (kmcCosmic      ) : return "cosmic";           break;
  case (kmcDirt        ) : return "dirt";             break;
  default                : return "--";           
  }
  return "";
}
//________________________________________________________________________________________
string util::CoarseModeAsString(conv::ReacModeCoarse_t m)
{
  return util::AsString(m);
}
//________________________________________________________________________________________
string util::AsStringTex(ReacModeCoarse_t m, bool root)
{
  string name = "";

  switch(m) { 
  case (kmcNuMuCCQE    ) : return "#nu_{#mu} CC QE";         break;
  case (kmcNuMuCCRES   ) : return "#nu_{#mu} CC RES";        break;
  case (kmcNuMuCCDIS   ) : return "#nu_{#mu} CC DIS";        break;
  case (kmcNuMuCCCOH   ) : return "#nu_{#mu} CC COH";        break;
  case (kmcNuMuCCMEC   ) : return "#nu_{#mu} CC MEC";        break;
  case (kmcNuMuNCQE    ) : return "#nu_{#mu} NC QE";         break;
  case (kmcNuMuNCRES   ) : return "#nu_{#mu} NC RES";        break;
  case (kmcNuMuNCDIS   ) : return "#nu_{#mu} NC DIS";        break;
  case (kmcNuMuNCCOH   ) : return "#nu_{#mu} NC COH";        break;
  case (kmcNuMuNCMEC   ) : return "#nu_{#mu} NC MEC";        break;
  case (kmcNuMuBarCCQE ) : return "#bar{#nu}_{#mu} CC QE";      break;
  case (kmcNuMuBarCCRES) : return "#bar{#nu}_{#mu} CC RES";     break;
  case (kmcNuMuBarCCDIS) : return "#bar{#nu}_{#mu} CC DIS";     break;
  case (kmcNuMuBarCCCOH) : return "#bar{#nu}_{#mu} CC COH";     break;
  case (kmcNuMuBarCCMEC) : return "#bar{#nu}_{#mu} CC MEC";     break;
  case (kmcNuMuBarNCQE ) : return "#bar{#nu}_{#mu} NC QE";      break;
  case (kmcNuMuBarNCRES) : return "#bar{#nu}_{#mu} NC RES";     break;
  case (kmcNuMuBarNCDIS) : return "#bar{#nu}_{#mu} NC DIS";     break;
  case (kmcNuMuBarNCCOH) : return "#bar{#nu}_{#mu} NC COH";     break;
  case (kmcNuMuBarNCMEC) : return "#bar{#nu}_{#mu} NC MEC";     break;
  case (kmcNuECCQE     ) : return "#nu_{e} CC QE";          break;
  case (kmcNuECCRES    ) : return "#nu_{e} CC RES";         break;
  case (kmcNuECCDIS    ) : return "#nu_{e} CC DIS";         break;
  case (kmcNuECCCOH    ) : return "#nu_{e} CC COH";         break;
  case (kmcNuECCMEC    ) : return "#nu_{e} CC MEC";         break;
  case (kmcNuENCQE     ) : return "#nu_{e} NC QE";          break;
  case (kmcNuENCRES    ) : return "#nu_{e} NC RES";         break;
  case (kmcNuENCDIS    ) : return "#nu_{e} NC DIS";         break;
  case (kmcNuENCCOH    ) : return "#nu_{e} NC COH";         break;
  case (kmcNuENCMEC    ) : return "#nu_{e} NC MEC";         break;
  case (kmcNuEBarCCQE  ) : return "#bar{#nu}_{e} CC QE";       break;
  case (kmcNuEBarCCRES ) : return "#bar{#nu}_{e} CC RES";      break;
  case (kmcNuEBarCCDIS ) : return "#bar{#nu}_{e} CC DIS";      break;
  case (kmcNuEBarCCCOH ) : return "#bar{#nu}_{e} CC COH";      break;
  case (kmcNuEBarCCMEC ) : return "#bar{#nu}_{e} CC MEC";      break;
  case (kmcNuEBarNCQE  ) : return "#bar{#nu}_{e} NC QE";       break;
  case (kmcNuEBarNCRES ) : return "#bar{#nu}_{e} NC RES";      break;
  case (kmcNuEBarNCDIS ) : return "#bar{#nu}_{e} NC DIS";      break;
  case (kmcNuEBarNCCOH ) : return "#bar{#nu}_{e} NC COH";      break;
  case (kmcNuEBarNCMEC ) : return "#bar{#nu}_{e} NC MEC";      break;
  case (kmcCohElastic  ) : return "COH Elastic";       break;
  case (kmcElScat      ) : return "Electron scattering";  break;
  case (kmcIMDAnh      ) : return "IMD annihilation"; break;
  case (kmcIBD         ) : return "Inverse Beta Decay";              break;
  case (kmcGlashow     ) : return "Glashow Resonance";          break;
  case (kmcAMNuGamma   ) : return "Anomaly-mediated neutrino-gamma";   break;
  case (kmcDiffractive ) : return "Diffractive";      break;
  case (kmcEM          ) : return "EM";               break;
  case (kmcWeakMix     ) : return "Weak [CC+NC+interference]";          break;
  case (kmcCosmic      ) : return "Cosmic";           break;
  case (kmcDirt        ) : return "Dirt";             break;
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
  for (int i = 0 ; i < conv::kNReacModesCoarse ; i++ ) {
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
    conv::ReacModeCoarse_t coarse = conv::kCoarseReacPriority[m][icoarse];
    if(c == coarse) {
      return true;
    }
  }
  return false;
}
//________________________________________________________________________________________

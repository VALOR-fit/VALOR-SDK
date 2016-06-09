//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <TROOT.h>

#include "valor/Conventions/PhysParamId.h"
#include "valor/Conventions/NuOscMixParam.h"
#include "valor/Conventions/Opt.h"
#include "valor/Utilities/UtlStr.h"

using namespace valor;
using namespace valor::conv;




//________________________________________________________________________________________
string util::RawParamAsStringRaw(PhysParamId_t p)
{
  switch(p) {

    case ( kPhypMixing23 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sinsq_2th23"; break;
           case(kNuMixSinsqTheta ) : return "sinsq_th23";   break;
           default                 : return "--";                break;
         }
    }
    break;
    case ( kPhypMixing13 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sinsq_2th_13";  break;
           case(kNuMixSinsqTheta ) : return "sinsq_th13";    break;
           default                 : return "--";                 break;
         }
    }
    break;
    case ( kPhypMixing12 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sinsq_2th12";  break;
           case(kNuMixSinsqTheta ) : return "sinsq_th12";    break;
           default                 : return "--";                 break;
         }
    }
    break;
    case ( kPhypCPV ) : 
    {
        conv::NuOscDeltaCP_t cpv = Opt::Instance()->gOscDeltaCP;
        switch(cpv) {
           case(kNuDeltaCPPhase) : return "delta_cp";      break;
           case(kNuDeltaCPSin  ) : return "sin_delta_cp"; break;
           default               : return "--";            break;
        }
    }
    break;

    case ( kPhypDm23sq  ) :
    {
        NuOscDmsqScheme_t dmscheme = Opt::Instance()->GetEffectiveDmsqScheme();
        switch(dmscheme) {
          case(kNuDmsq12andAbs23) : return "dmsq_32";                  break;
          case(kNuDmsq12andAbs31) : return "dmsq_31";                  break;
          case(kNuDmsqFogliLisi ) : return "dmsq_fl";                   break;
          default                 : return "--";                         break;
        }
    }
    break;

    case ( kPhypDm12sq ) : 
    {
        NuOscDmsqScheme_t dmscheme = Opt::Instance()->GetEffectiveDmsqScheme();
        switch(dmscheme) {
          case(kNuDmsq12andAbs23) : return "dmsq_12";                  break;
          case(kNuDmsq12andAbs31) : return "dmsq_12";                  break;
          case(kNuDmsqFogliLisi ) : return "dmsq_fl";                  break;
          default                 : return "--";                       break;
        }
    }
    break;
    
    // Nubar parameters
    case ( kPhypMixing23Bar ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sinsq_2th23_bar"; break;
           case(kNuMixSinsqTheta ) : return "sinsq_th23_bar";   break;
           default                 : return "--";                    break;
         }
    }
    break;
    case ( kPhypMixing13Bar ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sinsq_2th13_bar";  break;
           case(kNuMixSinsqTheta ) : return "sinsq_th13_bar";    break;
           default                 : return "--";                     break;
         }
    }
    break;
    case ( kPhypMixing12Bar ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sinsq_2th12_bar";  break;
           case(kNuMixSinsqTheta ) : return "sinsq_th12_bar";    break;
           default                 : return "--";                     break;
         }
    }
    break;
    case ( kPhypCPVBar ) : 
    {
        conv::NuOscDeltaCP_t cpv = Opt::Instance()->gOscDeltaCP;
        switch(cpv) {
           case(kNuDeltaCPPhase) : return "deltacp_bar";      break;
           case(kNuDeltaCPSin  ) : return "sin_deltacp_bar"; break;
           default               : return "--";                break;
        }
    }
    break;

    case ( kPhypDm23sqBar  ) :
    {
        NuOscDmsqScheme_t dmscheme = Opt::Instance()->GetEffectiveDmsqScheme();
        switch(dmscheme) {
          case(kNuDmsq12andAbs23) : return "dmsq_32_bar";                  break;
          case(kNuDmsq12andAbs31) : return "dmsq_31_bar";                  break;
          case(kNuDmsqFogliLisi ) : return "dmsq_fl_bar";                  break;
          default                 : return "--";                             break;
        }
    }
    break;

    case ( kPhypDm12sqBar ) : 
    {
        NuOscDmsqScheme_t dmscheme = Opt::Instance()->GetEffectiveDmsqScheme();
        switch(dmscheme) {
          case(kNuDmsq12andAbs23) : return "dmsq_12_bar";                  break;
          case(kNuDmsq12andAbs31) : return "dmsq_12_bar";                  break;
          case(kNuDmsqFogliLisi ) : return "dmsq_fl_bar";                  break;
          default                 : return "--";                           break;
        }
    }
    break;

    // NEW BETABAR
    // Beta: Numubar->Nuebar normalization parameter
    case ( kPhypBetaBar               ) : return "beta_bar";                    break;
    //

    // 
    case ( kPhypAlphaDecay            ) : return "alpha_decay";                break;
    case ( kPhypSinsq2ThetaDecay      ) : return "sinsq_2th_decay";       break;
    case ( kPhypMusqDecoh             ) : return "musq_decoh";                 break;
    case ( kPhypSinsq2ThetaDecoh      ) : return "sinsq_2th_decoh";       break;

    // Steriles
    case ( kPhypMixing14 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sinsq_2th14";  break;
           case(kNuMixSinsqTheta ) : return "sinsq_th14";    break;
           default                 : return "--";                 break;
         }
    }
    break;
    case ( kPhypMixing15 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sinsq_2th15";  break;
           case(kNuMixSinsqTheta ) : return "sinsq_th15";    break;
           default                 : return "--";                 break;
         }
    }
    break;
    case ( kPhypMixing24 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sinsq_2th24";  break;
           case(kNuMixSinsqTheta ) : return "sinsq_th24";    break;
           default                 : return "--";                 break;
         }
    }
    break;
    case ( kPhypMixing25 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sinsq_2th25";  break;
           case(kNuMixSinsqTheta ) : return "sinsq_th25";    break;
           default                 : return "--";                 break;
         }
    }
    break;
    case ( kPhypMixing34 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sinsq_2th34";  break;
           case(kNuMixSinsqTheta ) : return "sinsq_th34";    break;
           default                 : return "--";                 break;
         }
    }
    break;
    case ( kPhypMixing35 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sinsq_2th35";  break;
           case(kNuMixSinsqTheta ) : return "sinsq_th35";    break;
           default                 : return "--";                 break;
         }
    }
    break;

    case ( kPhypDelta12               ) : return "delta_12";        break;
    case ( kPhypDelta13               ) : return "delta_13";        break;
    case ( kPhypDelta15               ) : return "delta_15";        break;
    case ( kPhypDelta34               ) : return "delta_34";        break;
    case ( kPhypDelta35               ) : return "delta_35";        break;

    case ( kPhypDm41sq                ) : return "dmsq_41";       break;
    case ( kPhypDm51sq                ) : return "dmsq_51";       break;

    // Nonstandard matter interaction
    case ( kPhypNSIepsMuTau           ) : return "nsi_epsilonMuTau";    break;

    default:
    {
    }

  }//switch
  
  if (p >= kNuis000 && p< kLastNuisParam) {
    string out(TString::Format("nuis%03d",p-kNuis000).Data());
    return out;
  }
  
  if (p == kLastNuisParam) {
    return "LastNuisanceParam";
  }

  return "?";
}


//________________________________________________________________________________________
string util::RawParamAsString(PhysParamId_t p)
{
  switch(p) {

    case ( kPhypMixing23 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sin^2(2*theta_23)"; break;
           case(kNuMixSinsqTheta ) : return "sin^2(theta_23)";   break;
           default                 : return "--";                break;
         }
    }
    break;
    case ( kPhypMixing13 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sin^2(2*theta_13)";  break;
           case(kNuMixSinsqTheta ) : return "sin^2(theta_13)";    break;
           default                 : return "--";                 break;
         }
    }
    break;
    case ( kPhypMixing12 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sin^2(2*theta_12)";  break;
           case(kNuMixSinsqTheta ) : return "sin^2(theta_12)";    break;
           default                 : return "--";                 break;
         }
    }
    break;
    case ( kPhypCPV ) : 
    {
        conv::NuOscDeltaCP_t cpv = Opt::Instance()->gOscDeltaCP;
        switch(cpv) {
           case(kNuDeltaCPPhase) : return "delta_cp";      break;
           case(kNuDeltaCPSin  ) : return "sin(delta_cp)"; break;
           default               : return "--";            break;
        }
    }
    break;

    case ( kPhypDm23sq  ) :
    {
        NuOscDmsqScheme_t dmscheme = Opt::Instance()->GetEffectiveDmsqScheme();
        switch(dmscheme) {
          case(kNuDmsq12andAbs23) : return "|Dm^2_32|";                  break;
          case(kNuDmsq12andAbs31) : return "|Dm^2_31|";                  break;
          case(kNuDmsqFogliLisi ) : return "|Dm^2_FL|";                   break;
          default                 : return "--";                         break;
        }
    }
    break;

    case ( kPhypDm12sq ) : 
    {
        NuOscDmsqScheme_t dmscheme = Opt::Instance()->GetEffectiveDmsqScheme();
        switch(dmscheme) {
          case(kNuDmsq12andAbs23) : return "Dm^2_12";                  break;
          case(kNuDmsq12andAbs31) : return "Dm^2_12";                  break;
          case(kNuDmsqFogliLisi ) : return "dm^2_FL";                  break;
          default                 : return "--";                       break;
        }
    }
    break;
    
    // Nubar parameters
    case ( kPhypMixing23Bar ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sin^2(2*theta_23 bar)"; break;
           case(kNuMixSinsqTheta ) : return "sin^2(theta_23 bar)";   break;
           default                 : return "--";                    break;
         }
    }
    break;
    case ( kPhypMixing13Bar ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sin^2(2*theta_13 bar)";  break;
           case(kNuMixSinsqTheta ) : return "sin^2(theta_13 bar)";    break;
           default                 : return "--";                     break;
         }
    }
    break;
    case ( kPhypMixing12Bar ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sin^2(2*theta_12 bar)";  break;
           case(kNuMixSinsqTheta ) : return "sin^2(theta_12 bar)";    break;
           default                 : return "--";                     break;
         }
    }
    break;
    case ( kPhypCPVBar ) : 
    {
        conv::NuOscDeltaCP_t cpv = Opt::Instance()->gOscDeltaCP;
        switch(cpv) {
           case(kNuDeltaCPPhase) : return "delta_cp bar";      break;
           case(kNuDeltaCPSin  ) : return "sin(delta_cp bar)"; break;
           default               : return "--";                break;
        }
    }
    break;

    case ( kPhypDm23sqBar  ) :
    {
        NuOscDmsqScheme_t dmscheme = Opt::Instance()->GetEffectiveDmsqScheme();
        switch(dmscheme) {
          case(kNuDmsq12andAbs23) : return "|Dm^2_32| bar";                  break;
          case(kNuDmsq12andAbs31) : return "|Dm^2_31| bar";                  break;
          case(kNuDmsqFogliLisi ) : return "|Dm^2_FL| bar";                  break;
          default                 : return "--";                             break;
        }
    }
    break;

    case ( kPhypDm12sqBar ) : 
    {
        NuOscDmsqScheme_t dmscheme = Opt::Instance()->GetEffectiveDmsqScheme();
        switch(dmscheme) {
          case(kNuDmsq12andAbs23) : return "Dm^2_12 bar";                  break;
          case(kNuDmsq12andAbs31) : return "Dm^2_12 bar";                  break;
          case(kNuDmsqFogliLisi ) : return "dm^2_FL bar";                  break;
          default                 : return "--";                           break;
        }
    }
    break;

    // NEW BETABAR
    // Beta: Numubar->Nuebar normalization parameter
    case ( kPhypBetaBar               ) : return "beta bar";                    break;
    //

    // 
    case ( kPhypAlphaDecay            ) : return "alpha_decay";                break;
    case ( kPhypSinsq2ThetaDecay      ) : return "sin^2(2*theta_decay)";       break;
    case ( kPhypMusqDecoh             ) : return "mu^2_decoh";                 break;
    case ( kPhypSinsq2ThetaDecoh      ) : return "sin^2(2*theta_decoh)";       break;

    // Steriles
    case ( kPhypMixing14 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sin^2(2*theta_14)";  break;
           case(kNuMixSinsqTheta ) : return "sin^2(theta_14)";    break;
           default                 : return "--";                 break;
         }
    }
    break;
    case ( kPhypMixing15 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sin^2(2*theta_15)";  break;
           case(kNuMixSinsqTheta ) : return "sin^2(theta_15)";    break;
           default                 : return "--";                 break;
         }
    }
    break;
    case ( kPhypMixing24 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sin^2(2*theta_24)";  break;
           case(kNuMixSinsqTheta ) : return "sin^2(theta_24)";    break;
           default                 : return "--";                 break;
         }
    }
    break;
    case ( kPhypMixing25 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sin^2(2*theta_25)";  break;
           case(kNuMixSinsqTheta ) : return "sin^2(theta_25)";    break;
           default                 : return "--";                 break;
         }
    }
    break;
    case ( kPhypMixing34 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sin^2(2*theta_34)";  break;
           case(kNuMixSinsqTheta ) : return "sin^2(theta_34)";    break;
           default                 : return "--";                 break;
         }
    }
    break;
    case ( kPhypMixing35 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "sin^2(2*theta_35)";  break;
           case(kNuMixSinsqTheta ) : return "sin^2(theta_35)";    break;
           default                 : return "--";                 break;
         }
    }
    break;

    case ( kPhypDelta12               ) : return "delta_12";        break;
    case ( kPhypDelta13               ) : return "delta_13";        break;
    case ( kPhypDelta15               ) : return "delta_15";        break;
    case ( kPhypDelta34               ) : return "delta_34";        break;
    case ( kPhypDelta35               ) : return "delta_35";        break;

    case ( kPhypDm41sq                ) : return "|Dm^2_41|";       break;
    case ( kPhypDm51sq                ) : return "|Dm^2_51|";       break;

    // Nonstandard matter interaction
    case ( kPhypNSIepsMuTau           ) : return "nsi_epsilonMuTau";    break;

    default:
    {
    }

  }//switch
  
  if (p >= kNuis000 && p< kLastNuisParam) {
    string out(TString::Format("nuis%03d",p-kNuis000).Data());
    return out;
  }
  
  if (p == kLastNuisParam) {
    return "LastNuisanceParam";
  }

  return "?";
}
//________________________________________________________________________________________
string util::RawParamAsStringTex(PhysParamId_t p)
{
  switch(p) {

    case ( kPhypMixing23 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "$\\sin^2(2\\theta_{23})$"; break;
           case(kNuMixSinsqTheta ) : return "$\\sin^2(\\theta_{23})$";  break;
           default                 : return "--";                       break;
         }
    }
    break;

    case ( kPhypMixing13 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "$\\sin^2(2\\theta_{13})$"; break;
           case(kNuMixSinsqTheta ) : return "$\\sin^2(\\theta_{13})$";  break;
           default                 : return "--";                       break;
         }
    }
    break;

    case ( kPhypMixing12 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "$\\sin^2(2\\theta_{12})$"; break;
           case(kNuMixSinsqTheta ) : return "$\\sin^2(\\theta_{12})$";  break;
           default                 : return "--";                       break;
         }
    }
    break;

    case ( kPhypCPV ) : 
    {
        conv::NuOscDeltaCP_t cpv = Opt::Instance()->gOscDeltaCP;
        switch(cpv) {
           case(kNuDeltaCPPhase) : return "$\\delta_{CP}$";      break;
           case(kNuDeltaCPSin  ) : return "$sin(\\delta_{CP})$"; break;
           default               : return "--";                  break;
        }
    }
    break;

    case ( kPhypDm12sq ) : 
    {
        NuOscDmsqScheme_t dmscheme = Opt::Instance()->GetEffectiveDmsqScheme();
        switch(dmscheme) {
          case(kNuDmsq12andAbs23) : return "$\\Delta m^2_{21}$";                  break;
          case(kNuDmsq12andAbs31) : return "$\\Delta m^2_{21}$";                  break;
          case(kNuDmsqFogliLisi ) : return "$\\delta m^2$";                       break;
          default                 : return "--";                                  break;
        }
    }
    break;

    case ( kPhypDm23sq ) :
    {
        NuOscDmsqScheme_t dmscheme = Opt::Instance()->GetEffectiveDmsqScheme();
        switch(dmscheme) {
          case(kNuDmsq12andAbs23) : return "$|\\Delta m^2_{32}|$";                  break;
          case(kNuDmsq12andAbs31) : return "$|\\Delta m^2_{31}|$";                  break;
          case(kNuDmsqFogliLisi ) : return "$|\\Delta m^2|$";                       break;
          default                 : return "--";                                  break;
        }
    }
    break;
    
    case ( kPhypMixing23Bar ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "$\\sin^2(2\\bar{\\theta_{23}})$"; break;
           case(kNuMixSinsqTheta ) : return "$\\sin^2(\\bar{\\theta_{23}})$";  break;
           default                 : return "--";                              break;
         }
    }
    break;

    case ( kPhypMixing13Bar ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "$\\sin^2(2\\bar{\\theta_{13}})$"; break;
           case(kNuMixSinsqTheta ) : return "$\\sin^2(\\bar{\\theta_{13}})$";  break;
           default                 : return "--";                              break;
         }
    }
    break;

    case ( kPhypMixing12Bar ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "$\\sin^2(2\\bar{\\theta_{12}})$"; break;
           case(kNuMixSinsqTheta ) : return "$\\sin^2(\\bar{\\theta_{12}})$";  break;
           default                 : return "--";                              break;
         }
    }
    break;

    case ( kPhypCPVBar ) : 
    {
        conv::NuOscDeltaCP_t cpv = Opt::Instance()->gOscDeltaCP;
        switch(cpv) {
           case(kNuDeltaCPPhase) : return "$\\bar{\\delta_{CP}}$";      break;
           case(kNuDeltaCPSin  ) : return "$sin(\\bar{\\delta_{CP}})$"; break;
           default               : return "--";                         break;
        }
    }
    break;

    case ( kPhypDm12sqBar ) : 
    {
        NuOscDmsqScheme_t dmscheme = Opt::Instance()->GetEffectiveDmsqScheme();
        switch(dmscheme) {
          case(kNuDmsq12andAbs23) : return "$\\bar{\\Delta m^2_{21}}$";             break;
          case(kNuDmsq12andAbs31) : return "$\\bar{\\Delta m^2_{21}}$";             break;
          case(kNuDmsqFogliLisi ) : return "$\\bar{\\delta m^2}$";                  break;
          default                 : return "--";                                  break;
        }
    }
    break;

    case ( kPhypDm23sqBar ) :
    {
        NuOscDmsqScheme_t dmscheme = Opt::Instance()->GetEffectiveDmsqScheme();
        switch(dmscheme) {
          case(kNuDmsq12andAbs23) : return "$|\\bar{\\Delta m^2_{32}}|$";                  break;
          case(kNuDmsq12andAbs31) : return "$|\\bar{\\Delta m^2_{31}}|$";                  break;
          case(kNuDmsqFogliLisi ) : return "$|\\bar{\\Delta m^2}|$";                       break;
          default                 : return "--";                                  break;
        }
    }
    break;

    // NEW BETABAR
    // Beta: Numubar->Nuebar normalization parameter
    case ( kPhypBetaBar   ) : return "$\\bar{\\beta}$";                    break;
    //

    case ( kPhypAlphaDecay            ) : return "$\\alpha_{decay}$";                break;
    case ( kPhypSinsq2ThetaDecay      ) : return "$\\sin^2(2\\theta_{decay})$";      break;
    case ( kPhypMusqDecoh             ) : return "$\\mu^2_{decoh}$";                 break;
    case ( kPhypSinsq2ThetaDecoh      ) : return "$\\sin^2(2\\theta_{decoh})$";      break;

    // Steriles
    case ( kPhypMixing14 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "$\\sin^2(2\\theta_{14})$";   break;
           case(kNuMixSinsqTheta ) : return "$\\sin^2(\\theta_{14})$";    break;
           default                 : return "--";                         break;
         }
    }
    break;

    case ( kPhypMixing15 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "$\\sin^2(2\\theta_{15})$";  break;
           case(kNuMixSinsqTheta ) : return "$\\sin^2(\\theta_{15})$";   break;
           default                 : return "--";                        break;
         }
    }
    break;

    case ( kPhypMixing24 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "$\\sin^2(2\\theta_{24})$";  break;
           case(kNuMixSinsqTheta ) : return "$\\sin^2(\\theta_{24})$";   break;
           default                 : return "--";                        break;
         }
    }
    break;

    case ( kPhypMixing25 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "$\\sin^2(2\\theta_{25})$";  break;
           case(kNuMixSinsqTheta ) : return "$\\sin^2(\\theta_{25})$";   break;
           default                 : return "--";                        break;
         }
    }
    break;

    case ( kPhypMixing34 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "$\\sin^2(2\\theta_{34})$"; break;
           case(kNuMixSinsqTheta ) : return "$\\sin^2(\\theta_{34})$";  break;
           default                 : return "--";                       break;
         }
    }
    break;

    case ( kPhypMixing35 ) :
    {
        NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
        switch(omp) {
           case(kNuMixSinsq2Theta) : return "$\\sin^2(2\\theta_{35})$";  break;
           case(kNuMixSinsqTheta ) : return "$\\sin^2(\\theta_{35})$";   break;
           default                 : return "--";                        break;
         }
    }
    break;

    case ( kPhypDelta12               ) : return "$\\delta_{12}$";                   break;
    case ( kPhypDelta13               ) : return "$\\delta_{13}$";                   break;
    case ( kPhypDelta15               ) : return "$\\delta_{15}$";                   break;
    case ( kPhypDelta34               ) : return "$\\delta_{34}$";                   break;
    case ( kPhypDelta35               ) : return "$\\delta_{35}$";                   break;
    case ( kPhypDm41sq                ) : return "$|\\Delta m^2_{41}|$";             break;
    case ( kPhypDm51sq                ) : return "$|\\Delta m^2_{51}|$";             break;

    // Nonstandard matter interaction
    case ( kPhypNSIepsMuTau           ) : return "$nsi_\\epsilon_{\\mu\\tau}$";      break;

    default :
      return "--";
  }
  return "--";
}
//________________________________________________________________________________________
string util::RawParamAsStringRooTex(PhysParamId_t p)
{
  switch(p) {

    case ( kPhypMixing23 ) :
    {
      NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
      switch(omp) {
      case(kNuMixSinsq2Theta) : return "sin^{2}(2#theta_{23})";  break;
      case(kNuMixSinsqTheta ) : return "sin^{2}(#theta_{23})";   break;
      default                 : return "--";                     break;
      }
    }
    break;

    case ( kPhypMixing13 ) :
    {
      NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
      switch(omp) {
      case(kNuMixSinsq2Theta) : return "sin^{2}(2#theta_{13})";   break;
      case(kNuMixSinsqTheta ) : return "sin^{2}(#theta_{13})";    break;
      default                 : return "--";                      break;
      }
    }
    break;

    case ( kPhypMixing12 ) :
    {
      NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
      switch(omp) {
      case(kNuMixSinsq2Theta) : return "sin^{2}(2#theta_{12})";   break;
      case(kNuMixSinsqTheta ) : return "sin^{2}(#theta_{12})";    break;
      default                 : return "--";                      break;
      }
    }
    break;

    case ( kPhypCPV ) : 
    {
        conv::NuOscDeltaCP_t cpv = Opt::Instance()->gOscDeltaCP;
        switch(cpv) {
           case(kNuDeltaCPPhase) : return "#delta_{CP}";      break;
           case(kNuDeltaCPSin  ) : return "sin(#delta_{CP})"; break;
           default               : return "--";               break;
        }
    }
    break;

    case ( kPhypDm23sq ) :
    {
      NuOscDmsqScheme_t omp = Opt::Instance()->GetEffectiveDmsqScheme();
      switch(omp) {
        case(kNuDmsqFogliLisi ) : return "|#Deltam^{2}|";         break;
//      case(kNuDmsqFogliLisi ) : return "|#Deltam^{2}| = |m^{2}_{3} - (m^{2}_{1} + m^{2}_{1})/2|";  break;
        case(kNuDmsq12andAbs23) : return "|#Deltam^{2}_{32}|";    break;
        case(kNuDmsq12andAbs31) : return "|#Deltam^{2}_{31}|";    break;
        default                 : return "--";                    break;
      }
    }
    break;

    case ( kPhypDm12sq ) :
    {
      NuOscDmsqScheme_t omp = Opt::Instance()->GetEffectiveDmsqScheme();
      switch(omp) {
        case(kNuDmsqFogliLisi):
          return "#deltam^{2}";                        break;
//        return "#deltam^{2} = #Deltam^{2}_{21}";     break;
        case(kNuDmsq12andAbs23):
        case(kNuDmsq12andAbs31):
          return "#Delta m^{2}_{21}";                  break;
        default                 :
          return "--";                                 break;
      }
    }
    break;
    
    case ( kPhypMixing23Bar ) :
    {
      NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
      switch(omp) {
      case(kNuMixSinsq2Theta) : return "sin^{2}(#bar{2#theta_{23}})";  break;
      case(kNuMixSinsqTheta ) : return "sin^{2}(#bar{#theta_{23}})";   break;
      default                 : return "--";                           break;
      }
    }
    break;

    case ( kPhypMixing13Bar ) :
    {
      NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
      switch(omp) {
      case(kNuMixSinsq2Theta) : return "sin^{2}(2#bar{#theta_{13}})";   break;
      case(kNuMixSinsqTheta ) : return "sin^{2}(#bar{#theta_{13}})";    break;
      default                 : return "--";                      break;
      }
    }
    break;

    case ( kPhypMixing12Bar ) :
    {
      NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
      switch(omp) {
      case(kNuMixSinsq2Theta) : return "sin^{2}(2#bar{#theta_{12}})";   break;
      case(kNuMixSinsqTheta ) : return "sin^{2}(#bar{#theta_{12}})";    break;
      default                 : return "--";                      break;
      }
    }
    break;

    case ( kPhypCPVBar ) : 
    {
        conv::NuOscDeltaCP_t cpv = Opt::Instance()->gOscDeltaCP;
        switch(cpv) {
           case(kNuDeltaCPPhase) : return "#bar{#delta_{CP}}";      break;
           case(kNuDeltaCPSin  ) : return "sin(#bar{#delta_{CP}})"; break;
           default               : return "--";               break;
        }
    }
    break;

    case ( kPhypDm23sqBar ) :
    {
      NuOscDmsqScheme_t omp = Opt::Instance()->GetEffectiveDmsqScheme();
      switch(omp) {
        case(kNuDmsqFogliLisi ) : return "|#bar{#Deltam^{2}}|";         break;
//      case(kNuDmsqFogliLisi ) : return "|#bar{#Deltam^{2}}| = |m^{2}_{3} - (m^{2}_{1} + m^{2}_{1})/2|";  break;
        case(kNuDmsq12andAbs23) : return "|#bar{#Deltam^{2}_{32}}|";    break;
        case(kNuDmsq12andAbs31) : return "|#bar{#Deltam^{2}_{31}}|";    break;
        default                 : return "--";                          break;
      }
    }
    break;

    case ( kPhypDm12sqBar ) :
    {
      NuOscDmsqScheme_t omp = Opt::Instance()->GetEffectiveDmsqScheme();
      switch(omp) {
        case(kNuDmsqFogliLisi):
          return "#bar{#deltam^{2}}";                        break;
//        return "#bar{#deltam^{2}} = #Deltam^{2}_{21}";     break;
        case(kNuDmsq12andAbs23):
        case(kNuDmsq12andAbs31):
          return "#bar{#Delta m^{2}_{21}}";                  break;
        default                 :
          return "--";                                 break;
      }
    }
    break;

    // NEW BETABAR
    // Beta: Numubar->Nuebar normalization parameter
    case ( kPhypBetaBar               ) : return "#bar{#beta}";                    break;
    //

    case ( kPhypAlphaDecay            ) : return "#alpha_{decay}";                 break;
    case ( kPhypSinsq2ThetaDecay      ) : return "sin^{2}(2#theta_{decay})";       break;
    case ( kPhypMusqDecoh             ) : return "#mu^{2}_{decoh}";                break;
    case ( kPhypSinsq2ThetaDecoh      ) : return "sin^{2}(2#theta_{decoh})";       break;

    default :
      return "--";
  }
  return "--";
}
//________________________________________________________________________________________
valor::conv::PhysParamId_t
  valor::util::PhysParamIdFromRawString(string sh) 
{
  string lsh = util::ToLowerCase(sh);
  for (int i = 0 ; i < knppmax ; i++) {
    PhysParamId_t k = (PhysParamId_t) i ;
    string s = util::ToLowerCase(RawParamAsString(k));
    if (k != kPhypUndefined && (lsh == s) ) return k;
  }
  std::cerr<<"util::PhysParamIdFromRawString >> Unmatched string: "<<sh<<std::endl;
  return kPhypUndefined;
}
//________________________________________________________________________________________
bool util::IsMixingAngle(PhysParamId_t p)
{
  switch(p) {
    case ( kPhypMixing23              ) : return true; break;
    case ( kPhypMixing13              ) : return true; break;
    case ( kPhypMixing12              ) : return true; break;
    case ( kPhypMixing23Bar           ) : return true; break;
    case ( kPhypMixing13Bar           ) : return true; break;
    case ( kPhypMixing12Bar           ) : return true; break;
    case ( kPhypSinsq2ThetaDecay      ) : return true; break;
    case ( kPhypSinsq2ThetaDecoh      ) : return true; break;
    case ( kPhypMixing14              ) : return true; break;
    case ( kPhypMixing15              ) : return true; break;
    case ( kPhypMixing24              ) : return true; break;
    case ( kPhypMixing25              ) : return true; break;
    case ( kPhypMixing34              ) : return true; break;
    case ( kPhypMixing35              ) : return true; break;

    default :
      return false;
  }
  return false;
}
//________________________________________________________________________________________
bool util::IsMassSplitting(PhysParamId_t p)
{
  switch(p) {
    case ( kPhypDm23sq           ) : return true;
    case ( kPhypDm12sq           ) : return true;
    case ( kPhypDm23sqBar        ) : return true;
    case ( kPhypDm12sqBar        ) : return true;
    case ( kPhypDm41sq           ) : return true;
    case ( kPhypDm51sq           ) : return true;

    default :
      return false;
  }
  return false;
}
//________________________________________________________________________________________
bool util::IsPhase(PhysParamId_t p)
{
  switch(p) {
    case ( kPhypCPV              ) : return true;
    case ( kPhypCPVBar           ) : return true;
    case ( kPhypDelta12          ) : return true;
    case ( kPhypDelta13          ) : return true;
    case ( kPhypDelta15          ) : return true;
    case ( kPhypDelta34          ) : return true;
    case ( kPhypDelta35          ) : return true;

    default :
      return false;
  }
  return false;
}
//________________________________________________________________________________________
bool util::IsNormalization(PhysParamId_t p)
{
  switch(p) {
    case ( kPhypBetaBar           ) : return true;

    default :
      return false;
  }
  return false;
}
//________________________________________________________________________________________
bool util::IsSystematic(PhysParamId_t p)
{
  if(p >= kNuis000) return true;
  return false;
}
//________________________________________________________________________________________


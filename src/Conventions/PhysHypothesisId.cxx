//________________________________________________________________________________________
/*
 
 Copyright (c) 2010-2016, The VALOR group. All rights reserved.
 
 ChangeLog:
 
 */
//________________________________________________________________________________________

#include <sstream>
#include <cassert>
#include <TROOT.h>

#include "Conventions/PhysHypothesisId.h"
#include "Conventions/Opt.h"

using std::ostringstream;

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________
string util::AsString(PhysHypothesis_t h)
{
  switch(h) {
    case (kHypDecay) :
      return "<Neutrino Decay>";
      break;
    case (kHypDecoh) :
      return "<Neutrino Decoherence>";
      break;
    case (kHypNuMuDisapp2F) :
      return "<2-Flavour Neutrino Oscillations: nu_mu disappearance>";
      break;
    case (kHypOsc3F) :
      return "<3-flavour Neutrino Oscillations>";
      break;
    case (kHypOsc3FBar) :
      return "<3-flavour Neutrino Oscillations, split nu and nubar>";
      break;
    case (kHypOsc3FBarDissap) :
      return "<3-flavour Neutrino Oscillations, split 23 sector nu and nubar>";
      break;
      // NEW BETABAR
    case (kHypOsc3FBarNormBar) :
      return "<3-flavour Neutrino Oscillations, split nu and nubar, normalization numubar->nuebar>";
      break;
      //
      // NEW BETABAR 2
    case (kHypOsc3FNormBar) :
      return "<3-flavour Neutrino Oscillations, normalization numubar->nuebar>";
      break;
      //
    case (kHypOscSterile4F) :
      return "<3+1 Sterile Neutrino Oscillations>";
      break;
    case (kHypOscSterile5F) :
      return "<3+2 Sterile Neutrino Oscillations>";
      break;
    case (kHypNSI3F) :
      return "<3-flavour Neutrino Oscillations plus mu-tau NSI>";
      break;
    case (kHypVoid) :
      return "<Systematics Constraint>";
      break;
    default:
      return "undefined";
      break;
  }
  return "";
}
//________________________________________________________________________________________
string util::PhysHypAsString(PhysHypothesis_t h) { return AsString(h); }
//________________________________________________________________________________________
string util::AsString (
                       PhysHypothesis_t hyp, double mix12, double mix13, double mix23, double deltacp,
                       double dm12sq, double dm23sq)
{
  // The code builds strings which are typically used in building names of
  // files containing valor results
  //
  
  NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
  NuOscOctant_t   oct = Opt::Instance()->gOscOctantChoice;
  string mixparam = "?";
  switch(omp) {
    case(kNuMixSinsq2Theta):
      switch(oct) {
        case(kNuOctantOne):
          mixparam = "sinsq2theta_octantone";
          break;
        case(kNuOctantTwo):
          mixparam = "sinsq2theta_octanttwo";
          break;
        default:
          return "";
      }
      break;
    case(kNuMixSinsqTheta):
      mixparam = "sinsqtheta";
      break;
    default:
      return "";
  }
  
  ostringstream out;
  
  //
  // 3-flavour oscillations
  // Build the string as mix[12_nnn,13_nnn,23_nnn],dcp_nnn,dmsq[12_nnn,23_nnn]eV2
  // where mix is either sinsqtheta or sinsq2theta and nnn is the parameter
  // value with the specified numerical accuracy.
  // Params with value of -999 (randomized during toy-MC generation) will not be
  // included in the output string
  //
  if( hyp == valor::conv::kHypOsc3F )
  {
    double rndmz = -900;
    bool show_mix12   = (mix12   > rndmz);
    bool show_mix13   = (mix13   > rndmz);
    bool show_mix23   = (mix23   > rndmz);
    bool show_mix     = show_mix12 || show_mix13 || show_mix23;
    bool show_deltacp = (deltacp > rndmz);
    bool show_dm12sq  = (dm12sq  > rndmz);
    bool show_dm23sq  = (dm23sq  > rndmz);
    bool show_dmsq    = show_dm12sq || show_dm23sq;
    
    if(show_mix)
    {
      out << mixparam << "[";
      if(show_mix12) {
        out << "12_" << Form("%.3f",mix12);
      }
      if(show_mix13) {
        if(show_mix12) {
          out << ",";
        }
        out << "13_" << Form("%.3f",mix13);
      }
      if(show_mix23) {
        if(show_mix13 || (!show_mix13 && show_mix12)) {
          out << ",";
        }
        out << "23_" << Form("%.3f",mix23);
      }
      out << "]";
    }
    
    if(show_mix && (show_deltacp || show_dmsq))
    {
      out << ",";
    }
    
    if(show_deltacp)
    {
      out << "dcp_" << Form("%.3f",deltacp);
    }
    
    if(show_deltacp && show_dmsq)
    {
      out << ",";
    }
    
    if(show_dmsq)
    {
      out << "dmsq_" << util::AsString(Opt::Instance()->gMassHierarchyAssumed) << "[";
      if(show_dm12sq) {
        out << "12_" << Form("%.8f",dm12sq);
      }
      if(show_dm23sq) {
        if(show_dm12sq) {
          out << ",";
        }
        out << "23_" << Form("%.6f",dm23sq);
      }
      out << "]eV2";
    }
    
  }//3f
  
  // Placeholder for nubar
  if (hyp == valor::conv::kHypOsc3FBar) {
    out<<"3f+3fbar";
  }
  
  return out.str();
  
}





// NEW BETABAR

//________________________________________________________________________________________
string util::AsString (
                       PhysHypothesis_t hyp, double mix12, double mix13, double mix23,
                       double deltacp, double dm12sq, double dm23sq, double betabar, double mix23bar, double dm23sqbar)
{
  // The code builds strings which are typically used in building names of
  // files containing valor results
  //
  
  NuOscMixParam_t omp = Opt::Instance()->gOscMixingParamChoice;
  NuOscOctant_t   oct = Opt::Instance()->gOscOctantChoice;
  string mixparam = "?";
  switch(omp) {
    case(kNuMixSinsq2Theta):
      switch(oct) {
        case(kNuOctantOne):
          mixparam = "sinsq2theta_octantone";
          break;
        case(kNuOctantTwo):
          mixparam = "sinsq2theta_octanttwo";
          break;
        default:
          return "";
      }
      break;
    case(kNuMixSinsqTheta):
      mixparam = "sinsqtheta";
      break;
    default:
      return "";
  }
  
  ostringstream out;
  
  //
  // 3-flavour oscillations
  // Build the string as mix[12_nnn,13_nnn,23_nnn],dcp_nnn,dmsq[12_nnn,23_nnn]eV2
  // where mix is either sinsqtheta or sinsq2theta and nnn is the parameter
  // value with the specified numerical accuracy.
  // Params with value of -999 (randomized during toy-MC generation) will not be
  // included in the output string
  //
  if( hyp == valor::conv::kHypOsc3F )
  {
    double rndmz = -900;
    bool show_mix12   = (mix12   > rndmz);
    bool show_mix13   = (mix13   > rndmz);
    bool show_mix23   = (mix23   > rndmz);
    bool show_mix     = show_mix12 || show_mix13 || show_mix23;
    bool show_deltacp = (deltacp > rndmz);
    bool show_dm12sq  = (dm12sq  > rndmz);
    bool show_dm23sq  = (dm23sq  > rndmz);
    bool show_dmsq    = show_dm12sq || show_dm23sq;
    
    if(show_mix)
    {
      out << mixparam << "[";
      if(show_mix12) {
        out << "12_" << Form("%.3f",mix12);
      }
      if(show_mix13) {
        if(show_mix12) {
          out << ",";
        }
        out << "13_" << Form("%.3f",mix13);
      }
      if(show_mix23) {
        if(show_mix13 || (!show_mix13 && show_mix12)) {
          out << ",";
        }
        out << "23_" << Form("%.3f",mix23);
      }
      out << "]";
    }
    
    if(show_mix && (show_deltacp || show_dmsq))
    {
      out << ",";
    }
    
    if(show_deltacp)
    {
      out << "dcp_" << Form("%.3f",deltacp);
    }
    
    if(show_deltacp && show_dmsq)
    {
      out << ",";
    }
    
    if(show_dmsq)
    {
      out << "dmsq_" << util::AsString(Opt::Instance()->gMassHierarchyAssumed) << "[";
      if(show_dm12sq) {
        out << "12_" << Form("%.8f",dm12sq);
      }
      if(show_dm23sq) {
        if(show_dm12sq) {
          out << ",";
        }
        out << "23_" << Form("%.6f",dm23sq);
      }
      out << "]eV2";
    }
    
  }//3f
  
  // Placeholder for nubar
  if (hyp == valor::conv::kHypOsc3FBar) {
    out<<"3f+3fbar";
  }
  
  if (hyp == valor::conv::kHypOsc3FBarDissap)
  {
    double rndmz = -900;
    bool show_mix12     = (mix12      > rndmz);
    bool show_mix13     = (mix13      > rndmz);
    bool show_mix23     = (mix23      > rndmz);
    bool show_mix23bar  = (mix23      > rndmz);
    bool show_mix       = show_mix12 || show_mix13 || show_mix23;
    bool show_deltacp   = (deltacp    > rndmz);
    bool show_dm12sq    = (dm12sq     > rndmz);
    bool show_dm23sq    = (dm23sq     > rndmz);
    bool show_dm23sqbar = (dm23sqbar  > rndmz);
    bool show_dmsq      = show_dm12sq || show_dm23sq || show_dm23sqbar;
    
    if(show_mix)
    {
      out << mixparam << "[";
      if(show_mix12) {
        out << "12_" << Form("%.3f",mix12);
      }
      if(show_mix13) {
        if(show_mix12) {
          out << ",";
        }
        out << "13_" << Form("%.3f",mix13);
      }
      if(show_mix23) {
        if(show_mix13 || (!show_mix13 && show_mix12)) {
          out << ",";
        }
        out << "23_" << Form("%.3f",mix23);
      }
      if(show_mix23bar) {
        if(show_mix23) {
          out << ",";
        }
        out << ",23bar_" << Form("%.3f",mix23bar);
      }
      
      
      out << "]";
    }
    
    if(show_mix && (show_deltacp || show_dmsq))
    {
      out << ",";
    }
    
    if(show_deltacp)
    {
      out << "dcp_" << Form("%.3f",deltacp);
    }
    
    if(show_deltacp && show_dmsq)
    {
      out << ",";
    }
    
    if(show_dmsq)
    {
      out << "dmsq_" << util::AsString(Opt::Instance()->gMassHierarchyAssumed) << "[";
      if(show_dm12sq) {
        out << "12_" << Form("%.8f",dm12sq);
      }
      if(show_dm23sq) {
        if(show_dm12sq) {
          out << ",";
        }
        out << "23_" << Form("%.6f",dm23sq);
      }
      if(show_dm23sqbar) {
        if(show_dm23sq) {
          out << ",";
        }
        out << ",23bar_" << Form("%.6f",dm23sqbar);
      }
      out << "]eV2";
      
    }//3f+23bar
    
    // NEW BETABAR
    // Placeholder for nubar+norm nubar
    if (hyp == valor::conv::kHypOsc3FBarNormBar) {
      out<<"3f+3fbar+normbar";
    }
    //
    
    // NEW BETABAR 2
    // Placeholder for norm nubar
    if (hyp == valor::conv::kHypOsc3FNormBar) {
      out<<"3f+normbar";
    }
    //
    
    // Placeholder for steriles
    if (hyp == valor::conv::kHypOscSterile4F) {
      out<<"sterile_3+1";
    }
    if (hyp == valor::conv::kHypOscSterile5F) {
      out<<"sterile_3+2";
    }
    
    //
    // 2-flavour oscillations, numu disappearance
    // Build the string as mix23_nnn,dm23sq_nnneV2
    // where mix is either sinsqtheta or sinsq2theta and nnn is the parameter
    // value with the specified numerical accuracy.
    //
    if( hyp == valor::conv::kHypNuMuDisapp2F)
    {
      out << mixparam << "23_" << Form("%.3f",mix23) << ",dm23sq_" << Form("%.5f",dm23sq) << "eV2";
    }
    
    return out.str();
  }//3f+23bar
  
  /*
   // Placeholder for nubar+norm
   if (hyp == valor::conv::kHypOsc3FBarNormBar) {
   
   double rndmz = -900;
   bool show_mix12   = (mix12   > rndmz);
   bool show_mix13   = (mix13   > rndmz);
   bool show_mix23   = (mix23   > rndmz);
   bool show_mix     = show_mix12 || show_mix13 || show_mix23;
   bool show_deltacp = (deltacp > rndmz);
   bool show_dm12sq  = (dm12sq  > rndmz);
   bool show_dm23sq  = (dm23sq  > rndmz);
   bool show_dmsq    = show_dm12sq || show_dm23sq;
   bool show_betabar = (betabar > rndmz);
   
   if(show_mix)
   {
   out << mixparam << "[";
   if(show_mix12) {
   out << "12_" << Form("%.3f",mix12);
   }
   if(show_mix13) {
   if(show_mix12) {
   out << ",";
   }
   out << "13_" << Form("%.3f",mix13);
   }
   if(show_mix23) {
   if(show_mix13 || (!show_mix13 && show_mix12)) {
   out << ",";
   }
   out << "23_" << Form("%.3f",mix23);
   }
   out << "]";
   }
   
   if(show_mix && (show_deltacp || show_dmsq))
   {
   out << ",";
   }
   
   if(show_deltacp)
   {
   out << "dcp_" << Form("%.3f",deltacp);
   }
   
   if(show_deltacp && show_dmsq)
   {
   out << ",";
   }
   
   if(show_dmsq)
   {
   out << "dmsq_" << util::AsString(Opt::Instance()->gMassHierarchyAssumed) << "[";
   if(show_dm12sq) {
   out << "12_" << Form("%.8f",dm12sq);
   }
   if(show_dm23sq) {
   if(show_dm12sq) {
   out << ",";
   }
   out << "23_" << Form("%.6f",dm23sq);
   }
   out << "]eV2";
   }
   
   if(show_betabar)
   {
   out << ",betabar_" << Form("%.3f",betabar);
   }
   
   }//3f_oscbar_normbar
   */
  
  // NEW BETABAR 2
  
  // Placeholder for norm nuebar
  if (hyp == valor::conv::kHypOsc3FNormBar) {
    
    double rndmz = -900;
    bool show_mix12   = (mix12   > rndmz);
    bool show_mix13   = (mix13   > rndmz);
    bool show_mix23   = (mix23   > rndmz);
    bool show_mix     = show_mix12 || show_mix13 || show_mix23;
    bool show_deltacp = (deltacp > rndmz);
    bool show_dm12sq  = (dm12sq  > rndmz);
    bool show_dm23sq  = (dm23sq  > rndmz);
    bool show_dmsq    = show_dm12sq || show_dm23sq;
    bool show_betabar = (betabar > rndmz);
    
    if(show_mix)
    {
      out << mixparam << "[";
      if(show_mix12) {
        out << "12_" << Form("%.3f",mix12);
      }
      if(show_mix13) {
        if(show_mix12) {
          out << ",";
        }
        out << "13_" << Form("%.3f",mix13);
      }
      if(show_mix23) {
        if(show_mix13 || (!show_mix13 && show_mix12)) {
          out << ",";
        }
        out << "23_" << Form("%.3f",mix23);
      }
      out << "]";
    }
    
    if(show_mix && (show_deltacp || show_dmsq))
    {
      out << ",";
    }
    
    if(show_deltacp)
    {
      out << "dcp_" << Form("%.3f",deltacp);
    }
    
    if(show_deltacp && show_dmsq)
    {
      out << ",";
    }
    
    if(show_dmsq)
    {
      out << "dmsq_" << util::AsString(Opt::Instance()->gMassHierarchyAssumed) << "[";
      if(show_dm12sq) {
        out << "12_" << Form("%.8f",dm12sq);
      }
      if(show_dm23sq) {
        if(show_dm12sq) {
          out << ",";
        }
        out << "23_" << Form("%.6f",dm23sq);
      }
      out << "]eV2";
    }
    
    if(show_betabar)
    {
      out << ",betabar_" << Form("%.3f",betabar);
    }
    
  }//3f_normbar
  
  //
  
  // Placeholder for steriles
  if (hyp == valor::conv::kHypOscSterile4F) {
    out<<"sterile_3+1";
  }
  if (hyp == valor::conv::kHypOscSterile5F) {
    out<<"sterile_3+2";
  }
  
  //
  // 2-flavour oscillations, numu disappearance
  // Build the string as mix23_nnn,dm23sq_nnneV2
  // where mix is either sinsqtheta or sinsq2theta and nnn is the parameter
  // value with the specified numerical accuracy.
  //
  if( hyp == valor::conv::kHypNuMuDisapp2F)
  {
    out << mixparam << "23_" << Form("%.3f",mix23) << ",dm23sq_" << Form("%.5f",dm23sq) << "eV2";
  }
  
  return out.str();
}
//

//________________________________________________________________________________________
PhysHypothesis_t util::PhysHypFromString (string sh)
{
  if(sh == "decay"            ) return kHypDecay;
  if(sh == "decoh"            ) return kHypDecoh;
  if(sh == "2f_numu_disap"    ) return kHypNuMuDisapp2F;
  if(sh == "3f_osc"           ) return kHypOsc3F;
  if(sh == "3f_oscbar"        ) return kHypOsc3FBar;
  if(sh == "3f_oscbar_dissap" ) return kHypOsc3FBarDissap;
  
  // NEW BETABAR
  if(sh == "3f_oscbar_normbar") return kHypOsc3FBarNormBar;
  //
  // NEW BETABAR 2
  if(sh == "3f_osc_normbar") return kHypOsc3FNormBar;
  //
  
  if(sh == "3f_nsi"        ) return kHypNSI3F;
  if(sh == "4f_sterile_osc") return kHypOscSterile4F;
  if(sh == "5f_sterile_osc") return kHypOscSterile5F;
  if(sh == "void"          ) return kHypVoid;
  
  return kHypUndefined;
}
//________________________________________________________________________________________


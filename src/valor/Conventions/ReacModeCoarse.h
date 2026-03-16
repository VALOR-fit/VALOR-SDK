//____________________________________________________________________________
/*!

\class     valor::conv::ReacModeCoarse_t

\brief     Enumeration of coarse true reaction modes.
           Not a systematic characterization and not mutually exlusive.
           These codes are used only in plots.
           One or more ReacMode_t types are mapped into a ReacModeCoarse_t.

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   April 15, 2014

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_dune_coarse_reaction_modes_h
#define _valor_dune_coarse_reaction_modes_h

#include <string>
#include <unordered_map>
#include <array>
#include "valor/Conventions/ReacMode.h"

using std::string;

namespace valor {
namespace conv {

// Coarse reaction modes are going to match the GENIE interaction mode (roughly)
// 
typedef enum EReacModeCoarse
{
  kmcUndefined      = -999,
  kmcNuMuCCQE       =   1,          // numu     CC QE
  kmcNuMuCCRES      =   2,          // numu     CC RES (RES and RSPP in GENIE speak)
  kmcNuMuCCDIS      =   3,          // numu     CC DIS
  kmcNuMuCCCOH      =   4,          // numu     CC COH
  kmcNuMuCCMEC      =   5,          // numu     CC MEC
  kmcNuMuNCQE       =   6,          // numu     NC QE
  kmcNuMuNCRES      =   7,          // numu     NC RES
  kmcNuMuNCDIS      =   8,          // numu     NC DIS
  kmcNuMuNCCOH      =   9,          // numu     NC COH
  kmcNuMuNCMEC      =  10,          // numu     NC MEC
  kmcNuMuBarCCQE    = - 1,          // numubar  CC QE
  kmcNuMuBarCCRES   = - 2,          // numubar  CC RES (RES and RSPP in GENIE speak)
  kmcNuMuBarCCDIS   = - 3,          // numubar  CC DIS
  kmcNuMuBarCCCOH   = - 4,          // numubar  CC COH
  kmcNuMuBarCCMEC   = - 5,          // numubar  CC MEC
  kmcNuMuBarNCQE    = - 6,          // numubar  NC QE
  kmcNuMuBarNCRES   = - 7,          // numubar  NC RES
  kmcNuMuBarNCDIS   = - 8,          // numubar  NC DIS
  kmcNuMuBarNCCOH   = - 9,          // numubar  NC COH
  kmcNuMuBarNCMEC   = -10,          // numubar  NC MEC
  kmcNuECCQE        =  11,          // nue      CC QE
  kmcNuECCRES       =  12,          // nue      CC RES (RES and RSPP in GENIE speak)
  kmcNuECCDIS       =  13,          // nue      CC DIS
  kmcNuECCCOH       =  14,          // nue      CC COH
  kmcNuECCMEC       =  15,          // nue      CC MEC
  kmcNuENCQE        =  16,          // nue      NC QE
  kmcNuENCRES       =  17,          // nue      NC RES
  kmcNuENCDIS       =  18,          // nue      NC DIS
  kmcNuENCCOH       =  19,          // nue      NC COH
  kmcNuENCMEC       =  20,          // nue      NC MEC
  kmcNuEBarCCQE     = -11,          // nuebar   CC QE
  kmcNuEBarCCRES    = -12,          // nuebar   CC RES (RES and RSPP in GENIE speak)
  kmcNuEBarCCDIS    = -13,          // nuebar   CC DIS
  kmcNuEBarCCCOH    = -14,          // nuebar   CC COH
  kmcNuEBarCCMEC    = -15,          // nuebar   CC MEC
  kmcNuEBarNCQE     = -16,          // nuebar   NC QE
  kmcNuEBarNCRES    = -17,          // nuebar   NC RES
  kmcNuEBarNCDIS    = -18,          // nuebar   NC DIS
  kmcNuEBarNCCOH    = -19,          // nuebar   NC COH
  kmcNuEBarNCMEC    = -20,          // nuebar   NC MEC
  kmcCohElastic     = 100,          // any coh elastic
  kmcElScat         = 101,          // any electron scattering
  kmcIMDAnh         = 102,          // any inverse muon decay annihilation
  kmcIBD            = 103,          // any inverse beta decay
  kmcGlashow        = 104,          // any Glashow resonance
  kmcAMNuGamma      = 105,          // any anomaly-mediated v+gamma
  kmcDiffractive    = 106,          // any diffractive (COH on H)
  kmcEM             = 107,          // any EM 
  kmcWeakMix        = 108,          // any weak mix
  kmcCosmic         = 109,          // cosmic
  kmcDirt           = 110,          // dirt
} ReacModeCoarse_t;

//static const int kNReacModesCoarse = (int)kmcDirt + 1;
static const int kNReacModesCoarse = 52;
 
// An array showing which ReacMode_ts go into which ReacModeCoarse_t (in order of priority
// Keeping as an array of 1 element for now, it can be extended.
static const int kMaxCoarseReacOptions = 1;

static std::unordered_map< ReacMode_t, 
  std::array<ReacModeCoarse_t, kMaxCoarseReacOptions> > kCoarseReacPriority =
  {
    { kmNuMuCCQE,         { kmcNuMuCCQE     } },
    { kmNuMuCCRESpPiP,    { kmcNuMuCCRES    } },
    { kmNuMuCCRESnPi0,    { kmcNuMuCCRES    } },
    { kmNuMuCCRESnPiP,    { kmcNuMuCCRES    } },
    { kmNuMuCCRESSPP,     { kmcNuMuCCRES    } },
    { kmNuMuCCDIS,        { kmcNuMuCCDIS    } },
    { kmNuMuCCCOH,        { kmcNuMuCCCOH    } },
    { kmNuMuCCMEC,        { kmcNuMuCCMEC    } },
    { kmNuMuNCQE,         { kmcNuMuNCQE     } },
    { kmNuMuNCRESpPi0,    { kmcNuMuNCRES    } },
    { kmNuMuNCRESpPiP,    { kmcNuMuNCRES    } },
    { kmNuMuNCRESnPi0,    { kmcNuMuNCRES    } },
    { kmNuMuNCRESnPiM,    { kmcNuMuNCRES    } },
    { kmNuMuNCRESSPP,     { kmcNuMuNCRES    } },
    { kmNuMuNCDIS,        { kmcNuMuNCDIS    } },
    { kmNuMuNCCOH,        { kmcNuMuNCCOH    } },
    { kmNuMuNCMEC,        { kmcNuMuNCMEC    } },
    { kmNuMuBarCCQE,      { kmcNuMuBarCCQE  } },
    { kmNuMuBarCCRESnPiM, { kmcNuMuBarCCRES } },
    { kmNuMuBarCCRESpPi0, { kmcNuMuBarCCRES } },
    { kmNuMuBarCCRESpPiM, { kmcNuMuBarCCRES } },
    { kmNuMuBarCCRESSPP,  { kmcNuMuBarCCRES } },
    { kmNuMuBarCCDIS,     { kmcNuMuBarCCDIS } },
    { kmNuMuBarCCCOH,     { kmcNuMuBarCCCOH } },
    { kmNuMuBarCCMEC,     { kmcNuMuBarCCMEC } },
    { kmNuMuBarNCQE,      { kmcNuMuBarNCQE  } },
    { kmNuMuBarNCRESpPi0, { kmcNuMuBarNCRES } },
    { kmNuMuBarNCRESpPiP, { kmcNuMuBarNCRES } },
    { kmNuMuBarNCRESnPi0, { kmcNuMuBarNCRES } },
    { kmNuMuBarNCRESnPiM, { kmcNuMuBarNCRES } },
    { kmNuMuBarNCRESSPP,  { kmcNuMuBarNCRES } },
    { kmNuMuBarNCDIS,     { kmcNuMuBarNCDIS } },
    { kmNuMuBarNCCOH,     { kmcNuMuBarNCCOH } },
    { kmNuMuBarNCMEC,     { kmcNuMuBarNCMEC } },
    { kmNuECCQE,          { kmcNuECCQE      } },
    { kmNuECCRESpPiP,     { kmcNuECCRES     } },
    { kmNuECCRESnPi0,     { kmcNuECCRES     } },
    { kmNuECCRESnPiP,     { kmcNuECCRES     } },
    { kmNuECCRESSPP,      { kmcNuECCRES     } },
    { kmNuECCDIS,         { kmcNuECCDIS     } },
    { kmNuECCCOH,         { kmcNuECCCOH     } },
    { kmNuECCMEC,         { kmcNuECCMEC     } },
    { kmNuENCQE,          { kmcNuENCQE      } },
    { kmNuENCRESpPi0,     { kmcNuENCRES     } },
    { kmNuENCRESpPiP,     { kmcNuENCRES     } },
    { kmNuENCRESnPi0,     { kmcNuENCRES     } },
    { kmNuENCRESnPiM,     { kmcNuENCRES     } },
    { kmNuENCRESSPP,      { kmcNuENCRES     } },
    { kmNuENCDIS,         { kmcNuENCDIS     } },
    { kmNuENCCOH,         { kmcNuENCCOH     } },
    { kmNuENCMEC,         { kmcNuENCMEC     } },
    { kmNuEBarCCQE,       { kmcNuEBarCCQE   } },
    { kmNuEBarCCRESnPiM,  { kmcNuEBarCCRES  } },
    { kmNuEBarCCRESpPi0,  { kmcNuEBarCCRES  } },
    { kmNuEBarCCRESpPiM,  { kmcNuEBarCCRES  } },
    { kmNuEBarCCRESSPP,   { kmcNuEBarCCRES  } },
    { kmNuEBarCCDIS,      { kmcNuEBarCCDIS  } },
    { kmNuEBarCCCOH,      { kmcNuEBarCCCOH  } },
    { kmNuEBarCCMEC,      { kmcNuEBarCCMEC  } },
    { kmNuEBarNCQE,       { kmcNuEBarNCQE   } },
    { kmNuEBarNCRESpPi0,  { kmcNuEBarNCRES  } },
    { kmNuEBarNCRESpPiP,  { kmcNuEBarNCRES  } },
    { kmNuEBarNCRESnPi0,  { kmcNuEBarNCRES  } },
    { kmNuEBarNCRESnPiM,  { kmcNuEBarNCRES  } },
    { kmNuEBarNCRESSPP,   { kmcNuEBarNCRES  } },
    { kmNuEBarNCDIS,      { kmcNuEBarNCDIS  } },
    { kmNuEBarNCCOH,      { kmcNuEBarNCCOH  } },
    { kmNuEBarNCMEC,      { kmcNuEBarNCMEC  } },
    { kmNuMuToNuECCQE,         { kmcNuECCQE     } },
    { kmNuMuToNuECCRESpPiP,    { kmcNuECCRES    } },
    { kmNuMuToNuECCRESnPi0,    { kmcNuECCRES    } },
    { kmNuMuToNuECCRESnPiP,    { kmcNuECCRES    } },
    { kmNuMuToNuECCRESSPP,     { kmcNuECCRES    } },
    { kmNuMuToNuECCDIS,        { kmcNuECCDIS    } },
    { kmNuMuToNuECCCOH,        { kmcNuECCCOH    } },
    { kmNuMuToNuECCMEC,        { kmcNuECCMEC    } },
    { kmNuMuToNuENCQE,         { kmcNuENCQE     } },
    { kmNuMuToNuENCRESpPi0,    { kmcNuENCRES    } },
    { kmNuMuToNuENCRESpPiP,    { kmcNuENCRES    } },
    { kmNuMuToNuENCRESnPi0,    { kmcNuENCRES    } },
    { kmNuMuToNuENCRESnPiM,    { kmcNuENCRES    } },
    { kmNuMuToNuENCRESSPP,     { kmcNuENCRES    } },
    { kmNuMuToNuENCDIS,        { kmcNuENCDIS    } },
    { kmNuMuToNuENCCOH,        { kmcNuENCCOH    } },
    { kmNuMuToNuENCMEC,        { kmcNuENCMEC    } },
    { kmNuMuToNuTauCCQE,         { kmcUndefined } },
    { kmNuMuToNuTauCCRESpPiP,    { kmcUndefined } },
    { kmNuMuToNuTauCCRESnPi0,    { kmcUndefined } },
    { kmNuMuToNuTauCCRESnPiP,    { kmcUndefined } },
    { kmNuMuToNuTauCCRESSPP,     { kmcUndefined } },
    { kmNuMuToNuTauCCDIS,        { kmcUndefined } },
    { kmNuMuToNuTauCCCOH,        { kmcUndefined } },
    { kmNuMuToNuTauCCMEC,        { kmcUndefined } },
    { kmNuMuToNuTauNCQE,         { kmcUndefined } },
    { kmNuMuToNuTauNCRESpPi0,    { kmcUndefined } },
    { kmNuMuToNuTauNCRESpPiP,    { kmcUndefined } },
    { kmNuMuToNuTauNCRESnPi0,    { kmcUndefined } },
    { kmNuMuToNuTauNCRESnPiM,    { kmcUndefined } },
    { kmNuMuToNuTauNCRESSPP,     { kmcUndefined } },
    { kmNuMuToNuTauNCDIS,        { kmcUndefined } },
    { kmNuMuToNuTauNCCOH,        { kmcUndefined } },
    { kmNuMuToNuTauNCMEC,        { kmcUndefined } },
    { kmNuMuBarToNuEBarCCQE,      { kmcNuEBarCCQE  } },
    { kmNuMuBarToNuEBarCCRESnPiM, { kmcNuEBarCCRES } },
    { kmNuMuBarToNuEBarCCRESpPi0, { kmcNuEBarCCRES } },
    { kmNuMuBarToNuEBarCCRESpPiM, { kmcNuEBarCCRES } },
    { kmNuMuBarToNuEBarCCRESSPP,  { kmcNuEBarCCRES } },
    { kmNuMuBarToNuEBarCCDIS,     { kmcNuEBarCCDIS } },
    { kmNuMuBarToNuEBarCCCOH,     { kmcNuEBarCCCOH } },
    { kmNuMuBarToNuEBarCCMEC,     { kmcNuEBarCCMEC } },
    { kmNuMuBarToNuEBarNCQE,      { kmcNuEBarNCQE  } },
    { kmNuMuBarToNuEBarNCRESpPi0, { kmcNuEBarNCRES } },
    { kmNuMuBarToNuEBarNCRESpPiP, { kmcNuEBarNCRES } },
    { kmNuMuBarToNuEBarNCRESnPi0, { kmcNuEBarNCRES } },
    { kmNuMuBarToNuEBarNCRESnPiM, { kmcNuEBarNCRES } },
    { kmNuMuBarToNuEBarNCRESSPP,  { kmcNuEBarNCRES } },
    { kmNuMuBarToNuEBarNCDIS,     { kmcNuEBarNCDIS } },
    { kmNuMuBarToNuEBarNCCOH,     { kmcNuEBarNCCOH } },
    { kmNuMuBarToNuEBarNCMEC,     { kmcNuEBarNCMEC } },
    { kmNuMuBarToNuTauBarCCQE,      { kmcUndefined } },
    { kmNuMuBarToNuTauBarCCRESnPiM, { kmcUndefined } },
    { kmNuMuBarToNuTauBarCCRESpPi0, { kmcUndefined } },
    { kmNuMuBarToNuTauBarCCRESpPiM, { kmcUndefined } },
    { kmNuMuBarToNuTauBarCCRESSPP,  { kmcUndefined } },
    { kmNuMuBarToNuTauBarCCDIS,     { kmcUndefined } },
    { kmNuMuBarToNuTauBarCCCOH,     { kmcUndefined } },
    { kmNuMuBarToNuTauBarCCMEC,     { kmcUndefined } },
    { kmNuMuBarToNuTauBarNCQE,      { kmcUndefined } },
    { kmNuMuBarToNuTauBarNCRESpPi0, { kmcUndefined } },
    { kmNuMuBarToNuTauBarNCRESpPiP, { kmcUndefined } },
    { kmNuMuBarToNuTauBarNCRESnPi0, { kmcUndefined } },
    { kmNuMuBarToNuTauBarNCRESnPiM, { kmcUndefined } },
    { kmNuMuBarToNuTauBarNCRESSPP,  { kmcUndefined } },
    { kmNuMuBarToNuTauBarNCDIS,     { kmcUndefined } },
    { kmNuMuBarToNuTauBarNCCOH,     { kmcUndefined } },
    { kmNuMuBarToNuTauBarNCMEC,     { kmcUndefined } },
    { kmNuEToNuMuCCQE,         { kmcNuMuCCQE     } },
    { kmNuEToNuMuCCRESpPiP,    { kmcNuMuCCRES    } },
    { kmNuEToNuMuCCRESnPi0,    { kmcNuMuCCRES    } },
    { kmNuEToNuMuCCRESnPiP,    { kmcNuMuCCRES    } },
    { kmNuEToNuMuCCRESSPP,     { kmcNuMuCCRES    } },
    { kmNuEToNuMuCCDIS,        { kmcNuMuCCDIS    } },
    { kmNuEToNuMuCCCOH,        { kmcNuMuCCCOH    } },
    { kmNuEToNuMuCCMEC,        { kmcNuMuCCMEC    } },
    { kmNuEToNuMuNCQE,         { kmcNuMuNCQE     } },
    { kmNuEToNuMuNCRESpPi0,    { kmcNuMuNCRES    } },
    { kmNuEToNuMuNCRESpPiP,    { kmcNuMuNCRES    } },
    { kmNuEToNuMuNCRESnPi0,    { kmcNuMuNCRES    } },
    { kmNuEToNuMuNCRESnPiM,    { kmcNuMuNCRES    } },
    { kmNuEToNuMuNCRESSPP,     { kmcNuMuNCRES    } },
    { kmNuEToNuMuNCDIS,        { kmcNuMuNCDIS    } },
    { kmNuEToNuMuNCCOH,        { kmcNuMuNCCOH    } },
    { kmNuEToNuMuNCMEC,        { kmcNuMuNCMEC    } },
    { kmNuEToNuTauCCQE,         { kmcUndefined } },
    { kmNuEToNuTauCCRESpPiP,    { kmcUndefined } },
    { kmNuEToNuTauCCRESnPi0,    { kmcUndefined } },
    { kmNuEToNuTauCCRESnPiP,    { kmcUndefined } },
    { kmNuEToNuTauCCRESSPP,     { kmcUndefined } },
    { kmNuEToNuTauCCDIS,        { kmcUndefined } },
    { kmNuEToNuTauCCCOH,        { kmcUndefined } },
    { kmNuEToNuTauCCMEC,        { kmcUndefined } },
    { kmNuEToNuTauNCQE,         { kmcUndefined } },
    { kmNuEToNuTauNCRESpPi0,    { kmcUndefined } },
    { kmNuEToNuTauNCRESpPiP,    { kmcUndefined } },
    { kmNuEToNuTauNCRESnPi0,    { kmcUndefined } },
    { kmNuEToNuTauNCRESnPiM,    { kmcUndefined } },
    { kmNuEToNuTauNCRESSPP,     { kmcUndefined } },
    { kmNuEToNuTauNCDIS,        { kmcUndefined } },
    { kmNuEToNuTauNCCOH,        { kmcUndefined } },
    { kmNuEToNuTauNCMEC,        { kmcUndefined } },
    { kmNuEBarToNuMuBarCCQE,      { kmcNuMuBarCCQE  } },
    { kmNuEBarToNuMuBarCCRESnPiM, { kmcNuMuBarCCRES } },
    { kmNuEBarToNuMuBarCCRESpPi0, { kmcNuMuBarCCRES } },
    { kmNuEBarToNuMuBarCCRESpPiM, { kmcNuMuBarCCRES } },
    { kmNuEBarToNuMuBarCCRESSPP,  { kmcNuMuBarCCRES } },
    { kmNuEBarToNuMuBarCCDIS,     { kmcNuMuBarCCDIS } },
    { kmNuEBarToNuMuBarCCCOH,     { kmcNuMuBarCCCOH } },
    { kmNuEBarToNuMuBarCCMEC,     { kmcNuMuBarCCMEC } },
    { kmNuEBarToNuMuBarNCQE,      { kmcNuMuBarNCQE  } },
    { kmNuEBarToNuMuBarNCRESpPi0, { kmcNuMuBarNCRES } },
    { kmNuEBarToNuMuBarNCRESpPiP, { kmcNuMuBarNCRES } },
    { kmNuEBarToNuMuBarNCRESnPi0, { kmcNuMuBarNCRES } },
    { kmNuEBarToNuMuBarNCRESnPiM, { kmcNuMuBarNCRES } },
    { kmNuEBarToNuMuBarNCRESSPP,  { kmcNuMuBarNCRES } },
    { kmNuEBarToNuMuBarNCDIS,     { kmcNuMuBarNCDIS } },
    { kmNuEBarToNuMuBarNCCOH,     { kmcNuMuBarNCCOH } },
    { kmNuEBarToNuMuBarNCMEC,     { kmcNuMuBarNCMEC } },
    { kmNuEBarToNuTauBarCCQE,      { kmcUndefined } },
    { kmNuEBarToNuTauBarCCRESnPiM, { kmcUndefined } },
    { kmNuEBarToNuTauBarCCRESpPi0, { kmcUndefined } },
    { kmNuEBarToNuTauBarCCRESpPiM, { kmcUndefined } },
    { kmNuEBarToNuTauBarCCRESSPP,  { kmcUndefined } },
    { kmNuEBarToNuTauBarCCDIS,     { kmcUndefined } },
    { kmNuEBarToNuTauBarCCCOH,     { kmcUndefined } },
    { kmNuEBarToNuTauBarCCMEC,     { kmcUndefined } },
    { kmNuEBarToNuTauBarNCQE,      { kmcUndefined } },
    { kmNuEBarToNuTauBarNCRESpPi0, { kmcUndefined } },
    { kmNuEBarToNuTauBarNCRESpPiP, { kmcUndefined } },
    { kmNuEBarToNuTauBarNCRESnPi0, { kmcUndefined } },
    { kmNuEBarToNuTauBarNCRESnPiM, { kmcUndefined } },
    { kmNuEBarToNuTauBarNCRESSPP,  { kmcUndefined } },
    { kmNuEBarToNuTauBarNCDIS,     { kmcUndefined } },
    { kmNuEBarToNuTauBarNCCOH,     { kmcUndefined } },
    { kmNuEBarToNuTauBarNCMEC,     { kmcUndefined } },
    { kmCohElastic,                { kmcCohElastic } },
    { kmElScat,                    { kmcElScat } },
    { kmIMDAnh,                    { kmcIMDAnh } },
    { kmIBD,                       { kmcIBD } },
    { kmGlashow,                   { kmcGlashow } },
    { kmAMNuGamma,                 { kmcAMNuGamma } },
    { kmDiffractive,               { kmcDiffractive } },
    { kmEM,                        { kmcEM } },
    { kmWeakMix,                   { kmcWeakMix } },
    { kmCosmic,                    { kmcCosmic } },
    { kmDirt,                      { kmcDirt } }
  };

} // conv

//
// Associated utilities
//
namespace util {

  string AsString    (conv::ReacModeCoarse_t m);
  string AsStringTex (conv::ReacModeCoarse_t m, bool root);

  // python aliases
  string CoarseModeAsString(conv::ReacModeCoarse_t m);

  conv::ReacModeCoarse_t CoarseModeFromString (std::string s);

  // Is the given reac mode contained within the given coarse reac mode?
  bool ModeWithinCoarse(conv::ReacMode_t m, conv::ReacModeCoarse_t c);
} // util

} // valor
  
#endif 


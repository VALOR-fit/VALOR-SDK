//____________________________________________________________________________
/*!

\class     valor::conv::Sample_t

\brief     Enumeration of DUNE event samples

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           Steve Dennis <s.r.dennis \at liverpool.ac.uk>
           University of Liverpool

\created   March 20, 2014

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_sample_h_
#define _valor_sample_h_

#include <string>

#include "valor/Conventions/BeamConf.h"

namespace valor {
namespace conv {

// ...........................................................................
// Define samples
//
typedef enum ESampleOld
{
   //2016b near detector selections
   ksOldUndefined       = -1,
   ksNuMuCC          =  0, // numu CC inclusive - disabled in 2016b, split in subsamples
   ksNuMuCC0pi1trk   =  1, // numu CC 0 pions - 1 track (mu)
   ksNuMuCC0pi2trk   =  2, // numu CC 0 pions - 2 tracks (mu + proton)
   ksNuMuCC1piC      =  3, // numu CC 1 pion+/-  (mu + 1 pi+/- + X) and not included in the Delta enhanced selection (code 12)
   ksNuMuCC2piC      =  4, // numu CC 2 pions+/- (mu + 2 pi+/- + X) - disabled in 2016b
   ksNuMuCC1pi0      =  5, // numu CC 1 pion0  (mu + 1 pi0 + X)
   ksNuMuCC1piC1pi0  =  6, // numu CC 1 pion+/- & 1 pion0 (mu + 1 pi+/- + 1pi0 + X)
   ksNuMuCCoth       =  7, // numu CC - other
   ksNuECC           =  8, // nue CC inclusive - disabled in 2016b, split in subsamples
   ksNuMuCCWS        =  9, // wrong sign numu CC inclusive - disabled in 2016b, split in subsamples
   ksNC              = 10, // NC inclusive - disabled in 2016b, split in subsamples
   ksNuMuCC0piNtrk   = 11, // numu CC - 0 pions - N tracks (mu + >1 nucleons)
   ksNuMuCC3trkDelta = 12, // numu CC - 3 track Delta-enhanched (mu + proton + pi+, Wreco ~1.2 GeV)
   ksNuMuCCWS0pi     = 13, // wrong sign numu CC - 0 pions (mu+ + X )
   ksNuMuCCWS1piC    = 14, // wrong sign numu CC - 1 pion+/- (mu+ + 1 pi+/- + X)
   ksNuMuCCWS1pi0    = 15, // wrong sign numu CC - 1 pion0 (mu+ + 1 pi0 + X)
   ksNuMuCCWSoth     = 16, // wrong sign numu CC - other
   ksNuECC0pi        = 17, // nue CC - 0 pions (e + X)
   ksNuECC1piC       = 18, // nue CC - 1 pion+/- (e + 1 pi+/- + X)
   ksNuECC1pi0       = 19, // nue CC - 1 pion0 (e + 1 pi0 + X)
   ksNuECCoth        = 20, // nue CC - other
   ksNC0pi           = 21, // NC - 0 pions (nucleons)
   ksNC1piC          = 22, // NC - 1 pion+/- (1 pi+/- + X)
   ksNC1pi0          = 23, // NC - 1 pion0 (1 pi0 + X)
   ksNCoth           = 24, // NC - other
   ksNeutrinoElec    = 25, // Neutrino-electron elastic scattering nu + e
   ksInvMuDecay      = 26, // Inverse muon decay (nuebar + e -> mu + numubar or numu + e -> mu + nue)
   ksNuECCWS         = 27, // wrong sign nue CC inclusive
   //2016b far detector selections
   ksNuMuLikeCChigh      , // numu-like (for the moment both numu and numubar), high resolution (CCQE-like)
   ksNuMuLikeCClow       , // numu-like (for the moment both numu and numubar), low resolution (nonCCQE-like)
   ksNuMuBarCChigh       , // numubar, high resolution (CCQE-like)   - CURRENTLY UNUSED
   ksNuMuBarCClow        , // numubar, low resolution (nonCCQE-like) - CURRENTLY UNUSED
   ksNuELikeCChigh       , // nue-like (for the moment both nue and nuebar), high resolution (CCQE-like)
   ksNuELikeCClow        , // nue-like (for the moment both nue and nuebar), low resolution (nonCCQE-like)
   ksNuEBarCChigh        , // nuebar, high resolution (CCQE-like)    - CURRENTLY UNUSED
   ksNuEBarCClow         , // nuebar, low resolution (nonCCQE-like)  - CURRENTLY UNUSED
   // SuperK selections
   ks1Rmu                , // Single-ring muon-like
   ks1Re                 , // Single-ring electron-like
   ksMultiRe             , // Multi-ring electron-like
   // PRISM selections
   ksNuMuCCoa00          , // numuCC-like, off-axis bin "00" (arbitrary definition of off-axis bin: specified by range of off-axis angles in input DST)
   ksNuMuCCoa01          , // numuCC-like, off-axis bin "01"
   ksNuMuCCoa02          , // numuCC-like, off-axis bin "02"
   ksNuMuCCoa03          , // numuCC-like, off-axis bin "03"
   ksNuMuCCoa04          , // numuCC-like, off-axis bin "04"
   ksNuMuCCoa05          , // numuCC-like, off-axis bin "05"
   ksNuMuCCoa06          , // numuCC-like, off-axis bin "06"
   ksNuMuCCoa07          , // numuCC-like, off-axis bin "07"
   ksNuMuCCoa08          , // numuCC-like, off-axis bin "08"
   ksNuMuCCoa09          , // numuCC-like, off-axis bin "09"
   ksNuECCoa00           , // nueCC-like, off-axis bin "00" (arbitrary definition of off-axis bin: specified by range of off-axis angles in input DST)
   ksNuECCoa01           , // nueCC-like, off-axis bin "01"
   ksNuECCoa02           , // nueCC-like, off-axis bin "02"
   ksNuECCoa03           , // nueCC-like, off-axis bin "03"
   ksNuECCoa04           , // nueCC-like, off-axis bin "04"
   ksNuECCoa05           , // nueCC-like, off-axis bin "05"
   ksNuECCoa06           , // nueCC-like, off-axis bin "06"
   ksNuECCoa07           , // nueCC-like, off-axis bin "07"
}
SampleOld_t; 

// I define new samples.
// Each is going to have up to 8 OAA bins + an inclusive
// Far detector samples don't have off axis bins.
typedef enum ESample {
  ksUndefined       = -1,
  ksNearNuMuCC      = 0, // numu CC inclusive
  ksNearNuMuCCOAA1  = 1, // 0th OAA bin
  ksNearNuMuCCOAA2  = 2, // 1st OAA bin
  ksNearNuMuCCOAA3  = 3, // 2nd OAA bin
  ksNearNuMuCCOAA4  = 4, // 3rd OAA bin
  ksNearNuMuCCOAA5  = 5, // 4th OAA bin
  ksNearNuMuCCOAA6  = 6, // 5th OAA bin
  ksNearNuMuCCOAA7  = 7, // 6th OAA bin
  ksNearNuMuCCOAA8  = 8, // 7th OAA bin
  ksNearNuMuCCOAA9  = 9, // 8th OAA bin
  ksNearNuMuCC_0p0pi      = 10, // numu CC, 0p0pi final state
  ksNearNuMuCC_0p0piOAA1  = 11, 
  ksNearNuMuCC_0p0piOAA2  = 12, 
  ksNearNuMuCC_0p0piOAA3  = 13, 
  ksNearNuMuCC_0p0piOAA4  = 14, 
  ksNearNuMuCC_0p0piOAA5  = 15, 
  ksNearNuMuCC_0p0piOAA6  = 16, 
  ksNearNuMuCC_0p0piOAA7  = 17, 
  ksNearNuMuCC_0p0piOAA8  = 18, 
  ksNearNuMuCC_0p0piOAA9  = 19, 
  ksNearNuMuCC_1p0pi      = 20, // numu CC, 1p0pi final state
  ksNearNuMuCC_1p0piOAA1  = 21, 
  ksNearNuMuCC_1p0piOAA2  = 22, 
  ksNearNuMuCC_1p0piOAA3  = 23, 
  ksNearNuMuCC_1p0piOAA4  = 24, 
  ksNearNuMuCC_1p0piOAA5  = 25, 
  ksNearNuMuCC_1p0piOAA6  = 26, 
  ksNearNuMuCC_1p0piOAA7  = 27, 
  ksNearNuMuCC_1p0piOAA8  = 28, 
  ksNearNuMuCC_1p0piOAA9  = 29, 
  ksNearNuMuCC_Np0pi      = 30, // numu CC, Np0pi final state
  ksNearNuMuCC_Np0piOAA1  = 31, 
  ksNearNuMuCC_Np0piOAA2  = 32, 
  ksNearNuMuCC_Np0piOAA3  = 33, 
  ksNearNuMuCC_Np0piOAA4  = 34, 
  ksNearNuMuCC_Np0piOAA5  = 35, 
  ksNearNuMuCC_Np0piOAA6  = 36, 
  ksNearNuMuCC_Np0piOAA7  = 37, 
  ksNearNuMuCC_Np0piOAA8  = 38, 
  ksNearNuMuCC_Np0piOAA9  = 39, 
  ksNearNuMuCC_1pi      = 40, // numu CC, 1pi final state
  ksNearNuMuCC_1piOAA1  = 41, 
  ksNearNuMuCC_1piOAA2  = 42, 
  ksNearNuMuCC_1piOAA3  = 43, 
  ksNearNuMuCC_1piOAA4  = 44, 
  ksNearNuMuCC_1piOAA5  = 45, 
  ksNearNuMuCC_1piOAA6  = 46, 
  ksNearNuMuCC_1piOAA7  = 47, 
  ksNearNuMuCC_1piOAA8  = 48, 
  ksNearNuMuCC_1piOAA9  = 49, 
  ksNearNuMuCC_Other      = 50, // numu CC, Other final state
  ksNearNuMuCC_OtherOAA1  = 51, 
  ksNearNuMuCC_OtherOAA2  = 52, 
  ksNearNuMuCC_OtherOAA3  = 53, 
  ksNearNuMuCC_OtherOAA4  = 54, 
  ksNearNuMuCC_OtherOAA5  = 55, 
  ksNearNuMuCC_OtherOAA6  = 56, 
  ksNearNuMuCC_OtherOAA7  = 57, 
  ksNearNuMuCC_OtherOAA8  = 58, 
  ksNearNuMuCC_OtherOAA9  = 59, 
  ksFarNuMuCC     = 60,
  ksFarNuMuCC_0pi = 61,
  ksFarNuMuCC_1pi = 62,
  ksFarNuMuCC_Npi = 63,
} Sample_t;

static const int kMaxNSamples = 65;

} // conv

namespace util {

  std::string    AsString         (conv::Sample_t s);
  std::string    AsStringTeX      (conv::Sample_t s, bool root = false);
  bool           IsCC             (conv::Sample_t s);
  bool           IsCCQELike       (conv::Sample_t s);
  bool           IsNC             (conv::Sample_t s);
  bool           IsFar            (conv::Sample_t s);
  bool           IsMuLike         (conv::Sample_t s);
  bool           IsELike          (conv::Sample_t s);
  conv::Sample_t SampleFromInt    (int s);
  conv::Sample_t SampleFromString (std::string s);

  // aliases methods
  std::string  SampleAsString    (conv::Sample_t s);
  std::string  SampleAsStringTeX (conv::Sample_t s, bool root = false);

  bool SampleIsCC       (conv::Sample_t s);
  bool SampleIsCCQELike (conv::Sample_t s);
  bool SampleIsNC       (conv::Sample_t s);
  bool SampleIsFar      (conv::Sample_t s);
  bool SampleIsMuLike   (conv::Sample_t s);
  bool SampleIsELike    (conv::Sample_t s);
} // util
} // valor

#endif

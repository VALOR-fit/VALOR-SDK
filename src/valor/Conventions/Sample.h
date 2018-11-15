//____________________________________________________________________________
/*!

\class     valor::conv::Sample_t

\brief     Enumeration of DUNE event samples

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           Steve Dennis <s.r.dennis \at liverpool.ac.uk>
           University of Liverpool

\created   March 20, 2014

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

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
typedef enum ESample 
{
   //2016b near detector selections
   ksUndefined       = -1,
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
   ksNuEBarCClow           // nuebar, low resolution (nonCCQE-like)  - CURRENTLY UNUSED
} 
Sample_t; 

static const int kMaxNSamples = 37;

// 
// ...........................................................................
// Define the list of samples used in present analysis.
//
// Note: The order and position of samples in the array is important for other arrays. Maintain the not remove or re-arrange.
// If you want to remove a sample from the fit, just comment-out the corresponding line and replace with ksUndefined.
// kSample array elements that contain the ksUndefined flag are ignored.
//
// To add a new sample in the fit, do the following:
// - Edit Sample.cxx: Add a new case (for the new sample) is all switch statements and make sure the boundaries defined
//   in SampleFromInt() are correct.
// - Edit SampleReacMode.h: Add an entry for the new sample in each array (to define modes for the new sample etc)
// - Edit SampleKineBinning.h/cxx: Check that an appropriate binning scheme exists, and add a new case (for the new sample)
//   all switch statements.
// - Edit FitParams.h: Add instructions on how to apply systematics for the new sample 
// -
 
//this is the maximum number of selections except the ones not used in 2016a or 2016b analyses
// For 2016b = total except: undefined, numuCC, 2piC, nueCC, NC, wrong numu, and only 4 for FD (either nu in FHC or nubar in RHC)
 static const int kNSamples = 27;

// ---------------------------------------------------------------------
// NOTE:
// This is used to loop over all beam configurations / samples.
// Probably could be removed once we start using the Compound* classes
// This is now for 2016b, should include only the kNSamples above
 static const Sample_t kSample[kNBeamConf][kNSamples] =
{
   // kbLBNF_FHC
   {
     //2016b near detector selections
     ksNuMuCC0pi1trk, 
     ksNuMuCC0pi2trk,
     ksNuMuCC0piNtrk,
     ksNuMuCC3trkDelta,
     ksNuMuCC1piC,
     ksNuMuCC1pi0,
     ksNuMuCC1piC1pi0,
     ksNuMuCCoth,
     ksNuMuCCWS0pi,
     ksNuMuCCWS1piC,
     ksNuMuCCWS1pi0,
     ksNuMuCCWSoth,
     ksNuECC0pi,
     ksNuECC1piC,
     ksNuECC1pi0,
     ksNuECCoth,
     ksNC0pi,
     ksNC1piC,
     ksNC1pi0,
     ksNCoth,
     ksNeutrinoElec,
     ksInvMuDecay,
     ksNuECCWS,
     //2016b far detector selections
     ksNuMuLikeCChigh,  
     ksNuMuLikeCClow,  
     ksNuELikeCChigh,           
     ksNuELikeCClow          
   },

   {
     //2016b near detector selections
     ksNuMuCC0pi1trk, 
     ksNuMuCC0pi2trk,
     ksNuMuCC0piNtrk,
     ksNuMuCC3trkDelta,
     ksNuMuCC1piC,
     ksNuMuCC1pi0,
     ksNuMuCC1piC1pi0,
     ksNuMuCCoth,
     ksNuMuCCWS0pi,
     ksNuMuCCWS1piC,
     ksNuMuCCWS1pi0,
     ksNuMuCCWSoth,
     ksNuECC0pi,
     ksNuECC1piC,
     ksNuECC1pi0,
     ksNuECCoth,
     ksNC0pi,
     ksNC1piC,
     ksNC1pi0,
     ksNCoth,
     ksNeutrinoElec,
     ksInvMuDecay,
     ksNuECCWS,
     //2016b far detector selections //LORENA
     ksNuMuLikeCChigh,   
     ksNuMuLikeCClow,  
     ksNuELikeCChigh,     
     ksNuELikeCClow        
   }
};
// ---------------------------------------------------------------------


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
  conv::Sample_t GetActiveSample  (int ibeam, int isample);
  
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


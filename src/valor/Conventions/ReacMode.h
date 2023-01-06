//____________________________________________________________________________
/*!

\class     valor::conv::ReacMode_t

\brief     Enumeration of true reaction modes considered separately within 
           each DUNE analysis sample

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   April 15, 2014

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_reaction_modes_h
#define _valor_reaction_modes_h

#include <string>

//FWMK
#include "valor/Conventions/NuFlavour.h"

namespace valor {
namespace conv {

static const int kNReacModes = 204; //excluding undefined and other

typedef enum EReacMode 
{
   kmUndefined = -1, 
   kmNuMuCCQE,            // numu    CC QE
   kmNuMuCCMEC,           // numu    CC MEC
   kmNuMuCC1piC,          // numu    CC 1 pi+/-
   kmNuMuCC1pi0,          // numu    CC 1 pi0
   kmNuMuCC2piC,          // numu    CC 2 pi+/-
   kmNuMuCC2pi0,          // numu    CC 2 pi0
   kmNuMuCCpiCpi0,        // numu    CC 1 pi0 + 1 pi+/-
   kmNuMuCCcoh,           // numu    CC coherent
   kmNuMuCCIMD,           // numu + e- -> mu + nue inverse muon decay (IMD) 
   kmNuMuCCELScat,        // numu + e- -> numu + e- CC elastic scattering 
   kmNuMuCCDIS,           // numu    CC Deep Inelastic Scattering
   kmNuMuCCMpi,           // numu    CC multi pi (>1 pion of any charge(s))
   kmNuMuCCmisc,          // numu    CC miscellaneous modes that require separate treatment to CC other
   kmNuMuCCoth,           // numu    CC other
   kmNuMuNCEL,            // numu    NC EL
   kmNuMuNCMEC,           // numu    NC MEC
   kmNuMuNC1piC,          // numu    NC 1 pi+/-
   kmNuMuNC1pi0,          // numu    NC 1 pi0
   kmNuMuNC2piC,          // numu    NC 2 pi+/-
   kmNuMuNC2pi0,          // numu    NC 2 pi0
   kmNuMuNCpiCpi0,        // numu    NC 1 pi0 + 1 pi+/-
   kmNuMuNCcoh,           // numu    NC coherent
   kmNuMuNC1Gamma,        // numu    NC 1 gamma
   kmNuMuNCELScat,        // numu + e- -> numu + e- NC elastic scattering
   kmNuMuNCoth,           // numu    NC other
   kmNuMuBarCCQE,         // numubar CC QE
   kmNuMuBarCCMEC,        // numubar CC MEC
   kmNuMuBarCC1piC,       // numubar CC 1 pi+/-
   kmNuMuBarCC1pi0,       // numubar CC 1 pi0
   kmNuMuBarCC2piC,       // numubar CC 2 pi+/-
   kmNuMuBarCC2pi0,       // numubar CC 2 pi0
   kmNuMuBarCCpiCpi0,     // numubar CC 1 pi0 + 1 pi+/-
   kmNuMuBarCCcoh,        // numubar CC coherent
   kmNuMuBarCCELScat,     // numubar + e- -> numubar + e- CC elastic scattering
   kmNuMuBarCCDIS,        // numubar CC Deep Inelastic Scattering
   kmNuMuBarCCMpi,        // numubar CC multi pi (>1 pion of any charge(s))
   kmNuMuBarCCmisc,       // numubar CC miscellaneous modes that require separate treatment to CC other
   kmNuMuBarCCoth,        // numubar CC other
   kmNuMuBarNCEL,         // numubar NC EL
   kmNuMuBarNCMEC,        // numubar NC MEC
   kmNuMuBarNC1piC,       // numubar NC 1 pi+/-
   kmNuMuBarNC1pi0,       // numubar NC 1 pi0
   kmNuMuBarNC2piC,       // numubar NC 2 pi+/-
   kmNuMuBarNC2pi0,       // numubar NC 2 pi0
   kmNuMuBarNCpiCpi0,     // numubar NC 1 pi0 + 1 pi+/-
   kmNuMuBarNCcoh,        // numubar NC coherent
   kmNuMuBarNC1Gamma,     // numubar NC 1 gamma
   kmNuMuBarNCELScat,     // numubar + e- -> numubar + e- NC elastic scattering
   kmNuMuBarNCoth,        // numubar NC other
   kmNuECCQE,             // nue     CC QE
   kmNuECCMEC,            // nue     CC MEC
   kmNuECC1piC,           // nue     CC 1 pi+/-
   kmNuECC1pi0,           // nue     CC 1 pi0
   kmNuECC2piC,           // nue     CC 2 pi+/-
   kmNuECC2pi0,           // nue     CC 2 pi0
   kmNuECCpiCpi0,         // nue     CC 1 pi0 + 1 pi+/-
   kmNuECCcoh,            // nue     CC coherent
   kmNuECCDIS,            // nue     CC Deep Inelastic Scattering
   kmNuECCMpi,            // nue     CC multi pi (>1 pion of any charge(s))
   kmNuECCmisc,           // nue     CC miscellaneous modes that require separate treatment to CC other
   kmNuECCoth,            // nue     CC other
   kmNuENCEL,             // nue     NC EL
   kmNuENCMEC,            // nue     NC MEC
   kmNuENC1piC,           // nue     NC 1 pi+/-
   kmNuENC1pi0,           // nue     NC 1 pi0
   kmNuENC2piC,           // nue     NC 2 pi+/-
   kmNuENC2pi0,           // nue     NC 2 pi0
   kmNuENCpiCpi0,         // nue     NC 1 pi0 + 1 pi+/-
   kmNuENCcoh,            // nue     NC coherent
   kmNuENC1Gamma,         // nue     NC 1 gamma
   kmNuEELScat,           // nue + e- -> nue + e- NC+CC+interference elastic scattering
   kmNuENCoth,            // nue     NC other
   kmNuEBarCCQE,          // nuebar  CC QE
   kmNuEBarCCMEC,         // nuebar  CC MEC
   kmNuEBarCC1piC,        // nuebar  CC 1 pi+/-
   kmNuEBarCC1pi0,        // nuebar  CC 1 pi0
   kmNuEBarCC2piC,        // nuebar  CC 2 pi+/-
   kmNuEBarCC2pi0,        // nuebar  CC 2 pi0
   kmNuEBarCCpiCpi0,      // nuebar  CC 1 pi0 + 1 pi+/-
   kmNuEBarCCcoh,         // nuebar  CC coherent
   kmNuEBarCCIMD,         // nuebar + e- -> mu + numubar (IMD)
   kmNuEBarCCDIS,         // nuebar  CC Deep Inelastic Scattering
   kmNuEBarCCMpi,         // nuebar  CC multi pi (>1 pion of any charge(s))
   kmNuEBarCCmisc,        // nuebar  CC miscellaneous modes that require separate treatment to CC other
   kmNuEBarCCoth,         // nuebar  CC other
   kmNuEBarNCEL,          // nuebar  NC EL
   kmNuEBarNCMEC,         // nuebar  NC MEC
   kmNuEBarNC1piC,        // nuebar  NC 1 pi+/-
   kmNuEBarNC1pi0,        // nuebar  NC 1 pi0
   kmNuEBarNC2piC,        // nuebar  NC 2 pi+/-
   kmNuEBarNC2pi0,        // nuebar  NC 2 pi0
   kmNuEBarNCpiCpi0,      // nuebar  NC 1 pi0 + 1 pi+/-
   kmNuEBarNCcoh,         // nuebar  NC coherent
   kmNuEBarNC1Gamma,      // nuebar  NC 1 gamma
   kmNuEBarELScat,        // nuebar + e- -> nuebar + e- NC+CC+interference elastic scattering
   kmNuEBarNCoth,         // nuebar  NC other
   //now added the same modes for the oscillated ones
   kmNuMuToNuECCQE,       // numu to nue   CC QE
   kmNuMuToNuECCMEC,      // numu to nue   CC MEC
   kmNuMuToNuECC1piC,     // numu to nue   CC 1 pi+/-
   kmNuMuToNuECC1pi0,     // numu to nue   CC 1 pi0
   kmNuMuToNuECC2piC,     // numu to nue   CC 2 pi+/-
   kmNuMuToNuECC2pi0,     // numu to nue   CC 2 pi0
   kmNuMuToNuECCpiCpi0,   // numu to nue   CC 1 pi0 + 1 pi+/-
   kmNuMuToNuECCcoh,      // numu to nue   CC coherent
   kmNuMuToNuEELScat,     // numu to nue: nue + e- -> nue + e- NC+CC+interference elastic scattering
   kmNuMuToNuECCDIS,      // numu to nue   CC Deep Inelastic Scattering
   kmNuMuToNuECCMpi,      // numu to nue   CC multi pi (>1 pion of any charge(s))
   kmNuMuToNuECCmisc,     // numu to nue   CC miscellaneous modes that require separate treatment to CC other
   kmNuMuToNuECCoth,      // numu to nue   CC other
   kmNuMuToNuTauCCQE,     // numu to nutau    CC QE
   kmNuMuToNuTauCCMEC,    // numu to nutau    CC MEC
   kmNuMuToNuTauCC1piC,   // numu to nutau    CC 1 pi+/-
   kmNuMuToNuTauCC1pi0,   // numu to nutau    CC 1 pi0
   kmNuMuToNuTauCC2piC,   // numu to nutau    CC 2 pi+/-
   kmNuMuToNuTauCC2pi0,   // numu to nutau    CC 2 pi0
   kmNuMuToNuTauCCpiCpi0, // numu to nutau    CC 1 pi0 + 1 pi+/-
   kmNuMuToNuTauCCcoh,    // numu to nutau    CC coherent
   kmNuMuToNuTauCCELScat, // numu to nutau: nutau + e- elastic scattering
   kmNuMuToNuTauCCDIS,    // numu to nutau    CC Deep Inelastic Scattering
   kmNuMuToNuTauCCMpi,    // numu to nutau    CC multi pi (>1 pion of any charge(s))
   kmNuMuToNuTauCCmisc,   // numu to nutau    CC miscellaneous modes that require separate treatment to CC other
   kmNuMuToNuTauCCoth,    // numu to nutau    CC other
   kmNuEToNuMuCCQE,       // nue to numu    CC QE
   kmNuEToNuMuCCMEC,      // nue to numu    CC MEC
   kmNuEToNuMuCC1piC,     // nue to numu    CC 1 pi+/-
   kmNuEToNuMuCC1pi0,     // nue to numu    CC 1 pi0
   kmNuEToNuMuCC2piC,     // nue to numu    CC 2 pi+/-
   kmNuEToNuMuCC2pi0,     // nue to numu    CC 2 pi0
   kmNuEToNuMuCCpiCpi0,   // nue to numu    CC 1 pi0 + 1 pi+/-
   kmNuEToNuMuCCcoh,      // nue to numu    CC coherent
   kmNuEToNuMuCCIMD,      // nue to numu: numu + e- -> mu + nue inverse muon decay (IMD) 
   kmNuEToNuMuCCELScat,   // nue to numu: numu + e- CC elastic scattering
   kmNuEToNuMuCCDIS,      // nue to numu    CC Deep Inelastic Scattering
   kmNuEToNuMuCCMpi,      // nue to numu    CC multi pi (>1 pion of any charge(s))
   kmNuEToNuMuCCmisc,     // nue to numu    CC miscellaneous modes that require separate treatment to CC other
   kmNuEToNuMuCCoth,      // nue to numu    CC other
   kmNuEToNuTauCCQE,      // nue to nutau    CC QE
   kmNuEToNuTauCCMEC,     // nue to nutau    CC MEC
   kmNuEToNuTauCC1piC,    // nue to nutau    CC 1 pi+/-
   kmNuEToNuTauCC1pi0,    // nue to nutau    CC 1 pi0
   kmNuEToNuTauCC2piC,    // nue to nutau    CC 2 pi+/-
   kmNuEToNuTauCC2pi0,    // nue to nutau    CC 2 pi0
   kmNuEToNuTauCCpiCpi0,  // nue to nutau    CC 1 pi0 + 1 pi+/-
   kmNuEToNuTauCCcoh,     // nue to nutau    CC coherent
   kmNuEToNuTauCCELScat,  // ne to nutau: nutau + e- elastic scattering
   kmNuEToNuTauCCDIS,     // nue to nutau    CC Deep Inelastic Scattering
   kmNuEToNuTauCCMpi,     // nue to nutau    CC multi pi (>1 pion of any charge(s))
   kmNuEToNuTauCCmisc,    // nue to nutau    CC miscellaneous modes that require separate treatment to CC other
   kmNuEToNuTauCCoth,     // nue to nutau    CC other
   kmNuMuBarToNuEBarCCQE,            // numubar to nuebar    CC QE
   kmNuMuBarToNuEBarCCMEC,           // numubar to nuebar    CC MEC
   kmNuMuBarToNuEBarCC1piC,          // numubar to nuebar    CC 1 pi+/-
   kmNuMuBarToNuEBarCC1pi0,          // numubar to nuebar    CC 1 pi0
   kmNuMuBarToNuEBarCC2piC,          // numubar to nuebar    CC 2 pi+/-
   kmNuMuBarToNuEBarCC2pi0,          // numubar to nuebar    CC 2 pi0
   kmNuMuBarToNuEBarCCpiCpi0,        // numubar to nuebar    CC 1 pi0 + 1 pi+/-
   kmNuMuBarToNuEBarCCcoh,           // numubar to nuebar    CC coherent
   kmNuMuBarToNuEBarCCIMD,           // numubar to nuebar:  nuebar + e- -> mu + numubar (IMD)
   kmNuMuBarToNuEBarELScat,          // numubar to nuebar:  nuebar + e- -> nuebar + e- NC+CC+interference elastic scattering
   kmNuMuBarToNuEBarCCDIS,           // numubar to nuebar    CC Deep Inelastic Scattering
   kmNuMuBarToNuEBarCCMpi,           // numubar to nuebar    CC multi pi (>1 pion of any charge(s))
   kmNuMuBarToNuEBarCCmisc,          // numubar to nuebar    CC miscellaneous modes that require separate treatment to CC other
   kmNuMuBarToNuEBarCCoth,           // numubar to nuebar    CC other
   kmNuMuBarToNuTauBarCCQE,          // numubar to nutaubar    CC QE
   kmNuMuBarToNuTauBarCCMEC,         // numubar to nutaubar    CC MEC
   kmNuMuBarToNuTauBarCC1piC,        // numubar to nutaubar    CC 1 pi+/-
   kmNuMuBarToNuTauBarCC1pi0,        // numubar to nutaubar    CC 1 pi0
   kmNuMuBarToNuTauBarCC2piC,        // numubar to nutaubar    CC 2 pi+/-
   kmNuMuBarToNuTauBarCC2pi0,        // numubar to nutaubar    CC 2 pi0
   kmNuMuBarToNuTauBarCCpiCpi0,      // numubar to nutaubar    CC 1 pi0 + 1 pi+/-
   kmNuMuBarToNuTauBarCCcoh,         // numubar to nutaubar    CC coherent
   kmNuMuBarToNuTauBarCCELScat,      // numubar to nutaubar: nutaubar + e- elastic scattering
   kmNuMuBarToNuTauBarCCDIS,         // numubar to nutaubar    CC Deep Inelastic Scattering
   kmNuMuBarToNuTauBarCCMpi,         // numubar to nutaubar    CC multi pi (>1 pion of any charge(s))
   kmNuMuBarToNuTauBarCCmisc,        // numubar to nutaubar    CC miscellaneous modes that require separate treatment to CC other
   kmNuMuBarToNuTauBarCCoth,         // numubar to nutaubar    CC other
   kmNuEBarToNuMuBarCCQE,            // nuebar to numubar    CC QE
   kmNuEBarToNuMuBarCCMEC,           // nuebar to numubar    CC MEC
   kmNuEBarToNuMuBarCC1piC,          // nuebar to numubar    CC 1 pi+/-
   kmNuEBarToNuMuBarCC1pi0,          // nuebar to numubar    CC 1 pi0
   kmNuEBarToNuMuBarCC2piC,          // nuebar to numubar    CC 2 pi+/-
   kmNuEBarToNuMuBarCC2pi0,          // nuebar to numubar    CC 2 pi0
   kmNuEBarToNuMuBarCCpiCpi0,        // nuebar to numubar    CC 1 pi0 + 1 pi+/-
   kmNuEBarToNuMuBarCCcoh,           // nuebar to numubar    CC coherent
   kmNuEBarToNuMuBarCCELScat,        // nuebar to numubar: numubar + e- elastic scattering
   kmNuEBarToNuMuBarCCDIS,           // nuebar to numubar    CC Deep Inelastic Scattering
   kmNuEBarToNuMuBarCCMpi,           // nuebar to numubar    CC multi pi (>1 pion of any charge(s))
   kmNuEBarToNuMuBarCCmisc,          // nuebar to numubar    CC miscellaneous modes that require separate treatment to CC other
   kmNuEBarToNuMuBarCCoth,           // nuebar to numubar    CC other
   kmNuEBarToNuTauBarCCQE,           // nuebar to nutaubar    CC QE
   kmNuEBarToNuTauBarCCMEC,          // nuebar to nutaubar    CC MEC
   kmNuEBarToNuTauBarCC1piC,         // nuebar to nutaubar    CC 1 pi+/-
   kmNuEBarToNuTauBarCC1pi0,         // nuebar to nutaubar    CC 1 pi0
   kmNuEBarToNuTauBarCC2piC,         // nuebar to nutaubar    CC 2 pi+/-
   kmNuEBarToNuTauBarCC2pi0,         // nuebar to nutaubar    CC 2 pi0
   kmNuEBarToNuTauBarCCpiCpi0,       // nuebar to nutaubar    CC 1 pi0 + 1 pi+/-
   kmNuEBarToNuTauBarCCcoh,          // nuebar to nutaubar    CC coherent
   kmNuEBarToNuTauBarCCELScat,       // nuebar to nutaubar: nutaubar + e- elastic scattering
   kmNuEBarToNuTauBarCCDIS,          // nuebar to nutaubar    CC Deep Inelastic Scattering
   kmNuEBarToNuTauBarCCMpi,          // nuebar to nutaubar    CC multi pi (>1 pion of any charge(s))
   kmNuEBarToNuTauBarCCmisc,         // nuebar to nutaubar    CC miscellaneous modes that require separate treatment to CC other
   kmNuEBarToNuTauBarCCoth,          // nuebar to nutaubar    CC other
   kmCosmic,                         //  first non-neutrino induced - all above here will pass IsNeutrinoAny
   kmDirt,                           // non-neutrino induced, dirt 
   kmOther                           // other, eg contributions from interactions outside the fiducial etc
} 
 ReacMode_t; 

// Same as above but not broken down by nu flavour
// careful: now there is a difference in the nu + e elastic scattering 
typedef enum EReacType
{
  kmtUndefined = -1,
  kmtCCQE,        // CC QE
  kmtCCMEC,       // CC MEC
  kmtCC1piC,      // CC 1 pi+/-
  kmtCC1pi0,      // CC 1 pi0
  kmtCC2piC,      // CC 2 pi+/-
  kmtCC2pi0,      // CC 2 pi0
  kmtCCpiCpi0,    // CC 1 pi0 + 1 pi+/-
  kmtCCcoh,       // CC coherent
  kmtCCIMD,       // CC IMD
  kmtCCELScat,    // CC nu + e elastic scattering (not nue)
  kmtCCDIS,       // CC Deep Inelastic Scattering
  kmtCCMpi,       // CC multi pi (>1 pion of any charge(s))
  kmtCCmisc,      // CC miscellaneous modes that require separate treatment to CC other
  kmtCCoth,       // CC other
  kmtNCEL,        // NC EL - not used! 
  kmtNCMEC,       // NC MEC - not used! 
  kmtNC1piC,      // NC 1 pi+/-
  kmtNC1pi0,      // NC 1 pi0
  kmtNC2piC,      // NC 2 pi+/-
  kmtNC2pi0,      // NC 2 pi0
  kmtNCpiCpi0,    // NC 1 pi0 + 1 pi+/-
  kmtNCcoh,       // NC coherent
  kmtNC1Gamma,    // NC 1 gamma
  kmtNCELScat,    // NC nu + e elastic scattering (not nue)
  kmtNCoth,       // NC other
  kmtELScat,      // nue + e elastic scattering (nue, NC + CC + interference) 
  kmtCosmic,      // cosmic
  kmtDirt,        // dirt
  kmtOther        // misc
}
ReacType_t;

static const int kNReacType = 28;

// Map the reaction modes to their type
static const ReacType_t kReacModeToType[kNReacModes] =
{
   /*   kmNuMuCCQE          */ kmtCCQE     ,
   /*   kmNuMuCCMEC         */ kmtCCMEC    ,
   /*   kmNuMuCC1piC        */ kmtCC1piC   ,
   /*   kmNuMuCC1pi0        */ kmtCC1pi0   ,
   /*   kmNuMuCC2piC        */ kmtCC2piC   ,
   /*   kmNuMuCC2pi0        */ kmtCC2pi0   ,
   /*   kmNuMuCCpiCpi0      */ kmtCCpiCpi0 ,
   /*   kmNuMuCCcoh         */ kmtCCcoh    ,
   /*   kmNuMuCCIMD         */ kmtCCIMD    ,  
   /*   kmNuMuCCELScat      */ kmtCCELScat ,
   /*   kmNuMuCCDIS         */ kmtCCDIS    ,
   /*   kmNuMuCCMpi         */ kmtCCMpi    ,
   /*   kmNuMuCCmisc        */ kmtCCmisc   ,
   /*   kmNuMuCCoth         */ kmtCCoth    ,
   /*   kmNuMuNCEL          */ kmtNCEL     ,
   /*   kmNuMuNCMEC         */ kmtNCMEC    ,
   /*   kmNuMuNC1piC        */ kmtNC1piC   ,
   /*   kmNuMuNC1pi0        */ kmtNC1pi0   ,
   /*   kmNuMuNC2piC        */ kmtNC2piC   ,
   /*   kmNuMuNC2pi0        */ kmtNC2pi0   ,
   /*   kmNuMuNCpiCpi0      */ kmtNCpiCpi0 ,
   /*   kmNuMuNCcoh         */ kmtNCcoh    ,
   /*   kmNuMuN1Gamma       */ kmtNC1Gamma ,
   /*   kmNuMuNCELScat      */ kmtNCELScat ,
   /*   kmNuMuNCoth         */ kmtNCoth    ,
   /*   kmNuMuBarCCQE       */ kmtCCQE     ,
   /*   kmNuMuBarCCMEC      */ kmtCCMEC    ,
   /*   kmNuMuBarCC1piC     */ kmtCC1piC   ,
   /*   kmNuMuBarCC1pi0     */ kmtCC1pi0   ,
   /*   kmNuMuBarCC2piC     */ kmtCC2piC   ,
   /*   kmNuMuBarCC2pi0     */ kmtCC2pi0   ,
   /*   kmNuMuBarCCpiCpi0   */ kmtCCpiCpi0 ,
   /*   kmNuMuBarCCcoh      */ kmtCCcoh    ,
   /*   kmNuMuBarCCELScat   */ kmtCCELScat ,
   /*   kmNuMuBarCCDIS      */ kmtCCDIS    ,
   /*   kmNuMuBarCCMpi      */ kmtCCMpi    ,
   /*   kmNuMuBarCCmisc     */ kmtCCmisc   ,
   /*   kmNuMuBarCCoth      */ kmtCCoth    ,
   /*   kmNuMuBarNCEL       */ kmtNCEL     ,
   /*   kmNuMuBarNCMEC      */ kmtNCMEC    ,
   /*   kmNuMuBarNC1piC     */ kmtNC1piC   ,
   /*   kmNuMuBarNC1pi0     */ kmtNC1pi0   ,
   /*   kmNuMuBarNC2piC     */ kmtNC2piC   ,
   /*   kmNuMuBarNC2pi0     */ kmtNC2pi0   ,
   /*   kmNuMuBarNCpiCpi0   */ kmtNCpiCpi0 ,
   /*   kmNuMuBarNCcoh      */ kmtNCcoh    ,
   /*   kmNuMuBarNC1Gamma   */ kmtNC1Gamma ,
   /*   kmNuMuBarNCELScat   */ kmtNCELScat ,
   /*   kmNuMuBarNCoth      */ kmtNCoth    ,
   /*   kmNuECCQE           */ kmtCCQE     ,
   /*   kmNuECCMEC          */ kmtCCMEC    ,
   /*   kmNuECC1piC         */ kmtCC1piC   ,
   /*   kmNuECC1pi0         */ kmtCC1pi0   ,
   /*   kmNuECC2piC         */ kmtCC2piC   ,
   /*   kmNuECC2pi0         */ kmtCC2pi0   ,
   /*   kmNuECCpiCpi0       */ kmtCCpiCpi0 ,
   /*   kmNuECCcoh          */ kmtCCcoh    ,
   /*   kmNuECCDIS          */ kmtCCDIS    ,
   /*   kmNuECCMpi          */ kmtCCMpi    ,
   /*   kmNuECCmisc         */ kmtCCmisc   ,
   /*   kmNuECCoth          */ kmtCCoth    ,
   /*   kmNuENCEL           */ kmtNCEL     ,
   /*   kmNuENCMEC          */ kmtNCMEC    ,
   /*   kmNuENC1piC         */ kmtNC1piC   ,
   /*   kmNuENC1pi0         */ kmtNC1pi0   ,
   /*   kmNuENC2piC         */ kmtNC2piC   ,
   /*   kmNuENC2pi0         */ kmtNC2pi0   ,
   /*   kmNuENCpiCpi0       */ kmtNCpiCpi0 ,
   /*   kmNuENCcoh          */ kmtNCcoh    ,
   /*   kmNuENC1Gamma       */ kmtNC1Gamma ,
   /*   kmNuEELScat         */ kmtELScat   ,
   /*   kmNuENCoth          */ kmtNCoth    , 
   /*   kmNuEBarCCQE        */ kmtCCQE     , 
   /*   kmNuEBarCCMEC       */ kmtCCMEC    , 
   /*   kmNuEBarCC1piC      */ kmtCC1piC   , 
   /*   kmNuEBarCC1pi0      */ kmtCC1pi0   , 
   /*   kmNuEBarCC2piC      */ kmtCC2piC   , 
   /*   kmNuEBarCC2pi0      */ kmtCC2pi0   , 
   /*   kmNuEBarCCpiCpi0    */ kmtCCpiCpi0 , 
   /*   kmNuEBarCCcoh       */ kmtCCcoh    , 
   /*   kmNuEBarCCIMD       */ kmtCCIMD    ,
   /*   kmNuEBarCCDIS       */ kmtCCDIS    ,
   /*   kmNuEBarCCMpi       */ kmtCCMpi    ,
   /*   kmNuEBarCCmisc      */ kmtCCmisc   ,
   /*   kmNuEBarCCoth       */ kmtCCoth    ,
   /*   kmNuEBarNCEL        */ kmtNCEL     ,
   /*   kmNuEBarNCMEC       */ kmtNCMEC    ,
   /*   kmNuEBarNC1piC      */ kmtNC1piC   ,
   /*   kmNuEBarNC1pi0      */ kmtNC1pi0   ,
   /*   kmNuEBarNC2piC      */ kmtNC2piC   ,
   /*   kmNuEBarNC2pi0      */ kmtNC2pi0   ,
   /*   kmNuEBarNCpiCpi0    */ kmtNCpiCpi0 ,
   /*   kmNuEBarNCcoh       */ kmtNCcoh    ,
   /*   kmNuEBarNC1Gamma    */ kmtNC1Gamma ,
   /*   kmNuEBarELScat      */ kmtELScat   ,
   /*   kmNuEBarNCoth       */ kmtNCoth    ,
   /*   kmNuMuToNuECCQE          */ kmtCCQE     ,
   /*   kmNuMuToNuECCMEC         */ kmtCCMEC    ,
   /*   kmNuMuToNuECC1piC        */ kmtCC1piC   ,
   /*   kmNuMuToNuECC1pi0        */ kmtCC1pi0   ,
   /*   kmNuMuToNuECC2piC        */ kmtCC2piC   ,
   /*   kmNuMuToNuECC2pi0        */ kmtCC2pi0   ,
   /*   kmNuMuToNuECCpiCpi0      */ kmtCCpiCpi0 ,
   /*   kmNuMuToNuECCcoh         */ kmtCCcoh    ,
   /*   kmNuMuToNuEELScat        */ kmtELScat   ,
   /*   kmNuMuToNuECCDIS         */ kmtCCDIS    ,
   /*   kmNuMuToNuECCMpi         */ kmtCCMpi    ,
   /*   kmNuMuToNuECCmisc        */ kmtCCmisc   ,
   /*   kmNuMuToNuECCoth         */ kmtCCoth    ,
   /*   kmNuMuToNuTauCCQE        */ kmtCCQE     ,
   /*   kmNuMuToNuTauCCMEC       */ kmtCCMEC    ,
   /*   kmNuMuToNuTauCC1piC      */ kmtCC1piC   ,
   /*   kmNuMuToNuTauCC1pi0      */ kmtCC1pi0   ,
   /*   kmNuMuToNuTauCC2piC      */ kmtCC2piC   ,
   /*   kmNuMuToNuTauCC2pi0      */ kmtCC2pi0   ,
   /*   kmNuMuToNuTauCCpiCpi0    */ kmtCCpiCpi0 ,
   /*   kmNuMuToNuTauCCcoh       */ kmtCCcoh    ,
   /*   kmNuMuToNuTauELScat      */ kmtCCELScat ,
   /*   kmNuMuToNuTauCCDIS       */ kmtCCDIS    ,
   /*   kmNuMuToNuTauCCMpi       */ kmtCCMpi    ,
   /*   kmNuMuToNuTauCCmisc      */ kmtCCmisc   ,
   /*   kmNuMuToNuTauCCoth       */ kmtCCoth    ,
   /*   kmNuEToNuMuCCQE          */ kmtCCQE     ,
   /*   kmNuEToNuMuCCMEC         */ kmtCCMEC    ,
   /*   kmNuEToNuMuCC1piC        */ kmtCC1piC   ,
   /*   kmNuEToNuMuCC1pi0        */ kmtCC1pi0   ,
   /*   kmNuEToNuMuCC2piC        */ kmtCC2piC   ,
   /*   kmNuEToNuMuCC2pi0        */ kmtCC2pi0   ,
   /*   kmNuEToNuMuCCpiCpi0      */ kmtCCpiCpi0 ,
   /*   kmNuEToNuMuCCcoh         */ kmtCCcoh    ,
   /*   kmNuEToNuMuCCIMD         */ kmtCCIMD    ,
   /*   kmNuEToNuMuELScat        */ kmtCCELScat ,
   /*   kmNuEToNuMuCCDIS         */ kmtCCDIS    ,
   /*   kmNuEToNuMuCCMpi         */ kmtCCMpi    ,
   /*   kmNuEToNuMuCCmisc        */ kmtCCmisc   ,
   /*   kmNuEToNuMuCCoth         */ kmtCCoth    ,
   /*   kmNuEToNuTauCCQE         */ kmtCCQE     ,
   /*   kmNuEToNuTauCCMEC        */ kmtCCMEC    ,
   /*   kmNuEToNuTauCC1piC       */ kmtCC1piC   ,
   /*   kmNuEToNuTauCC1pi0       */ kmtCC1pi0   ,
   /*   kmNuEToNuTauCC2piC       */ kmtCC2piC   ,
   /*   kmNuEToNuTauCC2pi0       */ kmtCC2pi0   ,
   /*   kmNuEToNuTauCCpiCpi0     */ kmtCCpiCpi0 ,
   /*   kmNuEToNuTauCCcoh        */ kmtCCcoh    ,
   /*   kmNuEToNuTauELScat       */ kmtCCELScat ,
   /*   kmNuEToNuTauCCDIS        */ kmtCCDIS    ,
   /*   kmNuEToNuTauCCMpi        */ kmtCCMpi    ,
   /*   kmNuEToNuTauCCmisc       */ kmtCCmisc   ,
   /*   kmNuEToNuTauCCoth        */ kmtCCoth    ,
   /*   kmNuMuBarToNuEBarCCQE          */ kmtCCQE     ,
   /*   kmNuMuBarToNuEBarCCMEC         */ kmtCCMEC    ,
   /*   kmNuMuBarToNuEBarCC1piC        */ kmtCC1piC   ,
   /*   kmNuMuBarToNuEBarCC1pi0        */ kmtCC1pi0   ,
   /*   kmNuMuBarToNuEBarCC2piC        */ kmtCC2piC   ,
   /*   kmNuMuBarToNuEBarCC2pi0        */ kmtCC2pi0   ,
   /*   kmNuMuBarToNuEBarCCpiCpi0      */ kmtCCpiCpi0 ,
   /*   kmNuMuBarToNuEBarCCcoh         */ kmtCCcoh    ,
   /*   kmNuMuBarToNuEBarCCIMD         */ kmtCCIMD    ,
   /*   kmNuMuBarToNuEBarELScat        */ kmtELScat   ,
   /*   kmNuMuBarToNuEBarCCDIS         */ kmtCCDIS    ,
   /*   kmNuMuBarToNuEBarCCMpi         */ kmtCCMpi    ,
   /*   kmNuMuBarToNuEBarCCmisc        */ kmtCCmisc   ,
   /*   kmNuMuBarToNuEBarCCoth         */ kmtCCoth    ,
   /*   kmNuMuBarToNuTauBarCCQE        */ kmtCCQE     ,
   /*   kmNuMuBarToNuTauBarCCMEC       */ kmtCCMEC    ,
   /*   kmNuMuBarToNuTauBarCC1piC      */ kmtCC1piC   ,
   /*   kmNuMuBarToNuTauBarCC1pi0      */ kmtCC1pi0   ,
   /*   kmNuMuBarToNuTauBarCC2piC      */ kmtCC2piC   ,
   /*   kmNuMuBarToNuTauBarCC2pi0      */ kmtCC2pi0   ,
   /*   kmNuMuBarToNuTauBarCCpiCpi0    */ kmtCCpiCpi0 ,
   /*   kmNuMuBarToNuTauBarCCcoh       */ kmtCCcoh    ,
   /*   kmNuMuBarToNuTauBarELScat      */ kmtCCELScat ,
   /*   kmNuMuBarToNuTauBarCCDIS       */ kmtCCDIS    ,
   /*   kmNuMuBarToNuTauBarCCMpi       */ kmtCCMpi    ,
   /*   kmNuMuBarToNuTauBarCCmisc      */ kmtCCmisc   ,
   /*   kmNuMuBarToNuTauBarCCoth       */ kmtCCoth    ,
   /*   kmNuEBarToNuMuBarCCQE          */ kmtCCQE     ,
   /*   kmNuEBarToNuMuBarCCMEC         */ kmtCCMEC    ,
   /*   kmNuEBarToNuMuBarCC1piC        */ kmtCC1piC   ,
   /*   kmNuEBarToNuMuBarCC1pi0        */ kmtCC1pi0   ,
   /*   kmNuEBarToNuMuBarCC2piC        */ kmtCC2piC   ,
   /*   kmNuEBarToNuMuBarCC2pi0        */ kmtCC2pi0   ,
   /*   kmNuEBarToNuMuBarCCpiCpi0      */ kmtCCpiCpi0 ,
   /*   kmNuEBarToNuMuBarCCcoh         */ kmtCCcoh    ,
   /*   kmNuEBarToNuMuBarELScat        */ kmtCCELScat ,
   /*   kmNuEBarToNuMuBarCCDIS         */ kmtCCDIS    ,
   /*   kmNuEBarToNuMuBarCCMpi         */ kmtCCMpi    ,
   /*   kmNuEBarToNuMuBarCCmisc        */ kmtCCmisc   ,
   /*   kmNuEBarToNuMuBarCCoth         */ kmtCCoth    ,
   /*   kmNuEBarToNuTauBarCCQE         */ kmtCCQE     ,
   /*   kmNuEBarToNuTauBarCCMEC        */ kmtCCMEC    ,
   /*   kmNuEBarToNuTauBarCC1piC       */ kmtCC1piC   ,
   /*   kmNuEBarToNuTauBarCC1pi0       */ kmtCC1pi0   ,
   /*   kmNuEBarToNuTauBarCC2piC       */ kmtCC2piC   ,
   /*   kmNuEBarToNuTauBarCC2pi0       */ kmtCC2pi0   ,
   /*   kmNuEBarToNuTauBarCCpiCpi0     */ kmtCCpiCpi0 ,
   /*   kmNuEBarToNuTauBarCCcoh        */ kmtCCcoh    ,
   /*   kmNuEBarToNuTauBarELScat       */ kmtCCELScat ,
   /*   kmNuEBarToNuTauBarCCDIS        */ kmtCCDIS    ,
   /*   kmNuEBarToNuTauBarCCMpi        */ kmtCCMpi    ,
   /*   kmNuEBarToNuTauBarCCmisc       */ kmtCCmisc   ,
   /*   kmNuEBarToNuTauBarCCoth        */ kmtCCoth    ,
   /*   kmCosmic                       */ kmtCosmic   ,
   /*   kmDirt                         */ kmtDirt     ,
// /*   kmOther             */ kmtOther 
};

// Map the reaction modes to their final neutrino flavour 
static const NuFlavour_t kReacModeToFinalFlavour[kNReacModes] =
{
   /*   kmNuMuCCQE          */ kNuMu    ,
   /*   kmNuMuCCMEC         */ kNuMu    ,
   /*   kmNuMuCC1piC        */ kNuMu    ,
   /*   kmNuMuCC1pi0        */ kNuMu    ,
   /*   kmNuMuCC2piC        */ kNuMu    ,
   /*   kmNuMuCC2pi0        */ kNuMu    ,
   /*   kmNuMuCCpiCpi0      */ kNuMu    ,
   /*   kmNuMuCCcoh         */ kNuMu    ,
   /*   kmNuMuCCIMD         */ kNuMu    ,
   /*   kmNuMuCCELScat      */ kNuMu    ,
   /*   kmNuMuCCDIS         */ kNuMu    ,
   /*   kmNuMuCCMpi         */ kNuMu    ,
   /*   kmNuMuCCmisc        */ kNuMu    ,
   /*   kmNuMuCCoth         */ kNuMu    ,
   /*   kmNuMuNCEL          */ kNuMu    ,
   /*   kmNuMuNCMEC         */ kNuMu    ,
   /*   kmNuMuNC1piC        */ kNuMu    ,
   /*   kmNuMuNC1pi0        */ kNuMu    ,
   /*   kmNuMuNC2piC        */ kNuMu    ,
   /*   kmNuMuNC2pi0        */ kNuMu    ,
   /*   kmNuMuNCpiCpi0      */ kNuMu    ,
   /*   kmNuMuNCcoh         */ kNuMu    ,
   /*   kmNuMuNC1Gamma      */ kNuMu    ,
   /*   kmNuMuNCELScat      */ kNuMu    ,
   /*   kmNuMuNCoth         */ kNuMu    ,
   /*   kmNuMuBarCCQE       */ kNuMuBar ,
   /*   kmNuMuBarCCMEC      */ kNuMuBar ,
   /*   kmNuMuBarCC1piC     */ kNuMuBar ,
   /*   kmNuMuBarCC1pi0     */ kNuMuBar ,
   /*   kmNuMuBarCC2piC     */ kNuMuBar ,
   /*   kmNuMuBarCC2pi0     */ kNuMuBar ,
   /*   kmNuMuBarCCpiCpi0   */ kNuMuBar ,
   /*   kmNuMuBarCCcoh      */ kNuMuBar ,
   /*   kmNuMuBarCCELScat   */ kNuMuBar ,
   /*   kmNuMuBarCCDIS      */ kNuMuBar ,
   /*   kmNuMuBarCCMpi      */ kNuMuBar ,
   /*   kmNuMuBarCCmisc     */ kNuMuBar ,
   /*   kmNuMuBarCCoth      */ kNuMuBar ,
   /*   kmNuMuBarNCEL       */ kNuMuBar ,
   /*   kmNuMuBarNCMEC      */ kNuMuBar ,
   /*   kmNuMuBarNC1piC     */ kNuMuBar ,
   /*   kmNuMuBarNC1pi0     */ kNuMuBar ,
   /*   kmNuMuBarNC2piC     */ kNuMuBar ,
   /*   kmNuMuBarNC2pi0     */ kNuMuBar ,
   /*   kmNuMuBarNCpiCpi0   */ kNuMuBar ,
   /*   kmNuMuBarNCcoh      */ kNuMuBar ,
   /*   kmNuMuBarNC1Gamma   */ kNuMuBar ,
   /*   kmNuMuBarNCELScat   */ kNuMuBar ,
   /*   kmNuMuBarNCoth      */ kNuMuBar ,
   /*   kmNuECCQE           */ kNuE     ,
   /*   kmNuECCMEC          */ kNuE     ,
   /*   kmNuECC1piC         */ kNuE     ,
   /*   kmNuECC1pi0         */ kNuE     ,
   /*   kmNuECC2piC         */ kNuE     ,
   /*   kmNuECC2pi0         */ kNuE     ,
   /*   kmNuECCpiCpi0       */ kNuE     ,
   /*   kmNuECCcoh          */ kNuE     ,
   /*   kmNuECCDIS          */ kNuE     ,
   /*   kmNuECCMpi          */ kNuE     ,
   /*   kmNuECCmisc         */ kNuE     ,
   /*   kmNuECCoth          */ kNuE     ,
   /*   kmNuENCEL           */ kNuE     ,
   /*   kmNuENCMEC          */ kNuE     ,
   /*   kmNuENC1piC         */ kNuE     ,
   /*   kmNuENC1pi0         */ kNuE     ,
   /*   kmNuENC2piC         */ kNuE     ,
   /*   kmNuENC2pi0         */ kNuE     ,
   /*   kmNuENCpiCpi0       */ kNuE     ,
   /*   kmNuENCcoh          */ kNuE     ,
   /*   kmNuENC1Gamma       */ kNuE     ,
   /*   kmNuEELScat         */ kNuE     ,
   /*   kmNuENCoth          */ kNuE     , 
   /*   kmNuEBarCCQE        */ kNuEBar  , 
   /*   kmNuEBarCCMEC       */ kNuEBar  , 
   /*   kmNuEBarCC1piC      */ kNuEBar  , 
   /*   kmNuEBarCC1pi0      */ kNuEBar  , 
   /*   kmNuEBarCC2piC      */ kNuEBar  , 
   /*   kmNuEBarCC2pi0      */ kNuEBar  , 
   /*   kmNuEBarCCpiCpi0    */ kNuEBar  , 
   /*   kmNuEBarCCcoh       */ kNuEBar  , 
   /*   kmNuEBarCCIMD       */ kNuEBar  ,
   /*   kmNuEBarCCDIS       */ kNuEBar  ,
   /*   kmNuEBarCCMpi       */ kNuEBar  ,
   /*   kmNuEBarCCmisc      */ kNuEBar  ,
   /*   kmNuEBarCCoth       */ kNuEBar  ,
   /*   kmNuEBarNCEL        */ kNuEBar  ,
   /*   kmNuEBarNCMEC       */ kNuEBar  ,
   /*   kmNuEBarNC1piC      */ kNuEBar  ,
   /*   kmNuEBarNC1pi0      */ kNuEBar  ,
   /*   kmNuEBarNC2piC      */ kNuEBar  ,
   /*   kmNuEBarNC2pi0      */ kNuEBar  ,
   /*   kmNuEBarNCpiCpi0    */ kNuEBar  ,
   /*   kmNuEBarNCcoh       */ kNuEBar  ,
   /*   kmNuEBarNC1Gamma    */ kNuEBar  ,
   /*   kmNuEBarELScat      */ kNuEBar  ,
   /*   kmNuEBarNCoth       */ kNuEBar  ,
   /*   kmNuMuToNuECCQE          */ kNuE    ,
   /*   kmNuMuToNuECCMEC         */ kNuE    ,
   /*   kmNuMuToNuECC1piC        */ kNuE    ,
   /*   kmNuMuToNuECC1pi0        */ kNuE    ,
   /*   kmNuMuToNuECC2piC        */ kNuE    ,
   /*   kmNuMuToNuECC2pi0        */ kNuE    ,
   /*   kmNuMuToNuECCpiCpi0      */ kNuE    ,
   /*   kmNuMuToNuECCcoh         */ kNuE    ,
   /*   kmNuMuToNuEELScat        */ kNuE    ,
   /*   kmNuMuToNuECCDIS         */ kNuE    ,
   /*   kmNuMuToNuECCMpi         */ kNuE    ,
   /*   kmNuMuToNuECCmisc        */ kNuE    ,
   /*   kmNuMuToNuECCoth         */ kNuE    ,
   /*   kmNuMuToNuTauCCQE        */ kNuTau    ,
   /*   kmNuMuToNuTauCCMEC       */ kNuTau    ,
   /*   kmNuMuToNuTauCC1piC      */ kNuTau    ,
   /*   kmNuMuToNuTauCC1pi0      */ kNuTau    ,
   /*   kmNuMuToNuTauCC2piC      */ kNuTau    ,
   /*   kmNuMuToNuTauCC2pi0      */ kNuTau    ,
   /*   kmNuMuToNuTauCCpiCpi0    */ kNuTau    ,
   /*   kmNuMuToNuTauCCcoh       */ kNuTau    ,
   /*   kmNuMuToNuTauCCELScat    */ kNuTau    ,
   /*   kmNuMuToNuTauCCDIS       */ kNuTau    ,
   /*   kmNuMuToNuTauCCMpi       */ kNuTau    ,
   /*   kmNuMuToNuTauCCmisc      */ kNuTau    ,
   /*   kmNuMuToNuTauCCoth       */ kNuTau    ,
   /*   kmNuEToNuMuCCQE          */ kNuMu    ,
   /*   kmNuEToNuMuCCMEC         */ kNuMu    ,
   /*   kmNuEToNuMuCC1piC        */ kNuMu    ,
   /*   kmNuEToNuMuCC1pi0        */ kNuMu    ,
   /*   kmNuEToNuMuCC2piC        */ kNuMu    ,
   /*   kmNuEToNuMuCC2pi0        */ kNuMu    ,
   /*   kmNuEToNuMuCCpiCpi0      */ kNuMu    ,
   /*   kmNuEToNuMuCCcoh         */ kNuMu    ,
   /*   kmNuEToNuMuCCIMD         */ kNuMu    ,
   /*   kmNuEToNuMuCCELScat      */ kNuMu    ,
   /*   kmNuEToNuMuCCDIS         */ kNuMu    ,
   /*   kmNuEToNuMuCCMpi         */ kNuMu    ,
   /*   kmNuEToNuMuCCmisc        */ kNuMu    ,
   /*   kmNuEToNuMuCCoth         */ kNuMu    ,
   /*   kmNuEToNuTauCCQE         */ kNuTau    ,
   /*   kmNuEToNuTauCCMEC        */ kNuTau    ,
   /*   kmNuEToNuTauCC1piC       */ kNuTau    ,
   /*   kmNuEToNuTauCC1pi0       */ kNuTau    ,
   /*   kmNuEToNuTauCC2piC       */ kNuTau    ,
   /*   kmNuEToNuTauCC2pi0       */ kNuTau    ,
   /*   kmNuEToNuTauCCpiCpi0     */ kNuTau    ,
   /*   kmNuEToNuTauCCcoh        */ kNuTau    ,
   /*   kmNuEToNuTauCCELScat     */ kNuTau    ,
   /*   kmNuEToNuTauCCDIS        */ kNuTau    ,
   /*   kmNuEToNuTauCCMpi        */ kNuTau    ,
   /*   kmNuEToNuTauCCmisc       */ kNuTau    ,
   /*   kmNuEToNuTauCCoth        */ kNuTau    ,
   /*   kmNuMuBarToNuEBarCCQE          */ kNuEBar    ,
   /*   kmNuMuBarToNuEBarCCMEC         */ kNuEBar    ,
   /*   kmNuMuBarToNuEBarCC1piC        */ kNuEBar    ,
   /*   kmNuMuBarToNuEBarCC1pi0        */ kNuEBar    ,
   /*   kmNuMuBarToNuEBarCC2piC        */ kNuEBar    ,
   /*   kmNuMuBarToNuEBarCC2pi0        */ kNuEBar    ,
   /*   kmNuMuBarToNuEBarCCpiCpi0      */ kNuEBar    ,
   /*   kmNuMuBarToNuEBarCCcoh         */ kNuEBar    ,
   /*   kmNuMuBarToNuEBarCCIMD         */ kNuEBar    ,
   /*   kmNuMuBarToNuEBarELScat        */ kNuEBar    ,
   /*   kmNuMuBarToNuEBarCCDIS         */ kNuEBar    ,
   /*   kmNuMuBarToNuEBarCCMpi         */ kNuEBar    ,
   /*   kmNuMuBarToNuEBarCCmisc        */ kNuEBar    ,
   /*   kmNuMuBarToNuEBarCCoth         */ kNuEBar    ,
   /*   kmNuMuBarToNuTauBarCCQE        */ kNuTauBar    ,
   /*   kmNuMuBarToNuTauBarCCMEC       */ kNuTauBar    ,
   /*   kmNuMuBarToNuTauBarCC1piC      */ kNuTauBar    ,
   /*   kmNuMuBarToNuTauBarCC1pi0      */ kNuTauBar    ,
   /*   kmNuMuBarToNuTauBarCC2piC      */ kNuTauBar    ,
   /*   kmNuMuBarToNuTauBarCC2pi0      */ kNuTauBar    ,
   /*   kmNuMuBarToNuTauBarCCpiCpi0    */ kNuTauBar    ,
   /*   kmNuMuBarToNuTauBarCCcoh       */ kNuTauBar    ,
   /*   kmNuMuBarToNuTauBarCCELScat    */ kNuTauBar    ,
   /*   kmNuMuBarToNuTauBarCCDIS       */ kNuTauBar    ,
   /*   kmNuMuBarToNuTauBarCCMpi       */ kNuTauBar    ,
   /*   kmNuMuBarToNuTauBarCCmisc      */ kNuTauBar    ,
   /*   kmNuMuBarToNuTauBarCCoth       */ kNuTauBar    ,
   /*   kmNuEBarToNuMuBarCCQE          */ kNuMuBar    ,
   /*   kmNuEBarToNuMuBarCCMEC         */ kNuMuBar    ,
   /*   kmNuEBarToNuMuBarCC1piC        */ kNuMuBar    ,
   /*   kmNuEBarToNuMuBarCC1pi0        */ kNuMuBar    ,
   /*   kmNuEBarToNuMuBarCC2piC        */ kNuMuBar    ,
   /*   kmNuEBarToNuMuBarCC2pi0        */ kNuMuBar    ,
   /*   kmNuEBarToNuMuBarCCpiCpi0      */ kNuMuBar    ,
   /*   kmNuEBarToNuMuBarCCcoh         */ kNuMuBar    ,
   /*   kmNuEBarToNuMuBarCCELScat      */ kNuMuBar    ,
   /*   kmNuEBarToNuMuBarCCDIS         */ kNuMuBar    ,
   /*   kmNuEBarToNuMuBarCCMpi         */ kNuMuBar    ,
   /*   kmNuEBarToNuMuBarCCmisc        */ kNuMuBar    ,
   /*   kmNuEBarToNuMuBarCCoth         */ kNuMuBar    ,
   /*   kmNuEBarToNuTauBarCCQE         */ kNuTauBar    ,
   /*   kmNuEBarToNuTauBarCCMEC        */ kNuTauBar    ,
   /*   kmNuEBarToNuTauBarCC1piC       */ kNuTauBar    ,
   /*   kmNuEBarToNuTauBarCC1pi0       */ kNuTauBar    ,
   /*   kmNuEBarToNuTauBarCC2piC       */ kNuTauBar    ,
   /*   kmNuEBarToNuTauBarCC2pi0       */ kNuTauBar    ,
   /*   kmNuEBarToNuTauBarCCpiCpi0     */ kNuTauBar    ,
   /*   kmNuEBarToNuTauBarCCcoh        */ kNuTauBar    ,
   /*   kmNuEBarToNuTauBarCCELScat     */ kNuTauBar    ,
   /*   kmNuEBarToNuTauBarCCDIS        */ kNuTauBar    ,
   /*   kmNuEBarToNuTauBarCCMpi        */ kNuTauBar    ,
   /*   kmNuEBarToNuTauBarCCmisc       */ kNuTauBar    ,
   /*   kmNuEBarToNuTauBarCCoth        */ kNuTauBar    ,
   /*   kmCosmic                       */ kCosmic      ,
   /*   kmDirt                         */ kDirt,
// /*   kmOther             */ kNuFlavourUndefined
};

static const NuFlavour_t kReacModeToInitFlavour[kNReacModes] =
{
   /*   kmNuMuCCQE          */ kNuMu    ,
   /*   kmNuMuCCMEC         */ kNuMu    ,
   /*   kmNuMuCC1piC        */ kNuMu    ,
   /*   kmNuMuCC1pi0        */ kNuMu    ,
   /*   kmNuMuCC2piC        */ kNuMu    ,
   /*   kmNuMuCC2pi0        */ kNuMu    ,
   /*   kmNuMuCCpiCpi0      */ kNuMu    ,
   /*   kmNuMuCCcoh         */ kNuMu    ,
   /*   kmNuMuCCIMD         */ kNuMu    ,
   /*   kmNuMuCCELScat      */ kNuMu    ,
   /*   kmNuMuCCDIS         */ kNuMu    ,
   /*   kmNuMuCCMpi         */ kNuMu    ,
   /*   kmNuMuCCmisc        */ kNuMu    ,
   /*   kmNuMuCCoth         */ kNuMu    ,
   /*   kmNuMuNCEL          */ kNuMu    ,
   /*   kmNuMuNCMEC         */ kNuMu    ,
   /*   kmNuMuNC1piC        */ kNuMu    ,
   /*   kmNuMuNC1pi0        */ kNuMu    ,
   /*   kmNuMuNC2piC        */ kNuMu    ,
   /*   kmNuMuNC2pi0        */ kNuMu    ,
   /*   kmNuMuNCpiCpi0      */ kNuMu    ,
   /*   kmNuMuNCcoh         */ kNuMu    ,
   /*   kmNuMuNC1Gamma      */ kNuMu    ,
   /*   kmNuMuNCELScat      */ kNuMu    ,
   /*   kmNuMuNCoth         */ kNuMu    ,
   /*   kmNuMuBarCCQE       */ kNuMuBar ,
   /*   kmNuMuBarCCMEC      */ kNuMuBar ,
   /*   kmNuMuBarCC1piC     */ kNuMuBar ,
   /*   kmNuMuBarCC1pi0     */ kNuMuBar ,
   /*   kmNuMuBarCC2piC     */ kNuMuBar ,
   /*   kmNuMuBarCC2pi0     */ kNuMuBar ,
   /*   kmNuMuBarCCpiCpi0   */ kNuMuBar ,
   /*   kmNuMuBarCCcoh      */ kNuMuBar ,
   /*   kmNuMuBarCCELScat   */ kNuMuBar ,
   /*   kmNuMuBarCCDIS      */ kNuMuBar ,
   /*   kmNuMuBarCCMpi      */ kNuMuBar ,
   /*   kmNuMuBarCCmisc     */ kNuMuBar ,
   /*   kmNuMuBarCCoth      */ kNuMuBar ,
   /*   kmNuMuBarNCEL       */ kNuMuBar ,
   /*   kmNuMuBarNCMEC      */ kNuMuBar ,
   /*   kmNuMuBarNC1piC     */ kNuMuBar ,
   /*   kmNuMuBarNC1pi0     */ kNuMuBar ,
   /*   kmNuMuBarNC2piC     */ kNuMuBar ,
   /*   kmNuMuBarNC2pi0     */ kNuMuBar ,
   /*   kmNuMuBarNCpiCpi0   */ kNuMuBar ,
   /*   kmNuMuBarNCcoh      */ kNuMuBar ,
   /*   kmNuMuBarNC1Gamma   */ kNuMuBar ,
   /*   kmNuMuBarNCELScat   */ kNuMuBar ,
   /*   kmNuMuBarNCoth      */ kNuMuBar ,
   /*   kmNuECCQE           */ kNuE     ,
   /*   kmNuECCMEC          */ kNuE     ,
   /*   kmNuECC1piC         */ kNuE     ,
   /*   kmNuECC1pi0         */ kNuE     ,
   /*   kmNuECC2piC         */ kNuE     ,
   /*   kmNuECC2pi0         */ kNuE     ,
   /*   kmNuECCpiCpi0       */ kNuE     ,
   /*   kmNuECCcoh          */ kNuE     ,
   /*   kmNuECCDIS          */ kNuE     ,
   /*   kmNuECCMpi          */ kNuE     ,
   /*   kmNuECCmisc         */ kNuE     ,
   /*   kmNuECCoth          */ kNuE     ,
   /*   kmNuENCEL           */ kNuE     ,
   /*   kmNuENCMEC          */ kNuE     ,
   /*   kmNuENC1piC         */ kNuE     ,
   /*   kmNuENC1pi0         */ kNuE     ,
   /*   kmNuENC2piC         */ kNuE     ,
   /*   kmNuENC2pi0         */ kNuE     ,
   /*   kmNuENCpiCpi0       */ kNuE     ,
   /*   kmNuENCcoh          */ kNuE     ,
   /*   kmNuENC1Gamma       */ kNuE     ,
   /*   kmNuEELScat         */ kNuE     ,
   /*   kmNuENCoth          */ kNuE     , 
   /*   kmNuEBarCCQE        */ kNuEBar  , 
   /*   kmNuEBarCCMEC       */ kNuEBar  , 
   /*   kmNuEBarCC1piC      */ kNuEBar  , 
   /*   kmNuEBarCC1pi0      */ kNuEBar  , 
   /*   kmNuEBarCC2piC      */ kNuEBar  , 
   /*   kmNuEBarCC2pi0      */ kNuEBar  , 
   /*   kmNuEBarCCpiCpi0    */ kNuEBar  , 
   /*   kmNuEBarCCcoh       */ kNuEBar  , 
   /*   kmNuEBarCCIMD       */ kNuEBar  ,
   /*   kmNuEBarCCDIS       */ kNuEBar  ,
   /*   kmNuEBarCCMpi       */ kNuEBar  ,
   /*   kmNuEBarCCmisc      */ kNuEBar  ,
   /*   kmNuEBarCCoth       */ kNuEBar  ,
   /*   kmNuEBarNCEL        */ kNuEBar  ,
   /*   kmNuEBarNCMEC       */ kNuEBar  ,
   /*   kmNuEBarNC1piC      */ kNuEBar  ,
   /*   kmNuEBarNC1pi0      */ kNuEBar  ,
   /*   kmNuEBarNC2piC      */ kNuEBar  ,
   /*   kmNuEBarNC2pi0      */ kNuEBar  ,
   /*   kmNuEBarNCpiCpi0    */ kNuEBar  ,
   /*   kmNuEBarNCcoh       */ kNuEBar  ,
   /*   kmNuEBarNC1Gamma    */ kNuEBar  ,
   /*   kmNuEBarELScat      */ kNuEBar  ,
   /*   kmNuEBarNCoth       */ kNuEBar  ,
   /*   kmNuMuToNuECCQE          */ kNuMu    ,
   /*   kmNuMuToNuECCMEC         */ kNuMu    ,
   /*   kmNuMuToNuECC1piC        */ kNuMu    ,
   /*   kmNuMuToNuECC1pi0        */ kNuMu    ,
   /*   kmNuMuToNuECC2piC        */ kNuMu    ,
   /*   kmNuMuToNuECC2pi0        */ kNuMu    ,
   /*   kmNuMuToNuECCpiCpi0      */ kNuMu    ,
   /*   kmNuMuToNuECCcoh         */ kNuMu    ,
   /*   kmNuMuToNuEELScat        */ kNuMu    ,
   /*   kmNuMuToNuECCDIS         */ kNuMu    ,
   /*   kmNuMuToNuECCMpi         */ kNuMu    ,
   /*   kmNuMuToNuECCmisc        */ kNuMu    ,
   /*   kmNuMuToNuECCoth         */ kNuMu    ,
   /*   kmNuMuToNuTauCCQE        */ kNuMu    ,
   /*   kmNuMuToNuTauCCMEC       */ kNuMu    ,
   /*   kmNuMuToNuTauCC1piC      */ kNuMu    ,
   /*   kmNuMuToNuTauCC1pi0      */ kNuMu    ,
   /*   kmNuMuToNuTauCC2piC      */ kNuMu    ,
   /*   kmNuMuToNuTauCC2pi0      */ kNuMu    ,
   /*   kmNuMuToNuTauCCpiCpi0    */ kNuMu    ,
   /*   kmNuMuToNuTauCCcoh       */ kNuMu    ,
   /*   kmNuMuToNuTauCCELScat    */ kNuMu    ,
   /*   kmNuMuToNuTauCCDIS       */ kNuMu    ,
   /*   kmNuMuToNuTauCCMpi       */ kNuMu    ,
   /*   kmNuMuToNuTauCCmisc      */ kNuMu    ,
   /*   kmNuMuToNuTauCCoth       */ kNuMu    ,
   /*   kmNuEToNuMuCCQE          */ kNuE    ,
   /*   kmNuEToNuMuCCMEC         */ kNuE    ,
   /*   kmNuEToNuMuCC1piC        */ kNuE    ,
   /*   kmNuEToNuMuCC1pi0        */ kNuE    ,
   /*   kmNuEToNuMuCC2piC        */ kNuE    ,
   /*   kmNuEToNuMuCC2pi0        */ kNuE    ,
   /*   kmNuEToNuMuCCpiCpi0      */ kNuE    ,
   /*   kmNuEToNuMuCCcoh         */ kNuE    ,
   /*   kmNuEToNuMuCCIMD         */ kNuE    ,
   /*   kmNuEToNuMuCCELScat      */ kNuE    ,
   /*   kmNuEToNuMuCCDIS         */ kNuE    ,
   /*   kmNuEToNuMuCCMpi         */ kNuE    ,
   /*   kmNuEToNuMuCCmisc        */ kNuE    ,
   /*   kmNuEToNuMuCCoth         */ kNuE    ,
   /*   kmNuEToNuTauCCQE         */ kNuE    ,
   /*   kmNuEToNuTauCCMEC        */ kNuE    ,
   /*   kmNuEToNuTauCC1piC       */ kNuE    ,
   /*   kmNuEToNuTauCC1pi0       */ kNuE    ,
   /*   kmNuEToNuTauCC2piC       */ kNuE    ,
   /*   kmNuEToNuTauCC2pi0       */ kNuE    ,
   /*   kmNuEToNuTauCCpiCpi0     */ kNuE    ,
   /*   kmNuEToNuTauCCcoh        */ kNuE    ,
   /*   kmNuEToNuTauCCELScat     */ kNuE    ,
   /*   kmNuEToNuTauCCDIS        */ kNuE    ,
   /*   kmNuEToNuTauCCMpi        */ kNuE    ,
   /*   kmNuEToNuTauCCmisc       */ kNuE    ,
   /*   kmNuEToNuTauCCoth        */ kNuE    ,
   /*   kmNuMuBarToNuEBarCCQE          */ kNuMuBar    ,
   /*   kmNuMuBarToNuEBarCCMEC         */ kNuMuBar    ,
   /*   kmNuMuBarToNuEBarCC1piC        */ kNuMuBar    ,
   /*   kmNuMuBarToNuEBarCC1pi0        */ kNuMuBar    ,
   /*   kmNuMuBarToNuEBarCC2piC        */ kNuMuBar    ,
   /*   kmNuMuBarToNuEBarCC2pi0        */ kNuMuBar    ,
   /*   kmNuMuBarToNuEBarCCpiCpi0      */ kNuMuBar    ,
   /*   kmNuMuBarToNuEBarCCcoh         */ kNuMuBar    ,
   /*   kmNuMuBarToNuEBarCCIMD         */ kNuMuBar    ,
   /*   kmNuMuBarToNuEBarELScat        */ kNuMuBar    ,
   /*   kmNuMuBarToNuEBarCCDIS         */ kNuMuBar    ,
   /*   kmNuMuBarToNuEBarCCMpi         */ kNuMuBar    ,
   /*   kmNuMuBarToNuEBarCCmisc        */ kNuMuBar    ,
   /*   kmNuMuBarToNuEBarCCoth         */ kNuMuBar    ,
   /*   kmNuMuBarToNuTauBarCCQE        */ kNuMuBar    ,
   /*   kmNuMuBarToNuTauBarCCMEC       */ kNuMuBar    ,
   /*   kmNuMuBarToNuTauBarCC1piC      */ kNuMuBar    ,
   /*   kmNuMuBarToNuTauBarCC1pi0      */ kNuMuBar    ,
   /*   kmNuMuBarToNuTauBarCC2piC      */ kNuMuBar    ,
   /*   kmNuMuBarToNuTauBarCC2pi0      */ kNuMuBar    ,
   /*   kmNuMuBarToNuTauBarCCpiCpi0    */ kNuMuBar    ,
   /*   kmNuMuBarToNuTauBarCCcoh       */ kNuMuBar    ,
   /*   kmNuMuBarToNuTauBarCCELScat    */ kNuMuBar    ,
   /*   kmNuMuBarToNuTauBarCCDIS       */ kNuMuBar    ,
   /*   kmNuMuBarToNuTauBarCCMpi       */ kNuMuBar    ,
   /*   kmNuMuBarToNuTauBarCCmisc      */ kNuMuBar    ,
   /*   kmNuMuBarToNuTauBarCCoth       */ kNuMuBar    ,
   /*   kmNuEBarToNuMuBarCCQE          */ kNuEBar    ,
   /*   kmNuEBarToNuMuBarCCMEC         */ kNuEBar    ,
   /*   kmNuEBarToNuMuBarCC1piC        */ kNuEBar    ,
   /*   kmNuEBarToNuMuBarCC1pi0        */ kNuEBar    ,
   /*   kmNuEBarToNuMuBarCC2piC        */ kNuEBar    ,
   /*   kmNuEBarToNuMuBarCC2pi0        */ kNuEBar    ,
   /*   kmNuEBarToNuMuBarCCpiCpi0      */ kNuEBar    ,
   /*   kmNuEBarToNuMuBarCCcoh         */ kNuEBar    ,
   /*   kmNuEBarToNuMuBarCCELScat      */ kNuEBar    ,
   /*   kmNuEBarToNuMuBarCCDIS         */ kNuEBar    ,
   /*   kmNuEBarToNuMuBarCCMpi         */ kNuEBar    ,
   /*   kmNuEBarToNuMuBarCCmisc        */ kNuEBar    ,
   /*   kmNuEBarToNuMuBarCCoth         */ kNuEBar    ,
   /*   kmNuEBarToNuTauBarCCQE         */ kNuEBar    ,
   /*   kmNuEBarToNuTauBarCCMEC        */ kNuEBar    ,
   /*   kmNuEBarToNuTauBarCC1piC       */ kNuEBar    ,
   /*   kmNuEBarToNuTauBarCC1pi0       */ kNuEBar    ,
   /*   kmNuEBarToNuTauBarCC2piC       */ kNuEBar    ,
   /*   kmNuEBarToNuTauBarCC2pi0       */ kNuEBar    ,
   /*   kmNuEBarToNuTauBarCCpiCpi0     */ kNuEBar    ,
   /*   kmNuEBarToNuTauBarCCcoh        */ kNuEBar    ,
   /*   kmNuEBarToNuTauBarCCELScat     */ kNuEBar    ,
   /*   kmNuEBarToNuTauBarCCDIS        */ kNuEBar    ,
   /*   kmNuEBarToNuTauBarCCMpi        */ kNuEBar    ,
   /*   kmNuEBarToNuTauBarCCmisc       */ kNuEBar    ,
   /*   kmNuEBarToNuTauBarCCoth        */ kNuEBar    ,
   /*   kmCosmic                       */ kCosmic    ,
   /*   kmDirt                         */ kDirt,
// /*   kmOther             */ kNuFlavourUndefined
};


} // conv


//
// Associated utilities
//
namespace util {

  std::string        AsString             (conv::ReacMode_t m);
  std::string        AsStringTeX          (conv::ReacMode_t m, bool root = false);
  std::string        AsString             (conv::ReacType_t t);
  std::string        AsStringTeX2         (conv::ReacType_t t, bool root = false);
  conv::ReacMode_t   ReacModeFromInt      (int m);
  conv::ReacMode_t   ReacModeFromString   (std::string s);
  conv::NuFlavour_t  ReacModeToInitFlavor (conv::ReacMode_t m);
  conv::NuFlavour_t  ReacModeToFinalFlavor(conv::ReacMode_t m);
  conv::ReacType_t   ReacModeToType       (conv::ReacMode_t m);
  conv::ReacMode_t   ReacTypeToMode       (conv::ReacType_t t, conv::NuFlavour_t nu_init, conv::NuFlavour_t nu_final);
  
  bool               IsCC               (conv::ReacMode_t m);
  bool               IsNuMu             (conv::ReacMode_t m);
  bool               IsNuMuBar          (conv::ReacMode_t m);
  bool               IsNuE              (conv::ReacMode_t m);
  bool               IsNuEBar           (conv::ReacMode_t m);
  bool               IsNuMuToNuE        (conv::ReacMode_t m);
  bool               IsNuMuToNuTau      (conv::ReacMode_t m);
  bool               IsNuEToNuMu        (conv::ReacMode_t m);
  bool               IsNuEToNuTau       (conv::ReacMode_t m);
  bool               IsNuEToNuMu        (conv::ReacMode_t m);
  bool               IsNuMuBarToNuEBar  (conv::ReacMode_t m);
  bool               IsNuMuBarToNuTauBar(conv::ReacMode_t m);
  bool               IsNuEBarToNuMuBar  (conv::ReacMode_t m);
  bool               IsNuEBarToNuTauBar (conv::ReacMode_t m);
  bool               IsCosmic           (conv::ReacMode_t m);
  bool               IsDirt             (conv::ReacMode_t m);
  bool               IsNeutrinoAny      (conv::ReacMode_t m);

  conv::ReacMode_t   NuEModeToOscNuEMode(conv::ReacMode_t m);

  // Aliases for above
  std::string      ModeAsString        (conv::ReacMode_t m);
  std::string      ModeAsStringTeX     (conv::ReacMode_t m, bool root = false);
  bool             ModeIsCC            (conv::ReacMode_t m);  
  std::string      ReacTypeAsString    (conv::ReacType_t t);
  std::string      ReacTypeAsStringTeX (conv::ReacType_t t, bool root = false);

} // util

} // valor
  
#endif 


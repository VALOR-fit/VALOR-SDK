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
#include "valor/Conventions/ReacMode.h"

using std::string;

namespace valor {
namespace conv {

typedef enum EReacModeCoarse
{
   kmcUndefined = -1, 
   kmcNuMuCC,             // numu     CC all
   kmcNuMuCCQEMEC,        // numu     CC QE+MEC
   kmcNuMuCCQE,           // numu     CC QE
   kmcNuMuCCMEC,          // numu     CC MEC
   kmcNuMuCC1piC,         // numu     CC 1 pipm
   kmcNuMuCC1pi0,         // numu     CC 1 pi0
   kmcNuMuCC1pi,          // numu     CC 1 pi any
   kmcNuMuCC2pi,          // numu     CC 2 pi
   kmcNuMuCCIMD,          // numu     IMD
   kmcNuMuCCElScat,       // numu     elastic
   kmcNuMuCCDIS,          // numu     CC Deep Inelastic Scattering
   kmcNuMuCCMpi,          // numu     CC multi pi (>1 pion of any charge(s))
   kmcNuMuCCmisc,         // numu     CC miscellaneous modes that require separate treatment to CC other
   kmcNuMuCCoth,          // numu     CC other
   kmcNuMuNC,             // numu     NC all
   kmcNuMuBarCC,          // numubar  CC all
   kmcNuMuBarCCQEMEC,     // numubar  CC QE+MEC
   kmcNuMuBarCCQE,        // numubar  CC QE
   kmcNuMuBarCCMEC,       // numubar  CC MEC
   kmcNuMuBarCC1piC,      // numubar  CC 1 pipm
   kmcNuMuBarCC1pi0,      // numubar  CC 1 pi0
   kmcNuMuBarCC1pi,       // numubar  CC 1 pi any
   kmcNuMuBarCC2pi,       // numubar  CC 2 pi
   kmcNuMuBarCCIMD,       // numubar  IMD
   kmcNuMuBarCCElScat,    // numubar  elastic
   kmcNuMuBarCCDIS,       // numubar  CC Deep Inelastic Scattering
   kmcNuMuBarCCMpi,       // numubar  CC multi pi (>1 pion of any charge(s))
   kmcNuMuBarCCmisc,      // numubar  CC miscellaneous modes that require separate treatment to CC other
   kmcNuMuBarCCoth,       // numubar  CC other
   kmcNuMuBarNC,          // numubar  NC all
   kmcNuENuEBarCC,        // nue(bar) CC all
   kmcNuECC,              // nue      CC all
   kmcAllButNuECC,              // nue CC bkg
   kmcNuECCQEMEC,         // nue      CC QE+MEC
   kmcNuECCQE,            // nue      CC QE
   kmcNuECCMEC,           // nue      CC MEC
   kmcNuECC1pi,           // nue      CC 1 pi
   kmcNuECC2pi,           // nue      CC 2 pi
   kmcNuECCDIS,           // nue      CC Deep Inelastic Scattering
   kmcNuECCMpi,           // nue      CC multi pi (>1 pion of any charge(s))
   kmcNuECCmisc,          // nue      CC miscellaneous modes that require separate treatment to CC other
   kmcNuECCoth,           // nue      CC other
   kmcNuEElScat,          // nue      elastic
   kmcNuENC,              // nue      NC all
   kmcNuEBarCC,           // nuebar   CC all
   kmcNuEBarCCQEMEC,      // nuebar   CC QE+MEC
   kmcNuEBarCCQE,         // nuebar   CC QE
   kmcNuEBarCCMEC,        // nuebar   CC MEC
   kmcNuEBarCC1pi,        // nuebar   CC 1 pi
   kmcNuEBarCC2pi,        // nuebar   CC 2 pi
   kmcNuEBarCCDIS,        // nuebar   CC Deep Inelastic Scattering
   kmcNuEBarCCMpi,        // nuebar   CC multi pi (>1 pion of any charge(s))
   kmcNuEBarCCmisc,       // nuebar   CC miscellaneous modes that require separate treatment to CC other
   kmcNuEBarCCoth,        // nuebar   CC other
   kmcNuEBarElScat,       // nuebar   elastic
   kmcNuEBarNC,           // nuebar   NC all
   kmcNuTauNuTauBarCC,    // nutau(bar) CC all
   kmcNC,                 // NC all
   kmcNC0pi,              // NC 0 pi
   kmcNC1piC,             // NC 1 piC
   kmcNC1pi0,             // NC 1 pi0
   kmcNC2pi,              // NC 2 pi
   kmcNCoth,              // NC other
   //for the far detector
   kmcOscNuMuCC,             // nue->numu     CC all
   kmcOscNuMuBarCC,          // nuebar->numubar  CC all
   kmcOscNuECC,              // numu->nue     CC all
   kmcUnOscNuECC,            // nue->nue     CC all
   kmcOscNuEBarCC,           // numubar->nuebar  CC all
   kmcUnOscNuEBarCC,         // nuebar->nuebar  CC all
   kmcNuTauCC,               // numu(nue)->nutau CC all (separated from nutaubar)
   kmcNuTauBarCC,            // numubar(nuebar)->nutaubar CC all (separated from nutau)
   kmcCosmic                 // cosmics
} 
ReacModeCoarse_t;

<<<<<<< Updated upstream
static const int kNReacModesCoarse = (int)kmcCosmic + 1;
=======
//static const int kNReacModesCoarse = (int)kmcDirt + 1;
static const int kNReacModesCoarse = 205;
>>>>>>> Stashed changes


// An array showing which ReacMode_ts go into which ReacModeCoarse_t (in order of priority
static const int kMaxCoarseReacOptions = 6;

static const ReacModeCoarse_t kCoarseReacPriority[kNReacModes][kMaxCoarseReacOptions] = 
{
   /* kmNuMuCCQE                   */ {kmcNuMuCCQE    , kmcNuMuCCQEMEC, kmcNuMuCCoth, kmcNuMuCC   , kmcAllButNuECC, kmcUndefined},
   /* kmNuMuCCMEC                  */ {kmcNuMuCCMEC   , kmcNuMuCCQEMEC, kmcNuMuCCoth, kmcNuMuCC   , kmcAllButNuECC, kmcUndefined},
   /* kmNuMuCC1piC                 */ {kmcNuMuCC1piC  , kmcNuMuCC1pi  , kmcNuMuCCoth, kmcNuMuCC   , kmcAllButNuECC, kmcUndefined},
   /* kmNuMuCC1pi0                 */ {kmcNuMuCC1pi0  , kmcNuMuCC1pi  , kmcNuMuCCoth, kmcNuMuCC   , kmcAllButNuECC, kmcUndefined},
   /* kmNuMuCC2piC                 */ {kmcNuMuCC2pi   , kmcNuMuCCoth  , kmcNuMuCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuCC2pi0                 */ {kmcNuMuCC2pi   , kmcNuMuCCoth  , kmcNuMuCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuCCpiCpi0               */ {kmcNuMuCC2pi   , kmcNuMuCCoth  , kmcNuMuCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuCCcoh                  */ {kmcNuMuCC1pi   , kmcNuMuCCoth  , kmcNuMuCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuCCIMD                  */ {kmcNuMuCCIMD   , kmcNuMuCCoth  , kmcNuMuCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuCCELScat               */ {kmcNuMuCCElScat, kmcNuMuCCoth  , kmcNuMuCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuCCDIS                  */ {kmcNuMuCCDIS   , kmcNuMuCCoth  , kmcNuMuCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuCCMpi                  */ {kmcNuMuCCMpi   , kmcNuMuCCoth  , kmcNuMuCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuCCmisc                 */ {kmcNuMuCCmisc  , kmcNuMuCCoth  , kmcNuMuCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuCCoth                  */ {kmcNuMuCCoth   , kmcNuMuCC     , kmcAllButNuECC, kmcUndefined, kmcUndefined, kmcUndefined},
   /* kmNuMuNCEL                   */ {kmcNC0pi       , kmcNCoth      , kmcNC       , kmcNuMuNC   , kmcAllButNuECC, kmcUndefined},
   /* kmNuMuNCMEC                  */ {kmcNC0pi       , kmcNCoth      , kmcNC       , kmcNuMuNC   , kmcAllButNuECC, kmcUndefined},
   /* kmNuMuNC1piC                 */ {kmcNC1piC      , kmcNCoth      , kmcNC       , kmcNuMuNC   , kmcAllButNuECC, kmcUndefined},
   /* kmNuMuNC1pi0                 */ {kmcNC1pi0      , kmcNCoth      , kmcNC       , kmcNuMuNC   , kmcAllButNuECC, kmcUndefined},
   /* kmNuMuNC2piC                 */ {kmcNC2pi       , kmcNCoth      , kmcNC       , kmcNuMuNC   , kmcAllButNuECC, kmcUndefined},
   /* kmNuMuNC2pi0                 */ {kmcNC2pi       , kmcNCoth      , kmcNC       , kmcNuMuNC   , kmcAllButNuECC, kmcUndefined},
   /* kmNuMuNCpiCpi0               */ {kmcNC2pi       , kmcNCoth      , kmcNC       , kmcNuMuNC   , kmcAllButNuECC, kmcUndefined},
   /* kmNuMuNCcoh                  */ {kmcNC1pi0      , kmcNCoth      , kmcNC       , kmcNuMuNC   , kmcAllButNuECC, kmcUndefined},
   /* kmNuMuNC1Gamma               */ {kmcNCoth       , kmcNuMuNC     , kmcNC       , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuNCELScat               */ {kmcNCoth       , kmcNC         , kmcNuMuNC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuNCoth                  */ {kmcNCoth       , kmcNC         , kmcNuMuNC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   
   /* kmNuMuBarCCQE                */ {kmcNuMuBarCCQE    , kmcNuMuBarCCQEMEC, kmcNuMuBarCCoth, kmcNuMuBarCC, kmcAllButNuECC, kmcUndefined},
   /* kmNuMuBarCCMEC               */ {kmcNuMuBarCCMEC   , kmcNuMuBarCCQEMEC, kmcNuMuBarCCoth, kmcNuMuBarCC, kmcAllButNuECC, kmcUndefined},
   /* kmNuMuBarCC1piC              */ {kmcNuMuBarCC1piC  , kmcNuMuBarCC1pi  , kmcNuMuBarCCoth, kmcNuMuBarCC, kmcAllButNuECC, kmcUndefined},
   /* kmNuMuBarCC1pi0              */ {kmcNuMuBarCC1pi0  , kmcNuMuBarCC1pi  , kmcNuMuBarCCoth, kmcNuMuBarCC, kmcAllButNuECC, kmcUndefined},
   /* kmNuMuBarCC2piC              */ {kmcNuMuBarCC2pi   , kmcNuMuBarCCoth  , kmcNuMuBarCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuBarCC2pi0              */ {kmcNuMuBarCC2pi   , kmcNuMuBarCCoth  , kmcNuMuBarCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuBarCCpiCpi0            */ {kmcNuMuBarCC2pi   , kmcNuMuBarCCoth  , kmcNuMuBarCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuBarCCcoh               */ {kmcNuMuBarCC1pi   , kmcNuMuBarCCoth  , kmcNuMuBarCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuBarCCELScat            */ {kmcNuMuBarCCElScat, kmcNuMuBarCCoth  , kmcNuMuBarCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuBarCCDIS               */ {kmcNuMuBarCCDIS   , kmcNuMuBarCCoth  , kmcNuMuBarCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuBarCCMpi               */ {kmcNuMuBarCCMpi   , kmcNuMuBarCCoth  , kmcNuMuBarCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuBarCCmisc              */ {kmcNuMuBarCCmisc  , kmcNuMuBarCCoth  , kmcNuMuBarCC   , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuBarCCoth               */ {kmcNuMuBarCCoth   , kmcNuMuBarCC     , kmcAllButNuECC   , kmcUndefined, kmcUndefined, kmcUndefined},
   /* kmNuMuBarNCEL                */ {kmcNC0pi    , kmcNCoth      , kmcNC       , kmcNuMuBarNC, kmcAllButNuECC, kmcUndefined},
   /* kmNuMuBarNCMEC               */ {kmcNC0pi    , kmcNCoth      , kmcNC       , kmcNuMuBarNC, kmcAllButNuECC, kmcUndefined},
   /* kmNuMuBarNC1piC              */ {kmcNC1piC   , kmcNCoth      , kmcNC       , kmcNuMuBarNC, kmcAllButNuECC, kmcUndefined},
   /* kmNuMuBarNC1pi0              */ {kmcNC1pi0   , kmcNCoth      , kmcNC       , kmcNuMuBarNC, kmcAllButNuECC, kmcUndefined},
   /* kmNuMuBarNC2piC              */ {kmcNCoth    , kmcNC         , kmcNuMuBarNC, kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuBarNC2pi0              */ {kmcNCoth    , kmcNC         , kmcNuMuBarNC, kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuBarNCpiCpi0            */ {kmcNCoth    , kmcNC         , kmcNuMuBarNC, kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuBarNCcoh               */ {kmcNC1pi0   , kmcNCoth      , kmcNC       , kmcNuMuBarNC, kmcAllButNuECC, kmcUndefined},
   /* kmNuMuBarNC1Gamma            */ {kmcNCoth    , kmcNuMuBarNC  , kmcNC       , kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuBarNCELScat            */ {kmcNCoth    , kmcNC         , kmcNuMuBarNC, kmcAllButNuECC, kmcUndefined, kmcUndefined},
   /* kmNuMuBarNCoth               */ {kmcNCoth    , kmcNC         , kmcNuMuBarNC, kmcAllButNuECC, kmcUndefined, kmcUndefined},
   
   /* kmNuECCQE                    */ {kmcNuECCQE  , kmcNuECCQEMEC , kmcNuECCoth   , kmcNuECC      , kmcNuENuEBarCC, kmcUnOscNuECC} ,
   /* kmNuECCMEC                   */ {kmcNuECCMEC , kmcNuECCQEMEC , kmcNuECCoth   , kmcNuECC      , kmcNuENuEBarCC, kmcUnOscNuECC} ,
   /* kmNuECC1piC                  */ {kmcNuECC1pi , kmcNuECCoth   , kmcNuECC      , kmcNuENuEBarCC, kmcUnOscNuECC , kmcUndefined} ,
   /* kmNuECC1pi0                  */ {kmcNuECC1pi , kmcNuECCoth   , kmcNuECC      , kmcNuENuEBarCC, kmcUnOscNuECC , kmcUndefined} ,
   /* kmNuECC2piC                  */ {kmcNuECC2pi , kmcNuECCoth   , kmcNuECC      , kmcNuENuEBarCC, kmcUnOscNuECC , kmcUndefined} ,
   /* kmNuECC2pi0                  */ {kmcNuECC2pi , kmcNuECCoth   , kmcNuECC      , kmcNuENuEBarCC, kmcUnOscNuECC , kmcUndefined} ,
   /* kmNuECCpiCpi0                */ {kmcNuECC2pi , kmcNuECCoth   , kmcNuECC      , kmcNuENuEBarCC, kmcUnOscNuECC , kmcUndefined} ,
   /* kmNuECCcoh                   */ {kmcNuECC1pi , kmcNuECCoth   , kmcNuECC      , kmcNuENuEBarCC, kmcUnOscNuECC , kmcUndefined} ,
   /* kmNuECCDIS                   */ {kmcNuECCDIS , kmcNuECCoth   , kmcNuECC      , kmcNuENuEBarCC, kmcUnOscNuECC , kmcUndefined} ,
   /* kmNuECCMpi                   */ {kmcNuECCMpi , kmcNuECCoth   , kmcNuECC      , kmcNuENuEBarCC, kmcUnOscNuECC , kmcUndefined} ,
   /* kmNuECCmisc                  */ {kmcNuECCmisc, kmcNuECCoth   , kmcNuECC      , kmcNuENuEBarCC, kmcUnOscNuECC , kmcUndefined} ,
   /* kmNuECCoth                   */ {kmcNuECCoth , kmcNuECC      , kmcNuENuEBarCC, kmcUnOscNuECC , kmcUndefined  , kmcUndefined} ,
   /* kmNuENCEL                    */ {kmcNC0pi    , kmcNCoth      , kmcNC         , kmcNuENC      , kmcAllButNuECC  , kmcUndefined},
   /* kmNuENCMEC                   */ {kmcNC0pi    , kmcNCoth      , kmcNC         , kmcNuENC      , kmcAllButNuECC  , kmcUndefined},
   /* kmNuENC1piC                  */ {kmcNC1piC   , kmcNCoth      , kmcNC         , kmcNuENC      , kmcAllButNuECC  , kmcUndefined},
   /* kmNuENC1pi0                  */ {kmcNC1pi0   , kmcNCoth      , kmcNC         , kmcNuENC      , kmcAllButNuECC  , kmcUndefined},
   /* kmNuENC2piC                  */ {kmcNC2pi    , kmcNCoth      , kmcNC         , kmcNuENC      , kmcAllButNuECC  , kmcUndefined},
   /* kmNuENC2pi0                  */ {kmcNC2pi    , kmcNCoth      , kmcNC         , kmcNuENC      , kmcAllButNuECC  , kmcUndefined},
   /* kmNuENCpiCpi0                */ {kmcNC2pi    , kmcNCoth      , kmcNC         , kmcNuENC      , kmcAllButNuECC  , kmcUndefined},
   /* kmNuENCcoh                   */ {kmcNC1pi0   , kmcNCoth      , kmcNC         , kmcNuENC      , kmcAllButNuECC  , kmcUndefined},
   /* kmNuENC1Gamma                */ {kmcNCoth    , kmcNuENC      , kmcNC         , kmcAllButNuECC  , kmcUndefined  , kmcUndefined},
   /* kmNuEELScat                  */ {kmcNuEElScat, kmcNCoth      , kmcNC         , kmcNuENC      , kmcAllButNuECC  , kmcUndefined},
   /* kmNuENCoth                   */ {kmcNCoth    , kmcNC         , kmcNuENC      , kmcAllButNuECC  , kmcUndefined  , kmcUndefined},
   
   /* kmNuEBarCCQE                 */ {kmcNuEBarCCQE  , kmcNuEBarCCQEMEC , kmcNuEBarCCoth, kmcNuEBarCC     , kmcNuENuEBarCC  , kmcNuECC} ,
   /* kmNuEBarCCMEC                */ {kmcNuEBarCCMEC , kmcNuEBarCCQEMEC , kmcNuEBarCCoth, kmcNuEBarCC     , kmcNuENuEBarCC  , kmcNuECC} ,
   /* kmNuEBarCC1piC               */ {kmcNuEBarCC1pi , kmcNuEBarCCoth   , kmcNuEBarCC   , kmcNuENuEBarCC  , kmcUnOscNuEBarCC, kmcNuECC} ,
   /* kmNuEBarCC1pi0               */ {kmcNuEBarCC1pi , kmcNuEBarCCoth   , kmcNuEBarCC   , kmcNuENuEBarCC  , kmcUnOscNuEBarCC, kmcNuECC} ,
   /* kmNuEBarCC2piC               */ {kmcNuEBarCC2pi , kmcNuEBarCCoth   , kmcNuEBarCC   , kmcNuENuEBarCC  , kmcUnOscNuEBarCC, kmcNuECC} ,
   /* kmNuEBarCC2pi0               */ {kmcNuEBarCC2pi , kmcNuEBarCCoth   , kmcNuEBarCC   , kmcNuENuEBarCC  , kmcUnOscNuEBarCC, kmcNuECC} ,
   /* kmNuEBarCCpiCpi0             */ {kmcNuEBarCC2pi , kmcNuEBarCCoth   , kmcNuEBarCC   , kmcNuENuEBarCC  , kmcUnOscNuEBarCC, kmcNuECC} ,
   /* kmNuEBarCCcoh                */ {kmcNuEBarCC1pi , kmcNuEBarCCoth   , kmcNuEBarCC   , kmcNuENuEBarCC  , kmcUnOscNuEBarCC, kmcNuECC} ,
   /* kmNuEBarCCIMD                */ {kmcNuEBarCCoth , kmcNuEBarCC      , kmcNuENuEBarCC, kmcUnOscNuEBarCC, kmcNuECC    , kmcUndefined} ,
   /* kmNuEBarCCDIS                */ {kmcNuEBarCCDIS , kmcNuEBarCCoth   , kmcNuEBarCC   , kmcNuENuEBarCC  , kmcUnOscNuEBarCC, kmcNuECC},
   /* kmNuEBarCCMpi                */ {kmcNuEBarCCMpi , kmcNuEBarCCoth   , kmcNuEBarCC   , kmcNuENuEBarCC  , kmcUnOscNuEBarCC, kmcNuECC},
   /* kmNuEBarCCmisc               */ {kmcNuEBarCCmisc, kmcNuEBarCCoth   , kmcNuEBarCC   , kmcNuENuEBarCC  , kmcUnOscNuEBarCC, kmcNuECC},
   /* kmNuEBarCCoth                */ {kmcNuEBarCCoth , kmcNuEBarCC      , kmcNuENuEBarCC, kmcUnOscNuEBarCC, kmcNuECC    , kmcUndefined} ,
   /* kmNuEBarNCEL                 */ {kmcNC0pi       , kmcNCoth         , kmcNC         , kmcNuEBarNC     , kmcAllButNuECC    , kmcUndefined},
   /* kmNuEBarNCMEC                */ {kmcNC0pi       , kmcNCoth         , kmcNC         , kmcNuEBarNC     , kmcAllButNuECC    , kmcUndefined},
   /* kmNuEBarNC1piC               */ {kmcNC1piC      , kmcNCoth         , kmcNC         , kmcNuEBarNC     , kmcAllButNuECC    , kmcUndefined},
   /* kmNuEBarNC1pi0               */ {kmcNC1pi0      , kmcNCoth         , kmcNC         , kmcNuEBarNC     , kmcAllButNuECC    , kmcUndefined},
   /* kmNuEBarNC2piC               */ {kmcNC2pi       , kmcNCoth         , kmcNC         , kmcNuEBarNC     , kmcAllButNuECC    , kmcUndefined},
   /* kmNuEBarNC2pi0               */ {kmcNC2pi       , kmcNCoth         , kmcNC         , kmcNuEBarNC     , kmcAllButNuECC    , kmcUndefined},
   /* kmNuEBarNCpiCpi0             */ {kmcNC2pi       , kmcNCoth         , kmcNC         , kmcNuEBarNC     , kmcAllButNuECC    , kmcUndefined},
   /* kmNuEBarNCcoh                */ {kmcNC1pi0      , kmcNCoth         , kmcNC         , kmcNuEBarNC     , kmcAllButNuECC    , kmcUndefined},
   /* kmNuMuNC1Gamma               */ {kmcNCoth       , kmcNuEBarNC      , kmcNC         , kmcAllButNuECC    , kmcUndefined    , kmcUndefined},
   /* kmNuEBarELScat               */ {kmcNuEBarElScat, kmcNCoth         , kmcNC         , kmcNuEBarNC     , kmcAllButNuECC    , kmcUndefined},
   /* kmNuEBarNCoth                */ {kmcNCoth       , kmcNC            , kmcNuEBarNC   , kmcAllButNuECC    , kmcUndefined    , kmcUndefined},
   
   /* kmNuMuToNuECCQE              */ {kmcOscNuECC     , kmcNuECC          , kmcNuENuEBarCC, kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuECCMEC             */ {kmcOscNuECC     , kmcNuECC          , kmcNuENuEBarCC, kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuECC1piC            */ {kmcOscNuECC     , kmcNuECC          , kmcNuENuEBarCC, kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuECC1pi0            */ {kmcOscNuECC     , kmcNuECC          , kmcNuENuEBarCC, kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuECC2piC            */ {kmcOscNuECC     , kmcNuECC          , kmcNuENuEBarCC, kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuECC2pi0            */ {kmcOscNuECC     , kmcNuECC          , kmcNuENuEBarCC, kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuECCpiCpi0          */ {kmcOscNuECC     , kmcNuECC          , kmcNuENuEBarCC, kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuECCcoh             */ {kmcOscNuECC     , kmcNuECC          , kmcNuENuEBarCC, kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuEELScat            */ {kmcOscNuECC     , kmcNuECC          , kmcNuENuEBarCC, kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuECCDIS             */ {kmcOscNuECC     , kmcNuECC          , kmcNuENuEBarCC, kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuECCMpi             */ {kmcOscNuECC     , kmcNuECC          , kmcNuENuEBarCC, kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuECCmisc            */ {kmcOscNuECC     , kmcNuECC          , kmcNuENuEBarCC, kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuECCoth             */ {kmcOscNuECC     , kmcNuECC          , kmcNuENuEBarCC, kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuTauCCQE            */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuTauCCMEC           */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuTauCC1piC          */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuTauCC1pi0          */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuTauCC2piC          */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuTauCC2pi0          */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuTauCCpiCpi0        */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuTauCCcoh           */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuTauCCELScat        */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuTauCCDIS           */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuTauCCMpi           */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuTauCCmisc          */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuToNuTauCCoth           */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuEToNuMuCCQE              */ {kmcOscNuMuCC    , kmcNuMuCCQE       , kmcNuMuCCQEMEC, kmcNuMuCCoth, kmcNuMuCC   , kmcUndefined },
   /* kmNuEToNuMuCCMEC             */ {kmcOscNuMuCC    , kmcNuMuCCMEC      , kmcNuMuCCQEMEC, kmcNuMuCCoth, kmcNuMuCC   , kmcUndefined },
   /* kmNuEToNuMuCC1piC            */ {kmcOscNuMuCC    , kmcNuMuCC1piC     , kmcNuMuCC1pi  , kmcNuMuCC   , kmcUndefined, kmcUndefined },
   /* kmNuEToNuMuCC1pi0            */ {kmcOscNuMuCC    , kmcNuMuCC1pi0     , kmcNuMuCC1pi  , kmcNuMuCC   , kmcUndefined, kmcUndefined },
   /* kmNuEToNuMuCC2piC            */ {kmcOscNuMuCC    , kmcNuMuCC2pi      , kmcNuMuCCoth  , kmcNuMuCC   , kmcUndefined, kmcUndefined },
   /* kmNuEToNuMuCC2pi0            */ {kmcOscNuMuCC    , kmcNuMuCC2pi      , kmcNuMuCCoth  , kmcNuMuCC   , kmcUndefined, kmcUndefined },
   /* kmNuEToNuMuCCpiCpi0          */ {kmcOscNuMuCC    , kmcNuMuCC2pi      , kmcNuMuCCoth  , kmcNuMuCC   , kmcUndefined, kmcUndefined },
   /* kmNuEToNuMuCCcoh             */ {kmcOscNuMuCC    , kmcNuMuCC1pi      , kmcNuMuCCoth  , kmcNuMuCC   , kmcUndefined, kmcUndefined },
   /* kmNuEToNuMuCCIMD             */ {kmcOscNuMuCC    , kmcNuMuCCIMD      , kmcNuMuCCoth  , kmcNuMuCC   , kmcUndefined, kmcUndefined },
   /* kmNuEToNuMuCCELScat          */ {kmcOscNuMuCC    , kmcNuMuCCElScat   , kmcNuMuCCoth  , kmcNuMuCC   , kmcUndefined, kmcUndefined },
   /* kmNuEToNuMuToNuECCDIS        */ {kmcOscNuMuCC    , kmcNuMuCCDIS      , kmcNuMuCCoth  , kmcNuMuCC   , kmcUndefined, kmcUndefined },
   /* kmNuEToNuMuToNuECCMpi        */ {kmcOscNuMuCC    , kmcNuMuCCMpi      , kmcNuMuCCoth  , kmcNuMuCC   , kmcUndefined, kmcUndefined },
   /* kmNuEToNuMuToNuECCmisc       */ {kmcOscNuMuCC    , kmcNuMuCCmisc     , kmcNuMuCCoth  , kmcNuMuCC   , kmcUndefined, kmcUndefined },
   /* kmNuEToNuMuCCoth             */ {kmcOscNuMuCC    , kmcNuMuCCoth      , kmcNuMuCC     , kmcNuMuCC   , kmcUndefined, kmcUndefined },
   /* kmNuEToNuTauCCQE             */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuEToNuTauCCMEC            */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuEToNuTauCC1piC           */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuEToNuTauCC1pi0           */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuEToNuTauCC2piC           */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuEToNuTauCC2pi0           */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuEToNuTauCCpiCpi0         */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuEToNuTauCCcoh            */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuEToNuTauCCELScat         */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuEToNuTauCCDIS            */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuEToNuTauCCMpi            */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuEToNuTauCCmisc           */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuEToNuTauCCoth            */ {kmcNuTauCC      , kmcNuTauNuTauBarCC, kmcUndefined  , kmcUndefined, kmcUndefined, kmcUndefined },
   /* kmNuMuBarToNuEBarCCQE        */ {kmcOscNuEBarCC  , kmcNuEBarCC       , kmcNuENuEBarCC, kmcNuECC, kmcUndefined, kmcUndefined },
   /* kmNuMuBarToNuEBarCCMEC       */ {kmcOscNuEBarCC  , kmcNuEBarCC       , kmcNuENuEBarCC, kmcNuECC, kmcUndefined, kmcUndefined },
   /* kmNuMuBarToNuEBarCC1piC      */ {kmcOscNuEBarCC  , kmcNuEBarCC       , kmcNuENuEBarCC, kmcNuECC, kmcUndefined, kmcUndefined },
   /* kmNuMuBarToNuEBarCC1pi0      */ {kmcOscNuEBarCC  , kmcNuEBarCC       , kmcNuENuEBarCC, kmcNuECC, kmcUndefined, kmcUndefined },
   /* kmNuMuBarToNuEBarCC2piC      */ {kmcOscNuEBarCC  , kmcNuEBarCC       , kmcNuENuEBarCC, kmcNuECC, kmcUndefined, kmcUndefined },
   /* kmNuMuBarToNuEBarCC2pi0      */ {kmcOscNuEBarCC  , kmcNuEBarCC       , kmcNuENuEBarCC, kmcNuECC, kmcUndefined, kmcUndefined },
   /* kmNuMuBarToNuEBarCCpiCpi0    */ {kmcOscNuEBarCC  , kmcNuEBarCC       , kmcNuENuEBarCC, kmcNuECC, kmcUndefined, kmcUndefined },
   /* kmNuMuBarToNuEBarCCcoh       */ {kmcOscNuEBarCC  , kmcNuEBarCC       , kmcNuENuEBarCC, kmcNuECC, kmcUndefined, kmcUndefined },
   /* kmNuMuBarToNuEBarCCIMD       */ {kmcOscNuEBarCC  , kmcNuEBarCC       , kmcNuENuEBarCC, kmcNuECC, kmcUndefined, kmcUndefined },
   /* kmNuMuBarToNuEBarELScat      */ {kmcOscNuEBarCC  , kmcNuEBarCC       , kmcNuENuEBarCC, kmcNuECC, kmcUndefined, kmcUndefined },
   /* kmNuMuBarToNuEBarCCDIS       */ {kmcOscNuEBarCC  , kmcNuEBarCC       , kmcNuENuEBarCC, kmcNuECC, kmcUndefined, kmcUndefined },
   /* kmNuMuBarToNuEBarCCMpi       */ {kmcOscNuEBarCC  , kmcNuEBarCC       , kmcNuENuEBarCC, kmcNuECC, kmcUndefined, kmcUndefined },
   /* kmNuMuBarToNuEBarCCmisc      */ {kmcOscNuEBarCC  , kmcNuEBarCC       , kmcNuENuEBarCC, kmcNuECC, kmcUndefined, kmcUndefined },
   /* kmNuMuBarToNuEBarCCoth       */ {kmcOscNuEBarCC  , kmcNuEBarCC       , kmcNuENuEBarCC, kmcNuECC, kmcUndefined, kmcUndefined },
   /* kmNuMuBarToNuTauBarCCQE      */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcNuECC     , kmcUndefined, kmcUndefined, kmcUndefined},
   /* kmNuMuBarToNuTauBarCCMEC     */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined, kmcUndefined, kmcUndefined},
   /* kmNuMuBarToNuTauBarCC1piC    */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined, kmcUndefined, kmcUndefined},
   /* kmNuMuBarToNuTauBarCC1pi0    */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined, kmcUndefined, kmcUndefined},
   /* kmNuMuBarToNuTauBarCC2piC    */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined, kmcUndefined, kmcUndefined},
   /* kmNuMuBarToNuTauBarCC2pi0    */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined, kmcUndefined, kmcUndefined},
   /* kmNuMuBarToNuTauBarCCpiCpi0  */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined, kmcUndefined, kmcUndefined},
   /* kmNuMuBarToNuTauBarCCcoh     */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined, kmcUndefined, kmcUndefined},
   /* kmNuMuBarToNuTauBarCCELScat  */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined, kmcUndefined, kmcUndefined},
   /* kmNuMuBarToNuTauBarCCDIS     */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined, kmcUndefined, kmcUndefined},
   /* kmNuMuBarToNuTauBarCCMpi     */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined, kmcUndefined, kmcUndefined},
   /* kmNuMuBarToNuTauBarCCmisc    */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined, kmcUndefined, kmcUndefined},
   /* kmNuMuBarToNuTauBarCCoth     */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined, kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuMuBarCCQE        */ {kmcOscNuMuBarCC , kmcNuMuBarCCQE    , kmcNuMuBarCCQEMEC, kmcNuMuBarCCoth,kmcNuMuBarCC, kmcUndefined},
   /* kmNuEBarToNuMuBarCCMEC       */ {kmcOscNuMuBarCC , kmcNuMuBarCCMEC   , kmcNuMuBarCCQEMEC, kmcNuMuBarCCoth,kmcNuMuBarCC, kmcUndefined},
   /* kmNuEBarToNuMuBarCC1piC      */ {kmcOscNuMuBarCC , kmcNuMuBarCC1piC  , kmcNuMuBarCC1pi  , kmcNuMuBarCC   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuMuBarCC1pi0      */ {kmcOscNuMuBarCC , kmcNuMuBarCC1pi0  , kmcNuMuBarCC1pi  , kmcNuMuBarCC   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuMuBarCC2piC      */ {kmcOscNuMuBarCC , kmcNuMuBarCC2pi   , kmcNuMuBarCCoth  , kmcNuMuBarCC   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuMuBarCC2pi0      */ {kmcOscNuMuBarCC , kmcNuMuBarCC2pi   , kmcNuMuBarCCoth  , kmcNuMuBarCC   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuMuBarCCpiCpi0    */ {kmcOscNuMuBarCC , kmcNuMuBarCC2pi   , kmcNuMuBarCCoth  , kmcNuMuBarCC   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuMuBarCCcoh       */ {kmcOscNuMuBarCC , kmcNuMuBarCC1pi   , kmcNuMuBarCCoth  , kmcNuMuBarCC   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuMuBarCCELScat    */ {kmcOscNuMuBarCC , kmcNuMuBarCCElScat, kmcNuMuBarCCoth  , kmcNuMuBarCC   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuMuBarCCDIS       */ {kmcOscNuMuBarCC , kmcNuMuBarCCDIS   , kmcNuMuBarCCoth  , kmcNuMuCC      , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuMuBarCCMpi       */ {kmcOscNuMuBarCC , kmcNuMuBarCCMpi   , kmcNuMuBarCCoth  , kmcNuMuCC      , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuMuBarCCmisc      */ {kmcOscNuMuBarCC , kmcNuMuBarCCmisc  , kmcNuMuBarCCoth  , kmcNuMuCC      , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuMuBarCCoth       */ {kmcOscNuMuBarCC , kmcNuMuBarCCoth   , kmcNuMuBarCC     , kmcUndefined   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuTauBarCCQE       */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuTauBarCCMEC      */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuTauBarCC1piC     */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuTauBarCC1pi0     */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuTauBarCC2piC     */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuTauBarCC2pi0     */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuTauBarCCpiCpi0   */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuTauBarCCcoh      */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuTauBarCCELScat   */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuTauBarCCDIS      */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuTauBarCCMpi      */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuTauBarCCmisc     */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined   , kmcUndefined, kmcUndefined},
   /* kmNuEBarToNuTauBarCCoth      */ {kmcNuTauBarCC   , kmcNuTauNuTauBarCC, kmcUndefined     , kmcUndefined   , kmcUndefined, kmcUndefined},
<<<<<<< Updated upstream
   /*kmCosmic                      */ {kmcCosmic       , kmcUndefined      , kmcUndefined     , kmcUndefined   , kmcUndefined, kmcUndefined}
// /* kmOther                      */ {kmcUndefined , kmcUndefined   , kmcUndefined, kmcUndefined, kmcUndefined },
=======
   /* kmCosmic                     */ {kmcCosmic       , kmcAllButNuECC      , kmcUndefined     , kmcUndefined   , kmcUndefined, kmcUndefined},
   /* kmDirt                       */ {kmcDirt         , kmcAllButNuECC      , kmcUndefined     , kmcUndefined   , kmcUndefined, kmcUndefined},
   /* kmNuECCQE                    */ {kmcAllButNuECC  , kmcNuECCoth   , kmcUndefined      , kmcUndefined, kmcUndefined, kmcUndefined}
// /* kmOther                      */ {kmcAllButNuECC , kmcUndefined   , kmcUndefined, kmcUndefined, kmcUndefined },
>>>>>>> Stashed changes

}; //

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


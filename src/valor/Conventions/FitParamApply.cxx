//________________________________________________________________________________________
/*

Copyright (c) 2010-2018, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#include <utility>

#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Conventions/FitParamApply.h"

using namespace valor;
using namespace valor::conv;

// Groups of systematics
// Beam
const BeamApplyMask_t valor::conv::kbApplyToAll = kbApplyToFHC | kbApplyToRHC;

// Nu flavour
const FlavApplyMask_t valor::conv::kfApplyToNuAny    = kfApplyToNuMu    | kfApplyToNuE   ; // NuAny means any non-barred
const FlavApplyMask_t valor::conv::kfApplyToNuBarAny = kfApplyToNuMuBar | kfApplyToNuEBar;
const FlavApplyMask_t valor::conv::kfApplyToNuEAny   = kfApplyToNuE     | kfApplyToNuEBar;
const FlavApplyMask_t valor::conv::kfApplyToNuMuAny  = kfApplyToNuMu    | kfApplyToNuMuBar;
const FlavApplyMask_t valor::conv::kfApplyToAll      = kfApplyToNuAny   | kfApplyToNuBarAny;

// Reaction type
const ReacTypeApplyMask_t valor::conv::ktApplyToCCQELike    = ktApplyToCCQE     | ktApplyToCCMEC;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllCCELScat = ktApplyToCCELScat | ktApplyToNuEELScat; // nu + e CC elastic scattering (inverse muon/tau decay) and nue+e
const ReacTypeApplyMask_t valor::conv::ktApplyToAllCC1pi    = ktApplyToCC1piC   | ktApplyToCC1pi0;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllCC2pi    = ktApplyToCC2piC   | ktApplyToCC2pi0 | ktApplyToCCpiCpi0;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllCC       = ktApplyToCCQELike | ktApplyToAllCC1pi | ktApplyToAllCC2pi | ktApplyToCCcoh | ktApplyToCCoth | ktApplyToCCELScat;

const ReacTypeApplyMask_t valor::conv::ktApplyToNCElLike    = ktApplyToNCEL     | ktApplyToNCMEC; //this is not used, modes disabled
const ReacTypeApplyMask_t valor::conv::ktApplyToAllNCELScat = ktApplyToNCELScat | ktApplyToNuEELScat; //nu+e NC elastic scattering
const ReacTypeApplyMask_t valor::conv::ktApplyToAllNC1pi    = ktApplyToNC1piC   | ktApplyToNC1pi0;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllNC2pi    = ktApplyToNC2piC   | ktApplyToNC2pi0   | ktApplyToNCpiCpi0;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllNC       = ktApplyToNCElLike | ktApplyToAllNC1pi | ktApplyToAllNC2pi | ktApplyToNCcoh | ktApplyToNCoth | ktApplyToNCELScat;

const ReacTypeApplyMask_t valor::conv::ktApplyToAllCoh = ktApplyToCCcoh | ktApplyToNCcoh;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllELScat = ktApplyToCCELScat | ktApplyToNuEELScat | ktApplyToNCELScat;

const ReacTypeApplyMask_t valor::conv::ktApplyToAll = ktApplyToAllCC | ktApplyToAllNC | ktApplyToAllELScat;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllButCoh    = ktApplyToAll & (~ktApplyToAllCoh);
const ReacTypeApplyMask_t valor::conv::ktApplyToAllButELScat = ktApplyToAll & (~ktApplyToAllELScat);
const ReacTypeApplyMask_t valor::conv::ktApplyToAllWithFSI   = ktApplyToAll & (~ktApplyToAllCoh) & (~ktApplyToAllELScat);

// Detectors
const DetectorApplyMask_t valor::conv::kdApplyToAllDUNE = kdApplyToDUNEND | kdApplyToDUNEFD;
const DetectorApplyMask_t valor::conv::kdApplyToAllSBN  = kdApplyToSBND | kdApplyToMicroBooNE | kdApplyToICARUS;
const DetectorApplyMask_t valor::conv::kdApplyToAllSBN2Det  = kdApplyToSBND | kdApplyToMicroBooNE;
const DetectorApplyMask_t valor::conv::kdApplyToAllND  = kdApplyToDUNEND | kdApplyToSBND | kdApplyToMicroBooNE | kdApplyToICARUS;
const DetectorApplyMask_t valor::conv::kdApplyToAll    = kdApplyToAllDUNE | kdApplyToAllSBN;


// Samples
const SelecApplyMask_t valor::conv::ksApplyToAllND = (
  ksApplyToNuMuCC | ksApplyToNuMuCC0pi1trk| ksApplyToNuMuCC0pi2trk | ksApplyToNuMuCC0piNtrk | 
  ksApplyToNuMuCC3trkDelta | ksApplyToNuMuCC1piC | ksApplyToNuMuCC2piC | ksApplyToNuMuCC1pi0 | 
  ksApplyToNuMuCC1piC1pi0 | ksApplyToNuMuCCoth | ksApplyToNuMuCCWS0pi | ksApplyToNuMuCCWS1piC | ksApplyToNuMuCCWS1pi0 | 
  ksApplyToNuMuCCWSoth | ksApplyToNuECC0pi | ksApplyToNuECC1piC | ksApplyToNuECC1pi0 | ksApplyToNuECCoth | 
  ksApplyToNC0pi | ksApplyToNC1piC | ksApplyToNC1pi0 | ksApplyToNCoth | ksApplyToNeutrinoElec | ksApplyToInvMuDecay | 
  ksApplyToNuECCWS | ksApplyToNuECC | ksApplyToNuMuCCWS | ksApplyToNC
);

const SelecApplyMask_t valor::conv::ksApplyToAllFD = ksApplyToLowRes | ksApplyToHighRes;
const SelecApplyMask_t valor::conv::ksApplyToAll = ksApplyToAllND | ksApplyToAllFD;
const SelecApplyMask_t valor::conv::ksApplyToAllNC =  ( ksApplyToNC0pi | ksApplyToNC1piC | ksApplyToNC1pi0 | ksApplyToNCoth );
const SelecApplyMask_t valor::conv::ksApplyToAllNuECC =  (ksApplyToNuECC0pi      | ksApplyToNuECC1piC    | ksApplyToNuECC1pi0      | ksApplyToNuECCoth);

// Targets
const NuclearTargetApplyMask_t valor::conv::kntApplyToAll = (
  kntApplyToInclusive | kntApplyToOther | kntApplyToHydrogen | kntApplyToCarbon | kntApplyToOxygen | kntApplyToArgon | kntApplyToCalcium
);

// Which masks apply to which beam modes
// This is defined for all samples in the BeamConf_t enum.
const BeamApplyMask_t valor::conv::kApplyToBeamMode[kNBeamConf] = {
  /* kbLBNF_FHC */ kbApplyToFHC ,
  /* kbLBNF_RHC */ kbApplyToRHC ,
};


// Which masks apply to which samples
// This is defined for all samples in the Sample_t enum.
const SelecApplyMask_t valor::conv::kApplyToSample[kMaxNSamples] = {
  /* ksNuMuCC         */ ksApplyToNuMuCC          ,
  /* ksNuMuCC0pi1trk  */ ksApplyToNuMuCC0pi1trk   ,
  /* ksNuMuCC0pi2trk  */ ksApplyToNuMuCC0pi2trk   ,
  /* ksNuMuCC1piC     */ ksApplyToNuMuCC1piC      ,
  /* ksNuMuCC2piC     */ ksApplyToNuMuCC2piC      ,
  /* ksNuMuCC1pi0     */ ksApplyToNuMuCC1pi0      ,
  /* ksNuMuCC1piC1pi0 */ ksApplyToNuMuCC1piC1pi0  ,
  /* ksNuMuCCoth      */ ksApplyToNuMuCCoth       ,
  /* ksNuECC          */ ksApplyToNuECC           ,
  /* ksNuMuCCWS       */ ksApplyToNuMuCCWS        ,
  /* ksNC             */ ksApplyToNC              ,
  /* ksNuMuCC0piNtrk  */ ksApplyToNuMuCC0piNtrk   ,
  /* ksNuMuCC3trkDelta*/ ksApplyToNuMuCC3trkDelta ,
  /* ksNuMuCCWS0pi    */ ksApplyToNuMuCCWS0pi     ,
  /* ksNuMuCCWS1piC   */ ksApplyToNuMuCCWS1piC    ,
  /* ksNuMuCCWS1pi0   */ ksApplyToNuMuCCWS1pi0    ,
  /* ksNuMuCCWSoth    */ ksApplyToNuMuCCWSoth     ,
  /* ksNuECC0pi       */ ksApplyToNuECC0pi        ,
  /* ksNuECC1piC      */ ksApplyToNuECC1piC       ,
  /* ksNuECC1pi0      */ ksApplyToNuECC1pi0       ,
  /* ksNuECCoth       */ ksApplyToNuECCoth        ,
  /* ksNC0pi          */ ksApplyToNC0pi           ,
  /* ksNC1piC         */ ksApplyToNC1piC          ,
  /* ksNC1pi0         */ ksApplyToNC1pi0          ,
  /* ksNCoth          */ ksApplyToNCoth           ,
  /* ksNeutrinoElec   */ ksApplyToNuECC           , // ksApplyToNeutrinoElec    ,
  /* ksInvMuDecay     */ ksApplyToNuMuCCoth       , // ksApplyToInvMuDecay      ,
  /* ksNuECCWS        */ ksApplyToNuECCWS         ,
  /* ksNuMuLikeCChigh */ ksApplyToHighRes         ,
  /* ksNuMuLikeCClow  */ ksApplyToLowRes          ,
  /* ksNuMuBarCChigh  */ ksApplyToHighRes         ,
  /* ksNuMuBarCClow   */ ksApplyToLowRes          ,
  /* ksNuELikeCChigh  */ ksApplyToHighRes         ,
  /* ksNuELikeCClow   */ ksApplyToLowRes          ,
  /* ksNuEBarCChigh   */ ksApplyToHighRes         ,
  /* ksNuEBarCClow    */ ksApplyToLowRes
};

// Which masks apply to which parent particle flavour
const DetectorApplyMask_t valor::conv::kApplyToDetector[conv::kNDetectorsDefined] = {
  /* kdDUNEND    */  kdApplyToDUNEND,
  /* kdDUNEFD1ph */  kdApplyToDUNEFD,
  /* kdDUNEFD2ph */  kdApplyToDUNEFD,
  /* kdSBND      */  kdApplyToSBND,
  /* kdSBNuBooNE */  kdApplyToMicroBooNE,
  /* kdSBNICARUS */  kdApplyToICARUS,
  /* kdSuperK    */  0,
  /* kdHyperK    */  0,
  /* kdND280     */  0,
  /* kdND280Up   */  0,
  /* kdINGRID    */  0,
  /* kdNuPRISM   */  0,
  /* kdNuTIT     */  0
};

// Which masks apply to which parent particle flavour
const FlavApplyMask_t valor::conv::kApplyToFlavour[conv::kNNuFlavour] = {
  /* kNuE     */ kfApplyToNuE     ,
  /* kNuMu    */ kfApplyToNuMu    ,
  /* kNuTau   */ 0                ,
  /* kNuEBar  */ kfApplyToNuEBar  ,
  /* kNuMuBar */ kfApplyToNuMuBar ,
  /* kNuTau   */ 0                ,
};

// Which masks apply to which reaction type
const ReacTypeApplyMask_t valor::conv::kApplyToReacType[conv::kNReacType] = {
  /* kmtCCQE,     */ ktApplyToCCQE    ,
  /* kmtCCMEC,    */ ktApplyToCCMEC   ,
  /* kmtCC1piC,   */ ktApplyToCC1piC  ,
  /* kmtCC1pi0,   */ ktApplyToCC1pi0  ,
  /* kmtCC2piC,   */ ktApplyToCC2piC  ,
  /* kmtCC2pi0,   */ ktApplyToCC2pi0  ,
  /* kmtCCpiCpi0, */ ktApplyToCCpiCpi0,
  /* kmtCCcoh,    */ ktApplyToCCcoh   ,
  /* kmtCCIMD,    */ ktApplyToCCELScat,
  /* kmtCCELScat, */ ktApplyToCCELScat,
  /* kmtCCoth,    */ ktApplyToCCoth,
  /* kmtNCEL,     */ ktApplyToNCEL    ,
  /* kmtNCMEC,    */ ktApplyToNCMEC   | ktApplyToNCEL,
  /* kmtNC1piC,   */ ktApplyToNC1piC  | ktApplyToNCoth, //why | ktApplyToNCoth ?
  /* kmtNC1pi0,   */ ktApplyToNC1pi0  | ktApplyToNCoth,
  /* kmtNC2piC,   */ ktApplyToNC2piC  | ktApplyToNCoth,
  /* kmtNC2pi0,   */ ktApplyToNC2pi0  | ktApplyToNCoth,
  /* kmtNCpiCpi0, */ ktApplyToNCpiCpi0| ktApplyToNCoth,
  /* kmtNCcoh,    */ ktApplyToNCcoh   | ktApplyToNCoth,
  /* kmtNCELScat, */ ktApplyToNCELScat,
  /* kmtNCoth,    */ ktApplyToNCoth   ,
  /* kmtELScat  , */ ktApplyToNuEELScat,
  /* kmtOther     */ 0
};

// Which masks apply to which nucleus
const NuclearTargetApplyMask_t valor::conv::kApplyToNuclearTarget[conv::kNNuclearTarget] = {
  /* kntInclusive */ kntApplyToInclusive ,
  /* kntOther     */ kntApplyToOther     ,
  /* kntHydrogen  */ kntApplyToHydrogen  ,
  /* kntCarbon    */ kntApplyToCarbon    ,
  /* kntOxygen    */ kntApplyToOxygen    ,
  /* kntArgon     */ kntApplyToArgon     ,
  /* kntCalcium   */ kntApplyToCalcium
};

//________________________________________________________________________________________
valor::util::ApplyMaskNaming * valor::util::ApplyMaskNaming::fInstance = 0;
//________________________________________________________________________________________
valor::util::ApplyMaskNaming::ApplyMaskNaming()
{
  this->Init();
  fInstance = 0;
}
//________________________________________________________________________________________
const valor::util::ApplyMaskNaming * valor::util::ApplyMaskNaming::Instance()
{
  return MutableInstance();
}
//________________________________________________________________________________________
valor::util::ApplyMaskNaming * valor::util::ApplyMaskNaming::MutableInstance()
{
  if(fInstance == 0) {
    static valor::util::ApplyMaskNaming::Cleaner cleaner;
    cleaner.DummyMethodAndSilentCompiler();
    fInstance = new valor::util::ApplyMaskNaming;
  }
  return fInstance;
}
//________________________________________________________________________________________
void valor::util::ApplyMaskNaming::Init(void)
{
  // Beam Mask names
  fBeamMaskToName.insert( std::make_pair(kbApplyToFHC, "fhc") );
  fBeamMaskToName.insert( std::make_pair(kbApplyToRHC, "rhc") );
  fBeamMaskToName.insert( std::make_pair(kbApplyToAll, "all") );
  for(auto const& pair : fBeamMaskToName) {
    fBeamNameToMask.insert( std::make_pair(pair.second, pair.first) );
  }

  // Flavour mask names
  fFlavMaskToName.insert( std::make_pair(kfApplyToNuMu    , "numu") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToNuMuBar , "numubar") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToNuE     , "nue") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToNuEBar  , "nuebar") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToNuAny   , "nuany") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToNuBarAny, "nubarany") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToNuEAny  , "nueany") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToNuMuAny , "numuany") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToAll     , "all") );
  for(auto const& pair : fFlavMaskToName) {
    fFlavNameToMask.insert( std::make_pair(pair.second, pair.first) );
  }

  // Selection mask names
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuMuCC         , "numucc") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuMuCC0pi1trk  , "numucc0pi1trk") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuMuCC0pi2trk  , "numucc0pi2trk") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuMuCC0piNtrk  , "numucc0pintrk") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuMuCC3trkDelta, "numucc3trkdelta") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuMuCC1piC     , "numucc1pic") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuMuCC2piC     , "numucc2pic") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuMuCC1pi0     , "numucc1pi0") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuMuCC1piC1pi0 , "numucc1pic1pi0") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuMuCCoth      , "numuccoth") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuMuCCWS0pi    , "numuccws0pi") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuMuCCWS1piC   , "numuccws1pic") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuMuCCWS1pi0   , "numuccws1pi0") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuMuCCWSoth    , "numuccwsoth") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuECC0pi       , "nuecc0pi") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuECC1piC      , "nuecc1pic") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuECC1pi0      , "nuecc1pi0") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuECCoth       , "nueccoth") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNC0pi          , "nc0pi") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNC1piC         , "nc1pic") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNC1pi0         , "nc1pi0") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNCoth          , "ncoth") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNeutrinoElec   , "neutrinoelec") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToInvMuDecay     , "invmudecay") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuECCWS        , "nueccws") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuECC          , "nuecc") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNuMuCCWS       , "numuccws") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNC             , "nc") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToHighRes        , "highres") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToLowRes         , "lowres") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToAllND          , "allnd") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToAllFD          , "allfd") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToAll            , "all") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToAllNC          , "allnc") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToAllNuECC       , "allnuecc") );
  for(auto const& pair : fSelecMaskToName) {
    fSelecNameToMask.insert( std::make_pair(pair.second, pair.first) );
  }

  // Reaction type mask names
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCQE        , "ccqe") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCMEC       , "ccmec") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCC1piC      , "cc1pic") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCC1pi0      , "cc1pi0") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCC2piC      , "cc2pic") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCC2pi0      , "cc2pi0") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCpiCpi0    , "ccpicpi0") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCcoh       , "cccoh") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCELScat    , "ccelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCoth       , "ccoth") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNCEL        , "ncel") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNCMEC       , "ncmec") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNC1piC      , "nc1pic") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNC1pi0      , "nc1pi0") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNC2piC      , "nc2pic") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNC2pi0      , "nc2pi0") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNCpiCpi0    , "ncpicpi0") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNCcoh       , "nccoh") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNCELScat    , "ncelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNCoth       , "ncoth") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNuEELScat   , "nueelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCQELike    , "ccqelike") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCCELScat , "allccelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCC1pi    , "allcc1pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCC2pi    , "allcc2pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCC       , "allcc") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNCElLike    , "ncellike") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllNCELScat , "allncelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllNC1pi    , "allnc1pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllNC2pi    , "allnc2pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllNC       , "allnc") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCoh      , "allcoh") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllELScat   , "allelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAll         , "all") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllButCoh   , "allbutcoh") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllButELScat, "allbutelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllWithFSI  , "allwithfsi") );
  for(auto const& pair : fReacTypeMaskToName) {
    fReacTypeNameToMask.insert( std::make_pair(pair.second, pair.first) );
  }

  // Detector mask names
  fDetectorMaskToName.insert( std::make_pair(kdApplyToDUNEND    , "dunend") );
  fDetectorMaskToName.insert( std::make_pair(kdApplyToDUNEFD    , "dunefd") );
  fDetectorMaskToName.insert( std::make_pair(kdApplyToSBND      , "sbnd") );
  fDetectorMaskToName.insert( std::make_pair(kdApplyToMicroBooNE, "microboone") );
  fDetectorMaskToName.insert( std::make_pair(kdApplyToICARUS    , "icarus") );
  fDetectorMaskToName.insert( std::make_pair(kdApplyToAllDUNE   , "alldune") );
  fDetectorMaskToName.insert( std::make_pair(kdApplyToAllSBN    , "allsbn") );
  fDetectorMaskToName.insert( std::make_pair(kdApplyToAllSBN2Det, "allsbn2det") );
  fDetectorMaskToName.insert( std::make_pair(kdApplyToAllND     , "allnd") );
  fDetectorMaskToName.insert( std::make_pair(kdApplyToAll       , "all") );
  for(auto const& pair : fDetectorMaskToName) {
    fDetectorNameToMask.insert( std::make_pair(pair.second, pair.first) );
  }

  // Nuclear target mask names
  fNuclearTargetMaskToName.insert( std::make_pair(kntApplyToInclusive, "inclusive") );
  fNuclearTargetMaskToName.insert( std::make_pair(kntApplyToOther    , "other") );
  fNuclearTargetMaskToName.insert( std::make_pair(kntApplyToHydrogen , "hydrogen") );
  fNuclearTargetMaskToName.insert( std::make_pair(kntApplyToCarbon   , "carbon") );
  fNuclearTargetMaskToName.insert( std::make_pair(kntApplyToOxygen   , "oxygen") );
  fNuclearTargetMaskToName.insert( std::make_pair(kntApplyToArgon    , "argon") );
  fNuclearTargetMaskToName.insert( std::make_pair(kntApplyToCalcium  , "calcium") );
  fNuclearTargetMaskToName.insert( std::make_pair(kntApplyToAll      , "all") );
  for(auto const& pair : fNuclearTargetMaskToName) {
    fNuclearTargetNameToMask.insert( std::make_pair(pair.second, pair.first) );
  }
}
//________________________________________________________________________________________
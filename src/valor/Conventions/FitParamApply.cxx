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
const BeamApplyMask_t valor::util::ApplyToBeamMode(BeamConf_t b)
{
  switch (b)
  {
    case kbLBNF_FHC  : return kbApplyToFHC;
    case kbLBNF_RHC  : return kbApplyToRHC;
    case kbJPARC_FHC : return kbApplyToFHC;
    case kbJPARC_RHC : return kbApplyToRHC;
    case kbNuMI_FHC  : return kbApplyToFHC;
    case kbNuMI_RHC  : return kbApplyToRHC;
    case kbBNB_FHC   : return kbApplyToFHC;
    case kbBNB_RHC   : return kbApplyToRHC;
    default:
      LOG("ApplyMask",pWARN) << "No beam apply mask set for " << util::AsString(b) << ". Applying to none.";
      return 0;
  }
}
//________________________________________________________________________________________
const SelecApplyMask_t valor::util::ApplyToSample(conv::Sample_t s)
{
  switch (s) {
    case ksNuMuCC          : return ksApplyToNuMuCC;
    case ksNuMuCC0pi1trk   : return ksApplyToNuMuCC0pi1trk;
    case ksNuMuCC0pi2trk   : return ksApplyToNuMuCC0pi2trk;
    case ksNuMuCC1piC      : return ksApplyToNuMuCC1piC;
    case ksNuMuCC2piC      : return ksApplyToNuMuCC2piC;
    case ksNuMuCC1pi0      : return ksApplyToNuMuCC1pi0;
    case ksNuMuCC1piC1pi0  : return ksApplyToNuMuCC1piC1pi0;
    case ksNuMuCCoth       : return ksApplyToNuMuCCoth;
    case ksNuECC           : return ksApplyToNuECC;
    case ksNuMuCCWS        : return ksApplyToNuMuCCWS;
    case ksNC              : return ksApplyToNC;
    case ksNuMuCC0piNtrk   : return ksApplyToNuMuCC0piNtrk;
    case ksNuMuCC3trkDelta : return ksApplyToNuMuCC3trkDelta;
    case ksNuMuCCWS0pi     : return ksApplyToNuMuCCWS0pi;
    case ksNuMuCCWS1piC    : return ksApplyToNuMuCCWS1piC;
    case ksNuMuCCWS1pi0    : return ksApplyToNuMuCCWS1pi0;
    case ksNuMuCCWSoth     : return ksApplyToNuMuCCWSoth;
    case ksNuECC0pi        : return ksApplyToNuECC0pi;
    case ksNuECC1piC       : return ksApplyToNuECC1piC;
    case ksNuECC1pi0       : return ksApplyToNuECC1pi0;
    case ksNuECCoth        : return ksApplyToNuECCoth;
    case ksNC0pi           : return ksApplyToNC0pi;
    case ksNC1piC          : return ksApplyToNC1piC;
    case ksNC1pi0          : return ksApplyToNC1pi0;
    case ksNCoth           : return ksApplyToNCoth;
    case ksNeutrinoElec    : return ksApplyToNuECC;     // ksApplyToNeutrinoElec
    case ksInvMuDecay      : return ksApplyToNuMuCCoth; // ksApplyToInvMuDecay
    case ksNuECCWS         : return ksApplyToNuECCWS;
    case ksNuMuLikeCChigh  : return ksApplyToHighRes;
    case ksNuMuLikeCClow   : return ksApplyToLowRes;
    case ksNuMuBarCChigh   : return ksApplyToHighRes;
    case ksNuMuBarCClow    : return ksApplyToLowRes;
    case ksNuELikeCChigh   : return ksApplyToHighRes;
    case ksNuELikeCClow    : return ksApplyToLowRes;
    case ksNuEBarCChigh    : return ksApplyToHighRes;
    case ksNuEBarCClow     : return ksApplyToLowRes;
    default:
      LOG("ApplyMask",pWARN) << "No selection apply mask set for " << util::AsString(s) << ". Applying to none.";
      return 0;
  }
}
//________________________________________________________________________________________
const DetectorApplyMask_t valor::util::ApplyToDetector(conv::Detector_t d) 
{
  switch (d) {
    case kdDUNEND    : return kdApplyToDUNEND;
    case kdDUNEFD1ph : return kdApplyToDUNEFD;
    case kdDUNEFD2ph : return kdApplyToDUNEFD;
    case kdSBND      : return kdApplyToSBND;
    case kdSBNuBooNE : return kdApplyToMicroBooNE;
    case kdSBNICARUS : return kdApplyToICARUS;
    case kdSuperK    : return 0;
    case kdHyperK    : return 0;
    case kdND280     : return 0;
    case kdND280Up   : return 0;
    case kdINGRID    : return 0;
    case kdNuPRISM   : return 0;
    case kdNuTIT     : return 0;
    default:
      LOG("ApplyMask",pWARN) << "No detector apply mask set for " << util::AsString(d) << ". Applying to none.";
      return 0;
  }
}
//________________________________________________________________________________________
const FlavApplyMask_t valor::util::ApplyToFlavour(conv::NuFlavour_t f)
{
  switch (f) {
    case kNuE      : return kfApplyToNuE;
    case kNuMu     : return kfApplyToNuMu;
    case kNuTau    : return 0;
    case kNuEBar   : return kfApplyToNuEBar;
    case kNuMuBar  : return kfApplyToNuMuBar;
    case kNuTauBar : return 0;
    default:
      LOG("ApplyMask",pWARN) << "No flavour apply mask set for " << util::AsString(f) << ". Applying to none.";
      return 0;
  }
}
//________________________________________________________________________________________
const ReacTypeApplyMask_t valor::util::ApplyToReacType(conv::ReacType_t t) 
{
  switch (t) {
    case kmtCCQE     : return ktApplyToCCQE;
    case kmtCCMEC    : return ktApplyToCCMEC;
    case kmtCC1piC   : return ktApplyToCC1piC;
    case kmtCC1pi0   : return ktApplyToCC1pi0;
    case kmtCC2piC   : return ktApplyToCC2piC;
    case kmtCC2pi0   : return ktApplyToCC2pi0;
    case kmtCCpiCpi0 : return ktApplyToCCpiCpi0;
    case kmtCCcoh    : return ktApplyToCCcoh;
    case kmtCCIMD    : return ktApplyToCCELScat;
    case kmtCCELScat : return ktApplyToCCELScat;
    case kmtCCoth    : return ktApplyToCCoth;
    case kmtNCEL     : return ktApplyToNCEL;
    case kmtNCMEC    : return ktApplyToNCMEC    | ktApplyToNCEL;
    case kmtNC1piC   : return ktApplyToNC1piC   | ktApplyToNCoth; //why | ktApplyToNCoth ?
    case kmtNC1pi0   : return ktApplyToNC1pi0   | ktApplyToNCoth;
    case kmtNC2piC   : return ktApplyToNC2piC   | ktApplyToNCoth;
    case kmtNC2pi0   : return ktApplyToNC2pi0   | ktApplyToNCoth;
    case kmtNCpiCpi0 : return ktApplyToNCpiCpi0 | ktApplyToNCoth;
    case kmtNCcoh    : return ktApplyToNCcoh    | ktApplyToNCoth;
    case kmtNCELScat : return ktApplyToNCELScat;
    case kmtNCoth    : return ktApplyToNCoth;
    case kmtELScat   : return ktApplyToNuEELScat;
    case kmtOther    : return 0;
    default:
      LOG("ApplyMask",pWARN) << "No reac type apply mask set for " << util::AsString(t) << ". Applying to none.";
      return 0;
  }
}
//________________________________________________________________________________________
const NuclearTargetApplyMask_t valor::util::ApplyToNuclearTarget(conv::NuclearTarget_t nt) 
{
  switch (nt) {
    case kntInclusive : return kntApplyToInclusive;
    case kntOther     : return kntApplyToOther;
    case kntHydrogen  : return kntApplyToHydrogen;
    case kntCarbon    : return kntApplyToCarbon;
    case kntOxygen    : return kntApplyToOxygen;
    case kntArgon     : return kntApplyToArgon;
    case kntCalcium   : return kntApplyToCalcium;
    default:
      LOG("ApplyMask",pWARN) << "No nuclear target apply mask set for " << util::AsString(nt) << ". Applying to none.";
      return 0;
  }
}
//________________________________________________________________________________________
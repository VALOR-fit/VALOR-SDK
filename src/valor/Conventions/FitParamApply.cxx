//________________________________________________________________________________________
/*

Copyright (c) 2010-2018, The VALOR Fitting Group, All Rights Reserved.

*/
//________________________________________________________________________________________

#include <utility>

#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Conventions/FitParamApply.h"
#include "valor/Utilities/UtlSystem.h"

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
const ReacTypeApplyMask_t valor::conv::ktApplyToT2KLegCCoth = ktApplyToCCDIS    | ktApplyToCCMpi    |  ktApplyToCCmisc;

const ReacTypeApplyMask_t valor::conv::ktApplyToCCQELike    = ktApplyToCCQE     | ktApplyToCCMEC;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllCCELScat = ktApplyToCCELScat | ktApplyToNuEELScat; // nu + e CC elastic scattering (inverse muon/tau decay) and nue+e
const ReacTypeApplyMask_t valor::conv::ktApplyToAllCC1pi    = ktApplyToCC1piC   | ktApplyToCC1pi0;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllCC2pi    = ktApplyToCC2piC   | ktApplyToCC2pi0 | ktApplyToCCpiCpi0;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllCC       = ktApplyToCCQELike | ktApplyToAllCC1pi | ktApplyToAllCC2pi |
                                                              ktApplyToCCcoh    | ktApplyToCCDIS    | ktApplyToCCMpi    |
                                                              ktApplyToCCmisc   | ktApplyToCCoth    | ktApplyToCCELScat;
const ReacTypeApplyMask_t valor::conv::ktApplyToCCnQE       = ktApplyToAllCC & (~ktApplyToCCQELike);

const ReacTypeApplyMask_t valor::conv::ktApplyToNCElLike    = ktApplyToNCEL     | ktApplyToNCMEC; //this is not used, modes disabled
const ReacTypeApplyMask_t valor::conv::ktApplyToAllNCELScat = ktApplyToNCELScat | ktApplyToNuEELScat; //nu+e NC elastic scattering
const ReacTypeApplyMask_t valor::conv::ktApplyToAllNC1pi    = ktApplyToNC1piC   | ktApplyToNC1pi0;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllNC2pi    = ktApplyToNC2piC   | ktApplyToNC2pi0   | ktApplyToNCpiCpi0;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllNC       = ktApplyToNCElLike | ktApplyToAllNC1pi | ktApplyToAllNC2pi | ktApplyToNCcoh | ktApplyToNC1Gamma | ktApplyToNCoth | ktApplyToNCELScat;

const ReacTypeApplyMask_t valor::conv::ktApplyToAllCoh = ktApplyToCCcoh | ktApplyToNCcoh;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllELScat = ktApplyToCCELScat | ktApplyToNuEELScat | ktApplyToNCELScat;
const ReacTypeApplyMask_t valor::conv::ktApplyToAll1pi = ktApplyToAllCC1pi | ktApplyToAllNC1pi;

const ReacTypeApplyMask_t valor::conv::ktApplyToAll = ktApplyToAllCC | ktApplyToAllNC | ktApplyToAllELScat;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllButCoh    = ktApplyToAll & (~ktApplyToAllCoh);
const ReacTypeApplyMask_t valor::conv::ktApplyToAllButELScat = ktApplyToAll & (~ktApplyToAllELScat);
const ReacTypeApplyMask_t valor::conv::ktApplyToAllWithFSI   = ktApplyToAll & (~ktApplyToAllCoh) & (~ktApplyToAllELScat);
const ReacTypeApplyMask_t valor::conv::ktApplyToAllButCosmic = ktApplyToAll & (~ktApplyToCosmic);



// Detectors
const DetectorApplyMask_t valor::conv::kdApplyToAllDUNE = kdApplyToDUNEND | kdApplyToDUNEFD;
const DetectorApplyMask_t valor::conv::kdApplyToAllSBN  = kdApplyToSBND | kdApplyToMicroBooNE | kdApplyToICARUS;
const DetectorApplyMask_t valor::conv::kdApplyToAllSBN2Det  = kdApplyToSBND | kdApplyToMicroBooNE;
const DetectorApplyMask_t valor::conv::kdApplyToAllND  = kdApplyToDUNEND | kdApplyToSBND | kdApplyToMicroBooNE | kdApplyToICARUS | kdApplyToND280;
const DetectorApplyMask_t valor::conv::kdApplyToAllT2K  = kdApplyToND280 | kdApplyToSuperK;
const DetectorApplyMask_t valor::conv::kdApplyToAll    = kdApplyToAllDUNE | kdApplyToAllSBN | kdApplyToAllT2K;



// Samples
const SelecApplyMask_t valor::conv::ksApplyToAllND = (
  ksApplyToNuMuCC | ksApplyToNuMuCC0pi1trk| ksApplyToNuMuCC0pi2trk | ksApplyToNuMuCC0piNtrk |
  ksApplyToNuMuCC3trkDelta | ksApplyToNuMuCC1piC | ksApplyToNuMuCC2piC | ksApplyToNuMuCC1pi0 |
  ksApplyToNuMuCC1piC1pi0 | ksApplyToNuMuCCoth | ksApplyToNuMuCCWS0pi | ksApplyToNuMuCCWS1piC | ksApplyToNuMuCCWS1pi0 |
  ksApplyToNuMuCCWSoth | ksApplyToNuECC0pi | ksApplyToNuECC1piC | ksApplyToNuECC1pi0 | ksApplyToNuECCoth |
  ksApplyToNC0pi | ksApplyToNC1piC | ksApplyToNC1pi0 | ksApplyToNCoth | ksApplyToNeutrinoElec | ksApplyToInvMuDecay |
  ksApplyToNuECCWS | ksApplyToNuECC | ksApplyToNuMuCCWS | ksApplyToNC
);


const SelecApplyMask_t valor::conv::ksApplyToELikeRingAny = ksApplyTo1RingE | ksApplyToMultiRingE;                    // SuperK selections
const SelecApplyMask_t valor::conv::ksApplyTo1RingAny     = ksApplyTo1RingE | ksApplyTo1RingMu;                       // SuperK selections
const SelecApplyMask_t valor::conv::ksApplyToAllRings     = ksApplyTo1RingE | ksApplyTo1RingMu | ksApplyToMultiRingE; // SuperK selections

const SelecApplyMask_t valor::conv::ksApplyToLowRes = ksApplyToMuLikeLowRes | ksApplyToELikeLowRes;
const SelecApplyMask_t valor::conv::ksApplyToHighRes = ksApplyToMuLikeHighRes | ksApplyToELikeHighRes;
const SelecApplyMask_t valor::conv::ksApplyToAllFD = ksApplyToLowRes | ksApplyToHighRes | ksApplyToAllRings;
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
  fSelecMaskToName.insert( std::make_pair(ksApplyToMuLikeHighRes  , "mulikehighres") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToMuLikeLowRes   , "mulikelowres") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToELikeHighRes   , "elikehighres") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToELikeLowRes    , "elikelowres") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToAllND          , "allnd") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToAllFD          , "allfd") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToAll            , "all") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToAllNC          , "allnc") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToAllNuECC       , "allnuecc") );
  fSelecMaskToName.insert( std::make_pair(ksApplyTo1RingMu        , "1rmu") );
  fSelecMaskToName.insert( std::make_pair(ksApplyTo1RingE         , "1re") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToMultiRingE     , "multire") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToELikeRingAny   , "e-likerings") );
  fSelecMaskToName.insert( std::make_pair(ksApplyTo1RingAny       , "1rany") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToAllRings       , "allrings") );
  for(auto const& pair : fSelecMaskToName) {
    fSelecNameToMask.insert( std::make_pair(pair.second, pair.first) );
  }

  // Reaction type mask names
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCQE         , "ccqe") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCMEC        , "ccmec") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCC1piC       , "cc1pic") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCC1pi0       , "cc1pi0") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCC2piC       , "cc2pic") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCC2pi0       , "cc2pi0") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCpiCpi0     , "ccpicpi0") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCcoh        , "cccoh") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCIMD        , "ccimd") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCELScat     , "ccelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCDIS        , "ccdis") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCMpi        , "ccmpi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCmisc       , "ccmisc") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCoth        , "ccoth") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToT2KLegCCoth  , "t2klegccoth") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNCEL         , "ncel") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNCMEC        , "ncmec") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNC1piC       , "nc1pic") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNC1pi0       , "nc1pi0") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNC2piC       , "nc2pic") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNC2pi0       , "nc2pi0") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNCpiCpi0     , "ncpicpi0") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNCcoh        , "nccoh") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNC1Gamma     , "nc1gamma") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNCELScat     , "ncelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNCoth        , "ncoth") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNuEELScat    , "nueelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCosmic       , "cosmic") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCQELike     , "ccqelike") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCCELScat  , "allccelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCC1pi     , "allcc1pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCC2pi     , "allcc2pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCC        , "allcc") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCnQE        , "ccnqe") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNCElLike     , "ncellike") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllNCELScat  , "allncelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllNC1pi     , "allnc1pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllNC2pi     , "allnc2pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllNC        , "allnc") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCoh       , "allcoh") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllELScat    , "allelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAll1pi       , "all1pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAll          , "all") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllButCoh    , "allbutcoh") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllButELScat , "allbutelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllWithFSI   , "allwithfsi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllButCosmic , "allbutcosmic") );
  for(auto const& pair : fReacTypeMaskToName) {
    fReacTypeNameToMask.insert( std::make_pair(pair.second, pair.first) );
  }

  // Detector mask names
  fDetectorMaskToName.insert( std::make_pair(kdApplyToDUNEND    , "dunend") );
  fDetectorMaskToName.insert( std::make_pair(kdApplyToDUNEFD    , "dunefd") );
  fDetectorMaskToName.insert( std::make_pair(kdApplyToSBND      , "sbnd") );
  fDetectorMaskToName.insert( std::make_pair(kdApplyToMicroBooNE, "microboone") );
  fDetectorMaskToName.insert( std::make_pair(kdApplyToICARUS    , "icarus") );
  fDetectorMaskToName.insert( std::make_pair(kdApplyToND280     , "nd280") );
  fDetectorMaskToName.insert( std::make_pair(kdApplyToSuperK    , "superk") );
  fDetectorMaskToName.insert( std::make_pair(kdApplyToAllT2K    , "allt2k") );
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
std::string valor::util::AsString(const conv::ApplyMask_t & mask)
{
  return "\n("
    "det: "        + util::DetectorMaskAsString     (mask.detector)   + ", "
    "beam: "       + util::BeamMaskAsString         (mask.beam)       + ", "
    "init flav: "  + util::FlavMaskAsString         (mask.flav_init)  + ", "
    "final flav: " + util::FlavMaskAsString         (mask.flav_final) + ", "
    "selec: "      + util::SelecMaskAsString        (mask.selec)      + ", "
    "reac type: "  + util::ReacTypeMaskAsString     (mask.reactype)   + ", "
    "target: "     + util::NuclearTargetMaskAsString(mask.target)     + ")\n";
}
//________________________________________________________________________________________
std::string valor::util::ApplyMaskAsString(const conv::ApplyMask_t & mask)
{
  return util::AsString(mask);
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
    case ksNuMuLikeCChigh  : return ksApplyToMuLikeHighRes;
    case ksNuMuLikeCClow   : return ksApplyToMuLikeLowRes;
    case ksNuMuBarCChigh   : return ksApplyToMuLikeHighRes;
    case ksNuMuBarCClow    : return ksApplyToMuLikeLowRes;
    case ksNuELikeCChigh   : return ksApplyToELikeHighRes;
    case ksNuELikeCClow    : return ksApplyToELikeLowRes;
    case ksNuEBarCChigh    : return ksApplyToELikeHighRes;
    case ksNuEBarCClow     : return ksApplyToELikeLowRes;
    case ks1Rmu            : return ksApplyTo1RingMu;
    case ks1Re             : return ksApplyTo1RingE;
    case ksMultiRe         : return ksApplyToMultiRingE;
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
    case kdSuperK    : return kdApplyToSuperK;
    case kdND280     : return kdApplyToND280;
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
    case kmtCCDIS    : return ktApplyToCCDIS;
    case kmtCCMpi    : return ktApplyToCCMpi;
    case kmtCCmisc   : return ktApplyToCCmisc;
    case kmtCCoth    : return ktApplyToCCoth;
    case kmtNCEL     : return ktApplyToNCEL;
    case kmtNCMEC    : return ktApplyToNCMEC;
    case kmtNC1piC   : return ktApplyToNC1piC;
    case kmtNC1pi0   : return ktApplyToNC1pi0;
    case kmtNC2piC   : return ktApplyToNC2piC;
    case kmtNC2pi0   : return ktApplyToNC2pi0;
    case kmtNCpiCpi0 : return ktApplyToNCpiCpi0;
    case kmtNCcoh    : return ktApplyToNCcoh;
    case kmtNC1Gamma : return ktApplyToNC1Gamma;
    case kmtNCELScat : return ktApplyToNCELScat;
    case kmtNCoth    : return ktApplyToNCoth;
    case kmtELScat   : return ktApplyToNuEELScat;
    case kmtOther    : return 0;
    case kmtCosmic   : return ktApplyToCosmic;
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
conv::ApplyMask_t valor::util::ReadApplyMaskAll(TXMLEngine* xmleng, XMLNodePointer_t top_node)
{
  conv::DetectorApplyMask_t      detector   = conv::kdApplyToAll;
  conv::BeamApplyMask_t          beam       = conv::kbApplyToAll;
  conv::FlavApplyMask_t          flav_init  = conv::kfApplyToAll;
  conv::FlavApplyMask_t          flav_final = conv::kfApplyToAll;
  conv::SelecApplyMask_t         selec      = conv::ksApplyToAll;
  conv::ReacTypeApplyMask_t      reactype   = conv::ktApplyToAll;
  conv::NuclearTargetApplyMask_t target     = conv::kntApplyToAll;
  if(util::NodeExists(xmleng, top_node, "apply_to")) {
    detector   = util::ReadDetectorApplyMask     (xmleng, top_node, "detector");
    beam       = util::ReadBeamApplyMask         (xmleng, top_node, "beam");
    flav_init  = util::ReadFlavApplyMask         (xmleng, top_node, "flav_init");
    flav_final = util::ReadFlavApplyMask         (xmleng, top_node, "flav_final");
    selec      = util::ReadSelecApplyMask        (xmleng, top_node, "selection");
    reactype   = util::ReadReacTypeApplyMask     (xmleng, top_node, "reac_type");
    target     = util::ReadNuclearTargetApplyMask(xmleng, top_node, "target");
  }
  conv::ApplyMask_t apply_mask = {detector, beam, flav_init, flav_final, selec, reactype, target};
  return apply_mask;
}
//________________________________________________________________________________________
conv::DetectorApplyMask_t valor::util::ReadDetectorApplyMask(TXMLEngine* xmleng, XMLNodePointer_t top_node, string node_name)
{
  conv::DetectorApplyMask_t mask = conv::kdApplyToAll;
  string node_path = "apply_to/" + node_name;
  if(util::IsNodeSpecified(xmleng, top_node, node_path)) {
    string content = util::XML2String(xmleng, top_node, node_path);
    std::string trimmed_content = util::ToLowerCase(util::RemoveAllWhitespace(content));
    try {
      mask = util::DetectorMaskFromString(util::ToLowerCase(trimmed_content));
    }
    catch (const std::out_of_range& oor) {
      LOG("ApplyMask", pFATAL) << "When reading node '" << node_name << "' apply mask name \'" << trimmed_content
                               << "\' (from user input \'" << content
                               << "\') was not recognised - threw an out of range error: " << oor.what();
      util::Die(1);
    }
  }
  return mask;
}
//________________________________________________________________________________________
conv::BeamApplyMask_t valor::util::ReadBeamApplyMask(TXMLEngine* xmleng, XMLNodePointer_t top_node, string node_name)
{
  conv::BeamApplyMask_t mask = conv::kbApplyToAll;
  string node_path = "apply_to/" + node_name;
  if(util::IsNodeSpecified(xmleng, top_node, node_path)) {
    string content = util::XML2String(xmleng, top_node, node_path);
    std::string trimmed_content = util::ToLowerCase(util::RemoveAllWhitespace(content));
    try {
      mask = util::BeamMaskFromString(util::ToLowerCase(trimmed_content));
    }
    catch (const std::out_of_range& oor) {
      LOG("ApplyMask", pFATAL) << "When reading node '" << node_name << "' apply mask name \'" << trimmed_content
                               << "\' (from user input \'" << content
                               << "\') was not recognised - threw an out of range error: " << oor.what();
      util::Die(1);
    }
  }
  return mask;
}
//________________________________________________________________________________________
conv::FlavApplyMask_t valor::util::ReadFlavApplyMask(TXMLEngine* xmleng, XMLNodePointer_t top_node, string node_name)
{
  FlavApplyMask_t mask = conv::kfApplyToAll;
  string node_path = "apply_to/" + node_name;
  if(util::IsNodeSpecified(xmleng, top_node, node_path)) {
    string content = util::XML2String(xmleng, top_node, node_path);
    std::string trimmed_content = util::ToLowerCase(util::RemoveAllWhitespace(content));
    try {
      mask = util::FlavMaskFromString(trimmed_content);
    }
    catch (const std::out_of_range& oor) {
      LOG("ApplyMask", pFATAL) << "When reading node '" << node_name << "' apply mask name \'" << trimmed_content
                               << "\' (from user input \'" << content
                               << "\') was not recognised - threw an out of range error: " << oor.what();
      util::Die(1);
    }
  }
  return mask;
}
//________________________________________________________________________________________
conv::SelecApplyMask_t valor::util::ReadSelecApplyMask(TXMLEngine* xmleng, XMLNodePointer_t top_node, string node_name)
{
  SelecApplyMask_t mask = conv::ksApplyToAll;
  string node_path = "apply_to/" + node_name;
  if(util::IsNodeSpecified(xmleng, top_node, node_path)) {
    string content = util::XML2String(xmleng, top_node, node_path);
    std::string trimmed_content = util::ToLowerCase(util::RemoveAllWhitespace(content));
    try {
      mask = util::SelecMaskFromString(util::ToLowerCase(trimmed_content));
    }
    catch (const std::out_of_range& oor) {
      LOG("ApplyMask", pFATAL) << "When reading node '" << node_name << "' apply mask name \'" << trimmed_content
                               << "\' (from user input \'" << content
                               << "\') was not recognised - threw an out of range error: " << oor.what();
      util::Die(1);
    }
  }
  return mask;
}
//________________________________________________________________________________________
conv::ReacTypeApplyMask_t valor::util::ReadReacTypeApplyMask(TXMLEngine* xmleng, XMLNodePointer_t top_node, string node_name)
{
  ReacTypeApplyMask_t mask = conv::ktApplyToAll;
  string node_path = "apply_to/" + node_name;
  if(util::IsNodeSpecified(xmleng, top_node, node_path)) {
    string content = util::XML2String(xmleng, top_node, node_path);
    std::string trimmed_content = util::ToLowerCase(util::RemoveAllWhitespace(content));
    std::string reac_type_str = util::ReplaceAll(trimmed_content, "2p2h", "mec");  // 2p2h and MEC are the same thing, but we internally use MEC only
    try {
      mask = util::ReacTypeMaskFromString(util::ToLowerCase(reac_type_str));
    }
    catch (const std::out_of_range& oor) {
      LOG("ApplyMask", pFATAL) << "When reading node '" << node_name << "' apply mask name \'" << trimmed_content
                               << "\' (from user input \'" << content
                               << "\') was not recognised - threw an out of range error: " << oor.what();
      util::Die(1);
    }
  }
  return mask;
}
//________________________________________________________________________________________
conv::NuclearTargetApplyMask_t valor::util::ReadNuclearTargetApplyMask(TXMLEngine* xmleng, XMLNodePointer_t top_node, string node_name)
{
  NuclearTargetApplyMask_t mask = conv::kntApplyToAll;
  string node_path = "apply_to/" + node_name;
  if(util::IsNodeSpecified(xmleng, top_node, node_path)) {
    string content = util::XML2String(xmleng, top_node, node_path);
    std::string trimmed_content = util::ToLowerCase(util::RemoveAllWhitespace(content));
    try {
      mask = util::NuclearTargetMaskFromString(util::ToLowerCase(trimmed_content));
    }
    catch (const std::out_of_range& oor) {
      LOG("ApplyMask", pFATAL) << "When reading node '" << node_name << "' apply mask name \'" << trimmed_content
                               << "\' (from user input \'" << content
                               << "\') was not recognised - threw an out of range error: " << oor.what();
      util::Die(1);
    }
  }
  return mask;
}
//________________________________________________________________________________________
bool valor::util::IsAppliedTo(conv::ApplyMask_t mask_to_check, conv::BeamConf_t b, conv::Sample_t s, conv::Detector_t d, conv::ReacMode_t m, conv::NuclearTarget_t nt)
{
  if(b  == conv::kbUndefined ) { return false; }
  if(s  == conv::ksUndefined ) { return false; }
  if(d  == conv::kdUndefined ) { return false; }
  if(m  == conv::kmUndefined ) { return false; }
  if(nt == conv::kntUndefined) { return false; }

  NuFlavour_t f_init  = kReacModeToInitFlavour [m];
  NuFlavour_t f_final = kReacModeToFinalFlavour[m];
  conv::ReacType_t  t = conv::kReacModeToType[m];

  if(f_init  == kNuFlavourUndefined) { return false; }
  if(f_final == kNuFlavourUndefined) { return false; }
  if(t == kmtUndefined)              { return false; }

  // Check whether or not to apply the param to this
  bool apply_to_beam          = (util::ApplyToBeamMode     (b)       & mask_to_check.beam      ).any();
  bool apply_to_sample        = (util::ApplyToSample       (s)       & mask_to_check.selec     ).any();
  bool apply_to_flavour_init  = (util::ApplyToFlavour      (f_init)  & mask_to_check.flav_init ).any();
  bool apply_to_flavour_final = (util::ApplyToFlavour      (f_final) & mask_to_check.flav_final).any();
  bool apply_to_type          = (util::ApplyToReacType     (t)       & mask_to_check.reactype  ).any();
  bool apply_to_detector      = (util::ApplyToDetector     (d)       & mask_to_check.detector  ).any();
  bool apply_to_target        = (util::ApplyToNuclearTarget(nt)      & mask_to_check.target    ).any();

  bool apply = (apply_to_beam && apply_to_sample && apply_to_flavour_init && apply_to_flavour_final && apply_to_type &&
                apply_to_detector && apply_to_target);

  return apply;
}
//________________________________________________________________________________________
bool valor::util::IsAppliedTo(conv::ApplyMask_t mask_to_check, conv::ApplyMask_t mask)
{
  bool apply_to_beam          = (mask_to_check.beam       & mask.beam      ).any();
  bool apply_to_sample        = (mask_to_check.selec      & mask.selec     ).any();
  bool apply_to_flavour_init  = (mask_to_check.flav_init  & mask.flav_init ).any();
  bool apply_to_flavour_final = (mask_to_check.flav_final & mask.flav_final).any();
  bool apply_to_type          = (mask_to_check.reactype   & mask.reactype  ).any();
  bool apply_to_detector      = (mask_to_check.detector   & mask.detector  ).any();
  bool apply_to_target        = (mask_to_check.target     & mask.target    ).any();

  bool apply = (apply_to_beam && apply_to_sample && apply_to_flavour_init && apply_to_flavour_final &&
                apply_to_type && apply_to_detector && apply_to_target);

  return apply;
}
//________________________________________________________________________________________

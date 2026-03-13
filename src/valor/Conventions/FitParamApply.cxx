//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

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
const FlavApplyMask_t valor::conv::kfApplyToNuAny     = kfApplyToNuMu    | kfApplyToNuE   ; // NuAny means any non-barred
const FlavApplyMask_t valor::conv::kfApplyToNuBarAny  = kfApplyToNuMuBar | kfApplyToNuEBar;
const FlavApplyMask_t valor::conv::kfApplyToNuEAny    = kfApplyToNuE     | kfApplyToNuEBar;
const FlavApplyMask_t valor::conv::kfApplyToNuMuAny   = kfApplyToNuMu    | kfApplyToNuMuBar;
const FlavApplyMask_t valor::conv::kfApplyToAll       = kfApplyToNuAny   | kfApplyToNuBarAny;
const FlavApplyMask_t valor::conv::kfApplyToAllCosmic = kfApplyToAll     | kfApplyToCosmic;
const FlavApplyMask_t valor::conv::kfApplyToAllDirt   = kfApplyToAllCosmic     | kfApplyToDirt;

// Reaction type
const ReacTypeApplyMask_t valor::conv::ktApplyToT2KLegCCoth = ktApplyToCCDIS    | ktApplyToCCMpi    |  ktApplyToCCmisc;
const ReacTypeApplyMask_t valor::conv::ktApplyToCCDISMpi    = ktApplyToCCDIS    | ktApplyToCCMpi;

const ReacTypeApplyMask_t valor::conv::ktApplyToCCQELike    = ktApplyToCCQE     | ktApplyToCCMEC;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllCCELScat = ktApplyToCCELScat | ktApplyToNuEELScat; // nu + e CC elastic scattering (inverse muon/tau decay) and nue+e
const ReacTypeApplyMask_t valor::conv::ktApplyToAllCC1pi    = ktApplyToCC1piC   | ktApplyToCC1pi0;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllCC2pi    = ktApplyToCC2piC   | ktApplyToCC2pi0 | ktApplyToCCpiCpi0;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllCCn0pi   = ktApplyToAllCC1pi | ktApplyToAllCC2pi | ktApplyToCCoth;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllCCNonRes = ktApplyToAllCC1pi | ktApplyToAllCC2pi;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllCC       = ktApplyToCCQELike | ktApplyToAllCC1pi | ktApplyToAllCC2pi |
                                                              ktApplyToCCcoh    | ktApplyToCCDIS    | ktApplyToCCMpi    |
                                                              ktApplyToCCmisc   | ktApplyToCCoth    | ktApplyToCCELScat;
const ReacTypeApplyMask_t valor::conv::ktApplyToCCnQE       = ktApplyToAllCC & (~ktApplyToCCQELike);

const ReacTypeApplyMask_t valor::conv::ktApplyToNCElLike    = ktApplyToNCEL     | ktApplyToNCMEC; //this is not used, modes disabled
const ReacTypeApplyMask_t valor::conv::ktApplyToAllNCELScat = ktApplyToNCELScat | ktApplyToNuEELScat; //nu+e NC elastic scattering
const ReacTypeApplyMask_t valor::conv::ktApplyToAllNC1pi    = ktApplyToNC1piC   | ktApplyToNC1pi0;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllNC2pi    = ktApplyToNC2piC   | ktApplyToNC2pi0   | ktApplyToNCpiCpi0;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllNCn0pi   = ktApplyToAllNC1pi | ktApplyToAllNC2pi | ktApplyToNCoth;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllNCNonRes = ktApplyToAllNC1pi | ktApplyToAllNC2pi;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllNC       = ktApplyToNCElLike | ktApplyToAllNC1pi | ktApplyToAllNC2pi | ktApplyToNCcoh | ktApplyToNC1Gamma | ktApplyToNCoth | ktApplyToNCELScat;

const ReacTypeApplyMask_t valor::conv::ktApplyToAllCoh       = ktApplyToCCcoh | ktApplyToNCcoh;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllQELike    = ktApplyToCCQELike | ktApplyToNCElLike;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllMEC       = ktApplyToCCMEC | ktApplyToNCMEC;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllELScat    = ktApplyToCCELScat | ktApplyToNuEELScat | ktApplyToNCELScat;
const ReacTypeApplyMask_t valor::conv::ktApplyToAll1pi       = ktApplyToAllCC1pi | ktApplyToAllNC1pi;
const ReacTypeApplyMask_t valor::conv::ktApplyToAll2pi       = ktApplyToAllCC2pi | ktApplyToAllNC2pi;
const ReacTypeApplyMask_t valor::conv::ktApplyToAlln0pi      = ktApplyToAllCCn0pi | ktApplyToAllNCn0pi;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllNonRes    = ktApplyToAllCCNonRes | ktApplyToAllNCNonRes;

const ReacTypeApplyMask_t valor::conv::ktApplyToAll          = ktApplyToAllCC | ktApplyToAllNC | ktApplyToAllELScat;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllPlusCosmic= ktApplyToAllCC | ktApplyToAllNC | ktApplyToAllELScat | ktApplyToCosmic;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllPlusDirt  = ktApplyToAllCC | ktApplyToAllNC | ktApplyToAllELScat | ktApplyToCosmic | ktApplyToDirt;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllPi        = ktApplyToAll1pi | ktApplyToAll2pi | ktApplyToAlln0pi;
const ReacTypeApplyMask_t valor::conv::ktApplyToAllButCoh    = ktApplyToAll   & (~ktApplyToAllCoh);
const ReacTypeApplyMask_t valor::conv::ktApplyToAllButELScat = ktApplyToAll   & (~ktApplyToAllELScat);
const ReacTypeApplyMask_t valor::conv::ktApplyToAllWithFSI   = ktApplyToAll   & (~ktApplyToAllCoh) & (~ktApplyToAllELScat) & (~ktApplyToNC1Gamma);
const ReacTypeApplyMask_t valor::conv::ktApplyToAllWithPiFSI = ktApplyToAll   & (~ktApplyToAllQELike) & (~ktApplyToAllCoh) & (~ktApplyToAllELScat) & (~ktApplyToNC1Gamma);
const ReacTypeApplyMask_t valor::conv::ktApplyToAllButCosmic = ktApplyToAll   & (~ktApplyToCosmic);
//const ReacTypeApplyMask_t valor::conv::ktApplyToAllButNuECC = (ktApplyToAll)    & (~ksApplyToNuECC0pi) & (~ksApplyToNuECC1piC) & (~ksApplyToNuECC1pi0) & (~ksApplyToNuECCoth);
const ReacTypeApplyMask_t valor::conv::ktApplyToPiAndOth  = ktApplyToCCoth | ktApplyToNCoth | ktApplyToAll1pi | ktApplyToAllCC2pi | ktApplyToNC2piC | ktApplyToNC2pi0;

// Detectors
const DetectorApplyMask_t valor::conv::kdApplyToAllDUNE = kdApplyToDUNEND | kdApplyToDUNEFD;
const DetectorApplyMask_t valor::conv::kdApplyToAllSBN  = kdApplyToSBND | kdApplyToMicroBooNE | kdApplyToICARUS;
const DetectorApplyMask_t valor::conv::kdApplyToAllSBN2Det  = kdApplyToSBND | kdApplyToMicroBooNE;
const DetectorApplyMask_t valor::conv::kdApplyToAllND  = kdApplyToDUNEND | kdApplyToSBND | kdApplyToMicroBooNE | kdApplyToICARUS | kdApplyToND280;
const DetectorApplyMask_t valor::conv::kdApplyToAllT2K  = kdApplyToND280 | kdApplyToSuperK;
const DetectorApplyMask_t valor::conv::kdApplyToAll    = kdApplyToAllDUNE | kdApplyToAllSBN | kdApplyToAllT2K;



// Samples
const SelecApplyMask_t valor::conv::ksApplyToAllND = 
  ( ksApplyToNearNuMuCC     | 
    ksApplyToNearNuMuCCOAA1 | ksApplyToNearNuMuCCOAA2 | ksApplyToNearNuMuCCOAA3 | 
    ksApplyToNearNuMuCCOAA4 | ksApplyToNearNuMuCCOAA5 | ksApplyToNearNuMuCCOAA6 | 
    ksApplyToNearNuMuCCOAA7 | ksApplyToNearNuMuCCOAA8 | ksApplyToNearNuMuCCOAA9 |
    ksApplyToNearNuMuCC_0p0pi     | 
    ksApplyToNearNuMuCC_0p0piOAA1 | ksApplyToNearNuMuCC_0p0piOAA2 | ksApplyToNearNuMuCC_0p0piOAA3 | 
    ksApplyToNearNuMuCC_0p0piOAA4 | ksApplyToNearNuMuCC_0p0piOAA5 | ksApplyToNearNuMuCC_0p0piOAA6 | 
    ksApplyToNearNuMuCC_0p0piOAA7 | ksApplyToNearNuMuCC_0p0piOAA8 | ksApplyToNearNuMuCC_0p0piOAA9 |
    ksApplyToNearNuMuCC_1p0pi     | 
    ksApplyToNearNuMuCC_1p0piOAA1 | ksApplyToNearNuMuCC_1p0piOAA2 | ksApplyToNearNuMuCC_1p0piOAA3 | 
    ksApplyToNearNuMuCC_1p0piOAA4 | ksApplyToNearNuMuCC_1p0piOAA5 | ksApplyToNearNuMuCC_1p0piOAA6 | 
    ksApplyToNearNuMuCC_1p0piOAA7 | ksApplyToNearNuMuCC_1p0piOAA8 | ksApplyToNearNuMuCC_1p0piOAA9 |
    ksApplyToNearNuMuCC_Np0pi     | 
    ksApplyToNearNuMuCC_Np0piOAA1 | ksApplyToNearNuMuCC_Np0piOAA2 | ksApplyToNearNuMuCC_Np0piOAA3 | 
    ksApplyToNearNuMuCC_Np0piOAA4 | ksApplyToNearNuMuCC_Np0piOAA5 | ksApplyToNearNuMuCC_Np0piOAA6 | 
    ksApplyToNearNuMuCC_Np0piOAA7 | ksApplyToNearNuMuCC_Np0piOAA8 | ksApplyToNearNuMuCC_Np0piOAA9 |
    ksApplyToNearNuMuCC_1pi     | 
    ksApplyToNearNuMuCC_1piOAA1 | ksApplyToNearNuMuCC_1piOAA2 | ksApplyToNearNuMuCC_1piOAA3 | 
    ksApplyToNearNuMuCC_1piOAA4 | ksApplyToNearNuMuCC_1piOAA5 | ksApplyToNearNuMuCC_1piOAA6 | 
    ksApplyToNearNuMuCC_1piOAA7 | ksApplyToNearNuMuCC_1piOAA8 | ksApplyToNearNuMuCC_1piOAA9 |
    ksApplyToNearNuMuCC_Other     | 
    ksApplyToNearNuMuCC_OtherOAA1 | ksApplyToNearNuMuCC_OtherOAA2 | ksApplyToNearNuMuCC_OtherOAA3 | 
    ksApplyToNearNuMuCC_OtherOAA4 | ksApplyToNearNuMuCC_OtherOAA5 | ksApplyToNearNuMuCC_OtherOAA6 | 
    ksApplyToNearNuMuCC_OtherOAA7 | ksApplyToNearNuMuCC_OtherOAA8 | ksApplyToNearNuMuCC_OtherOAA9
    );

const SelecApplyMask_t valor::conv::ksApplyToAllNuMuCC = 
  ( ksApplyToNearNuMuCC     | 
    ksApplyToNearNuMuCCOAA1 | ksApplyToNearNuMuCCOAA2 | ksApplyToNearNuMuCCOAA3 | 
    ksApplyToNearNuMuCCOAA4 | ksApplyToNearNuMuCCOAA5 | ksApplyToNearNuMuCCOAA6 | 
    ksApplyToNearNuMuCCOAA7 | ksApplyToNearNuMuCCOAA8 | ksApplyToNearNuMuCCOAA9 |
    ksApplyToNearNuMuCC_0p0pi     | 
    ksApplyToNearNuMuCC_0p0piOAA1 | ksApplyToNearNuMuCC_0p0piOAA2 | ksApplyToNearNuMuCC_0p0piOAA3 | 
    ksApplyToNearNuMuCC_0p0piOAA4 | ksApplyToNearNuMuCC_0p0piOAA5 | ksApplyToNearNuMuCC_0p0piOAA6 | 
    ksApplyToNearNuMuCC_0p0piOAA7 | ksApplyToNearNuMuCC_0p0piOAA8 | ksApplyToNearNuMuCC_0p0piOAA9 |
    ksApplyToNearNuMuCC_1p0pi     | 
    ksApplyToNearNuMuCC_1p0piOAA1 | ksApplyToNearNuMuCC_1p0piOAA2 | ksApplyToNearNuMuCC_1p0piOAA3 | 
    ksApplyToNearNuMuCC_1p0piOAA4 | ksApplyToNearNuMuCC_1p0piOAA5 | ksApplyToNearNuMuCC_1p0piOAA6 | 
    ksApplyToNearNuMuCC_1p0piOAA7 | ksApplyToNearNuMuCC_1p0piOAA8 | ksApplyToNearNuMuCC_1p0piOAA9 |
    ksApplyToNearNuMuCC_Np0pi     | 
    ksApplyToNearNuMuCC_Np0piOAA1 | ksApplyToNearNuMuCC_Np0piOAA2 | ksApplyToNearNuMuCC_Np0piOAA3 | 
    ksApplyToNearNuMuCC_Np0piOAA4 | ksApplyToNearNuMuCC_Np0piOAA5 | ksApplyToNearNuMuCC_Np0piOAA6 | 
    ksApplyToNearNuMuCC_Np0piOAA7 | ksApplyToNearNuMuCC_Np0piOAA8 | ksApplyToNearNuMuCC_Np0piOAA9 |
    ksApplyToNearNuMuCC_1pi     | 
    ksApplyToNearNuMuCC_1piOAA1 | ksApplyToNearNuMuCC_1piOAA2 | ksApplyToNearNuMuCC_1piOAA3 | 
    ksApplyToNearNuMuCC_1piOAA4 | ksApplyToNearNuMuCC_1piOAA5 | ksApplyToNearNuMuCC_1piOAA6 | 
    ksApplyToNearNuMuCC_1piOAA7 | ksApplyToNearNuMuCC_1piOAA8 | ksApplyToNearNuMuCC_1piOAA9 |
    ksApplyToNearNuMuCC_Other     | 
    ksApplyToNearNuMuCC_OtherOAA1 | ksApplyToNearNuMuCC_OtherOAA2 | ksApplyToNearNuMuCC_OtherOAA3 | 
    ksApplyToNearNuMuCC_OtherOAA4 | ksApplyToNearNuMuCC_OtherOAA5 | ksApplyToNearNuMuCC_OtherOAA6 | 
    ksApplyToNearNuMuCC_OtherOAA7 | ksApplyToNearNuMuCC_OtherOAA8 | ksApplyToNearNuMuCC_OtherOAA9 |
    ksApplyToFarNuMuCC     |
    ksApplyToFarNuMuCC_0pi | ksApplyToFarNuMuCC_1pi | ksApplyToFarNuMuCC_Npi
    );


const SelecApplyMask_t valor::conv::ksApplyToAllFD = 
  ( ksApplyToFarNuMuCC     |
    ksApplyToFarNuMuCC_0pi | ksApplyToFarNuMuCC_1pi | ksApplyToFarNuMuCC_Npi
    );

const SelecApplyMask_t valor::conv::ksApplyToAll = ksApplyToAllND | ksApplyToAllFD;

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
  fFlavMaskToName.insert( std::make_pair(kfApplyToNuMu      , "numu") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToNuMuBar   , "numubar") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToNuE       , "nue") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToNuEBar    , "nuebar") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToNuAny     , "nuany") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToNuBarAny  , "nubarany") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToNuEAny    , "nueany") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToNuMuAny   , "numuany") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToAll       , "all") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToAllCosmic , "allcosmic") );
  fFlavMaskToName.insert( std::make_pair(kfApplyToAllDirt   , "alldirt") );
  for(auto const& pair : fFlavMaskToName) {
    fFlavNameToMask.insert( std::make_pair(pair.second, pair.first) );
  }

  // Selection mask names
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC    , "numucc_nd") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCCOAA1, "numucc_nd_oaa1") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCCOAA2, "numucc_nd_oaa2") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCCOAA3, "numucc_nd_oaa3") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCCOAA4, "numucc_nd_oaa4") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCCOAA5, "numucc_nd_oaa5") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCCOAA6, "numucc_nd_oaa6") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCCOAA7, "numucc_nd_oaa7") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCCOAA8, "numucc_nd_oaa8") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCCOAA9, "numucc_nd_oaa9") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_0p0pi    , "numucc_0p0pi_nd") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_0p0piOAA1, "numucc_0p0pi_nd_oaa1") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_0p0piOAA2, "numucc_0p0pi_nd_oaa2") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_0p0piOAA3, "numucc_0p0pi_nd_oaa3") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_0p0piOAA4, "numucc_0p0pi_nd_oaa4") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_0p0piOAA5, "numucc_0p0pi_nd_oaa5") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_0p0piOAA6, "numucc_0p0pi_nd_oaa6") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_0p0piOAA7, "numucc_0p0pi_nd_oaa7") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_0p0piOAA8, "numucc_0p0pi_nd_oaa8") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_0p0piOAA9, "numucc_0p0pi_nd_oaa9") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1p0pi    , "numucc_1p0pi_nd") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1p0piOAA1, "numucc_1p0pi_nd_oaa1") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1p0piOAA2, "numucc_1p0pi_nd_oaa2") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1p0piOAA3, "numucc_1p0pi_nd_oaa3") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1p0piOAA4, "numucc_1p0pi_nd_oaa4") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1p0piOAA5, "numucc_1p0pi_nd_oaa5") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1p0piOAA6, "numucc_1p0pi_nd_oaa6") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1p0piOAA7, "numucc_1p0pi_nd_oaa7") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1p0piOAA8, "numucc_1p0pi_nd_oaa8") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1p0piOAA9, "numucc_1p0pi_nd_oaa9") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_Np0pi    , "numucc_Np0pi_nd") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_Np0piOAA1, "numucc_Np0pi_nd_oaa1") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_Np0piOAA2, "numucc_Np0pi_nd_oaa2") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_Np0piOAA3, "numucc_Np0pi_nd_oaa3") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_Np0piOAA4, "numucc_Np0pi_nd_oaa4") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_Np0piOAA5, "numucc_Np0pi_nd_oaa5") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_Np0piOAA6, "numucc_Np0pi_nd_oaa6") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_Np0piOAA7, "numucc_Np0pi_nd_oaa7") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_Np0piOAA8, "numucc_Np0pi_nd_oaa8") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_Np0piOAA9, "numucc_Np0pi_nd_oaa9") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1pi    , "numucc_1pi_nd") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1piOAA1, "numucc_1pi_nd_oaa1") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1piOAA2, "numucc_1pi_nd_oaa2") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1piOAA3, "numucc_1pi_nd_oaa3") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1piOAA4, "numucc_1pi_nd_oaa4") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1piOAA5, "numucc_1pi_nd_oaa5") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1piOAA6, "numucc_1pi_nd_oaa6") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1piOAA7, "numucc_1pi_nd_oaa7") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1piOAA8, "numucc_1pi_nd_oaa8") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_1piOAA9, "numucc_1pi_nd_oaa9") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_Other    , "numucc_other_nd") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_OtherOAA1, "numucc_other_nd_oaa1") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_OtherOAA2, "numucc_other_nd_oaa2") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_OtherOAA3, "numucc_other_nd_oaa3") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_OtherOAA4, "numucc_other_nd_oaa4") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_OtherOAA5, "numucc_other_nd_oaa5") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_OtherOAA6, "numucc_other_nd_oaa6") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_OtherOAA7, "numucc_other_nd_oaa7") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_OtherOAA8, "numucc_other_nd_oaa8") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToNearNuMuCC_OtherOAA9, "numucc_other_nd_oaa9") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToFarNuMuCC, "numucc_fd") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToFarNuMuCC_0pi, "numucc_0pi_fd") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToFarNuMuCC_1pi, "numucc_1pi_fd") );
  fSelecMaskToName.insert( std::make_pair(ksApplyToFarNuMuCC_Npi, "numucc_Npi_fd") );
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
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCDISMpi     , "ccdismpi") );
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
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToDirt         , "dirt") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCQELike     , "ccqelike") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCCELScat  , "allccelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCC1pi     , "allcc1pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCC2pi     , "allcc2pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCCn0pi    , "allccn0pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCCNonRes  , "allccnonres") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCC        , "allcc") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToCCnQE        , "ccnqe") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToNCElLike     , "ncellike") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllNCELScat  , "allncelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllNC1pi     , "allnc1pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllNC2pi     , "allnc2pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllNCn0pi    , "allncn0pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllNCNonRes  , "allncnonres") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllNC        , "allnc") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllCoh       , "allcoh") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllQELike    , "allqelike") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllMEC       , "allmec") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllELScat    , "allelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAll1pi       , "all1pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAll2pi       , "all2pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAlln0pi      , "alln0pi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllNonRes    , "allnonres") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAll          , "all") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllPlusCosmic, "allpluscosmic") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllPlusDirt  , "allplusdirt") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllPi        , "allpi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllWithPiFSI , "allwithpifsi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllButCoh    , "allbutcoh") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllButELScat , "allbutelscat") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllWithFSI   , "allwithfsi") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToAllButCosmic , "allbutcosmic") );
  fReacTypeMaskToName.insert( std::make_pair(ktApplyToPiAndOth     , "piandoth") );
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
      VLOG("ApplyMask",pWARN) << "No beam apply mask set for " << util::AsString(b) << ". Applying to none.";
      return 0;
  }
}
//________________________________________________________________________________________
const SelecApplyMask_t valor::util::ApplyToSample(conv::Sample_t s)
{
  switch (s) {
  case ksNearNuMuCC     : return ksApplyToNearNuMuCC;
  case ksNearNuMuCCOAA1 : return ksApplyToNearNuMuCCOAA1;
  case ksNearNuMuCCOAA2 : return ksApplyToNearNuMuCCOAA2;
  case ksNearNuMuCCOAA3 : return ksApplyToNearNuMuCCOAA3;
  case ksNearNuMuCCOAA4 : return ksApplyToNearNuMuCCOAA4;
  case ksNearNuMuCCOAA5 : return ksApplyToNearNuMuCCOAA5;
  case ksNearNuMuCCOAA6 : return ksApplyToNearNuMuCCOAA6;
  case ksNearNuMuCCOAA7 : return ksApplyToNearNuMuCCOAA7;
  case ksNearNuMuCCOAA8 : return ksApplyToNearNuMuCCOAA8;
  case ksNearNuMuCCOAA9 : return ksApplyToNearNuMuCCOAA9;
  case ksNearNuMuCC_0p0pi     : return ksApplyToNearNuMuCC_0p0pi;
  case ksNearNuMuCC_0p0piOAA1 : return ksApplyToNearNuMuCC_0p0piOAA1;
  case ksNearNuMuCC_0p0piOAA2 : return ksApplyToNearNuMuCC_0p0piOAA2;
  case ksNearNuMuCC_0p0piOAA3 : return ksApplyToNearNuMuCC_0p0piOAA3;
  case ksNearNuMuCC_0p0piOAA4 : return ksApplyToNearNuMuCC_0p0piOAA4;
  case ksNearNuMuCC_0p0piOAA5 : return ksApplyToNearNuMuCC_0p0piOAA5;
  case ksNearNuMuCC_0p0piOAA6 : return ksApplyToNearNuMuCC_0p0piOAA6;
  case ksNearNuMuCC_0p0piOAA7 : return ksApplyToNearNuMuCC_0p0piOAA7;
  case ksNearNuMuCC_0p0piOAA8 : return ksApplyToNearNuMuCC_0p0piOAA8;
  case ksNearNuMuCC_0p0piOAA9 : return ksApplyToNearNuMuCC_0p0piOAA9;
  case ksNearNuMuCC_1p0pi     : return ksApplyToNearNuMuCC_1p0pi;
  case ksNearNuMuCC_1p0piOAA1 : return ksApplyToNearNuMuCC_1p0piOAA1;
  case ksNearNuMuCC_1p0piOAA2 : return ksApplyToNearNuMuCC_1p0piOAA2;
  case ksNearNuMuCC_1p0piOAA3 : return ksApplyToNearNuMuCC_1p0piOAA3;
  case ksNearNuMuCC_1p0piOAA4 : return ksApplyToNearNuMuCC_1p0piOAA4;
  case ksNearNuMuCC_1p0piOAA5 : return ksApplyToNearNuMuCC_1p0piOAA5;
  case ksNearNuMuCC_1p0piOAA6 : return ksApplyToNearNuMuCC_1p0piOAA6;
  case ksNearNuMuCC_1p0piOAA7 : return ksApplyToNearNuMuCC_1p0piOAA7;
  case ksNearNuMuCC_1p0piOAA8 : return ksApplyToNearNuMuCC_1p0piOAA8;
  case ksNearNuMuCC_1p0piOAA9 : return ksApplyToNearNuMuCC_1p0piOAA9;
  case ksNearNuMuCC_Np0pi     : return ksApplyToNearNuMuCC_Np0pi;
  case ksNearNuMuCC_Np0piOAA1 : return ksApplyToNearNuMuCC_Np0piOAA1;
  case ksNearNuMuCC_Np0piOAA2 : return ksApplyToNearNuMuCC_Np0piOAA2;
  case ksNearNuMuCC_Np0piOAA3 : return ksApplyToNearNuMuCC_Np0piOAA3;
  case ksNearNuMuCC_Np0piOAA4 : return ksApplyToNearNuMuCC_Np0piOAA4;
  case ksNearNuMuCC_Np0piOAA5 : return ksApplyToNearNuMuCC_Np0piOAA5;
  case ksNearNuMuCC_Np0piOAA6 : return ksApplyToNearNuMuCC_Np0piOAA6;
  case ksNearNuMuCC_Np0piOAA7 : return ksApplyToNearNuMuCC_Np0piOAA7;
  case ksNearNuMuCC_Np0piOAA8 : return ksApplyToNearNuMuCC_Np0piOAA8;
  case ksNearNuMuCC_Np0piOAA9 : return ksApplyToNearNuMuCC_Np0piOAA9;
  case ksNearNuMuCC_1pi     : return ksApplyToNearNuMuCC_1pi;
  case ksNearNuMuCC_1piOAA1 : return ksApplyToNearNuMuCC_1piOAA1;
  case ksNearNuMuCC_1piOAA2 : return ksApplyToNearNuMuCC_1piOAA2;
  case ksNearNuMuCC_1piOAA3 : return ksApplyToNearNuMuCC_1piOAA3;
  case ksNearNuMuCC_1piOAA4 : return ksApplyToNearNuMuCC_1piOAA4;
  case ksNearNuMuCC_1piOAA5 : return ksApplyToNearNuMuCC_1piOAA5;
  case ksNearNuMuCC_1piOAA6 : return ksApplyToNearNuMuCC_1piOAA6;
  case ksNearNuMuCC_1piOAA7 : return ksApplyToNearNuMuCC_1piOAA7;
  case ksNearNuMuCC_1piOAA8 : return ksApplyToNearNuMuCC_1piOAA8;
  case ksNearNuMuCC_1piOAA9 : return ksApplyToNearNuMuCC_1piOAA9;
  case ksNearNuMuCC_Other     : return ksApplyToNearNuMuCC_Other;
  case ksNearNuMuCC_OtherOAA1 : return ksApplyToNearNuMuCC_OtherOAA1;
  case ksNearNuMuCC_OtherOAA2 : return ksApplyToNearNuMuCC_OtherOAA2;
  case ksNearNuMuCC_OtherOAA3 : return ksApplyToNearNuMuCC_OtherOAA3;
  case ksNearNuMuCC_OtherOAA4 : return ksApplyToNearNuMuCC_OtherOAA4;
  case ksNearNuMuCC_OtherOAA5 : return ksApplyToNearNuMuCC_OtherOAA5;
  case ksNearNuMuCC_OtherOAA6 : return ksApplyToNearNuMuCC_OtherOAA6;
  case ksNearNuMuCC_OtherOAA7 : return ksApplyToNearNuMuCC_OtherOAA7;
  case ksNearNuMuCC_OtherOAA8 : return ksApplyToNearNuMuCC_OtherOAA8;
  case ksNearNuMuCC_OtherOAA9 : return ksApplyToNearNuMuCC_OtherOAA9;
  default:
    VLOG("ApplyMask",pWARN) << "No selection apply mask set for " << util::AsString(s) << ". Applying to none.";
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
      VLOG("ApplyMask",pWARN) << "No detector apply mask set for " << util::AsString(d) << ". Applying to none.";
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
    case kCosmic   : return kfApplyToCosmic;
    case kDirt     : return kfApplyToDirt;
    default:
      VLOG("ApplyMask",pWARN) << "No flavour apply mask set for " << util::AsString(f) << ". Applying to none.";
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
    case kmtDirt     : return ktApplyToDirt;
    default:
      VLOG("ApplyMask",pWARN) << "No reac type apply mask set for " << util::AsString(t) << ". Applying to none.";
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
      VLOG("ApplyMask",pWARN) << "No nuclear target apply mask set for " << util::AsString(nt) << ". Applying to none.";
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
      VLOG("ApplyMask", pFATAL) << "When reading node '" << node_name << "' apply mask name \'" << trimmed_content
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
      VLOG("ApplyMask", pFATAL) << "When reading node '" << node_name << "' apply mask name \'" << trimmed_content
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
      VLOG("ApplyMask", pFATAL) << "When reading node '" << node_name << "' apply mask name \'" << trimmed_content
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
      VLOG("ApplyMask", pFATAL) << "When reading node '" << node_name << "' apply mask name \'" << trimmed_content
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
      VLOG("ApplyMask", pFATAL) << "When reading node '" << node_name << "' apply mask name \'" << trimmed_content
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
      VLOG("ApplyMask", pFATAL) << "When reading node '" << node_name << "' apply mask name \'" << trimmed_content
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

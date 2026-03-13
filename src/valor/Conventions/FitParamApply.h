//____________________________________________________________________________
/*!

\file      FitParamApply.h

\brief     Controls which parameters get applied to which beam/sample/mode bins.

\author    Steve Dennis <s.r.dennis \at liverpool.ac.uk>
           University of Liverpool

\created   March 20, 2016

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_apply_params_h_
#define _valor_apply_params_h_

#include <bitset>
#include <map>

// FMWK
#include "valor/Conventions/NuFlavour.h"
#include "valor/Conventions/BeamConf.h"
#include "valor/Conventions/NuclearTarget.h"
#include "valor/Conventions/ReacMode.h"
#include "valor/Conventions/Sample.h"
#include "valor/Conventions/Detector.h"
#include "valor/Utilities/UtlXML.h"

namespace valor {
  namespace conv {

    static const unsigned long kApplyMaskOne     = 1ul;
    static const int nbitsBeamApplyMask          = 2;
    static const int nbitsFlavApplyMask          = 6;
    static const int nbitsSelecApplyMask         = 38;
    static const int nbitsReacTypeApplyMask      = 29;
    static const int nbitsDetectorApplyMask      = 7;
    static const int nbitsNuclearTargetApplyMask = 7;

    typedef std::bitset<nbitsBeamApplyMask>           BeamApplyMask_t;
    typedef std::bitset<nbitsFlavApplyMask>           FlavApplyMask_t;
    typedef std::bitset<nbitsSelecApplyMask>          SelecApplyMask_t;
    typedef std::bitset<nbitsReacTypeApplyMask>       ReacTypeApplyMask_t;
    typedef std::bitset<nbitsDetectorApplyMask>       DetectorApplyMask_t;
    typedef std::bitset<nbitsNuclearTargetApplyMask>  NuclearTargetApplyMask_t;

    // Which physics parameters are applied to which beam/sample/mode
    // This one uses only the parameters listed in kFitParam and ArrIdx must be used to get the right value.
    typedef struct {
      DetectorApplyMask_t      detector;
      BeamApplyMask_t          beam;
      FlavApplyMask_t          flav_init;
      FlavApplyMask_t          flav_final;
      SelecApplyMask_t         selec;
      ReacTypeApplyMask_t      reactype;
      NuclearTargetApplyMask_t target;
    } ApplyMask_t;

    static const BeamApplyMask_t kbApplyToFHC         = (kApplyMaskOne <<  0 ); // Current
    static const BeamApplyMask_t kbApplyToRHC         = (kApplyMaskOne <<  1 );

    static const FlavApplyMask_t kfApplyToNuMu            = (kApplyMaskOne << 0 ); // True flavour
    static const FlavApplyMask_t kfApplyToNuMuBar         = (kApplyMaskOne << 1 );
    static const FlavApplyMask_t kfApplyToNuE             = (kApplyMaskOne << 2 );
    static const FlavApplyMask_t kfApplyToNuEBar          = (kApplyMaskOne << 3 );
    static const FlavApplyMask_t kfApplyToCosmic          = (kApplyMaskOne << 4 );
    static const FlavApplyMask_t kfApplyToDirt            = (kApplyMaskOne << 5 );

    static const SelecApplyMask_t ksApplyToNearNuMuCC      = (kApplyMaskOne <<  0 ); // Sample
    static const SelecApplyMask_t ksApplyToNearNuMuCCOAA1  = (kApplyMaskOne <<  1 );
    static const SelecApplyMask_t ksApplyToNearNuMuCCOAA2  = (kApplyMaskOne <<  2 );
    static const SelecApplyMask_t ksApplyToNearNuMuCCOAA3  = (kApplyMaskOne <<  3 );
    static const SelecApplyMask_t ksApplyToNearNuMuCCOAA4  = (kApplyMaskOne <<  4 );
    static const SelecApplyMask_t ksApplyToNearNuMuCCOAA5  = (kApplyMaskOne <<  5 );
    static const SelecApplyMask_t ksApplyToNearNuMuCCOAA6  = (kApplyMaskOne <<  6 );
    static const SelecApplyMask_t ksApplyToNearNuMuCCOAA7  = (kApplyMaskOne <<  7 );
    static const SelecApplyMask_t ksApplyToNearNuMuCCOAA8  = (kApplyMaskOne <<  8 );
    static const SelecApplyMask_t ksApplyToNearNuMuCCOAA9  = (kApplyMaskOne <<  9 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_0p0pi      = (kApplyMaskOne << 10 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_0p0piOAA1  = (kApplyMaskOne << 11 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_0p0piOAA2  = (kApplyMaskOne << 12 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_0p0piOAA3  = (kApplyMaskOne << 13 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_0p0piOAA4  = (kApplyMaskOne << 14 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_0p0piOAA5  = (kApplyMaskOne << 15 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_0p0piOAA6  = (kApplyMaskOne << 16 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_0p0piOAA7  = (kApplyMaskOne << 17 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_0p0piOAA8  = (kApplyMaskOne << 18 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_0p0piOAA9  = (kApplyMaskOne << 19 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1p0pi      = (kApplyMaskOne << 20 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1p0piOAA1  = (kApplyMaskOne << 21 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1p0piOAA2  = (kApplyMaskOne << 22 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1p0piOAA3  = (kApplyMaskOne << 23 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1p0piOAA4  = (kApplyMaskOne << 24 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1p0piOAA5  = (kApplyMaskOne << 25 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1p0piOAA6  = (kApplyMaskOne << 26 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1p0piOAA7  = (kApplyMaskOne << 27 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1p0piOAA8  = (kApplyMaskOne << 28 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1p0piOAA9  = (kApplyMaskOne << 29 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_Np0pi      = (kApplyMaskOne << 30 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_Np0piOAA1  = (kApplyMaskOne << 31 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_Np0piOAA2  = (kApplyMaskOne << 32 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_Np0piOAA3  = (kApplyMaskOne << 33 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_Np0piOAA4  = (kApplyMaskOne << 34 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_Np0piOAA5  = (kApplyMaskOne << 35 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_Np0piOAA6  = (kApplyMaskOne << 36 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_Np0piOAA7  = (kApplyMaskOne << 37 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_Np0piOAA8  = (kApplyMaskOne << 38 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_Np0piOAA9  = (kApplyMaskOne << 39 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1pi      = (kApplyMaskOne << 40 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1piOAA1  = (kApplyMaskOne << 41 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1piOAA2  = (kApplyMaskOne << 42 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1piOAA3  = (kApplyMaskOne << 43 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1piOAA4  = (kApplyMaskOne << 44 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1piOAA5  = (kApplyMaskOne << 45 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1piOAA6  = (kApplyMaskOne << 46 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1piOAA7  = (kApplyMaskOne << 47 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1piOAA8  = (kApplyMaskOne << 48 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_1piOAA9  = (kApplyMaskOne << 49 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_Other      = (kApplyMaskOne << 50 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_OtherOAA1  = (kApplyMaskOne << 51 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_OtherOAA2  = (kApplyMaskOne << 52 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_OtherOAA3  = (kApplyMaskOne << 53 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_OtherOAA4  = (kApplyMaskOne << 54 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_OtherOAA5  = (kApplyMaskOne << 55 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_OtherOAA6  = (kApplyMaskOne << 56 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_OtherOAA7  = (kApplyMaskOne << 57 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_OtherOAA8  = (kApplyMaskOne << 58 );
    static const SelecApplyMask_t ksApplyToNearNuMuCC_OtherOAA9  = (kApplyMaskOne << 59 );
    static const SelecApplyMask_t ksApplyToFarNuMuCC     = (kApplyMaskOne << 60);
    static const SelecApplyMask_t ksApplyToFarNuMuCC_0pi = (kApplyMaskOne << 61);
    static const SelecApplyMask_t ksApplyToFarNuMuCC_1pi = (kApplyMaskOne << 62);
    static const SelecApplyMask_t ksApplyToFarNuMuCC_Npi = (kApplyMaskOne << 63);

    static const ReacTypeApplyMask_t ktApplyToCCQE         = (kApplyMaskOne <<  0 ); // Reaction type
    static const ReacTypeApplyMask_t ktApplyToCCMEC        = (kApplyMaskOne <<  1 );
    static const ReacTypeApplyMask_t ktApplyToCC1piC       = (kApplyMaskOne <<  2 );
    static const ReacTypeApplyMask_t ktApplyToCC1pi0       = (kApplyMaskOne <<  3 );
    static const ReacTypeApplyMask_t ktApplyToCC2piC       = (kApplyMaskOne <<  4 );
    static const ReacTypeApplyMask_t ktApplyToCC2pi0       = (kApplyMaskOne <<  5 );
    static const ReacTypeApplyMask_t ktApplyToCCpiCpi0     = (kApplyMaskOne <<  6 );
    static const ReacTypeApplyMask_t ktApplyToCCcoh        = (kApplyMaskOne <<  7 );
    static const ReacTypeApplyMask_t ktApplyToCCIMD        = (kApplyMaskOne <<  8 );
    static const ReacTypeApplyMask_t ktApplyToCCELScat     = (kApplyMaskOne <<  9 );
    static const ReacTypeApplyMask_t ktApplyToCCDIS        = (kApplyMaskOne << 10 );
    static const ReacTypeApplyMask_t ktApplyToCCMpi        = (kApplyMaskOne << 11 );
    static const ReacTypeApplyMask_t ktApplyToCCmisc       = (kApplyMaskOne << 12 );
    static const ReacTypeApplyMask_t ktApplyToCCoth        = (kApplyMaskOne << 13 );
    static const ReacTypeApplyMask_t ktApplyToNCEL         = (kApplyMaskOne << 14 );
    static const ReacTypeApplyMask_t ktApplyToNCMEC        = (kApplyMaskOne << 15 );
    static const ReacTypeApplyMask_t ktApplyToNC1piC       = (kApplyMaskOne << 16 );
    static const ReacTypeApplyMask_t ktApplyToNC1pi0       = (kApplyMaskOne << 17 );
    static const ReacTypeApplyMask_t ktApplyToNC2piC       = (kApplyMaskOne << 18 );
    static const ReacTypeApplyMask_t ktApplyToNC2pi0       = (kApplyMaskOne << 19 );
    static const ReacTypeApplyMask_t ktApplyToNCpiCpi0     = (kApplyMaskOne << 20 );
    static const ReacTypeApplyMask_t ktApplyToNCcoh        = (kApplyMaskOne << 21 );
    static const ReacTypeApplyMask_t ktApplyToNC1Gamma     = (kApplyMaskOne << 22 );
    static const ReacTypeApplyMask_t ktApplyToNCELScat     = (kApplyMaskOne << 23 );
    static const ReacTypeApplyMask_t ktApplyToNCoth        = (kApplyMaskOne << 24 );
    static const ReacTypeApplyMask_t ktApplyToNuEELScat    = (kApplyMaskOne << 25 ); //only for nue: CC + NC + interference
    static const ReacTypeApplyMask_t ktApplyToOther        = (kApplyMaskOne << 26 ); //only for nue: CC + NC + interference
    static const ReacTypeApplyMask_t ktApplyToCosmic       = (kApplyMaskOne << 27 ); //only for nue: CC + NC + interference
    static const ReacTypeApplyMask_t ktApplyToDirt         = (kApplyMaskOne << 28 ); //only for nue: CC + NC + interference

    static const DetectorApplyMask_t kdApplyToDUNEND          = (kApplyMaskOne << 0 ); // Dectectors
    static const DetectorApplyMask_t kdApplyToDUNEFD          = (kApplyMaskOne << 1 );
    static const DetectorApplyMask_t kdApplyToSBND            = (kApplyMaskOne << 2 );
    static const DetectorApplyMask_t kdApplyToMicroBooNE      = (kApplyMaskOne << 3 );
    static const DetectorApplyMask_t kdApplyToICARUS          = (kApplyMaskOne << 4 );
    static const DetectorApplyMask_t kdApplyToND280           = (kApplyMaskOne << 5 );
    static const DetectorApplyMask_t kdApplyToSuperK          = (kApplyMaskOne << 6 );

    // Nuclear Targets
    static const NuclearTargetApplyMask_t kntApplyToInclusive = (kApplyMaskOne << 0 ); // Nuclear targets.
    static const NuclearTargetApplyMask_t kntApplyToOther     = (kApplyMaskOne << 1 );
    static const NuclearTargetApplyMask_t kntApplyToHydrogen  = (kApplyMaskOne << 2 );
    static const NuclearTargetApplyMask_t kntApplyToCarbon    = (kApplyMaskOne << 3 );
    static const NuclearTargetApplyMask_t kntApplyToOxygen    = (kApplyMaskOne << 4 );
    static const NuclearTargetApplyMask_t kntApplyToArgon     = (kApplyMaskOne << 5 );
    static const NuclearTargetApplyMask_t kntApplyToCalcium   = (kApplyMaskOne << 6 );







    extern const BeamApplyMask_t kbApplyToAll;

    // Nu flavour
    extern const FlavApplyMask_t kfApplyToNuAny; // NuAny means any non-barred. If it's not written that it applies to osc or unosc flavour, it is for unosc.
    extern const FlavApplyMask_t kfApplyToNuBarAny;
    extern const FlavApplyMask_t kfApplyToNuEAny;
    extern const FlavApplyMask_t kfApplyToNuMuAny;
    extern const FlavApplyMask_t kfApplyToAllCosmic;
    extern const FlavApplyMask_t kfApplyToAllDirt;
    extern const FlavApplyMask_t kfApplyToAll;

    // Reaction type
    extern const ReacTypeApplyMask_t ktApplyToCCQELike;
    extern const ReacTypeApplyMask_t ktApplyToAllCCELScat; // nu + e CC elastic scattering (inverse muon/tau decay) and nue+e
    extern const ReacTypeApplyMask_t ktApplyToAllCC1pi;
    extern const ReacTypeApplyMask_t ktApplyToAllCC2pi;
    extern const ReacTypeApplyMask_t ktApplyToAllCCn0pi;
    extern const ReacTypeApplyMask_t ktApplyToAllCCNonRes;
    extern const ReacTypeApplyMask_t ktApplyToAllCC;
    extern const ReacTypeApplyMask_t ktApplyToCCnQE;

    extern const ReacTypeApplyMask_t ktApplyToNCElLike; //this is not used, modes disabled
    extern const ReacTypeApplyMask_t ktApplyToAllNCELScat; //nu+e NC elastic scattering
    extern const ReacTypeApplyMask_t ktApplyToAllNC1pi;
    extern const ReacTypeApplyMask_t ktApplyToAllNC2pi;
    extern const ReacTypeApplyMask_t ktApplyToAllNCn0pi;
    extern const ReacTypeApplyMask_t ktApplyToAllNCNonRes;
    extern const ReacTypeApplyMask_t ktApplyToAllNC;

    extern const ReacTypeApplyMask_t ktApplyToAllCoh;
    extern const ReacTypeApplyMask_t ktApplyToAllQELike; // CC & NC QE Like
    extern const ReacTypeApplyMask_t ktApplyToAllMEC;
    extern const ReacTypeApplyMask_t ktApplyToAllELScat;
    extern const ReacTypeApplyMask_t ktApplyToAll1pi;
    extern const ReacTypeApplyMask_t ktApplyToAll2pi;
    extern const ReacTypeApplyMask_t ktApplyToAlln0pi;
    extern const ReacTypeApplyMask_t ktApplyToAllNonRes;
    extern const ReacTypeApplyMask_t ktApplyToAllCosmic;
    extern const ReacTypeApplyMask_t ktApplyToAllDirt;

    extern const ReacTypeApplyMask_t ktApplyToAll;
    extern const ReacTypeApplyMask_t ktApplyToAllPlusCosmic;
    extern const ReacTypeApplyMask_t ktApplyToAllPlusDirt;
    extern const ReacTypeApplyMask_t ktApplyToAllPi;
    extern const ReacTypeApplyMask_t ktApplyToAllWithPiFSI;
    extern const ReacTypeApplyMask_t ktApplyToAllButCoh;
    extern const ReacTypeApplyMask_t ktApplyToAllButELScat;
    extern const ReacTypeApplyMask_t ktApplyToAllWithFSI;
    extern const ReacTypeApplyMask_t ktApplyToAllButCosmic;
    extern const ReacTypeApplyMask_t ktApplyToPiAndOth;

    extern const ReacTypeApplyMask_t ktApplyToT2KLegCCoth;
    extern const ReacTypeApplyMask_t ktApplyToCCDISMpi;

    // Detectors
    extern const DetectorApplyMask_t kdApplyToAllDUNE;
    extern const DetectorApplyMask_t kdApplyToAllSBN;
    extern const DetectorApplyMask_t kdApplyToAllSBN2Det;
    extern const DetectorApplyMask_t kdApplyToAllT2K;
    extern const DetectorApplyMask_t kdApplyToAllND;
    extern const DetectorApplyMask_t kdApplyToAll;


    // Samples
    extern const SelecApplyMask_t ksApplyToAllND;
    extern const SelecApplyMask_t ksApplyToAllFD;
    extern const SelecApplyMask_t ksApplyToAll;
    extern const SelecApplyMask_t ksApplyToAllNC;
    extern const SelecApplyMask_t ksApplyToAllNuMuCC;
    extern const SelecApplyMask_t ksApplyToAllNuECC;
    extern const SelecApplyMask_t ksApplyToAllButNuECC;
    extern const SelecApplyMask_t ksApplyToELikeRingAny;  // SuperK selections
    extern const SelecApplyMask_t ksApplyTo1RingAny;      // SuperK selections
    extern const SelecApplyMask_t ksApplyToAllRings;      // SuperK selections
    extern const SelecApplyMask_t ksApplyToLowRes;
    extern const SelecApplyMask_t ksApplyToHighRes;
    extern const SelecApplyMask_t ksApplyToInnerNuMuPRISM3;
    extern const SelecApplyMask_t ksApplyToAllNuMuPRISM3;
    extern const SelecApplyMask_t ksApplyToOuterNuMuPRISM3;



    // Targets
    extern const NuclearTargetApplyMask_t kntApplyToAll;






  } //conv
  namespace util{
    // Singleton to keep maps containing the apply masks and their corresponding names.
    // There are cases where either the name or the bitset is know and the other is wanted, usually in VALOR we have an
    // AsString() function and iterate over that to get the bitset from a string, but here that can mean iterating over
    // 2^30 cases for each parameter which is prohibitively expensive, so have a vector of pairs instead where we iterate
    // over ~35 entries
    class ApplyMaskNaming {
      public:
        static const ApplyMaskNaming * Instance();
        static       ApplyMaskNaming * MutableInstance();

        template<size_t sz> struct bitset_comparer {
          bool operator() (const std::bitset<sz> &b1, const std::bitset<sz> &b2) const {
            return b1.to_ulong() < b2.to_ulong();
          }
        };

        // Return the apply mask naming maps with key = apply mask, value = name
        const std::map<conv::BeamApplyMask_t,          std::string, ApplyMaskNaming::bitset_comparer<conv::nbitsBeamApplyMask>>          GetBeamMaskToNameMap         (void) const { return fBeamMaskToName; }
        const std::map<conv::FlavApplyMask_t,          std::string, ApplyMaskNaming::bitset_comparer<conv::nbitsFlavApplyMask>>          GetFlavMaskToNameMap         (void) const { return fFlavMaskToName; }
        const std::map<conv::SelecApplyMask_t,         std::string, ApplyMaskNaming::bitset_comparer<conv::nbitsSelecApplyMask>>         GetSelecMaskToNameMap        (void) const { return fSelecMaskToName; }
        const std::map<conv::ReacTypeApplyMask_t,      std::string, ApplyMaskNaming::bitset_comparer<conv::nbitsReacTypeApplyMask>>      GetReacTypeMaskToNameMap     (void) const { return fReacTypeMaskToName; }
        const std::map<conv::DetectorApplyMask_t,      std::string, ApplyMaskNaming::bitset_comparer<conv::nbitsDetectorApplyMask>>      GetDetectorMaskToNameMap     (void) const { return fDetectorMaskToName; }
        const std::map<conv::NuclearTargetApplyMask_t, std::string, ApplyMaskNaming::bitset_comparer<conv::nbitsNuclearTargetApplyMask>> GetNuclearTargetMaskToNameMap(void) const { return fNuclearTargetMaskToName; }

        // Return the apply mask naming maps with key = name, value = apply mask
        const std::map<std::string, conv::BeamApplyMask_t>          GetBeamNameToMaskMap         (void) const { return fBeamNameToMask; }
        const std::map<std::string, conv::FlavApplyMask_t>          GetFlavNameToMaskMap         (void) const { return fFlavNameToMask; }
        const std::map<std::string, conv::SelecApplyMask_t>         GetSelecNameToMaskMap        (void) const { return fSelecNameToMask; }
        const std::map<std::string, conv::ReacTypeApplyMask_t>      GetReacTypeNameToMaskMap     (void) const { return fReacTypeNameToMask; }
        const std::map<std::string, conv::DetectorApplyMask_t>      GetDetectorNameToMaskMap     (void) const { return fDetectorNameToMask; }
        const std::map<std::string, conv::NuclearTargetApplyMask_t> GetNuclearTargetNameToMaskMap(void) const { return fNuclearTargetNameToMask; }

      private:
        ApplyMaskNaming();
        ApplyMaskNaming(const ApplyMaskNaming &) {}
        ~ApplyMaskNaming() {}

        void Init();

        static ApplyMaskNaming * fInstance;

        std::map<conv::BeamApplyMask_t,          std::string, ApplyMaskNaming::bitset_comparer<conv::nbitsBeamApplyMask>>          fBeamMaskToName;
        std::map<conv::FlavApplyMask_t,          std::string, ApplyMaskNaming::bitset_comparer<conv::nbitsFlavApplyMask>>          fFlavMaskToName;
        std::map<conv::SelecApplyMask_t,         std::string, ApplyMaskNaming::bitset_comparer<conv::nbitsSelecApplyMask>>         fSelecMaskToName;
        std::map<conv::ReacTypeApplyMask_t,      std::string, ApplyMaskNaming::bitset_comparer<conv::nbitsReacTypeApplyMask>>      fReacTypeMaskToName;
        std::map<conv::DetectorApplyMask_t,      std::string, ApplyMaskNaming::bitset_comparer<conv::nbitsDetectorApplyMask>>      fDetectorMaskToName;
        std::map<conv::NuclearTargetApplyMask_t, std::string, ApplyMaskNaming::bitset_comparer<conv::nbitsNuclearTargetApplyMask>> fNuclearTargetMaskToName;

        // Reverse of the above maps
        std::map<std::string, conv::BeamApplyMask_t>          fBeamNameToMask;
        std::map<std::string, conv::FlavApplyMask_t>          fFlavNameToMask;
        std::map<std::string, conv::SelecApplyMask_t>         fSelecNameToMask;
        std::map<std::string, conv::ReacTypeApplyMask_t>      fReacTypeNameToMask;
        std::map<std::string, conv::DetectorApplyMask_t>      fDetectorNameToMask;
        std::map<std::string, conv::NuclearTargetApplyMask_t> fNuclearTargetNameToMask;

        struct Cleaner {
          void DummyMethodAndSilentCompiler() { }
          ~Cleaner() {
            if (ApplyMaskNaming::fInstance !=0) {
              delete ApplyMaskNaming::fInstance;
              ApplyMaskNaming::fInstance = 0;
            }
          }
        };
        friend struct Cleaner;
      };

    // Given an apply mask return the name of the mask
    inline std::string BeamMaskAsString         (conv::BeamApplyMask_t          mask) { return util::ApplyMaskNaming::Instance()->GetBeamMaskToNameMap().at(mask); }
    inline std::string FlavMaskAsString         (conv::FlavApplyMask_t          mask) { return util::ApplyMaskNaming::Instance()->GetFlavMaskToNameMap().at(mask); }
    inline std::string SelecMaskAsString        (conv::SelecApplyMask_t         mask) { return util::ApplyMaskNaming::Instance()->GetSelecMaskToNameMap().at(mask); }
    inline std::string ReacTypeMaskAsString     (conv::ReacTypeApplyMask_t      mask) { return util::ApplyMaskNaming::Instance()->GetReacTypeMaskToNameMap().at(mask); }
    inline std::string DetectorMaskAsString     (conv::DetectorApplyMask_t      mask) { return util::ApplyMaskNaming::Instance()->GetDetectorMaskToNameMap().at(mask); }
    inline std::string NuclearTargetMaskAsString(conv::NuclearTargetApplyMask_t mask) { return util::ApplyMaskNaming::Instance()->GetNuclearTargetMaskToNameMap().at(mask); }

    // Given the name of a mask, return the apply mask bitset
    inline conv::BeamApplyMask_t          BeamMaskFromString         (std::string name) { return util::ApplyMaskNaming::Instance()->GetBeamNameToMaskMap().at(name); }
    inline conv::FlavApplyMask_t          FlavMaskFromString         (std::string name) { return util::ApplyMaskNaming::Instance()->GetFlavNameToMaskMap().at(name); }
    inline conv::SelecApplyMask_t         SelecMaskFromString        (std::string name) { return util::ApplyMaskNaming::Instance()->GetSelecNameToMaskMap().at(name); }
    inline conv::ReacTypeApplyMask_t      ReacTypeMaskFromString     (std::string name) { return util::ApplyMaskNaming::Instance()->GetReacTypeNameToMaskMap().at(name); }
    inline conv::DetectorApplyMask_t      DetectorMaskFromString     (std::string name) { return util::ApplyMaskNaming::Instance()->GetDetectorNameToMaskMap().at(name); }
    inline conv::NuclearTargetApplyMask_t NuclearTargetMaskFromString(std::string name) { return util::ApplyMaskNaming::Instance()->GetNuclearTargetNameToMaskMap().at(name); }

    // Which masks apply to which beam modes
    const conv::BeamApplyMask_t ApplyToBeamMode(conv::BeamConf_t b);

    // Which masks apply to which samples
    const conv::SelecApplyMask_t ApplyToSample(conv::Sample_t s);

    // Which masks apply to which parent particle flavour
    const conv::DetectorApplyMask_t ApplyToDetector(conv::Detector_t d);

    // Which masks apply to which parent particle flavour
    const conv::FlavApplyMask_t ApplyToFlavour(conv::NuFlavour_t f);

    // Which masks apply to which reaction type
    const conv::ReacTypeApplyMask_t ApplyToReacType(conv::ReacType_t t);

    // Which masks apply to which nucleus
    const conv::NuclearTargetApplyMask_t ApplyToNuclearTarget(conv::NuclearTarget_t nt);

    conv::ApplyMask_t              ReadApplyMaskAll          (TXMLEngine* xmleng, XMLNodePointer_t top_node                  );
    conv::DetectorApplyMask_t      ReadDetectorApplyMask     (TXMLEngine* xmleng, XMLNodePointer_t top_node, string node_name);
    conv::BeamApplyMask_t          ReadBeamApplyMask         (TXMLEngine* xmleng, XMLNodePointer_t top_node, string node_name);
    conv::FlavApplyMask_t          ReadFlavApplyMask         (TXMLEngine* xmleng, XMLNodePointer_t top_node, string node_name);
    conv::SelecApplyMask_t         ReadSelecApplyMask        (TXMLEngine* xmleng, XMLNodePointer_t top_node, string node_name);
    conv::ReacTypeApplyMask_t      ReadReacTypeApplyMask     (TXMLEngine* xmleng, XMLNodePointer_t top_node, string node_name);
    conv::NuclearTargetApplyMask_t ReadNuclearTargetApplyMask(TXMLEngine* xmleng, XMLNodePointer_t top_node, string node_name);
    bool                           IsAppliedTo               (conv::ApplyMask_t mask_to_check, conv::BeamConf_t b, conv::Sample_t s,
                                                              conv::Detector_t d, conv::ReacMode_t m, conv::NuclearTarget_t nt);
    bool                           IsAppliedTo               (conv::ApplyMask_t mask_to_check, conv::ApplyMask_t mask);
    std::string                    AsString                  (const conv::ApplyMask_t & mask);
    std::string                    ApplyMaskAsString         (const conv::ApplyMask_t & mask);
  } //util

  std::ostream& operator<<(std::ostream& s, const conv::ApplyMask_t & m);

} //valor

#endif // _valor_apply_params_h_

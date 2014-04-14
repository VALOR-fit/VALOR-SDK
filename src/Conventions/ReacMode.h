//____________________________________________________________________________
/*!

\class     t2kana::conv::ReacMode_t

\brief     Enumeration of true reaction modes considered separately within 
           each analysis sample

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   Oct 20, 2010

\copyright (c) 2010-2014, The VALOR group

*/
//____________________________________________________________________________

#ifndef _t2kana_reaction_modes_h
#define _t2kana_reaction_modes_h

#include <string>

using std::string;

namespace t2kana {

namespace constants {
static const int kNReacModes = 53;  
}

namespace conv {

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
   kmNuMuCCoth,           // numu    CC other
   kmNuMuNCEL,            // numu    NC EL
   kmNuMuNCMEC,           // numu    NC MEC
   kmNuMuNC1piC,          // numu    NC 1 pi+/-
   kmNuMuNC1pi0,          // numu    NC 1 pi0
   kmNuMuNC2piC,          // numu    NC 2 pi+/-
   kmNuMuNC2pi0,          // numu    NC 2 pi0
   kmNuMuNCpiCpi0,        // numu    NC 1 pi0 + 1 pi+/-
   kmNuMuNCcoh,           // numu    NC coherent
   kmNuMuNCoth,           // numu    NC other
   kmNuMuBarCCQE,         // numubar CC QE
   kmNuMuBarCCMEC,        // numubar CC MEC
   kmNuMuBarCC1piC,       // numubar CC 1 pi+/-
   kmNuMuBarCC1pi0,       // numubar CC 1 pi0
   kmNuMuBarCC2piC,       // numubar CC 2 pi+/-
   kmNuMuBarCC2pi0,       // numubar CC 2 pi0
   kmNuMuBarCCpiCpi0,     // numubar CC 1 pi0 + 1 pi+/-
   kmNuMuBarCCcoh,        // numubar CC coherent
   kmNuMuBarCCoth,        // numubar CC other
   kmNuMuBarNCEL,         // numubar NC EL
   kmNuMuBarNCMEC,        // numubar NC MEC
   kmNuMuBarNC1piC,       // numubar NC 1 pi+/-
   kmNuMuBarNC1pi0,       // numubar NC 1 pi0
   kmNuMuBarNC2piC,       // numubar NC 2 pi+/-
   kmNuMuBarNC2pi0,       // numubar NC 2 pi0
   kmNuMuBarNCpiCpi0,     // numubar NC 1 pi0 + 1 pi+/-
   kmNuMuBarNCcoh,        // numubar NC coherent
   kmNuMuBarNCoth,        // numubar NC other
   kmNuECCQE,             // nue     CC QE
   kmNuECCMEC,            // nue     CC MEC
   kmNuECC1piC,           // nue     CC 1 pi+/-
   kmNuECC1pi0,           // nue     CC 1 pi0
   kmNuECC2piC,           // nue     CC 2 pi+/-
   kmNuECC2pi0,           // nue     CC 2 pi0
   kmNuECCpiCpi0,         // nue     CC 1 pi0 + 1 pi+/-
   kmNuECCcoh,            // nue     CC coherent
   kmNuECCoth,            // nue     CC other
   kmNuENCEL,             // nue     NC EL
   kmNuENCMEC,            // nue     NC MEC
   kmNuENC1piC,           // nue     NC 1 pi+/-
   kmNuENC1pi0,           // nue     NC 1 pi0
   kmNuENC2piC,           // nue     NC 2 pi+/-
   kmNuENC2pi0,           // nue     NC 2 pi0
   kmNuENCpiCpi0,         // nue     NC 1 pi0 + 1 pi+/-
   kmNuENCcoh,            // nue     NC coherent
   kmNuENCoth,            // nue     NC other
   kmNuEBarCCQE,          // nuebar  CC QE
   kmNuEBarCCMEC,         // nuebar  CC MEC
   kmNuEBarCC1piC,        // nuebar  CC 1 pi+/-
   kmNuEBarCC1pi0,        // nuebar  CC 1 pi0
   kmNuEBarCC2piC,        // nuebar  CC 2 pi+/-
   kmNuEBarCC2pi0,        // nuebar  CC 2 pi0
   kmNuEBarCCpiCpi0,      // nuebar  CC 1 pi0 + 1 pi+/-
   kmNuEBarCCcoh,         // nuebar  CC coherent
   kmNuEBarCCoth,         // nuebar  CC other
   kmNuEBarNCEL,          // nuebar  NC EL
   kmNuEBarNCMEC,         // nuebar  NC MEC
   kmNuEBarNC1piC,        // nuebar  NC 1 pi+/-
   kmNuEBarNC1pi0,        // nuebar  NC 1 pi0
   kmNuEBarNC2piC,        // nuebar  NC 2 pi+/-
   kmNuEBarNC2pi0,        // nuebar  NC 2 pi0
   kmNuEBarNCpiCpi0,      // nuebar  NC 1 pi0 + 1 pi+/-
   kmNuEBarNCcoh,         // nuebar  NC coherent
   kmNuEBarNCoth          // nuebar  NC other
} 
ReacMode_t; 

} // conv

namespace util {

  string AsString(conv::ReacMode_t m);

}

} // t2kana
  
#endif 


//____________________________________________________________________________
/*!

\file      NDSampleModes

\brief     Define which true reaction modes to consider for each ND sample

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           George Christodoulou <georgios \at hep.ph.liv.ac.uk>
           University of Liverpool

           Nick Grant <n.grant \at lancaster.ac.uk>
           University of Lancaster

\created   March 20, 2014

\copyright (c) 2010-2014, The VALOR group

*/
//____________________________________________________________________________

#ifndef _t2kana_nd_sample_modes_h_
#define _t2kana_nd_sample_modes_h_

#include "Conventions/ReacMode.h"
#include "NDFit/NDSample.h"

namespace t2kana {  
namespace conv   {

//
// Define which true reactuin modes are considered for each ND sample 
// included in the analysis
//

static const int kNNDModes  = 72;

static const ReacMode_t kNDReactionModes[kNNDSamples][kNNDModes] = 
{
  // sample: ksNDNuMuCC
  {
    kmNuMuCCQE,            
    kmNuMuCCMEC,           
    kmNuMuCC1piC,          
    kmNuMuCC1pi0,          
    kmNuMuCC2piC,          
    kmNuMuCC2pi0,          
    kmNuMuCCpiCpi0,        
    kmNuMuCCcoh,           
    kmNuMuCCoth,           
    kmNuMuNCEL,            
    kmNuMuNCMEC,           
    kmNuMuNC1piC,          
    kmNuMuNC1pi0,          
    kmNuMuNC2piC,          
    kmNuMuNC2pi0,          
    kmNuMuNCpiCpi0,        
    kmNuMuNCcoh,           
    kmNuMuNCoth,           
    kmNuMuBarCCQE,         
    kmNuMuBarCCMEC,        
    kmNuMuBarCC1piC,       
    kmNuMuBarCC1pi0,       
    kmNuMuBarCC2piC,       
    kmNuMuBarCC2pi0,       
    kmNuMuBarCCpiCpi0,     
    kmNuMuBarCCcoh,        
    kmNuMuBarCCoth,        
    kmNuMuBarNCEL,         
    kmNuMuBarNCMEC,        
    kmNuMuBarNC1piC,       
    kmNuMuBarNC1pi0,       
    kmNuMuBarNC2piC,       
    kmNuMuBarNC2pi0,       
    kmNuMuBarNCpiCpi0,     
    kmNuMuBarNCcoh,        
    kmNuMuBarNCoth,        
    kmNuECCQE,             
    kmNuECCMEC,            
    kmNuECC1piC,           
    kmNuECC1pi0,           
    kmNuECC2piC,           
    kmNuECC2pi0,           
    kmNuECCpiCpi0,         
    kmNuECCcoh,            
    kmNuECCoth,            
    kmNuENCEL,             
    kmNuENCMEC,           
    kmNuENC1piC,           
    kmNuENC1pi0,           
    kmNuENC2piC,           
    kmNuENC2pi0,           
    kmNuENCpiCpi0,         
    kmNuENCcoh,            
    kmNuENCoth,            
    kmNuEBarCCQE,          
    kmNuEBarCCMEC,         
    kmNuEBarCC1piC,        
    kmNuEBarCC1pi0,        
    kmNuEBarCC2piC,        
    kmNuEBarCC2pi0,        
    kmNuEBarCCpiCpi0,      
    kmNuEBarCCcoh,         
    kmNuEBarCCoth,         
    kmNuEBarNCEL,          
    kmNuEBarNCMEC,         
    kmNuEBarNC1piC,        
    kmNuEBarNC1pi0,        
    kmNuEBarNC2piC,        
    kmNuEBarNC2pi0,        
    kmNuEBarNCpiCpi0,      
    kmNuEBarNCcoh,         
    kmNuEBarNCoth          
  },

  // sample: ksNDNuMuCC
  {
    kmNuMuCCQE,            
    kmNuMuCCMEC,           
    kmNuMuCC1piC,          
    kmNuMuCC1pi0,          
    kmNuMuCC2piC,          
    kmNuMuCC2pi0,          
    kmNuMuCCpiCpi0,        
    kmNuMuCCcoh,           
    kmNuMuCCoth,           
    kmNuMuNCEL,            
    kmNuMuNCMEC,           
    kmNuMuNC1piC,          
    kmNuMuNC1pi0,          
    kmNuMuNC2piC,          
    kmNuMuNC2pi0,          
    kmNuMuNCpiCpi0,        
    kmNuMuNCcoh,           
    kmNuMuNCoth,           
    kmNuMuBarCCQE,         
    kmNuMuBarCCMEC,        
    kmNuMuBarCC1piC,       
    kmNuMuBarCC1pi0,       
    kmNuMuBarCC2piC,       
    kmNuMuBarCC2pi0,       
    kmNuMuBarCCpiCpi0,     
    kmNuMuBarCCcoh,        
    kmNuMuBarCCoth,        
    kmNuMuBarNCEL,         
    kmNuMuBarNCMEC,        
    kmNuMuBarNC1piC,       
    kmNuMuBarNC1pi0,       
    kmNuMuBarNC2piC,       
    kmNuMuBarNC2pi0,       
    kmNuMuBarNCpiCpi0,     
    kmNuMuBarNCcoh,        
    kmNuMuBarNCoth,        
    kmNuECCQE,             
    kmNuECCMEC,            
    kmNuECC1piC,           
    kmNuECC1pi0,           
    kmNuECC2piC,           
    kmNuECC2pi0,           
    kmNuECCpiCpi0,         
    kmNuECCcoh,            
    kmNuECCoth,            
    kmNuENCEL,             
    kmNuENCMEC,           
    kmNuENC1piC,           
    kmNuENC1pi0,           
    kmNuENC2piC,           
    kmNuENC2pi0,           
    kmNuENCpiCpi0,         
    kmNuENCcoh,            
    kmNuENCoth,            
    kmNuEBarCCQE,          
    kmNuEBarCCMEC,         
    kmNuEBarCC1piC,        
    kmNuEBarCC1pi0,        
    kmNuEBarCC2piC,        
    kmNuEBarCC2pi0,        
    kmNuEBarCCpiCpi0,      
    kmNuEBarCCcoh,         
    kmNuEBarCCoth,         
    kmNuEBarNCEL,          
    kmNuEBarNCMEC,         
    kmNuEBarNC1piC,        
    kmNuEBarNC1pi0,        
    kmNuEBarNC2piC,        
    kmNuEBarNC2pi0,        
    kmNuEBarNCpiCpi0,      
    kmNuEBarNCcoh,         
    kmNuEBarNCoth          
  },

};

/*
//
// -----------------------------------------------------------------------
//
// Info on flavours and CC/NC modes
//
// -----------------------------------------------------------------------
//

static const bool kIsCC[kns][knm]= {

  // > 1 muon-like ring
  {  1, //numu
     1,
     1,
     1,
     0,
     0,
#ifdef __USE_OSC_NUMU_MC_TEMPLATES__
     1, //oscnumu
     1,
     1,
     1,
#endif
     1, //numubar
     1,
     1,
     1,
     0,
     0,
#ifdef __USE_OSC_NUMUBAR_MC_TEMPLATES__
     1, //oscnumubar
     1,
     1,
     1,
#endif
     1, //nue
     1,
     1,
     1,
     0,
     0,
     1, //oscnue
     1,
     1,
     1,
     1, //nuebar
     1,
     1,
     1,
     0,
     0,
#ifdef __USE_OSC_NUEBAR_MC_TEMPLATES__
     1, //oscnuebar
     1,
     1,
     1,
#endif
#ifdef USE_MEC
     1, //mec
#else
     0,
#endif
#ifndef __USE_OSC_NUMU_MC_TEMPLATES__
     0,
     0,
     0,
     0,
#endif
#ifndef __USE_OSC_NUMUBAR_MC_TEMPLATES__
     0,
     0,
     0,
     0,
#endif
#ifndef __USE_OSC_NUEBAR_MC_TEMPLATES__
     0,
     0,
     0,
     0,
#endif
     0, //always undefined (12)
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0
  }
  ,
  // > 1 e-like ring
  {  1, //numu
     1,
     1,
     1,
     0,
     0,
     0,
#ifdef __USE_OSC_NUMU_MC_TEMPLATES__
     1, //oscnumu
     1,
     1,
     1,
#endif
     1, //numubar
     1,
     1,
     1,
     0,
     0,
     0,
#ifdef __USE_OSC_NUMUBAR_MC_TEMPLATES__
     1, //oscnumubar
     1,
     1,
     1,
#endif
     1, //nue
     1,
     1,
     1,
     0,
     0,
     0,
     1, //oscnue
     1,
     1,
     1,
     1, //nuebar
     1,
     1,
     1,
     0,
     0,
     0,
#ifdef __USE_OSC_NUEBAR_MC_TEMPLATES__
     1, //oscnuebar
     1,
     1,
     1,
#endif
#ifndef __USE_OSC_NUMU_MC_TEMPLATES__
      0, //undefined
      0, //undefined
      0, //undefined
      0, //undefined
#endif
#ifndef __USE_OSC_NUMUBAR_MC_TEMPLATES__
      0, //undefined
      0, //undefined
      0, //undefined
      0, //undefined
#endif
#ifndef __USE_OSC_NUEBAR_MC_TEMPLATES__
      0, //undefined
      0, //undefined
      0, //undefined
      0, //undefined
#endif
      0, //always undefined (9)
      0, //undefined
      0, //undefined
      0, //undefined
      0, //undefined
      0, //undefined
      0, //undefined
      0, //undefined
      0  //undefined
  }
  ,
  // > 1 muon-like ring (RHC)
  {  1, //numu
     1,
     1,
     1,
     0,
     0,
#ifdef __USE_OSC_NUMU_MC_TEMPLATES__
     1, //oscnumu
     1,
     1,
     1,
#endif
     1, //numubar
     1,
     1,
     1,
     0,
     0,
#ifdef __USE_OSC_NUMUBAR_MC_TEMPLATES__
     1, //oscnumubar
     1,
     1,
     1,
#endif
     1, //nue
     1,
     1,
     1,
     0,
     0,
     1, //oscnue
     1,
     1,
     1,
     1, //nuebar
     1,
     1,
     1,
     0,
     0,
#ifdef __USE_OSC_NUEBAR_MC_TEMPLATES__
     1, //oscnuebar
     1,
     1,
     1,
#endif
#ifdef USE_MEC
     1, //mec
#else
     0,
#endif
#ifndef __USE_OSC_NUMU_MC_TEMPLATES__
     0,
     0,
     0,
     0,
#endif
#ifndef __USE_OSC_NUMUBAR_MC_TEMPLATES__
     0,
     0,
     0,
     0,
#endif
#ifndef __USE_OSC_NUEBAR_MC_TEMPLATES__
     0,
     0,
     0,
     0,
#endif
     0, //always undefined (12)
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0
  }
  ,
  // > 1 e-like ring (RHC)
    {  1, //numu
        1,
        1,
        1,
        0,
        0,
        0,
#ifdef __USE_OSC_NUMU_MC_TEMPLATES__
        1, //oscnumu
        1,
        1,
        1,
#endif
        1, //numubar
        1,
        1,
        1,
        0,
        0,
        0,
#ifdef __USE_OSC_NUMUBAR_MC_TEMPLATES__
        1, //oscnumubar
        1,
        1,
        1,
#endif
        1, //nue
        1,
        1,
        1,
        0,
        0,
        0,
        1, //oscnue
        1,
        1,
        1,
        1, //nuebar
        1,
        1,
        1,
        0,
        0,
        0,
#ifdef __USE_OSC_NUEBAR_MC_TEMPLATES__
        1, //oscnuebar
        1,
        1,
        1,
#endif
#ifndef __USE_OSC_NUMU_MC_TEMPLATES__
        0, //undefined
        0, //undefined
        0, //undefined
        0, //undefined
#endif
#ifndef __USE_OSC_NUMUBAR_MC_TEMPLATES__
        0, //undefined
        0, //undefined
        0, //undefined
        0, //undefined
#endif
#ifndef __USE_OSC_NUEBAR_MC_TEMPLATES__
        0, //undefined
        0, //undefined
        0, //undefined
        0, //undefined
#endif
        0, //always undefined (9)
        0, //undefined
        0, //undefined
        0, //undefined
        0, //undefined
        0, //undefined
        0, //undefined
        0, //undefined
        0  //undefined
    }
};
*/

} // conv
} // t2kana
  
#endif 


//____________________________________________________________________________
/*!

\class     t2k::conv::NDSample_t

\brief     Enumeration of LBNx ND systematic constraint samples

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

#ifndef _t2kana_nd_sample_h_
#define _t2kana_nd_sample_h_

#include <string>

using std::string;

namespace t2kana {
namespace conv {

//
// Define samples
//
typedef enum ENDSample 
{
   ksNDUndefined = -1, 
   ksNDNuMuCC,          // numu CC inclusive
   ksNDNuMuCC1Trk,      // numu CC - 1 track
   ksNDNuMuCC2TrkQE,    // numu CC - 2 track QE-enhanched (mu + proton)
   ksNDNuMuCC1PiChg,    // numu CC - 2 track (mu + 1 pi+/-)
   ksNDNuMuCC1Pi0,      // numu CC - 2 track (mu + 1 pi0)
   ksNDNC,              // NC inclusive
   ksNDNC1pi0           // NC 1pi0
} 
NDSample_t; 

//
// Define list of samples used in present analysis
// Note: the order of samples in the array is important for other arrays
//
static const int kNNDSamples  = 2; 

static const NDSample_t kNDSample[kNNDSamples] =
{
   ksNDNuMuCC,          
   ksNDNuMuCC1Trk      
};

/*
static const NDSample_t kNDSample[kNNDSamples] =
{
   ksNDNuMuCC,          
   ksNDNuMuCC1Trk,      
   ksNDUndefined, // ksNDNuMuCC2TrkQE,    
   ksNDUndefined, // ksNDNuMuCC1PiChg,    
   ksNDUndefined, // ksNDNuMuCC1Pi0,      
   ksNDUndefined,
   ksNDUndefined
};
*/

} // conv

namespace util {

  string AsString(conv::NDSample_t s);

} // util

} // t2kana
  
#endif 


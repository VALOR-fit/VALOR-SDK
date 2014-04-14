//____________________________________________________________________________
/*!

\class     t2k::conv::NDSample_t

\brief     Enumeration of LBNx ND systematic constraint samples

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
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

static const int kNNDSamples  = 4; 

typedef enum ENDSample 
{
   ksNDUndefined = -1, 
   ksNDNuMuCCLike, // numu CC inclusive
   ksNDNuECCLike   // nue  CC inclusive
} 
NDSample_t; 

} // conv

namespace util {

  string AsString(conv::NDSample_t s);

} // util

} // t2kana
  
#endif 


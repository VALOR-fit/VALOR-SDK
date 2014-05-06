//____________________________________________________________________________
/*!

\file      NDSampleBinning

\brief     Binning information for samples included in the ND fit.

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

#ifndef _t2kana_nd_sample_binning_h_
#define _t2kana_nd_sample_binning_h_

#include "NDFit/NDSample.h"

namespace t2kana {
namespace conv   {

static const int kNMaxBinZones = 4;

// ...........................................................................
// Binning in (Ereco,yreco) space for QE-like processes
//

static const int kNBinZonesQELike = 4;

static const int kNErecoBinsQELike[kNBinZonesQELike] = {  28, 2, 1, 9 };
static const int kMaxNErecoBinsQELike = 28;

static const double kErecoBinEdgesQELike[kNBinZonesQELike][kMaxNErecoBinsQELike+1] = 
{
  // zone 1
  { 0.800,  1.000,  1.200,  1.400,  1.600,  1.800,  1.900,  2.000,  2.100,  2.200,  
    2.300,  2.400,  2.500,  2.600,  2.700,  2.800,  2.900,  3.000,  3.100,  3.200,  
    3.300,  3.400,  3.600,  3.800,  4.000,  4.200,  4.400,  4.600,  5.000
  },

  // zone 2
  { 0.000,  0.500,  0.800, -1,     -1,     -1,     -1,     -1,     -1,     -1, 
   -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,
   -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1
  },

  // zone 3
  { 0.800,  5.000, -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,
   -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,
   -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1
  },

  // zone 4
  { 5.000,  5.500,  6.000,  7.000,  8.000,  9.000, 10.000, 15.000, 20.000, 50.000
   -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,
   -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1
  }
};


static const int kNYrecoBinsQELike[kNBinZonesQELike] = {  15, 1, 4, 5 };
static const int kMaxNYrecoBinsQELike = 15;
static const double kYrecoBinEdgesQELike[kNBinZonesQELike][kMaxNYrecoBinsQELike+1] = 
{
  // zone 1
  { 0.000,  0.005,  0.010,  0.020,  0.030,  0.040,  0.050,  0.060,  0.070,  0.080,  
    0.090,  0.100,  0.120,  0.140,  0.160,  0.200
  },

  // zone 2
  { 0.000,  1.000, -1,     -1,     -1,     -1,     -1,     -1,     -1,     -1,
   -1,     -1,     -1,     -1,     -1,     -1
  },

  // zone 3
  { 0.200,  0.250,  0.300,  0.400,  1.000, -1,     -1,     -1,     -1,     -1,
   -1,     -1,     -1,     -1,     -1,     -1
  },

  // zone 4
  { 0.000,  0.010,  0.020,  0.040,  0.100,  1.000,  -1,     -1,     -1,     -1,
   -1,     -1,     -1,     -1,     -1,     -1
  }
};


// ...........................................................................
// Simple, unoptimized inning in (Ereco,yreco) space 
//

static const int kNErecoBinsSimple = 38;

static const double kErecoBinEdgesSimple[kNErecoBinsSimple+1] = 
{
    0.000,  0.400, 
    0.800,  1.000,  1.200,  1.400,  1.600,  1.800,  1.900,  2.000,  2.100,  2.200,  
    2.300,  2.400,  2.500,  2.600,  2.700,  2.800,  2.900,  3.000,  3.100,  3.200,  
    3.300,  3.400,  3.600,  3.800,  4.000,  4.200,  4.400,  4.600,  5.000,
    6.000,  7.000,  8.000,  9.000, 10.000, 15.000, 20.000, 50.000
};

static const int kNYrecoBinsSimple = 10;

static const double kYrecoBinEdgesSimple[kYErecoBinsSimple+1] = 
{
    0.000,  0.100,  0.200,  0.300,  0.400,  0.500,  0.600,  0.700,  0.800,  0.900,
    1.000
};



} // conv


namespace util   {

   int      NumErecoBins  (conv::NDSample_t sample, int zone);
   double * ErecoBinEdges (conv::NDSample_t sample, int zone);
   int      NumYrecoBins  (conv::NDSample_t sample, int zone);
   double * YrecoBinEdges (conv::NDSample_t sample, int zone);

} // util

} // t2kana
  
#endif 


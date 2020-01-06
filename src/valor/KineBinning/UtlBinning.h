/*!

\brief     Contains utility functions for integrating and plotting VALORArrays
           based on Mondrian and Edge2Edge binning.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick
          
\created   Feb 21, 2017

\copyright (c) 2010-2017, The VALOR group

*/

#ifndef valor_util_binning
#define valor_util_binning

#include <vector>

#include "TH1D.h"
#include "TH2D.h"

// valor
#include "valor/Conventions/KineSpace.h"
#include "valor/KineBinning/KBCubHcmbMondrian.h"
#include "valor/KineBinning/KBCubHcmbEdge2Edge.h"
#include "valor/Utilities/SingleSampleKineBinning.h"

namespace valor
{
namespace util
{
namespace binning
{

   /**
      Produce a 1D ROOT-compatible histogram of the specified kinematic variable
      from a VALORArray with a specified binning.

      \param v The VALORArray containing the spectrum to be plotted.
      \param b The binning used by the VALORArray.
      \param ks The kinematic space for which the histogram is to be produced.
      \param use_simple Whether to use the simple binning. Otherwise, use optimised.
      \return A 1D histogram of the specified kinematic variable.

   */
   TH1D* Make1DHistogram(const VALORArray& v,
         const SingleSampleKineBinning& b, const conv::KineSpace_t ks,
         bool use_simple = false, conv::ReacMode_t m = conv::kmUndefined);

   /**
      Produce a 2D ROOT-compatible, optimised histogram of specified kinematic
      variables from a VALORArray with a specified binning.

      Note that if the input VALORBinning is specified as following a Mondrian type binning scheme,
      the ROOT histogram produced by this function should be used only
      for plotting purposes, as mapping from optimised to ROOT-compatible binning
      is not guaranteed to conserve total event counts.

      \param v The VALORArray containing the spectrum to be plotted.
      \param b The binning used by the VALORArray.
      \param ks The kinematic variables for which the histogram is to be produced.
      \return A 2D histogram of the specified kinematic variables.

   */
   TH2D* Make2DHistogram(const VALORArray& v,
         const VALORBinning* b,
         const std::vector<conv::KineVar_t>& ks);

}
}
}

#endif


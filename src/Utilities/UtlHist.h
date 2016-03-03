//____________________________________________________________________________
/*!

\class     valor::utils

\brief     Utilities for making histograms

\author    Steve Dennis <stephen.dennis \at warwick.ac.uk>
           University of Warwick
          
\created   Jun 23, 2014

\copyright (c) 2010-2016, The VALOR group.

*/
//____________________________________________________________________________

#ifndef _valor_hist_utils_h_
#define _valor_hist_utils_h_

#include <TH1D.h>
#include <TH2D.h>
#include <TObjArray.h>

#include <FitCore/PhysParamList.h>
#include <FitCore/FitParamList.h>

#include "Utilities/VALORArray.h"

namespace valor {
namespace util {
  // Returns a 1D histogram representing the VALORArray. If array is not 1D, integrates out other dimensions.
  TH1D Make1DHistogramFromArray(const VALORArray & va, const double * bin_edges, int dimension = 0);
  
  // Returns a 1D histogram representing the VALORArray. If array is not 1D, integrates out other dimensions.
  TH2D Make2DHistogramFromArray(const VALORArray & va, double * const bin_edges_x, const double * bin_edges_y, int dimension_x=0, int dimension_y=1);
  
  TObjArray * MakeHistogramsOfParameterValues(int n_entries, const PhysParamList *const* pparms, int nbins=-1, const FitParamList * get_limits_from = 0);

} // util
} // valor
  
#endif



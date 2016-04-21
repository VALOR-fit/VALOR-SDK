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

#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TObjArray.h>

#include "valor/FitCore/PhysParamList.h"
#include "valor/FitCore/FitParamList.h"

#include "valor/VArray/VALORArray.h"

namespace valor {
namespace util {
  // Returns a 1D histogram representing the VALORArray. If array is not 1D, integrates out other dimensions.
  TH1D Make1DHistogramFromArray(const VALORArray & va, const double * bin_edges, int dimension = 0);
  
  // Returns a 1D histogram representing the VALORArray. If array is not 1D, integrates out other dimensions.
  TH2D Make2DHistogramFromArray(const VALORArray & va, double * const bin_edges_x, const double * bin_edges_y, int dimension_x=0, int dimension_y=1);
  
  // Turns an array of pointers to PhysParamLists into a TObjArray containing a histogram showing the distribution of each parameter in that list. Array owns hists.
  TObjArray * MakeHistogramsOfParameterValues(int n_entries, const PhysParamList * const * pparms, int nbins=-1, const FitParamList * get_limits_from = 0);
  
  // Use a TF1* to make a cloned version of a TH1* containing the values of that function (with correct normalisation) 
  TH1 * MakeFittedHistogramShowingFittedValues(const TH1* hist, const TF1* func, int npoints = 1000);

} // util
} // valor
  
#endif



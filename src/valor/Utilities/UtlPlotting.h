/*!

\class     valor::util::UtlPlotting

\brief     Contains utility functions for plotting histograms based on Mondrian
           and Edge2Edge binning.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick
          
\created   Mar 08, 2017

\copyright (c) 2010-2017, The VALOR group

*/

#ifndef valor_util_utl_plotting
#define valor_util_utl_plotting

#include <vector>

#include <TH1D.h>
#include <TH2D.h>

// valor


namespace valor
{
namespace util
{
namespace plotting
{
   /**
      Plot a 2D histogram.

      \param histogram The histogram to plot.
      \param title The plot title.
      \param xTitle The x-axis title.
      \param yTitle The y-axis title.
      \param filename The filename for plot output.
   */
   void Plot(TH2D* histogram, const std::string& title,
         const std::string& xTitle, const std::string yTitle,
         const std::string& filename);

   /**
      Plot a 1D histogram.

      \param histogram The histogram to plot.
      \param title The plot title.
      \param xTitle The x-axis title.
      \param yTitle The y-axis title.
      \param filename The filename for plot output.
   */
   void Plot(TH1D* histogram, const std::string& title,
         const std::string& xTitle, const std::string yTitle,
         const std::string& filename);
}
}
}

#endif


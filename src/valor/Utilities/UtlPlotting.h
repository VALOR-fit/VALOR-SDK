/*!

\class     valor::util::UtlPlotting

\brief     Contains utility functions for plotting histograms based on Mondrian
           and Edge2Edge binning.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick
          
\created   Mar 08, 2017

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

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

   /***
    * Makes a custom palette for 2D colour plots
    *
    * @param palette_name
    * @return true = palette set successfully
    */
   bool SetupCustomPalette(std::string palette_name = "valor_inverted_dark_body_radiator");
}
}
}

#endif


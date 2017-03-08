/*!

\class     valor::util::UtlPlotting

\brief     Contains utility functions for plotting histograms based on Mondrian
           and Edge2Edge binning.

\author    Andy Chappell <A.Chappell \t warwick.ac.uk
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

class UtlPlotting
{
public:
   /**
      Plot a 2D histogram.

      \param histogram The histogram to plot.
      \param title The plot title.
      \param xTitle The x-axis title.
      \param yTitle The y-axis title.
      \param filename The filename for plot output.
   */
   static void Plot(TH2D* histogram, const std::string& title,
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
   static void Plot(TH1D* histogram, const std::string& title,
         const std::string& xTitle, const std::string yTitle,
         const std::string& filename);

private:
   /**
      Plot an arbitrary histogram.

      The interface for this method takes a pointer to a TH1 object, which is
      the base class for all histograms, irrespective of dimensionality. The
      functions required for this plot routine make use only of the common
      interface of all of the histogram subclasses and so supports plotting of
      arbitrary histograms.

      \param histogram The histogram to plot.
      \param title The plot title.
      \param xTitle The x-axis title.
      \param yTitle The y-axis title.
      \param filename The filename for plot output.
      \param options Drawing options to pass to TH1::Draw.
   */
   static void GenericPlot(TH1* histogram, const std::string& title,
         const std::string& xTitle, const std::string yTitle,
         const std::string& filename, const std::string& options);

private:
   UtlPlotting(){ }
};

}
}

#endif


#include "valor/Utilities/UtlPlotting.h"

// Root
#include <TCanvas.h>

namespace valor
{
namespace util
{
namespace plotting
{

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
void GenericPlot(TH1* histogram, const std::string& title,
      const std::string& xTitle, const std::string yTitle,
      const std::string& filename, const std::string& options)
{
   TCanvas c("Canvas", "", 800, 600);
   c.cd();

   histogram->SetTitle(title.c_str());
   histogram->SetStats(false);

   TAxis* x = histogram->GetXaxis();
   x->SetTitle(xTitle.c_str());
   x->SetTitleOffset(1.2);
   TAxis* y = histogram->GetYaxis();
   y->SetTitle(yTitle.c_str());
   y->SetTitleOffset(1.2);

   histogram->Draw(options.c_str());
   c.Print(filename.c_str());
}

void Plot(TH2D* histogram, const std::string& title,
      const std::string& xTitle, const std::string yTitle,
      const std::string& filename)
{
   GenericPlot(histogram, title, xTitle, yTitle, filename, "colz");
}

void Plot(TH1D* histogram, const std::string& title,
      const std::string& xTitle, const std::string yTitle,
      const std::string& filename)
{
   GenericPlot(histogram, title, xTitle, yTitle, filename, "");
}

}
}
}

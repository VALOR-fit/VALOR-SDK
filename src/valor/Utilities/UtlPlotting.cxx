#include "valor/Utilities/UtlPlotting.h"

// Root
#include <TCanvas.h>
#include <TStyle.h>
#include <TColor.h>

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

bool SetupCustomPalette(std::string palette_name)
{
   if(palette_name == "valor_inverted_dark_body_radiator") {
      // This is a modified version of ROOT's inverted dark body radiator palette, with:
      //  low = white (instead of light yellow)
      //  high = dark red (instead of black)
      // This works well with all the different colourblind modes from
      // https://www.color-blindness.com/coblis-color-blindness-simulator/
      Double_t red  [8] = {1., 234./255., 237./255., 230./255., 212./255., 156./255., 99./255., 45./255.};
      Double_t green[8] = {1., 238./255., 238./255., 168./255., 101./255.,  45./255.,  0./255.,  0./255.};
      Double_t blue [8] = {1.,  95./255.,  11./255.,   8./255.,   9./255.,   3./255.,  1./255.,  1./255.};
      Double_t stops[8] = {0.,     0.143,     0.286,     0.429,     0.572,     0.715,    0.858,       1.};
      const Int_t NCont = 500;
      int status = TColor::CreateGradientColorTable(8, stops, red, green, blue, NCont);
      gStyle->SetNumberContours(NCont);
      return status != -1;
   }
   else {
      return false;
   }
}

}
}
}

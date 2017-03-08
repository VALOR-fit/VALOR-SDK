#include "valor/Utilities/UtlPlotting.h"

// Root
#include <TCanvas.h>

namespace valor
{
namespace util
{

void UtlPlotting::Plot(TH2D* histogram, const std::string& title,
      const std::string& xTitle, const std::string yTitle,
      const std::string& filename)
{
   GenericPlot(histogram, title, xTitle, yTitle, filename, "colz");
}

void UtlPlotting::Plot(TH1D* histogram, const std::string& title,
      const std::string& xTitle, const std::string yTitle,
      const std::string& filename)
{
   GenericPlot(histogram, title, xTitle, yTitle, filename, "");
}

void UtlPlotting::GenericPlot(TH1* histogram, const std::string& title,
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

}
}

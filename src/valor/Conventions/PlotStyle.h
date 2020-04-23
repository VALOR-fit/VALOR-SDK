//____________________________________________________________________________
/*!

\class     valor::conv::PlotStyle

\brief     Plot style for a ROOT figure (line style, fill style, marker style)

\author    Steve Dennis <s.r.dennis \at liverpool.ac.uk>
           University of Liverpool
          
\created   Dec 2019

\copyright (c) 2010-2010, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_fmwk_plot_style_h_
#define _valor_fmwk_plot_style_h_

#include <TStyle.h>

namespace valor {

  class PlotStyle {
  public:
    static const short kNoApply = -1;
    PlotStyle();
    
    void UpdateStyleOf(TObject * plot) const;
    void Reset();
    
    // constructor in the form of the old SampleReacMode arrays.
    static PlotStyle PlotStyleSimple(Color_t line_color, Color_t fill_color, Style_t fill_style);
    
    // And setters/getters
    void SetLineWidth   (Width_t);
    void SetLineColor   (Color_t);
    void SetLineStyle   (Style_t);
    void SetFillColor   (Color_t);
    void SetFillStyle   (Style_t);
    void SetMarkerColor (Color_t);
    void SetMarkerStyle (Style_t);
    void SetMarkerSize  (Size_t );
    
    Width_t GetLineWidth   () const;
    Color_t GetLineColor   () const;
    Style_t GetLineStyle   () const;
    Color_t GetFillColor   () const;
    Style_t GetFillStyle   () const;
    Color_t GetMarkerColor () const;
    Style_t GetMarkerStyle () const;
    Size_t  GetMarkerSize  () const;
    
  private:
    Width_t fLineWidth;
    Color_t fLineColor;
    Style_t fLineStyle;
    Color_t fFillColor;
    Style_t fFillStyle;
    Color_t fMarkerColor;
    Style_t fMarkerStyle;
    Size_t  fMarkerSize;
  } ;// PlotStyle
//
// Associated utilities
//
namespace util {
  
} // util

} // valor
  
#endif 


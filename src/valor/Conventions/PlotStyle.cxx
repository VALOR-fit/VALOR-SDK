//________________________________________________________________________________________
/*

   Copyright (c) 2010-2019, The VALOR Fitting Group, All Rights Reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <algorithm>

#include "TAttMarker.h"
#include "TAttLine.h"
#include "TAttFill.h"

//fmwk
#include "valor/Messenger/Messenger.h"
#include "valor/Conventions/PlotStyle.h"

using namespace valor;

PlotStyle::PlotStyle()
{
  this->Reset();
}
//________________________________________________________________________________________
void PlotStyle::Reset()
{
  fLineWidth   = kNoApply;
  fLineColor   = kNoApply;
  fLineStyle   = kNoApply;
  fFillColor   = kNoApply;
  fFillStyle   = kNoApply;
  fMarkerColor = kNoApply;
  fMarkerStyle = kNoApply;
  fMarkerSize  = kNoApply;
}
//________________________________________________________________________________________
void PlotStyle::UpdateStyleOf(TObject * plot) const
{
  TAttLine   * al = dynamic_cast<TAttLine   * >(plot);
  TAttFill   * af = dynamic_cast<TAttFill   * >(plot);
  TAttMarker * am = dynamic_cast<TAttMarker * >(plot);
  if (al) {
    if ( fLineColor != kNoApply ) { al->SetLineColor(fLineColor); }
    if ( fLineStyle != kNoApply ) { al->SetLineStyle(fLineStyle); }
    if ( fLineWidth != kNoApply ) { al->SetLineWidth(fLineWidth); }
  }
  if (af) {
    if ( fFillColor != kNoApply ) { af->SetFillColor(fFillColor); }
    if ( fFillStyle != kNoApply ) { af->SetFillStyle(fFillStyle); }
  }
  if (am) {
    if ( fMarkerColor != kNoApply ) { am->SetMarkerColor(fMarkerColor); }
    if ( fMarkerStyle != kNoApply ) { am->SetMarkerStyle(fMarkerStyle); }
    if ( fMarkerSize >= 0) {am->SetMarkerSize (fMarkerSize);}
  }
  if (!al && !af && !am) {
    LOG("PlotStyle",pWARN)<<"Did not update any styles";
  }
}
//________________________________________________________________________________________
void PlotStyle::SetLineWidth   (Width_t x) { fLineWidth   = x ; }
void PlotStyle::SetLineColor   (Color_t x) { fLineColor   = x ; }
void PlotStyle::SetLineStyle   (Style_t x) { fLineStyle   = x ; }
void PlotStyle::SetFillColor   (Color_t x) { fFillColor   = x ; }
void PlotStyle::SetFillStyle   (Style_t x) { fFillStyle   = x ; }
void PlotStyle::SetMarkerColor (Color_t x) { fMarkerColor = x ; }
void PlotStyle::SetMarkerStyle (Style_t x) { fMarkerStyle = x ; }
void PlotStyle::SetMarkerSize  (Size_t  x) { fMarkerSize  = x ; }
//________________________________________________________________________________________
Width_t PlotStyle::GetLineWidth   () const { return fLineWidth   ; }
Color_t PlotStyle::GetLineColor   () const { return fLineColor   ; }
Style_t PlotStyle::GetLineStyle   () const { return fLineStyle   ; }
Color_t PlotStyle::GetFillColor   () const { return fFillColor   ; }
Style_t PlotStyle::GetFillStyle   () const { return fFillStyle   ; }
Color_t PlotStyle::GetMarkerColor () const { return fMarkerColor ; }
Style_t PlotStyle::GetMarkerStyle () const { return fMarkerStyle ; }
Size_t  PlotStyle::GetMarkerSize  () const { return fMarkerSize  ; }
//________________________________________________________________________________________
PlotStyle PlotStyle::PlotStyleSimple(Color_t line_color, Color_t fill_color, Style_t fill_style)
{
  PlotStyle ps;
  ps.SetLineColor(line_color);
  ps.SetFillColor(fill_color);
  ps.SetFillStyle(fill_style);
  return ps;
}
//________________________________________________________________________________________

//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <algorithm>
#include <vector>

#include "valor/Utilities/Interpolator.h"

using namespace valor;

#ifdef __VALOR_FMWK_GSL_ENABLED__
//________________________________________________________________________________________
valor::util::Interpolator::Interpolator(const TSpline * spl, ROOT::Math::Interpolation::Type type)
  : ROOT::Math::Interpolator(0,type),fXmin(0),fXmax(0)
{
  if (spl) {
    int n_knots = spl->GetNp();
    std::vector<double> x(n_knots);
    std::vector<double> y(n_knots);
    for (int i = 0 ; i < n_knots; i++) {
      spl->GetKnot(i,x[i],y[i]);
    }
    this->SetData(n_knots,x.data(),y.data());
    fXmin = *std::min_element(x.begin(),x.end());
    fXmax = *std::max_element(x.begin(),x.end());
  }
}
//________________________________________________________________________________________
valor::util::Interpolator::Interpolator(const TSpline & spl, ROOT::Math::Interpolation::Type type)
  : ROOT::Math::Interpolator(0,type),fXmin(0),fXmax(0)
{
  int n_knots = spl.GetNp();
  std::vector<double> x(n_knots);
  std::vector<double> y(n_knots);
  for (int i = 0 ; i < n_knots; i++) {
    spl.GetKnot(i,x[i],y[i]);
  }
  this->SetData(n_knots,x.data(),y.data());
  fXmin = *std::min_element(x.begin(),x.end());
  fXmax = *std::max_element(x.begin(),x.end());
}
//________________________________________________________________________________________
double valor::util::Interpolator::EvalWithLimits(double x) const
{
  x = std::max(x,fXmin);
  x = std::min(x,fXmax);
  return this->Eval(x);
}
//________________________________________________________________________________________
double valor::util::Interpolator::Xmin() const
{
  return fXmin;
}
//________________________________________________________________________________________
double valor::util::Interpolator::Xmax() const
{
  return fXmax;
}
//________________________________________________________________________________________
TGraph valor::util::Interpolator::AsGraph(unsigned int npoints) const
{
  std::vector<double> x(npoints),y(npoints);
  double min = fXmin;
  double max = fXmax;
  double step = (max-min)/(npoints-1);
  for (unsigned int i = 0 ; i < npoints ; i++) {
    x[i] = min+(step*i);
    y[i] = this->Eval(x[i]);
  }
  return TGraph(npoints,x.data(),y.data());
}
//________________________________________________________________________________________
#endif // __VALOR_FMWK_GSL_ENABLED__

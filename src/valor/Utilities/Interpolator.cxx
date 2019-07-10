//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <algorithm>
#include <vector>

#include "valor/Utilities/Interpolator.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Messenger/Messenger.h"

using namespace valor;

#ifdef __VALOR_FMWK_GSL_ENABLED__
//________________________________________________________________________________________
valor::util::Interpolator::Interpolator(const TSpline * spl, bool use_gsl, ROOT::Math::Interpolation::Type type)
  : fGSLSpline(nullptr),fROOTSpline(nullptr),fXmin(0),fXmax(0),fName(""),fTitle(""),fUseLimits(true)
{
  if(use_gsl){
    fGSLSpline = new ROOT::Math::Interpolator(0,type);
    if (spl) {
      int n_knots = spl->GetNp();
      std::vector<double> x(n_knots);
      std::vector<double> y(n_knots);
      for (int i = 0 ; i < n_knots; i++) {
        spl->GetKnot(i,x[i],y[i]);
      }
      fGSLSpline->SetData(n_knots,x.data(),y.data());
      fXmin      = *std::min_element(x.begin(),x.end());
      fXmax      = *std::max_element(x.begin(),x.end());
      fName      = spl->GetName();
      fTitle     = spl->GetTitle();
      fUseLimits = true;   // GSL splines can't be evaluated outside their limits
    }
  }
  else {
    fROOTSpline = new TSpline3(*((TSpline3*)spl));
    if (spl) {
      fXmin       = spl->GetXmin();
      fXmax       = spl->GetXmax();
      fName       = spl->GetName();
      fTitle      = spl->GetTitle();
    }
  }
}
//________________________________________________________________________________________
valor::util::Interpolator::Interpolator(const TSpline & spl, bool use_gsl, ROOT::Math::Interpolation::Type type)
  : fGSLSpline(nullptr),fROOTSpline(nullptr),fXmin(0),fXmax(0),fName(""),fTitle(""),fUseLimits(true)
{
  if(use_gsl){
    fGSLSpline = new ROOT::Math::Interpolator(0,type);
    int n_knots = spl.GetNp();
    std::vector<double> x(n_knots);
    std::vector<double> y(n_knots);
    for (int i = 0 ; i < n_knots; i++) {
      spl.GetKnot(i,x[i],y[i]);
    }
    fGSLSpline->SetData(n_knots,x.data(),y.data());
    fXmin      = *std::min_element(x.begin(),x.end());
    fXmax      = *std::max_element(x.begin(),x.end());
    fName      = spl.GetName();
    fTitle     = spl.GetTitle();
  }
  else {
    fROOTSpline = new TSpline3(static_cast<const TSpline3 &>(spl));
    fXmin       = spl.GetXmin();
    fXmax       = spl.GetXmax();
    fName       = spl.GetName();
    fTitle      = spl.GetTitle();
  }
}
//________________________________________________________________________________________
double valor::util::Interpolator::Eval(double x) const
{
  CheckSplineType();
  if(fUseLimits){
    x = std::max(x,fXmin);
    x = std::min(x,fXmax);
  }
  else if(x < fXmin || x > fXmax) {
    std::cerr << "Evaluating spline [" << fName << "] at point " << x << " outside of its limits of ["
              << fXmin << ", " << fXmax << "]" << std::endl;
  }
  if(fGSLSpline) { return fGSLSpline ->Eval(x); }
  else           { return fROOTSpline->Eval(x); }
}
//________________________________________________________________________________________
void valor::util::Interpolator::CheckSplineType(void) const
{
  if(fGSLSpline && fROOTSpline) {
    LOG("Messenger",pFATAL) << "Both GSL and ROOT splines are defined. This shouldn't happen!";
    util::Die(-1);
  }
  if(!fGSLSpline && !fROOTSpline) {
    LOG("Messenger",pFATAL) << "Neither GSL or ROOT splines are defined. This shouldn't happen!";
    util::Die(-1);
  }
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
void valor::util::Interpolator::SetUseLimits(bool u)
{
  if(!u && fGSLSpline) {
    LOG("Messenger",pWARN)
      << "GSL splines can't be evaluated outside of their limits, so ignoring the call to Interpolator::SetUseLimits(false)";
  }
  else {
#ifdef __LET_ME_DO_STUPID_STUFF_WITH_MY_SPLINES__
    fUseLimits = u;
#else
    fUseLimits = true;
    if(!u) {
      LOG("Messenger",pWARN)
        << "If you want to evaluate splines outside of their limits, configure the SDK with the flag --enable-spline-stupidity";
    }
#endif
  }
}
//________________________________________________________________________________________
valor::util::Interpolator::~Interpolator()
{
  if(fGSLSpline ) { delete fGSLSpline;  }
  if(fROOTSpline) { delete fROOTSpline; }
}
//________________________________________________________________________________________
#endif // __VALOR_FMWK_GSL_ENABLED__

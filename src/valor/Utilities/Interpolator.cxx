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
valor::util::Interpolator::Interpolator(const TSpline * spl, valor::util::Interpolator::Type type)
  : fGSLSpline(nullptr),fROOTSpline(nullptr),fXmin(0),fXmax(0),fName(""),fTitle(""),fUseLimits(true)
{
  assert(spl);
  bool use_gsl = type < Type::kTSpline3;
  if(use_gsl){
    int type_int = static_cast<int>(type);
    ROOT::Math::Interpolation::Type type_root = static_cast<ROOT::Math::Interpolation::Type>(type_int);
    fGSLSpline = new ROOT::Math::Interpolator(0,type_root);

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
  else {
    const int n_knots = spl->GetNp();
    std::vector<double> x(n_knots),y(n_knots);
    for (int ik = 0 ; ik < n_knots; ik++) {
      spl->GetKnot(ik,x[ik],y[ik]);
    }
    if (type == Type::kTSpline3) {
      fROOTSpline = new TSpline3(spl->GetTitle(),x.data(),y.data(),n_knots);
    }
    else if (type == Type::kTSpline5) {
      fROOTSpline = new TSpline5(spl->GetTitle(),x.data(),y.data(),n_knots);
    }
    else {
      std::cerr<<"Unknown spline type: "<<(int)type<<std::endl;
      util::Die(-1); return;
    }
    fXmin       = spl->GetXmin();
    fXmax       = spl->GetXmax();
    fName       = spl->GetName();
  }
}
valor::util::Interpolator::Interpolator(const TSpline & spl, valor::util::Interpolator::Type type)
 : Interpolator(&spl,type) {}
//________________________________________________________________________________________
double valor::util::Interpolator::Eval(double x) const
{
  CheckSplineType();
  if(fUseLimits){
    x = std::max(x,fXmin);
    x = std::min(x,fXmax);
  }
  else if(x < fXmin || x > fXmax) {
    LOG("Interpolator",pWARN)  << "Evaluating spline [" << fName << "] at point " << x << " outside of its limits of ["
                               << fXmin << ", " << fXmax << "]";
  }
  if(fGSLSpline) { return fGSLSpline ->Eval(x); }
  else           { return fROOTSpline->Eval(x); }
}
//________________________________________________________________________________________
void valor::util::Interpolator::CheckSplineType(void) const
{
  if(fGSLSpline && fROOTSpline) {
    LOG("Interpolator",pFATAL) << "Both GSL and ROOT splines are defined. This shouldn't happen!";
    util::Die(-1);
  }
  if(!fGSLSpline && !fROOTSpline) {
    LOG("Interpolator",pFATAL) << "Neither GSL or ROOT splines are defined. This shouldn't happen!";
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
    LOG("Interpolator",pWARN)
      << "GSL splines can't be evaluated outside of their limits, so ignoring the call to Interpolator::SetUseLimits(false)";
  }
  else {
#ifdef __LET_ME_DO_STUPID_STUFF_WITH_MY_SPLINES__
    fUseLimits = u;
#else
    fUseLimits = true;
    if(!u) {
      LOG("Interpolator",pWARN)
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
std::string valor::util::AsString(Interpolator::Type t, Option_t * /*opt*/)
{
  if     (t == Interpolator::Type::kLINEAR)           { return "linear";         }
  else if(t == Interpolator::Type::kPOLYNOMIAL)       { return "polynomial";     }
  else if(t == Interpolator::Type::kCSPLINE)          { return "cubic";          }
  else if(t == Interpolator::Type::kCSPLINE_PERIODIC) { return "cubic_periodic"; }
  else if(t == Interpolator::Type::kAKIMA)            { return "akima";          }
  else if(t == Interpolator::Type::kAKIMA_PERIODIC)   { return "akima_periodic"; }
  else if(t == Interpolator::Type::kTSpline3)         { return "tspline3";       }
  else if(t == Interpolator::Type::kTSpline5)         { return "tspline5";       }
  else                                                { return "--";             }
}
//________________________________________________________________________________________
std::string valor::util::InterpolatorTypeAsString(Interpolator::Type t, Option_t * opt) {return util::AsString(t,opt);}
//________________________________________________________________________________________
TGraph valor::util::Interpolator::AsTGraph(int npoints) const
{
  std::vector<double> xarr;
  std::vector<double> yarr;
  for (int i = 0 ; i < npoints ; i++) {
    double x = i*(fXmax-fXmin)/npoints +fXmin;
    double y = this->Eval(x);
    xarr.push_back(x);
    yarr.push_back(y);
  }
  return TGraph(npoints,xarr.data(),yarr.data());
}
//________________________________________________________________________________________
#endif // __VALOR_FMWK_GSL_ENABLED__

//____________________________________________________________________________
/*!

\class     valor::util::Interpolator

\brief     Basically the same as ROOT::Math::Interpolator but stores its own X limits, name and title.

\author    Steve Dennis <s.r.dennis \at liverpool.ac.uk
           University of Liverpool
          
\created   August 2016

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef valor_interpolator_h
#define valor_interpolator_h

#include "TSpline.h"
#include "TGraph.h"
#include "valor/Conventions/Build.h"

#ifdef __VALOR_FMWK_GSL_ENABLED__
#include "Math/Interpolator.h"
#endif

namespace valor {
  
namespace util {
#ifdef __VALOR_FMWK_GSL_ENABLED__

class Interpolator {
  public:

    enum class Type {
      // same as ROOT::Math::Interpolation::Type
      kLINEAR,
      kPOLYNOMIAL,
      kCSPLINE,
      kCSPLINE_PERIODIC,
      kAKIMA,
      kAKIMA_PERIODIC,
      // and ROOT options
      kTSpline3,
      kTSpline5
    };

    Interpolator(const TSpline * spl = 0, valor::util::Interpolator::Type type = valor::util::Interpolator::Type::kCSPLINE);
    Interpolator(const TSpline & spl    , valor::util::Interpolator::Type type = valor::util::Interpolator::Type::kCSPLINE);

    ~Interpolator();

    double Xmin() const;
    double Xmax() const;
    double Eval(double x) const;

    std::string GetName (void) const { return fName ; };
    std::string GetTitle(void) const { return fTitle; };

    void SetName (std::string s) { fName  = s; }
    void SetTitle(std::string s) { fTitle = s; }
    void SetUseLimits(bool u);
    
    TGraph AsTGraph(int n_points = 40) const;

  private:

    void CheckSplineType(void) const;

    ROOT::Math::Interpolator* fGSLSpline;
    TSpline* fROOTSpline;  // For when we don't want to use GSL splines

    double fXmin;
    double fXmax;
    std::string fName;
    std::string fTitle;
    bool fUseLimits;
};

  std::string        AsString                  (Interpolator::Type t, Option_t * opt=0);
  std::string        InterpolatorTypeAsString  (Interpolator::Type t, Option_t * opt=0); // Alias for above

#else
class Interpolator : public TObject {};
#endif //__VALOR_FMWK_GSL_ENABLED__
} // util

} // valor
  
#endif // valor_interpolator_h


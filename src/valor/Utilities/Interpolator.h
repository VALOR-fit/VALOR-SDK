//____________________________________________________________________________
/*!

\class     valor::util::Interpolator

\brief     Basically the same as ROOT::Math::Interpolator but stores its own X limits.

\author    Steve Dennis <s.r.dennis \at liverpool.ac.uk
           University of Liverpool
          
\created   August 2016

\copyright (c) 2010-2016, The VALOR group

*/
//____________________________________________________________________________

#ifndef valor_interpolator_h
#define valor_interpolator_h

#include "TSpline.h"
#include "valor/Conventions/Build.h"

#ifdef __VALOR_FMWK_GSL_ENABLED__
#include "Math/Interpolator.h"
#endif

namespace valor {
  
namespace util {
#ifdef __VALOR_FMWK_GSL_ENABLED__

class Interpolator : public ROOT::Math::Interpolator {
  public:
    Interpolator(const TSpline * spl = 0, ROOT::Math::Interpolation::Type type = ROOT::Math::Interpolation::kCSPLINE);
    Interpolator(const TSpline & spl, ROOT::Math::Interpolation::Type type = ROOT::Math::Interpolation::kCSPLINE);
    double Xmin() const;
    double Xmax() const;
    double EvalWithLimits(double x) const;
    
  private:
    double fXmin;
    double fXmax;
};
#else
class Interpolator : public TObject {};
#endif //__VALOR_FMWK_GSL_ENABLED__
} // util

} // valor
  
#endif // valor_interpolator_h


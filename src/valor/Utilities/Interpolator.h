//____________________________________________________________________________
/*!

\class     valor::util::Interpolator

\brief     Basically the same as ROOT::Math::Interpolator but stores its own X limits, name and title.

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

class Interpolator {
  public:
    Interpolator(const TSpline * spl = 0, bool use_gsl = true, ROOT::Math::Interpolation::Type type = ROOT::Math::Interpolation::kCSPLINE);
    Interpolator(const TSpline & spl    , bool use_gsl = true, ROOT::Math::Interpolation::Type type = ROOT::Math::Interpolation::kCSPLINE);

    ~Interpolator();

    double Xmin() const;
    double Xmax() const;
    double Eval(double x) const;

    std::string GetName (void) const { return fName ; };
    std::string GetTitle(void) const { return fTitle; };

    void SetName (std::string s) { fName  = s; }
    void SetTitle(std::string s) { fTitle = s; }
    void SetUseLimits(bool u);

  private:

    void CheckSplineType(void) const;
    
    ROOT::Math::Interpolator* fGSLSpline;
    TSpline3* fROOTSpline;  // For when we don't want to use GSL splines
    
    double fXmin;
    double fXmax;
    std::string fName;
    std::string fTitle;
    bool fUseLimits;
};
#else
class Interpolator : public TObject {};
#endif //__VALOR_FMWK_GSL_ENABLED__
} // util

} // valor
  
#endif // valor_interpolator_h


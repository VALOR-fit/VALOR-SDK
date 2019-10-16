//____________________________________________________________________________
/*!

\class     valor::RFL

\brief     Pre-computed response function library for non-linear parameters
           of the VALOR analysis.

\author    Steve Dennis <s.r.dennis \at University of Liverpool>
           University of Liverpool
           
           Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   April 7, 2016

\copyright (c) 2010-2016, The VALOR group.

*/
//____________________________________________________________________________

#ifndef __valor_response_function_library__
#define __valor_response_function_library__

#include <iostream>
#include <utility>

#include "TSpline.h"

// FMWK
#include "valor/Conventions/Constants.h"
#include "valor/Conventions/PhysParamId.h"
#include "valor/Utilities/Interpolator.h"
#include "valor/Utilities/KinematicPoint.h"
#include "valor/Utilities/KinematicBin.h"
#include "valor/Conventions/BeamConf.h"
#include "valor/Conventions/ReacMode.h"
#include "valor/Conventions/Sample.h"
#include "valor/Conventions/SplineEvalType.h"

namespace valor {
class SingleSampleKineBinning;
  
namespace util {

class RFL {

public:

  RFL();
  RFL(const SingleSampleKineBinning * binning, int n_params, const conv::PhysParamId_t * param_ids);
  RFL(const SingleSampleKineBinning * binning, const std::vector<conv::PhysParamId_t> & params);
 ~RFL();

  // Define a dummy spline pointer to an object we know, so we can use its address as a special argument.
  static const valor::util::Interpolator * kNoSplineRequired;

  // And a flat spline object
  static const valor::util::Interpolator * kFlatSpline; //A flat spline that we'll use if we're missing RFs in testing versions
  
  void Print(std::ostream & stream) const;
  friend std::ostream & operator << (std::ostream & stream, const RFL & rfl);
  
  const valor::util::Interpolator * GetSpline(
    conv::ReacMode_t m, conv::PhysParamId_t p,
    const int& itrue, const int& ireco ) const;
    
  int Param2Idx(conv::PhysParamId_t p) const;
                      
  
  // Pattern for spline names.
  void SetSplineNamePattern(std::string pattern);
  
  // Get the name of the spline we're looking for
  TString BuildSplineName(conv::ReacMode_t m, conv::PhysParamId_t p,
                          const int & itrue, const int & ireco) const;
                          
  // Read splines from config file
  bool Read (string file, Option_t * opt = 0);
  
  // Get the bin corresponding to a point in kinematical space, returns pair<itrue,ireco>
  std::pair<int,int> GetBin(const util::KinematicPoint & kp, conv::ReacMode_t m = conv::kmUndefined) const;

  // Get the name of the method we'll use when applying weights. See GenericSingleSampleFitModel::ApplyPhysics for usage
  const conv::SplEvalType_t & GetSplineEvalType(void) const { return fSplineWghtMethod; }

  // When spline weights come out negative, force them to this value instead
  const double & GetForceNegWghtTo(void) const { return fForceNegWghtTo; }
  
private:

  const SingleSampleKineBinning * fBinning;

  // Actual response function block
  // Each entry be a big block, with dimensions [conv::kNReacModes][NFitParams][NBinsTrue][NBinsObs]
  const valor::util::Interpolator ** fSplines;
  UInt_t    fSplineBlockSize;
  
  // The spline indices
  inline int SplineIndex(const int& im, const int& ip, const int& itrue, const int & ireco) const
  {
    return (ireco + itrue * fNBinsReco  + 
            ip * fNBinsReco * fNBinsTrue  +
            im * fNBinsReco * fNBinsTrue *fNParams);
  }
  
  // The beam and sample (used in spline names)
  conv::BeamConf_t fBeam;
  conv::Sample_t   fSample;
  conv::Detector_t fDetector;
  
  // Details of binning
  std::vector<conv::ReacMode_t> fModes;   //[fNModes]
  int                           fNParams; //
  conv::PhysParamId_t *         fParams;  //[fParams]
  
  // These numbers store the largest number of bins for any mode
  unsigned int fNBinsTrue;    //
  unsigned int fNBinsReco;    //
  
  // Name for the splines (used in filename)
  TString fSplineNamePattern;

  // The method we'll use when applying weights. See GenericSingleSampleFitModel::ApplyPhysics for usage
  conv::SplEvalType_t fSplineWghtMethod;

  // If this is true, allow splines to go negative, but when this happens the weights will get set to 0
  bool fAllowNegativeSplines;

  // The type of GSL spline to use. Options are: linear, polynomial, cubic, cubic_periodic, akima, or akima_periodic.
  // Defaults to Akima.
  valor::util::Interpolator::Type fSplineType;
  
  // Array storing parameters mapped to indice within this object
  int fParam2Idx[conv::knppmax];

  // When spline weights come out negative, force them to this value instead
  double fForceNegWghtTo;
  
  // Check spline is positive
  static bool CheckForNegativeValues(const valor::util::Interpolator * spl);

  // The input spline doesn't necessarily have the knots in the right order, so sort them
  static void SortSpline(TSpline3 * spl, bool sort_spline);
};

std::ostream & operator << (std::ostream & stream, const RFL & rfl);

}      // util namespace
}      // valor namespace

#endif 

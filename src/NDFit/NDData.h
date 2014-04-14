//____________________________________________________________________________
/*!

\class     t2kana::NDData

\brief     LBNE ND Data class. Derives from FitDataABC

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           George Christodoulou <georgios \at hep.ph.liv.ac.uk>
           University of Liverpool

           Nick Grant <n.grant \at lancaster.ac.uk>
           University of Lancaster

\created   March 27, 2014

\copyright (c) 2010-2014, The VALOR group

*/
//____________________________________________________________________________

#ifndef _t2kana_nd_data_h_
#define _t2kana_nd_data_h_

#include <string>

#include "FitCore/FitDataABC.h"

class TObjArray;
class TH1D;

namespace t2kana {

class NDData: public FitDataABC
{
public:
  NDData();
  virtual ~NDData();

  //
  // Implement the FitDataABC interface
  // See FitDataABC for method decription
  //

  string Name           (void);
  bool   Read           (string file, Option_t * opt = 0);
  bool   Generate       (FitModelABC * pred, const FitParamList & fp,   bool fluctuate=true);
  bool   Generate       (FitModelABC * pred, const PhysHypothesis & ph, bool fluctuate=true);
  bool   Generate       (FitModelABC * pred, bool fluctuate=true);
  bool   Write          (TObjArray * data_collection, const char * tag = "data");
  bool   Write          (const char * filename, const char * tag = "data", const char * option_file="update");
  int    NumOfBins      (void);
  double Statistic      (FitModelABC * pred, const FitParamList & fp,   Option_t * opt = "likelihood-ratio");
  double Statistic      (FitModelABC * pred, const PhysHypothesis & ph, Option_t * opt = "likelihood-ratio");
  double Statistic      (FitModelABC * pred, Option_t * opt = "likelihood-ratio");

private:

};

} // t2kana

#endif


//____________________________________________________________________________
/*!

\class     valor::X01Data

\brief     Example 01 - Class deriving from FitDataABC

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   March 27, 2014

\copyright (c) 2010-2016, The VALOR group

*/
//____________________________________________________________________________

#ifndef _valor_example_01_data_h_
#define _valor_example_01_data_h_

#include <string>

#include "FitCore/FitDataABC.h"

class TObjArray;
class TH1D;

namespace valor {

class X01Data: public FitDataABC
{
public:
  X01Data();
  virtual ~X01Data();

  //
  // Implement the FitDataABC interface
  // See FitDataABC for method decription
  //

  string Name           (void);
  bool   Read           (string file, Option_t * opt = 0);
  bool   Generate       (FitModelABC * pred, const FitParamList & fp,   bool fluctuate=true);
  bool   Generate       (FitModelABC * pred, const PhysHypothesis & ph, bool fluctuate=true);
  bool   Generate       (FitModelABC * pred, bool fluctuate=true);
  bool   Write          (TObjArray* collection, const char * tag = "data", Option_t * write_opt = 0);
  bool   Write          (const char * filename, const char * tag = "data", Option_t * write_opt = 0, Option_t * file_opt = 0);
  int    NumOfBins      (void);
  double Statistic      (FitModelABC * pred, const FitParamList & fp,   Option_t * opt = "likelihood-ratio");
  double Statistic      (FitModelABC * pred, const PhysHypothesis & ph, Option_t * opt = "likelihood-ratio");
  double Statistic      (FitModelABC * pred, Option_t * opt = "likelihood-ratio");

private:

  TH1D * fTotSpectrumData;
};

} // valor

#endif


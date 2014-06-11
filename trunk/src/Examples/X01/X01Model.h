//____________________________________________________________________________
/*!

\class     valor::Ex01Model

\brief     Example 01 - Class deriving from FitModelABC

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   July 05, 2014

\copyright (c) 2010-2014, The VALOR group

*/
//____________________________________________________________________________

#ifndef _valor_example_01_model_h_
#define _valor_example_01_model_h_

#include <string>
#include <TObjArray.h>

#include "FitCore/FitModelABC.h"
#include "FitCore/PhysHypothesis.h"
#include "FitCore/PhysParamList.h"
#include "Utilities/NuOsc3Flavour.h"

class TH1D;

namespace valor {

class X01Model: public FitModelABC
{
public:
  X01Model();
 ~X01Model();

  //
  // Implement the FitModelABC interface
  // See FitModelABC for method decription
  //

  string        Name                   (void);
  bool          Read                   (string file, Option_t * opt = 0);
  bool          Generate               (const PhysHypothesis & ph, Option_t * opt = 0);
  void          RandomizeSystematics   (void);
  void          TweakSystematic        (string syst_name, double twkdial=0., Option_t * opt = 0); 
  void          TweakSystematic        (conv::PhysParamId_t syst, double twkdial=0., Option_t * opt = 0);
  void          ResetSystematics       (void);  
  void          PrintSystematics       (ostream & stream = std::cout) const;
  PhysParamList Tweaks                 (void);
  bool          Generate               (const FitParamList & fp, Option_t * opt = 0);
  bool          Write                  (TObjArray * collection, const char * tag = "model", Option_t * write_opt = 0);
  bool          Write                  (const char * filename,  const char * tag = "model", Option_t * write_opt = 0, Option_t * file_opt = 0);
  double        GetFitStatisticPenalty (Option_t * opt = 0) const;

private:

  TH1D * fTotSpectrumPdf;
  TH1D * fSigSpectrumPdf;
  TH1D * fBkgSpectrumPdf;

  PhysHypothesis fPhysHypothesis;
  NuOsc3Flavour  fNuOsc3Flavour;
};

} // valor

#endif


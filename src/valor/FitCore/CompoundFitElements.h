//____________________________________________________________________________
/*!

\class     valor::CompoundFitElements

\brief   

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   April 06, 2016

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_dune_compound_fit_elements_h_
#define _valor_dune_compound_fit_elements_h_

#include <string>

#include <TXMLEngine.h>

//fmwk
#include "valor/FitCore/CompoundFitData.h"
#include "valor/FitCore/CompoundFitModel.h"
#include "valor/FitCore/CorrelatedSystematics.h"
#include "valor/Conventions/KineSpace.h"
#include "valor/Conventions/BeamConf.h"
#include "valor/Conventions/Sample.h"
#include "valor/Conventions/Detector.h"

namespace valor {

class CompoundFitElements
{
public:
  CompoundFitElements();
 ~CompoundFitElements();

  bool Read(string xmlconfig, Option_t * opt = "");

  FitDataABC *  FitData  (void) const { return fCompoundFitData;  }
  FitModelABC * FitModel (void) const { return fCompoundFitModel; }

  bool DataGiven(void) const { return fDataFileGiven; }

private:

  void Init  (void);
  void Book  (void);
  void Clean (void);
  
  bool ReadParameterList(TXMLEngine* xmleng, XMLNodePointer_t top_node, Option_t * opt = "");
  bool ReadElementList  (TXMLEngine* xmleng, XMLNodePointer_t top_node, Option_t * opt = "");
  bool ReadElement      (TXMLEngine* xmleng, XMLNodePointer_t top_node, Option_t * opt = "");
  CorrelatedSystematics * ReadSystematicGroup (TXMLEngine* xmleng, XMLNodePointer_t top_node, Option_t * opt = "");

  FitDataABC *  AdoptFitData  (string config_file, conv::Detector_t d, conv::BeamConf_t b, conv::Sample_t s, conv::KineSpace_t o, string class_name, FitModelABC *model_abc, Option_t * opt = "");
  FitModelABC * AdoptFitModel (string config_file, conv::Detector_t d, conv::BeamConf_t b, conv::Sample_t s, conv::KineSpace_t o, string class_name, Option_t * opt = "");
  void *        InstantiateUsingDictionaryRegistration (string class_name);

  CompoundFitData  * fCompoundFitData;
  CompoundFitModel * fCompoundFitModel;

  bool fDataFileGiven = false;
};

} // valor

#endif


//____________________________________________________________________________
/*!

\class     valor::util::nm;

\brief     VALOR/DUNE package naming conventions

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   April 05, 2016

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_naming_conventions_h_
#define _valor_naming_conventions_h_

#include "valor/Conventions/KineSpace.h"

#include "valor/Conventions/Detector.h"
#include "valor/Conventions/BeamConf.h"
#include "valor/Conventions/Sample.h"


namespace valor {
namespace util  {
namespace nm    {
  
  static const std::string delimiter = "__";

  //
  // A VALOR fit dataset is a delimiter-separated list of:
  //
  // 1) The name of the detector
  //    e.g. `sbnd'
  // 2) The name of the beam configuration
  //    e.g. `fhc' 
  // 3) The name of the sample
  //    e.g. `numuCC2trk0pi'
  // 4) The name of the observable distribution --- CURRENTLY UNUSED ---
  //    e.g. `Ereco_yreco'
  //
  // Therefore the unique name is a `detector__beam__sample__observable' string
  //    e.g `sbnd__fhc__numuCC2trk0pi__Ereco_yreco'
  //
  // The following functions assemble the dataset name, or tokenize it
  // and return individual components.
  //
  string                   DSetName         (string det_nm,  string beam_nm,  string sample_nm, string obsrv_nm);
  string                   DSetName         (valor::conv::Detector_t det, valor::conv::BeamConf_t beam, valor::conv::Sample_t sample, valor::conv::KineSpace_t obsrv);
  string                   DSetDetectorName (string dataset);   // 1st token
  valor::conv::Detector_t  DSetDetector     (string dataset);   // 1st token
  string                   DSetBeamName     (string dataset);   // 2st token
  valor::conv::BeamConf_t  DSetBeam         (string dataset);   // 2st token
  string                   DSetSampleName   (string dataset);   // 3nd token
  valor::conv::Sample_t    DSetSample       (string dataset);   // 3nd token
/*
  string                   DSetObsrvName    (string dataset);   // 4th token
  valor::conv::KineSpace_t DSetObsrv        (string dataset);   // 4th token   
*/
  string                   DSetToken        (string dataset, int i);            

  //
  // A VALOR fit model is currently named as the dataset it corresponds to.
  // However using a different set of functions for naming datasets and models
  // to decouple the two and make it easier for them to evolve differently
  //
  string                   MdlName         (string det_nm,  string beam_nm,  string sample_nm, string obsrv_nm);
  string                   MdlName         (valor::conv::Detector_t det, valor::conv::BeamConf_t beam, valor::conv::Sample_t sample, valor::conv::KineSpace_t obsrv);
  string                   MdlDetectorName (string dataset);   // 1st token
  valor::conv::Detector_t  MdlDetector     (string dataset);   // 1st token
  string                   MdlBeamName     (string dataset);   // 2st token
  valor::conv::BeamConf_t  MdlBeam         (string dataset);   // 2st token
  string                   MdlSampleName   (string dataset);   // 3nd token
  valor::conv::Sample_t    MdlSample       (string dataset);   // 3nd token
/*
  string                   MdlObsrvName    (string dataset);   // 4th token
  valor::conv::KineSpace_t MdlObsrv        (string dataset);   // 4th token   
*/
  string                  MdlToken        (string dataset, int i);            


  //
  // Large collection of objects in various formats (arrays, histograms, graphs).
  // can be written out by FitModelABC____Write() or FitDataABC____Write() implementations
  // These objects represent the various pieces of data, model predictions, as well as
  // data about the data (e.g. arrays with bin edges etc.) and must be named uniquely.
  // The name of each object is a colon-separated list of 
  // 1) An optional tag 
  //    e.g. `best_fit', `toy_mc_18', ...
  // 2) The name of the dataset or model where it belongs 
  //    e.g. `sbnd__fhc__numuCC2trk0pi__Ereco_yreco'
  // 3) A string that further specifies the output quantity 
  //    e.g. `full_2d', `1d_slice_17', `Ereco_bin_edges',....
  // 4) A string indicating the format (the same information can be output in several formats)
  //    e.g. `arr1d', `arr2d', `th1d', `th2d',...
  // So the unique name of an output object name may look like
  // `toy_mc_18__sbnd__fhc__numuCC2trk0pi__Ereco_yreco__full_2d__arr2d'
  //
  string DatumName (string tag, string mother_obj_name, string quantity, string format);

} // nm    namespace
} // util  namespace
} // valor namespace

#endif // _valor_naming_conventions_h_

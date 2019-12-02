//____________________________________________________________________________
/*!

\class     valor::util::nm;

\brief     VALOR/DUNE package naming conventions

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   April 05, 2016

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

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
  // Therefore the unique name is a `detector__beam__sample__observable' std::string
  //    e.g `sbnd__fhc__numuCC2trk0pi__Ereco_yreco'
  //
  // The following functions assemble the dataset name, or tokenize it
  // and return individual components.
  //
  std::string              DSetName         (std::string det_nm,  std::string beam_nm,  std::string sample_nm, std::string obsrv_nm);
  std::string              DSetName         (valor::conv::Detector_t det, valor::conv::BeamConf_t beam, valor::conv::Sample_t sample, valor::conv::KineSpace_t obsrv);
  std::string              DSetDetectorName (std::string dataset);   // 1st token
  valor::conv::Detector_t  DSetDetector     (std::string dataset);   // 1st token
  std::string              DSetBeamName     (std::string dataset);   // 2st token
  valor::conv::BeamConf_t  DSetBeam         (std::string dataset);   // 2st token
  std::string              DSetSampleName   (std::string dataset);   // 3nd token
  valor::conv::Sample_t    DSetSample       (std::string dataset);   // 3nd token
/*
  std::string              DSetObsrvName    (std::string dataset);   // 4th token
  valor::conv::KineSpace_t DSetObsrv        (std::string dataset);   // 4th token   
*/
  std::string              DSetToken        (std::string dataset, int i);            

  //
  // A VALOR fit model is currently named as the dataset it corresponds to.
  // However using a different set of functions for naming datasets and models
  // to decouple the two and make it easier for them to evolve differently
  //
  std::string              MdlName         (std::string det_nm,  std::string beam_nm,  std::string sample_nm, std::string obsrv_nm);
  std::string              MdlName         (valor::conv::Detector_t det, valor::conv::BeamConf_t beam, valor::conv::Sample_t sample, valor::conv::KineSpace_t obsrv);
  std::string              MdlDetectorName (std::string dataset);   // 1st token
  valor::conv::Detector_t  MdlDetector     (std::string dataset);   // 1st token
  std::string              MdlBeamName     (std::string dataset);   // 2st token
  valor::conv::BeamConf_t  MdlBeam         (std::string dataset);   // 2st token
  std::string              MdlSampleName   (std::string dataset);   // 3nd token
  valor::conv::Sample_t    MdlSample       (std::string dataset);   // 3nd token
/*
  std::string              MdlObsrvName    (std::string dataset);   // 4th token
  valor::conv::KineSpace_t MdlObsrv        (std::string dataset);   // 4th token   
*/
  std::string              MdlToken        (std::string dataset, int i);            


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
  // 3) A std::string that further specifies the output quantity 
  //    e.g. `full_2d', `1d_slice_17', `Ereco_bin_edges',....
  // 4) A std::string indicating the format (the same information can be output in several formats)
  //    e.g. `arr1d', `arr2d', `th1d', `th2d',...
  // So the unique name of an output object name may look like
  // `toy_mc_18__sbnd__fhc__numuCC2trk0pi__Ereco_yreco__full_2d__arr2d'
  //
  std::string DatumName (std::string tag, std::string mother_obj_name, std::string quantity, std::string format);

} // nm    namespace
} // util  namespace
} // valor namespace

#endif // _valor_naming_conventions_h_

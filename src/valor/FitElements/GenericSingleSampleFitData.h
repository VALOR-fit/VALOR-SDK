//____________________________________________________________________________
/*!

\class     valor::GenericSingleSampleFitData

\brief     A generic class for storing data for a single sample, 
           in a specific beam configuration and for a specific detector.
           //
           // Some description about what info is stored for the sample
           //
           Derives from FitDataABC

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           Steve Dennis <s.r.dennis@liverpool.ac.uk>
           University of Liverpool

\created   March 27, 2014

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_generic_single_sample_data_h_
#define _valor_generic_single_sample_data_h_

#include <string>

//fmwk
#include "valor/Conventions/BeamConf.h"
#include "valor/Conventions/Sample.h"
#include "valor/Conventions/Detector.h"
#include "valor/FitCore/SingleSampleFitDataABC.h"
#include "valor/FitElements/GenericSingleSampleFitModel.h"
#include "valor/VArray/VALORArray.h"
#include "valor/Utilities/SingleSampleKineBinning.h"
#include "valor/Utilities/ToyStream.h"

class TObjArray;
class TH1D;

namespace valor {

class GenericSingleSampleFitData: public SingleSampleFitDataABC
{
public:
  GenericSingleSampleFitData(void);
  virtual ~GenericSingleSampleFitData();

  //
  // Implement the FitDataABC interface
  // See FitDataABC for method decription
  //

  string       Name              (void) const;
  bool         Read              (string file, Option_t * opt = 0);
  virtual bool Read              (util::ToyStream* stream);
  bool         Generate          (FitModelABC * pred, const FitParamList & fp,   bool fluctuate=true);
  bool         Generate          (FitModelABC * pred, const PhysHypothesis & ph, bool fluctuate=true);
  bool         Generate          (FitModelABC * pred, bool fluctuate=true, Option_t* read_opt = "+SB");
  bool         Write             (TObjArray * data_collection, const char * tag = "data", Option_t * write_opt = 0);
  bool         Write             (const char * filename, const char * tag = "data", const char * option_file="update", Option_t * file_opt = 0);
  virtual bool Write             (util::ToyStream* stream);
  virtual long StreamSize        (Option_t * opt = 0) const;  // Get the size (in bytes) that would be used to read/write the VALORArrays from/to a binary toy file with the specified option
  int          NumOfBins         (Option_t * opt = 0) const;
  double       GetFitStatistic   (FitModelABC * pred, const FitParamList & fp,   Option_t * opt = "likelihood-ratio", std::map<std::string,double> * likelihood_components = 0);
  double       GetFitStatistic   (FitModelABC * pred, const PhysHypothesis & ph, Option_t * opt = "likelihood-ratio", std::map<std::string,double> * likelihood_components = 0);
  double       GetFitStatistic   (FitModelABC * pred, Option_t * opt = "likelihood-ratio", std::map<std::string,double> * likelihood_components = 0);
  void         SetAssociatedModel(FitModelABC *model_abc); // Call this before Read to set the model which should be associated to the data
  virtual bool DataExists        (Option_t * opt = 0) const;  // Are the data arrays non-empty?

  const SingleSampleKineBinning * GetBinning(void) const { return fBinning; };

  // Get a string including the data name and the total PDF integral
  virtual std::string GetPDFIntegralAsString(Option_t * opt = 0);

  virtual void SetFromModel(void); // Set various parameters from the GSSFM

  // Initialise and return a new ToyStream object, with setting set appropriately for reading/writing using this class
  virtual util::ToyStream* InitToyStream(const string file_name, const string read_or_write = "read", string valor_array_format = "");

private:

  void Init  (void);
  void Book  (void);
  void Clean (void);
  void Zero  (Option_t * opt);
  void FillFromDST  (TTree * t); // <----- NOTE: Some code duplication with GSSFM - eliminate in future

  // Name of this VALOR fit dataset (see Conventions/Names.h)
  string fName;
  // And the corresponding model (used to get the right PDFs)
  string fModelName;

  // Data distributions:
  // Each distribution is an array that can be directly compared with data.
  // Distributions with binning heavily optimized to reduce the CPU intensity of the fit.
  bool         fDataObNull; 
  VALORArray * fDataOb    ; 
  // Below are versions the use a simple (~fine and ~regular) binning throughout the whole range.
  bool         fDataSbNull;
  VALORArray * fDataSb;

 // Actual or fake/mock data?
  bool fIsFakeData;

  // Binning booked with. Does not own
  const SingleSampleKineBinning * fBinning;

  double fFiducialMass;
  double fPOT;

  //Do we use 1D binning for chisq?
  bool fUse1DChisq;
  void SetUse1DChisq(bool use1d) { fUse1DChisq = use1d; }

  // Define associated GSSFM
  GenericSingleSampleFitModel *fGSSFM;

  std::string fDefVALORArrayFormat; // The default VALORArray format used to read/write toys
  
};

} // valor

#endif


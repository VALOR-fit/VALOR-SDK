//____________________________________________________________________________
/*!

\class     valor::CompoundFitData

\brief     A fit data object made up of a vector of simpler fit data object.

\author    Steve Dennis <s.r.dennis \at liverpool.ac.uk>
           University of Liverpool

\created   April 14, 2016

\copyright (c) 2010-2016, The VALOR group

*/
//____________________________________________________________________________

#ifndef _valor_compound_fit_data_h_
#define _valor_compound_fit_data_h_


#include <string>
#include <TROOT.h>

#include "valor/FitCore/CompoundFitModel.h"
#include "valor/FitCore/FitDataABC.h"

namespace valor {

class FitModelABC;
class FitParamList;
class PhysHypothesis;

class CompoundFitData : public FitDataABC
{
public:
  CompoundFitData();
 ~CompoundFitData();
  
// FitDataABC interface

  // Identify fit data with a name (to be propagated in output files, etc)
  virtual string Name(void) const;  
  virtual string CompoundName(void) const;  // Concatenates the Name() of each subdata

  // Get the size (in bytes) that would be used to read/write the VALORArrays from/to a binary toy file with the specified option
  virtual long StreamSize(Option_t* opt = 0) const;

  // Read data from the input file (ROOT, XML, other)
  // Use input option as needed to specify what exactly to read from the input file.
  // Return true if data were successfully read out
  virtual bool Read(string file, Option_t * opt = 0);
  virtual bool Read(util::ToyStream* stream);

  // Generate mock data (toy-MC)
  // Toy-MC is generated from using the input model 
  // If a FitParamList is specified, all physics parameters and systematics are set from its values
  // If a PhysHypothesis is specified, all physics parameters are set from its values and it is
  // assumed that systematics were tweaked independently
  // If no parameters are specified then the pre-generated model will be used.
  virtual bool Generate (FitModelABC * model, const FitParamList & fp,   bool fluctuate=true);
  virtual bool Generate (FitModelABC * model, const PhysHypothesis & ph, bool fluctuate=true);
  virtual bool Generate (FitModelABC * model, bool fluctuate=true, Option_t* read_opt = "+SB");

  // Write datasets to the input container or to the input file

  virtual bool Write (TObjArray * collection, const char * tag = "data", Option_t * write_opt = 0);
  virtual bool Write (const char * filename,  const char * tag = "data", Option_t * write_opt = 0, Option_t * file_opt = 0);
  virtual bool Write (util::ToyStream* stream);

  // Number of bins in fit data, or number of events for unbinned fits
  virtual int NumOfBins (Option_t * opt = 0) const;

  // Calculate the specified statistic
  // The corresponding prediction is taken from the input model.
  // Similar considerations as in Write() method, see above, apply for how this prediction is generated.
  // This method calculates the statistical term. The penalty term is not included - you must ask the model for this specifically.
  virtual double GetFitStatistic (FitModelABC * pred, const FitParamList & fp,   Option_t * stat_opt = "likelihood-ratio", std::map<std::string,double> * likelihood_components = 0);
  virtual double GetFitStatistic (FitModelABC * pred, const PhysHypothesis & ph, Option_t * stat_opt = "likelihood-ratio", std::map<std::string,double> * likelihood_components = 0);
  virtual double GetFitStatistic (FitModelABC * pred, Option_t * stat_opt = "likelihood-ratio", std::map<std::string,double> * likelihood_components = 0);
  
// Own interface
  virtual void AddSubData(FitDataABC * data, Option_t * opt = 0);
  virtual const FitDataABC * GetSubData(int data_idx, Option_t * opt = 0) const;
  virtual FitDataABC * GetSubData(int data_idx, Option_t * opt = 0);
  virtual unsigned int GetNSubData(Option_t * opt = 0) const;
  
  void SetName(std::string name);

  // Associate a FitModelABC with the data to define the binning and other attributes of the experimental setup
  virtual void SetAssociatedModel(FitModelABC *model_abc);

  // Initialise and return a new ToyStream object, with setting set appropriately for reading/writing using this class
  virtual util::ToyStream* InitToyStream(const string file_name, const string read_or_write = "read", string valor_array_format = "");

  // Get the total PDF integral of each subdata as a string
  virtual std::string GetPDFIntegralAsString(Option_t * opt = 0);

  virtual void SetFromModel(void); // Set various parameters from the model

  // Are the data arrays non-empty?
  virtual bool DataExists(Option_t * opt = 0) const;

  void SetUse1DChisq(bool use1d) {
    for (std::vector<FitDataABC*>::iterator it = fSubData.begin(); it != fSubData.end(); ++it)
      (*it)->SetUse1DChisq(use1d);
  };

protected:
  std::string fName;
  std::vector<FitDataABC *> fSubData;
  CompoundFitModel *fCFM;
  std::string fDefVALORArrayFormat; // The default VALORArray format used to read/write toys
};

} // valor

#endif


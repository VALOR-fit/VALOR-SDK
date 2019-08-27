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

  // Read data from the input file (ROOT, XML, other)
  // Use input option as needed to specify what exactly to read from the input file.
  // Return true if data were succesfully read out
  virtual bool Read(string file, Option_t * opt = 0);
  virtual bool Read(std::ifstream& stream, FitModelABC * model = 0, Option_t * read_opt = 0);

  // Generate mock data (toy-MC)
  // Toy-MC is generated from using the input model 
  // If a FitParamList is specified, all physics parameters and systematics are set from its values
  // If a PhysHypothesis is specified, all physics parameters are set from its values and it is
  // assumed that systematics were tweaked independently
  // If no parameters are specified then the pre-generated model will be used.
  virtual bool Generate (FitModelABC * model, const FitParamList & fp,   bool fluctuate=true);
  virtual bool Generate (FitModelABC * model, const PhysHypothesis & ph, bool fluctuate=true);
  virtual bool Generate (FitModelABC * model, bool fluctuate=true);

  // Write datasets to the input container or to the input file

  virtual bool Write (TObjArray * collection, const char * tag = "data", Option_t * write_opt = 0);
  virtual bool Write (const char * filename,  const char * tag = "data", Option_t * write_opt = 0, Option_t * file_opt = 0);
  virtual bool Write (std::ofstream& stream, Option_t * write_opt = 0);

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

protected:
  std::string fName;
  std::vector<FitDataABC *> fSubData;
};

} // valor

#endif


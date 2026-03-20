//____________________________________________________________________________
/*!

\class     valor::CompoundFitModel

\brief     A fit model made up of a vector of simpler fit models.

\author    Steve Dennis <s.r.dennis \at liverpool.ac.uk>
           University of Liverpool

\created   April 04, 2016

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_compound_fit_model_h_
#define _valor_compound_fit_model_h_

#include <string>
#include <iostream>
#include <vector>

#include "valor/FitCore/CorrelatedSystematics.h"
#include "valor/FitCore/FitModelABC.h"

class TObjArray;

namespace valor {

class CompoundFitModel : public FitModelABC
{
public:
  CompoundFitModel();
 ~CompoundFitModel();
// Fit Model ABC interface
  // Identify fit model with a name (to be propagated in output files, etc)
  virtual std::string Name(void) const ;
  virtual string CompoundName(void) const;  // Concatenates the Name() of each submodel

  // Read configuration data from file
  virtual bool Read(std::string file, Option_t * opt = 0) ;
  virtual bool Read(util::ToyStream* stream);

  // Get the size (in bytes) that would be used to read/write the VALORArrays from/to a binary toy file with the specified option
  virtual int StreamSize(Option_t* opt = 0) const;

  // Below are a set methods to generate model predictions for a given physics hypothesis and a set of nuisance parameters.
  // The model is first tweaked appropriately by setting systematics to particular values or by randomizing them.
  // Then one uses the tweaked model to generate predictions for the given physics hypothesis.
  // The input option in TweakSystematics() can be used as needed in a particular context to interpret the input tweak
  // (for example, in T2K context, to specify whether the input tweak is wrt to the nominal and BANFF-tuned MC parameters).
  virtual bool Generate             (const PhysHypothesis & ph, Option_t * opt = 0) ;
  
  // This one will randomize all systematics
  virtual void RandomizeSystematics (FitParamList& fpl, Option_t * opt = 0);
  // These two only randomize the systematics inside that return "true" in the should_randomize argument
  virtual void RandomizeSystematics (bool (*should_randomize)(conv::PhysParamId_t), FitParamList& fpl, Option_t * opt = 0);
  virtual void RandomizeSystematics (std::map<valor::conv::PhysParamId_t, bool> should_randomize, FitParamList& fpl, Option_t * opt = 0);
  
  virtual void TweakSystematic      (std::string syst_name, double twkdial=0., Option_t * opt = 0) ; 
  virtual void TweakSystematic      (conv::PhysParamId_t syst, double twkdial=0., Option_t * opt = 0) ; 
  // Additional related methods to reset the systematics and enquire their values.
  virtual void ResetSystematics         (FitParamList& fpl, Option_t * opt = 0) ;
  virtual void ForceSystematicsToUntuned(FitParamList& fpl, Option_t * opt = 0) ;
  virtual void PrintSystematics         (std::ostream & stream = std::cout, Option_t * opt = 0) const ;
  virtual PhysParamList Tweaks          (Option_t * opt = 0) const ;

  // This is a second interface for generating predictions for a given physics hypothesis and systematics.
  // This interface method was developed for fitters. 
  // The fit parameter list contains all relevant parameters, describing both a physics hypothesis and systematic variations.
  virtual bool Generate (const FitParamList & fp, Option_t * opt = 0) ;

  // Write all predictions to the input container or to the input file
  virtual bool Write (TObjArray* collection, const char * tag = "model", Option_t * write_opt = 0) ;
  virtual bool Write (const char * filename, const char * tag = "model", Option_t * write_opt = 0, Option_t * file_opt = 0) ;
  virtual bool Write (util::ToyStream* stream);

  // Get penalty term for current systematics
  virtual double GetFitStatisticPenalty(Option_t * opt = 0) const ;
  
// Own interface
  virtual void AddSubModel(FitModelABC * model, Option_t * opt = 0);
  virtual const FitModelABC * GetSubModel(int model_idx, Option_t * opt = 0) const;
  virtual FitModelABC * GetSubModel(int model_idx, Option_t * opt = 0);
  virtual const std::vector<FitModelABC *> GetSubModels(Option_t * opt = 0) const;
  virtual std::vector<FitModelABC *> GetSubModels(Option_t * opt = 0);
  virtual unsigned int GetNSubModels(Option_t * opt = 0) const;

  // By default SetSystConf just points at an external syst object. Use opt="copy" or "adopt" if you want control.
  virtual void  SetSystConf(CorrelatedSystematics * syst, Option_t * opt = "");
  virtual       CorrelatedSystematics * GetSystematics(Option_t * opt = "");
  virtual const CorrelatedSystematics * GetSystematics(Option_t * opt = "") const;
  
  void SetName(std::string name);

  // Initialise and return a new ToyStream object, with setting set appropriately for reading/writing using this class
  virtual util::ToyStream* InitToyStream(const string file_name, const string read_or_write = "read", string valor_array_format = "");

  // Get the total PDF integral of each submodel as a string
  virtual std::string GetPDFIntegralAsString(Option_t * opt = 0);

protected:
  std::string fName;
  std::vector<FitModelABC *> fSubModels;
  
  CorrelatedSystematics * fSystematics;
  bool fOwnerOfSystematics;
  std::string fDefVALORArrayFormat; // The default VALORArray format used to read/write toys

};

} // valor

#endif


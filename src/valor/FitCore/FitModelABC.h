//____________________________________________________________________________
/*!

\class     valor::FitModelABC

\brief     Pure abstract base class (pABC) for fit models.

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   March 30, 2014

\copyright (c) 2010-2016, The VALOR group

*/
//____________________________________________________________________________

#ifndef _valor_fit_model_abc_h_
#define _valor_fit_model_abc_h_

#include <fstream>
#include <string>
#include <iostream>
#include <map>

#include "valor/Conventions/PhysParamId.h"
#include "valor/FitCore/PhysHypothesis.h"
#include "valor/FitCore/PhysParamList.h"

class TObjArray;

namespace valor {

class FitModelABC
{
public:
  virtual ~FitModelABC();

  // Identify fit model with a name (to be propagated in output files, etc)
  virtual std::string Name(void) const = 0;

  // Read configuration data from file
  virtual bool Read(std::string file, Option_t * opt = 0) = 0;
  virtual bool Read(std::ifstream& stream, Option_t* read_opt = 0) = 0;

  // Below are a set methods to generate model predictions for a given physics hypothesis and a set of nuisance parameters.
  // The model is first tweaked appropriately by setting systematics to particular values or by randomizing them.
  // Then one uses the tweaked model to generate predictions for the given physics hypothesis.
  // The input option in TweakSystematics() can be used as needed in a particular context to interpret the input tweak
  // (for example, in T2K context, to specify whether the input tweak is wrt to the nominal and BANFF-tuned MC parameters).
  virtual bool Generate             (const PhysHypothesis & ph, Option_t * opt = 0) = 0;
  
  // This one will randomize all systematics
  virtual void RandomizeSystematics (Option_t * opt = 0) = 0;
  // These two only randomize the systematics inside that return "true" in the should_randomize argument
  virtual void RandomizeSystematics (bool (*should_randomize)(conv::PhysParamId_t), Option_t * opt = 0) = 0;
  virtual void RandomizeSystematics (std::map<valor::conv::PhysParamId_t, bool> should_randomize, Option_t * opt = 0) = 0;
   
  virtual void TweakSystematic      (std::string syst_name, double twkdial=0., Option_t * opt = 0) = 0; 
  virtual void TweakSystematic      (conv::PhysParamId_t syst, double twkdial=0., Option_t * opt = 0) = 0; 
  // Additional related methods to reset the systematics and enquire their values.
  virtual void ResetSystematics (Option_t * opt = 0) = 0;  
  virtual void PrintSystematics (std::ostream & stream = std::cout, Option_t * opt = 0) const = 0;
  virtual PhysParamList Tweaks(Option_t * opt = 0) const = 0;

  // This is a second interface for generating predictions for a given physics hypothesis and systematics.
  // This interface method was developed for fitters. 
  // The fit parameter list contains all relevant parameters, describing both a physics hypothesis and systematic variations.
  virtual bool Generate (const FitParamList & fp, Option_t * opt = 0) = 0;

  // Write all predictions to the input container or to the input file
  virtual bool Write (TObjArray* collection, const char * tag = "model", Option_t * write_opt = 0) = 0;
  virtual bool Write (const char * filename, const char * tag = "model", Option_t * write_opt = 0, Option_t * file_opt = 0) = 0;
  virtual bool Write(std::ofstream& stream, Option_t* write_opt = 0) = 0;
  
  // Get penalty term for current systematics
  virtual double GetFitStatisticPenalty(Option_t * opt = 0) const = 0;
  
  // Interface for the systematic parameters
  // By default SetSystConf just points at an external syst object. Use opt="copy" or "adopt" if you want control.
  virtual void  SetSystConf(CorrelatedSystematics * syst, Option_t * opt = "") = 0;
  virtual       CorrelatedSystematics * GetSystematics(Option_t * opt = "") = 0;
  virtual const CorrelatedSystematics * GetSystematics(Option_t * opt = "") const = 0;

protected:
   FitModelABC();
};

} // valor

#endif


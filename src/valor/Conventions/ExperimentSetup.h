//____________________________________________________________________________
/*!
   
\class     valor::ExperimentSetup
      
\brief     A singleton to any information that gets compiled into experiment-specific code but that may be needed by the SDK
   
\author   Steve Dennis <s.r.dennis \at liverpool.ac.uk>
          University of Liverpool

\created  September 2019

\copyright (c) 2010-2019, The VALOR group.

*/
//____________________________________________________________________________
   
#ifndef _valor_experiment_setup_h_
#define _valor_experiment_setup_h_

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "valor/Conventions/BeamConf.h"
#include "valor/Conventions/ReacMode.h"
#include "valor/Conventions/ReacModeCoarse.h"
#include "valor/Conventions/Sample.h"

namespace valor {

// Fills the 
extern bool DoExperimentSetup();

class ExperimentSetup
{
public:

  static ExperimentSetup * MutableInstance();
  static const ExperimentSetup * Instance();
  
  void Print(std::ostream & stream = std::cout) const;
  
  bool IsSetup() const;
  
  // Simple getter methods for all elements
  const std::string & ExperimentName() const;
  const std::vector<conv::BeamConf_t> & BeamConfs() const;
  const std::vector< std::vector<conv::Sample_t> > & Samples() const;
  const std::vector<conv::Sample_t> & Samples(conv::BeamConf_t) const;
  const std::vector<std::vector<std::vector<conv::ReacMode_t> > > & ReactionModes() const;
  const std::vector<conv::ReacMode_t> & ReactionModes(conv::BeamConf_t, conv::Sample_t) const;
  const std::vector<std::vector<std::vector<conv::ReacModeCoarse_t> > > & ReactionModesCoarse() const;
  const std::vector<conv::ReacModeCoarse_t> & ReactionModesCoarse(conv::BeamConf_t, conv::Sample_t) const;
  const std::vector<std::vector<std::vector<std::vector<int> > > > & PlotStyle() const;
  const std::vector<int> & PlotStyle(conv::BeamConf_t, conv::Sample_t, conv::ReacModeCoarse_t) const;

  // python aliases
  const std::vector<conv::Sample_t>         & SamplesFromBeam                        (conv::BeamConf_t b)                                              const { return this->Samples(b); }
  const std::vector<conv::ReacModeCoarse_t> & ReactionModesCoarseFromBeamAndSample   (conv::BeamConf_t b, conv::Sample_t s)                            const { return this->ReactionModesCoarse(b,s); }
  const std::vector<int>                    & PlotStyleFromBeamAndSampleAndCoarseMode(conv::BeamConf_t b, conv::Sample_t s, conv::ReacModeCoarse_t mc) const { return this->PlotStyle(b, s, mc); }

  // Get the array index a specific enum value represents for this experiment
  int BeamConfIndex(conv::BeamConf_t) const;
  int SampleIndex(conv::BeamConf_t b, conv::Sample_t s) const;
  int ReacModeIndex(conv::BeamConf_t b, conv::Sample_t s, conv::ReacMode_t m) const;
  int ReacModeCoarseIndex(conv::BeamConf_t b, conv::Sample_t s, conv::ReacModeCoarse_t mc) const;
  
private:

  ExperimentSetup();
  //~ ExperimentSetup(const ExperimentSetup &);
 ~ExperimentSetup();

  static ExperimentSetup * fInstance;

  struct Cleaner {
    void DummyMethodAndSilentCompiler() { }
   ~Cleaner() {
      if (ExperimentSetup::fInstance !=0) {
        delete ExperimentSetup::fInstance;
        ExperimentSetup::fInstance = 0;
      }
    }
  };
  friend struct Cleaner;
  
  std::string fExperimentName;
  
  bool fIsSetup;
  
  std::vector<conv::BeamConf_t> fBeamConfs;
  // NBeamConf -> NSamples
  std::vector< std::vector<conv::Sample_t> > fSamples;
  // NBeamConf -> NSamples -> NModes
  std::vector<std::vector<std::vector<conv::ReacMode_t> > > fReactionModes;
  //NBeamConf->NSample->NReacModeCoarse
  std::vector<std::vector<std::vector<conv::ReacModeCoarse_t> > > fReactionModesCoarse;
  
  // Oh god. NBeamConf->NSample->NReacModeCoarse->[line color, fill color, fill style]
  std::vector<std::vector<std::vector<std::vector<int> > > > fPlotStyle;
  
  
  friend bool DoExperimentSetup();
};

} // valor

#endif

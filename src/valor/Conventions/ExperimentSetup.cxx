//________________________________________________________________________________________
/*

Copyright (c) 2010-2019, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <iostream>
#include <iterator>

#include "valor/Conventions/ExperimentSetup.h"

using namespace valor;
using namespace valor::conv;
using std::string;
using std::vector;

//________________________________________________________________________________________
ExperimentSetup * ExperimentSetup::fInstance = 0;
//________________________________________________________________________________________
ExperimentSetup::ExperimentSetup()
{
}
//________________________________________________________________________________________
ExperimentSetup::ExperimentSetup(const ExperimentSetup &)
{
}
//________________________________________________________________________________________
ExperimentSetup::~ExperimentSetup()
{
}
//________________________________________________________________________________________
void ExperimentSetup::Print(std::ostream & stream) const
{
  stream<<ExperimentName()<<std::endl;
}
//________________________________________________________________________________________
bool ExperimentSetup::IsSetup() const
{
  return fIsSetup;
}
//________________________________________________________________________________________
const string & ExperimentSetup::ExperimentName() const
{
  return fExperimentName;
}
//________________________________________________________________________________________
const vector<BeamConf_t> & ExperimentSetup::BeamConfs() const
{
  return fBeamConfs;
}
//________________________________________________________________________________________
const vector< vector<Sample_t> > & ExperimentSetup::Samples() const
{
  return fSamples;
}
//________________________________________________________________________________________
const vector<vector<vector<ReacMode_t> > > & ExperimentSetup::ReactionModes() const
{
  return fReactionModes;
}
//________________________________________________________________________________________
const vector<vector<vector<ReacModeCoarse_t> > > & ExperimentSetup::ReactionModesCoarse() const
{
  return fReactionModesCoarse;
}
//________________________________________________________________________________________
const vector<vector<vector<vector<int> > > > & ExperimentSetup::PlotStyle() const
{
  return fPlotStyle;
}
//____________________________________________________________________________
const valor::ExperimentSetup * valor::ExperimentSetup::Instance() {return MutableInstance();}
//________________________________________________________________________________________
valor::ExperimentSetup * valor::ExperimentSetup::MutableInstance() 
{
  if(fInstance == 0) {
    static valor::ExperimentSetup::Cleaner cleaner;
    cleaner.DummyMethodAndSilentCompiler();
    fInstance = new valor::ExperimentSetup;
  }
  return fInstance;
}
//________________________________________________________________________________________
// Next three methods are ugly replacements for ArrIdx, which only had to exist for speed in the first place
int valor::ExperimentSetup::BeamConfIndex(conv::BeamConf_t b) const
{
  for (size_t ib = 0 ; ib < fBeamConfs.size(); ib++ ) {
    if (fBeamConfs[ib] == b) {
      return ib;
    }
  }
  return -1;
}
//________________________________________________________________________________________
int valor::ExperimentSetup::SampleIndex(conv::BeamConf_t b, conv::Sample_t s) const
{
  int ib = this->BeamConfIndex(b);
  for (size_t is = 0 ; is < fSamples[ib].size() ; is++) {
    if (fSamples[ib][is] == s) {return is;}
  }
  return -1;
}
//________________________________________________________________________________________
int valor::ExperimentSetup::ReacModeIndex(conv::BeamConf_t b, conv::Sample_t s, conv::ReacMode_t m) const
{
  int ib = this->BeamConfIndex(b);
  int is = this->SampleIndex(b,s);
  for (size_t im = 0 ; im < fReactionModes[ib][is].size() ; im++) {
    if (fReactionModes[ib][is][im]==m) {return im;}
  }
  return -1;
}
//________________________________________________________________________________________

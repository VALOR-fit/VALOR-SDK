//________________________________________________________________________________________
/*

Copyright (c) 2010-2019, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <iostream>

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
//____________________________________________________________________________
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

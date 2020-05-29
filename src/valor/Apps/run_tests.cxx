//____________________________________________________________________________
/*!

\app       valor-run_tests

\brief     Tests for GENIE true kinematics.

\options     --mc-events-file   : The file containing the GENIE events [required]
             --mesg-thresholds  : Message thresholds [def: "verbosity.xml"]

\author    Andy Chappell <A.Chappell \at warwick.ac.uk>
           University of Warwick

\created   February 20, 2017

\copyright (c) 2010-2017, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

// VALOR/SDK
#include "valor/CmdLine/CmdLnArgList.h"
#include "valor/Utilities/UtlFile.h"
#include "valor/Utilities/UtlInit.h"
#include "valor/GENIEUtilities/GENIEIncludes.h"

using namespace valor;

/**
   Performs basic configuration.

   The environment is set to dune and the set of mandatory and optional command
   line arguments is loaded. Message thresholds are set.

   \param argc The number of command line arguments.
   \param argv The command line arguments.
   \param arg  An object to populate with the command line arguments as key
      value pairs.
*/
void Configure(const int argc, const char** argv, CmdLnArgList& arg)
{
   arg = util::init::Init("apps/run_tests.arg", "apps/seeds.arg",
         argc, argv);

   //~ genie::Messenger::Instance()->SetPrioritiesFromXmlFile(
         //~ util::file::GetConfigurationFilename("mesg-thresholds"));
}

int main(const int argc, const char** argv)
{
   CmdLnArgList arg;
   Configure(argc, argv, arg);
   return testing::BatchTests(arg);
}

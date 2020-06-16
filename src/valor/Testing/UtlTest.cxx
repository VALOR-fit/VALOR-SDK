//____________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

// ROOT
#include <TFile.h>
#include <TTree.h>
// VALOR/SDK
#include "valor/Testing/UtlTest.h"
#include "valor/Utilities/UtlFile.h"
#if GENIE_ENABLED==1
#include "valor/GENIEUtilities/GENIEIncludes.h"
#endif

namespace valor
{
namespace testing
{

//____________________________________________________________________________
bool UtlTestCase::TestGetTrueKinematics()
{
#if GENIE_ENABLED==1
   // Retrieve the location of the GENIE MC events
   std::string filename = dynamic_cast<UtlTest*>(_test)->_mcEventsFilename;
   TFile file(filename.c_str(), "READ");

   // Load the tree containing the events and specify the n-tuple to load events
   // into
   TTree* tree = dynamic_cast<TTree*>(file.Get("gtree"));
   genie::NtpMCEventRecord* mcrec = 0;
   tree->SetBranchAddress("gmcrec", &mcrec);

   // The expected truth values of E and Q2 - gntp.test.ghep.root
   double Eexp[10] = {0.577132, 0.701179, 4.43347, 0.564247, 0.456497,
      0.621421, 0.329308, 1.20505, 0.688733, 0.512196};
   double Q2exp[10] = {0.539777, 0.212278, 0.273801, 0.469212, 0.106599,
      0.484452, 0.151665, 0.157586, 0.184618, 0.364481};

   // Test retrieval of Etrue values
   bool passed = true;
   for(Long64_t i = 0; i < tree->GetEntries(); ++i)
   {
      tree->GetEntry(i);
      genie::EventRecord& event = *(mcrec->event);

      conv::KineSpace_t kv = conv::kspEtrue;
      std::vector<double> vals = valor::util::GetTrueKinematics(&event, kv);

      passed = Test::Equal(vals.size(), 1) && passed;

      passed = Test::Equal(vals[0], Eexp[i], 5e-6) && passed;
   }

   // Test retrieval of Etrue and Q2true values
   for(Long64_t i = 0; i < tree->GetEntries(); ++i)
   {
      tree->GetEntry(i);
      genie::EventRecord& event = *(mcrec->event);

      conv::KineSpace_t kv = conv::kspEtrueQ2true;
      std::vector<double> vals = valor::util::GetTrueKinematics(&event, kv);

      passed = Test::Equal(vals.size(), 2) && passed;

      passed = Test::Equal(vals[0], Eexp[i], 5e-6) && passed;

      passed = Test::Equal(vals[1], Q2exp[i]) && passed;

      mcrec->Clear();
   }

   delete tree;

   return passed;

#else
   return true;
#endif
}
//____________________________________________________________________________
void UtlTest::Setup()
{
   _mcEventsFilename = _arg->GetString("mc-events-file");
}
//____________________________________________________________________________
void UtlTest::Teardown()
{

}
//____________________________________________________________________________
void UtlTest::AddTests()
{
   UtlTestCase* testCase = new UtlTestCase(this);

   Add(new TestKey("TestGetTrueKinematics", testCase),
         [testCase](void) { return testCase->TestGetTrueKinematics(); });
}
//____________________________________________________________________________

}
}

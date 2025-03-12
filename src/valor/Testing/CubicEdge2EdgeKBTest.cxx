//____________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

// VALOR/SDK
#include "valor/Testing/CubicEdge2EdgeKBTest.h"
#include "valor/Conventions/ModeList.h"
#include "valor/Utilities/UtlFile.h"

namespace valor
{
namespace testing
{
//____________________________________________________________________________
bool CubicEdge2EdgeKBTestCase::TestContract()
{
   SingleSampleKineBinning& binning =
      *(dynamic_cast<CubicEdge2EdgeKBTest*>(_test)->_binning);

   const KBCubHcmbEdge2Edge * tmpb =
     dynamic_cast<const KBCubHcmbEdge2Edge*>(binning.RecoSmpBinning());
   if (!tmpb) { return false; }
   const KBCubHcmbEdge2Edge& recoBinning = *tmpb;

   bool passed = true;

   // Ereco
   KBCubHcmbEdge2Edge* newBinning =
     dynamic_cast<KBCubHcmbEdge2Edge*>(recoBinning.Contract(conv::kspEreco));

   passed = Test::Equal(newBinning->NBins(), 4) && passed;

   // Check bin edges
   passed = Test::Equal(newBinning->BinMinimum(0, 0), 0.0) &&
      Test::Equal(newBinning->BinMaximum(0, 0), 50.0) && passed;
   passed = Test::Equal(newBinning->BinMinimum(1, 0), 50.0) &&
      Test::Equal(newBinning->BinMaximum(1, 0), 100.0) && passed;
   passed = Test::Equal(newBinning->BinMinimum(2, 0), 100.0) &&
      Test::Equal(newBinning->BinMaximum(2, 0), 150.0) && passed;
   passed = Test::Equal(newBinning->BinMinimum(3, 0), 150.0) &&
      Test::Equal(newBinning->BinMaximum(3, 0), 200.0) && passed;

   // Check minima and maxima
   passed = Test::Equal(newBinning->Minimum(0), 0.0) && passed;
   passed = Test::Equal(newBinning->Maximum(0), 200.0) && passed;

   delete newBinning;

   // Yreco
   newBinning =
     dynamic_cast<KBCubHcmbEdge2Edge*>(recoBinning.Contract(conv::kspYreco));

   passed = Test::Equal(newBinning->NBins(), 3) && passed;

   // Check bin edges
   passed = Test::Equal(newBinning->BinMinimum(0, 0), 0.0) &&
      Test::Equal(newBinning->BinMaximum(0, 0), 1.0 / 3.0) && passed;
   passed = Test::Equal(newBinning->BinMinimum(1, 0), 1.0 / 3.0) &&
      Test::Equal(newBinning->BinMaximum(1, 0), 2.0 / 3.0) && passed;
   passed = Test::Equal(newBinning->BinMinimum(2, 0), 2.0 / 3.0) &&
      Test::Equal(newBinning->BinMaximum(2, 0), 1.0) && passed;

   // Check minima and maxima
   passed = Test::Equal(newBinning->Minimum(0), 0.0) && passed;
   passed = Test::Equal(newBinning->Maximum(0), 1.0) && passed;

   delete newBinning;

   return passed;
}
//____________________________________________________________________________
void CubicEdge2EdgeKBTest::Setup()
{
   // Retrieve the location of the Mondrian XML file
   std::string filename = _arg->GetString("binning-file");
   std::string fullFilename = util::file::GetConfigurationFilename(filename);

   ModeList ml(util::CompleteConfigPath("modelists/UnOscLowE.xml"));

   _binning = new SingleSampleKineBinning(conv::kdDUNEFD, conv::kbLBNF_FHC,
        conv::ksNuMuCC,ml.Modes());
   _binning->Read(fullFilename);
}
//____________________________________________________________________________
void CubicEdge2EdgeKBTest::Teardown()
{
   delete _binning;
}
//____________________________________________________________________________
void CubicEdge2EdgeKBTest::AddTests()
{
   CubicEdge2EdgeKBTestCase* testCase = new CubicEdge2EdgeKBTestCase(this);

   Add(new TestKey("TestContract", testCase),
         [testCase](void) { return testCase->TestContract(); });
}
//____________________________________________________________________________

}
}

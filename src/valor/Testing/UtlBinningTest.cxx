//____________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

// ROOT
#include <TAxis.h>
#include <TCanvas.h>
// VALOR/SDK
#include "valor/Testing/UtlBinningTest.h"
#include "valor/Conventions/ModeList.h"
#include "valor/Utilities/UtlFile.h"
#include "valor/Utilities/UtlPlotting.h"

namespace valor
{
namespace testing
{
//____________________________________________________________________________
bool UtlBinningTestCase::TestMake2DOptimisedHistogram()
{
   SingleSampleKineBinning& binning =
      *(dynamic_cast<UtlBinningTest*>(_test)->_binning);
   VALORArray& v = *(dynamic_cast<UtlBinningTest*>(_test)->_vOpt);

   TH2D* histo = util::binning::Make2DHistogram(v, binning.RecoOptBinning(),
         util::KineVar(conv::kspErecoYreco));

   bool passed = true;

   // Bin 0 in Mondrian contains 105, so bins (1, 1) to (1, 5) should be 105
   passed = Test::Equal(histo->GetBinContent(1, 1), 105.0) && passed;
   passed = Test::Equal(histo->GetBinContent(1, 5), 105.0) && passed;

   // Bin 4 in Mondrian contains 125, so bins (2, 8) to (3, 8) should be 125
   passed = Test::Equal(histo->GetBinContent(2, 8), 125.0) && passed;
   passed = Test::Equal(histo->GetBinContent(3, 8), 125.0) && passed;

   // Bin 6 in Mondrian contains 135, so bins (3, 1) to (4, 1) should be 135
   passed = Test::Equal(histo->GetBinContent(3, 1), 135.0) && passed;
   passed = Test::Equal(histo->GetBinContent(4, 1), 135.0) && passed;

   // Bin 11 in Mondrian contains 160, so bins (4, 8) to (4, 9) should be 160
   passed = Test::Equal(histo->GetBinContent(4, 8), 160.0) && passed;
   passed = Test::Equal(histo->GetBinContent(4, 9), 160.0) && passed;

   util::plotting::Plot(histo, "Mondrian histogram test", "E_{r} / GeV",
         "y_{r}", "mondrian.pdf");

   delete histo;

   return passed;
}
//____________________________________________________________________________
bool UtlBinningTestCase::TestMake2DSimpleHistogram()
{
   SingleSampleKineBinning& binning =
      *(dynamic_cast<UtlBinningTest*>(_test)->_binning);
   VALORArray& v = *(dynamic_cast<UtlBinningTest*>(_test)->_vSmp);

   TH2D* histo = util::binning::Make2DHistogram(v, binning.RecoSmpBinning(),
         util::KineVar(conv::kspErecoYreco));

   bool passed = true;

   // Bin 0 in Edge2Edge contains 105, so bin (1, 1) should be 105
   passed = Test::Equal(histo->GetBinContent(1, 1), 105.0) && passed;

   // Bin 3 in Edge2Edge contains 120, so bin (4, 1) should be 120
   passed = Test::Equal(histo->GetBinContent(4, 1), 120.0) && passed;

   // Bin 8 in Edge2Edge contains 145, so bin (1, 3) should be 145
   passed = Test::Equal(histo->GetBinContent(1, 3), 145.0) && passed;

   // Bin 5 in Edge2Edge contains 130, so bin (2, 2) should be 130
   passed = Test::Equal(histo->GetBinContent(2, 2), 130.0) && passed;

   // Bin 11 in Edge2Edge contains 160, so bin (4, 3) should be 160
   passed = Test::Equal(histo->GetBinContent(4, 3), 160.0) && passed;

   util::plotting::Plot(histo, "Edge-to-edge histogram test", "E_{r} / GeV",
         "y_{r}", "edge2edge.pdf");

   delete histo;

   return passed;
}
//____________________________________________________________________________
bool UtlBinningTestCase::TestMake1DHistogram()
{
   SingleSampleKineBinning& binning =
      *(dynamic_cast<UtlBinningTest*>(_test)->_binning);
   VALORArray& v = *(dynamic_cast<UtlBinningTest*>(_test)->_vSmp);

   TH1D* histo = util::binning::Make1DHistogram(v, binning, conv::kspEreco);

   bool passed = true;

   // Bins 0, 4, 8 in Edge2Edge contains 375, so bin (1) should be 375
   passed = Test::Equal(histo->GetBinContent(1), 375.0) && passed;

   // Bin 1, 5, 9 in Edge2Edge contains 390, so bin (2) should be 390
   passed = Test::Equal(histo->GetBinContent(2), 390.0) && passed;

   // Bin 2, 6, 10 in Edge2Edge contains 405, so bin (3) should be 405
   passed = Test::Equal(histo->GetBinContent(3), 405.0) && passed;

   // Bin 3, 7, 11 in Edge2Edge contains 420, so bin (4) should be 420
   passed = Test::Equal(histo->GetBinContent(4), 420.0) && passed;

   util::plotting::Plot(histo, "1D histogram test", "E_{r} / GeV",
         "Events", "ereco.pdf");

   delete histo;

   return passed;
}
//____________________________________________________________________________
void UtlBinningTest::Setup()
{
   // Retrieve the location of the Mondrian XML file
   std::string filename = _arg->GetString("binning-file");
   std::string fullFilename = util::file::GetConfigurationFilename(filename);

   ModeList ml(util::CompleteConfigPath("modelists/UnOscLowE.xml"));

   //_binning = new SingleSampleKineBinning(conv::kdDUNEFD1ph, conv::kbLBNF_FHC,
   //     conv::ksNuMuCC,ml.Modes());
   _binning = new SingleSampleKineBinning(conv::kdSBND, conv::kbBNB_FHC,
					  conv::ksNearNuMuCC, ml.Modes());
   _binning->Read(fullFilename);

   const VALORBinning& recoOptBinning = *(_binning->RecoOptBinning());
   const VALORBinning& recoSmpBinning = *(_binning->RecoSmpBinning());
   const VALORBinning& trueBinning    = *(_binning->TrueSmpBinning(0));

   _vOpt = new VALORArray(recoOptBinning.NBins(), trueBinning.NBins());
   for(unsigned int i = 0; i < recoOptBinning.NBins(); ++i)
   {
      for(unsigned int j = 0; j < trueBinning.NBins(); ++j)
      {
         _vOpt->SetEntry(j * 10 + (i + 1), i, j);
      }
   }

   _vSmp = new VALORArray(recoSmpBinning.NBins(), trueBinning.NBins());
   for(unsigned int i = 0; i < recoSmpBinning.NBins(); ++i)
   {
      for(unsigned int j = 0; j < trueBinning.NBins(); ++j)
      {
         _vSmp->SetEntry(j * 10 + (i + 1), i, j);
      }
   }
}
//____________________________________________________________________________
void UtlBinningTest::Teardown()
{
   delete _binning;
   delete _vOpt;
   delete _vSmp;
}
//____________________________________________________________________________
void UtlBinningTest::AddTests()
{
   UtlBinningTestCase* testCase = new UtlBinningTestCase(this);

   Add(new TestKey("TestMake2DOptimisedHistogram", testCase),
         [testCase](void) { return testCase->TestMake2DOptimisedHistogram(); });
   Add(new TestKey("TestMake2DSimpleHistogram", testCase),
         [testCase](void) { return testCase->TestMake2DSimpleHistogram(); });
   Add(new TestKey("TestMake1DHistogram", testCase),
         [testCase](void) { return testCase->TestMake1DHistogram(); });
}
//____________________________________________________________________________

}
}

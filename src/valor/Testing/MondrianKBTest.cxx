#include "valor/Testing/MondrianKBTest.h"

// valor
#include "valor/Utilities/UtlFile.h"

namespace valor
{
namespace testing
{

bool MondrianKBTestCase::TestContract()
{
   KBCubHcmbMondrian& binning = *(dynamic_cast<MondrianKBTest*>(_test)->_binning);
   KBCubHcmbMondrian* newBinning = binning.Contract(conv::kspEreco);

   bool passed = true;

   // This is not quite right yet, contraction can produce overlapping bins,
   // need to think about how to handle this
   // May be easiest to just run through the edges, and add them to an ordered
   // set and reconstruct the bins from that
   passed = Test::Equal(newBinning->NBins(), 5) && passed;

   // Check bin edges
   passed = Test::Equal(newBinning->BinMinimum(0, 0), 0.0) &&
      Test::Equal(newBinning->BinMaximum(0, 0), 40.0) && passed;
   passed = Test::Equal(newBinning->BinMinimum(1, 0), 40.0) &&
      Test::Equal(newBinning->BinMaximum(1, 0), 150.0) && passed;
   passed = Test::Equal(newBinning->BinMinimum(2, 0), 40.0) &&
      Test::Equal(newBinning->BinMaximum(2, 0), 180.0) && passed;
   passed = Test::Equal(newBinning->BinMinimum(3, 0), 150.0) &&
      Test::Equal(newBinning->BinMaximum(3, 0), 200.0) && passed;
   passed = Test::Equal(newBinning->BinMinimum(4, 0), 180.0) &&
      Test::Equal(newBinning->BinMaximum(4, 0), 200.0) && passed;

   // Check minima and maxima
   passed = Test::Equal(newBinning->Minimum(0), 0.0) && passed;
   passed = Test::Equal(newBinning->Maximum(0), 200.0) && passed;

   delete newBinning;

   return passed;
}

void MondrianKBTest::Setup()
{
   // Retrieve the location of the Mondrian XML file
   std::string filename = _arg->GetString("reco-binning-file");
   std::string fullFilename = util::file::GetConfigurationFilename(filename);

   _binning = new KBCubHcmbMondrian;
   _binning->Read(fullFilename);
}

void MondrianKBTest::Teardown()
{
   delete _binning;
}

void MondrianKBTest::AddTests()
{
   MondrianKBTestCase* testCase = new MondrianKBTestCase(this);

   Add(new TestKey("TestContract", testCase),
         [testCase](void) { return testCase->TestContract(); });
}

}
}

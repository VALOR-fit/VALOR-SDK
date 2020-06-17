//____________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

// VALOR/SDK
#include "valor/Testing/ConfigurationHandlerTest.h"
#include "valor/Testing/ConfigurationTest.h"
#include "valor/Testing/MondrianKBTest.h"
#include "valor/Testing/PhysParamsTest.h"
#include "valor/Testing/TestSuite.h"
#include "valor/Testing/StandardTests.h"
#include "valor/Testing/CubicEdge2EdgeKBTest.h"
#include "valor/Testing/UtlBinningTest.h"
#include "valor/Testing/UtlTest.h"

namespace valor
{
namespace testing
{
//____________________________________________________________________________
bool BatchTests(CmdLnArgList& arg)
{
   testing::TestSuite suite(arg);

   suite.Add("UtlTest", new UtlTest);
   suite.Add("MondrianKBTest", new MondrianKBTest);
   suite.Add("CubicEdge2EdgeKBTest", new CubicEdge2EdgeKBTest);
   suite.Add("UtlBinningTest", new UtlBinningTest);
   suite.Add("PhysParamsTest", new PhysParamsTest);
   suite.Add("ConfigurationTest", new ConfigurationTest);
   suite.Add("ConfigurationHandlerTest", new ConfigurationHandlerTest);

   return suite.Run();
}
//____________________________________________________________________________

}
}

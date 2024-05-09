#include "valor/Utilities/UtlInit.h"

// valor
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/Env.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/UtlFile.h"
#include "valor/Utilities/UtlStyle.h"

namespace valor
{
namespace util
{
namespace init
{

CmdLnArgList Init(const std::string& appArgs,
      const std::string& seedArgs, const int argc, const char* argv[])
{
   PrintBanner(Env::Instance()->Banner(), 500);
  
   CmdLnArgList arg;
   arg.Read(argc, argv, (file::GetConfigurationFilename(appArgs) +
            ":" + file::GetConfigurationFilename(seedArgs)).c_str());

   valor::Messenger::Instance()->Configure(file::GetConfigurationFilename(
					   arg.GetString("mesg-thresholds")));

   // Set the random seeds
   Rnd::Instance()->SetSeeds(arg);

   return arg;
}

}
}
}


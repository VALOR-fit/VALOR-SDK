#ifndef valor_genie_includes
#define valor_genie_includes

#include "dune/Conventions/DuneBuild.h"

#if GENIE_VERSION >= 3
#include "Framework/Conventions/GBuild.h"
#include "Framework/Conventions/Units.h"
#include "Framework/EventGen/EventRecord.h"
#include "Framework/GHEP/GHepParticle.h"
#include "Framework/ParticleData/PDGCodes.h"
#include "Framework/ParticleData/PDGLibrary.h"
#include "Framework/ParticleData/PDGUtils.h"
//~ #include "Framework/Messenger/Messenger.h"
#include "Framework/Ntuple/NtpMCEventRecord.h"
#include "Framework/Ntuple/NtpMCFormat.h"
#include "Framework/Ntuple/NtpMCTreeHeader.h"
#include "Framework/Utils/AppInit.h"
#include "Framework/Utils/CmdLnArgParser.h"
#include "Framework/Utils/RunOpt.h"

//Reweight
#include "RwFramework/GReWeight.h"
#include "RwFramework/GReWeightI.h"
#include "RwFramework/GSyst.h"
#include "RwFramework/GSystSet.h"
#include "RwFramework/GSystUncertainty.h"

#include "RwCalculators/GReWeightINuke.h"


#else //genie 2

#include "Conventions/GBuild.h"
#include "Conventions/Units.h"
#include "EVGCore/EventRecord.h"
#include "GHEP/GHepParticle.h"
#include "PDG/PDGCodes.h"
#include "PDG/PDGLibrary.h"
#include "PDG/PDGUtils.h"
//~ #include "Messenger/Messenger.h"
#include "Ntuple/NtpMCEventRecord.h"
#include "Ntuple/NtpMCFormat.h"
#include "Ntuple/NtpMCTreeHeader.h"
#include "Utils/AppInit.h"
#include "Utils/CmdLnArgParser.h"
#include "Utils/RunOpt.h"
#include "ReWeight/GReWeight.h"
#include "ReWeight/GReWeightI.h"
#include "ReWeight/GReWeightINuke.h"
#include "ReWeight/GSyst.h"
#include "ReWeight/GSystSet.h"
#include "ReWeight/GSystUncertainty.h"

#endif


#endif // valor_genie_includes

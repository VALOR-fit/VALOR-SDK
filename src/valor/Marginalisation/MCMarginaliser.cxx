#include "valor/Marginalisation/MCMarginaliser.h"
#include "valor/Messenger/Messenger.h"
#include "valor/FitCore/FitDataABC.h"
#include "valor/FitCore/FitModelABC.h"
#include "valor/FitCore/FitParamList.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/XMLConfig.h"

// Temp include - can remove when generate doesn't depend on TMath.Pi()
#include "TMath.h"

namespace valor
{
namespace marginalisation
{

MCMarginaliser::MCMarginaliser(FitModelABC* model, FitParamList* parameters,
      util::IModelCache* cache):
   fNumToys(0), fParams(parameters), fCache(cache), fCurrentToy(0), fModel(model)
{
}

MCMarginaliser::~MCMarginaliser()
{
}

void MCMarginaliser::Marginalise()
{
   Rnd::Instance()->SetSeeds(*fArg);

   for(int n = 0; n < fNumToys; ++n)
   {
      LOG("MCMarginaliser", pNOTICE) << "Generating toy " << (n + 1) <<
         " of " << fNumToys;
      RandomizeNuisance();
      fModel->Generate(*fParams, "+SB+SYS");
      fModel->Cache(fCache);
   }
}

FitModelABC* MCMarginaliser::Begin()
{
   fCurrentToy = 0;
   fCache->ReadFileHeader();
   fModel->Retrieve(fCache);
   return fModel;
}

FitModelABC* MCMarginaliser::End()
{
   return 0;
}

FitModelABC* MCMarginaliser::Next()
{
   if(++fCurrentToy < fNumToys)
   {
      fModel->Retrieve(fCache);
      return fModel;
   }
   else
   {
      return 0;
   }
}

int MCMarginaliser::GetNumToys() const
{
   return fNumToys;
}

void MCMarginaliser::Configure(const CmdLnArgList& arg)
{
   fArg = &arg;
   fNumToys = arg.GetInt("num-marginalisation-toys");
   if(fNumToys <= 0)
   {
      LOG("MCMarginaliser", pERROR) <<
         "Number of marginalisation toys must be positive";   
   }
}

void MCMarginaliser::RandomizeNuisance()
{
   TString options(fArg->GetString("param-values-option"));

   // Loop until all parameters are within physical limits
   do
   {
      fModel->ResetSystematics("all");
      fModel->RandomizeSystematics(options.Data());
      PhysParamList tweaks = fModel->Tweaks();
      fParams->FromPhysParamList(tweaks, "current");
   }
   while(!AllParametersWithinBounds());
}

bool MCMarginaliser::AllParametersWithinBounds()
{
   for(int i = 0; i < fParams->Size(); ++i)
   {
      conv::PhysParamId_t p = fParams->WhichParam(i);
      if(fParams->CurrValue(p) > fParams->MaxValue(p) ||
         fParams->CurrValue(p) < fParams->MinValue(p))
      {
         LOG("MCMarginaliser", pDEBUG) << "Nuisance " <<
            util::NameRegistry::Instance()->AsString(p) << " = " <<
            fParams->CurrValue(p) << " from [" <<
            fParams->MinValue(p) << "," <<
            fParams->MaxValue(p)  << "]";
         return false;
      }
   }

   return true;
} 

}
}

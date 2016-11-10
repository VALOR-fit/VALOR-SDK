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

MCMarginaliser::MCMarginaliser(FitModelABC* model, FitParamList* parameters):
   fNumToys(0), fModel(model), fParams(parameters)
{
}

MCMarginaliser::~MCMarginaliser()
{
   std::vector<FitModelABC*>::iterator it = fToys.begin();
   while(it != fToys.end())
   {
      delete *it;
      it = fToys.erase(it);
   }
}

void MCMarginaliser::Marginalise(FitModelABC* model)
{
   Rnd::Instance()->SetSeeds(*fArg);
   TString tag(fArg->GetString("tag"));

   for(int n = 0; n < fNumToys; ++n)
   {
      LOG("MCMarginaliser", pNOTICE) << "Generating toy " << (n + 1) <<
         " of " << fNumToys;
      //RandomizeNuisance();
      model->GenerateUnoscillated(*fParams, "+SB");

      // Temporary in memory storage
      fToys.push_back(model);
      TString filename = TString::Format("%s_%d.root", tag.Data(), n);
      model->Write(filename.Data());
   }
}

std::vector<FitModelABC*>::iterator MCMarginaliser::Begin()
{
   return fToys.begin();
}

std::vector<FitModelABC*>::iterator MCMarginaliser::End()
{
   return fToys.end();
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

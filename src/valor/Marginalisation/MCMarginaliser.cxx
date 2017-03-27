#include "valor/Marginalisation/MCMarginaliser.h"
#include "valor/Messenger/Messenger.h"
#include "valor/FitCore/FitDataABC.h"
#include "valor/FitCore/FitModelABC.h"
#include "valor/FitCore/FitParamList.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/XMLConfig.h"
#include "valor/Utilities/ReadableCache.h"

#include <sstream>
#include <algorithm>

#include "TFile.h"
#include "TH1D.h"

namespace valor
{
namespace marginalisation
{

MCMarginaliser::MCMarginaliser(FitModelABC* model, FitParamList* parameters):
   fNumToys(0), fParams(parameters), fCurrentToy(0), fModel(model)
{
}

MCMarginaliser::~MCMarginaliser()
{
}

void MCMarginaliser::Marginalise()
{
   Rnd::Instance()->SetSeeds(*fArg);
   std::ostringstream ss;
   ss << outputDir << "sys.params.root";
   TFile file(ss.str().c_str(), "RECREATE");

   // Histogram to store the distribution of parameter values in marginalisation
   // toys
   TH1D* hParams = new TH1D[fParams->Size()];
   // Choose a number of bins appropriate to the number of toys
   // fNumToys / 6 is effective for 30 toy test runs, whilst 100 makes sense for
   // a full 10K toy run
   int nbins = std::min(fNumToys / 6, 25);
   for(int i = 0; i < fParams->Size(); ++i)
   {
      conv::PhysParamId_t p = fParams->WhichParam(i);
      std::string name = util::NameRegistry::Instance()->AsString(p);
      hParams[i] = TH1D(name.c_str(), name.c_str(), nbins,
            fParams->MinValue(p), fParams->MaxValue(p));
   }

   for(int n = 0; n < fNumToys; ++n)
   {
      LOG("MCMarginaliser", pNOTICE) << "Generating toy " << (n + 1) <<
         " of " << fNumToys;
      RandomizeNuisance();
      fModel->Generate(*fParams, "+SB+SYS");
      fModel->Cache();
      if(n % 10 == 0)
      {
         for(int i = 0; i < fParams->Size(); ++i)
         {
   //         conv::PhysParamId_t p = fParams->WhichParam(i);
   //         std::string name = util::NameRegistry::Instance()->AsString(p);
   //         hParam = dynamic_cast<TH1D*>(file.Get(name.c_str()));
            hParams[i].Fill(fParams->CurrValueByIndex(i));
   //         hParam->Write("", TObject::kOverwrite);
         }
      }
   }

   file.Write();
   file.Close();
   delete[] hParams;
}

FitModelABC* MCMarginaliser::Begin()
{
   // Temporary reset of cache file offsets etc - once cache is implemented
   // in full this will happen transparently
   util::ReadableCache::Instance()->Reset();
   fCurrentToy = 0;
   return fModel->Retrieve();
}

FitModelABC* MCMarginaliser::End()
{
   return 0;
}

FitModelABC* MCMarginaliser::Next()
{
   if(++fCurrentToy < fNumToys)
   {
      return fModel->Retrieve();
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
   std::string ext = "margtoys.valor";
   outputDir = arg.GetString("marginalisation-toys");
   outputDir = outputDir.substr(0, outputDir.size() - ext.size());
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

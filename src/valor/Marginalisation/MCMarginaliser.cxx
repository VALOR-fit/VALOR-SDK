#include "valor/Marginalisation/MCMarginaliser.h"
#include "valor/Messenger/Messenger.h"
#include "valor/FitCore/FitDataABC.h"
#include "valor/FitCore/FitModelABC.h"
#include "valor/FitCore/FitParamList.h"
#include "valor/Utilities/Rnd.h"

namespace valor
{
namespace marginalisation
{

MCMarginaliser::MCMarginaliser(FitInputs* fitInputs):
   fFirstRun(true), fNumToys(0), fFitInputs(fitInputs)
{
}

double MCMarginaliser::Marginalise()
{
   RandomizeNuisance();
   FitDataABC& data = fFitInputs->Observation();
   FitModelABC& model = fFitInputs->Expectation();
   FitParamList& fp = fFitInputs->FitParams();
   double totalLikelihood = 0.0;
   for(int i = 0; i < fNumToys; ++i)
   {
      totalLikelihood += exp(-data.GetFitStatistic(&model, fp, NULL) / 2);
   }

   return totalLikelihood / fNumToys;
}

void MCMarginaliser::Configure(const CmdLnArgList& arg)
{
   fArg = &arg;
   fNumToys = arg.GetInt("num-marginalisation-toys");
   if(fNumToys <= 0)
   {
      LOG("valor_dune", pERROR) <<
         "Number of marginalisation toys must be positive";   
   }
}

void MCMarginaliser::RandomizeNuisance()
{
   Rnd::Instance()->SetSeeds(*fArg);

   // Do randomisation here   
}

}
}

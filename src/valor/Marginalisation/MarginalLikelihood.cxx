#include "valor/Marginalisation/MarginalLikelihood.h"
#include "valor/Marginalisation/MCMarginaliser.h"
#include "valor/Messenger/Messenger.h"

#include "TMath.h"

namespace valor
{
namespace marginalisation
{

MarginalLikelihood::MarginalLikelihood(MarginaliserABC* marginaliser) :
   LikelihoodABC(), fMarginaliser(marginaliser)
{
}

double MarginalLikelihood::Likelihood(FitDataABC* data, FitParamList* params)
{
   PhysHypothesis hypothesis(*params);
   double totalLikelihood = 0;
   MCMarginaliser* marginaliser = dynamic_cast<MCMarginaliser*>(fMarginaliser);
   assert(marginaliser);
   std::vector<FitModelABC*>::iterator it;
   int i = 0;
   for(it = marginaliser->Begin(); it != marginaliser->End(); ++it)
   {
      i++;
      FitModelABC* model = *it;
      // Apply oscillation to the marginalisation toy
      model->Oscillate(hypothesis, NULL);
      // Calculate the likelihood
      double chi2 = data->GetFitStatistic(model, NULL);
      totalLikelihood += exp(-chi2 / 2);
      LOG("MarginalLikelihood", pNOTICE) << "Toy " << i << " FP: " <<
         params->CurrValueByIndex(0) << " Chi2: " << chi2;
   }
   double averageLikelihood = totalLikelihood / marginaliser->GetNumToys();
   double lnLikelihood = -2 * log(averageLikelihood + 1);
   LOG("MarginalLikelihood", pNOTICE) << "FP: " <<
         params->CurrValueByIndex(0) << " -2lnL: " << lnLikelihood;

   return lnLikelihood;
}

}
}

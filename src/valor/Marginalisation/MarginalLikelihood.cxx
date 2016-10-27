#include "valor/Marginalisation/MarginalLikelihood.h"
#include "valor/Marginalisation/MCMarginaliser.h"

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
   double totalLikelihood = 0;
   MCMarginaliser* marginaliser = dynamic_cast<MCMarginaliser*>(fMarginaliser);
   assert(marginaliser);
   std::vector<FitModelABC*>::iterator it; 
   for(it = marginaliser->Begin(); it != marginaliser->End(); ++it)
   {
      FitModelABC* model = *it;
      // Calculate the likelihood
      double chi2 = data->GetFitStatistic(model, *params, NULL);
      totalLikelihood += exp(-chi2 / 2);
   }
   double averageLikelihood = totalLikelihood / marginaliser->GetNumToys();

   return averageLikelihood;
}

}
}

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
   MCMarginaliser* marginaliser = dynamic_cast<MCMarginaliser*>(fMarginaliser);
   assert(marginaliser);
   std::vector<double> chi2Values;
   int i = 0;
   std::vector<FitModelABC*>::iterator it = marginaliser->Begin();
   // Calculate probability once for a given set of oscillation parameters
   double pOsc = (*it)->Oscillate(hypothesis, NULL);
   for( ; it != marginaliser->End(); ++it)
   {
      i++;
      FitModelABC* model = *it;
      // Apply oscillation to the marginalisation toy
      // Need a function here that actually applies the oscillation probability
      // to the model, having already calculated the probability
      //model->Oscillate(hypothesis, NULL);
      // Calculate the likelihood
      double chi2 = data->GetFitStatistic(model, NULL);
      chi2Values.push_back(chi2);
   }
   std::sort(chi2Values.begin(), chi2Values.end());
   double avgChi2 = 0.5 * (chi2Values[0] + chi2Values[chi2Values.size() - 1]);
   double halfAvgChi2 = 0.5 * avgChi2;
   double scale = halfAvgChi2 <= PRECISION_SCALE ? halfAvgChi2 : PRECISION_SCALE;
   double totalLikelihood = 0;
   for(std::vector<double>::iterator it = chi2Values.begin();
         it != chi2Values.end(); ++it)
   {
      double likelihood = exp(-0.5 * *it + scale);
      totalLikelihood += likelihood;
      LOG("MarginalLikelihood", pNOTICE) << "Toy " << i << " FP: " <<
         params->CurrValueByIndex(0) << " Chi2: " << *it << " L: " << likelihood;
   }

   double averageLikelihood = totalLikelihood / marginaliser->GetNumToys();
   double lnLikelihood = 2 * scale - 2 * log(averageLikelihood);
   LOG("MarginalLikelihood", pNOTICE) << "FP: " <<
         params->CurrValueByIndex(0) << " -2lnL: " << lnLikelihood;

   return lnLikelihood;
}

}
}

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
   FitParamList* free = params->FreeParameterSubset();
   for(FitModelABC* m = marginaliser->Begin() ; m != marginaliser->End();
         m = marginaliser->Next())
   {
      // Apply oscillation to the marginalisation toy
      m->Generate(hypothesis, "+OSC");
      // Calculate the likelihood
      double chi2 = data->GetFitStatistic(m, NULL);
      chi2Values.push_back(chi2);
   }

   std::sort(chi2Values.begin(), chi2Values.end());
   double avgChi2 = 0.5 * (chi2Values[0] + chi2Values[chi2Values.size() - 1]);
   double halfAvgChi2 = 0.5 * avgChi2;
   double scale = halfAvgChi2 <= PRECISION_SCALE ? halfAvgChi2 : PRECISION_SCALE;
   double totalLikelihood = 0;
   int i = 0;
   for(std::vector<double>::iterator it = chi2Values.begin();
         it != chi2Values.end(); ++it, i++)
   {
      double likelihood = exp(-0.5 * *it + scale);
      totalLikelihood += likelihood;
   }

   double averageLikelihood = totalLikelihood / marginaliser->GetNumToys();
   double lnLikelihood = 2 * scale - 2 * log(averageLikelihood);
   LOG("MarginalLikelihood", pNOTICE) << "FP: " << free->CurrValueByIndex(0) <<
      " L: " << averageLikelihood << " -2lnL: " << lnLikelihood;

   delete free;

   return lnLikelihood;
}

}
}

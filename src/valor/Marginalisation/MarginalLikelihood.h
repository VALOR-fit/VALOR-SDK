#ifndef MARGINALLIKELIHOOD_H
#define MARGINALLIKELIHOOD_H

#include "valor/FitCore/LikelihoodABC.h"
#include "valor/Marginalisation/MarginaliserABC.h"

namespace valor
{
namespace marginalisation
{

   class MarginalLikelihood : LikelihoodABC
   {
   public:
      MarginalLikelihood(MarginaliserABC* marginaliser);
      virtual ~MarginalLikelihood(){ };

      /**
         Computes the marginal likelihood.
     
         \param data The data against which the model is to be compared.
         \param params The set of oscillation parameters to fit.
      */
      virtual double Likelihood(FitDataABC* data, FitParamList* params);

   private:
      MarginaliserABC* fMarginaliser;
   };

}
}

#endif

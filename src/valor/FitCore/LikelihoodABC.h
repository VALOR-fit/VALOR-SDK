#ifndef LIKELIHOODABC_H
#define LIKELIHOODABC_H

#include <string>

#include "valor/FitCore/FitDataABC.h"
#include "valor/FitCore/FitParamList.h"

namespace valor
{
   class LikelihoodABC
   {
   public:
      virtual ~LikelihoodABC(){ };

      /**
         Computes the likelihood.

         \param data The data against which the model is to be compared.
         \param params The set of oscillation parameters to fit.
      */
      virtual double Likelihood(FitDataABC* data, FitParamList* params) = 0;

      /**
         Enables validation.

         \param filename
            The file to which validation information will be written.
      */
      virtual void EnableValidation(std::string filename) = 0;

   protected:
      LikelihoodABC() {}
   };
}

#endif

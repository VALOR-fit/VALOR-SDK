#ifndef MARGINALLIKELIHOOD_H
#define MARGINALLIKELIHOOD_H

#include "valor/FitCore/LikelihoodABC.h"
#include "valor/Marginalisation/MarginaliserABC.h"

namespace valor
{
namespace marginalisation
{

   class MarginalLikelihood : public LikelihoodABC
   {
   public:
      MarginalLikelihood(MarginaliserABC* marginaliser);
      virtual ~MarginalLikelihood();

      /**
         Computes the marginal likelihood.
     
         \param data The data against which the model is to be compared.
         \param params The set of oscillation parameters to fit.
      */
      virtual double Likelihood(FitDataABC* data, FitParamList* params);

      /**
         Enables validation.

         \param filename
            The file to which validation information will be written.
      */
      virtual void EnableValidation(std::string filename);

   private:
      void PreCalcMarginalValues(FitParamList& fp);

   private:
      MarginaliserABC* fMarginaliser;
      const double PRECISION_SCALE = 80;
      std::map<conv::PhysParamId_t, std::vector<double>*> marginalValues;
      bool computedMarginalValues;
      std::string _validationFilename;
      bool _validate;

      struct marginalValueDeleter
      {
         template <typename T>
         void operator()(const T& x) const
         {
            delete x.second;
         }
      };
   };
}
}

#endif

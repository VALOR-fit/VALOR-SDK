#ifndef MCMARGINALISER_H
#define MCMARGINALISER_H

#include "valor/Conventions/PhysParamId.h"
#include "valor/Marginalisation/MarginaliserABC.h"
#include "valor/FitCore/FitInputs.h"
#include "valor/VArray/VALORArrayD.h"

namespace valor
{
namespace marginalisation
{
   class MCMarginaliser : public MarginaliserABC
   {
   public:
      MCMarginaliser(FitInputs* fitInputs);
      MCMarginaliser(const MCMarginaliser& original);
      virtual ~MCMarginaliser();

      /**
         Marginalises over nuisance parameters.

         \return The average likelihood over the marginalisation toys.
      */
      virtual double Marginalise();

      /**
         Configures the marginaliser.

         \param arg
            The CmdLnArgList from which configuration parameters are to be read.
      */
      virtual void Configure(const CmdLnArgList& arg);

   private:
      void RandomizeNuisance();
      double Marginalise1D();
      double Marginalise2D();
      conv::PhysParamId_t GetPhysParamId(const std::string& name);

   private:
      int fNumToys;
      int fNumDimensions;
      conv::PhysParamId_t fParam0;
      conv::PhysParamId_t fParam1;
      FitInputs* fFitInputs;
      VALORArrayD* fLikelihood;
   };
}
}

#endif

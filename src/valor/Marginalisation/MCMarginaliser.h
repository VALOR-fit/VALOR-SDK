#ifndef MCMARGINALISER_H
#define MCMARGINALISER_H

#include "valor/Marginalisation/MarginaliserABC.h"
#include "valor/FitCore/FitInputs.h"

namespace valor
{
namespace marginalisation
{
   class MCMarginaliser : public MarginaliserABC
   {
   public:
      MCMarginaliser(FitInputs* fitInputs);
      virtual ~MCMarginaliser() {};

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

   private:
      bool fFirstRun;
      int fNumToys;
      FitInputs* fFitInputs;
   };
}
}

#endif

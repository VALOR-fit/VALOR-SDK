#ifndef MCMARGINALISER_H
#define MCMARGINALISER_H

#include "valor/Conventions/PhysParamId.h"
#include "valor/Marginalisation/MarginaliserABC.h"
#include "valor/FitCore/FitModelABC.h"
#include "valor/FitCore/FitParamList.h"

namespace valor
{
namespace marginalisation
{
   class MCMarginaliser : public MarginaliserABC
   {
   public:
      MCMarginaliser(FitModelABC* model, FitParamList* parameters);
      virtual ~MCMarginaliser();

      /**
         Marginalises over nuisance parameters.
      */
      virtual void Marginalise();

      /**
         The first marginalisation toy in the set.

         \return The first marginalisation toy.
      */
      virtual FitModelABC* Begin();

      /**
         The terminating marginalisation toy in the set.

         \return The terminating marginalisation toy.
      */
      virtual FitModelABC* End();

      /**
         The next marginalisation toy in the set.

         \return The next marginalisation toy.
      */
      virtual FitModelABC* Next();

      /**
         Returns the number of marginalisation toys.

         \return The number of marginalisation toys.
      */
      int GetNumToys() const;

      /**
         Configures the marginaliser.

         \param arg
            The CmdLnArgList from which configuration parameters are to be read.
      */
      virtual void Configure(const CmdLnArgList& arg);

   private:
      void RandomizeNuisance();
      bool AllParametersWithinBounds();

   private:
      int fNumToys;
      FitParamList* fParams;
      int fCurrentToy;
      FitModelABC* fModel;
   };
}
}

#endif

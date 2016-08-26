#ifndef MARGINALISERABC_H
#define MARGINALISERABC_H

#include "valor/CmdLine/CmdLnArgList.h"
#include "valor/VArray/VALORArrayD.h"

namespace valor
{
namespace marginalisation
{
   class MarginaliserABC
   {
   public:
      virtual ~MarginaliserABC(){ };

      /**
         Marginalises over nuisance parameters.
      */
      virtual void Marginalise() = 0;

      /**
         Retrieves the average likelihood calculated during a 1D
         marginalisation.

         \return The average likelihood over the marginalization toys.
      */
      virtual TH1D* GetAverageLikelihood1D() = 0;

      /**
         Retrieves the average likelihood calculated during a 2D
         marginalisation.

         \return The average likelihood over the marginalization toys.
      */
      virtual TH2D* GetAverageLikelihood2D() = 0;

      /**
         Configures the marginaliser.

         \param arg
            The CmdLnArgList from which configuration parameters are to be read.
      */
      virtual void Configure(const CmdLnArgList& arg) = 0;

   protected:
      MarginaliserABC();

   protected:
      const CmdLnArgList* fArg;
   };
}
}

#endif

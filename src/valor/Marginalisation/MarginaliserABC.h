#ifndef MARGINALISERABC_H
#define MARGINALISERABC_H

#include "valor/CmdLine/CmdLnArgList.h"
#include "valor/FitCore/FitModelABC.h"

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
         The first marginalisation toy in the set.

         \return The first marginalisation toy.
      */
      virtual FitModelABC* Begin() = 0;

      /**
         The terminating marginalisation toy in the set.

         \return The terminating marginalisation toy.
      */
      virtual FitModelABC* End() = 0;

      /**
         The next marginalisation toy in the set.

         \return The next marginalisation toy.
      */
      virtual FitModelABC* Next() = 0;

      /**
         Configures the marginaliser.

         \param arg
            The CmdLnArgList from which configuration parameters are to be read.
      */
      virtual void Configure(const CmdLnArgList& arg) = 0;

   protected:
      MarginaliserABC() : fArg(0) {}

   protected:
      const CmdLnArgList* fArg;
   };
}
}

#endif

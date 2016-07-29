#ifndef MARGINALISERABC_H
#define MARGINALISERABC_H

#include "valor/CmdLine/CmdLnArgList.h"

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

         \return The average likelihood over the marginalization toys.
      */
      virtual double Marginalise() = 0;

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

#ifndef MARGINALISATIONABC_H
#define MARGINALISATIONABC_H

namespace valor
{
namespace marginalisation
{
   class MarginalisationABC
   {
   public:
      virtual ~MarginalisationABC() {};

      /**
         Marginalises over nuisance parameters.

         \return The average likelihood over the marginalization toys.
      */
      virtual double Marginalise() = 0;
   protected:
      MarginalisationABC();
   };
}
}

#endif

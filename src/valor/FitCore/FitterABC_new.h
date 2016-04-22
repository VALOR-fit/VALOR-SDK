//____________________________________________________________________________
/*!

\class     valor::FitterABC

\brief     Pure abstract base class (pABC) for VALOR fitters.

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   July 05, 2011

\copyright (c) 2010-2016, The VALOR group

*/
//____________________________________________________________________________

#ifndef _valor_fitter_abc_new_h_
#define _valor_fitter_abc_new_h_

#include <TString.h>

#include "valor/Registry/Registry.h"

namespace valor {

class FitInputs;

class FitterABC_new
{
public:
   virtual ~FitterABC_new();

   virtual bool Configure (const Registry & conf)  = 0;
/*
the above replaces:

   virtual void       SetTol        (double tol)             = 0;
   virtual void       SetStr        (int str   )             = 0;

and many other optiosn passed through the ctor 
and is a uniform interface that can be used with any fitter
*/

   virtual bool DoTheFit (FitInputs * inputs) = 0; 



   virtual const Registry & FitInfo(void) const = 0;

/*
the above replaces:

   virtual int        FitStatusCode (Option_t * option = "") = 0; 
   virtual double     FitTime       (void)                   = 0;
   virtual int        FitNTries     (void)                   = 0;
   virtual int        NDOF          (void)                   = 0;
   virtual int        NBinsInFit    (void)                   = 0;
   virtual int        NFitParams    (void)                   = 0;
*/

protected:
   FitterABC_new();
};

} // valor

#endif


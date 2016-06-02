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

#ifndef _valor_fitter_abc_h_
#define _valor_fitter_abc_h_

#include <TString.h>
#include "valor/Registry/Registry.h"

namespace valor {

class FitterABC
{
public:
   virtual ~FitterABC();

   virtual bool       Configure     (const Registry & conf)        = 0;
   virtual bool       DoTheFit      (void)                         = 0; 
   virtual int        FitStatusCode (Option_t * option = "") const = 0; 
   virtual double     FitTime       (void) const                   = 0;
   virtual int        FitNTries     (void) const                   = 0;
   virtual int        NDOF          (void) const                   = 0;
   virtual int        NBinsInFit    (void) const                   = 0;
   virtual int        NFitParams    (void) const                   = 0;
   virtual void       SetTol        (double tol)                   = 0;
   virtual void       SetStr        (int str   )                   = 0;

protected:
   FitterABC();
};

} // valor

#endif


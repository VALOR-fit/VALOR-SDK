//____________________________________________________________________________
/*!

\class     valor::FitterABC

\brief     Pure abstract base class (pABC) for VALOR fitters.

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   July 05, 2011

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_fitter_abc_h_
#define _valor_fitter_abc_h_

#include <TString.h>

#include "valor/Registry/Registry.h"

namespace valor {

class Likelihood;

class FitterABC
{
public:
   virtual ~FitterABC();

   virtual bool Configure (const Registry & conf)  = 0;
/*
the above replaces:

   virtual void       SetTol        (double tol)             = 0;
   virtual void       SetStr        (int str   )             = 0;

and many other optiosn passed through the ctor
and is a uniform interface that can be used with any fitter
*/

  //virtual bool DoTheFit (Likelihood * likelihood) = 0;
  virtual bool DoTheFit (void) = 0;


  /*virtual const Registry & FitInfo(void) const = 0;*/
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
   FitterABC();
};

} // valor

#endif

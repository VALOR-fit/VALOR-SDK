//____________________________________________________________________________
/*!

\class    valor::FitterMinuit2

\brief    T2KAna fitter based on Minuit2

\author   Thomas Dealtry <thomas.dealtry \at physics.ox.ac.uk>
          STFC, Rutherford Appleton Laboratory
	  Subdepartment of Particle Physics, University of Oxford

\created  October 21, 2012

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved
*/
//____________________________________________________________________________

#ifndef _valor_fitter_minuit2_h_
#define _valor_fitter_minuit2_h_

#include <TString.h>
#include <TObject.h>
#include <TMatrixD.h>
#include <Minuit2/MnUserCovariance.h>
#include <Minuit2/FunctionMinimum.h>

//#include "valor/Conventions.h"
#include "valor/Core/FitterABC.h"
#include "valor/Core/FitInputs.h"

namespace valor {

class FitterMinuit2: public FitterABC
{
public:
   FitterMinuit2();
   FitterMinuit2(FitInputs * inputs, Option_t * option = "minuit2");
   virtual ~FitterMinuit2();

   virtual bool       DoTheFit      (void);
   virtual int        FitStatusCode (Option_t * option = "");
   virtual double     FitTime       (void)  { return fFitTime;                    }
   virtual int        FitNTries     (void)  { return fFitTries;               }
   virtual int        NDOF          (void)  { return fNFitBins - fNFreeFitParams; }
   virtual int        NBinsInFit    (void)  { return fNFitBins;                   }
   virtual int        NFitParams    (void)  { return fNFreeFitParams;             }
   virtual void       SetTol        (double /*tol*/) {}
   virtual void       SetStr        (int    /*str*/) {}

protected:

   void  Init    (void);
   void  Init    (FitInputs * fit_inputs, Option_t * option = "minuit2");
   bool  RunOnce (void);
   void  ConstructCovMatrix(void);
   void  GetStatusCode(ROOT::Minuit2::FunctionMinimum bestfit);

   FitInputs*  fFitInputs;        ///< fit inputs
   TString     fOption;           ///< fit option
   int         fStatusCode;       ///< fit status code
   double      fFitTime;          ///< fit time
   int         fFitTries;          ///< fit time
   int         fNFitBins;         ///< number of bins in fit
   int         fNFreeFitParams;   ///< number of free params in fit
   TMatrixD*   fCovMtrx;          ///< covariance matrix of all free parameters in the fit

   ROOT::Minuit2::MnUserParameters fUserParams;   ///< store the user parameters
};


} // valor

#endif


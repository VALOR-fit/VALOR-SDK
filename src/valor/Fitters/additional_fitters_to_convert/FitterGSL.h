//____________________________________________________________________________
/*!

\class    valor::FitterGSL

\brief    T2KAna fitter based on GSL implementation of the following algoritms:

          ....................................................................
           Algorithm name                                 | GSL fitter option
          ....................................................................
           Simulated annealing                            | SimAnneal
           Levemberg-Marquardt                            | LevMarq
           Fletcher-Reeves conjugate gradient             | ConjugateFR
           Polak-Ribiere conjugate gradient               | ConjugatePR
           Vector Broyden-Fletcher-Goldfarb-Shanno (BFGS) | VectorBFGS
           Steepest Descent                               | SteepestDescent
          ....................................................................

\reference
          http://www.gnu.org/software/gsl/manual/html_node/Multidimensional-Minimization.html

\author   Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
          STFC, Rutherford Appleton Laboratory

\created  March 06, 2013

*/
//____________________________________________________________________________

#ifndef _valor_fitter_gsl_h_
#define _valor_fitter_gsl_h_

#include "valor/Conventions/Build.h"

#include <TString.h>
#include <TObject.h>
#include <TMatrixD.h>

#ifdef __VALOR_FMWK_GSL_ENABLED__
#include <Math/Minimizer.h>
#include <Math/IFunction.h>
#endif

#include "valor/Core/FitterABC.h"
#include "valor/Core/FitInputs.h"

namespace valor {

class FitterGSL: public FitterABC
{
public:
   FitterGSL();
   FitterGSL(FitInputs * inputs, Option_t * option);
   virtual ~FitterGSL();

   virtual bool       Configure     (const Registry & /*conf*/) {return true;}
   virtual bool       DoTheFit      (void);
   virtual int        FitStatusCode (Option_t * /*option*/ = "") const {return -999;}
   virtual double     FitTime       (void) const        { return fFitTime;                    }
   virtual int        FitNTries     (void) const        { return fFitTries;                   }
   virtual int        NDOF          (void) const        { return fNFitBins - fNFreeFitParams; }
   virtual int        NBinsInFit    (void) const        { return fNFitBins;                   }
   virtual int        NFitParams    (void) const        { return fNFreeFitParams;             }
   virtual void       SetTol        (double tol)        { fTolerance  = tol ;                 }//Minuit targets an EDM of 0.001*tolerance for chisq dist
   virtual void       SetMaxCalls   (int ncalls)        { fMaxNumCalls = ncalls ;             }
   virtual void       SetStr        (int /*str*/)       { }
   
protected:

   void Init  (void);
   void Init  (FitInputs * fit_inputs, Option_t * option);

   FitInputs*  fFitInputs;        ///< fit inputs
   TString     fOption;           ///< fit option
   int         fStatusCode;       ///< fit status code
   double      fFitTime;          ///< fit time
   int         fFitTries;         ///< fit time
   int         fNFitBins;         ///< number of bins in fit
   int         fNFreeFitParams;   ///< number of free params in fit
   double      fTolerance;        ///< Fit tolerance
   int         fMaxNumCalls;      ///< Max number of calls

#ifdef __VALOR_FMWK_GSL_ENABLED__
   ROOT::Math::IMultiGenFunction * fFunc;
   ROOT::Math::FitMethodFunction * fFitMethodFunc;
   ROOT::Math::Minimizer *         fMinimizer;
#endif
};


} // valor

#endif


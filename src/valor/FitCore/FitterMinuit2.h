//____________________________________________________________________________
/*!

\class    valor::FitterMinuit2

\brief    T2KAna fitter based on Minuit2

\author   Thomas Dealtry <thomas.dealtry \at physics.ox.ac.uk>
          STFC, Rutherford Appleton Laboratory
          Subdepartment of Particle Physics, University of Oxford

          Steve Dennis <s.r.dennis \at liverpool.ac.uk>
          University of Liverpool

\created  October 21, 2012 - major rewrite June 2016

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved
*/
//____________________________________________________________________________

#ifndef _valor_fitter_minuit2_h_
#define _valor_fitter_minuit2_h_

#include <TString.h>
#include <TMatrixD.h>
#include <Minuit2/MnUserCovariance.h>
#include <Minuit2/FunctionMinimum.h>

#include "valor/FitCore/FitterABC.h"

namespace valor {

class Likelihood;

class FitterMinuit2: public FitterABC
{
public:
   FitterMinuit2();
   FitterMinuit2(Likelihood * lkhd, Option_t * option = "minuit2"); // option: "minuit2","minuit2+hesse_err","minuit2+minos_err" (migrad_err by default)
   // Also supported - fumili
   // Options can also specify tolerance=x and max function evaluations, eg
   // option="minuit+hesse+tolerance=0.01+maxcalls=1E6"
   virtual ~FitterMinuit2();

   virtual bool       Configure     (const Registry & conf);
   virtual bool       DoTheFit      (void);
   virtual int        FitStatusCode (Option_t * option = "") const; // option: "migrad_min", "migrad_err", "hesse_err", "minos_err"
   virtual double     FitTime       (void) const        { return fFitTime;                    }
   virtual int        FitNTries     (void) const        { return fFitTries;                   }
   virtual int        NDOF          (void) const;
   virtual int        NBinsInFit    (void) const        { return fNFitBins;                   }
   virtual int        NFitParams    (void) const;
   virtual void       SetTol        (double tol)        { fTol  = tol ;                       }//Minuit targets an EDM of 0.001*tolerance for chisq dist
   virtual void       SetStr        (int str   )        { fStr  = str ;                       }
   virtual void       SetMaxCalls   (int ncalls)        { fMaxNumCalls = ncalls ;             }
   // max number of function calls
  // if zero, max_calls = 200 + 100 * npar + 5 * npar**2

protected:

   void  Init    (void);
   void  Init    (Likelihood * lkhd, Option_t * option = "minuit2");
   bool  RunOnce (void);
   void  FillPriorCovMatrix(Option_t * opt = "");

   void CopyCovarianceToFPL(const ROOT::Minuit2::MnUserCovariance & mncov);

   Likelihood* fLikelihood;          ///< fit inputs
   TString     fOption;              ///< fit option
   int         fMigradMinStatusCode; ///< MIGRAD minimization status code
   int         fMigradErrStatusCode; ///< MIGRAD err status code
   int         fHesseErrStatusCode;  ///< HESSE err status code
   int         fMinosErrStatusCode;  ///< MINOS err status code
   double      fFitTime;             ///< time taken to do the fit
   int         fFitTries;            ///< number of tries taken to do the fit
   int         fNFitBins;            ///< number of bins in fit
   bool        fCalcHesseErrors;     ///< calculate HESSE errors?
   bool        fCalcMinosErrors;     ///< calculate MINOS errors?
   double      fTol;                 ///< set tolerance
   int         fMaxNumCalls;         ///< Maximum number of evaluations of fit function
   int         fStr;                 ///< strategy
   TMatrixD    fCovMtrx;             ///< covariance matrix of all free parameters in the fit

   ROOT::Minuit2::MnUserParameters fUserParams;   ///< store the user parameters
   bool fUseFumili;
};


} // valor

#endif

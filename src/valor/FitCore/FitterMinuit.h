//____________________________________________________________________________
/*!

\class     valor::FitterMinuit

\brief     Minuit-based VALOR fitter.

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
           Thomas Dealtry <Thomas.Dealtry \at physics.ox.ac.uk>
           Universty of Oxford
           Lorena Escudero <loressa \at ific.uv.es>
           IFIC Valencia
           Nick Grant <n.grant \at lancaster.ac.uk>
           University of Lancaster

\created   October 20, 2010

\copyright (c) 2010-2016, The VALOR group

*/
//____________________________________________________________________________

#ifndef _valor_fitter_minuit_h_
#define _valor_fitter_minuit_h_

#include <TMinuit.h>
#include <TString.h>

#include "valor/FitCore/FitterABC.h"

namespace valor {

class FitInputs;

class FitterMinuit: public FitterABC
{
public:
   FitterMinuit();
   FitterMinuit(FitInputs * inputs, Option_t * option = "minuit"); // option: "minuit","minuit+hesse_err","minuit+minos_err" (migrad_err by default)
   // Options can also specify tolerance=x and max function evaluations, eg
   // option="minuit+hesse+tolerance=0.01+maxcalls=1E6"
   virtual ~FitterMinuit();

   virtual bool       Configure     (const Registry & conf);
   virtual bool       DoTheFit      (void);
   virtual int        FitStatusCode (Option_t * option = ""); // option: "migrad_min", "migrad_err", "hesse_err", "minos_err"
   virtual double     FitTime       (void)              { return fFitTime;                    }
   virtual int        FitNTries     (void)              { return fFitTries;                   }
   virtual int        NDOF          (void)              { return fNFitBins - fNFreeFitParams; }
   virtual int        NBinsInFit    (void)              { return fNFitBins;                   }
   virtual int        NFitParams    (void)              { return fNFreeFitParams;             }
   virtual void       SetTol        (double tol)        { fTol  = tol ;                       }//Minuit targets an EDM of 0.001*tolerance for chisq dist
   virtual void       SetStr        (int str   )        { fStr  = str ;                       }
   virtual void       SetMaxCalls   (int ncalls)        { fMaxNumCalls = ncalls ;             }
   
protected:

   void  Init    (void);
   void  Init    (FitInputs * fit_inputs, Option_t * option = "minuit");
   bool  RunOnce (bool use_simplex);

   FitInputs*  fFitInputs;           ///< fit inputs
   TString     fOption;              ///< fit option
   int         fMigradMinStatusCode; ///< MIGRAD minimization status code
   int         fMigradErrStatusCode; ///< MIGRAD err status code
   int         fHesseErrStatusCode;  ///< HESSE err status code
   int         fMinosErrStatusCode;  ///< MINOS err status code
   double      fFitTime;             ///< time taken to do the fit
   int         fFitTries;            ///< number of tries taken to do the fit
   int         fNFitBins;            ///< number of bins in fit
   int         fNFreeFitParams;      ///< number of free params in fit
   bool        fCalcHesseErrors;     ///< calculate HESSE errors?
   bool        fCalcMinosErrors;     ///< calculate MINOS errors?
   double      fTol;                 ///< set tolerance
   int         fStr;                 ///< strategy
   int         fPrintLevel;
   int         fMaxNumCalls;         ///< Maximum number of evaluations of fit function
};

} // valor

#endif


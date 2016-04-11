//____________________________________________________________________________
/*!

\class     valor::VMinuitFitter

\brief     Minuit-based VALOR fitter.

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
           Thomas Dealtry <Thomas.Dealtry \at physics.ox.ac.uk>
           Universty of Oxford
           Lorena Escudero <loressa \at ific.uv.es>
           IFIC Valencia
           Nick Grant <n.grant \at lancaster.ac.uk>
           University of Lancaster

\config    ...................................................
           name         Type     Posible values       Default  
           ...................................................
           Minimizer    string   Migrad,Simplex       Migrad
           ErrType      string   Migrad,Hesse,Minos   Migrad   
           Tolerance    double                        0.01      [1]
           Strategy     int                           1         [2]
           MaxNumCalls  int                           1E+6
           PrintLevel   int                           -1        [2]

           [1] Target EDM = 0.001*tolerance for chisq dist
           [2] See Minuit manual

\created   October 20, 2010

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_minuit_fitter_h_
#define _valor_minuit_fitter_h_

#include <TMinuit.h>
#include <TString.h>

#include "FitCore/FitterABC.h"

namespace valor {

class FitInputs;

class VMinuitFitter: public FitterABC
{
public:
   VMinuitFitter();

   //----------------
   // NOTES: 
   //  - option is redundant - all options set via Configure(const Registry & conf);
   //  - does it need to be initialised with the FitInputs? couldn't pass the inputs in DoTheFit?
   //
   VMinuitFitter(FitInputs * inputs, Option_t * option = "minuit"); // option: "minuit","minuit+hesse_err","minuit+minos_err" (migrad_err by default)
   //----------------

   virtual ~VMinuitFitter();

   //---------------
   // NOTES:
   // Could also add FitterABC::Configure(string xmlfile) to read the registry (used below) from an XML file
   // --------------
   virtual bool    Configure     (const Registry & conf);
   virtual bool    DoTheFit      (void);
   virtual int     FitStatusCode (Option_t * option = ""); // option: "migrad_min", "migrad_err", "hesse_err", "minos_err"

   //---------------
   // NOTES:
   // Guess what is below is generic enough, but could potentially generalize with a
   // const Registry & FitterABC::FitInfo(void) 
   //---------------
   virtual double  FitTime       (void)              { return fFitTime;                    }
   virtual int     FitNTries     (void)              { return fFitTries;                   }
   virtual int     NDOF          (void)              { return fNFitBins - fNFreeFitParams; }
   virtual int     NBinsInFit    (void)              { return fNFitBins;                   }
   virtual int     NFitParams    (void)              { return fNFreeFitParams;             }

   // -------------
   // NOTES:
   // Remove these methods 
   // Greater flexibility configuring the fitter via Configure(const Registry & conf);
   //
   virtual void    SetTol        (double tol)        { fTol  = tol ;                       }
   virtual void    SetStr        (int str   )        { fStr  = str ;                       }
   virtual void    SetMaxCalls   (int ncalls)        { fMaxNumCalls = ncalls ;             }
   // -------------
   
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
   int         fStr;                 ///< set strategy
   int         fPrintLevel;
   int         fMaxNumCalls;
};

} // valor

#endif


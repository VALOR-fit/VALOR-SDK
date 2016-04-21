//____________________________________________________________________________
/*!

\class     valor::MinuitFitter

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

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_minuit_fitter_h_
#define _valor_minuit_fitter_h_

#include <TMinuit.h>
#include <TString.h>

#include "FitCore/FitterABC_new.h"

namespace valor {

class FitInputs;

class MinuitFitter: public FitterABC_new
{
public:
   MinuitFitter();
   virtual ~MinuitFitter();

   //
   // Configuration options:
   // ...................................................
   // name         Type     Posible values       Default  
   // ...................................................
   // Minimizer    string   Migrad,Simplex       Migrad
   // ErrType      string   Migrad,Hesse,Minos   Migrad   
   // Tolerance    double                        0.01      [1]
   // Strategy     int                           1         [2]
   // MaxNumCalls  int                           1E+6
   // PrintLevel   int                           -1        [2]
   //
   // [1] Target EDM = 0.001*tolerance for chisq dist
   // [2] See Minuit manual
   //
   virtual bool Configure (const Registry & conf);


   // As above, but generates the configuration Registry from an XML file.
   // The following schema is used:
   //
   // <valor>
   //    <fitter_config name = "blah">
   //       <param name="Minimizer" type="string> Migrad </param>
   //       <param name="ErrType"   type="string> Hesse  </param>
   //       ...
   //    </fitter_config>
   // </valor>
// virtual bool Configure(string xmlfile);

   //
   //
   virtual bool DoTheFit  (FitInputs * inputs);

   //
   //
   virtual const Registry & FitInfo (void) const;
   
protected:

   void  Init    (void);
   bool  RunOnce (bool use_simplex);

   Registry fConfig;
   Registry fFitInfo;

/*
   int         fMigradMinStatusCode; ///< MIGRAD minimization status code
   int         fMigradErrStatusCode; ///< MIGRAD err status code
   int         fHesseErrStatusCode;  ///< HESSE err status code
   int         fMinosErrStatusCode;  ///< MINOS err status code

   bool        fCalcHesseErrors;     ///< calculate HESSE errors?
   bool        fCalcMinosErrors;     ///< calculate MINOS errors?
   double      fTol;                 ///< set tolerance
   int         fStr;                 ///< set strategy
   int         fPrintLevel;
   int         fMaxNumCalls;
*/

};

} // valor

#endif


#ifndef FITTERMARGINAL_H
#define FITTERMARGINAL_H

#include <TMinuit.h>
#include <TString.h>

#include "valor/FitCore/FitterABC.h"

namespace valor {

class FitInputs;

class FitterMarginal: public FitterABC
{
public:
   FitterMarginal();
   FitterMarginal(FitInputs * inputs, Option_t * option = "minuit");
   virtual ~FitterMarginal();

   virtual bool Configure(const Registry & conf);
   virtual bool DoTheFit(void);
   virtual int FitStatusCode (Option_t * option = "") const;
   virtual double FitTime(void) const { return fFitTime; }
   virtual int FitNTries(void) const { return fFitTries; }
   virtual int NDOF(void) const { return fNFitBins - fNFreeFitParams; }
   virtual int NBinsInFit(void) const { return fNFitBins; }
   virtual int NFitParams(void) const { return fNFreeFitParams; }
   virtual void SetTol(double tol) { fTol = tol; }
   virtual void SetStr(int str) { fStr = str; }
   virtual void SetMaxCalls(int ncalls) { fMaxNumCalls = ncalls; }
   
protected:
   void Init(void);
   void Init(FitInputs * fit_inputs, Option_t * option = "minuit");
   bool RunOnce(bool use_simplex);

   FitInputs* fFitInputs;
   TString fOption;
   int fMigradMinStatusCode;
   int fMigradErrStatusCode;
   int fHesseErrStatusCode;
   int fMinosErrStatusCode;
   double fFitTime;
   int fFitTries;
   int fNFitBins;
   int fNFreeFitParams;
   bool fCalcHesseErrors;
   bool fCalcMinosErrors;
   double fTol;
   int fStr;
   int fPrintLevel;
   int fMaxNumCalls;
};

}

#endif

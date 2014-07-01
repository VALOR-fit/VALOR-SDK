//____________________________________________________________________________
/*!

\class     valor::FitParamList

\brief     List of parameters included in the fit

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   October 20, 2010

\copyright (c) 2010-2013, The VALOR group.

*/
//____________________________________________________________________________

#ifndef _valor_fit_param_list_h_
#define _valor_fit_param_list_h_

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include <TNamed.h>
#include <TObjString.h>
#include <TVirtualFitter.h>
#include <Minuit2/MnUserParameters.h>
#include <Math/Minimizer.h>
#include <TMatrixDSym.h>
#include <Fit/Fitter.h>

#include "Conventions/PhysParamId.h"
#include "Conventions/PhysHypothesisId.h"

#define SYST_NSIGMA   5   // number of sigma away from nominal nuisance parameters are allowed to be
#define SYST_STEP     0.1 // step size (in units of sigma)

using std::string;
using std::map;

namespace valor {

class PhysParamList;

class FitParamList: public TNamed {

public:
   FitParamList();
   FitParamList(int nparams);
   FitParamList(const FitParamList & plst);
  ~FitParamList();

   // Get information for the fit parameters
   int    Size               (void)                         const { return fNParams;  }
   string Name               (valor::conv::PhysParamId_t p) const { return fName[this->Id(p)].GetString().Data();   }
   double BestFitValue       (valor::conv::PhysParamId_t p) const { return fBestFitValue[this->Id(p)]; }
   double BestFitValueByIndex(int i                       ) const { return fBestFitValue[i];           }
   double CurrValue          (valor::conv::PhysParamId_t p) const { return fCurrValue   [this->Id(p)]; }
   double CurrValueByIndex   (int i                       ) const { return fCurrValue   [i];           }
   double InitValue          (valor::conv::PhysParamId_t p) const { return fInitValue   [this->Id(p)]; }
   double MinValue           (valor::conv::PhysParamId_t p) const { return fMinValue    [this->Id(p)]; }
   double MaxValue           (valor::conv::PhysParamId_t p) const { return fMaxValue    [this->Id(p)]; }
   double StepSize           (valor::conv::PhysParamId_t p) const { return fStepSize    [this->Id(p)]; }
   bool   IsFixed            (valor::conv::PhysParamId_t p) const { return fIsFixed     [this->Id(p)]; }
   bool   IsNuisance         (valor::conv::PhysParamId_t p) const { return fIsNuisance  [this->Id(p)]; }
   double MigradError        (valor::conv::PhysParamId_t p) const { return fMigradError [this->Id(p)]; }
   double HesseError         (valor::conv::PhysParamId_t p) const { return fHesseError  [this->Id(p)]; }
   double PlusError          (valor::conv::PhysParamId_t p) const { return fPlusError   [this->Id(p)]; }
   double MinusError         (valor::conv::PhysParamId_t p) const { return fMinusError  [this->Id(p)]; }

   void SetMinValue (valor::conv::PhysParamId_t p, double min) { fMinValue [this->Id(p)] = min; }
   void SetMaxValue (valor::conv::PhysParamId_t p, double max) { fMaxValue [this->Id(p)] = max; }

   // Add a parameter
   bool AddParam (
     int ip, valor::conv::PhysParamId_t p, double init, 
     double min, double max, double step, bool fix=false, bool nuisance=false, string name="");
      
   // Check whether the input parameter was included in the physics param list
   bool Exists (valor::conv::PhysParamId_t p) const;

   // Update current/running value for the given parameter
   void Update(valor::conv::PhysParamId_t p, double v);

   // Allow modifying the initial value (minimization may be sensitive to this choice)
   void SetInitValue(valor::conv::PhysParamId_t p, double v);

   // Reset all current param values to their specified initial values
   void ResetCurrToInitValues (void);

   // Copy current param values to best-fit values & vice-versa
   void CopyCurrToBestFitValues (void);
   void CopyBestFitToCurrValues (void);
   void CopyBestFitToInitValues (void);
   void CopyBestFitToInitValues (const FitParamList & plst);

   // Copy from input
   using TNamed::Copy; //supress clang 'hides overloaded virtual function [-Woverloaded-virtual]' warnings
   void Copy (const FitParamList & plst);

   // Set the covariance matrix
   void SetCovMatrix(TMatrixDSym & covmatrix);

   // Set the errors
   void SetErrors(double * errplus, double * errminus, double * errmigrad);
   
   // used with FitterMinuit, vector must be same size as FitParamList->Size()
   void SetMigradErrors(std::vector<double> err);
   void SetHesseErrors (std::vector<double> err);
   void SetMinosErrors (std::vector<double> plus, std::vector<double> minus);

   // Fix a parameter
   void FixParameter        (valor::conv::PhysParamId_t p); 
   void FixParameterByIndex (int ip); 

   // Free a parameter
   void FreeParameter        (valor::conv::PhysParamId_t p); 
   void FreeParameterByIndex (int ip); 

   // Set as nuisance parameter (contributes to \chi^{2}_{penalty})
   void SetNuisance (valor::conv::PhysParamId_t p, bool tf);

   // Set parameter list to input TVirtualFitter 
   void SetTo (TVirtualFitter * fitter);
   void SetTo (ROOT::Minuit2::MnUserParameters & params);
   void SetTo (ROOT::Math::Minimizer * minimizer);
   void SetTo (ROOT::Fit::Fitter * fitter);
   
   // Sync fit param values stored this object with the values stored in the input array.
   void Sync (const double * par);
   void Sync (std::vector<double> par);

   // copy initial, current or best-fit values to a PhysParamList 
   valor::PhysParamList AsPhysParamList(Option_t * option = "current") const;
   void FromPhysParamList(valor::PhysParamList pl, Option_t * option, bool skip_osc_params);

   // Get number of free (not fixed) parameters
   int GetNumberFreeParameters(void);
   int GetNumberParameters(void);

   // Set and get other fit properties: statistic at best-fit point, fit time, status codes etc
   void   ClearStatusCodes    (void);
   void   SetStatusCode       (string st, int code);
   int    GetStatusCode       (string st) const;
   void   SetBestFitStatistic (double stat_bf)   { fBestFitStatistic = stat_bf; }
   double GetBestFitStatistic (void) const       { return fBestFitStatistic;    }
   void   SetCurrStatistic    (double stat_curr) { fCurrStatistic = stat_curr;  }
   double GetCurrStatistic    (void) const       { return fCurrStatistic;       }
   void   SetFitTime          (double fit_time)  { fFitTime = fit_time;         }
   double GetFitTime          (void) const       { return fFitTime;             }
   void   SetFitTries         (int   fit_tries)  { fFitTries = fit_tries;       }
   int    GetFitTries         (void) const       { return fFitTries;            }
   
   TMatrixDSym * GetCovMatrix(void) { return fCovMatrix; }

   // Calculate the fit statistic penalty
   double HypothesisFitStatisticPenalty(Option_t * opt = 0) const;

   // Print
   //using TNamed::Print; //supress clang 'hides overloaded virtual function [-Woverloaded-virtual]' warnings
   void Print (std::ostream & stream) const;
   void Print (Option_t * option = "") const;
   friend std::ostream & operator << (std::ostream & stream, const FitParamList & plst);

   // Methods with an one-off effect in Print()
   void ShowOnlyCurrValuesInNextPrint   (void) const { fShowCurrValuesOnly    = true; }
   void ShowOnlyBestFitValuesInNextPrint(void) const { fShowBestFitValuesOnly = true; }

   // Using the "Named Constructor" C++ idiom for fast creation of typical parameter lists.
   // The string argument of NamedParameterList() can take the following values:
   // - "nuosc3f"                          : Fit for mixing_12, mixing_13, mixing_23, cp_param, Dm^2 and dm^2 (3-flavour formalism).
   //                                        The mixing_ij parameters can be sin^2(2theta_ij) or sin^2(theta_ij).
   //                                        The cp_param can be delta_CP or sin(delta_CP).
   //                                        The Dm^2 and dm^2 params can be |Dm23^2| and Dm12^2, or the ones used by Fogli and Lisi.
   // - "nuosc3f+nuisance"                 : As "nuosc3f" + all nuisance parameters
   // - "nuosc3f_12constrain"              : As "nuosc3f", but penalizing devations from 12-sector values measured in solar/reactor expts.
   // - "nuosc3f_12fix"                    : As "nuosc3f", but fixing 12-sector parameters to values measured in solar/reactor expts.
   // - "nuosc3f_numudisapp_fix4"          : As "nuosc3f"           but fixing mixing_12, mixing_13, cp_param and dm^2.
   // - "nuosc3f_numudisapp_fix4+nuisance" : As "nuosc3f+nuisance"  but fixing mixing_12, mixing_13, cp_param and dm^2.
   // - "nuosc3f_nueapp_fix4"              : As "nuosc3f"           but fixing mixing_12, mixing_23, Dm^2 and dm^2.
   // - "nuosc3f_nueapp_fix4+nuisance"     : As "nuosc3f+nuisance", but fixing mixing_12, mixing_23, Dm^2 and dm^2.
   // - "numudisapp2f"                     : Fit for mixing_23 and Dm^2 (2-flavour formalism)
   // - "numudisapp2f+nuisance"            : As "numudisapp2f" + all nuisance parameters
   // - "decay"                            : Fit for sin^2(2theta) and \alpha_{decay}
   // - "decoh"                            : Fit for sin^2(2theta) and \mu_{decoherence}
   static FitParamList * NamedParameterList (string name);

   // Get the name used in NamedParameterList
   string GetParListName  (void) const { return fParListName; }
   int    GetSystMode     (void) const { return fSystMode;     }

   // Work-out the physics hypothesis
   conv::PhysHypothesis_t PhysicsHypothesis(void) const;

   //Are the errors set?
   bool MigradErrorsSet (void) const { return fMigradErrorsSet; }
   bool HesseErrorsSet  (void) const { return fHesseErrorsSet;  }
   bool MinosErrorsSet  (void) const { return fMinosErrorsSet;  }

   int  Id      (valor::conv::PhysParamId_t p) const; // PhysParamId_t --> array slot
   valor::conv::PhysParamId_t WhichParam(int id) const; // array slot --> PhysParamId_t
   
   // Returns a fit param list with a physics hypothesis, and 'nuisance' empty parameters.
   static FitParamList * NuOsc3FlavourFit                          (int nuisance = 0);
   static FitParamList * NumuDisapFit                              (int nuisance = 0);
   static FitParamList * NumuDecayFit                              (int nuisance = 0);
   static FitParamList * NumuDecohFit                              (int nuisance = 0);
   static FitParamList * NuOscSterile4FlavourFit                   (int nuisance = 0);
   static FitParamList * NuOscSterile5FlavourFit                   (int nuisance = 0);
   
   static void           Append3FlavourOscillation                 (FitParamList * fp, int start);
   static void           Append2FlavourOscillation                 (FitParamList * fp, int start);
   static void           Append4FlavourSterileOscillation          (FitParamList * fp, int start);
   static void           Append5FlavourSterileOscillation          (FitParamList * fp, int start);
   static void           AppendAllSystematics                      (FitParamList * fp, int start);
   
   void SetParListName (string s)   { fParListName = s; }
   void SetSystMode    (int m)      { fSystMode    = m; }
   
private:

   void CleanUp        (void);
   void Init           (int nparams);

   // Various functions called by NamedParameterList(name)
   
   //~ static FitParamList * NuOsc3FlavourFitWithAllSyst               (void);
   //~ static FitParamList * NumuDisapFitWithAllSyst                   (void);
   //~ static FitParamList * NuOscSterile4FlavourFitWithAllSyst        (void);
   //~ static FitParamList * NuOscSterile5FlavourFitWithAllSyst        (void);
   

   //
   // Private data members
   //


   map<valor::conv::PhysParamId_t, int> fIdMap;

   string       fParListName;
   int          fSystMode; // used in FitInputs to make deciding on what to include in the chisq penalty faster. 0 no syst, 1 all syst, 2 selected syst, ...

   int          fNParams;   
   TObjString * fName;         //[fNParams]
   double *     fBestFitValue; //[fNParams]
   double *     fCurrValue;    //[fNParams]
   double *     fInitValue;    //[fNParams]
   double *     fMinValue;     //[fNParams]
   double *     fMaxValue;     //[fNParams]
   double *     fStepSize;     //[fNParams]
   bool *       fIsFixed;      //[fNParams]
   bool *       fIsNuisance;   //[fNParams]
   double *     fMigradError;  //[fNParams]
   double *     fHesseError;   //[fNParams]
   double *     fPlusError;    //[fNParams]
   double *     fMinusError;   //[fNParams]
   TMatrixDSym* fCovMatrix;  
   double       fBestFitStatistic; 
   double       fCurrStatistic; 
   double       fFitTime;
   int          fFitTries;

   bool fMigradErrorsSet;
   bool fHesseErrorsSet;
   bool fMinosErrorsSet;

   map<string, int> fStatusCodes;

   mutable bool fShowCurrValuesOnly;
   mutable bool fShowBestFitValuesOnly;

ClassDef(FitParamList,1)
};

std::ostream & operator<< (std::ostream & stream, const FitParamList & plst);

} // valor
  
#endif



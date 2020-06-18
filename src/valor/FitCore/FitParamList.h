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

#include "TH1F.h"
#include "TH2.h"
#include "TH1.h"

#include "valor/Conventions/PhysParamId.h"
#include "valor/Conventions/PhysHypothesisId.h"
#include "valor/Conventions/Build.h"

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
    double BestFitValue       (valor::conv::PhysParamId_t p) const { return fBestFitValue    [this->Id(p)]; }
    double CurrValue          (valor::conv::PhysParamId_t p) const { return fCurrValue       [this->Id(p)]; }
    double InitValue          (valor::conv::PhysParamId_t p) const { return fInitValue       [this->Id(p)]; }
    double MinValue           (valor::conv::PhysParamId_t p) const { return fMinValue        [this->Id(p)]; }
    double MaxValue           (valor::conv::PhysParamId_t p) const { return fMaxValue        [this->Id(p)]; }
    double StepSize           (valor::conv::PhysParamId_t p) const { return fStepSize        [this->Id(p)]; }
    bool   IsFixed            (valor::conv::PhysParamId_t p) const { return fIsFixed         [this->Id(p)]; }
    bool   IsNuisance         (valor::conv::PhysParamId_t p) const { return fIsNuisance      [this->Id(p)]; }
    double MigradError        (valor::conv::PhysParamId_t p) const { return fMigradError     [this->Id(p)]; }
    double HesseError         (valor::conv::PhysParamId_t p) const { return fHesseError      [this->Id(p)]; }
    double PlusError          (valor::conv::PhysParamId_t p) const { return fPlusError       [this->Id(p)]; }
    double MinusError         (valor::conv::PhysParamId_t p) const { return fMinusError      [this->Id(p)]; }
    double ParameterScaling   (valor::conv::PhysParamId_t p) const { return fParameterScaling[this->Id(p)]; }
    
    // Same as Above, but by index
    string NameByIndex            (int i) const  { return fName[i].GetString().Data();   }
    double BestFitValueByIndex    (int i) const  { return fBestFitValue    [i]; }
    double CurrValueByIndex       (int i) const  { return fCurrValue       [i]; }
    double InitValueByIndex       (int i) const  { return fInitValue       [i]; }
    double MinValueByIndex        (int i) const  { return fMinValue        [i]; }
    double MaxValueByIndex        (int i) const  { return fMaxValue        [i]; }
    double StepSizeByIndex        (int i) const  { return fStepSize        [i]; }
    bool   IsFixedByIndex         (int i) const  { return fIsFixed         [i]; }
    bool   IsNuisanceByIndex      (int i) const  { return fIsNuisance      [i]; }
    double MigradErrorByIndex     (int i) const  { return fMigradError     [i]; }
    double HesseErrorByIndex      (int i) const  { return fHesseError      [i]; }
    double PlusErrorByIndex       (int i) const  { return fPlusError       [i]; }
    double MinusErrorByIndex      (int i) const  { return fMinusError      [i]; }
    double ParameterScalingByIndex(int i) const  { return fParameterScaling[i]; }
    
    // Add a parameter
    bool AddParam (
       int ip, valor::conv::PhysParamId_t p, double init,
       double min, double max, double step, bool fix=false, bool nuisance=false, string name="");
    
    // Check whether the input parameter was included in the physics param list
    bool Exists (valor::conv::PhysParamId_t p) const;
    
    // Update current/running value for the given parameter
    void Update(valor::conv::PhysParamId_t p, double v, bool scaled = false);
    
    // Allow modifying the initial value (minimization may be sensitive to this choice)
    void SetInitValue   (valor::conv::PhysParamId_t p, double v);
    void SetCurrentValue(valor::conv::PhysParamId_t p, double v);
    void SetInitValueByIndex    (int i, double v);
    void SetCurrentValueByIndex (int i, double v);
    
    // Update limits
    void SetMinValue (valor::conv::PhysParamId_t p, double min) { fMinValue [this->Id(p)] = min; }
    void SetMaxValue (valor::conv::PhysParamId_t p, double max) { fMaxValue [this->Id(p)] = max; }
    void SetMinValueByIndex (int i, double min) { fMinValue [i] = min; }
    void SetMaxValueByIndex (int i, double max) { fMaxValue [i] = max; }

    void SetStepSize (valor::conv::PhysParamId_t p, double step) { fStepSize [this->Id(p)] = step; }
    void SetIsNuisance (valor::conv::PhysParamId_t p, bool is_nuisance) { fIsNuisance [this->Id(p)] = is_nuisance; }

    void SetParameterScaling(conv::PhysParamId_t p, double scaling) { fParameterScaling[this->Id(p)] = scaling;}
    void SetParameterScalingByIndex(int i , double scaling) { fParameterScaling[i] = scaling;}
    
    // Reset all current param values to their specified initial values
    void ResetCurrToInitValues (void);
    
    // Copy current param values to best-fit values & vice-versa
    void CopyCurrToBestFitValues (void);
    void CopyCurrToInitValues    (void);
    void CopyBestFitToCurrValues (void);
    void CopyBestFitToInitValues (void);
    void CopyBestFitToInitValues (const FitParamList & plst);
    
    // Copy from input
    using TNamed::Copy; //supress clang 'hides overloaded virtual function [-Woverloaded-virtual]' warnings
    void Copy (const FitParamList & plst);
    
    // Set the covariance matrix
    void SetCovMatrix(TMatrixDSym & covmatrix, unsigned int start_at = 0);
    
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
    
    // Create a new fit parameter list that includes only the free (i.e. not fixed) parameters of the original list
    FitParamList * FreeParameterSubset(void);

    // Create a new fit parameter list that includes only the non-marginalised parameters of the original list
    FitParamList * ParameterOfInterestSubset(void);

    // Set parameter list to input TVirtualFitter
    void SetTo (TVirtualFitter * fitter);
    void SetTo (ROOT::Minuit2::MnUserParameters & params);
    void SetTo (ROOT::Math::Minimizer * minimizer);
    void SetTo (ROOT::Fit::Fitter * fitter);
    
    // Sync fit param values stored this object with the values stored in the input array.
    void Sync (const double * par, bool scaling = false);
    void Sync (std::vector<double> par, bool scaling = false);
    
    // copy initial, current or best-fit values to a PhysParamList
    valor::PhysParamList AsPhysParamList(Option_t * option = "current") const;
    void FromPhysParamList(valor::PhysParamList pl, Option_t * option, bool skip_osc_params = false); // skip_osc_params is deprecated. Use option "skiposc" instead.
    
    // copy initial, current or best-fit values to a TVectorD
    TVectorD AsTVector(Option_t * option = "current") const;
    
    // Get number of free (not fixed) parameters
    int GetNumberFreeParameters(void) const;
    int GetNumberParameters    (void) const;

    // Get number of parameters of interest (i.e. non-marginalised parameters)
    int GetNumberParametersOfInterest(void) const;
    
    // Set and get other fit properties: statistic at best-fit point, fit time, status codes etc
    void   ClearStatusCodes    (void);
    void   SetStatusCode       (std::string st , int code);
    void   SetStatusCode       (const char  * st, int code);
    int    GetStatusCode       (std::string  st) const;
    int    GetStatusCode       (const char  * st) const;
    
    void   SetBestFitStatistic (double stat_bf)   { fBestFitStatistic = stat_bf; }
    double GetBestFitStatistic (void) const       { return fBestFitStatistic;    }
    void   SetCurrStatistic    (double stat_curr) { fCurrStatistic = stat_curr;  }
    double GetCurrStatistic    (void) const       { return fCurrStatistic;       }
    void   SetFitTime          (double fit_time)  { fFitTime = fit_time;         }
    double GetFitTime          (void) const       { return fFitTime;             }
    void   SetFitTries         (int   fit_tries)  { fFitTries = fit_tries;       }
    int    GetFitTries         (void) const       { return fFitTries;            }
    
    void   SetBestFitStatisticContributions (const std::map<std::string,double> & stat_bf)   { fBestFitStatisticContributions = stat_bf; }
    const std::map<std::string,double> & GetBestFitStatisticContributions (void) const { return fBestFitStatisticContributions;    }
    
    // Check if this FPL is complete and ready to be used
    bool IsComplete();
    
    TMatrixDSym *       GetCovMatrix(void)       { return fCovMatrix; }
    const TMatrixDSym * GetCovMatrix(void) const { return fCovMatrix; }
    
    // Calculate the fit statistic penalty
    double HypothesisFitStatisticPenalty(Option_t * opt = 0) const;
    void SetMix13UserError(double central, double error);
    
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
    static FitParamList * NamedParameterList (string name, size_t n_empty = 0);
    
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
    
    
    // MARGINALIZATION
    void SetDoMarg      (valor::conv::PhysParamId_t p, bool domarg);
    void SetPriorMode   (valor::conv::PhysParamId_t p, int priormode);
    void SetPriorMode   (valor::conv::PhysParamId_t p, string priormodename);
    void SetPriorScheme (valor::conv::PhysParamId_t p, string scheme_name);
    void SetMeanGaus    (valor::conv::PhysParamId_t p, double mean);
    void SetSigmaGaus   (valor::conv::PhysParamId_t p, double sigma);
    void SetMinValueFlat(valor::conv::PhysParamId_t p, double minvalmarg);
    void SetMaxValueFlat(valor::conv::PhysParamId_t p, double maxvalmarg);
    
    void SetPriorHisto  (valor::conv::PhysParamId_t p, TH1D *hprior);
    void SetPriorHisto2D(valor::conv::PhysParamId_t p, TH2D *hprior);
    void SetToyHisto    (valor::conv::PhysParamId_t p, TH1D *hprior);
    void SetToyHisto2D  (valor::conv::PhysParamId_t p, TH2D *hprior);
    
    void SetPriorDimension2D(valor::conv::PhysParamId_t p, bool isprior2d);
    
    bool   IsMarginalized    (valor::conv::PhysParamId_t p) const { return fDoMarg        [this->Id(p)]; }
    int    GetPriorMode      (valor::conv::PhysParamId_t p) const { return fPriorMode     [this->Id(p)]; }
    string GetPriorModeName  (valor::conv::PhysParamId_t p) const;
    int    GetPriorScheme    (valor::conv::PhysParamId_t p) const { return fPriorScheme   [this->Id(p)]; }
    string GetPriorSchemeName(valor::conv::PhysParamId_t p) const;
    double GetMeanGaus       (valor::conv::PhysParamId_t p) const { return fMeanGaus      [this->Id(p)]; }
    double GetSigmaGaus      (valor::conv::PhysParamId_t p) const { return fSigmaGaus     [this->Id(p)]; }
    double GetMinValueFlat   (valor::conv::PhysParamId_t p) const { return fMinValueFlat  [this->Id(p)]; }
    double GetMaxValueFlat   (valor::conv::PhysParamId_t p) const { return fMaxValueFlat  [this->Id(p)]; }
    
    TH1D * GetPriorHisto    (valor::conv::PhysParamId_t p) const { return fPriorHisto    [this->Id(p)]; }
    TH2D * GetPriorHisto2D  (valor::conv::PhysParamId_t p) const { return fPriorHisto2D  [this->Id(p)]; }
    TH1D * GetToyHisto      (valor::conv::PhysParamId_t p) const { return fToyHisto      [this->Id(p)]; }
    TH2D * GetToyHisto2D    (valor::conv::PhysParamId_t p) const { return fToyHisto2D    [this->Id(p)]; }
    
    bool   IsPrior2D        (valor::conv::PhysParamId_t p) const { return fIsPrior2D     [this->Id(p)]; }
    
    //
    
    
    // Returns a fit param list with a physics hypothesis, and 'nuisance' empty parameters.
    static FitParamList * NuOsc3FlavourFit                          (int nuisance = 0);
    static FitParamList * NuOsc3FlavourBarFit                       (int nuisance = 0);
    static FitParamList * NuOsc3FlavourBarDissapFit                 (int nuisance = 0);
    
    // NEW BETABAR 2
    static FitParamList * NuOsc3FlavourNormBarFit                   (int nuisance = 0);
    // NEW BETABAR
    static FitParamList * NuOsc3FlavourBarNormBarFit                (int nuisance = 0);
    //
    
    static FitParamList * NumuDisapFit                              (int nuisance = 0);
    static FitParamList * NumuDecayFit                              (int nuisance = 0);
    static FitParamList * NumuDecohFit                              (int nuisance = 0);
    static FitParamList * NuOscSterile4FlavourFit                   (int nuisance = 0);
    static FitParamList * NuOscSterile5FlavourFit                   (int nuisance = 0);
    static FitParamList * NuOscSterileEffectiveFit                  (int nuisance = 0);
    
    static void           Append3FlavourOscillation                 (FitParamList * fp, int start);
    static void           Append3FlavourOscillationBar              (FitParamList * fp, int start);
    static void           Append3Flavour23BarOscillation            (FitParamList * fp, int start);

    // NEW BETABAR
    static void           AppendNormBar                             (FitParamList * fp, int start);
    //
    
    static void           Append2FlavourOscillation                 (FitParamList * fp, int start);
    static void           Append4FlavourSterileOscillation          (FitParamList * fp, int start);
    static void           Append5FlavourSterileOscillation          (FitParamList * fp, int start);
    static void           AppendEffectiveSterileOscillation         (FitParamList * fp, int start);
    static void           AppendAllSystematics                      (FitParamList * fp, int start);
    
    void SetParListName (string s)   { fParListName = s; }
    void SetSystMode    (int m)      { fSystMode    = m; }
    
  private:
    
    void CleanUp        (void);
    void Init           (int nparams);
    
    // Returns the first open slot in list - returns conv::kBadResult otherwise.
    int FirstEmptyEntry();
    
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
    double     * fBestFitValue; //[fNParams]
    double     * fCurrValue;    //[fNParams]
    double     * fInitValue;    //[fNParams]
    double     * fMinValue;     //[fNParams]
    double     * fMaxValue;     //[fNParams]
    double     * fStepSize;     //[fNParams]
    bool       * fIsFixed;      //[fNParams]
    bool       * fIsNuisance;   //[fNParams]
    double     * fMigradError;  //[fNParams]
    double     * fHesseError;   //[fNParams]
    double     * fPlusError;    //[fNParams]
    double     * fMinusError;   //[fNParams]
    double     * fParameterScaling; //[fNParams] - scale all parameters by this value before copying too/from fitter
    TMatrixDSym* fCovMatrix;
    double       fBestFitStatistic;
    double       fCurrStatistic;
    double       fFitTime;
    int          fFitTries;
    
    bool fMigradErrorsSet;
    bool fHesseErrorsSet;
    bool fMinosErrorsSet;
    
    std::map<std::string, int> fStatusCodes;
    std::map<std::string, double> fBestFitStatisticContributions;
    
    mutable bool fShowCurrValuesOnly;
    mutable bool fShowBestFitValuesOnly;
    
    
    ////////////////////////////////////
    
    // MARGINALIZATION
    
    bool       *fDoMarg;         //[fNParams]  flag to marginalize the fit parameter
    int        *fPriorMode;      //[fNParams]  0 -> flat, 1 -> gaus, 2 -> user (histo)
    TObjString *fPriorModeName;  //[fNParams]  0 -> flat, 1 -> gaus, 2 -> user (histo)

    int        *fPriorScheme;    //[fNParams]  maps onto the relevant NuOscDeltaCP_t, NuOscDmsqScheme_t or NuOscMixParam_t enum for each param
    
    double *fMeanGaus;  //[fNParams]  // Mean of marginalization  (gaussian prior)
    double *fSigmaGaus; //[fNParams]  // Sigma of marginalization (gaussian prior)
    
    double *fMinValueFlat; //[fNParams]  // Low edge of the marginalization interval (flat prior)
    double *fMaxValueFlat; //[fNParams]  // Up  edge of the marginalization interval (flat prior)
    
    TH1D **fPriorHisto; //[fNParams]  // Prior histgram
    TH1D **fToyHisto;   //[fNParams]  // Extracted histgram from Prior 
    
    TH2D **fPriorHisto2D; //[fNParams]  // Prior histgram 
    TH2D **fToyHisto2D;   //[fNParams]  // Extracted histgram from Prior 
    
    bool *fIsPrior2D; //[fNParams] If user prior is used define if the 1D or 2D one has to be used
    
    ////////////////////////////////////
    
    bool fUseUserOscPenPriors;
    double fUserMix13Central;
    double fUserMix13Error;

    ClassDef(FitParamList,3)
  };
  
  std::ostream & operator<< (std::ostream & stream, const FitParamList & plst);
  
} // valor

#endif



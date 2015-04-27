//____________________________________________________________________________
/*!

\class    valor::conv

\brief    Constants

\author   Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
          STFC, Rutherford Appleton Laboratory
          
\created  Oct 20, 2010

*/
//____________________________________________________________________________

#ifndef constants_h
#define constants_h

namespace valor {
namespace conv   {

//
// Oscillation parameters, recent global fit values & 1 sigma errors and
// typical initialization and range values for fitter
//


/////////////////////////////////////////////
//                                         //
// T2K only joint fit results (VALOR)      //
//                                         //
/////////////////////////////////////////////

static const double kT2KBestFitDmsq12  = 7.5E-5; 
static const double kT2KBestFitSinsqTheta12  = 0.306; 
static const double kT2KBestFitSinsq2Theta12  = 0.8495; 

static const double kT2KBestFitDeltaCP_NH = 1.91;
static const double kT2KBestFitDeltaCP_IH = 1.01;

static const double kT2KBestFitDmsq23_NH  = 2.51E-3; 
static const double kT2KBestFitDmsq23_IH  = 2.49E-3; 

static const double kT2KBestFitSinsqTheta13_NH  = 0.0422; 
static const double kT2KBestFitSinsqTheta13_IH  = 0.0491; 

static const double kT2KBestFitSinsqTheta23_NH  = 0.524; 
static const double kT2KBestFitSinsqTheta23_IH  = 0.523; 

static const double kT2KBestFitSinsq2Theta13_NH  = 0.1617; 
static const double kT2KBestFitSinsq2Theta13_IH  = 0.1868; 

static const double kT2KBestFitSinsq2Theta23_NH  = 0.998; 
static const double kT2KBestFitSinsq2Theta23_IH  = 0.998; 

/////////////////////////////////////////////
//                                         //
// T2K+reactors joint fit results (VALOR)  //
//                                         //
/////////////////////////////////////////////

static const double kT2KReacBestFitDmsq12  = 7.5E-5; 
static const double kT2KReacBestFitSinsqTheta12  = 0.306; 
 static const double kT2KReacBestFitSinsq2Theta12  = 0.8495; 

static const double kT2KReacBestFitDeltaCP_NH = -1.55;
static const double kT2KReacBestFitDeltaCP_IH = -1.56;

static const double kT2KReacBestFitDmsq23_NH  = 2.51E-3; 
static const double kT2KReacBestFitDmsq23_IH  = 2.48E-3; 

static const double kT2KReacBestFitSinsqTheta13_NH  = 0.0248; 
static const double kT2KReacBestFitSinsqTheta13_IH  = 0.0252; 

static const double kT2KReacBestFitSinsqTheta23_NH  = 0.527; 
static const double kT2KReacBestFitSinsqTheta23_IH  = 0.533; 

static const double kT2KReacBestFitSinsq2Theta13_NH  = 0.0967; 
static const double kT2KReacBestFitSinsq2Theta13_IH  = 0.0983; 

static const double kT2KReacBestFitSinsq2Theta23_NH  = 0.997; 
static const double kT2KReacBestFitSinsq2Theta23_IH  = 0.996; 


/////////////////////////////////////
//                                 //
// Dm^2 global fit results         //
//                                 //
/////////////////////////////////////

// Updated

static const double kGlobFitDm12sq           = 7.53E-5; // (PDG 2014)

//static const double kGlobFitDm23sq           = 2.4E-3; // old
//static const double kGlobFitDm23sq           = 2.32E-3; 
//static const double kGlobFitDm23sq           = 2.44E-3; // (PDG 2014)
static const double kGlobFitDm23sq           = kT2KReacBestFitDmsq23_NH; 

static const double kGlobFitDmsqFL           = 2.36E-3;

//static const double kGlobFitErrDm23sq        = 0.04; // ~4%
//static const double kGlobFitErrDm23sq        = 0.025; // ~2.5% (PDG 2014)
static const double kGlobFitErrDm23sq        = 0.04; // ~4% (T2K Run1-4 NH)

//static const double kGlobFitErrDm12sq        = 0.027; // ~2.7%
static const double kGlobFitErrDm12sq        = 0.024; // ~2.4% (PDG 2014)

/////////////////////////////////////
//                                 //
// deltacp global fit results      //
//                                 //
/////////////////////////////////////

static const double kGlobFitDeltaCP          = kT2KReacBestFitDeltaCP_NH;
static const double kGlobFitErrDeltaCP       = 0.10; // ~10%

/////////////////////////////////////
//                                 //
// sinsq2theta global fit results  //
//                                 //
/////////////////////////////////////

//static const double kGlobFitSinsq2Theta23    = 1.00;
// static const double kGlobFitSinsq2Theta23    = 0.999; // (PDG 2014)
 static const double kGlobFitSinsq2Theta23    = kT2KReacBestFitSinsq2Theta23_NH;

//static const double kGlobFitSinsq2Theta13    = 0.084;  // Daya Bay Neutrino 2014 //0.098;
//static const double kGlobFitSinsq2Theta13    = 0.098;  // old (PDG 2012)   
//static const double kGlobFitSinsq2Theta13    = 0.095;  // old (PDG 2013)
//static const double kGlobFitSinsq2Theta13    = 0.093;  // (PDG 2014)
static const double kGlobFitSinsq2Theta13    = kT2KReacBestFitSinsq2Theta13_NH;  

//static const double kGlobFitSinsq2Theta12    = 0.8495; //fogli2011
//static const double kGlobFitSinsq2Theta12    = 0.857; //pdg2012
static const double kGlobFitSinsq2Theta12    = kT2KReacBestFitSinsq2Theta12; // (PDG 2014)

//static const double kGlobFitErrSinsq2Theta23 = 0.05;  // ~5%
//static const double kGlobFitErrSinsq2Theta23 = 0.018;  // ~1.8% (PDG 2014)
static const double kGlobFitErrSinsq2Theta23 = 0.109;  // ~10.9% (T2K Run1-4 NH)
 
//static const double kGlobFitErrSinsq2Theta12 = 0.031; // ~3.1% fogli2011
//static const double kGlobFitErrSinsq2Theta12 = 0.028; // ~2.8% pdg2012
static const double kGlobFitErrSinsq2Theta12 = 0.025; // ~2.5% (PDG 2014)

//static const double kGlobFitErrSinsq2Theta13 = 0.059;  // Daya Bay Neutrino 2014 
//static const double kGlobFitErrSinsq2Theta13 = 0.133;  // ~13.3%    // old (PDG 2012)
//static const double kGlobFitErrSinsq2Theta13 = 0.105; // ~10.5% (PDG 2013)
//static const double kGlobFitErrSinsq2Theta13 = 0.086; // ~8.6% (PDG 2014)
static const double kGlobFitErrSinsq2Theta13 = 0.324; // ~32.4% (T2K Run1-4 NH)



/////////////////////////////////////
//                                 //
// sinsqtheta global fit results  //
//                                 //
/////////////////////////////////////

//static const double kGlobFitSinsqTheta23    = 0.514; // (PDG 2014)
 static const double kGlobFitSinsqTheta23    = kT2KReacBestFitSinsqTheta23_NH; //

//static const double kGlobFitSinsqTheta13   = 0.02146; // Daya Bay Neutrino 2014  //0.0251;
//static const double kGlobFitSinsqTheta13   = 0.0251; // old (PDG 2012)
//static const double kGlobFitSinsqTheta13   = 0.0243; // (PDG 2013)
//static const double kGlobFitSinsqTheta13   = 0.0238; // (PDG 2014)
static const double kGlobFitSinsqTheta13   = kT2KReacBestFitSinsqTheta13_NH; 

//static const double kGlobFitSinsqTheta12    = 0.306; //fogli2011
//static const double kGlobFitSinsqTheta12    = 0.311; //pdg2012
static const double kGlobFitSinsqTheta12    = kT2KReacBestFitSinsqTheta12; 

//static const double kGlobFitErrSinsqTheta23 = 0.06;  // ~26% (1 sigma range)
//static const double kGlobFitErrSinsqTheta23 = 0.018;  // ~1.8% (PDG 2014)
static const double kGlobFitErrSinsqTheta23 = 0.109;  // ~10.9% (T2K Run1-4 NH)

//static const double kGlobFitErrSinsqTheta12 = 0.059;  // ~5.9% fogli2011
//static const double kGlobFitErrSinsqTheta12 = 0.053;  // ~5.3% pdg2012
static const double kGlobFitErrSinsqTheta12 = 0.025;  // ~2.5% (PDG 2014)

//static const double kGlobFitErrSinsqTheta13 = 0.059; // Daya Bay Neutrino 2014 //0.139;  // ~13.9%
//static const double kGlobFitErrSinsqTheta13 = 0.139; // ~13.9% // old (PDG 2012)
//static const double kGlobFitErrSinsqTheta13 = 0.105; // ~10.5% (PDG 2013)
// static const double kGlobFitErrSinsqTheta13 = 0.086; // ~8.6% (PDG 2014)
 static const double kGlobFitErrSinsqTheta13 = 0.324; // ~32.4% (T2K Run1-4 NH)



// projected future errors
//atmos params unknown
static const double kFutureAbsErrSinsqTheta23  = 0.05;//red line, page 36, PAC report final
static const double kFutureAbsErrSinsq2Theta23 = 0.19;//conversion of above (seems huge!)
static const double kFutureAbsErrDm23sq        = 0.045E-3;//red line, page 36, PAC report final
//mix13 using current daya bay systematic error
 static const double kFutureAbsErrSinsqTheta13  = 0.0013858; //conversion of number below
static const double kFutureAbsErrSinsq2Theta13 = 0.005;
//solar are known
static const double kFutureAbsErrSinsqTheta12  = 0.0;//if you make these non zero, also change FitInputs.cxx and add the relevant penalty term
static const double kFutureAbsErrSinsq2Theta12 = 0.0;
static const double kFutureAbsErrDm12sq        = 0.0;



// Dm^2 fitter init values and range and step
static const double kInitFitDm23sq           = kGlobFitDm23sq;
static const double kMinFitDm23sq            = 1.0E-3;
static const double kMaxFitDm23sq            = 6.0E-3;
static const double kStepDm23sq              = 1.0E-4;
static const double kInitFitDm12sq           = kGlobFitDm12sq;
static const double kMinFitDm12sq            = 6.0E-5;
static const double kMaxFitDm12sq            = 9.0E-5;
static const double kStepDm12sq              = 2.0E-6;
// Dm^2 fitter init values and range and step (fixnonbar)
static const double kInitFitDm23sqBar        = kGlobFitDm23sq;
static const double kMinFitDm23sqBar         = 1.0E-5;
static const double kMaxFitDm23sqBar         = 2.0E-2;
static const double kStepDm23sqBar           = 1.0E-4;
static const double kInitFitDm12sqBar        = kGlobFitDm12sq;
static const double kMinFitDm12sqBar         = 6.0E-5;
static const double kMaxFitDm12sqBar         = 9.0E-5;
static const double kStepDm12sqBar           = 2.0E-6;
// sinsq2theta fitter init values and range
static const double kInitFitSinsq2Theta23    = 0.90;
static const double kMinFitSinsq2Theta23     = 0.00;
static const double kMaxFitSinsq2Theta23     = 1.00;
static const double kStepSinsq2Theta23       = 5.0E-3;
static const double kInitFitSinsq2Theta13    = kGlobFitSinsq2Theta13;
static const double kMinFitSinsq2Theta13     = 0.00;
static const double kMaxFitSinsq2Theta13     = 1.00;
static const double kStepSinsq2Theta13       = 1.0E-3;
static const double kInitFitSinsq2Theta12    = kGlobFitSinsq2Theta12;
static const double kMinFitSinsq2Theta12     = 0.00;
static const double kMaxFitSinsq2Theta12     = 1.00;
static const double kStepSinsq2Theta12       = 1.0E-2;
// sinsq2theta fitter init values and range (fixnonbar)
static const double kInitFitSinsq2Theta23Bar = 0.90;
static const double kMinFitSinsq2Theta23Bar  = 0.00;
static const double kMaxFitSinsq2Theta23Bar  = 1.00;
static const double kStepSinsq2Theta23Bar    = 5.0E-3;
static const double kInitFitSinsq2Theta13Bar = kGlobFitSinsq2Theta13;
static const double kMinFitSinsq2Theta13Bar  = 0.00;
static const double kMaxFitSinsq2Theta13Bar  = 1.00;
static const double kStepSinsq2Theta13Bar    = 1.0E-3;
static const double kInitFitSinsq2Theta12Bar = kGlobFitSinsq2Theta12;
static const double kMinFitSinsq2Theta12Bar  = 0.00;
static const double kMaxFitSinsq2Theta12Bar  = 1.00;
static const double kStepSinsq2Theta12Bar    = 1.0E-2;
// sinsqtheta fitter init values and range
static const double kInitFitSinsqTheta23     = 0.60;
static const double kMinFitSinsqTheta23      = 0.00;
static const double kMaxFitSinsqTheta23      = 1.00;
static const double kStepSinsqTheta23        = 5.0E-3;
static const double kInitFitSinsqTheta13     = kGlobFitSinsqTheta13;
static const double kMinFitSinsqTheta13      = 0.00;
static const double kMaxFitSinsqTheta13      = 1.00;
static const double kStepSinsqTheta13        = 1.0E-3;
static const double kInitFitSinsqTheta12     = kGlobFitSinsqTheta12;
static const double kMinFitSinsqTheta12      = 0.00;
static const double kMaxFitSinsqTheta12      = 1.00;
static const double kStepSinsqTheta12        = 1.0E-2;
// sinsqtheta fitter init values and range (NuBar fix Nu)
static const double kInitFitSinsqTheta23Bar  = 0.60;
static const double kMinFitSinsqTheta23Bar   = 0.00;
static const double kMaxFitSinsqTheta23Bar   = 1.00;
static const double kStepSinsqTheta23Bar     = 5.0E-3;
static const double kInitFitSinsqTheta13Bar  = kGlobFitSinsqTheta13;
static const double kMinFitSinsqTheta13Bar   = 0.00;
static const double kMaxFitSinsqTheta13Bar   = 1.00;
static const double kStepSinsqTheta13Bar     = 1.0E-3;
static const double kInitFitSinsqTheta12Bar  = kGlobFitSinsqTheta12;
static const double kMinFitSinsqTheta12Bar   = 0.00;
static const double kMaxFitSinsqTheta12Bar   = 1.00;
static const double kStepSinsqTheta12Bar     = 1.0E-2;
/* ////////////////////////////////////////////////////////////////////// */
/* // DeltaCP fitter init values and range and step */
/* static const double kInitFitDeltaCP           = 0; */
/* static const double kMinFitDeltaCP            = - 4.71238898038468967; */
/* static const double kMaxFitDeltaCP            = + 4.71238898038468967; */
/* static const double kStepDeltaCP              = 1.0E-2; */
/* ////////////////////////////////////////////////////////////////////// */

// Sterile fitter init values and range.
static const double kInitFitSinsqTheta14     = 0.00;
static const double kMinFitSinsqTheta14      = 0.00;
static const double kMaxFitSinsqTheta14      = 1.00;
static const double kStepSinsqTheta14        = 5.0E-3;
static const double kInitFitSinsqTheta15     = 0.00;
static const double kMinFitSinsqTheta15      = 0.00;
static const double kMaxFitSinsqTheta15      = 1.00;
static const double kStepSinsqTheta15        = 5.0E-3;
static const double kInitFitSinsqTheta24     = 0.00;
static const double kMinFitSinsqTheta24      = 0.00;
static const double kMaxFitSinsqTheta24      = 1.00;
static const double kStepSinsqTheta24        = 5.0E-3;
static const double kInitFitSinsqTheta25     = 0.00;
static const double kMinFitSinsqTheta25      = 0.00;
static const double kMaxFitSinsqTheta25      = 1.00;
static const double kStepSinsqTheta25        = 5.0E-3;
static const double kInitFitSinsqTheta34     = 0.00;
static const double kMinFitSinsqTheta34      = 0.00;
static const double kMaxFitSinsqTheta34      = 1.00;
static const double kStepSinsqTheta34        = 5.0E-3;
static const double kInitFitSinsqTheta35     = 0.00;
static const double kMinFitSinsqTheta35      = 0.00;
static const double kMaxFitSinsqTheta35      = 1.00;
static const double kStepSinsqTheta35        = 5.0E-3;
static const double kInitFitSinsq2Theta14    = 0.00;
static const double kMinFitSinsq2Theta14     = 0.00;
static const double kMaxFitSinsq2Theta14     = 1.00;
static const double kStepSinsq2Theta14       = 5.0E-3;
static const double kInitFitSinsq2Theta15    = 0.00;
static const double kMinFitSinsq2Theta15     = 0.00;
static const double kMaxFitSinsq2Theta15     = 1.00;
static const double kStepSinsq2Theta15       = 5.0E-3;
static const double kInitFitSinsq2Theta24    = 0.00;
static const double kMinFitSinsq2Theta24     = 0.00;
static const double kMaxFitSinsq2Theta24     = 1.00;
static const double kStepSinsq2Theta24       = 5.0E-3;
static const double kInitFitSinsq2Theta25    = 0.00;
static const double kMinFitSinsq2Theta25     = 0.00;
static const double kMaxFitSinsq2Theta25     = 1.00;
static const double kStepSinsq2Theta25       = 5.0E-3;
static const double kInitFitSinsq2Theta34    = 0.00;
static const double kMinFitSinsq2Theta34     = 0.00;
static const double kMaxFitSinsq2Theta34     = 1.00;
static const double kStepSinsq2Theta34       = 5.0E-3;
static const double kInitFitSinsq2Theta35    = 0.00;
static const double kMinFitSinsq2Theta35     = 0.00;
static const double kMaxFitSinsq2Theta35     = 1.00;
static const double kStepSinsq2Theta35       = 5.0E-3;

static const double kInitFitDm41sq           = 3E-3;
static const double kMinFitDm41sq            = kGlobFitDm23sq;
static const double kMaxFitDm41sq            = 1.0E-2;
static const double kStepDm41sq              = 1.0E-4;
static const double kInitFitDm51sq           = 3E-3;
static const double kMinFitDm51sq            = kGlobFitDm23sq;
static const double kMaxFitDm51sq            = 1.0E-2;
static const double kStepDm51sq              = 1.0E-4;

// fitter chisq surface ranges
static const double kMinChisqSurfaceDm23sq            = 1.5E-3;
static const double kMaxChisqSurfaceDm23sq            = 3.3E-3;
static const double kMinChisqSurfaceSinsq2Theta23     = 0.80;
static const double kMaxChisqSurfaceSinsq2Theta23     = 1.00;


//
// Other constants
//
static const double kMaxAllowedWght = 2E+3;                          // Complain if any computed weights exceeds this value
static const double kSetNegEventsTo = 1E-8;                          // Set any bins with nevent < 0 to this value
static const double kASmallNum      = 1E-10;                         // Need to keep this precision for Escale computation (worse than E-15)
static const double kAVerySmallNum  = 1E-15;                         // Need to keep this precision for Marginalization (histo->GetBinCenter() has a precision of E-19)  
static const double kALargeNum      = 999999999.;                    // 
static const double kBadResult      = -999;                          // If a fit fails, give this value to Opt
static const double kOneMeV         = 0.001;                         // 
static const double kBaseline       = 295;                           // Tokai->Kamioka (km)
static const double kOscDimensFactor= 1.26693281;                    // Dimensional factor (GeV/eV^2/km) appearing in netrino oscillation formula
static const double kCrustDensity   = 2.6;                           // Average density of Earth's continental crust (gr / cm^3)
static const double kpi             = 3.141592653589793;             // pi
static const double khbar_SI        = 6.582119e-16;                  // hbar (eV s)
static const double kc_SI           = 2.9979246e10;                  // c (cm/s)
static const double khbarc_SI       = khbar_SI*kc_SI;                // hbar*c (eV * cm)
static const double khbarc3_SI      = khbarc_SI*khbarc_SI*khbarc_SI; // (hbar*c)^3 (eV^3 * cm^3)
static const double kGF             = 1.16637e-5;                    // Fermi coupling constant, (GeV^-2, natural units)
static const double kGF_SI          = kGF*khbarc3_SI*1.0e-18;        // Fermi coupling constant, (eV cm^3, SI units), 1e-18 is used for GeV^-2 -> eV^-2
static const double kNA             = 6.022142e23;                   // Avogadro number (mol^-1)

} // conv
} // valor
  
#endif 


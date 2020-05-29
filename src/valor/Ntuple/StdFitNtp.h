//____________________________________________________________________________ 
/*!

\class     valor::StdFitNtp.h

\brief     Standard VALOR fit n-tuple 

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   September 16, 2016

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_standard_output_fit_ntuple_h_
#define _valor_standard_output_fit_ntuple_h_

class TFile;
class TTree;
class TObjArray;

namespace valor {

class PhysParamList;
class FitParamList;

class StdFitNtp
{
public:
  StdFitNtp();
  virtual ~StdFitNtp();
   
  void Create (const char* file_name, const char* ntp_name = "valor_ntp", const char* ntp_label = "");
  void Fill   (void);
  void Reset  (void);
  void Save   (void);

  TFile *         OutputFile            (void) const { return fOutFile;         }
  TTree *         OutputValorResultTree (void) const { return fVTree;           }  
  PhysParamList * TrueParamArr          (void) const { return fTrueParamArr;    }  
  FitParamList *  BestFitParamArr       (void) const { return fBestFitParamArr; }
  TObjArray *     BestFitModelArr       (void) const { return fBestFitModelArr; }  
  TObjArray *     DataArr               (void) const { return fDataArr;         }  

protected:

  void Init  (void);
  void Clean (void);

  TFile *         fOutFile;            ///< Output ROOT file
  TTree *         fVTree;              ///< Output VALOR tree
  PhysParamList * fTrueParamArr;       ///< array holding true parameters
  FitParamList *  fBestFitParamArr;    ///< array holding best-fit parameters
  TObjArray *     fBestFitModelArr;    ///< array holding best-fit predictions for all datasets
  TObjArray *     fDataArr;            ///< array holding all datasets 
};

} // valor

#endif


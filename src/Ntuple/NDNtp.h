//____________________________________________________________________________ 
/*!

\class     valor::NDNtp

\brief     ND fit n-tuple writer

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   September 16, 2016

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_dune_nd_ntp_h_
#define _valor_dune_nd_ntp_h_

class TFile;
class TTree;
class TObjArray;

namespace valor {

class PhysParamList;
class FitParamList;

class NDNtp
{
public:
  NDNtp();
  virtual ~NDNtp();
   
  void Create (const char * output_file);
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

  TFile *         fOutFile;            ///< Output file
  TTree *         fVTree;              ///< VALOR/DUNE ND tree
  PhysParamList * fTrueParamArr;       ///< array holding true parameters
  FitParamList *  fBestFitParamArr;    ///< array holding best-fit parameters
  TObjArray *     fBestFitModelArr;    ///< array holding best-fit predictions for all datasets
  TObjArray *     fDataArr;            ///< array holding all datasets 
};

} // valor

#endif


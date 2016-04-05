//____________________________________________________________________________
/*!

\class     valor::CorrelatedSystematics

\brief     Info on correlated systematic errors.

\author    Steve Dennis <s.r.dennis \at University of Liverpool>
           University of Liverpool

           Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_corr_syst_h_
#define _valor_corr_syst_h_

#include <iostream>

#include <TObject.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include <TMatrixTSym.h>
#include <TXMLEngine.h>

#include "Conventions/PhysParamId.h"

namespace valor {

class FitParamList;
class PhysParamList;

class CorrelatedSystematics : public TNamed
{
public:
  CorrelatedSystematics(unsigned int n_syst, const conv::PhysParamId_t * syst_params, const std::string & xml_node_name = "syst");
  virtual ~CorrelatedSystematics();

  // Read inputs from XML file
  bool Read (TXMLEngine* xml, XMLNodePointer_t root_node);

  // Get flux covariance matrix and cholesky decomposition of covariance matrix
  const TMatrixD & CovMatrix         (void) const { return *fCovMtrx;          }
  const TMatrixD & CovMatrixInv      (void) const { return *fCovMtrxInv;       }
  const TMatrixD & CovMatrixCholesky (void) const { return *fCovMtrxChoL;      }
  
  const TVectorD & UntunedValues (void) const { return *fUntunedValues;}
  const TVectorD & InitialValues (void) const { return *fInitialValues;}
  const TVectorD & CurrentValues (void) const { return *fCurrentValues;}
  
  double OneSigmaError(conv::PhysParamId_t p) const;
  double OneSigmaErrorByIndex(int idx) const;

  // Penalty term included in the chisq 
  double ChisqPenalty (bool all_syst = true) const;
  
  // Tweak all the systematics, either to a specific vector or randomly with correlations.
  void Tweak(bool force_inputs, TVectorD * input_vec, Option_t * opt = 0);
  
  // Access vector values
  double CurrentValueByIndex(int idx) const;
  double CurrentValue(conv::PhysParamId_t p) const;
  
  double UntunedValueByIndex(int idx) const;
  double UntunedValue(conv::PhysParamId_t p) const;
  
  double InitialValueByIndex(int idx) const;
  double InitialValue(conv::PhysParamId_t p) const;
  
  // Change vector values
  void SetCurrentValueByIndex(int idx, double value);
  void SetCurrentValue(conv::PhysParamId_t p, double value);
  
  // Find out which parameters are at which indices
  unsigned int Size(void) const;
  conv::PhysParamId_t WhichParam(int idx) const;

  // Get flags
  bool IsTweaked   (void) const { return fIsTweaked;   } 
  bool IsConfigured(void) const { return fIsConfigured;}

  // Print
  void PrintSystematicVariations (std::ostream & stream = std::cout) const;
  
  // Interact with FPL
  void UpdateFrom    (const valor::FitParamList  & fpl);
  void UpdateFrom    (const valor::PhysParamList & ppl);
  
  bool AppendParamsTo      (valor::FitParamList  & fpl ) const;
  void CopyTweaksTo        (valor::FitParamList  & fpl ) const;
  void CopyTweaksTo        (valor::PhysParamList & ppl ) const;
  void CopyOneSigmaErrorsTo(valor::PhysParamList & ppl ) const;
  
  // Suppress clang warnings related to TObject
  using TObject::Read;
  using TObject::Draw;
  
  // Name strings
  void SetXMLNodeName(std::string xml_node_name);
  
  std::string GetXMLNodeName(void) const;
    
protected:
  CorrelatedSystematics();

  void Init      (unsigned int n_syst, const conv::PhysParamId_t * syst_params, const std::string & xml_node_name = "syst");
  void CleanUp   (void);
  
  void FillParam2Idx(void);

  int  Param2Idx (conv::PhysParamId_t p) const;     ///< convert flux PhysParamId_t to a covariance matrix or vector index
  conv::PhysParamId_t  Idx2Param (int index) const; ///< convert flux a covariance matrix or vector index to PhysParamId_t
  
  void CholeskyDecomposeMatrix (void);          ///< Cholesky decompose the covariance matrix
  
  TVectorD *  fUntunedValues;     ///< Untuned values of the systematic parameters (all 1 if unspecified).
  TVectorD *  fInitialValues;     ///< Initial values of parameters 
  TVectorD *  fCurrentValues;     ///< Current values of parameters 
  TMatrixD *  fCovMtrx;           ///< Covariance matrix
  TMatrixD *  fCovMtrxInv;        ///< Inverse covariance matrix
  TMatrixD *  fCovMtrxChoL;       ///< Triangular matrix obtained from Cholesky decomposition of covariance matrix
  bool        fIsTweaked;         ///< Were params tweaked?

  double * fUntunedArray; //! ///< Pointer to the actual memory location of the untuned values
  double * fInitialArray; //! ///< Pointer to the actual memory location of the initial values
  double * fCurrentArray; //! ///< Pointer to the actual memory location of the current values
  
  int fParam2Idx[conv::knppmax];
  
  unsigned int fNSystParameters;
  conv::PhysParamId_t * fSystParameters; //[fNSystParameters]
  
  std::string fXMLNodeName;
  
  bool fIsConfigured;
  
ClassDef(CorrelatedSystematics, 1)
};

} //valor

#endif

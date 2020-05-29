//____________________________________________________________________________
/*!
   
\class     valor::PhysHypothesis
      
\brief     This class describes a physics hypothesis (eg. effective 2-flavour
           oscillation, 3-flavour oscillation, 3+N sterile flavour oscillation,
           decay, decoherence, ...).
   
\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   June 06, 2011

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________
   
#ifndef _valor_phys_hypothesis_h_
#define _valor_phys_hypothesis_h_

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "valor/Conventions/PhysHypothesisId.h"
#include "valor/Conventions/PhysParamId.h"
#include "valor/Conventions/Detector.h"
#include "valor/FitCore/FitParamList.h"

using std::ostream;
using std::string;
using std::map;

namespace valor {

class PhysHypothesis
{
public:
   PhysHypothesis(conv::PhysHypothesis_t hypothesis = conv::kHypUndefined);
   PhysHypothesis(string hypothesis);
   PhysHypothesis(const PhysHypothesis & ph);
   PhysHypothesis(const FitParamList & fp);
  ~PhysHypothesis();

   void Init (conv::PhysHypothesis_t h);
   void Copy (const PhysHypothesis & ph);

   conv::PhysHypothesis_t Hypothesis (void) const { return fHypothesis; }
   
   bool Contains (conv::PhysParamId_t p) const;

   double CurrValue    (conv::PhysParamId_t p) const;
   void   SetCurrValue (conv::PhysParamId_t p, double v);
   
   void SetNuisance(conv::PhysParamId_t p, bool isNuisance);
   bool IsNuisance (conv::PhysParamId_t p) const;

   // Print
   void Print (ostream & stream) const;
   friend ostream & operator << (ostream & stream, const PhysHypothesis & ph);

   // Copy values to a PhysParamList 
   valor::PhysParamList AsPhysParamList(void) const;
   void CopyToPhysParamList(valor::PhysParamList & pl) const;

   std::vector<conv::PhysParamId_t> GetParams(void) const;

private:

   conv::PhysHypothesis_t fHypothesis;
   map<int,double>        fParams;
   map<int,bool>          fIsNuisance;
};

ostream & operator<< (ostream & stream, const PhysHypothesis & plst);

  namespace util {
    bool OscProbIsAppliedTo(conv::PhysHypothesis_t h, conv::Detector_t d);
  }

} // valor

#endif

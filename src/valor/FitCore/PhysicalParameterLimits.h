//____________________________________________________________________________
/*!
   
\class     valor::PhysicalParameterLimits
      
\brief     A set of physics parameter limits.
   
\author   Steve Dennis <s.r.dennis \at liverpool.ac.uk>
          University of Liverpool

\created  May 2016

\copyright (c) 2010-2016, The VALOR group.

*/
//____________________________________________________________________________
   
#ifndef _valor_physical_limits_h_
#define _valor_physical_limits_h_

#include <iostream>
#include <string>
#include <map>

#include <TNamed.h>

#include "valor/FitCore/FitParamList.h"
#include "valor/FitCore/PhysParamList.h"
#include "valor/Conventions/PhysParamId.h"

namespace valor {

class PhysicalParameterLimits
{
public:

  static PhysicalParameterLimits * Instance();
  
  double MinValue    (conv::PhysParamId_t p) const;
  void   SetMinValue (conv::PhysParamId_t p, double v);
  double MaxValue    (conv::PhysParamId_t p) const;
  void   SetMaxValue (conv::PhysParamId_t p, double v);
  
  double GetNearestPhysicalValue(conv::PhysParamId_t p, double v) const;
  
  bool IsWithinLimits(conv::PhysParamId_t p, double v) const;
  bool AllWithinLimits(const FitParamList & fparm) const;
  bool AllWithinLimits(const PhysParamList & fparm) const;

  const std::map<int,double> & Minima(void) const;
  const std::map<int,double> & Maxima(void) const;
  
  void SetDefaults(void);

private:

  std::map<int,double> fParamMinima;
  std::map<int,double> fParamMaxima;
  
  
  PhysicalParameterLimits();
  PhysicalParameterLimits(const PhysicalParameterLimits &);
 ~PhysicalParameterLimits();

  static PhysicalParameterLimits * fInstance;

  struct Cleaner {
      void DummyMethodAndSilentCompiler() { }
      ~Cleaner() {
         if (PhysicalParameterLimits::fInstance !=0) {
            delete PhysicalParameterLimits::fInstance;
            PhysicalParameterLimits::fInstance = 0;
         }
      }
  };
  friend struct Cleaner;
  
};

} // valor

#endif

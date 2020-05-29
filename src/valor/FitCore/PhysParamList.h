//____________________________________________________________________________
/*!
   
\class     valor::PhysParamList
      
\brief     A set of physics parameter values.
   
\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   June 05, 2013

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________
   
#ifndef _valor_phys_param_list_h_
#define _valor_phys_param_list_h_

#include <iostream>
#include <string>
#include <map>

#include <TNamed.h>
#include <TVectorD.h>

#include "valor/Conventions/PhysParamId.h"
#include "valor/FitCore/FitParamList.h"

using std::ostream;
using std::string;
using std::map;

namespace valor {
  
class CorrelatedSystematics;

class PhysParamList: public TNamed
{
public:
   PhysParamList();
   PhysParamList(const PhysParamList & pl);
  ~PhysParamList();

   int    NParams  (void) const;
   double Value    (conv::PhysParamId_t p) const;
   void   SetValue (conv::PhysParamId_t p, double v);
   //using TNamed::Clear; //supress clang 'hides overloaded virtual function [-Woverloaded-virtual]' warnings
   void   Clear    (Option_t * option = "");
   void   Append   (const PhysParamList & pl);
   using TNamed::Print; //supress clang 'hides overloaded virtual function [-Woverloaded-virtual]' warnings
   void   Print    (ostream & stream) const;
   
   bool Contains (conv::PhysParamId_t p) const;
   void Remove   (conv::PhysParamId_t p);

   double           operator () (conv::PhysParamId_t p) const;
   friend ostream & operator << (ostream & stream, const PhysParamList & pl);

   const map<int,double> & Map(void) const { return fParams; }
   
   // Get the contents of another PPL.
   void CopyFrom(const PhysParamList & ppl); // Wipes current contents
   void MergeFrom(const PhysParamList & ppl, Option_t * opt = 0); // Keeps current contents
   
   // Copy to other formats
   TVectorD AsTVector(const CorrelatedSystematics * ordering);

private:

   map<int,double> fParams;

ClassDef(PhysParamList,1)
};

ostream & operator<< (ostream & stream, const PhysParamList & plst);

} // valor

#endif

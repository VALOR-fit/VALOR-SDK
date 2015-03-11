//____________________________________________________________________________
/*!
   
\class     valor::PhysParamList
      
\brief     A set of physics parameter values.
   
\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   June 05, 2013

\copyright (c) 2010-2014, The VALOR group.

*/
//____________________________________________________________________________
   
#ifndef _valor_phys_param_list_h_
#define _valor_phys_param_list_h_

#include <iostream>
#include <string>
#include <map>

#include <TNamed.h>

#include "Conventions/PhysParamId.h"
#include "FitCore/FitParamList.h"

using std::ostream;
using std::string;
using std::map;

namespace valor {

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

private:

   map<int,double> fParams;

ClassDef(PhysParamList,1)
};

ostream & operator<< (ostream & stream, const PhysParamList & plst);

} // valor

#endif

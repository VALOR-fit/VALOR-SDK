//________________________________________________________________________________________
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <iomanip>
#include <cstdlib>

#include "FitCore/PhysParamList.h"
#include "Conventions/PhysParamId.h"

using std::cout;
using std::cerr;
using std::endl;
using std::setw;
using std::setprecision;

using namespace valor;
using namespace valor::conv;

ClassImp(PhysParamList)

//________________________________________________________________________________________
namespace valor {
  ostream & operator << (ostream & stream, const PhysParamList & pl)
  {
    pl.Print(stream);
    return stream;
  }
}
//________________________________________________________________________________________
PhysParamList::PhysParamList() :
TNamed()
{
  this->fParams.clear();
}
//________________________________________________________________________________________
PhysParamList::PhysParamList(const PhysParamList & pl) :
TNamed()
{
  this->fParams.clear();

  map<int,double>::const_iterator it = pl.fParams.begin();
  for( ; it != pl.fParams.end(); ++it) {
     PhysParamId_t p = (PhysParamId_t) it->first;
     double        v = it->second;
     this->SetValue(p,v);
  }
}
//________________________________________________________________________________________
PhysParamList::~PhysParamList()
{
  fParams.clear();
}
//________________________________________________________________________________________
int PhysParamList::NParams(void) const
{
  return (int) fParams.size();
}
//________________________________________________________________________________________
double PhysParamList::Value(PhysParamId_t p) const
{
  map<int,double>::const_iterator it = fParams.find(p);
  if(it == fParams.end()) {
     cerr << "PhysParamList >> Parameter [" 
          << util::RawParamAsString(p) << "] was not set!" << endl;
     return -99999;
  } 
  return it->second;
}
//________________________________________________________________________________________
void PhysParamList::SetValue(PhysParamId_t p, double v)
{
  fParams[p] = v;
}
//________________________________________________________________________________________
void PhysParamList::Clear(Option_t *)
{
  this->fParams.clear();
}
//________________________________________________________________________________________
void PhysParamList::Append(const PhysParamList & pl)
{
   const map<int,double> & m = pl.Map();
   map<int,double>::const_iterator iter = m.begin();
   for( ; iter != m.end(); ++iter) {
      PhysParamId_t p = (PhysParamId_t) iter->first;
      double v = iter->second;
      this->SetValue(p,v);
   }
}
//________________________________________________________________________________________
void PhysParamList::Print(ostream & stream) const
{
  int i=0;
  stream << endl;
  map<int,double>::const_iterator it = fParams.begin();
  for( ; it != fParams.end(); ++it) {
     PhysParamId_t p = (PhysParamId_t) it->first;
     double        v = it->second;
     stream << setw(5) << i++
            << setw(30) << util::RawParamAsString(p) << " = "
            << setprecision(8) << setw(12) << v
            << endl;      
  }
}
//________________________________________________________________________________________
double PhysParamList::operator () (conv::PhysParamId_t p) const
{
  return this->Value(p);
}
//________________________________________________________________________________________


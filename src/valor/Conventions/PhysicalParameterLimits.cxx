//________________________________________________________________________________________
/*

Copyright (c) 2010-2015, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <string>
#include <algorithm>
#include <iostream>

#include <TROOT.h>

#include "valor/Conventions/PhysicalParameterLimits.h"
#include "valor/Conventions/ParameterPool.h"
#include "valor/Utilities/NameRegistry.h"

using namespace valor;
using namespace valor::conv;

using std::endl;
using std::cout;
using std::cerr;
using std::map;

//#define show_debug_limits

//____________________________________________________________________________
PhysicalParameterLimits * PhysicalParameterLimits::fInstance = 0;
//____________________________________________________________________________
valor::PhysicalParameterLimits::PhysicalParameterLimits()
{
  this->SetDefaults();
  fInstance = 0;
}
//____________________________________________________________________________
valor::PhysicalParameterLimits::~PhysicalParameterLimits()
{
  return;
}
//____________________________________________________________________________
PhysicalParameterLimits * PhysicalParameterLimits::Instance()
{
  if(fInstance == 0) {
    static PhysicalParameterLimits::Cleaner cleaner;
    cleaner.DummyMethodAndSilentCompiler();
    fInstance = new PhysicalParameterLimits;
  }
  return fInstance;
}
//____________________________________________________________________________
double  valor::PhysicalParameterLimits::MinValue    (conv::PhysParamId_t p) const
{
  map<int,double>::const_iterator it = fParamMinima.find(p);
  if(it == fParamMinima.end()) {
     cerr << "PhysicalParameterLimits >> Parameter [" 
          << util::NameRegistry::Instance()->AsString(p) << "] was not set!" << endl;
     return -99999;
  } 
  return it->second;
}
//____________________________________________________________________________
void    valor::PhysicalParameterLimits::SetMinValue (conv::PhysParamId_t p, double v)
{
  fParamMinima[p] = v;
}
//____________________________________________________________________________
double  valor::PhysicalParameterLimits::MaxValue    (conv::PhysParamId_t p) const
{
  map<int,double>::const_iterator it = fParamMaxima.find(p);
  if(it == fParamMaxima.end()) {
     cerr << "PhysicalParameterLimits >> Parameter [" 
          << util::NameRegistry::Instance()->AsString(p) << "] was not set!" << endl;
     return -99999;
  } 
  return it->second;
}
//____________________________________________________________________________
void    valor::PhysicalParameterLimits::SetMaxValue (conv::PhysParamId_t p, double v)
{
  fParamMaxima[p] = v;
}
//____________________________________________________________________________
bool valor::PhysicalParameterLimits::IsWithinLimits(conv::PhysParamId_t p, double v) const
{
  if (v >= this->MinValue(p) && v <= this->MaxValue(p)) {
    return true;
  }
#ifdef show_debug_limits
  std::cout<<"PhysicalParameterLimits/IsWithinLimits >> False for param "<<util::NameRegistry::Instance()->AsString(p)<<" with value "<<v<<std::endl;
#endif
  return false;
}
//____________________________________________________________________________
bool valor::PhysicalParameterLimits::AllWithinLimits(const FitParamList & fparm) const
{
  for (int iparam = 0 ; iparam < fparm.Size(); iparam++) {
    PhysParamId_t p = fparm.WhichParam(iparam);
    double value = fparm.CurrValueByIndex(iparam);
    if (not this->IsWithinLimits(p,value)) {
      return false;
    }
  }
  return true;
}
//____________________________________________________________________________
bool valor::PhysicalParameterLimits::AllWithinLimits(const PhysParamList & pparm) const
{
  for (int iparam = 0 ; iparam < conv::knppmax; iparam++) {
    PhysParamId_t p = PhysParamId_t(iparam);
    if (not pparm.Contains(p)) continue;
    double value = pparm.Value(p);
    if (not this->IsWithinLimits(p,value)) {
      return false;
    }
  }
  return true;
}
//____________________________________________________________________________
const map<int,double> & valor::PhysicalParameterLimits::Minima(void) const
{
  return fParamMinima;
}
//____________________________________________________________________________
const map<int,double> & valor::PhysicalParameterLimits::Maxima(void) const
{
  return fParamMaxima;
}
//____________________________________________________________________________
double valor::PhysicalParameterLimits::GetNearestPhysicalValue(conv::PhysParamId_t p, double v) const
{
  if (this->IsWithinLimits(p,v)) {return v;}
  if (v < this->MinValue(p))     {return this->MinValue(p);}
  return this->MaxValue(p);
}
//____________________________________________________________________________
void valor::PhysicalParameterLimits::SetDefaults(void)
{
  for (int ip = 0 ; ip < knppmax; ip++) {
    PhysParamId_t p = (PhysParamId_t)ip;
    fParamMinima[p] = -9E9;
    fParamMaxima[p] =  9E9;
    
    if (util::IsNormalization(p)) {fParamMinima[p] = 0;}
    //~ if (util::IsEnergyScale  (p)) {fParamMinima[p] = 0;}
    if (util::IsMixingAngle  (p)) {fParamMinima[p] = -1; fParamMaxima[p] = 1;}
  }

  for (const Parameter & param : ParameterPool::Instance()->GetParams()) {
    if(param.HasLimits()) {
      const std::pair<double, double> & limits = param.GetLimits();
      fParamMinima[param.GetID()] = limits.first;
      fParamMaxima[param.GetID()] = limits.second;
    }
  }
  
  return;
}

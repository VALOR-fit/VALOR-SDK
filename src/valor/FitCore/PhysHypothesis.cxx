//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

//#define __show_init_mesg_info__

#include <iomanip>
#include <cstdlib>

#include "valor/Conventions/PhysHypothesisId.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/FitCore/PhysParamList.h"
#include "valor/FitCore/PhysHypothesis.h"

using std::cout;
using std::endl;
using std::setw;
using std::setprecision;

using namespace valor;
using namespace valor::conv;

//________________________________________________________________________________________

namespace valor {
  ostream & operator << (ostream & stream, const PhysHypothesis & ph)
  {
    ph.Print(stream);
    return stream;
  }
}
//________________________________________________________________________________________
PhysHypothesis::PhysHypothesis(PhysHypothesis_t h)
{
  this->Init(h);
}
//________________________________________________________________________________________
PhysHypothesis::PhysHypothesis(string sh)
{
  PhysHypothesis_t h = util::PhysHypFromString(sh);
  this->Init(h);
}
//________________________________________________________________________________________
PhysHypothesis::PhysHypothesis(const PhysHypothesis & ph)
{
  this->Copy(ph);
}
//________________________________________________________________________________________
PhysHypothesis::PhysHypothesis(const FitParamList & fp)
{
  PhysHypothesis_t hyp = fp.PhysicsHypothesis();

  // neutrino oscillations (3-flavour framework)
  if(hyp == kHypOsc3F)
  {
    this->Init(kHypOsc3F);
    this->SetCurrValue(kPhypDm12sq,   fp.CurrValue(kPhypDm12sq));
    this->SetCurrValue(kPhypDm23sq,   fp.CurrValue(kPhypDm23sq));
    this->SetCurrValue(kPhypMixing12, fp.CurrValue(kPhypMixing12));
    this->SetCurrValue(kPhypMixing13, fp.CurrValue(kPhypMixing13));
    this->SetCurrValue(kPhypMixing23, fp.CurrValue(kPhypMixing23));
    this->SetCurrValue(kPhypCPV,      fp.CurrValue(kPhypCPV));
  }
  // neutrino oscillations (3-flavour framework)
  else if(hyp == kHypOsc3FBar)
  {
    this->Init(kHypOsc3FBar);
    this->SetCurrValue(kPhypDm12sq,      fp.CurrValue(kPhypDm12sq));
    this->SetCurrValue(kPhypDm23sq,      fp.CurrValue(kPhypDm23sq));
    this->SetCurrValue(kPhypMixing12,    fp.CurrValue(kPhypMixing12));
    this->SetCurrValue(kPhypMixing13,    fp.CurrValue(kPhypMixing13));
    this->SetCurrValue(kPhypMixing23,    fp.CurrValue(kPhypMixing23));
    this->SetCurrValue(kPhypCPV,         fp.CurrValue(kPhypCPV));
    this->SetCurrValue(kPhypDm12sqBar,   fp.CurrValue(kPhypDm12sqBar));
    this->SetCurrValue(kPhypDm23sqBar,   fp.CurrValue(kPhypDm23sqBar));
    this->SetCurrValue(kPhypMixing12Bar, fp.CurrValue(kPhypMixing12Bar));
    this->SetCurrValue(kPhypMixing13Bar, fp.CurrValue(kPhypMixing13Bar));
    this->SetCurrValue(kPhypMixing23Bar, fp.CurrValue(kPhypMixing23Bar));
    this->SetCurrValue(kPhypCPVBar,      fp.CurrValue(kPhypCPVBar));
  }
  else if(hyp == kHypOsc3FBarDissap)
  {
    this->Init(kHypOsc3FBarDissap);
    this->SetCurrValue(kPhypDm12sq,      fp.CurrValue(kPhypDm12sq));
    this->SetCurrValue(kPhypDm23sq,      fp.CurrValue(kPhypDm23sq));
    this->SetCurrValue(kPhypMixing12,    fp.CurrValue(kPhypMixing12));
    this->SetCurrValue(kPhypMixing13,    fp.CurrValue(kPhypMixing13));
    this->SetCurrValue(kPhypMixing23,    fp.CurrValue(kPhypMixing23));
    this->SetCurrValue(kPhypCPV,         fp.CurrValue(kPhypCPV));
    this->SetCurrValue(kPhypDm23sqBar,   fp.CurrValue(kPhypDm23sqBar));
    this->SetCurrValue(kPhypMixing23Bar, fp.CurrValue(kPhypMixing23Bar));
  }


  // NEW BETABAR

  // neutrino oscillations (3-flavour framework), nu/nubar separated + numubar->nuebar osc normalization
  else if(hyp == kHypOsc3FBarNormBar)
  {
    this->Init(kHypOsc3FBarNormBar);
    this->SetCurrValue(kPhypDm12sq,      fp.CurrValue(kPhypDm12sq));
    this->SetCurrValue(kPhypDm23sq,      fp.CurrValue(kPhypDm23sq));
    this->SetCurrValue(kPhypMixing12,    fp.CurrValue(kPhypMixing12));
    this->SetCurrValue(kPhypMixing13,    fp.CurrValue(kPhypMixing13));
    this->SetCurrValue(kPhypMixing23,    fp.CurrValue(kPhypMixing23));
    this->SetCurrValue(kPhypCPV,         fp.CurrValue(kPhypCPV));
    this->SetCurrValue(kPhypDm12sqBar,   fp.CurrValue(kPhypDm12sqBar));
    this->SetCurrValue(kPhypDm23sqBar,   fp.CurrValue(kPhypDm23sqBar));
    this->SetCurrValue(kPhypMixing12Bar, fp.CurrValue(kPhypMixing12Bar));
    this->SetCurrValue(kPhypMixing13Bar, fp.CurrValue(kPhypMixing13Bar));
    this->SetCurrValue(kPhypMixing23Bar, fp.CurrValue(kPhypMixing23Bar));
    this->SetCurrValue(kPhypCPVBar,      fp.CurrValue(kPhypCPVBar));
    this->SetCurrValue(kPhypBetaBar,     fp.CurrValue(kPhypBetaBar));
  }

  // NEW BETABAR 2

  // neutrino oscillations (3-flavour framework), numubar->nuebar osc normalization
  else if(hyp == kHypOsc3FNormBar)
  {
    this->Init(kHypOsc3FNormBar);
    this->SetCurrValue(kPhypDm12sq,      fp.CurrValue(kPhypDm12sq));
    this->SetCurrValue(kPhypDm23sq,      fp.CurrValue(kPhypDm23sq));
    this->SetCurrValue(kPhypMixing12,    fp.CurrValue(kPhypMixing12));
    this->SetCurrValue(kPhypMixing13,    fp.CurrValue(kPhypMixing13));
    this->SetCurrValue(kPhypMixing23,    fp.CurrValue(kPhypMixing23));
    this->SetCurrValue(kPhypCPV,         fp.CurrValue(kPhypCPV));
    this->SetCurrValue(kPhypBetaBar,     fp.CurrValue(kPhypBetaBar));
  }

  //



  // numu disappearance, 2-flavour neutrino oscillation framework 
  else if(hyp == kHypNuMuDisapp2F) 
  {
    this->Init(kHypNuMuDisapp2F);
    this->SetCurrValue(kPhypDm23sq,   fp.CurrValue(kPhypDm23sq));
    this->SetCurrValue(kPhypMixing23, fp.CurrValue(kPhypMixing23));
  }

  // neutrino decay
  else if(hyp == kHypDecay) 
  {
    this->Init(kHypDecay);
    this->SetCurrValue(kPhypAlphaDecay,       fp.CurrValue(kPhypAlphaDecay));
    this->SetCurrValue(kPhypSinsq2ThetaDecay, fp.CurrValue(kPhypSinsq2ThetaDecay));
  }

  // neutrino decoherence
  else if(hyp == kHypDecoh) 
  {
    this->Init(kHypDecoh);
    this->SetCurrValue(kPhypMusqDecoh,          fp.CurrValue(kPhypMusqDecoh));
    this->SetCurrValue(kPhypSinsq2ThetaDecoh,   fp.CurrValue(kPhypSinsq2ThetaDecoh));
  }
  
  // Steriles
  else if(hyp == kHypOscSterile4F || hyp == kHypOscSterile5F ) {
    this->Init(hyp);
    for (int i = 0 ; i < fp.Size() ; i++) {
      PhysParamId_t p = fp.WhichParam(i);
      this->SetCurrValue(p, fp.CurrValue(p));
    }
  }

  else if(hyp == kHypVoid) {
    this->Init(hyp);
    for (int i = 0 ; i < fp.Size() ; i++) {
      PhysParamId_t p = fp.WhichParam(i);
      this->SetCurrValue(p, fp.CurrValue(p));
    }
  }

  // undefined
  else {  
    this->Init(kHypUndefined);
  }
  
// Set nuisance status:
  for (int i = 0 ; i < fp.Size() ; i++) {
    PhysParamId_t p = fp.WhichParam(i);
    this->SetNuisance(p, fp.IsNuisance(p));
  }

#ifdef __show_init_mesg_info__
  cout << " PhysHypothesis ctor > "
       << "Constructed physics hypothesis = " << *this << endl;
#endif
}
//________________________________________________________________________________________
PhysHypothesis::~PhysHypothesis()
{
  fHypothesis = kHypUndefined;
  fParams.clear();
}
//________________________________________________________________________________________
double PhysHypothesis::CurrValue(PhysParamId_t p) const
{
  map<int,double>::const_iterator it = fParams.find(p);
  if(it == fParams.end()) {
     exit(1);
  } 
  return it->second;
}
//________________________________________________________________________________________
void PhysHypothesis::SetCurrValue(PhysParamId_t p, double v)
{
  fParams[p] = v;
}
//________________________________________________________________________________________
void PhysHypothesis::Copy(const PhysHypothesis & ph)
{
  fHypothesis = ph.fHypothesis;

  fParams.clear();
  fParams.insert(ph.fParams.begin(), ph.fParams.end());
  
  fIsNuisance.clear();
  fIsNuisance.insert(ph.fIsNuisance.begin(), ph.fIsNuisance.end());
/*
  this->Init(ph.fHypothesis);
  map<int,double>::const_iterator it = ph.fParams.begin();
  for( ; it != ph.fParams.end(); ++it) {
     fParams[it->first] = it->second;
  }
*/
}
//________________________________________________________________________________________
void PhysHypothesis::Init(PhysHypothesis_t h)
{
  fHypothesis = h;

  fParams.clear();

  // numu disappearance, 2-flavour neutrino oscillation framework 
  if(fHypothesis == kHypNuMuDisapp2F) {
    fParams[kPhypMixing23] = 0;
    fParams[kPhypDm23sq]   = 0;
  }
  // neutrino oscillations (3-flavour framework)
  if(fHypothesis == kHypOsc3F) {
    fParams[kPhypMixing12]  = 0;
    fParams[kPhypMixing13]  = 0;
    fParams[kPhypMixing23]  = 0;
    fParams[kPhypCPV     ]  = 0;
    fParams[kPhypDm23sq  ]  = 0;
    fParams[kPhypDm12sq  ]  = 0;
  }
  // neutrino oscillations (3-flavour framework) - with separate mixings for neutrino and antineutrino
  if(fHypothesis == kHypOsc3FBar) {
    fParams[kPhypMixing12   ]  = 0;
    fParams[kPhypMixing13   ]  = 0;
    fParams[kPhypMixing23   ]  = 0;
    fParams[kPhypCPV        ]  = 0;
    fParams[kPhypDm23sq     ]  = 0;
    fParams[kPhypDm12sq     ]  = 0;
    fParams[kPhypMixing12Bar]  = 0;
    fParams[kPhypMixing13Bar]  = 0;
    fParams[kPhypMixing23Bar]  = 0;
    fParams[kPhypCPVBar     ]  = 0;
    fParams[kPhypDm23sqBar  ]  = 0;
    fParams[kPhypDm12sqBar  ]  = 0;
  }

  // neutrino oscillations (3-flavour framework) - with separate 23 sector mixings for neutrino and antineutrino
  if(fHypothesis == kHypOsc3FBar) {
    fParams[kPhypMixing12   ]  = 0;
    fParams[kPhypMixing13   ]  = 0;
    fParams[kPhypMixing23   ]  = 0;
    fParams[kPhypCPV        ]  = 0;
    fParams[kPhypDm23sq     ]  = 0;
    fParams[kPhypDm12sq     ]  = 0;
    fParams[kPhypMixing23Bar]  = 0;
    fParams[kPhypDm23sqBar  ]  = 0;
  }

  // NEW BETABAR
  
  // neutrino oscillations (3-flavour framework) - with separate mixings for neutrino and antineutrino + numubar->nuebar osc normalization
  if(fHypothesis == kHypOsc3FBarNormBar) {
    fParams[kPhypMixing12   ]  = 0;
    fParams[kPhypMixing13   ]  = 0;
    fParams[kPhypMixing23   ]  = 0;
    fParams[kPhypCPV        ]  = 0;
    fParams[kPhypDm23sq     ]  = 0;
    fParams[kPhypDm12sq     ]  = 0;
    fParams[kPhypMixing12Bar]  = 0;
    fParams[kPhypMixing13Bar]  = 0;
    fParams[kPhypMixing23Bar]  = 0;
    fParams[kPhypCPVBar     ]  = 0;
    fParams[kPhypDm23sqBar  ]  = 0;
    fParams[kPhypDm12sqBar  ]  = 0;
    fParams[kPhypBetaBar    ]  = 0;
  }
  //

  // NEW BETABAR 2
  
  // neutrino oscillations (3-flavour framework) - numubar->nuebar osc normalization
  if(fHypothesis == kHypOsc3FNormBar) {
    fParams[kPhypMixing12   ]  = 0;
    fParams[kPhypMixing13   ]  = 0;
    fParams[kPhypMixing23   ]  = 0;
    fParams[kPhypCPV        ]  = 0;
    fParams[kPhypDm23sq     ]  = 0;
    fParams[kPhypDm12sq     ]  = 0;
    fParams[kPhypBetaBar    ]  = 0;
  }
  //
  


  // neutrino decay 
  if(fHypothesis == kHypDecay) {
    fParams[kPhypSinsq2ThetaDecay] = 0;
    fParams[kPhypAlphaDecay]       = 0;
  }
  // neutrino decoherence 
  if(fHypothesis == kHypDecoh) {
    fParams[kPhypSinsq2ThetaDecoh] = 0;
    fParams[kPhypMusqDecoh]        = 0;
  }

}
//________________________________________________________________________________________
void PhysHypothesis::Print(ostream & stream) const
{
  stream << endl;
  stream << "................................................................." << endl;
  stream << "Hypothesis: \n" 
         << util::AsString(fHypothesis) << endl;
  stream << "Parameters: " << endl;
  map<int,double>::const_iterator it = fParams.begin();
  for( ; it != fParams.end(); ++it) {
     PhysParamId_t p = (PhysParamId_t) it->first;
     double        v = it->second;
     stream << " - "
            << setw(15)  << util::NameRegistry::Instance()->AsString(p) << " = "
            << setprecision(8) << setw(12) << v
            << endl;
  }
  stream << "................................................................." << endl;
}
//________________________________________________________________________________________
PhysParamList PhysHypothesis::AsPhysParamList(void) const
{
  PhysParamList pl;
  map<int, double>::const_iterator iter = fParams.begin();
  for( ; iter != fParams.end(); ++iter)
  {
    PhysParamId_t p = (PhysParamId_t)iter->first;
    double v = iter->second;
    pl.SetValue(p,v);
  }
  return pl;
}
//________________________________________________________________________________________
void PhysHypothesis::CopyToPhysParamList(PhysParamList & pl) const
{
  map<int, double>::const_iterator iter = fParams.begin();
  for( ; iter != fParams.end(); ++iter)
  {
    PhysParamId_t p = (PhysParamId_t)iter->first;
    double v = iter->second;
    pl.SetValue(p,v);
  }
  return;
}
//________________________________________________________________________________________
void PhysHypothesis::SetNuisance (conv::PhysParamId_t p, bool isNuisance)
{
  fIsNuisance[p] = isNuisance;
  return;
}
//________________________________________________________________________________________
bool PhysHypothesis::IsNuisance(conv::PhysParamId_t p) const
{
  map<int,bool>::const_iterator it = fIsNuisance.find(p);
  if(it == fIsNuisance.end()) {
    return false; // not set, assume non-nuisance
  }
  return it->second;
}
//________________________________________________________________________________________
bool PhysHypothesis::Contains(conv::PhysParamId_t p) const
{
  map<int,double>::const_iterator it = fParams.find(p);
  return (it != fParams.end());
}
//________________________________________________________________________________________

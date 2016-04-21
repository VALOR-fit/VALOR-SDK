//____________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

*/
//____________________________________________________________________________

#include <iostream>
#include <cstdlib>

#include <TROOT.h>
#include <TClass.h>

#include "FitCore/FitterABC_new.h"
#include "Fitters/FitterFactory.h"
#include "Messenger/Messenger.h"

using std::endl;

using namespace valor;

//____________________________________________________________________________
namespace valor {
  ostream & operator<<(ostream & stream, const FitterFactory & ff)
  {
    ff.Print(stream);
    return stream;
  }
}
//____________________________________________________________________________
FitterFactory * FitterFactory::fgInstance = 0;
//____________________________________________________________________________
FitterFactory::FitterFactory()
{
  fgInstance =  0;
}
//____________________________________________________________________________
FitterFactory::~FitterFactory()
{
// Clean up and report on the concrete algorithms  used in this instance.
// Don't clutter output if exiting in err.

  map<string, FitterABC_new *>::iterator iter;
  for(iter = fFitterPool.begin(); iter != fFitterPool.end(); ++iter) {
    FitterABC_new * fitter = iter->second;
    if(fitter) {
      delete fitter;
      fitter = 0;
    }
  }
  fFitterPool.clear();
  fgInstance = 0;
}
//____________________________________________________________________________
FitterFactory * FitterFactory::Instance()
{
  if(fgInstance == 0) {
    static FitterFactory::Cleaner cleaner;
    cleaner.DummyMethodAndSilentCompiler();
    fgInstance = new FitterFactory;
  }
  return fgInstance;
}
//____________________________________________________________________________
FitterABC_new * FitterFactory::GetFitter (string name)
{
  map<string, FitterABC_new *>::const_iterator iter = fFitterPool.find(name);
  bool found = (iter != fFitterPool.end());
  if(!found) {
     LOG("FitterFactory", pERROR) << "Fitter: " << name << " was not found!";
     return 0;
  }
  return iter->second;
}
//____________________________________________________________________________
bool FitterFactory::AddFitter(string name, string implementation) 
{
  map<string, FitterABC_new *>::const_iterator iter = fFitterPool.find(name);
  bool found = (iter != fFitterPool.end());
  if(found) {
     LOG("FitterFactory", pWARN) << "Fitter: " << name << " already exists";
     return false;
  } 
  
  LOG("FitterFactory", pDEBUG) 
      << "Instantiating fitter: " << name
      << " with the following implementation: " << implementation;

  TClass * tclass = TClass::GetClass(implementation.c_str());
  if(!tclass) {
       LOG("FitterFactory", pERROR) 
           << "Failed instantiating fitter: " << name
           << " with the following implementation: " << implementation;
       return false;
  }
  void * base = tclass->New();
  FitterABC_new * fitter = (FitterABC_new *) (base);
  if(!fitter) {
      LOG("FitterFactory", pERROR) << "Null fitter";
      return false;
  }

  pair<string, FitterABC_new *> pair(name, fitter);
  fFitterPool.insert(pair);
  return true;
}
//____________________________________________________________________________
void FitterFactory::Print(ostream & stream) const
{
  string frame(100,'.');

  stream << endl;
  map<string, FitterABC_new *>::const_iterator iter;
  for(iter = fFitterPool.begin(); iter != fFitterPool.end(); ++iter) {
    const string name = iter->first;
    const FitterABC_new * fitter = iter->second;
    stream << frame << endl;
    stream << "Fitter: " << name << endl;
    stream << "Config:";
    //stream << fitter->GetConfig();
  }
  stream << frame << endl;
}
//____________________________________________________________________________


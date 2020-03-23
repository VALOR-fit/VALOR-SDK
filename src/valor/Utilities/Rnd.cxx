//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <iostream>
#include <cassert>

#include "valor/Utilities/Rnd.h"
#include "valor/CmdLine/CmdLnArgList.h"
#include "valor/Messenger/Messenger.h"
//#include "Build.h"

using std::cerr;
using std::endl;

using namespace valor;

//____________________________________________________________________________
namespace valor {
  ostream & operator<<(ostream & stream, const Rnd & rnd)
  {
    rnd.Print(stream);
    return stream;
  }
}
//____________________________________________________________________________
Rnd * Rnd::fInstance = 0;
//____________________________________________________________________________
Rnd::Rnd()
{
  this->SetSeeds(); // default seed: 0
  fInstance = 0;
}
//____________________________________________________________________________
Rnd::~Rnd()
{

}
//____________________________________________________________________________
Rnd * Rnd::Instance()
{
  if(fInstance == 0) {
    static Rnd::Cleaner cleaner;
    cleaner.DummyMethodAndSilentCompiler();
    fInstance = new Rnd;
  }
  return fInstance;
}
//____________________________________________________________________________
void Rnd::SetSeeds(
  UInt_t toymcstat, UInt_t toymcsyst, UInt_t toymcosc, UInt_t fitter, UInt_t other)
{
  fSeedToyMCstat = toymcstat;
  fSeedToyMCsyst = toymcsyst;
  fSeedToyMCosc  = toymcosc ;
  fSeedFitter    = fitter   ;
  fSeedOther     = other    ;
  
#ifndef __SEED_TRANDOM_ZERO__
  unsigned int randseeds[5] = {};
  // If any seeds are zero, read from /dev/urandom to seed them.
  // /dev/random might be slow on batch machines
  if (! (toymcstat && toymcsyst && toymcosc && fitter && other) ) {
    FILE * devrandom = fopen("/dev/urandom","r");
    size_t ints_read = fread(randseeds, sizeof(unsigned int), 5, devrandom);
    assert(ints_read == 5);
    fclose(devrandom);
  }
    
  fSeedToyMCstat = ( toymcstat ? toymcstat : randseeds[0] ) + fSeedOffset;
  fSeedToyMCsyst = ( toymcsyst ? toymcsyst : randseeds[1] ) + fSeedOffset;
  fSeedToyMCosc  = ( toymcosc  ? toymcosc  : randseeds[2] ) + fSeedOffset;
  fSeedFitter    = ( fitter    ? fitter    : randseeds[3] ) + fSeedOffset;
  fSeedOther     = ( other     ? other     : randseeds[4] ) + fSeedOffset;
#endif

  LOG("Rnd", pDEBUG)
    << "** Setting seeds to random number generators:"
    << "\n   Generator [toyMC stat] --> " << fSeedToyMCstat
    << "\n   Generator [toyMC syst] --> " << fSeedToyMCsyst
    << "\n   Generator [toyMC osc ] --> " << fSeedToyMCosc
    << "\n   Generator [fitter    ] --> " << fSeedFitter
    << "\n   Generator [other     ] --> " << fSeedOther;

  fRndToyMCstat . SetSeed(fSeedToyMCstat);
  fRndToyMCsyst . SetSeed(fSeedToyMCsyst);
  fRndToyMCosc  . SetSeed(fSeedToyMCosc);
  fRndFitter    . SetSeed(fSeedFitter);
  fRndOther     . SetSeed(fSeedOther);
  
}
//____________________________________________________________________________
void Rnd::Print(ostream & stream) const
{
  stream << "[Random number generator seed numbers]" 
         << endl;

  stream << " - Toy MC (Stat) : ";
  if (fSeedToyMCstat != 0) {
    stream << fRndToyMCstat.GetSeed();
  } else {
    stream << " ** randomized ** (" << fRndToyMCstat.GetSeed() << ")";
  }
  stream << endl;

  stream << " - Toy MC (Syst) : ";
  if (fSeedToyMCsyst != 0) {
    stream << fRndToyMCsyst.GetSeed();
  } else {
    stream << " ** randomized ** (" << fRndToyMCsyst.GetSeed() << ")";
  }
  stream << endl;

  stream << " - Toy MC (Osc) : ";
  if (fSeedToyMCosc != 0) {
    stream << fRndToyMCosc.GetSeed();
  } else {
    stream << " ** randomized ** (" << fRndToyMCosc.GetSeed() << ")";
  }
  stream << endl;

  stream << " - Fitter : ";
  if (fSeedFitter != 0) {
    stream << fRndFitter.GetSeed();
  } else {
    stream << " ** randomized ** (" << fRndFitter.GetSeed() << ")";
  }
  stream << endl;

  stream << " - Other : ";
  if (fSeedOther != 0) {
    stream << fRndOther.GetSeed();
  } else {
    stream << " ** randomized ** (" << fRndOther.GetSeed() << ")";
  }
  stream << endl;
}
//________________________________________________________________________________________________________________________________________________________
void Rnd::SetSeeds(const CmdLnArgList & arg, unsigned int  seed_offset)
{
  this->SetSeeds(arg.GetInt("seed-toys-stat"),
                 arg.GetInt("seed-toys-syst"),
                 arg.GetInt("seed-toys-osc" ),
                 arg.GetInt("seed-fitter"   ),
                 arg.GetInt("seed-other"    ));
  fSeedOffset = seed_offset;
  return;
}
//________________________________________________________________________________________________________________________________________________________


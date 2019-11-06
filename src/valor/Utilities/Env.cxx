//________________________________________________________________________________________
/*

Copyright (c) 2010-2016, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <TSystem.h>

#include "valor/Utilities/Env.h"

using std::endl;

using namespace valor;

//________________________________________________________________________________________
namespace valor {
  ostream & operator<<(ostream & stream, const Env & env)
  {
    env.Print(stream);
    return stream;
  }
}
//________________________________________________________________________________________
Env * Env::fInstance = 0;
//________________________________________________________________________________________
Env::Env() 
{
  fTopLevelDir     = "";
  fTopLevelDataDir = "";
  fTopLevelConfDir = "";
  fBanner          = "";
  
  // Set defaults
  const char * topdir = gSystem->Getenv("VALOR_EXPT");
  if(!topdir) {
    topdir = gSystem->Getenv("VALOR_FMWK");
    assert(topdir);
  }
  fTopLevelDir = topdir;
  //fTopLevelDataDir = fTopLevelDir + "/data/";
  fTopLevelConfDir = fTopLevelDir + "/config/";
  fBanner          = fTopLevelDir + "/data/banner/banner.dat";

  const char * datadir = gSystem->Getenv("VALOR_DATA");
  if(!datadir) {
    datadir = gSystem->Getenv("VALOR_FMWK");
    assert(datadir);
  }

  fTopLevelDataDir = datadir;
  

  fInstance = 0;
}
//________________________________________________________________________________________
Env::Env(const Env &) 
{

}
//________________________________________________________________________________________
Env::~Env()
{

}
//________________________________________________________________________________________
const Env * Env::Instance()
{
  return MutableInstance();
}
//________________________________________________________________________________________
Env * Env::MutableInstance()
{
  if(fInstance == 0) {
    static Env::Cleaner cleaner;
    cleaner.DummyMethodAndSilentCompiler();
    fInstance = new Env;
  }
  return fInstance;
}
//________________________________________________________________________________________
void Env::Print(ostream & stream) const
{
  stream << "[Environment]" << endl;
  stream << " - top dir      : " << fTopLevelDir     << endl;
  stream << " - top data dir : " << fTopLevelDataDir << endl;
  stream << " - top conf dir : " << fTopLevelConfDir << endl;
}
//________________________________________________________________________________________

//________________________________________________________________________________________
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <TSystem.h>

#include "Utilities/Env.h"

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
  fAppName         = "";

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
Env * Env::Instance()
{
  if(fInstance == 0) {
    static Env::Cleaner cleaner;
    cleaner.DummyMethodAndSilentCompiler();
    fInstance = new Env;
  }
  return fInstance;
}
//________________________________________________________________________________________
void Env::AutoSet(Option_t * app)
{
  string valor_app = app;

  if(valor_app.find("t2xk") != string::npos) {
    const char * topdir = gSystem->Getenv("T2KOSCANA");
    if(topdir) {
       fTopLevelDir     = topdir;
       fTopLevelDataDir = fTopLevelDir + "/inputs/";
       fTopLevelConfDir = fTopLevelDir + "/config/";
       fAppName         = "t2xk";
    }//topdir
  }
  
  if(valor_app.find("dune") != string::npos) {
    const char * topdir = gSystem->Getenv("VALOR_DUNE");
    if(topdir) {
       fTopLevelDir     = topdir;
       fTopLevelDataDir = fTopLevelDir + "/data/";
       fTopLevelConfDir = fTopLevelDir + "/config/";
       fAppName         = "dune";
    }//topdir
  }
  
  if(valor_app.find("fmwk") != string::npos) {
    const char * topdir = gSystem->Getenv("VALOR_FMWK");
    if(topdir) {
       fTopLevelDir     = topdir;
       fTopLevelDataDir = fTopLevelDir + "/data/";
       fTopLevelConfDir = fTopLevelDir + "/config/";
       fAppName         = "fmwk";
    }//topdir
  }
}
//________________________________________________________________________________________
void Env::Print(ostream & stream) const
{
  stream << "[Environment]" << endl;
  stream << " - top dir      : " << fTopLevelDir     << endl;
  stream << " - top data dir : " << fTopLevelDataDir << endl;
  stream << " - top conf dir : " << fTopLevelConfDir << endl;
  stream << " - running app  : " << fAppName         << endl;
}
//________________________________________________________________________________________

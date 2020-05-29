//____________________________________________________________________________
/*!

\class     valor::Env

\brief     Singleton to keep information about the VALOR environment (paths etc)

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   June 03, 2014

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef __valor_env__
#define __valor_env__

#include <iostream>
#include <string>

using std::ostream;
using std::string;

namespace valor {

class Env {

public:

  static const Env * Instance();
  static Env * MutableInstance();
  
  string TopLevelDir     (void) const { return fTopLevelDir;     }
  string TopLevelDataDir (void) const { return fTopLevelDataDir; }
  string TopLevelConfDir (void) const { return fTopLevelConfDir; }
  string Banner          (void) const { return fBanner;          }

  void SetTopLevelDir     (string dir) { fTopLevelDir     = dir; }
  void SetTopLevelDataDir (string dir) { fTopLevelDataDir = dir; }
  void SetTopLevelConfDir (string dir) { fTopLevelConfDir = dir; }

  void Print(ostream & stream) const;
  friend ostream & operator << (ostream & stream, const Env & env);
  
private:

  Env();
  Env(const Env &);
 ~Env();

  static Env * fInstance;

  string fTopLevelDir;
  string fTopLevelDataDir;
  string fTopLevelConfDir;
  string fBanner;  

  struct Cleaner {
      void DummyMethodAndSilentCompiler() { }
      ~Cleaner() {
         if (Env::fInstance !=0) {
            delete Env::fInstance;
            Env::fInstance = 0;
         }
      }
  };
  friend struct Cleaner;
};

ostream & operator<< (ostream & stream, const Env & env);


}      // valor namespace
#endif // __valor_env__

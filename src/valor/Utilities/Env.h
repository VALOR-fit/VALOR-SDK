//____________________________________________________________________________
/*!

\class     valor::Env

\brief     Singleton to keep information about the VALOR environment (paths etc)

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   June 03, 2014

\copyright (c) 2010-2016, The VALOR group.

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

  static Env * Instance();
  
  string TopLevelDir     (void) { return fTopLevelDir;     }
  string TopLevelDataDir (void) { return fTopLevelDataDir; }
  string TopLevelConfDir (void) { return fTopLevelConfDir; }
  string AppName         (void) { return fAppName;         }
  string Banner          (void) { return fBanner;          }

  void AutoSet (Option_t * valor_application);

  void SetTopLevelDir     (string dir) { fTopLevelDir     = dir; }
  void SetTopLevelDataDir (string dir) { fTopLevelDataDir = dir; }
  void SetTopLevelConfDir (string dir) { fTopLevelConfDir = dir; }
  void SetAppName         (string app) { fAppName         = app; }

  void Print(ostream & stream) const;
  friend ostream & operator << (ostream & stream, const Env & env);

  bool DoWeDie() const;
  void SetDie(bool do_we_die);
  
private:

  Env();
  Env(const Env &);
 ~Env();

  static Env * fInstance;

  string fTopLevelDir;
  string fTopLevelDataDir;
  string fTopLevelConfDir;
  string fAppName;  
  string fBanner;

  // Do we really quit when we hit a util::Die()
  bool fDie;

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

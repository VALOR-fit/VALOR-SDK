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

namespace valor {

class Env {

public:

  static const Env * Instance();
  static Env * MutableInstance();
  
  std::string TopLevelDir     (void) const { return fTopLevelDir;     }
  std::string TopLevelDataDir (void) const { return fTopLevelDataDir; }
  std::string TopLevelConfDir (void) const { return fTopLevelConfDir; }
  std::string Banner          (void) const { return fBanner;          }

  void SetTopLevelDir     (std::string dir) { fTopLevelDir     = dir; }
  void SetTopLevelDataDir (std::string dir) { fTopLevelDataDir = dir; }
  void SetTopLevelConfDir (std::string dir) { fTopLevelConfDir = dir; }

  void Print(std::ostream & stream) const;
  friend std::ostream & operator << (std::ostream & stream, const Env & env);
  
private:

  Env();
  Env(const Env &);
 ~Env();

  static Env * fInstance;

  std::string fTopLevelDir;
  std::string fTopLevelDataDir;
  std::string fTopLevelConfDir;
  std::string fBanner;  

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

std::ostream & operator<< (std::ostream & stream, const Env & env);


}      // valor namespace
#endif // __valor_env__

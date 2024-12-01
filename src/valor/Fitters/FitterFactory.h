//____________________________________________________________________________
/*!

\class     valor::FitterFactory

\brief     Instantiate and store fitters 

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Lab

\created   April 21, 2016

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_fitter_factory_h_
#define _valor_fitter_factory_h_

#include <map>
#include <string>
#include <iostream>

using std::map;
using std::pair;
using std::string;
using std::ostream;

namespace valor {

class FitterABC;
class FitterFactory;

ostream & operator << (ostream & stream, const valor::FitterFactory & ff);

class FitterFactory {

public:

  static FitterFactory * Instance();

  // 
  bool AddFitter(string name, string implementation);

  //
  FitterABC * GetFitter (string name);

  // Print available fitters and their configuration
  void Print(ostream & stream) const;
  friend ostream & operator << (ostream & stream, const FitterFactory & ff);

private:

  FitterFactory();
  FitterFactory(const FitterFactory & ff);
  virtual ~FitterFactory();

  // Self
  static FitterFactory * fgInstance;

  // name -> fitter map
  map<string, FitterABC *> fFitterPool;

  //! singleton cleaner
  struct Cleaner {
      void DummyMethodAndSilentCompiler() { }
      ~Cleaner() {
         if (FitterFactory::fgInstance !=0) {
            delete FitterFactory::fgInstance;
            FitterFactory::fgInstance = 0;
         }
      }
  };
  friend struct Cleaner;
};

}      // valor namespace

#endif // _valor_fitter_factory_h_

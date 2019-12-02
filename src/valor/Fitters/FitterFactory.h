//____________________________________________________________________________
/*!

\class     valor::FitterFactory

\brief     Instantiate and store fitters 

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Lab

\created   April 21, 2016

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_fitter_factory_h_
#define _valor_fitter_factory_h_

#include <map>
#include <string>
#include <iostream>

namespace valor {

class FitterABC_new;
class FitterFactory;

std::ostream & operator << (std::ostream & stream, const valor::FitterFactory & ff);

class FitterFactory {

public:

  static FitterFactory * Instance();

  // 
  bool AddFitter(std::string name, string implementation);

  //
  FitterABC_new * GetFitter (string name);

  // Print available fitters and their configuration
  void Print(std::ostream & stream) const;
  friend std::ostream & operator << (std::ostream & stream, const FitterFactory & ff);

private:

  FitterFactory();
  FitterFactory(const FitterFactory & ff);
  virtual ~FitterFactory();

  // Self
  static FitterFactory * fgInstance;

  // name -> fitter map
  std::map<string, FitterABC_new *> fFitterPool;

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

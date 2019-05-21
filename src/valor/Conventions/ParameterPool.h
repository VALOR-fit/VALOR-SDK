//____________________________________________________________________________
/*!
   
\class     valor::ParameterPool
      
\brief     A singleton to hold the set of parameters currently being used.
   
\author   Steve Dennis <s.r.dennis \at liverpool.ac.uk>
          University of Liverpool

\created  May 2016

\copyright (c) 2010-2016, The VALOR group.

*/
//____________________________________________________________________________
   
#ifndef _valor_parameter_pool_h_
#define _valor_parameter_pool_h_

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "valor/Conventions/Parameter.h"

namespace valor {

class ParameterPool
{
public:

  static ParameterPool * MutableInstance();
  static const ParameterPool * Instance();
  
  void ReadXML(std::string xml_path);
  
  const std::vector<Parameter> & GetParams() const;
  const Parameter & GetParam (conv::PhysParamId_t) const;
  const bool IsParamInPool (conv::PhysParamId_t) const;

  // Get the Parameter corresponding to the input simple name
  const Parameter & ParamFromString (std::string str) const;

  void Print(std::ostream & stream = std::cout) const;
  
  // Make sure we haven't added two parameters with the same name
  void CheckForCollisions() const;
  
private:

  // Tell the framework classes what our parameter names are
  void PopulateNameRegistry(void);

  std::vector<Parameter> fParams;
  
  ParameterPool();
  ParameterPool(const ParameterPool &) {};
 ~ParameterPool() {this->Print();};

  static ParameterPool * fInstance;

  struct Cleaner {
      void DummyMethodAndSilentCompiler() { }
      ~Cleaner() {
         if (ParameterPool::fInstance !=0) {
            delete ParameterPool::fInstance;
            ParameterPool::fInstance = 0;
         }
      }
  };
  friend struct Cleaner;
  
};

} // valor

#endif

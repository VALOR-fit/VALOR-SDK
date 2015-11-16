//____________________________________________________________________________
/*!

\class     valor::util::NameRegistry

\brief     Contains maps of VALOR framework enums that allow names to be experiment specific.

\author    Steve Dennis <stephen.dennis \t warwick.ac.uk
           University of Warwick & STFC Rutherford Appleton Laboratory
          
\created   Nov 16, 2015

\copyright (c) 2010-2015, The VALOR group

*/
//____________________________________________________________________________

#ifndef valor_name_registry
#define valor_name_registry

#include <string>
#include <map>

#include <Conventions/PhysParamId.h>

namespace valor {
  
namespace util {

  // Give access to experiment-specific names within the framework.
  // Must populate in experiment code to do this.
  // See T2XK::PopulateNameRegistry for example (called in root2k.C)
  class NameRegistry {
    public:
      static const NameRegistry * Instance();
      static NameRegistry * MutableInstance();
      
      std::string ParamAsString      (conv::PhysParamId_t p) const;
      std::string ParamAsStringSimple(conv::PhysParamId_t p) const;
      
      // Overloaded aliases for above
      std::string AsString      (conv::PhysParamId_t p) const;
      std::string AsStringSimple(conv::PhysParamId_t p) const;
      
      // Maps for storing param names
      std::map<conv::PhysParamId_t, std::string> fParamNames;
      std::map<conv::PhysParamId_t, std::string> fParamNamesSimple;
      
    private:
      NameRegistry();
      NameRegistry(const NameRegistry &) {}
      ~NameRegistry() {}
      void Init();
      
      static NameRegistry * fInstance;
      struct Cleaner {
        void DummyMethodAndSilentCompiler() { } 
        ~Cleaner() {
          if (NameRegistry::fInstance !=0) {
            delete NameRegistry::fInstance;
            NameRegistry::fInstance = 0;
          }
        }
      };
      friend struct Cleaner;
  };

} // util

} // valor
  
#endif // valor_name_registry_h


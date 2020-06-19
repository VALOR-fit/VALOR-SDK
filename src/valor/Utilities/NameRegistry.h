//____________________________________________________________________________
/*!

\class     valor::util::NameRegistry

\brief     Contains maps of what names correspond to which numerical
*          values at run-time - can be used to check later if files are
*          compatible.

\author    Steve Dennis <sdennis \at hep.phy.cam.ac.uk>
           University of Cambridge
          
\created   Nov 16, 2015

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef valor_name_registry
#define valor_name_registry

#include <string>
#include <map>

#include "valor/Conventions/ReacMode.h"
#include "valor/Conventions/BeamConf.h"
#include "valor/Conventions/Detector.h"
#include "valor/Conventions/Sample.h"
#include "valor/Conventions/NuclearTarget.h"
#include "valor/Conventions/PhysParamId.h"

namespace valor {
  
namespace util {

  class NameRegistry {
    public:
      // Accessors for the global version.
      static const NameRegistry * Instance();
      static NameRegistry * MutableInstance();
      
      // And methods for any others
      NameRegistry();
      NameRegistry(const NameRegistry &) = default;
      ~NameRegistry() {}
      
      // Input DST types.
      std::string ModeAsString          (conv::ReacMode_t      m ) const;
      std::string SampleAsString        (conv::Sample_t        s ) const;
      std::string DetectorAsString      (conv::Detector_t      d ) const;
      std::string BeamConfAsString      (conv::BeamConf_t      b ) const;
      std::string NuclearTargetAsString (conv::NuclearTarget_t nt) const;
      // Overloaded aliases for above
      std::string AsString (conv::ReacMode_t      m ) const;
      std::string AsString (conv::Sample_t        s ) const;
      std::string AsString (conv::Detector_t      d ) const;
      std::string AsString (conv::BeamConf_t      b ) const;
      std::string AsString (conv::NuclearTarget_t nt) const;
      // And reversing the process (case sensitive)
      conv::ReacMode_t      ModeFromString          (const std::string &);
      conv::Sample_t        SampleFromString        (const std::string &);
      conv::Detector_t      DetectorFromString      (const std::string &);
      conv::BeamConf_t      BeamConfFromString      (const std::string &);
      conv::NuclearTarget_t NuclearTargetFromString (const std::string &);
      
      
      // Parameter types - used for output FPL trees.
      std::string ParamAsString          (conv::PhysParamId_t p) const;
      std::string ParamAsStringSimple    (conv::PhysParamId_t p) const;
      std::string ParamAsStringVerySimple(conv::PhysParamId_t p) const;
      std::string ParamAsStringTex       (conv::PhysParamId_t p) const;
      std::string ParamAsStringRooTex    (conv::PhysParamId_t p) const;

      // Overloaded aliases for above
      std::string AsString          (conv::PhysParamId_t p) const;
      std::string AsStringSimple    (conv::PhysParamId_t p) const;
      std::string AsStringVerySimple(conv::PhysParamId_t p) const;
      std::string AsStringTex       (conv::PhysParamId_t p) const;
      std::string AsStringRooTex    (conv::PhysParamId_t p) const;

      // Reversing the process (IGNORES CASE)
      conv::PhysParamId_t PhysParamFromString(const std::string &) const;
      
      // Check for differences between two
      bool HardcodedEnumsMatch(const NameRegistry &) const;
      bool ParametersMatch    (const NameRegistry &) const; // only checks main name
      
      // Maps for storing param names
      std::map<conv::PhysParamId_t, std::string> fParamNames;
      std::map<conv::PhysParamId_t, std::string> fParamNamesSimple;
      std::map<conv::PhysParamId_t, std::string> fParamNamesVerySimple;
      std::map<conv::PhysParamId_t, std::string> fParamNamesTex;
      std::map<conv::PhysParamId_t, std::string> fParamNamesRooTex;

    private:
    
      // Input members
      std::vector<std::string> fModeNames;
      std::vector<std::string> fSampleNames;
      std::vector<std::string> fDetectorNames;
      std::vector<std::string> fBeamConfNames;
      std::vector<std::string> fNuclearTargetNames;
      
        
      // Singleton
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


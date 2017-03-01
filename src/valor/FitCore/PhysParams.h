/*!

\class     valor::PhysParams

\brief     This class maintains the physics and nuisance parameters available to
           an analysis.

\author    Andy Chappell <A.Chappell \t warwick.ac.uk
           University of Warwick
          
\created   Mar 01, 2017

\copyright (c) 2010-2017, The VALOR group

*/

#ifndef valor_phys_params
#define valor_phys_params

#include <string>
#include <map>

// valor
#include "valor/Conventions/PhysParamId.h"

namespace valor
{

class PhysParams
{
public:
   /**
      Retrieves the instance of the PhysParams singleton.

      \return The PhysParams singelton instance.
   */
   static PhysParams* Instance();

   /**
      Reads the set of parameters for the analysis from a configuration file.

      The form of the configuration file is as follows:

      <?xml version="1.0"?>
      <analysis name="dune_nd_fd_analysis">
         <osc_params>           <!-- The analysis oscillation parameters -->
            <param_list>        <!-- The list of physics parameter keys -->
               kPhypMixing23,
               kPhypMixing13,
               kPhypMixing12,
               kPhypCPV,
               kPhypDm23sq,
               kPhypDm12sq
            </param_list>
         </osc_params>

         <!--
            Define the systematic parameters for the analysis.
            Multiple rule nodes can be defined for specifying various types of
            flux, detector and interaction systematics.
         -->
         <rule name="dune_nd_fhc_numu_flux">
            <detector_mask>dune_fd1ph</detector_mask>
            <beam_mask>lbnf_fhc</beam_mask>
            <flavour_mask>numu</flavour_mask>
            <selection_mask>all</selection_mask>
            <reactype_mask>all</reactype_mask>
            <nuctarget_mask>all</nuctarget_mask>
            <param_list>        <!-- The list of systematic parameter keys -->
               kNuisFHCNuMuFlux00,
               kNuisFHCNuMuFlux01,
               kNuisFHCNuMuFlux02
            </param_list>
         </rule>
      </analysis>

      \param filename The location of the configuration file to read.
   */
   void Read(const std::string& filename);

   /**
      Retrieve the ID of a parameter.

      \param param The parameter whose ID is to be found.
      \return The ID of the specified parameter.
   */
   conv::PhysParamId_t GetParamId(const std::string& param) const;

   /**
      Retrieve the ID of a parameter.

      \param param The parameter whose ID is to be found.
      \return The ID of the specified parameter.
   */
   conv::PhysParamId_t operator[](const std::string& param) const;

   /**
      Retrieve the number of physics and nuisance parameters that have been
      defined.

      \return The number of parameters defined for the analysis.
   */
   int NParams() const { return _nextId; }

   /**
      Destructor.
   */
   ~PhysParams();

private:
   /**
      Constructor.

      This is a singleton, so hide the constructor.
   */
   PhysParams();

   /**
      Adds a parameter to the set of physics and nuisance parameters.

      Attempts to add a parameter that already exists will cause termination of
      the application.

      \param param The parameter to be added to the analysis.
   */
   void AddParameter(const std::string& param);

   /**
      Processes the parameters found in a configuration file parameter list.

      Records the set of parameters specified in a parameter list.
   */
   void ProcessParameters(const std::string& parameterList);

   /*
      Disallow copy, move and assignment.

      There is an explicitly defined destructor, so move should not be defaulted,
      but delete anyway to be sure.
      
      Compiler complained over use of delete against these methods for reasons
      I don't understand, so disallowing the old-fashioned way.
   */
   PhysParams(const PhysParams& obj){ (void)obj; }
   PhysParams(const PhysParams&& obj){ (void)obj; }
   PhysParams& operator=(const PhysParams& obj)
   {
      return const_cast<PhysParams&>(obj);
   }
   PhysParams& operator=(const PhysParams&& obj)
   {
      return const_cast<PhysParams&>(obj);
   }

public:
   static const conv::PhysParamId_t Undefined = conv::kPhypUndefined;

private:
   /// The singleton
   static PhysParams* _instance;

   /// The physics and nuisance parameters available to the analysis
   std::map<std::string, conv::PhysParamId_t> _parameters;
   int _nextId;
};

}

#endif


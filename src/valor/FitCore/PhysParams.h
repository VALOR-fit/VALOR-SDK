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

// root
#include <TXMLEngine.h>

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
      Reads the parameter specification from a configuration file.

      The form of the configuration file is as follows:

      <?xml version="1.0"?>

      <parameters>
         <defaults>
            <values>
               <bestfit>1.0</bestfit>
               <init>1.0</init>
               <minimum>0.0</minimum>
               <maximum>2.0</maximum>
               <stepsize>0.0001</stepsize>
               <scaling>1.0</scaling>
            </values>
            <marginalisation>
               <set>False</set>               <!-- True/False -->
               <priors>
                  <gaus>
                     <mean>0.0</mean>
                     <sigma>1.0</sigma>
                  </gaus>
                  <flat>
                     <minimum>0.0</minimum>
                     <maximum>1.0</maximum>
                  </flat>

                  <user>
                     <histogram></histogram>  <!-- Filename -->
                  </user>
               </priors>
            </marginalisation>
            <properties>
               <fixed>False</fixed>           <!-- True/False -->
               <nuisance>True</nuisance>      <!-- True/False -->
            </properties>
            <errors>
               <migrad>
                 <set>False</set>             <!-- True/False -->
                 <error>0.0</error>
               </migrad>
               <hesse>
                 <set>False</set>             <!-- True/False -->
                 <error>0.0</error>
               </hesse>
               <minos>
                 <set>False</set>             <!-- True/False -->
                 <low_error>0.0</low_error>
                 <high_error>0.0</high_error>
               </minos>
            </errors>
         </defaults>
      </parameters>

      \param filename The location of the configuration file to read.
   */
   void Read(const std::string& filename);

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
   void Select(const std::string& filename);

   /**
      Retrieve the best-fit value for a parameter.

      \param param The parameter to retrieve the value from.
      \return The best fit value of the specified parameter.
   */
   double Bestfit(const std::string& param) const;

   /**
      Retrieve the initial value for a parameter.

      \param param The parameter to retrieve the value from.
      \return The initial value of the specified parameter.
   */
   double Initial(const std::string& param) const;

   /**
      Retrieve the minimum value for a parameter.

      \param param The parameter to retrieve the value from.
      \return The minimum value of the specified parameter.
   */
   double Minimum(const std::string& param) const;

   /**
      Retrieve the maximum value for a parameter.

      \param param The parameter to retrieve the value from.
      \return The maximum value of the specified parameter.
   */
   double Maximum(const std::string& param) const;

   /**
      Retrieve the step size for a parameter.

      \param param The parameter to retrieve the value from.
      \return The step size of the specified parameter.
   */
   double StepSize(const std::string& param) const;

   /**
      Retrieve the scaling for a parameter.

      \param param The parameter to retrieve the value from.
      \return The scaling of the specified parameter.
   */
   double Scaling(const std::string& param) const;

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
      Clear all parameters from the PhysParams singleton.
   */
   void Clear() { _nextId = 0; _parameters.clear(); }

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

      \param parameterList A list of comma-separated parameter values.
   */
   void ProcessParameters(const std::string& parameterList);

   /**
      Sets a particular parameter value.

      The value is read from XML if available, otherwise a default value is
      applied.

      \param value A reference to the variable where the value will be stored.
      \param xml The TXMLEngine to perform the parsing.
      \param node The node within which to search for values.
      \param path The path within the specified node (e.g. "child/grandchild").
      \param def The default value to use if the path does not exist.
   */
   void SetParameterValue(double& value, TXMLEngine* xml, XMLNodePointer_t node,
         const std::string& path, const double def);

   /**
      Processes a parameter to specify all of its values.

      \param xml The TXMLEngine to perform the parsing.
      \param node The node within which to search for values.
   */
   void ProcessParameter(TXMLEngine* xml, XMLNodePointer_t node);

   /**
      Processes default parameter values in a configuration file.

      \param xml The TXMLEngine to perform the parsing.
      \param node The node within which to search for default values.
   */
   void ProcessDefaults(TXMLEngine* xml, XMLNodePointer_t node);

   /**
      Checks if a specified parameter exists.

      This method will terminate the application if the parameter is not found.

      \param param The parameter to check for.
      \param value The name of the parameter value we're attempting to retrieve.
   */
   void StrictCheck(const std::string& param, const std::string& value) const;

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

private:
   struct Param
   {
      /**
         Constructor.

         \param name The name of the parameter.
         \param id The identifier of the parameter.
      */
      Param(const std::string& name, conv::PhysParamId_t id):
         _name{name}, _id{id}, _bestfit{0.0}, _initial{0.0}, _minimum{0.0},
         _maximum{0.0}, _stepsize{0.0}, _scaling{0.0}
      {
      }

      std::string _name;
      conv::PhysParamId_t _id;
      double _bestfit;
      double _initial;
      double _minimum;
      double _maximum;
      double _stepsize;
      double _scaling;
   };

public:
   static const conv::PhysParamId_t Undefined = conv::kPhypUndefined;

private:
   /// The singleton
   static PhysParams* _instance;

   /// The physics and nuisance parameters available to the analysis
   std::map<std::string, Param*> _parameters;
   /// The default parameter settings
   Param* _defaults;
   int _nextId;
};

}

#endif


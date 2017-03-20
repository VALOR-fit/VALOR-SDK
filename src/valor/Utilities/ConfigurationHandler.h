/*!

\class     valor::util::ConfigurationHandler

\brief     This class handles the processing of configuration files.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick
          
\created   Mar 16, 2017

\copyright (c) 2010-2017, The VALOR group

*/

#ifndef valor_util_configuration_handler
#define valor_util_configuration_handler

#include <string>

// Root
#include <TXMLEngine.h>

namespace valor
{
namespace util
{

class ConfigurationHandler
{
public:
   /**
      Constructor.

      This is a singleton, so hide the constructor.
   */
   ConfigurationHandler();

   /**
      Destructor.
   */
   ~ConfigurationHandler();

   /**
      Reads the top-level analysis configuration file.

      \param filename The name of the file to read.
   */
   void ReadAnalysis(const std::string& filename);

private:
   /**
      Reads the samples configuration file.

      XML format:
      <?xml version="1.0"?>
      <sample_list name="analysis_name">
        <sample name="sample_name">
          <detector>dune_nd</detector>
          <beam>lbnf_fhc</beam>
          <selection>numucc0pi1trk</selection>
          <data>observations.root</data>
          <mc>mc/dune_ndfit_2017a_FGT_pt4.xml</mc>
          <data_class>valor::GenericSingleSampleFitData</data_class>
          <model_class>valor::GenericSingleSampleFitModel</model_class>
        </sample>
      </sample_list>

      \param filename The name of the file to read.
   */
   void ReadSampleList(const std::string& filename);

   /**
      Processes sample nodes.

      \param rootNode The sample node to process.
   */
   void ProcessSamples(XMLNodePointer_t rootNode);

   /**
      Reads the parameter list configuration file.

      XML format:
      <?xml version="1.0"?>
      <parameter_list name="name">
	<parameter_group name="flux">
          <parameters>params/systematic/flux/flux_params.xml -->
          <penalty_term type="gaussian">
            <covariance_matrix>
              <filename>flux_covariance.root</filename>
              <objname>flux_covariance</objname>
              <objtype>TMatrixD</objtype>
            </covariance_matrix>
          </penalty_term>
          <postfit>
            <filename>flux_postfit.root</filename>
            <objname>flux_postfit</objname>    
            <objtype>TVectorD</objtype>
          </postfit>
	</parameter_group>
      </parameter_list>

      \param filename The name of the file to read.
   */
   void ReadParameterList(const std::string& filename);

   /**
      Processes oscillation parameters.

      \param rootNode The oscillation parameters node to process.
   */
   void ProcessOscillationParameters(XMLNodePointer_t rootNode);

   /**
      Processes systematic parameters.

      \param rootNode The systematic parameters node to process.
      \param type The type of systematic. This should be either "flux",
         "interaction", or "detector".
   */
   void ProcessSystematicParameters(XMLNodePointer_t rootNode,
         const std::string& type);

   /**
      Reads the parameters configuration file.

      XML format:
      <?xml version="1.0"?>
      <parameters>
         <defaults>
            <bestfit />1.0</bestfit>
            <init>1.0</bestfit>
            <minimum>0.0</minimum>
            <maximum>2.0</maximum>
            <step_size>0.0001</stepsize>
            <scaling>1.0</scaling>
            <fixed>false</fixed>
            <nuisance>true</nuisance>
            <marginalise>false</marginalise>
         </defaults>
         <parameter name="kPhypMixing13">
            <!-- Optionally override the default values above -->
            <errors>
               <migrad>
                  <error>0.0</error>
               </migrad>		
               <hesse>
                  <error>0.0</error>
               </hesse>
               <minos>
                  <low_error>0.0</low_error>
                  <high_error>0.0</high_error>
               </minos>
            </errors>
         </parameter>
      </parameters>
   */
   void ReadParameters(const std::string& filename);

   /**
      Processes default parameter values.

      \param rootNode The defaults node.
   */
   void ProcessDefaults(XMLNodePointer_t rootNode);

   /**
      Processes parameter values.

      \param rootNode The parameters node to process.
   */
   void ProcessParameters(XMLNodePointer_t rootNode);

   /**
      Processes error nodes.

      \param rootNode The errors node to process.
   */
   void ProcessErrors(XMLNodePointer_t rootNode);

   /**
      Helper function to create keys to store configuration information.

      Creates strings of the form <prefix><id><suffix>

      \param prefix The string prefix.
      \param id The id.
      \param suffix The string suffix.
   */
   std::string BuildString(const std::string& prefix, const int id,
         const std::string& suffix);

   /**
      Helper function to create keys to store configuration information.

      Creates strings of the form <prefix><infix><suffix>

      \param prefix The string prefix.
      \param infix The string infix..
      \param suffix The string suffix.
   */
   std::string BuildString(const std::string& prefix, const std::string& infix,
         const std::string& suffix);

private:
   TXMLEngine* _engine;
   int _nextParamId;
};

}
}

#endif


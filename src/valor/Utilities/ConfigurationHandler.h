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
      Reads the parameters configuration file.

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
      Helper function to create keys to store configuration information.

      Creates strings of the form <prefix><id>suffix>

      \param prefix The string prefix.
      \param id The id.
      \param suffix The string suffix.
   */
   std::string BuildString(const std::string& prefix, const int id,
         const std::string& suffix);

private:
   TXMLEngine* _engine;
};

}
}

#endif


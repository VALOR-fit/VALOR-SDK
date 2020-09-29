/*!

\brief     Contains utility functions for oscillation parameters.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick

           Francis Bench <f.bench \at liverpool.ac.uk
           University of Liverpool
          
\created   Jan 21, 2019

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/

#ifndef valor_util_oscillation
#define valor_util_oscillation

#include "valor/CmdLine/CmdLnArgList.h"
#include "valor/FitCore/FitParamList.h"
#include "valor/Utilities/UtlXML.h"
#include "valor/Utilities/OscParamValues.h"

namespace valor
{
namespace util
{
namespace oscillation
{
   /**
      Set the oscillation parameters in a FitParamList to fixed values provided
      by CmdLnArgList or set to be marginalised.

      \param arg The CmdLnArgList containing (or not) oscillation parameter values.
      \param fpl The FitParamList to be updated with the values determined by arg.
   */
   void SetOscillationParameters(CmdLnArgList& arg, FitParamList& fpl);

   /**
      Set the oscillation parameter priors (and associated minimum and maximum
      values in the FitParamList.

      \param arg The CmdLnArgList containing the XML filename from which the
                 priors should be read.
      \param fpl The FitParamList to be updated with the values determined by arg.
   */
   void SetParameterPriors(CmdLnArgList& arg, FitParamList& fpl);

  /**
     Set a single prior from an XML file. The prior can be on a single parameter,
     or a 2D prior on two correlated parameters.

     \param xmleng The XML engine for the prior XML file.
     \param top_node A pointer to a <prior> node.
     \param fpl The FitParamList to be updated with the prior.
     \return True if successful.
  */
   bool SetSinglePrior(TXMLEngine* xmleng, XMLNodePointer_t top_node, FitParamList& fpl );

   /**
      Throw any marginalised oscillation parameters according to the content of
      the FitParamList.

      \param fpl The FitParamList to be updated with thrown values. Prior values
                 and marginalisation settings should already be set in the
                 FitParamList.
   */
   void ThrowOscillationParameters(FitParamList& fpl);
   
    /**
      Set the oscillation parameters of a FitParamList such that no oscillation
      will occur. This sets the mass squared differences to arbitrary non-zero
      values, while setting all other oscillation parameters to zero.

      \param fpl The FitParamList to be updated.
   */
   void SetUnoscillated(FitParamList& fpl);

   /**
    * Randomise a single oscillation parameter according to its prior,
    * then convert the thrown value to the mixing/Dmsq/cpv scheme set in Opt::Instance, if needed.
    *
    * @param p Parameter to randomise
    * @param fpl FPL containing a prior for this parameter
    * @return Randomised value of parameter
    */
   double GetRandomizedParam(conv::PhysParamId_t p, FitParamList& fpl);

   /**
    * Randomise a pair of oscillation parameters according to a 2D histogram prior,
    * then convert the thrown value to the mixing/Dmsq/cpv scheme set in Opt::Instance, if needed.
    *
    * @param params A pair of parameters to randomise from a 2D histogram, accounting for their correlations
    * @param fpl FPL containing a 2D histogram prior for these parameters
    * @return a mapping of the parameter IDs to their randomised values
    */
   std::map<conv::PhysParamId_t,double> GetRandomizedParams2DCorrelated (std::pair<conv::PhysParamId_t,conv::PhysParamId_t> params, FitParamList& fpl);
    /**
    Struct to handle penalty term parameters

    **/
    struct PenaltyTerm{
        double mean;
        double sigma;
        std::string scheme;
        std::string param_names; 
    };


    /**
       Set the penalty term parameters
       \param arg The CmdLnArgList containing the XML filename from which the
                  priors should be read.
       \param penaltyparam The name of the oscillation parameter of interest.
       \return PenaltyTerm structre with assigned values if the penalty term parameters
   **/
   PenaltyTerm SetPenaltyTermParameters(CmdLnArgList& arg, string penaltyparam);

    /**
    Set penalty term parameters from an XML file. So far the only supported  type of penalty is Gaussian.
     TODO: add external penalty

    \param xmleng The XML engine for the prior+penlaty XML file.
    \param top_node A pointer to a <penalty> node.
    \param penaltyparam The name of the oscillation parameter of interest.
    \return PenaltyTerm structre with assigned values if the penalty term parameters
     **/
    PenaltyTerm SetSinglePenalty(TXMLEngine* xmleng, XMLNodePointer_t top_node, string penaltyparam);

    /**
    Increase the value of a single oscillation parameter of interest within a group of points submitted as a single job
   
    \param fpl FitParameterList pointer 
    \param param Name of the parameter 
    \param intial Starting (minimal) value of the  oscillation parameter 
    \param bin Number of the point for which we are calculation the value of the oscillation parameter 
    \param step Step between two points for oscillation parameter
    **/


    void IncreaseFreeParameterVal(FitParamList& fpl, std::string param, double init_val, int bin, double step);

}
}
}

#endif


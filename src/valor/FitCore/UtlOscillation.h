/*!

\brief     Contains utility functions for oscillation parameters.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk
           University of Warwick

           Francis Bench <f.bench \at liverpool.ac.uk
           University of Liverpool
          
\created   Jan 21, 2019

\copyright (c) 2010-2019, The VALOR group

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
   void SetOscillationParameterPriors(CmdLnArgList& arg, FitParamList& fpl);

  /**
     Set a single prior from an XML file. The prior can be on a single parameter,
     or a 2D prior on two correlated parameters.

     \param xmleng The XML engine for the prior XML file.
     \param top_node A pointer to a <prior> node.
     \param fpl The FitParamList to be updated with the prior.
     \return True if successful.
  */
   bool SetSinglePrior(TXMLEngine* xmleng, XMLNodePointer_t top_node, FitParamList& fpl);

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
   double GetRandomizedOscParam(conv::PhysParamId_t p, FitParamList& fpl);

   /**
    * Randomise a pair of oscillation parameters according to a 2D histogram prior,
    * then convert the thrown value to the mixing/Dmsq/cpv scheme set in Opt::Instance, if needed.
    *
    * @param params A pair of parameters to randomise from a 2D histogram, accounting for their correlations
    * @param fpl FPL containing a 2D histogram prior for these parameters
    * @return a mapping of the parameter IDs to their randomised values
    */
   std::map<conv::PhysParamId_t,double> GetRandomizedOscParams2DCorrelated (std::pair<conv::PhysParamId_t,conv::PhysParamId_t> params, FitParamList& fpl);
}
}
}

#endif


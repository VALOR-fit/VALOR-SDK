//____________________________________________________________________________
/*!

\file      Parameter

\brief     Reads and stores a single parameter specified from in an XML file

\note      Expects the parameter to be specified as per the following example:
           <param_list name="dune_nd">

            <param>
              <name>
              <simple> int_ccqe_nu_1 </simple>              <!-- must be specified -->
              <rootex> f_{int^{ccqe}_{#nu ; 1}} </rootex>   <!-- default: simple name -->
              </name>
              <apply_to_range>
                <etrue> 0, inf  </etrue>
                <q2true> 0, 0.3 </q2true>
              </apply_to_range>

              <type> normalisation </type>                  <!-- must be specified -->

              <categories>
                <category> int         </category>          <!-- If the only category is either "mixing", "mass_splitting", or "phase" then the parameter is treated as an oscillation parameter -->
                <category> constrained </category>
                <category> nu          </category>
              </categories>

              <apply_to>                                    <!-- default: apply to all -->
                <detector>  all    </detector>              <!-- Naming from Conventions/FitParamApply.cxx -->
                <beam>      all    </beam>
                <selection> all    </selection>
                <reac_type> ccqe   </reac_type>
                <flav>      nu any </flav>
                <target>    all    </target>
              </apply_to>

              <limits> 0, inf </limits>                     <!-- default depends on type: norm. = [0, inf]; lateral = [-inf, inf]; response = [0, inf] -->

            </param>

            ....

          </param_list>


\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           Steve Dennis <s.r.dennis \at liverpool.ac.uk>
           University of Liverpool

           Francis Bench <f.bench \at liverpool.ac.uk>
           University of Liverpool

\created   June 26, 2018

\copyright (c) 2010-2018, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_parameter_h_
#define _valor_parameter_h_

#include <TXMLEngine.h>
#include <vector>
#include <utility>
#include <map>

#include "valor/Conventions/KineVar.h"
#include "valor/Conventions/SystType.h"
#include "valor/Conventions/FitParamApply.h"

using namespace valor;

namespace valor {
  class Parameter {
    public:
      Parameter(conv::PhysParamId_t param_id, TXMLEngine* xml, XMLNodePointer_t param_node);
      Parameter();
      ~Parameter();
      
      // Stuff I definitely want to keep
      const conv::PhysParamId_t      &   GetID        (void) const { return fParam;         }
      const std::string              &   GetName      (void) const { return fName;          }
      const std::string              &   GetRooTeXName(void) const { return fRooTeXName;    }
      const std::string              &   GetTeXName   (void) const { return fTeXName;       }
      const conv::SystType_t         &   GetType      (void) const { return fType;          }
      const std::vector<std::string> &   GetCategories(void) const { return fCategories;    }
      const bool                     &   HasLimits    (void) const { return fHasLimits;     }
      const std::pair<double,double> &   GetLimits    (void) const { return fLimits;        }
      
      // Print out the parameter names, ranges, type, categories etc.
      virtual void Print(std::ostream & stream = std::cout) const;
      
      // Kinematic ranges if given
      const bool                                  & HasKinematicRange() const {return fHasKinematicRange;}
      const std::vector<conv::KineVar_t>          & GetKineVars      () const {return fKinematicVariables;}
      const std::vector<std::pair<double,double>> & GetKineRanges    () const {return fKinematicRange;}
      const std::pair<double,double>              & GetKineRange(conv::KineVar_t) const; // Needs implementing
      
      
      // Does this need to be here?
      conv::ApplyMask_t           GetApplyMask (void)               const { return fApply;         }
      bool IsAppliedTo(conv::BeamConf_t b, conv::Sample_t s, conv::Detector_t d,
                       conv::ReacMode_t m, conv::NuclearTarget_t nt) const;
                            
      // Search the list of user-specified categories for an entry with content exactly matching to_find (after converting to lower case and removing whitespace)
      bool SearchCategories(std::string to_find, bool one_category_only = false) const;

    protected:
      // The ID number that will be used to store stuff in (eg) output TTrees/FPLs
      conv::PhysParamId_t fParam;
      // Simple parameter name for printing etc.
      std::string fName;

      // ROOT TeX name for displaying on plots
      std::string fRooTeXName;
      // And in normal tex
      std::string fTeXName;

      // Is the parameter normalisation, lateral or a response function
      conv::SystType_t fType;

      // Whether the parameter is a mixing angle, phase, detector systematic etc.
      std::vector<std::string> fCategories;

      // xml file containing parameters
      TXMLEngine* fxml;

      // Node where this parameter is specified within fxml
      XMLNodePointer_t fParamNode;

      // What beams, detectors, true nu flavours, selections, reaction types and targets this parameter should be applied to
      conv::ApplyMask_t fApply;

      // Min. and max. physical limits of the possible values of this parameter
      bool fHasLimits;
      std::pair<double,double> fLimits;
      
      // And if we're applied to a specific parameter range
      bool fHasKinematicRange;
      std::vector<conv::KineVar_t> fKinematicVariables;
      std::vector<std::pair<double,double> >fKinematicRange;

      // Default values of each apply mask
      const conv::DetectorApplyMask_t      fDefApplyDetector      = conv::kdApplyToAll;
      const conv::BeamApplyMask_t          fDefApplyBeam          = conv::kbApplyToAll;
      const conv::FlavApplyMask_t          fDefApplyFlav          = conv::kfApplyToAll;
      const conv::SelecApplyMask_t         fDefApplySelec         = conv::ksApplyToAllND;
      const conv::ReacTypeApplyMask_t      fDefApplyReacType      = conv::ktApplyToAll;
      const conv::NuclearTargetApplyMask_t fDefApplyNuclearTarget = conv::kntApplyToAll;

      void Init(conv::PhysParamId_t param_id, TXMLEngine* xml, XMLNodePointer_t param_node);
      void ReadNames       ();
      // TODO: Sensible defaults for each kinevar in each used binning scheme should be set, but does each parameter apply to every kinevar in each binning scheme?
      void ReadRanges      ();
      void ReadType        ();
      void ReadCategories  ();
      void ReadApplyMaskAll();
      void ReadLimits      ();
      conv::DetectorApplyMask_t      ReadDetectorApplyMask     (std::string node_name) const;
      conv::BeamApplyMask_t          ReadBeamApplyMask         (std::string node_name) const;
      conv::FlavApplyMask_t          ReadFlavApplyMask         (std::string node_name) const;
      conv::SelecApplyMask_t         ReadSelecApplyMask        (std::string node_name) const;
      conv::ReacTypeApplyMask_t      ReadReacTypeApplyMask     (std::string node_name) const;
      conv::NuclearTargetApplyMask_t ReadNuclearTargetApplyMask(std::string node_name) const;

      // For a specified node which should contain a string: Does this node exist and is the string not empty?
      bool IsNodeSpecified(const std::string node_path) const;
      
      // Do we need this? If so, definitely not here
      bool OscProbIsAppliedTo(conv::Detector_t d, conv::BeamConf_t b, conv::Sample_t s,
                              conv::ReacMode_t m);
                              
      // Probably needs to go into whatever the syst list is
      // Tell the framework classes what our parameter names are
      void PopulateNameRegistry(void);

    // TODO: I reckon we can delete all of this
    /*
      string  AsString    (conv::PhysParamId_t p);
      string  AsStringSimple(conv::PhysParamId_t p);
      string  AsStringTeX (conv::PhysParamId_t p, bool root = false);

      // Aliases for above
      string  ParamAsString    (conv::PhysParamId_t p) ;
      string  ParamAsStringTeX (conv::PhysParamId_t p, bool root = false);

      conv::PhysParamId_t PhysParamFromString(string p);

      // If you give a CorrelatedSystematics object, it'll ignore your list's standard systematics and use this set instead
      FitParamList * GetFitParameterList (Option_t * param_list = "standard", const CorrelatedSystematics * syst = 0);

      conv::FitParamGroup_t GetParameterGroup(conv::PhysParamId_t p);
      // Get the list of parameters associated with a set name (eg flux, int, ccqe, fsi, 3fosc)
      std::vector<conv::PhysParamId_t> GetParameterSet(std::string set_name);

      // Return pointer to array of min/max
      const double * EtrueRange (conv::PhysParamId_t p);
      const double * Q2trueRange(conv::PhysParamId_t p);

     */

  };
} // valor


#endif

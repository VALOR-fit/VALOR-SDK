//____________________________________________________________________________
/*!

\file      Parameter

\brief     Reads and stores a single parameter specified from in an XML file

\note      Expects the parameter to be specified as per the following example:
           <param_list name="dune_nd">

            <param>
              <name>
                <simple> int_ccqe_nu_1 </simple>              <!-- must be specified -->
                <long> nu-mode CCQE; Qsq 0-0.3 GeVsq </long>
                <rootex> f_{int^{ccqe}_{#nu ; 1}} </rootex>   <!-- default: simple name -->
                <tex> f_{int^{ccqe}_{\nu ; 1}} </tex>
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
                <osc>       unosc  </osc>
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
#include <iostream>

#include "valor/Conventions/KineVar.h"
#include "valor/Conventions/SystType.h"
#include "valor/Conventions/FitParamApply.h"
#include "valor/Conventions/PhysParamId.h"
#include "valor/Conventions/Sample.h"
#include "valor/Conventions/Detector.h"

using namespace valor;

namespace valor {
  class Parameter {
    public:
      Parameter(conv::PhysParamId_t param_id, TXMLEngine* xml, XMLNodePointer_t param_node);
      Parameter();
      ~Parameter();

      const conv::PhysParamId_t      &   GetID        (void) const { return fParam;         }
      const std::string              &   GetName      (void) const { return fName;          }
      const std::string              &   GetNameLong  (void) const { return fNameLong;      }
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
      const std::pair<double,double>              & GetKineRange(conv::KineVar_t) const;
      

      conv::ApplyMask_t GetApplyMask (void) const { return fApply; }
      bool IsAppliedTo(conv::BeamConf_t b, conv::Sample_t s, conv::Detector_t d,
                       conv::ReacMode_t m, conv::NuclearTarget_t nt) const;
      bool IsAppliedTo(conv::BeamConf_t b, conv::Sample_t s, conv::Detector_t d, conv::NuFlavour_t f,
                       conv::ReacType_t t, conv::NuclearTarget_t nt, bool is_oscillated) const;
      bool IsAppliedTo(conv::ApplyMask_t mask) const;
                            
      // Search the list of user-specified categories for an entry with content exactly matching to_find (after converting to lower case and removing whitespace)
      bool SearchCategories(std::string to_find, bool one_category_only = false) const;

      double IsWithinLimits(double val) const;
      double GetNearestPhysicalValue(double val) const;

    protected:
      // The ID number that will be used to store stuff in (eg) output TTrees/FPLs
      conv::PhysParamId_t fParam;
      // Simple parameter name for printing etc.
      std::string fName;

      // Longer, more descriptive parameter name for more verbose printing
      std::string fNameLong;

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
      const conv::OscModeApplyMask_t       fDefApplyOscMode       = conv::koApplyToAll;
      const conv::SelecApplyMask_t         fDefApplySelec         = conv::ksApplyToAll;
      const conv::ReacTypeApplyMask_t      fDefApplyReacType      = conv::ktApplyToAll;
      const conv::NuclearTargetApplyMask_t fDefApplyNuclearTarget = conv::kntApplyToAll;

      void Init(conv::PhysParamId_t param_id, TXMLEngine* xml, XMLNodePointer_t param_node);
      void ReadNames       ();
      void ReadRanges      ();
      void ReadType        ();
      void ReadCategories  ();
      void ReadApplyMaskAll();
      void ReadLimits      ();
      conv::DetectorApplyMask_t      ReadDetectorApplyMask     (std::string node_name) const;
      conv::BeamApplyMask_t          ReadBeamApplyMask         (std::string node_name) const;
      conv::FlavApplyMask_t          ReadFlavApplyMask         (std::string node_name) const;
      conv::OscModeApplyMask_t       ReadOscModeApplyMask      (std::string node_name) const;
      conv::SelecApplyMask_t         ReadSelecApplyMask        (std::string node_name) const;
      conv::ReacTypeApplyMask_t      ReadReacTypeApplyMask     (std::string node_name) const;
      conv::NuclearTargetApplyMask_t ReadNuclearTargetApplyMask(std::string node_name) const;

      // For a specified node which should contain a string: Does this node exist and is the string not empty?
      bool IsNodeSpecified(const std::string node_path) const;

  };
} // valor


#endif

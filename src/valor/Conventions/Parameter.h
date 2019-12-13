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
                <flav_init>  nu any </flav_init>
                <flav_final> numu  </flav_final>
                <target>    all    </target>
              </apply_to>

              <limits> 0, inf </limits>                     <!-- default depends on type: norm. = [0, inf]; lateral = [-inf, inf]; response = [0, inf] -->

              <!-- Options used to set the FitParamList -->
              <init>        -999  </init>        <!-- Def: -999; if -999, then set in CorrelatedSystematics using the input init values vector; if can't do this, then throw an error -->
              <error>       -999  </error>       <!-- Def: -999; if -999, then set in CorrelatedSystematics using the input init values vector; if can't do this, then throw an error -->
              <range_mode>  sigma </range_mode>  <!-- Use either 'value' or 'sigma'. Def: sigma -->
              <range>       -5, 5 </min>         <!-- Def: -3 if mode = 'sigma', or 0 if mode = 'value' -->
              <step>        0.01  </step>        <!-- Def: (max-min)/10000 -->
              <fixed>       false </fixed>       <!-- Def: false -->
              <nuisance>    true  </nuisance>    <!-- Def: true -->
              <marginalise> true  </marginalise> <!-- Def: true -->

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
      const double                   &   GetInitValue (void) const { return fInit;          }
      const double                   &   GetError     (void) const { return fError;         }
      const std::string              &   GetRangeMode (void) const { return fRangeMode;     }
      const std::pair<double,double> &   GetThrowRange(void) const { return fRange;         }
      const double                   &   GetStepSize  (void) const { return fStepSize;      }
      const bool                     &   IsFixed      (void) const { return fIsFixed;       }
      const bool                     &   IsNuisance   (void) const { return fIsNuisance;    }
      const bool                     &   DoMarginalise(void) const { return fDoMarginalise; }
      const double                   &   OverwriteFlag(void) const { return fOverwriteFlag; }

      // Print out the parameter names, ranges, type, categories etc.
      virtual void Print(std::ostream & stream = std::cout) const;
      
      // Kinematic ranges if given
      const bool                                  & HasKinematicRange() const {return fHasKinematicRange;}
      const std::vector<conv::KineVar_t>          & GetKineVars      () const {return fKinematicVariables;}
      const std::vector<std::pair<double,double>> & GetKineRanges    () const {return fKinematicRange;}
      const std::pair<double,double>              & GetKineRange(conv::KineVar_t) const;
      

      conv::ApplyMask_t GetApplyMask (void) const { return fApply; }
                            
      // Search the list of user-specified categories for an entry with content exactly matching to_find (after converting to lower case and removing whitespace)
      bool SearchCategories(std::string to_find, bool one_category_only = false) const;

      bool IsWithinLimits(double val) const;
      double GetNearestPhysicalValue(double val) const;

      void SetInitValue     (const double val) { fInit         = val; }
      void SetError         (const double val) { fError        = val; }
      void SetThrowRangeLow (const double val) { fRange.first  = val; }
      void SetThrowRangeHigh(const double val) { fRange.second = val; }
      void SetStepSize      (const double val) { fStepSize     = val; }
      void SetDefaultInitValue (void);
      void SetDefaultError     (void);
      void SetDefaultThrowRange(void);
      void SetDefaultStepSize  (void);
      // TODO: may need to add a SetID() to allow the ParameterPool to appropriately assign PhysParamId's


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

      // Parameter initial value
      double fInit;
      // TODO: Update in CorrelatedSystematics if == fOverwriteFlag. If not in CorrelatedSystematics, set to default value

      // Error on the parameter, used only for automatically setting the parameter range when the range mode is 'sigma'
      double fError;
      // TODO: Update in CorrelatedSystematics if == fOverwriteFlag. If not in CorrelatedSystematics, set to default value

      // What the min and max values correspond to, e.g. parameter values, standard deviations,
      // or if the range should be set from the prior
      std::string fRangeMode;

      // The range of allowed values for this parameter used when randomising.
      // This does not have to be equal to the physical range, i.e. fLimits, but both ranges will be used.
      std::pair<double,double> fRange;
      // TODO: Update in CorrelatedSystematics if == fOverwriteFlag. If not in CorrelatedSystematics, set to default value

      // The change in this parameter's value used to scan parameter space when minimizing
      // NOTE: This is NOT used step between marginalisation grid points, that is set when
      // running the marginalisation apps
      double fStepSize;
      // TODO: Update in CorrelatedSystematics if == fOverwriteFlag; update based on the range.  If not in CorrelatedSystematics, set to default value

      // Is this parameter fixed or not?
      bool fIsFixed;

      // Do we treat it as being a nuisance parameter?
      bool fIsNuisance;

      // Should we marginalise this parameter?
      bool fDoMarginalise;

      // If e.g. CorrelatedSystematics tries to read a value and finds that it equals this flag,
      // then it knows it can overwrite this value
      const double fOverwriteFlag = -999;
      // TODO: Check each value read by CorrelatedSystematics against this, and overwrite that value if it was equal to this

      // Default values
      const double      fDefInit           = 1.;
      const double      fDefError          = 1.;
      const std::string fDefRangeMode      = "sigma";
      const double      fDefSymmetricRange = 3.;
      const double      fDefStepSizeFactor = 0.01;
      const bool        fDefIsFixed        = false;
      const bool        fDefIsNuisance     = true;
      const bool        fDefDoMarg         = true;

      void Init(conv::PhysParamId_t param_id, TXMLEngine* xml, XMLNodePointer_t param_node);
      void ReadNames       ();
      void ReadRanges      ();
      void ReadType        ();
      void ReadCategories  ();
      void ReadLimits      ();
      void ReadFitInfo     ();
  };
} // valor


#endif

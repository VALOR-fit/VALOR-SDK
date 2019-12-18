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

              <!--
                  The physical limits of this parameter.
                  Default depends on type: norm. = [0, inf]; lateral = [-inf, inf]; response = [0, inf]
              -->
              <limits> 0, inf </limits>


              <!-- Options used to set the FitParamList -->

              <!-- The initial value of this parameter. Def: 1 -->
              <init> 1.0 </init>

              <!--
                  The 1 sigma error on the initial value. This is ONLY used to set the range and only if
                  the range mode is 'sigma'.
                  Def: 1
              -->
              <error> 0.02 </error>

              <!--
                  This sets what the specified range actually corresponds to. Use either 'value' or 'sigma'.
                  If 'value', then the specified range is taken to be in the same units as the parameter.
                  If 'sigma', then the specified range is taken to be in units of standard deviations away
                  from the initial value. When checking the range, it is converted from 'sigma' mode
                  to 'value' mode automatically, so you should never have to manually convert.
                  Def: sigma
              -->
              <range_mode> sigma </range_mode>

              <!--
                  The value of this parameter will never be varied outside of this range when randomising
                  or minimising.
                  Def: [-3, 3] if mode = 'sigma'

                  NOTE ON RANGE VS LIMITS: Strictly speaking, both do not need to be used, as they are
                  internally combined into one set of limits when checking if a parameter is within
                  its limits; however, this range is purely included for convenience, as, for example,
                  the user may have a parameter with an inital value of 0.9 and 1 sigma error of 0.1 and
                  they wish to impose physical limits of [0, 1] whilst also not allowing the parameter
                  to vary outside of the 3 sigma range around the inital value. In this case, the physical
                  limits and the range are automatically combined to give limits of [0.6, 1].
                  In addition, the range can optionally be automatically updated based on the error and
                  inital value stored in the covariance matrix and inital values vector, respecitvely,
                  while the physical limits can not.
              -->
              <range> -5, 5 </min>

              <!--
                  This sets what the specified step size actually corresponds to. Use either 'value' or 'frac'.
                  If 'value', then the specified step size is taken to be in the same units as the parameter.
                  If 'frac', then the specified step size is taken to be a fraction of the combined range
                  and physical limits.
                  Def: frac
              -->
              <step_mode> frac </step>

              <!-- The step size. Def: 0.01 -->
              <step> 0.01 </step>

              <!-- Is this parameter fixed or free? Def: free (false) -->
              <fixed> false </fixed>

              <!-- Is this a nuisance parameter? Def: nuisance (true) -->
              <nuisance> true </nuisance>

              <!-- Should this parameter be marginalised? Def: marginalise (true) -->
              <marginalise> true </marginalise> <!-- Def: true -->

              <!--
                  Should the inital value, one sigma error, range and step size all be updated based on the
                  input covariance matrix and initial values vectors?
                  Def: Do update (true)

                  NOTE: Currently these values are all updated together, and any values read from this
                  config value are overwritten.
              -->
              <update_from_cov_matrix> true </update_from_cov_matrix>

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

      const conv::PhysParamId_t      &   GetID            (void) const { return fParam;            }
      const std::string              &   GetName          (void) const { return fName;             }
      const std::string              &   GetNameLong      (void) const { return fNameLong;         }
      const std::string              &   GetRooTeXName    (void) const { return fRooTeXName;       }
      const std::string              &   GetTeXName       (void) const { return fTeXName;          }
      const conv::SystType_t         &   GetType          (void) const { return fType;             }
      const std::vector<std::string> &   GetCategories    (void) const { return fCategories;       }
      const bool                     &   HasLimits        (void) const { return fHasLimits;        }
      const std::pair<double,double> &   GetPhysicalLimits(void) const { return fPhysicalLimits;   }
      const double                   &   GetInitValue     (void) const { return fInit;             }
      const double                   &   GetError         (void) const { return fError;            }
      const std::string              &   GetThrowRangeMode(void) const { return fRangeMode;        }    // TODO: Make this an enum instead...
      const std::pair<double,double> &   GetThrowRange    (void) const { return fRange;            }
      const std::string              &   GetStepSizeMode  (void) const { return fStepSizeMode;     }    // TODO: Make this an enum instead...
      const double                   &   GetStepSize      (void) const { return fStepSize;         }
      const bool                     &   IsFixed          (void) const { return fIsFixed;          }
      const bool                     &   IsNuisance       (void) const { return fIsNuisance;       }
      const bool                     &   DoMarginalise    (void) const { return fDoMarginalise;    }
      const bool                     &   DoUpdateFitInfo  (void) const { return fUpdateFitInfo;    }

      // Construct a single set of limits from both the physical limits and the throw range
      std::pair<double,double> GetLimits(void) const;

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

      void SetNameLong      (const std::string              val) { fNameLong       = val; }
      void SetRooTeXName    (const std::string              val) { fRooTeXName     = val; }
      void SetTeXName       (const std::string              val) { fTeXName        = val; }
      void SetType          (const conv::SystType_t         val) { fType           = val; }
      void SetCategories    (const std::vector<std::string> val) { fCategories     = val; }
      void SetHasLimits     (const bool                     val) { fHasLimits      = val; }
      void SetPhysicalLimits(const std::pair<double,double> val) { fPhysicalLimits = val; }
      void SetInitValue     (const double                   val) { fInit           = val; }
      void SetError         (const double                   val) { fError          = val; }
      void SetThrowRangeMode(const std::string              val) { fRangeMode      = val; }
      void SetThrowRange    (const std::pair<double,double> val) { fRange          = val; }
      void SetStepSizeMode  (const std::string              val) { fStepSizeMode   = val; }
      void SetStepSize      (const double                   val) { fStepSize       = val; }
      void SetIsFixed       (const bool                     val) { fIsFixed        = val; }
      void SetIsNuisance    (const bool                     val) { fIsNuisance     = val; }
      void SetDoMarginalise (const bool                     val) { fDoMarginalise  = val; }

      // Was the corresponding node specified in the parameter config file, or did we just set some default?
      const bool & NameLongWasSpecified(void)       const { return fNameLongWasSpecified;       }
      const bool & RooTeXNameWasSpecified(void)     const { return fRooTeXNameWasSpecified;     }
      const bool & TeXNameWasSpecified(void)        const { return fTeXNameWasSpecified;        }
      const bool & CategoriesWasSpecified(void)     const { return fCategoriesWasSpecified;     }
      const bool & HasLimitsWasSpecified(void)      const { return fHasLimitsWasSpecified;      }
      const bool & PhysicalLimitsWasSpecified(void) const { return fPhysicalLimitsWasSpecified; }
      const bool & InitValueWasSpecified(void)      const { return fInitValueWasSpecified;      }
      const bool & ErrorWasSpecified(void)          const { return fErrorWasSpecified;          }
      const bool & ThrowRangeModeWasSpecified(void) const { return fThrowRangeModeWasSpecified; }
      const bool & ThrowRangeWasSpecified(void)     const { return fThrowRangeWasSpecified;     }
      const bool & StepSizeModeWasSpecified(void)   const { return fStepSizeModeWasSpecified;   }
      const bool & StepSizeWasSpecified(void)       const { return fStepSizeWasSpecified;       }
      const bool & IsFixedWasSpecified(void)        const { return fIsFixedWasSpecified;        }
      const bool & IsNuisanceWasSpecified(void)     const { return fIsNuisanceWasSpecified;     }
      const bool & DoMarginaliseWasSpecified(void)  const { return fDoMarginaliseWasSpecified;  }

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
      std::pair<double,double> fPhysicalLimits;
      
      // And if we're applied to a specific parameter range
      bool fHasKinematicRange;
      std::vector<conv::KineVar_t> fKinematicVariables;
      std::vector<std::pair<double,double> >fKinematicRange;

      // Parameter initial value
      double fInit;

      // Error on the parameter, used only for automatically setting the parameter range when the range mode is 'sigma'
      double fError;

      // What the min and max values correspond to, e.g. parameter values, standard deviations,
      // or if the range should be set from the prior
      std::string fRangeMode;

      // The range of allowed values for this parameter used when randomising.
      // This does not have to be equal to the physical range, i.e. fPhysicalLimits, but both ranges will be used.
      std::pair<double,double> fRange;

      // What the step size value corresponds to. If 'value' then it's the absolute step size.
      // If 'frac' then it's the fraction of the range to step by
      std::string fStepSizeMode;

      // The change in this parameter's value used to scan parameter space when minimizing
      // NOTE: This is NOT used step between marginalisation grid points, that is set when
      // running the marginalisation apps
      double fStepSize;

      // Is this parameter fixed or not?
      bool fIsFixed;

      // Do we treat it as being a nuisance parameter?
      bool fIsNuisance;

      // Should we marginalise this parameter?
      bool fDoMarginalise;

      // Should we update the init value (and range if the range mode is 'sigma') from CorrelatedSystematics?
      bool fUpdateFitInfo;

      // Default values
      const double      fDefInit             = 1.;
      const double      fDefError            = 1.;
      const std::string fDefRangeMode        = "sigma";
      const double      fDefSymmetricRange   = 3.;
      const std::string fDefStepSizeMode     = "frac";
      const double      fDefStepSizeFactor   = 0.01;
      const bool        fDefIsFixed          = false;
      const bool        fDefIsNuisance       = true;
      const bool        fDefDoMarg           = true;
      const bool        fDefUpdateFitInfo    = true;
//      const std::string fDefInputValueScheme = fDoNotConvertValueFlag;

      // Was the corresponding node specified in the parameter config file, or did we just set some default?
      bool fNameLongWasSpecified;
      bool fRooTeXNameWasSpecified;
      bool fTeXNameWasSpecified;
      bool fCategoriesWasSpecified;
      bool fHasLimitsWasSpecified;
      bool fPhysicalLimitsWasSpecified;
      bool fInitValueWasSpecified;
      bool fErrorWasSpecified;
      bool fThrowRangeModeWasSpecified;
      bool fThrowRangeWasSpecified;
      bool fStepSizeModeWasSpecified;
      bool fStepSizeWasSpecified;
      bool fIsFixedWasSpecified;
      bool fIsNuisanceWasSpecified;
      bool fDoMarginaliseWasSpecified;

      void Init(conv::PhysParamId_t param_id, TXMLEngine* xml, XMLNodePointer_t param_node);
      void ReadNames       ();
      void ReadRanges      ();
      void ReadType        ();
      void ReadCategories  ();
      void ReadLimits      ();
      void ReadFitInfo     ();
  };

  namespace util {
    const std::string ReadSimpleName(TXMLEngine* xml, const XMLNodePointer_t& param_node);
  }
} // valor


#endif

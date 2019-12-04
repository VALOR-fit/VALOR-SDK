//____________________________________________________________________________
/*!

\class     valor::SingleSampleKineBinning

\brief     Kinematic binning choices for a single sample

           It specifies:
           - a single optimized binning for reco kine quantities (Mondrian type)
           - a corresponding simple binning for reco kine quantities (E2E type)
           - a simple binning for true kine quantities (E2E type) with reaction
             mode granuarity
           
           Configured using an XML file with the following schema:
           ..................................................................
           <?xml version="1.0" encoding="ISO-8859-1"?>

           <sample_binning>

               <!--
                 Single Mondrian binning scheme for reco quantities -->
               <reco_optimized>
                  mondrian_binning_scheme.xml
               </reco_optimized>

               <!--
                 Single edge-to-edge binning scheme for reco quantities -->
               <reco_simple>
                  edge_to_edge_binning_scheme.xml
               </reco_simple>

               <!--
                 Single edge-to-edge binning scheme for treue quantities.
                 Used both for optimized and simple templates
                -->
               <true_simple>
                   <!-- 
                     Applies to all modes unless a mode binning scheme
                     is specified explicitly below -->
                   <default>
                      another_edge_to_edge_binning_scheme.xml
                   <default>
                   <!--
                     Exception for specific modes (optional) -->
                   <a_weird_mode>
                      yet_another_edge_to_edge_binning_scheme.xml
                   </a_weird_mode>
               </true_simple>

           </sample_binning>
           ..................................................................

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

\created   April 7, 2016

\copyright (c) 2010-2016, The VALOR group.

*/
//____________________________________________________________________________

#ifndef __valor_single_sample_kinematic_binning__
#define __valor_single_sample_kinematic_binning__

#include <string>
#include <vector>

#include "valor/KineBinning/VALORBinning.h"
#include "valor/Conventions/BeamConf.h"
#include "valor/Conventions/Sample.h"
#include "valor/Conventions/ReacMode.h"
#include "valor/Conventions/UniqueSampleID.h"

namespace valor {

class SingleSampleKineBinning {

public:
  SingleSampleKineBinning(conv::Detector_t d, conv::BeamConf_t b, conv::Sample_t s, const std::vector<conv::ReacMode_t> & modes);
  SingleSampleKineBinning(UniqueSampleID usid, const std::vector<conv::ReacMode_t> & modes);
 ~SingleSampleKineBinning();

  bool Read(string file);

  const VALORBinning * RecoOptBinning(void)   const { return fRecoOptimizedBinning;    }
  const VALORBinning * RecoSmpBinning(void)   const { return fRecoSimpleBinning;       }
  const VALORBinning * TrueSmpBinning(int im) const { return fTrueSimpleBinning [im];  }
  // Return Simple Binning for the reaction mode, m
  const VALORBinning * TrueSmpBinning(conv::ReacMode_t m) const;
  
  // Cross sections binnings. For now truth is always the same as the simple binning, and reco is always optimised
  const VALORBinning * XsecRecoBinning(void              )  const { return fXsecRecoBinning  ;}
  const VALORBinning * XsecTrueBinning(conv::ReacMode_t m)  const;
  const VALORBinning * XsecTrueBinning(int im            )  const { return fXsecTrueBinning[im];}

  conv::BeamConf_t     GetBeamConf(void)      const { return fBeamConf; }
  conv::Sample_t       GetSample  (void)      const { return fSample;   }
  conv::Detector_t     GetDetector(void)      const { return fDetector; }
  
  // Find the optimised bin containing a simple bin for a reco event
  unsigned int GetRecoOptBin(unsigned int reco_smp_bin) const;
  
  // Get a list of all the kinematic variables included in this binning
  const std::vector<conv::KineVar_t> & KineVars() const;
  const std::vector<conv::KineVar_t> & TrueKineVars() const;
  const std::vector<conv::KineVar_t> & RecoKineVars() const;

  // Get a lit of the modes this binning is defined for
  const std::vector<conv::ReacMode_t> & GetReactionModes() const;

private:

  void Init    (void);
  void CleanUp (void);

  void CheckBinEdges(VALORBinning *opt_binning, VALORBinning *smp_binning) const;
  
  // The list of reaction modes we're using
  std::vector<conv::ReacMode_t> fReactionModes;
  // and a mapping from these to their index in here
  std::vector<long> fReactionModeIndex;

  conv::Detector_t                  fDetector;
  conv::BeamConf_t                  fBeamConf;
  conv::Sample_t                    fSample;
  VALORBinning *                    fRecoOptimizedBinning;     //
  VALORBinning *                    fRecoSimpleBinning;        //
  std::vector<VALORBinning *>       fTrueSimpleBinning;        // Per mode
  std::vector<VALORBinning *>       fTrueSimpleBinningUnique;  // 
  
  
  VALORBinning *              fXsecRecoBinning;        // Binning for Xsec splines
  std::vector<VALORBinning *> fXsecTrueBinning;        // Per mode
  std::vector<VALORBinning *> fXsecTrueBinningUnique;  // 
  
  std::vector<conv::KineVar_t> fKineVars;
  std::vector<conv::KineVar_t> fRecoKineVars;
  std::vector<conv::KineVar_t> fTrueKineVars;
};

} // valor namespace

#endif 

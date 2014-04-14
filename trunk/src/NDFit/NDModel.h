//____________________________________________________________________________
/*!

\class     t2kana::NDModel

\brief     LBNE ND Model class. Derives from FitModelABC

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           George Christodoulou <georgios \at hep.ph.liv.ac.uk>
           University of Liverpool

           Nick Grant <n.grant \at lancaster.ac.uk>
           University of Lancaster

\copyright (c) 2010-2014, The VALOR group

*/
//____________________________________________________________________________

#ifndef _t2kana_nd_model_h_
#define _t2kana_nd_model_h_

#include <string>
#include <TObjArray.h>

#include "FitCore/FitModelABC.h"
#include "FitCore/PhysHypothesis.h"
#include "FitCore/PhysParamList.h"
#include "Utilities/NuOsc3Flavour.h"

namespace t2kana {

class NDModel: public FitModelABC
{
public:
  NDModel();
 ~NDModel();

  //
  // Implement the FitModelABC interface
  // See FitModelABC for method decription
  //

  string        Name                 (void);
  bool          Read                 (string file, Option_t * opt = 0);
  bool          Generate             (const PhysHypothesis & ph);
  void          RandomizeSystematics (void);
  void          TweakSystematic      (string syst_name, double twkdial=0., Option_t * opt = 0); 
  void          TweakSystematic      (conv::PhysParamId_t syst, double twkdial=0., Option_t * opt = 0);
  void          ResetSystematics     (void);  
  void          PrintSystematics     (ostream & stream = std::cout) const;
  PhysParamList Tweaks               (void);
  bool          Generate             (const FitParamList & fp);
  bool          Write                (TObjArray * collection, const char * tag = "model", Option_t * write_opt = 0);
  bool          Write                (const char * filename,  const char * tag = "model", Option_t * write_opt = 0, Option_t * file_opt = 0);

private:


  PhysHypothesis fPhysHypothesis;
  NuOsc3Flavour  fNuOsc3Flavour;
};

} // t2kana

#endif


//____________________________________________________________________________
/*!

\class     valor::CompoundCorrelatedSystematics

\brief     Take lots of small covariance matrices and make a huge one.

\author    Steve Dennis <s.r.dennis \at liverpool.ac.uk>
           University of Liverpool

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_compound_syst_h_
#define _valor_compound_syst_h_

#include <vector>

#include <FitCore/CorrelatedSystematics.h>

namespace valor {

class CompoundCorrelatedSystematics: public CorrelatedSystematics
{
public:
  CompoundCorrelatedSystematics(const std::vector<const valor::CorrelatedSystematics*> & vec,
                                std::string xml_node_name = "syst");
                                
  CompoundCorrelatedSystematics(unsigned int n_cov_matrices,
                                const CorrelatedSystematics * const * syst_objs,
                                std::string xml_node_name = "syst");
                                
  virtual ~CompoundCorrelatedSystematics();

private:
  void CompoundInit(unsigned int n_cov_matrices,
                    const CorrelatedSystematics * const * syst_objs,
                    std::string xml_node_name = "syst");
  
ClassDef(CompoundCorrelatedSystematics, 1)
};

} //valor

#endif

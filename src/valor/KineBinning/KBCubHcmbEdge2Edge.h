//____________________________________________________________________________ 
/*!

\class     valor::KBCubHcmbEdge2Edge

\brief     Multi-dimensional kinematical binning class.           
           Binning in each dimension can be of fixed or variable width, but
           independent of the other dimensions.
           Each bin is a `cuboid'.
           In 2D, adjacent bins share one full side, hence the name.
           See `edge-to-edge tiling', https://en.wikipedia.org/wiki/Tessellation

           The binning is read from XML files.
           The XML schema is given by the following example:

           ...............................................
           <?xml version="1.0" encoding="ISO-8859-1"?>

           <binning>
              <space>
                 Ereco,yreco      <!- all KineSpace_t enums are supported -->
              </space>
              <type> 
                 edge-to-edge 
              </type>
              <Ereco_binning>     
                 <bin_size> fixed </bin_size>
                 <nbins> 20 </nbins>
                 <range> 0.,40. </range>
              </Ereco_binning>
              <yreco_binning>
                 <bin_size> variable </bin_size>
                 <nbins> 11 </nbins>
                 <bin_edges>
                     0.0,  1.2,   1.6,   2.0,   3.0,   5.0,
                    10.0, 20.0, 100.0, 110.0, 125.0, 200.0
                 </bin_edges>
              </yreco_binning>
           </binning>
           ...............................................

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           Steve Dennis <s.r.dennis@liverpool.ac.uk>
           University of Liverpool

\created   May 27, 2016

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_kinematical_binning_cuboid_honeycomb_edge_to_edge_h_
#define _valor_kinematical_binning_cuboid_honeycomb_edge_to_edge_h_

#include <vector>
#include <map>

#include <TNamed.h>

#include "valor/VArray/VALORArray.h"
#include "valor/VArray/VALORArrayBin.h"
#include "valor/Conventions/KineVar.h"
#include "valor/Conventions/KineSpace.h"

namespace valor {

class KBCubHcmbEdge2Edge: public TNamed
{
public:
  KBCubHcmbEdge2Edge();
  KBCubHcmbEdge2Edge(string name, string title);
  virtual ~KBCubHcmbEdge2Edge();

  // Read binning from file
  bool Read(string file);

  /**
    Takes this binning and creates a new binning using the specified subset of
    kinematic variables.

    \param ks The target kinematic space. This must be a strict subset of the
      kinematic space used by the current binning.
    \return The binning with the specified kinematic space.
  */
  KBCubHcmbEdge2Edge* Contract(conv::KineSpace_t ks) const;

  // Get the corresponding kinematical space
  conv::KineSpace_t KineSpace(void) const { return fKineSpace; }

  // Number of dimensions
  unsigned int NDimensions (void) const { return fNDimensions; }

  // List of kinematical variables
  const std::vector<valor::conv::KineVar_t> & KineVars (void) const { return fKineVars; }

  // Dimension -> Variable
  conv::KineVar_t KineVar(unsigned int dimension);

  // Variable -> Dimension
  unsigned int Dimension(conv::KineVar_t kv) const;

  // Number of bins by dimension or kinematical variable
  unsigned int NBins (unsigned int dimension ) const;
  unsigned int NBins (conv::KineVar_t kv     ) const;

  // Number of multi-dimensional bins
  unsigned int NBins (void) const;

  // Fixed or variable width?
  bool FixedWidth    (unsigned int dimension ) const;
  bool FixedWidth    (conv::KineVar_t kv     ) const;
  bool VariableWidth (unsigned int dimension ) const;
  bool VariableWidth (conv::KineVar_t kv     ) const;

  // Global min and max values
  double Minimum (unsigned int dimension ) const;
  double Minimum (conv::KineVar_t kv     ) const;
  double Maximum (unsigned int dimension ) const;
  double Maximum (conv::KineVar_t kv     ) const;

  // Bin min and max values
  double BinMinimum (unsigned int linearized_bin, unsigned int dimension ) const;
  double BinMinimum (unsigned int linearized_bin, conv::KineVar_t kv     ) const;
  double BinMaximum (unsigned int linearized_bin, unsigned int dimension ) const;
  double BinMaximum (unsigned int linearized_bin, conv::KineVar_t kv     ) const;

  // Bin range (min, max)
  std::vector<double> BinRange (unsigned int linearized_bin, unsigned int dimension ) const;
  std::vector<double> BinRange (unsigned int linearized_bin, conv::KineVar_t kv     ) const;

  // Bin width
  double              BinWidth (unsigned int linearized_bin, unsigned int dimension ) const;
  double              BinWidth (unsigned int linearized_bin, conv::KineVar_t kv     ) const;
  std::vector<double> BinWidth (unsigned int linearized_bin                         ) const;

  // Bin centre values
  double              BinCentre (unsigned int linearized_bin, unsigned int dimension ) const;
  double              BinCentre (unsigned int linearized_bin, conv::KineVar_t kv     ) const;
  std::vector<double> BinCentre (unsigned int linearized_bin                         ) const;

  // Bin edge arrays by dimension or kinematical variable
  const double *      BinEdgeArr    (unsigned int dimension ) const;
  const double *      BinEdgeArr    (conv::KineVar_t kv     ) const;
  const VALORArray *  BinEdgeVArr   (unsigned int dimension ) const;
  const VALORArray *  BinEdgeVArr   (conv::KineVar_t kv     ) const;  

  // Find bin
  unsigned int              FindBin           (double x, unsigned int dimension) const;
  unsigned int              FindBin           (double x, conv::KineVar_t kv) const;
  unsigned int              FindBin           (std::vector<double>& xarr) const;
  std::vector<unsigned int> FindBinArr        (std::vector<double> & xarr) const;
  unsigned int              FindLinearizedBin (std::vector<double> & xarr) const;

  unsigned int              LinearizedBin (std::vector<unsigned int> & binarr) const;
  std::vector<unsigned int> BinArr        (unsigned int linearized_bin) const; 
  unsigned int              Bin           (unsigned int linearized_bin, unsigned int dimension) const;  

protected:

  void Init    (void);
  void CleanUp (void);

  conv::KineSpace_t                       fKineSpace;     // kinematical space
  unsigned int                            fNDimensions;   // number of dimensions
  std::vector<conv::KineVar_t>            fKineVars;      // kinematical variables in given space
  std::map<conv::KineVar_t, unsigned int> fKineVar2Dim;   // kinematical variable -> dimension
  std::vector<bool>                       fFixedWidth;    // fixed or variable bin width?
  std::vector<double>                     fMinimum;       // 
  std::vector<double>                     fMaximum;       //
  std::vector<VALORArray*>                fBinEdges;      // bin edges
  
ClassDef(KBCubHcmbEdge2Edge, 1)
};

} // valor

#endif


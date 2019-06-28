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

#include "valor/KineBinning/VALORBinning.h"
#include "valor/VArray/VALORArray.h"
#include "valor/VArray/VALORArrayBin.h"
#include "valor/Conventions/KineVar.h"
#include "valor/Conventions/KineSpace.h"

namespace valor {

class KBCubHcmbEdge2Edge: public VALORBinning
{
public:
  KBCubHcmbEdge2Edge();
  KBCubHcmbEdge2Edge(string name, string title);
  virtual ~KBCubHcmbEdge2Edge();

  // Inherited stuff from VALORBinning
  using VALORBinning::GetNextBin;

  // Read binning from file
  virtual bool Read(string file);

  /**
    Takes this binning and creates a new binning using the specified subset of
    kinematic variables.

    \param ks The target kinematic space. This must be a strict subset of the
      kinematic space used by the current binning.
    \return The binning with the specified kinematic space.
  */
  VALORBinning* Contract(conv::KineSpace_t ks) const;
  
  // Fixed or variable width?
  bool FixedWidth    (unsigned int dimension ) const;
  bool FixedWidth    (conv::KineVar_t kv     ) const;
  bool VariableWidth (unsigned int dimension ) const;
  bool VariableWidth (conv::KineVar_t kv     ) const;
  
  // Numbers of bins (by dimension)
  unsigned int NBins (unsigned int dim  ) const;
  unsigned int NBins (conv::KineVar_t kv) const;
  unsigned int NBins (conv::KineVar_t kv, std::map<conv::KineVar_t, double> other_kv_vals) const;
  
  // Find bin
  unsigned int              FindBin1D         (double x, unsigned int dimension) const;
  unsigned int              FindBin1D         (double x, conv::KineVar_t kv) const;
  std::vector<unsigned int> FindBin1DArr      (std::vector<double> & xarr) const;

  unsigned int              LinearizedBin (std::vector<unsigned int> & binarr) const;
  std::vector<unsigned int> Bin1DArr      (unsigned int linearized_bin) const; 
  unsigned int              Bin1D         (unsigned int linearized_bin, unsigned int dimension) const;  
  unsigned int              Bin1D         (unsigned int linearized_bin, conv::KineVar_t kv) const;  
  
  
  // Bin Edges
  const double * BinEdgeArr(unsigned int dimension ) const;
  const double * BinEdgeArr(conv::KineVar_t kv     ) const;
  
  // Inherited stuff from VALORBinning
  using VALORBinning::NBins;
  // Bin min and max values
  virtual double BinMinimum (unsigned int bin, unsigned int dimension ) const;
  virtual double BinMinimum (unsigned int bin, conv::KineVar_t kv     ) const;
  virtual double BinMaximum (unsigned int bin, unsigned int dimension ) const;
  virtual double BinMaximum (unsigned int bin, conv::KineVar_t kv     ) const;

  // Bin range
  virtual std::vector<double> BinRange (unsigned int bin, unsigned int dimension ) const;
  virtual std::vector<double> BinRange (unsigned int bin, conv::KineVar_t kv     ) const;

  // Bin width
  virtual double              BinWidth (unsigned int bin, unsigned int dimension ) const;
  virtual double              BinWidth (unsigned int bin, conv::KineVar_t kv     ) const;
  virtual std::vector<double> BinWidth (unsigned int bin                         ) const;

  // Bin centre
  virtual double              BinCentre (unsigned int bin, unsigned int dimension ) const;
  virtual double              BinCentre (unsigned int bin, conv::KineVar_t kv     ) const;
  virtual std::vector<double> BinCentre (unsigned int bin                         ) const;

  // Find bin
  virtual unsigned int FindBin (std::vector<double> & xarr) const;
  virtual unsigned int FindBin (std::map<conv::KineVar_t, double> & xarr) const;

  // Returns a vector of TLines that mark the edges of each bin
  virtual std::vector<TLine>  GetBinningTLines(void) const;
  
  // Print a summary of the binning
  virtual void Print(std::ostream & stream = std::cout) const;
  
  
  using TObject::Read; // Tell clang we really do want this

protected:

  void Init    (void);
  void CleanUp (void);
  
  std::vector<bool>                 fFixedWidth; // Are the bins fixed or variable in this dimension?
  std::vector<std::vector<double> > fBinEdges;      // bin edges
  
ClassDef(KBCubHcmbEdge2Edge, 1)
};

} // valor

#endif


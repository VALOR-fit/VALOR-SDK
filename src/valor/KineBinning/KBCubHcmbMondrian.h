//____________________________________________________________________________ 
/*!

\class     valor::KBCubHcmbMondrian

\brief     Multi-dimensional kinematical binning 
           Each bin is a `cuboid'.
           The size of each cuboid bin is completely arbitrary and it is set
           individually for each bin.
           The only requirement is that the set of bins spans the entire space
           without overlaps or gaps.
           See Mondrian, https://en.wikipedia.org/wiki/Piet_Mondrian

           The binning is read from XML files.
           The XML schema is given by the following example:

           ..................................................................
           <?xml version="1.0" encoding="ISO-8859-1"?>

           <binning>
              <space>
                 Ereco,yreco  <!-- all KineSpace_t enums are supported -->
              </space>
              <type>
                 mondrian
              </type>
              <nbins> 
                 12 
              </nbins>

              <!-- Min,max for each kinematical variable -->
              <!-- Variables in the order listed in space name above -->
              <!-- Ereco min, Ereco max, yreco min, yreco max -->
              <bin_0>    0.0,   40.0,   0.00,   0.58 </bin_0>
              <bin_1>    0.0,   40.0,   0.58,   1.00 </bin_1>
              <bin_2>   40.0,  150.0,   0.00,   0.40 </bin_2>
              <bin_3>   40.0,  150.0,   0.40,   0.80 </bin_3>
              <bin_4>   40.0,  180.0,   0.80,   0.95 </bin_4>
              <bin_5>   40.0,  180.0,   0.95,   1.00 </bin_5>
              <bin_6>  150.0,  200.0,   0.00,   0.10 </bin_6>
              <bin_7>  150.0,  200.0,   0.10,   0.35 </bin_7>
              <bin_8>  150.0,  200.0,   0.35,   0.52 </bin_8>
              <bin_9>  150.0,  200.0,   0.52,   0.70 </bin_9>
              <bin_10> 150.0,  200.0,   0.70,   0.80 </bin_10>
              <bin_11> 180.0,  200.0,   0.80,   1.00 </bin_11>
           </binning>
           ..................................................................

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           Steve Dennis <s.r.dennis@liverpool.ac.uk>
           University of Liverpool

\created   May 27, 2016

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_kinematical_binning_cuboid_honeycomb_mondrian_h_
#define _valor_kinematical_binning_cuboid_honeycomb_mondrian_h_

#include <vector>
#include <map>

#include <TNamed.h>
#include <TLine.h>
#include <TH2D.h>

#include "valor/VArray/VALORArray.h"
#include "valor/VArray/VALORArrayBin.h"
#include "valor/Conventions/KineSpace.h"

namespace valor {

class KBCubHcmbEdge2Edge;

class KBCubHcmbMondrian: public TNamed
{
public:
  KBCubHcmbMondrian();
  KBCubHcmbMondrian(string name, string title);
  virtual ~KBCubHcmbMondrian();

  // Read binning from file
  using TObject::Read; // Tell clang we really do want this
  bool Read(string file);

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
  bool Contains(conv::KineVar_t kv) const;

  // Number of multi-dimensional bins 
  int NBins (void) const { return fNBins; }

  /**
    Takes this binning and creates a new binning using the specified subset of
    kinematic variables.

    \param ks The target kinematic space. This must be a strict subset of the
      kinematic space used by the current binning.
    \return The binning with the specified kinematic space.
  */
  KBCubHcmbMondrian* Contract(conv::KineSpace_t ks) const;

  // Global bin and max values
  double Minimum (unsigned int dimension ) const;
  double Minimum (conv::KineVar_t kv     ) const;
  double Maximum (unsigned int dimension ) const;
  double Maximum (conv::KineVar_t kv     ) const;

  // Bin min and max values
  double BinMinimum (unsigned int bin, unsigned int dimension ) const;
  double BinMinimum (unsigned int bin, conv::KineVar_t kv     ) const;
  double BinMaximum (unsigned int bin, unsigned int dimension ) const;
  double BinMaximum (unsigned int bin, conv::KineVar_t kv     ) const;

  // Bin range
  std::vector<double> BinRange (unsigned int bin, unsigned int dimension ) const;
  std::vector<double> BinRange (unsigned int bin, conv::KineVar_t kv     ) const;

  // Bin width
  double              BinWidth (unsigned int bin, unsigned int dimension ) const;
  double              BinWidth (unsigned int bin, conv::KineVar_t kv     ) const;
  std::vector<double> BinWidth (unsigned int bin                         ) const;

  // Bin centre
  double              BinCentre (unsigned int bin, unsigned int dimension ) const;
  double              BinCentre (unsigned int bin, conv::KineVar_t kv     ) const;
  std::vector<double> BinCentre (unsigned int bin                         ) const;

  // Find bin
  unsigned int FindBin (std::vector<double> & xarr) const;

  // An alternative `edge-to-edge' binning of the same kiematical space may be 
  // specified to simplify plotting. If specified, a bin mapping bwteen the
  // `edge-to-edge' and `mondrian' binning schemes will be specified.
  void UseEdge2EdgeBinning(const KBCubHcmbEdge2Edge * kbe2e);
  unsigned int MapFromLinearizedE2E(unsigned int e2e_linearized_bin) const;
  
  std::vector<TLine>  GetBinningTLines(void) const;

  /**
    Creates a T2HD with binning that is compatible with the Mondrian binning
    for the purpose of plotting.

    Note that the T2HD object returned by this method should not be used for any
    purpose other than the production of a Root compatible plot that will look
    like a Mondrian-binned histogram. The total number of events in the histogram,
    once filled will exceed that in the true Mondrian-binned histogram it
    represents and so is not valid for computation.

    \param kv1 The first kinematic variable to be plotted. This variable will be
      ignored if the Mondrian binning is already 2-dimensional. If the Mondrian
      binning has more than 2 dimensions and this variable is not provided the
      application will terminate with an error.
    \param kv2 The second kinematic variable to be plotted. This variable will be
      ignored if the Mondrian binning is already 2-dimensional. If the Mondrian
      binning has more than 2 dimensions and this variable is not provided the
      application will terminate with an error.

    \return A T2HD with bins suitable for Mondrian-binning-like plots.
  */
  TH2D* ToTH2D(conv::KineVar_t kv1 = conv::kvUndefined,
        conv::KineVar_t kv2 = conv::kvUndefined) const;
  
protected:

  void Init    (void);
  void CleanUp (void);

  conv::KineSpace_t                         fKineSpace;           // kinematical space
  unsigned int                              fNDimensions;         // number of dimensions
  std::vector<conv::KineVar_t>              fKineVars;            // kinematical variables in given space
  std::map<conv::KineVar_t, unsigned int>   fKineVar2Dim;         // kinematical variable -> dimension
  unsigned int                              fNBins;               // number of bins
  std::vector<double>                       fBinEdges;            // (xmin,xmax,ymin,ymax,...)_bin0,(xmin,xmax,ymin,ymax,...)_bin1,... 
  std::vector<double>                       fMinimum;             //
  std::vector<double>                       fMaximum;             //
  const KBCubHcmbEdge2Edge *                fKBE2E;               //
  std::map<unsigned int, unsigned int>      fBinMondrianFromE2E;  // e2e linearized bin --> mondrian bin (1:1)
  std::multimap<unsigned int, unsigned int> fBinMondrianToE2E;    // mondrian bin --> e2e linearized bin (1:N) 
  
ClassDef(KBCubHcmbMondrian, 1)

};

} // valor

#endif


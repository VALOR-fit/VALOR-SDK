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
#include <TH2Poly.h>

#include "valor/KineBinning/VALORBinning.h"
#include "valor/VArray/VALORArray.h"
#include "valor/VArray/VALORArrayBin.h"
#include "valor/Conventions/KineSpace.h"

namespace valor {

class KBCubHcmbEdge2Edge;

class KBCubHcmbMondrian: public VALORBinning
{
public:
  KBCubHcmbMondrian();
  KBCubHcmbMondrian(string name, string title);
  virtual ~KBCubHcmbMondrian();

  // Read binning from file
  using TObject::Read; // Tell clang we really do want this
  virtual bool Read(string file);

  /**
    Takes this binning and creates a new binning using the specified subset of
    kinematic variables.

    \param ks The target kinematic space. This must be a strict subset of the
      kinematic space used by the current binning.
    \return The binning with the specified kinematic space.
  */
  KBCubHcmbMondrian* Contract(conv::KineSpace_t ks) const;

  // An alternative `edge-to-edge' binning of the same kiematical space may be 
  // specified to simplify plotting. If specified, a bin mapping bwteen the
  // `edge-to-edge' and `mondrian' binning schemes will be specified.
  void UseEdge2EdgeBinning(const KBCubHcmbEdge2Edge * kbe2e);
  unsigned int MapFromLinearizedE2E(unsigned int e2e_linearized_bin) const;
  
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
        
  /** Alternatively, just make a TH2Poly, which is a similar thing to the Mondrian anyway
   */
  TH2Poly* ToTH2Poly(conv::KineVar_t kv1 = conv::kvUndefined,
        conv::KineVar_t kv2 = conv::kvUndefined) const;
        
        
        
  // Inherited stuff from VALORBinning
  using VALORBinning::NBins;
  using VALORBinning::GetNextBin;

  // Bin min and max values
  virtual double BinMinimum (unsigned int bin, unsigned int dimension ) const;
  virtual double BinMaximum (unsigned int bin, unsigned int dimension ) const;
  using VALORBinning::BinMinimum;
  using VALORBinning::BinMaximum;
  
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
  
  // Returns a vector of TLines that mark the edges of each bin
  virtual std::vector<TLine>  GetBinningTLines(void) const;
  
  // Print a summary of the binning
  virtual void Print(std::ostream & stream = std::cout) const;
  
  
protected:

  void Init    (void);
  void CleanUp (void);

  std::vector<double>                       fBinEdges;            // (xmin,xmax,ymin,ymax,...)_bin0,(xmin,xmax,ymin,ymax,...)_bin1,... 
  const KBCubHcmbEdge2Edge *                fKBE2E;               //
  std::map<unsigned int, unsigned int>      fBinMondrianFromE2E;  // e2e linearized bin --> mondrian bin (1:1)
  std::multimap<unsigned int, unsigned int> fBinMondrianToE2E;    // mondrian bin --> e2e linearized bin (1:N) 
  
ClassDef(KBCubHcmbMondrian, 1)

};

} // valor

#endif


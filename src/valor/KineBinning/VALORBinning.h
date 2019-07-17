//____________________________________________________________________________ 
/*!

\class     valor::VALORBinning

\brief     Shared interface for the binning classes.
*          Assumes a binning can be specified as a
*          linearised list of bins in some ND kinematic space.

\author    Steve Dennis <s.r.dennis@liverpool.ac.uk>
           University of Liverpool

\created   August 2017

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_kine_binning_interface_h_
#define _valor_kine_binning_interface_h_

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <iomanip>

#include <TNamed.h>
#include <TLine.h>
#include <TH2D.h>

#include "valor/VArray/VALORArray.h"
#include "valor/VArray/VALORArrayBin.h"
#include "valor/Conventions/KineSpace.h"

namespace valor {

class VALORBinning: public TNamed
{
public:

  // Returns new binning of appropriate type from file
  // If read_now is true, it will read this immediately
  // Otherwise just get the right type and return that, you must call Read() manually
  static VALORBinning * BinningFromFile(const std::string & xml_file, bool read_now = false);
  
  VALORBinning();
  VALORBinning(std::string name, std::string title);
  virtual ~VALORBinning();

  // Read binning from file
  virtual bool Read(string file) = 0;

  // Get the corresponding kinematical space
  conv::KineSpace_t KineSpace(void) const { return fKineSpace; }

  // Number of dimensions
  unsigned int NDimensions (void) const { return fNDimensions; }

  // List of kinematical variables
  const std::vector<valor::conv::KineVar_t> & KineVars (void) const { return fKineVars; }

  // Dimension -> Variable
  conv::KineVar_t KineVar(unsigned int dimension);

  // Variable -> Dimension
  const int & Dimension(conv::KineVar_t kv) const;
  bool Contains(conv::KineVar_t kv) const;

  // Number of multi-dimensional bins 
  unsigned int NBins (void) const { return fNBins; }

  /**
    Get the number of bins for a one dimension with all other kine vars fixed.

    \param kv: The varied kine var to get the next bins for.

    \param other_kv_vals: Only used for Mondiran binning. A map containing all kine vars in the binning scheme except
                          for kv and their bin centres. These are to be kept fixed.

    \return The number of bins.
   */
  virtual unsigned int NBins (conv::KineVar_t kv, const std::map<conv::KineVar_t, double> & other_kv_vals) const = 0;

  // Global bin and max values
  double Minimum (unsigned int dimension ) const;
  double Minimum (conv::KineVar_t kv     ) const;
  double Maximum (unsigned int dimension ) const;
  double Maximum (conv::KineVar_t kv     ) const;

  // Bin min and max values
  virtual double BinMinimum (unsigned int bin, unsigned int dimension ) const = 0;
  virtual double BinMaximum (unsigned int bin, unsigned int dimension ) const = 0;
  virtual double BinMinimum (unsigned int bin, conv::KineVar_t kv     ) const;
  virtual double BinMaximum (unsigned int bin, conv::KineVar_t kv     ) const;

  // Bin range
  virtual const std::vector<double> & BinRange (unsigned int bin, unsigned int dimension ) const = 0;
  virtual const std::vector<double> & BinRange (unsigned int bin, conv::KineVar_t kv     ) const;

  // Bin width
  virtual double              BinWidth (unsigned int bin, unsigned int dimension ) const;
  virtual double              BinWidth (unsigned int bin, conv::KineVar_t kv     ) const;
  virtual std::vector<double> BinWidth (unsigned int bin                         ) const;

  // Bin centre
  virtual double              BinCentre (unsigned int bin, unsigned int dimension ) const;
  virtual double              BinCentre (unsigned int bin, conv::KineVar_t kv     ) const;
  virtual std::vector<double> BinCentre (unsigned int bin                         ) const;

  // Find bin
  virtual unsigned int FindBin (const std::vector<double> & xarr) const = 0;
  virtual unsigned int FindBin (const std::map<conv::KineVar_t, double> & xarr) const = 0;

  /**
    Get the next bin in a loop over one kine var with all other kine vars fixed.
    // TODO: This works fine when interating over 1 dimension only (in an Ndim binning scheme),
    // TODO: but if you want to loop > 1 dimension this only works with rectangular binning. Fix this.

    \param current_bin: The current (global) bin number. The function starts searching from this + 1.

    \param kv: The varied kine var to get the next bins for.

    \param other_kv_vals: A map containing all kine vars in the binning scheme except for kv and their bin centres.
                          These are to be kept fixed.

    \return The (global) bin number corresponding to the next bin, if there is a next bin. If not, -999 is returned.
   */
  int GetNextBin (int current_bin, conv::KineVar_t kv, const std::map<conv::KineVar_t, double> & other_kv_vals) const;

  /**
    Takes this binning and creates a new binning using the specified subset of
    kinematic variables.

    \param ks The target kinematic space. This must be a strict subset of the
      kinematic space used by the current binning.
    \return The binning with the specified kinematic space.
  */
  virtual VALORBinning* Contract(conv::KineSpace_t ks) const = 0;

  // Returns a vector of TLines that mark the edges of each bin
  virtual std::vector<TLine>  GetBinningTLines(void) const = 0;
  
  // Print a summary of the binning
  virtual void Print(std::ostream & stream = std::cout) const = 0;
  
  
  using TObject::Read; // Tell clang we really do want this
  
protected:

  //~ void Init    (void);
  //~ void CleanUp (void);

  conv::KineSpace_t                         fKineSpace;           // kinematical space
  unsigned int                              fNDimensions;         // number of dimensions
  std::vector<conv::KineVar_t>              fKineVars;            // kinematical variables in given space
  std::vector<int>                          fKineVarToDim;        // kinematical variable -> dimension
  unsigned int                              fNBins;               // number of bins
  std::vector<double>                       fMinimum;             //
  std::vector<double>                       fMaximum;             //
  
ClassDef(VALORBinning, 1)

};

} // valor

#endif


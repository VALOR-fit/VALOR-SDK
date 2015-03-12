//____________________________________________________________________________
/*!

\class    valor::VALORArray

\brief    N-dimensional array (for N<=6)

\author   Steve Dennis <stephen.dennis \at warwick.ac.uk>
          University of Warwick, Rutherford Appleton Laboratory

\created  April 2014

*/
//____________________________________________________________________________

#ifndef _valor_array_h_
#define _valor_array_h_

#include <TNamed.h>
#include <vector>

#include "Conventions/Build.h"

namespace valor {

class VALORArray: public TNamed
{
public:
  // Constructors
  VALORArray();
  VALORArray(const std::vector<int> dimensions);   // List of dimension sizes
  VALORArray(int nd, const int * dimensions);      // List of dimension sizes
  VALORArray(int dim1, int dim2=-1, int dim3=-1, int dim4=-1, int dim5=-1, int dim6=-1); // Dimension sizes individually
  VALORArray(std::string file_to_read); // Read from a file
  VALORArray(const VALORArray & arr);
 ~VALORArray();
  
  // Retrives size of total array, or size of dimension
  int GetSize(int dim = -1) const;
  
  // Get value at specific point
  double GetEntry           (int ind1, int ind2=-1, int ind3=-1, int ind4=-1, int ind5=-1, int ind6=-1) const;
  double & GetEntryRef      (int ind1, int ind2=-1, int ind3=-1, int ind4=-1, int ind5=-1, int ind6=-1);
  const double & GetEntryRef(int ind1, int ind2=-1, int ind3=-1, int ind4=-1, int ind5=-1, int ind6=-1) const;
  
  // Set value at specific point
  void SetEntry(double value, int ind1, int ind2=-1, int ind3=-1, int ind4=-1, int ind5=-1, int ind6=-1);
  void AddToEntry(double value, int ind1, int ind2=-1, int ind3=-1, int ind4=-1, int ind5=-1, int ind6=-1);
  
  int GetNDim() const;
  const double * GetArray(void) const;
  double * GetArray(void);
  
  const int * GetDimensions(void) const;
  
  void SetAll(double value);
  
  void SetDimensionName(int dimension, const std::string & name);
  std::string GetDimensionName(int dimension) const;
  
  // Makes a new array of one fewer dimensions.
  // By default integrates over the full range in the removed dimension
  // Can specify min and max bins to get a smaller integrated range or a single bin.
  VALORArray GetSlice(int dimension, int min_bin = -1, int max_bin = -1) const;
  
  int Address(int ind1, int ind2=-1, int ind3=-1, int ind4=-1, int ind5=-1, int ind6=-1) const;
  void InverseAddress(int address, int * return_indices) const;
  
  double Integral() const;
  double Integral(int min_dim1     , int max_dim1=-1, int min_dim2=-1, int max_dim2=-1, 
                  int min_dim3 = -1, int max_dim3=-1, int min_dim4=-1, int max_dim4=-1, 
                  int min_dim5 = -1, int max_dim5=-1, int min_dim6=-1, int max_dim6=-1 ) const;
                  
  void Scale(double factor); // Scales every bin by factor.
  void ScaleBins(double factor,                              // Scales specific bin given. If -1 is given for a dimension, scale all bins in that dimension.
                 int bin1 = -1, int bin2 =-1, int bin3 =-1, 
                 int bin4 = -1, int bin5 =-1, int bin6 =-1);
  void ScaleBinRange (double factor, // Scales bins in range. If -1 given for min, work from 0. If -1 given for max, work from max.
                      int min_dim1     , int max_dim1=-1, int min_dim2=-1, int max_dim2=-1, 
                      int min_dim3 = -1, int max_dim3=-1, int min_dim4=-1, int max_dim4=-1, 
                      int min_dim5 = -1, int max_dim5=-1, int min_dim6=-1, int max_dim6=-1 );
                      
  void ReadFile (std::string file_to_read);
  void WriteFile(std::string file_to_write);
  
  // Resize the array, clearing contents
  void Resize(const std::vector<int> dimensions);   // List of dimension sizes
  void Resize(int nd, const int * dimensions);      // List of dimension sizes
  void Resize(int dim1, int dim2=-1, int dim3=-1, int dim4=-1, int dim5=-1, int dim6=-1); // Dimension sizes individually
  
private:
  int fNDim;
  int fSize;
  
  int fDimensions[6]; // Stores size of each dimension
  int * fIncrement;   //[fNDim] Stores number to increment in main array to get +1 in this dimension
  
  double * fArray;    //[fSize] Stores actual contents of array
  std::string fNames[6]; //Store the dimension names if we need them.
  
  void Construct(int nd, const int * dimensions);
  void Cleanup();
  
ClassDef(VALORArray, 2)
};

} //valor

#endif

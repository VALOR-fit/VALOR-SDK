//____________________________________________________________________________
/*!

\class     valor::VALORArrayT

\brief     N-dimensional array (for N<=6)

\author    Steve Dennis <s.r.dennis@liverpool.ac.uk>
           University of Liverpool

\created   April 2016

\copyright (c) 2010-2016, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_array_template_h_
#define _valor_array_template_h_

#include <TNamed.h>
#include <vector>

#include "valor/Conventions/Build.h"
#include "valor/VArray/VALORArrayBin.h"
#include "valor/VArray/VALORArrayHeader.h"

namespace valor { 

template <typename numtype> class VALORArrayT: public TNamed
{
public:
  // Constructors
  VALORArrayT();
  VALORArrayT(const std::vector<int> dimensions);   // List of dimension sizes
  VALORArrayT(int nd, const int * dimensions);      // List of dimension sizes
  VALORArrayT(int dim1, int dim2=-1, int dim3=-1, int dim4=-1, int dim5=-1, int dim6=-1); // Dimension sizes individually
  VALORArrayT(std::string file_to_read); // Read from a file
  VALORArrayT(const VALORArrayT<numtype> & arr);
  template <typename fromtype> VALORArrayT(const VALORArrayT<fromtype> & arr);
  
 ~VALORArrayT();
  
  // Retrives size of total array, or size of dimension
  int GetSize(int dim = -1) const;
  
  // Get value at specific point
  numtype GetEntry           (int ind1, int ind2=-1, int ind3=-1, int ind4=-1, int ind5=-1, int ind6=-1) const;
  numtype & GetEntryRef      (int ind1, int ind2=-1, int ind3=-1, int ind4=-1, int ind5=-1, int ind6=-1);
  const numtype & GetEntryRef(int ind1, int ind2=-1, int ind3=-1, int ind4=-1, int ind5=-1, int ind6=-1) const;
  // Set value at specific point
  void SetEntry(numtype value, int ind1, int ind2=-1, int ind3=-1, int ind4=-1, int ind5=-1, int ind6=-1);
  void AddToEntry(numtype value, int ind1, int ind2=-1, int ind3=-1, int ind4=-1, int ind5=-1, int ind6=-1);
  // Convert index to block address (and back)
  int Address(int ind1, int ind2=-1, int ind3=-1, int ind4=-1, int ind5=-1, int ind6=-1) const;
  void InverseAddress(int address, int * return_indices) const;
  
  // Same as above but using the VALORArrayBin struct
  numtype GetEntry           (const VALORArrayBin & bin) const;
  numtype & GetEntryRef      (const VALORArrayBin & bin);
  const numtype & GetEntryRef(const VALORArrayBin & bin) const;
  void SetEntry(numtype value, const VALORArrayBin & bin);
  void AddToEntry(numtype value, const VALORArrayBin & bin);
  int Address(const VALORArrayBin & bin) const;
  VALORArrayBin InverseAddress(int address) const;
  
  int GetNDim() const;
  const numtype * GetArray(void) const;
  numtype * GetArray(void);
  
  const int * GetDimensions(void) const;
  VALORArrayBin GetDimensionSizes(void) const;
  
  void SetAll(numtype value);
  
  void SetDimensionName(int dimension, const std::string & name);
  std::string GetDimensionName(int dimension) const;
  
  // Makes a new array of one fewer dimensions.
  // By default integrates over the full range in the removed dimension
  // Can specify min and max bins to get a smaller integrated range or a single bin.
  VALORArrayT GetSlice(int dimension, int min_bin = -1, int max_bin = -1) const;
  
  numtype Integral() const;
  numtype Integral(int min_dim1     , int max_dim1=-1, int min_dim2=-1, int max_dim2=-1, 
                  int min_dim3 = -1, int max_dim3=-1, int min_dim4=-1, int max_dim4=-1, 
                  int min_dim5 = -1, int max_dim5=-1, int min_dim6=-1, int max_dim6=-1 ) const;
                  
  void Scale(numtype factor); // Scales every bin by factor.
  void ScaleBins(numtype factor,                              // Scales specific bin given. If -1 is given for a dimension, scale all bins in that dimension.
                 int bin1 = -1, int bin2 =-1, int bin3 =-1, 
                 int bin4 = -1, int bin5 =-1, int bin6 =-1);
  void ScaleBinRange (numtype factor, // Scales bins in range. If -1 given for min, work from 0. If -1 given for max, work from max.
                      int min_dim1     , int max_dim1=-1, int min_dim2=-1, int max_dim2=-1, 
                      int min_dim3 = -1, int max_dim3=-1, int min_dim4=-1, int max_dim4=-1, 
                      int min_dim5 = -1, int max_dim5=-1, int min_dim6=-1, int max_dim6=-1 );
                      
  void ReadFile (std::string file_to_read);
  void WriteFile(std::string file_to_write);
  
  // Resize the array, clearing contents
  void Resize(const std::vector<int> dimensions);   // List of dimension sizes
  void Resize(int nd, const int * dimensions);      // List of dimension sizes
  void Resize(int dim1, int dim2=-1, int dim3=-1, int dim4=-1, int dim5=-1, int dim6=-1); // Dimension sizes individually
  
  void CopyFrom(const VALORArrayT<numtype> & arr);
  void CopyDataOnlyFrom(const VALORArrayT<numtype> & arr);
  
  // Compression info
  void SetCompressionLevel(int level);
  
  // Print the contents
  void PrintEntries(std::ostream & str = std::cout);
  
private:
  int fNDim;
  int fSize;
  
  Int_t fDimensions[6]; // Stores size of each dimension
  int   fIncrement [6]; // Stores number to increment in main array to get +1 in this dimension
  
  numtype * fArray;    //[fSize] Stores actual contents of array
  std::string fNames[6]; //Store the dimension names if we need them.
  
  void Construct (int nd, const int   * dimensions);
  void ConstructT(int nd, const Int_t * dimensions); // Copy of the above, using aliased type
  void Cleanup();
  
  int fCompressionLevel; // For our own file writing interface only.
  
  VALORArrayHeader::kVArrayType GetHeaderType(void);
  
  
ClassDef(VALORArrayT, 1)
};

// Function to convert between VALORarrays of different types
// Header-only so we don't have to manually specify code generation
namespace util {
  template<typename typefrom, typename typeto>
    VALORArrayT<typeto> ConvertVALORArray(const VALORArrayT<typefrom> & input)
  {
    VALORArrayT<typeto> output (input.GetNDim(),input.GetDimensions());
    
    typeto * output_arr = output.GetArray();
    const typefrom * input_arr = input.GetArray();
    
    for (int i = 0 ; i < output.GetSize() ; i++) {
      output_arr[i] = (typeto) input_arr[i];
    }
    for (int i = 0 ; i < output.GetNDim(); i++) {
      output.SetDimensionName(i,input.GetDimensionName(i));
    }
    return output;
  }
}//util

// Typedefs
typedef VALORArrayT<double    > VALORArray;
typedef VALORArrayT<double    > VALORArrayD;
typedef VALORArrayT<float     > VALORArrayF;
typedef VALORArrayT<int       > VALORArrayI;
typedef VALORArrayT<Long64_t  > VALORArrayL;
typedef VALORArrayT<ULong64_t > VALORArrayUL;

} //valor

#endif //_valor_array_template_h_

//____________________________________________________________________________
/*!

\class     valor::utils

\brief     XML utilities

\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   Oct 20, 2010

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#ifndef _valor_xml_utils_h_
#define _valor_xml_utils_h_

#include <string>
#include <vector>
#include <complex>

#include <TXMLEngine.h>
#include <TFile.h>
#include <TTree.h>
#include <TVectorD.h>
#include <TMatrixD.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TSpline.h>
#include <TGraph.h>
#include <valor/VArray/VALORArrayT.h>

class TGraph;
class TH1;
class TH1F;
class TH1D;
class TH2D;
class TH3D;
class TAxis;

using std::string;
using std::vector;
using std::pair;

namespace valor {
namespace util {

  XMLNodePointer_t FindNode          (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  /**
     Retrieves all nodes with a given name under a given node.

     This method returns all nodes that are direct children of a specified node.
     It will not look within other child nodes.

     \param xml_eng The TXXMLEngine that will perform the parsing.
     \param top_node The node within which to search.
     \param tag The XMl tag to search for.
     \return The set of nodes found.
  */
  vector<XMLNodePointer_t> FindNodes(TXMLEngine* xml_eng,
        XMLNodePointer_t top_node, const string& tag);

  // Retrieves all direct children of a given node (regardless of child node name).
  vector<XMLNodePointer_t> FindAllNodes(TXMLEngine* xml_eng, XMLNodePointer_t top_node);

  bool             NodeExists        (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);

  // For a specified node which should contain a string: Does this node exist and is the string not empty?
  bool             IsNodeSpecified   (TXMLEngine * xml_eng, XMLNodePointer_t top_node, const string node_path);

  bool             XML2Bool          (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  int              XML2Int           (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  double           XML2Dbl           (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  vector<double>   XML2DblArray      (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  VALORArray       XML2VArray        (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  string           XML2String        (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  pair<double,double> XML2DblDblPair (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);

  string           XML2RootFileName  (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  string           XML2RootObjName   (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  string           XML2RootObjType   (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);

  TFile *          XML2TFile         (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TH1F *           XML2TH1F          (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TGraph *         XML2TGraph        (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TH1D *           XML2TH1D          (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TH2D *           XML2TH2D          (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TH3D *           XML2TH3D          (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TTree *          XML2TTree         (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TVectorD *       XML2TVectorD      (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TMatrixDSym *    XML2TMatrixDSym   (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TMatrixD *       XML2TMatrixD      (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TMatrixD *       XML2TH2D2TMatrixD (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");
  TSpline3 *       XML2TSpline3      (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir = "<env>");

  TAxis *          XML2BinInfo       (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  
} // util
} // valor
  
#endif



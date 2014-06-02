//____________________________________________________________________________
/*

Copyright (c) 2010-2014, The VALOR group. All rights reserved.

ChangeLog:

*/
//____________________________________________________________________________

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <TROOT.h>
#include <TMath.h>
#include <TStyle.h>
#include <TColor.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>
#include <TAxis.h>
#include <TVectorD.h>
#include <TMatrixD.h>
#include <TXMLEngine.h>
#include <TFile.h>
#include <TSystem.h>

#include "Messenger/Messenger.h"
#include "Utilities/UtlXML.h"
#include "Utilities/UtlStr.h"
#include "Utilities/Rnd.h"

using std::setw;
using std::setfill;
using std::fixed;
using std::scientific;
using std::setprecision;
using std::ofstream;
using std::ostringstream;

using namespace valor;

//___________________________________________________________________________
XMLNodePointer_t valor::util::FindNode(
  TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
// Find & return the XMLNodePointer_t at the specified path
//
  vector<string> node_path = Tokens(path,"/");

  unsigned int ndepth = node_path.size();
  unsigned int idepth = 0;

  XMLNodePointer_t curr_node = xmleng->GetChild(topnode);
  while(curr_node) {
    string curr_node_name = xmleng->GetNodeName(curr_node);
    if(node_path[idepth] == curr_node_name) {
        idepth++;
        if(idepth == ndepth) {
          return curr_node;
        } else {
          curr_node = xmleng->GetChild(curr_node);
        }
    } else {
       curr_node = xmleng->GetNext(curr_node);
    }
  }
  return 0;
}
//___________________________________________________________________________
bool valor::util::NodeExists(
  TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
// Find & return the XMLNodePointer_t at the specified path
//
  vector<string> node_path = Tokens(path,"/");

  unsigned int ndepth = node_path.size();
  unsigned int idepth = 0;

  XMLNodePointer_t curr_node = xmleng->GetChild(topnode);
  while(curr_node) {
    string curr_node_name = xmleng->GetNodeName(curr_node);
    if(node_path[idepth] == curr_node_name) {
        idepth++;
        if(idepth == ndepth) {
          return true;
        } else {
          curr_node = xmleng->GetChild(curr_node);
        }
    } else {
       curr_node = xmleng->GetNext(curr_node);
    }
  }
  return false;
}
//___________________________________________________________________________
bool valor::util::XML2Bool(
  TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
// Get the specified node content as a boolean
//
  LOG("XML", pINFO) << "Reading info at XML node path: " << path;

  XMLNodePointer_t found_node = FindNode(xmleng,topnode,path);
  if(!found_node) {
    LOG("XML", pFATAL) 
       << "No node was found at input XML node path: " << path;
    exit(1);
  }
  string content = TrimSpaces(xmleng->GetNodeContent(found_node));

  if(content == "true" || 
     content == "TRUE" || 
     content == "True" || 
     content == "1"    || 
     content == "I") return true;

  if(content == "false" || 
     content == "FALSE" || 
     content == "False" || 
     content == "0"    || 
     content == "O") return false;

   LOG("XML", pFATAL) 
       << "Could not interpret content (" << content 
       << ") found at path: " << path << " as a boolean!";
    exit(1);

  return false;
}
//___________________________________________________________________________
double valor::util::XML2Dbl(
  TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
// Get the specified node content as a floating-point number
//
  LOG("XML", pINFO) << "Reading info at XML node path: " << path;

  XMLNodePointer_t found_node = FindNode(xmleng,topnode,path);
  if(!found_node) {
    LOG("XML", pFATAL) 
       << "No node was found at input XML node path: " << path;
    exit(1);
  }
  string content = xmleng->GetNodeContent(found_node);
  double value = atof(content.c_str());
  return value;
}
//___________________________________________________________________________
vector<double> valor::util::XML2DblArray(
  TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
// Get the specified node content as a vector floating-point number
// The node content should be a comma-separed list of numbers

  LOG("XML", pINFO) << "Reading info at XML node path: " << path;

  XMLNodePointer_t found_node = FindNode(xmleng,topnode,path);
  if(!found_node) {
    LOG("XML",pFATAL) 
       << "No node was found at input XML node path: " << path;
    exit(1);
  }
  string content = TrimSpaces(xmleng->GetNodeContent(found_node));

  vector<double> array;
  vector<string> str_tokens = Tokens(content, ",");
  vector<string>::const_iterator str_tokens_iter = str_tokens.begin();
  for( ; str_tokens_iter != str_tokens.end(); ++str_tokens_iter) {
     string token = *str_tokens_iter;
     array.push_back( atof(token.c_str()) );
  }
  return array;
}
//___________________________________________________________________________
string valor::util::XML2String(
  TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
// Get the specified node content as a floating-point number
//
  LOG("XML", pINFO) << "Reading info at XML node path: " << path;

  XMLNodePointer_t found_node = FindNode(xmleng,topnode,path);
  if(!found_node) {
    LOG("XML",pFATAL)
       << "No node was found at input XML node path: " << path;
    exit(1);
  }
  string content = xmleng->GetNodeContent(found_node);
  return TrimSpaces(content);
}
//___________________________________________________________________________
string valor::util::XML2RootFileName(
   TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
  string filename = XML2String(xmleng,topnode,path+"/filename");
  LOG("XML", pINFO) << "Filename: " << filename;
  return filename;
}
//___________________________________________________________________________
string valor::util::XML2RootObjName (
   TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
  string objname = XML2String(xmleng,topnode,path+"/objname");
  LOG("XML", pINFO) << "Object name: " << objname;
  return objname;
}
//___________________________________________________________________________
string valor::util::XML2RootObjType (
   TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
  string objtype = XML2String(xmleng,topnode,path+"/objtype");
  LOG("XML", pINFO) << "Object type: " << objtype;
  return objtype;
}
//___________________________________________________________________________
TFile * valor::util::XML2TFile(
  TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
  LOG("XML", pWARN) << "Note that this method uses the $T2KOSCANA path and would cause problems if called by the LBNE VALOR code ** Fix it ** ";

  LOG("XML", pINFO) << "Reading info at XML node path: " << path;

  string filename = XML2RootFileName(xmleng,topnode,path);

  TFile * file = new TFile(
    Form("%s/%s", gSystem->Getenv("T2KOSCANA"), filename.c_str()), "read");
  if(!file) {
    LOG("XML",pFATAL) << "No file: " << filename << " found";
    exit(1);
  }
  if(file->IsZombie()) {
    LOG("XML",pFATAL) << "File is a zombie: " << filename;
    exit(1);
  }

  return file;
}
//___________________________________________________________________________
TH1F * valor::util::XML2TH1F(
  TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
// Get a TH1F instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node path: " << path;

  string filename = XML2RootFileName(xmleng,topnode,path);
  string objname  = XML2RootObjName (xmleng,topnode,path);

  TFile file(
    Form("%s/%s", gSystem->Getenv("T2KOSCANA"), filename.c_str()), "read");
  TH1F * h = (TH1F*)file.Get(objname.c_str());
  if(!h) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    exit(1);
  }

  TH1F * hcopy = new TH1F(*h);
  hcopy->SetDirectory(0);

  file.Close();

  return hcopy;
}
//___________________________________________________________________________
TH1D * valor::util::XML2TH1D(
  TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
// Get a TH1D instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node path: " << path;

  string filename = XML2RootFileName(xmleng,topnode,path);
  string objname  = XML2RootObjName (xmleng,topnode,path);

  TFile file(
    Form("%s/%s", gSystem->Getenv("T2KOSCANA"), filename.c_str()), "read");
  TH1D * h = (TH1D*)file.Get(objname.c_str());
  if(!h) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    exit(1);
  }

  TH1D * hcopy = new TH1D(*h);
  hcopy->SetDirectory(0);

  file.Close();

  return hcopy;
}
//___________________________________________________________________________
TH2D * valor::util::XML2TH2D(
  TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
// Get a TH2D instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node path: " << path;

  string filename = XML2RootFileName(xmleng,topnode,path);
  string objname  = XML2RootObjName (xmleng,topnode,path);

  TFile file(
     Form("%s/%s", gSystem->Getenv("T2KOSCANA"), filename.c_str()), "read");    
  TH2D * h2 = (TH2D*)file.Get(objname.c_str());
  if(!h2) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    exit(1);
  }

  TH2D * h2copy = new TH2D(*h2);
  h2copy->SetDirectory(0);

  file.Close();

  return h2copy;
}
//___________________________________________________________________________
TTree * valor::util::XML2TTree(
   TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
// Get a TH2D instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node path: " << path;

  string filename = XML2RootFileName(xmleng,topnode,path);
  string objname  = XML2RootObjName (xmleng,topnode,path);

/////  TFile file(filename.c_str(), "read");
/////  TTree * tree = (TTree*)file.Get(objname.c_str());

  TFile * file = new TFile(
     Form("%s/%s", gSystem->Getenv("T2KOSCANA"), filename.c_str()), "read");
  TTree * tree = (TTree*)file->Get(objname.c_str());
  if(!tree) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
//    file.Close();
    exit(1);
  }

  return tree;
//  TTree * treecopy = tree->CloneTree();
//  file.Close();
//  return treecopy;
}
//___________________________________________________________________________
TVectorD * valor::util::XML2TVectorD(
   TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
// Get a TVectorD instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node path: " << path;

  string filename = XML2RootFileName(xmleng,topnode,path);
  string objname  = XML2RootObjName (xmleng,topnode,path);

  TFile file(
    Form("%s/%s", gSystem->Getenv("T2KOSCANA"), filename.c_str()), "read");
  TVectorD * vec = (TVectorD*)file.Get(objname.c_str());
  if(!vec) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    //return 0;
    exit(1);
  }

  TVectorD * vec_copy = new TVectorD(*vec);

  file.Close();

  return vec_copy;
}
//___________________________________________________________________________
TMatrixDSym * valor::util::XML2TMatrixDSym(
   TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
// Get a TMatrixDSym instance from the info in the input XML config file
//
  string filename = XML2RootFileName(xmleng,topnode,path);
  string objname  = XML2RootObjName (xmleng,topnode,path);

  TFile file(
    Form("%s/%s", gSystem->Getenv("T2KOSCANA"), filename.c_str()), "read");
  TMatrixDSym * matrix = (TMatrixDSym*)file.Get(objname.c_str());
  if(!matrix) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    exit(1);
  }

  TMatrixDSym * matrix_copy = new TMatrixDSym(*matrix);
  file.Close();

  return matrix_copy;
}
//___________________________________________________________________________
TMatrixD * valor::util::XML2TMatrixD(
    TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
// Get a TMatrixD instance from the info in the input XML config file
//
  string objtype  = XML2RootObjType (xmleng,topnode,path);
  if(objtype == "TH2D")
    return XML2TH2D2TMatrixD(xmleng,topnode,path);

  string filename = XML2RootFileName(xmleng,topnode,path);
  string objname  = XML2RootObjName (xmleng,topnode,path);

  TFile * file = new TFile( 
    Form("%s/%s", gSystem->Getenv("T2KOSCANA"), filename.c_str()), "read");
  TMatrixD * matrix = (TMatrixD*) file->Get(objname.c_str());
  if(!matrix) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file->Close();
    exit(1);
  }

  TMatrixD * matrix_copy = new TMatrixD(*matrix);
  file->Close();

  return matrix_copy;
}
//___________________________________________________________________________
TMatrixD * valor::util::XML2TH2D2TMatrixD(
  TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
// Get a TH2D instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node path: " << path;

  string filename = XML2RootFileName(xmleng,topnode,path);
  string objname  = XML2RootObjName (xmleng,topnode,path);

  TFile file(
     Form("%s/%s", gSystem->Getenv("T2KOSCANA"), filename.c_str()), "read");    
  TH2D * h2 = (TH2D*)file.Get(objname.c_str());
  if(!h2) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    exit(1);
  }

  TH2D * h2copy = new TH2D(*h2);
  h2copy->SetDirectory(0);

  file.Close();

  const int nrows = h2copy->GetNbinsX();
  const int ncols = h2copy->GetNbinsY();

  TMatrixD matrix(nrows,ncols);
  for(int i = 1; i <= nrows; i++) {
    for(int j = 1; j <= ncols; j++) {
      matrix(i-1,j-1)=h2copy->GetBinContent(i,j);
      LOG("XML",pDEBUG) << i << "\t" << j << "\t" << h2copy->GetBinContent(i,j);
    }
  }

  TMatrixD * matrix_copy = new TMatrixD(matrix);
#ifdef __show_xml_mesg_debug__
  matrix_copy->Print();
#endif

  return matrix_copy;
}
//___________________________________________________________________________
TSpline3 * valor::util::XML2TSpline3(
  TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
  string filename = XML2RootFileName(xmleng,topnode,path);
  string objname  = XML2RootObjName (xmleng,topnode,path);

  TFile * file = new TFile( 
    Form("%s/%s", gSystem->Getenv("T2KOSCANA"), filename.c_str()), "read");
  TSpline3 * spl = (TSpline3*) file->Get(objname.c_str());
  if(!spl) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file->Close();
    exit(1);
  }

  TSpline3 * spl_copy = new TSpline3(*spl);
  file->Close();

  return spl_copy;
}
//___________________________________________________________________________
TAxis *  valor::util::XML2BinInfo(
  TXMLEngine * xmleng, XMLNodePointer_t topnode, string path)
{
//
//
  LOG("XML", pINFO) << "Reading info at XML node path: " << path;

  XMLNodePointer_t found_node = FindNode(xmleng,topnode,path);
  if(!found_node) {
    LOG("XML",pFATAL) << "No node was found at input XML node path: " << path;
    //return 0;
    exit(1);
  }

  string binsz = xmleng->GetAttr(found_node,"bin_size");
  if(binsz == "fixed") {
    int    nbins = 0;
    double min   = 0;
    double max   = 0;
    XMLNodePointer_t child_node = xmleng->GetChild(found_node);
    while(child_node) {
      string child_node_name = xmleng->GetNodeName(child_node);
      string content = xmleng->GetNodeContent(child_node);
      if(child_node_name == "nbins") { nbins = atoi(TrimSpaces(content).c_str()); }
      if(child_node_name == "min"  ) { min   = atof(TrimSpaces(content).c_str()); }
      if(child_node_name == "max"  ) { max   = atof(TrimSpaces(content).c_str()); }
      child_node = xmleng->GetNext(child_node);    
    }
    LOG("XML",pINFO) 
       << "nbins = " << nbins << ", range = (" << min << ", " << max << ")";
    assert(nbins>0 && min<max);

    TAxis * axis = new TAxis(nbins,min,max);
    return axis;
  } 
  else   
  if(binsz == "variable") {
    LOG("XML",pFATAL) << "Not handling variable size bins yet";
    exit(1);
  }
  else {
    LOG("XML",pFATAL) << "Invalid bin_size attribute";
    exit(1);
  }

  return 0;
}
//___________________________________________________________________________


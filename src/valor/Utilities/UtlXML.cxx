//_____________________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

ChangeLog:

*/
//_____________________________________________________________________________________________________

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>

#include <TROOT.h>
#include <TMath.h>
#include <TStyle.h>
#include <TColor.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TAxis.h>
#include <TVectorD.h>
#include <TMatrixD.h>
#include <TXMLEngine.h>
#include <TFile.h>
#include <TSystem.h>

#include "valor/Conventions/Constants.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlXML.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/Env.h"

using std::setw;
using std::setfill;
using std::fixed;
using std::scientific;
using std::setprecision;
using std::ofstream;
using std::ostringstream;

using namespace valor;

// Just so we don't have to keep making it later
std::pair<double,double> max_limits = std::make_pair<double,double>(
  -std::numeric_limits<double>::max(), std::numeric_limits<double>::max());

//____________________________________________________________________________________________________
XMLNodePointer_t valor::util::FindNode(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
// Find & return the XMLNodePointer_t at the specified node_path
//
  vector<string> node_path_vec = Tokens(node_path,"/");

  unsigned int ndepth = node_path_vec.size();
  unsigned int idepth = 0;

  XMLNodePointer_t curr_node = xml_eng->GetChild(top_node);
  while(curr_node) {
    string curr_node_name = xml_eng->GetNodeName(curr_node);
    if(node_path_vec[idepth] == curr_node_name) {
        idepth++;
        if(idepth == ndepth) {
          return curr_node;
        } else {
          curr_node = xml_eng->GetChild(curr_node);
        }
    } else {
       curr_node = xml_eng->GetNext(curr_node);
    }
  }
  return 0;
}

vector<XMLNodePointer_t> valor::util::FindNodes(TXMLEngine* xml_eng,
      XMLNodePointer_t top_node, const string& tag)
{
   vector<XMLNodePointer_t> nodes;
   // Retrieve the first node for the search
   XMLNodePointer_t node = xml_eng->GetChild(top_node);
   if(node)
   {  // There is a node to examine
      if(xml_eng->GetNodeName(node) == tag)
      {  // The first node matches our search, store it
         nodes.push_back(node);
      }
      // Iterate through the remaining nodes, storing the matches
      while((node = xml_eng->GetNext(node)))
      {
         if(xml_eng->GetNodeName(node) == tag)
         {  // Node matches our search, store it
            nodes.push_back(node);
         }
      }
   }

   return nodes;
}
//____________________________________________________________________________________________________
vector<XMLNodePointer_t> valor::util::FindAllNodes(TXMLEngine* xml_eng, XMLNodePointer_t top_node)
{
  vector<XMLNodePointer_t> nodes;
  // Retrieve the first node for the search
  XMLNodePointer_t node = xml_eng->GetChild(top_node);
  if(node)
  {  // There is a node to examine
      nodes.push_back(node);
    // Iterate through the remaining nodes
    while((node = xml_eng->GetNext(node)))
    {
        nodes.push_back(node);
    }
  }
  return nodes;
}
//____________________________________________________________________________________________________
bool valor::util::NodeExists(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
// Find & return the XMLNodePointer_t at the specified node_path
//
  vector<string> node_path_vec = Tokens(node_path,"/");

  unsigned int ndepth = node_path_vec.size();
  unsigned int idepth = 0;

  XMLNodePointer_t curr_node = xml_eng->GetChild(top_node);
  while(curr_node) {
    string curr_node_name = xml_eng->GetNodeName(curr_node);
    if(node_path_vec[idepth] == curr_node_name) {
        idepth++;
        if(idepth == ndepth) {
          return true;
        } else {
          curr_node = xml_eng->GetChild(curr_node);
        }
    } else {
       curr_node = xml_eng->GetNext(curr_node);
    }
  }
  return false;
}
//____________________________________________________________________________________________________
bool valor::util::XML2Bool(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
// Get the specified node content as a boolean
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
  if(!found_node) {
    LOG("XML", pFATAL) 
       << "No node was found at input XML node node_path: " << node_path;
    util::Die(1); return false;
  }
  string content = TrimSpaces(xml_eng->GetNodeContent(found_node));

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
      << ") found at node_path: " << node_path << " as a boolean!";
  util::Die(1);
  return false;
}
//____________________________________________________________________________________________________
int valor::util::XML2Int(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
// Get the specified node content as a floating-point number
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
  if(!found_node) {
    LOG("XML", pFATAL) 
       << "No node was found at input XML node node_path: " << node_path;
    util::Die(1); return -1;
  }
  string content = xml_eng->GetNodeContent(found_node);
  int value = atoi(content.c_str());
  return value;
}
//____________________________________________________________________________________________________
double valor::util::XML2Dbl(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
// Get the specified node content as a floating-point number
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
  if(!found_node) {
    LOG("XML", pFATAL) 
       << "No node was found at input XML node node_path: " << node_path;
    util::Die(1); return conv::kBadResult;
  }
  string content = xml_eng->GetNodeContent(found_node);
  double value = conv::kBadResult;
  if(util::ToLowerCase(content) == "inf"){
    value = max_limits.second;
  }
  else if(util::ToLowerCase(content) == "-inf"){
    value = max_limits.first;
  }
  else{
    value = atof(content.c_str());
  }
  return value;
}
//____________________________________________________________________________________________________
vector<double> valor::util::XML2DblArray(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
// Get the specified node content as a vector floating-point number
// The node content should be a comma-separed list of numbers

  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;
  vector<double> array;
  XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
  if(!found_node) {
    LOG("XML",pFATAL) 
       << "No node was found at input XML node node_path: " << node_path;
    util::Die(1); return array;
  }
  const char * nc = xml_eng->GetNodeContent(found_node);
  if (not nc) return array;
  
  string content = TrimSpaces(nc);
  
  vector<string> str_tokens = Tokens(content, ",");
  vector<string>::const_iterator str_tokens_iter = str_tokens.begin();
  for( ; str_tokens_iter != str_tokens.end(); ++str_tokens_iter) {
     string token = TrimSpaces(*str_tokens_iter);
     if (not token.size() ) continue;
     if(util::ToLowerCase(token) == "inf"){
       array.push_back(max_limits.second);
     }
     else if(util::ToLowerCase(token) == "-inf"){
       array.push_back(max_limits.first);
     }
     else{
       array.push_back( atof(token.c_str()) );
     }
  }
  
  return array;
}
//____________________________________________________________________________________________________
string valor::util::XML2String(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
// Get the specified node content as a floating-point number
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;
 
  XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
  if(!found_node) {
    LOG("XML",pFATAL)
       << "No node was found at input XML node node_path: " << node_path;
    util::Die(1); return "badstring";
  }
  const char * nc = xml_eng->GetNodeContent(found_node);
  if (not nc) return string("");
  string content(nc);
  return TrimSpaces(content);
}
//____________________________________________________________________________________________________
pair<double,double> valor::util::XML2DblDblPair(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
// Get the specified node content as a pair of floating-point numbers
// The node content should be 2 comma-separed numbers

  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
  pair<double,double> pair;
  if(!found_node) {
    LOG("XML",pFATAL) 
       << "No node was found at input XML node node_path: " << node_path;
    util::Die(1); return pair;
  }
  const char * nc = xml_eng->GetNodeContent(found_node);
  if (not nc) return pair;
  
  string content = TrimSpaces(nc);
  
  vector<string> str_tokens = Tokens(content, ",");
  if(str_tokens.size() != 2) {
    LOG("XML",pFATAL)
      << "XML2DblDblPair cannot get exactly two values from: " << content;
    util::Die(1); return pair;
  }

  double arr[2];
  for(int i = 0; i < 2; ++i) {
    if(util::ToLowerCase(util::TrimSpaces(str_tokens[i])) == "inf") {
      arr[i] = max_limits.second;
    }
    else if(util::ToLowerCase(util::TrimSpaces(str_tokens[i])) == "-inf") {
      arr[i] = max_limits.first;
    }
    else {
      arr[i] = atof(str_tokens[i].c_str());
    }
  }

  pair.first  = arr[0];
  pair.second = arr[1];
  return pair;
}
//____________________________________________________________________________________________________
//
// When we specify a ROOT object to be obtained from within a file,
// the XML schema for specifying the ROOT filename, the object name and the 
// object type is:
//
// <some_node>
//    ...
//      <another_node>
//            <filename> blah </filename>
//            <objname>  blah </objname>
//            <objtype>  blah </objtype>
//      </another_node>
//    ...
// </some_node>
//
// The following three methods return the filename, the object name and the
// object type for the specified node path (<another_node> in above example).
//
//.....................................................................................................
string valor::util::XML2RootFileName(
   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
  string path = "";
  if(node_path != "") { path = node_path+"/"; }
  path += "filename";
  string filename = XML2String(xml_eng,top_node,path);
  LOG("XML", pINFO) << "Filename: " << filename;
  return filename;
}
//.....................................................................................................
string valor::util::XML2RootObjName (
   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
  string path = "";
  if(node_path != "") { path = node_path+"/"; }
  path += "objname";
  string objname = XML2String(xml_eng,top_node,path);
  LOG("XML", pINFO) << "Object name: " << objname;
  return objname;
}
//.....................................................................................................
string valor::util::XML2RootObjType (
   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
  string path = "";
  if(node_path != "") { path = node_path+"/"; }
  path += "objtype";
  string objtype = XML2String(xml_eng,top_node,path);
  LOG("XML", pINFO) << "Object type: " << objtype;
  return objtype;
}
//____________________________________________________________________________________________________
//
// The following set of methods return a ROOT object from a file.
// The path specified in the XML file file
//  eg 
//    <mc_sample>
//          <filename> data/t2k/run1_4/nue.root <filename>
//          ...
//    </mc_sample>
// does not need to be the absolute path.
// The following methods shall append path specified in the XML file to
// the input base_dir.
// If the default value of base_dir is unchanged, it will be set to the
// value of Env::TopLevelDataDir()
//
//.....................................................................................................

TFile * valor::util::XML2TFile(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  string filename = XML2RootFileName(xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  TString full_filename = TString::Format("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile * file = new TFile(full_filename.Data(),"read");
  if(!file) {
    LOG("XML",pFATAL) << "No file: " << full_filename << " found";
    util::Die(1); return 0;
  }
  if(file->IsZombie()) {
    LOG("XML",pFATAL) << "File is a zombie: " << full_filename;
    util::Die(1); return 0;
  }

  return file;
}
//____________________________________________________________________________________________________
TH1F * valor::util::XML2TH1F(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
// Get a TH1F instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  TString full_filename = TString::Format("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile file(full_filename,"read");
  TH1F * h = (TH1F*)file.Get(objname.c_str());
  if(!h) {
    LOG("XML",pFATAL) << "No " << objname << " in " << full_filename;
    file.Close();
    util::Die(1); return 0;
  }

  TH1F * hcopy = new TH1F(*h);
  hcopy->SetDirectory(0);

  file.Close();

  return hcopy;
}
//____________________________________________________________________________________________________
TGraph * valor::util::XML2TGraph(
                             TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
    // Get a TH1F instance from the info in the input XML config file
    //
    LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;
    
    string filename = XML2RootFileName(xml_eng,top_node,node_path);
    string objname  = XML2RootObjName (xml_eng,top_node,node_path);
    
    string used_base_dir = base_dir;
    if(base_dir.find("<env>") != string::npos) {
        used_base_dir = Env::Instance()->TopLevelDataDir();
    }
    
    TString full_filename = TString::Format("%s/%s", used_base_dir.c_str(), filename.c_str());
    
    TFile file(full_filename,"read");
    TGraph * h = (TGraph*)file.Get(objname.c_str());
    if(!h) {
      LOG("XML",pFATAL) << "No " << objname << " in " << full_filename;
      file.Close();
      util::Die(1); return 0;
    }
    
    TGraph * hcopy = new TGraph(*h);
    
    file.Close();
    
    return hcopy;
}
//____________________________________________________________________________________________________
TH1D * valor::util::XML2TH1D(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
// Get a TH1D instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  TString full_filename = TString::Format("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile file(full_filename,"read");
  TH1D * h = (TH1D*)file.Get(objname.c_str());
  if(!h) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    util::Die(1); return 0;
  }

  TH1D * hcopy = new TH1D(*h);
  hcopy->SetDirectory(0);
  
  delete h;
  file.Close();

  return hcopy;
}
//____________________________________________________________________________________________________
TH2D * valor::util::XML2TH2D(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
// Get a TH2D instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  TString full_filename = TString::Format("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile file(full_filename,"read");
  TH2D * h2 = (TH2D*)file.Get(objname.c_str());
  if(!h2) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    util::Die(1); return 0;
  }

  TH2D * h2copy = new TH2D(*h2);
  h2copy->SetDirectory(0);
  
  delete h2;
  file.Close();

  return h2copy;
}
//____________________________________________________________________________________________________
TH3D * valor::util::XML2TH3D(
                             TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
    // Get a TH3D instance from the info in the input XML config file
    //
    LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;
    
    string filename = XML2RootFileName(xml_eng,top_node,node_path);
    string objname  = XML2RootObjName (xml_eng,top_node,node_path);
    
    string used_base_dir = base_dir;
    if(base_dir.find("<env>") != string::npos) {
        used_base_dir = Env::Instance()->TopLevelDataDir();
    }
    
    TString full_filename = TString::Format("%s/%s", used_base_dir.c_str(), filename.c_str());
    
    TFile file(full_filename,"read");
    TH3D * h2 = (TH3D*)file.Get(objname.c_str());
    if(!h2) {
        LOG("XML",pFATAL) << "No " << objname << " in " << filename;
        file.Close();
        util::Die(1); return 0;
    }
    
    TH3D * h2copy = new TH3D(*h2);
    h2copy->SetDirectory(0);
    
    delete h2;
    file.Close();
    
    return h2copy;
}
//____________________________________________________________________________________________________
TTree * valor::util::XML2TTree(
   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
// Get a TH2D instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  TString full_filename = "";
  if(filename.rfind("/",0) == 0){
    LOG("XML", pNOTICE) << "File path starts with '/' therefore assume full path has been given: " << filename;
    full_filename = filename;
  }
  else{
    LOG("XML", pNOTICE) << "File path doesn't start with '/' therefore assume full path has not been given: " << filename;
    full_filename = TString::Format("%s/%s", used_base_dir.c_str(), filename.c_str());
    LOG("XML", pNOTICE) << "Full filename therefore: " << full_filename;
  }

  TFile * file = new TFile(full_filename.Data(),"read");
  TTree * tree = (TTree*)file->Get(objname.c_str());
  if(!tree) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
//  file.Close();
    util::Die(1); return 0;
  }

  return tree;
//TTree * treecopy = tree->CloneTree();
//file->Close();
//return treecopy;
}
//____________________________________________________________________________________________________
TVectorD * valor::util::XML2TVectorD(
   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
// Get a TVectorD instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  TString full_filename = TString::Format("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile file(full_filename,"read");
  TVectorD * vec = (TVectorD*)file.Get(objname.c_str());
  if(!vec) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    //return 0;
    util::Die(1); return 0;
  }
  
  TVectorD * vec_copy = new TVectorD(*vec);
  
  delete vec;
  file.Close();

  return vec_copy;
}
//____________________________________________________________________________________________________
TMatrixDSym * valor::util::XML2TMatrixDSym(
   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
// Get a TMatrixDSym instance from the info in the input XML config file
//
  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  TString full_filename = TString::Format("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile file(full_filename,"read");
  TMatrixDSym * matrix = (TMatrixDSym*)file.Get(objname.c_str());
  if(!matrix) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    util::Die(1); return 0;
  }

  TMatrixDSym * matrix_copy = new TMatrixDSym(*matrix);
  delete matrix;
  file.Close();

  return matrix_copy;
}
//____________________________________________________________________________________________________
TMatrixD * valor::util::XML2TMatrixD(
    TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
// Get a TMatrixD instance from the info in the input XML config file
//
  string objtype  = XML2RootObjType (xml_eng,top_node,node_path);
  if(objtype == "TH2D")
    return XML2TH2D2TMatrixD(xml_eng,top_node,node_path);

  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  TString full_filename = TString::Format("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile * file = new TFile(full_filename.Data(),"read"); 
  TMatrixD * matrix = (TMatrixD*) file->Get(objname.c_str());
  if(!matrix) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file->Close();
    util::Die(1); return 0;
  }

  TMatrixD * matrix_copy = new TMatrixD(*matrix);
  delete matrix;
  file->Close();

  return matrix_copy;
}
//____________________________________________________________________________________________________
TMatrixD * valor::util::XML2TH2D2TMatrixD(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
// Get a TH2D instance from the info in the input XML config file
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  TString full_filename = TString::Format("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile file(full_filename.Data(),"read");
  TH2D * h2 = (TH2D*)file.Get(objname.c_str());
  if(!h2) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    util::Die(1); return 0;
  }
  
  const int nrows = h2->GetNbinsX();
  const int ncols = h2->GetNbinsY();

  TMatrixD * matrix = new TMatrixD(nrows,ncols);
  for(int i = 1; i <= nrows; i++) {
    for(int j = 1; j <= ncols; j++) {
      (*matrix)(i-1,j-1) = h2->GetBinContent(i,j);
      LOG("XML",pDEBUG) << i << "\t" << j << "\t" << h2->GetBinContent(i,j);
    }
  }
#ifdef __show_xml_mesg_debug__
  matrix->Print();
#endif
  delete h2;
  file.Close();
  return matrix;
}
//____________________________________________________________________________________________________
TSpline3 * valor::util::XML2TSpline3(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path, string base_dir)
{
  string filename = XML2RootFileName(xml_eng,top_node,node_path);
  string objname  = XML2RootObjName (xml_eng,top_node,node_path);

  string used_base_dir = base_dir;
  if(base_dir.find("<env>") != string::npos) {
    used_base_dir = Env::Instance()->TopLevelDataDir();
  }

  TString full_filename = TString::Format("%s/%s", used_base_dir.c_str(), filename.c_str());

  TFile file(full_filename.Data(),"read"); 
  TSpline3 * spl = (TSpline3*) file.Get(objname.c_str());
  if(!spl) {
    LOG("XML",pFATAL) << "No " << objname << " in " << filename;
    file.Close();
    util::Die(1); return 0;
  }

  TSpline3 * spl_copy = new TSpline3(*spl);
  delete spl;
  file.Close();

  return spl_copy;
}
//____________________________________________________________________________________________________
TAxis *  valor::util::XML2BinInfo(
  TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
{
//
//
  LOG("XML", pINFO) << "Reading info at XML node node_path: " << node_path;

  XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
  if(!found_node) {
    LOG("XML",pFATAL) << "No node was found at input XML node node_path: " << node_path;
    util::Die(1); return 0;
  }

  string binsz = xml_eng->GetAttr(found_node,"bin_size");
  if(binsz == "fixed") {
    int    nbins = 0;
    double min   = 0;
    double max   = 0;
    XMLNodePointer_t child_node = xml_eng->GetChild(found_node);
    while(child_node) {
      string child_node_name = xml_eng->GetNodeName(child_node);
      string content = xml_eng->GetNodeContent(child_node);
      if(child_node_name == "nbins") { nbins = atoi(TrimSpaces(content).c_str()); }
      if(child_node_name == "min"  ) { min   = atof(TrimSpaces(content).c_str()); }
      if(child_node_name == "max"  ) { max   = atof(TrimSpaces(content).c_str()); }
      child_node = xml_eng->GetNext(child_node);    
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
    util::Die(1); return 0;
  }
  else {
    LOG("XML",pFATAL) << "Invalid bin_size attribute";
    util::Die(1); return 0;
  }

  return 0;
}
//____________________________________________________________________________________________________
bool valor::util::IsNodeSpecified(TXMLEngine* xmleng, XMLNodePointer_t top_node, const string node_path)
{
  bool specified = true;
  if(util::NodeExists(xmleng, top_node, node_path)) {
    std::string content = util::XML2String(xmleng, top_node, node_path);
    if(content == "") {
      specified = false;
    }
  }
  else {
    specified = false;
  }
  return specified;
}
//________________________________________________________________________________________

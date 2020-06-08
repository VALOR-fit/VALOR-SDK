//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

ChangeLog:

*/
//________________________________________________________________________________________

#include <iostream>

#include <TXMLEngine.h>

//fmwk
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlXML.h"
#include "valor/Messenger/Messenger.h"

#include "valor/Conventions/ModeList.h"

using namespace valor;
using namespace valor::conv;
using namespace std;

//________________________________________________________________________________________
ModeList::ModeList(std::string modelist_file, std::string coarse_modelist_file)
{
  this->Init(modelist_file, coarse_modelist_file);
}
//________________________________________________________________________________________
ModeList::ModeList(TXMLEngine* xml_model, XMLNodePointer_t root_node)
{
  // Get the mode and coarse mode files to read from
  string modelist_file        = util::XML2String(xml_model, root_node, "modelist");
  string coarse_modelist_file = util::XML2String(xml_model, root_node, "coarse_modelist");
  this->Init(modelist_file, coarse_modelist_file);
}
//________________________________________________________________________________________
void ModeList::Init(std::string modelist_file, std::string coarse_modelist_file)
{
  // Read the modes
  TXMLEngine * xml_reac_mode = new TXMLEngine;
  XMLDocPointer_t doc_reac_mode = xml_reac_mode->ParseFile(util::CompleteConfigPath(modelist_file).c_str());
  XMLNodePointer_t root_node_reac_mode = xml_reac_mode->DocGetRootElement(doc_reac_mode);
  XMLNodePointer_t curr_child_node_reac_mode = xml_reac_mode->GetChild(root_node_reac_mode);
  while(curr_child_node_reac_mode)
  {
    const char * nc = xml_reac_mode->GetNodeContent(curr_child_node_reac_mode);
    string content(util::RemoveAllWhitespace(nc));
    conv::ReacMode_t mode = util::ReacModeFromString(content);
    if (mode==conv::kmUndefined) {
      LOG("ModeList",pFATAL) << "Got undefined mode from string "<<content;
      util::Die(-1); return;
    }
    fModes.push_back(mode);
    curr_child_node_reac_mode = xml_reac_mode->GetNext(curr_child_node_reac_mode);
  }

  // We're done with the mode list XML file now
  xml_reac_mode->FreeDoc(doc_reac_mode);
  delete xml_reac_mode;


  //Read the coarse mdes
  // Read the modes
  if(coarse_modelist_file != "") {
    TXMLEngine * xml_coarse_mode = new TXMLEngine;
    XMLDocPointer_t doc_coarse_mode = xml_coarse_mode->ParseFile(util::CompleteConfigPath(coarse_modelist_file).c_str());
    XMLNodePointer_t root_node_coarse_mode = xml_coarse_mode->DocGetRootElement(doc_coarse_mode);
    XMLNodePointer_t coarse_mode_node = xml_coarse_mode->GetChild(root_node_coarse_mode);
    while(coarse_mode_node)
    {
      string coarse_mode_name = xml_coarse_mode->GetNodeName(coarse_mode_node);
      if(coarse_mode_name != "coarse_mode") {
        LOG("ModeList",pFATAL) << "Found a node, \'" << coarse_mode_name
                               << "\', which does not correspond to a coarse mode";
        util::Die(-1); return;
      }

      // Get the coarse mode from the specified name
      string name = "";
      conv::ReacModeCoarse_t mode;
      if(util::IsNodeSpecified(xml_coarse_mode, coarse_mode_node, "name")) {
        name = util::XML2String(xml_coarse_mode, coarse_mode_node, "name");
        mode = util::CoarseModeFromString(name);
        if (mode==conv::kmcUndefined) {
          LOG("ModeList",pFATAL) << "Got undefined coarse mode from string "<<name;
          util::Die(-1); return;
        }
        fModesCoarse.push_back(mode);
      }
      else {
        LOG("ModeList", pFATAL) << "No name node was found for a coarse mode";
        util::Die(-1); return;
      }

      // Coarse mode line colour
      if(util::IsNodeSpecified(xml_coarse_mode, coarse_mode_node, "line_col")) {
        int content = util::XML2Int(xml_coarse_mode, coarse_mode_node, "line_col");
        fModesCoarseLineCol.insert(std::pair<conv::ReacModeCoarse_t, int>(mode, content));
      }
      else {
        fModesCoarseLineCol.insert(std::pair<conv::ReacModeCoarse_t, int>(mode, fDefLineCol));
        LOG("ModeList", pINFO) << "No line_col node was found for \'" << name << "\', setting to \'" << fDefLineCol <<"\'";
      }

      // Coarse mode fill colour
      if(util::IsNodeSpecified(xml_coarse_mode, coarse_mode_node, "fill_col")) {
        int content = util::XML2Int(xml_coarse_mode, coarse_mode_node, "fill_col");
        fModesCoarseFillCol.insert(std::pair<conv::ReacModeCoarse_t, int>(mode, content));
      }
      else {
        int use_fill_col = fDefFillCol + fModesCoarse.size() - 1;
        fModesCoarseFillCol.insert(std::pair<conv::ReacModeCoarse_t, int>(mode, use_fill_col));
        LOG("ModeList", pINFO) << "No fill_col node was found for \'" << name << "\', setting to \'" << use_fill_col <<"\'";
      }

      // Coarse mode fill style
      if(util::IsNodeSpecified(xml_coarse_mode, coarse_mode_node, "fill_sty")) {
        int content = util::XML2Int(xml_coarse_mode, coarse_mode_node, "fill_sty");
        fModesCoarseFillSty.insert(std::pair<conv::ReacModeCoarse_t, int>(mode, content));
      }
      else {
        fModesCoarseFillSty.insert(std::pair<conv::ReacModeCoarse_t, int>(mode, fDefFillSty));
        LOG("ModeList", pINFO) << "No fill_sty node was found for \'" << name << "\', setting to \'" << fDefFillSty <<"\'";
      }

      coarse_mode_node = xml_coarse_mode->GetNext(coarse_mode_node);
    }

    // We're done with the coarse mode list XML file now
    xml_coarse_mode->FreeDoc(doc_coarse_mode);
    delete xml_coarse_mode;
  }

  return;
}
//________________________________________________________________________________________
const std::vector<conv::ReacMode_t> & ModeList::Modes() const
{
  return fModes;
}
//________________________________________________________________________________________
const std::vector<conv::ReacModeCoarse_t> & ModeList::CoarseModes() const
{
  return fModesCoarse;
}
//________________________________________________________________________________________
int ModeList::GetCoarseModeLineCol(conv::ReacModeCoarse_t mc) const
{
  int val = -999;
  try {
    val = fModesCoarseLineCol.at(mc);
  }
  catch(...) {
    LOG("ModeList", pWARN) << "Could not get the line colour for coarse mode \'" << util::AsString(mc)
                           << "\', instead returning " << fDefLineCol;
    val = fDefLineCol;
  }
  return val;
}
//________________________________________________________________________________________
int ModeList::GetCoarseModeFillCol(conv::ReacModeCoarse_t mc) const
{
  int val = -999;
  try {
    val = fModesCoarseFillCol.at(mc);
  }
  catch(...) {
    LOG("ModeList", pWARN) << "Could not get the fill colour for coarse mode \'" << util::AsString(mc)
                           << "\', instead returning " << fDefFillCol;
    val = fDefFillCol;
  }
  return val;
}
//________________________________________________________________________________________
int ModeList::GetCoarseModeFillSty(conv::ReacModeCoarse_t mc) const
{
  int val = -999;
  try {
    val = fModesCoarseFillSty.at(mc);
  }
  catch(...) {
    LOG("ModeList", pWARN) << "Could not get the fill style for coarse mode \'" << util::AsString(mc)
                           << "\', instead returning " << fDefFillSty;
    val = fDefFillSty;
  }
  return val;
}
//________________________________________________________________________________________

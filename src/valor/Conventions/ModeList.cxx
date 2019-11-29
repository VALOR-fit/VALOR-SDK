//________________________________________________________________________________________
/*

Copyright (c) 2010-2019, The VALOR Fitting Group, All Rights Reserved.

ChangeLog:

*/
//________________________________________________________________________________________

#include <iostream>

#include <TXMLEngine.h>

//fmwk
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Messenger/Messenger.h"

#include "valor/Conventions/ModeList.h"

using namespace valor;
using namespace valor::conv;
using namespace std;

//________________________________________________________________________________________
ModeList::ModeList(std::string xml_file)
{
  TXMLEngine * xml_syst = new TXMLEngine;
  XMLDocPointer_t doc_syst = xml_syst->ParseFile(xml_file.c_str());
  XMLNodePointer_t root_node_syst = xml_syst->DocGetRootElement(doc_syst);
  XMLNodePointer_t curr_child_node = xml_syst->GetChild(root_node_syst);
  const string parameters_file_name(xml_syst->GetAttrValue(xml_syst->GetFirstAttr(root_node_syst)));
  //Read syst parameters from xml_syst file   
  while(curr_child_node)
  {
    const char * nc = xml_syst->GetNodeContent(curr_child_node);  
    string content(util::RemoveAllWhitespace(nc));
    conv::ReacMode_t mode = util::ReacModeFromString(content);
    if (mode==conv::kmUndefined) {
      LOG("ModeList",pFATAL) << "Got undefined mode from string "<<content;
      util::Die(-1); return;
    }
    fModes.push_back(mode);
    curr_child_node = xml_syst->GetNext(curr_child_node);
  }
  
  // We're done with the mode list XML file now
  xml_syst->FreeDoc(doc_syst);
  delete xml_syst;
  return;
}
//________________________________________________________________________________________
const std::vector<conv::ReacMode_t> & ModeList::Modes() const
{
  return fModes;
}
//________________________________________________________________________________________

//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//________________________________________________________________________________________

#include <cassert>

#include <TXMLEngine.h>
#include <TClass.h>

// fmwk
#include "valor/Core/CompoundFitElements.h"

#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Core/CompoundFitData.h"
#include "valor/Core/CompoundFitModel.h"
#include "valor/Core/CompoundCorrelatedSystematics.h"
#include "valor/Core/SingleSampleFitDataABC.h"
#include "valor/Core/SingleSampleFitModelABC.h"
#include "valor/Conventions/ParameterPool.h"

using namespace valor;
using namespace std;

//________________________________________________________________________________________
CompoundFitElements::CompoundFitElements(void)
{
  this->Init();
}
//________________________________________________________________________________________
CompoundFitElements::~CompoundFitElements(void)
{
  this->Clean();
}
//________________________________________________________________________________________
bool CompoundFitElements::Read(string xmlfile, Option_t * opt)
{
// It reads a list of fit datasets and models and setups compound datasets and fit models.
// The following XML schema is expected.
// 
// .....................................................................
// <?xml version="1.0"?>
//
// <valor>
//   <element_list name="DUNE-NDFit">
//
//      <element name="blah">
//         <detector>     dunend                       </detector>
//         <beam>         fhc                          </beam>
//         <sample>       numuccincl                   </sample>
//         <observable>   Ereco,yreco                  </observable>
//         <data>         data.xml                     </data>
//         <mc>           mc_production.xml            </mc>
//         <data_class>   valor::GenericSingleSampleData     </data_class>
//         <model_class>  valor::GenericSingleSampleFitModel </model_class>
//      </element>
//
//   </element_list>
// </valor>
// ......................................................................
//

  this->Book();
  
  vector<const CorrelatedSystematics*> syst_objects;

  VLOG("CompoundFitElements", pNOTICE) 
    << "Reading compound fit setup from XML file [" << xmlfile << "]";

  // Parse the input XML configuration file
  TXMLEngine* xmleng = new TXMLEngine;   
  XMLDocPointer_t xmldoc = xmleng->ParseFile(xmlfile.c_str());
  if (xmldoc==0) {
    VLOG("CompoundFitElements", pERROR) 
      << "Can not parse input XML file [" << xmlfile << "]";
    util::Die(1); return false;
  }
  XMLNodePointer_t root_node = xmleng->DocGetRootElement(xmldoc);
  
  // Let's make sure we do the parameter definitions before we get to the matrix definititions
  XMLNodePointer_t curr_node = xmleng->GetChild(root_node);
  while(curr_node) {
    string curr_node_name = xmleng->GetNodeName(curr_node);
    if(!strcmp(curr_node_name.c_str(),"parameter_definitions")) {
      bool success = this->ReadParameterList(xmleng,curr_node,opt);
      if (!success) {
        VLOG("CompoundFitElements", pERROR) 
            << "Failed to read parameter list. Exiting.";
        util::Die(1); return false;
      }
    }
    curr_node = xmleng->GetNext(curr_node);
  }

  // Now we'll go over the list of parameter matrices in the fit
  curr_node = xmleng->GetChild(root_node);
  while(curr_node) {
    string curr_node_name = xmleng->GetNodeName(curr_node);
    if (curr_node_name == "systematics") {
      const char * syst_name_cstr = xmleng->GetAttr(curr_node,"name");
      string syst_list_name = (syst_name_cstr ? syst_name_cstr : "NoNameFoundInXML");
      VLOG("CompoundFitElements", pNOTICE) << "Found systematic list named: " << syst_list_name;

      // Read the systematics
      XMLNodePointer_t group_node = xmleng->GetChild(curr_node);
      while (group_node) {
        syst_objects.push_back(this->ReadSystematicGroup(xmleng,group_node,opt));
        group_node = xmleng->GetNext(group_node);
      }
    }

    curr_node = xmleng->GetNext(curr_node);
  }

  if (syst_objects.size() == 0 ) {
    VLOG("CompoundFitElements", pNOTICE) << "No systematics found in config file.";
  }

  // The compound fit model must have the syst conf set before it's read
  CorrelatedSystematics * syst = new CompoundCorrelatedSystematics(syst_objects);
  fCompoundFitModel->SetSystConf(syst,"adopt");

  // Now we'll go over to get the elements
  curr_node = xmleng->GetChild(root_node);
  while(curr_node) {
    string curr_node_name = xmleng->GetNodeName(curr_node);
    if(curr_node_name == "element_list") {
       const char * list_name_cstr = xmleng->GetAttr(curr_node,"name");
       string list_name = (list_name_cstr ? list_name_cstr : "NoNameFoundInXML");
       VLOG("CompoundFitElements", pNOTICE) << "Found element list named: " << list_name;
       bool success = this->ReadElementList(xmleng,curr_node,opt);
       if (!success) {
         VLOG("CompoundFitElements", pERROR) 
            << "Failed to read all elements in list. Exiting.";
            util::Die(1); return false;
       }
       fCompoundFitData ->SetName(list_name);
       fCompoundFitModel->SetName(list_name);
       
       VLOG("CompoundFitElements", pNOTICE) << "Created model with "<<fCompoundFitModel->GetNSubModels()<<" submodels.";
    }
    
    curr_node = xmleng->GetNext(curr_node);
  }
  
  xmleng->FreeDoc(xmldoc);
  delete xmleng;

  return true;
}
//________________________________________________________________________________________
bool CompoundFitElements::ReadElementList(TXMLEngine* xmleng, XMLNodePointer_t top_node, Option_t * opt)
{
  XMLNodePointer_t curr_node = xmleng->GetChild(top_node);
  while(curr_node) {
      string curr_node_name = xmleng->GetNodeName(curr_node);
      if(curr_node_name == "element") {
        string element_name = xmleng->GetAttr(curr_node,"name");
        VLOG("CompoundFitElements", pNOTICE) << "Found element named: " << element_name;	
        bool ok = this->ReadElement(xmleng,curr_node, opt);
        if(!ok) return false;
      }
      curr_node = xmleng->GetNext(curr_node);
  }
  return true;
}
//________________________________________________________________________________________
bool CompoundFitElements::ReadElement(TXMLEngine* xmleng, XMLNodePointer_t top_node, Option_t * opt)
{
  string detector_name    = "";
  string beam_name        = "";
  string sample_name      = "";
  string observable_name  = "";
  string data_file        = "";
  string mc_file          = "";
  string data_class       = "";
  string model_class      = "";
  string binning          = "";

  XMLNodePointer_t curr_node = xmleng->GetChild(top_node);
  while(curr_node) {
      string curr_node_name = xmleng->GetNodeName(curr_node);
      if(xmleng->GetNodeContent(curr_node)) {
        string content = util::TrimSpaces(xmleng->GetNodeContent(curr_node));
        VLOG("CompoundFitElements", pNOTICE) << curr_node_name << " --> " << content;
        if(curr_node_name == "detector"   ) { detector_name   = content; }
        if(curr_node_name == "beam"       ) { beam_name       = content; }
        if(curr_node_name == "sample"     ) { sample_name     = content; }
        if(curr_node_name == "observable" ) { observable_name = content; }
        if(curr_node_name == "data"       ) { data_file       = content; }
        if(curr_node_name == "mc"         ) { mc_file         = content; }
        if(curr_node_name == "data_class" ) { data_class      = content; }
        if(curr_node_name == "model_class") { model_class     = content; }
        if(curr_node_name == "binning"    ) { binning         = content; }
      }
      curr_node = xmleng->GetNext(curr_node);
  }

  conv::Detector_t  detector   = util::DetectorFromString(detector_name);
  conv::BeamConf_t  beam       = util::BeamConfFromString(beam_name);
  conv::Sample_t    sample     = util::SampleFromString(sample_name);
  conv::KineSpace_t observable = util::KineSpaceFromString(observable_name);

  bool success = true;
  if (detector   == conv::kdUndefined ) {
    VLOG("CompoundFitElements",pERROR)<<"Unknown detector: "<<detector_name;
    success = false;
  }
  if (beam       == conv::kbUndefined ) {
    VLOG("CompoundFitElements",pERROR)<<"Unknown beam conf: "<<beam_name;
    success = false;
  }
  if (sample     == conv::ksUndefined ) {
    VLOG("CompoundFitElements",pERROR)<<"Unknown sample: "<<sample_name;
    success = false;
  }
  if (observable == conv::kspUndefined) {
    VLOG("CompoundFitElements",pERROR)<<"Unknown observable: "<<observable_name;
    success = false;
  }
  
  TString model_opt = opt;
  if (!binning.empty()) {
    model_opt = TString::Format("%s binning=%s",opt,binning.c_str());
  }

  if(!data_file.empty()) // The data tag has been filled
    fDataFileGiven = true;

  if (success) {
    FitModelABC * fit_model = 
       this->AdoptFitModel(mc_file,   detector, beam, sample, observable, model_class, model_opt.Data());
    assert(fit_model);

    FitDataABC *  fit_data  =
       this->AdoptFitData (data_file, detector, beam, sample, observable, data_class, fit_model, opt);
    assert(fit_data);
    
    fCompoundFitModel->AddSubModel(fit_model);
    fCompoundFitData ->AddSubData (fit_data);
  }

  return success;
}
//________________________________________________________________________________________
CorrelatedSystematics * CompoundFitElements::ReadSystematicGroup (
 TXMLEngine* xmleng, XMLNodePointer_t top_node, Option_t * /*opt*/)
{
  const char * syst_name_cstr = xmleng->GetAttr(top_node,"name");
  string syst_list_name = (syst_name_cstr ? syst_name_cstr : "NoNameFoundInXML");
  VLOG("CompoundFitElements", pNOTICE) << "Reading systematics group: "<<syst_list_name;
  
  CorrelatedSystematics * syst = new CorrelatedSystematics;
  syst->SetXMLNodeName("");
  syst->Read(xmleng, top_node);
  return syst;
}
//________________________________________________________________________________________
bool CompoundFitElements::ReadParameterList (
 TXMLEngine* xmleng, XMLNodePointer_t top_node, Option_t * /*opt*/)
{
  XMLNodePointer_t curr_node = xmleng->GetChild(top_node);
  while(curr_node) {
    std::string curr_node_name = xmleng->GetNodeName(curr_node);
    if(!strcmp(curr_node_name.c_str(),"filename")) {
      VLOG("CompoundFitElements", pNOTICE)
        << "Reading parameter list named \'" << xmleng->GetAttr(curr_node, "desc") << "\' into the parameter pool";
      string content = util::TrimSpaces(xmleng->GetNodeContent(curr_node));
      ParameterPool::MutableInstance()->ReadXML(content);
    }
    curr_node = xmleng->GetNext(curr_node);
  }
  ParameterPool::Instance()->CheckForCollisions();
  return true;
}
//________________________________________________________________________________________
void CompoundFitElements::Init(void)
{
  fCompoundFitData  = 0;
  fCompoundFitModel = 0;
}
//________________________________________________________________________________________
void CompoundFitElements::Book(void)
{
  fCompoundFitData  = new CompoundFitData;
  fCompoundFitModel = new CompoundFitModel;
}
//________________________________________________________________________________________
void CompoundFitElements::Clean(void)
{
  if(fCompoundFitData ) delete fCompoundFitData;
  if(fCompoundFitModel) delete fCompoundFitModel;
}
//________________________________________________________________________________________
FitDataABC * CompoundFitElements::AdoptFitData(
  string config_file, 
  conv::Detector_t d, conv::BeamConf_t b, conv::Sample_t s, conv::KineSpace_t o, 
  string class_name,
  FitModelABC *model_abc,
  Option_t * opt)
{
  void * vbase = this->InstantiateUsingDictionaryRegistration(class_name);
  SingleSampleFitDataABC * s_dataset = (SingleSampleFitDataABC *) (vbase);
  s_dataset->Configure(d,b,s,o);
  s_dataset->SetAssociatedModel(model_abc);
  s_dataset->SetFromModel();
  if(fDataFileGiven){
    VLOG("CompoundFitElements", pNOTICE)
      << "Data file given so will Read it. Will be overwritten if Generate is called later";
    s_dataset->Read(config_file, opt);
  }
  else{
    VLOG("CompoundFitElements", pNOTICE)
      << "No data file given so cannot Read, will have to Generate";
    //VLOG("CompoundFitElements", pFATAL)
    //  << "Need to write GSSFM association and Generate does not currently work yet, please pass data file to read instead";
    //util::Die(1);
  }
  FitDataABC * dataset = (FitDataABC *) (s_dataset);
  return dataset;
}
//________________________________________________________________________________________
FitModelABC * CompoundFitElements::AdoptFitModel(
  string config_file, 
  conv::Detector_t d, conv::BeamConf_t b, conv::Sample_t s, conv::KineSpace_t o,
  string class_name,
  Option_t * opt)
{
  void * vbase = this->InstantiateUsingDictionaryRegistration(class_name);
  SingleSampleFitModelABC * s_model = (SingleSampleFitModelABC *) (vbase);
  s_model->Configure(d,b,s,o);
  s_model->SetSystConf(fCompoundFitModel->GetSystematics(), "");
  s_model->Read(config_file, opt);
  FitModelABC * model = (FitModelABC *) (s_model);
  return model;
}
//________________________________________________________________________________________
void * CompoundFitElements::InstantiateUsingDictionaryRegistration(string class_name)
{
  VLOG("CompoundFitElements", pDEBUG) 
     << "Getting class: " << class_name;
  TClass * tclass = TClass::GetClass(class_name.c_str());
  if(!tclass) {
     VLOG("CompoundFitElements", pERROR) 
         << "Failed getting class = " << class_name;
     return 0;
  }
  void * vbase = tclass->New();
  if(!vbase) {
     VLOG("CompoundFitElements", pERROR) 
         << "Failed instantiating class = " << class_name;
     return 0;
  }
  return vbase;
}
//________________________________________________________________________________________


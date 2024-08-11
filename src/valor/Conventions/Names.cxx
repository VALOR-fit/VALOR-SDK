//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//________________________________________________________________________________________

#include <cassert>
#include <vector>

#include "TString.h"

// fmwk
#include "valor/Conventions/Names.h"

#include "valor/Utilities/UtlStr.h"
#include "valor/Messenger/Messenger.h"


using std::vector;

//________________________________________________________________________________________
string valor::util::nm::DSetName(
  string det_nm, string beam_nm, string sample_nm, string /*obsrv_nm*/)
{
  string dataset = det_nm + delimiter + beam_nm + delimiter + sample_nm /* + delimiter + obsrv_nm */;
  return dataset;
}
//________________________________________________________________________________________
string valor::util::nm::DSetName(
  valor::conv::Detector_t det,  valor::conv::BeamConf_t beam, 
  valor::conv::Sample_t sample, valor::conv::KineSpace_t obsrv)
{
  string det_nm    = valor::util::AsString(det);
  string beam_nm   = valor::util::AsString(beam);
  string sample_nm = valor::util::AsString(sample);
  string obsrv_nm  = valor::util::AsString(obsrv);

  string dataset = det_nm + delimiter + beam_nm + delimiter + sample_nm /* + delimiter + obsrv_nm */;
  return dataset;
}
//________________________________________________________________________________________
string valor::util::nm::DSetDetectorName(string dataset)
{
  return valor::util::nm::DSetToken(dataset, 0);
}
//________________________________________________________________________________________
valor::conv::Detector_t valor::util::nm::DSetDetector(string dataset)
{
  string detector_name = valor::util::nm::DSetDetectorName(dataset);
  return valor::util::DetectorFromString(detector_name);
}
//________________________________________________________________________________________
string valor::util::nm::DSetBeamName(string dataset)
{
  return valor::util::nm::DSetToken(dataset, 1);
}
//________________________________________________________________________________________
valor::conv::BeamConf_t valor::util::nm::DSetBeam(string dataset)
{
  string beam_name = valor::util::nm::DSetBeamName(dataset);
  return valor::util::BeamConfFromString(beam_name);
}
//________________________________________________________________________________________
string valor::util::nm::DSetSampleName(string dataset)
{
  return valor::util::nm::DSetToken(dataset, 2);
}
//________________________________________________________________________________________
valor::conv::Sample_t valor::util::nm::DSetSample(string dataset)
{
  string sample_name = valor::util::nm::DSetSampleName(dataset);
  return valor::util::SampleFromString(sample_name);
}
//________________________________________________________________________________________
/*
string valor::util::nm::DSetObsrvName(string dataset)
{

}
//________________________________________________________________________________________
KSpace_t? valor::util::nm::DSetObsrv(string dataset)
{

}
//________________________________________________________________________________________
*/
string valor::util::nm::DSetToken(string dataset, int i)
{
  const int ntokens = 3; // change to 4 once KSpace_t is used
  if(dataset.find(delimiter) != string::npos) {
    vector<string> tokens = valor::util::Tokens(dataset, delimiter);
    if(tokens.size() == ntokens) {
       if(i>=0 && i < ntokens) {
          return tokens[i];
       }
    }
  }
  VLOG("DUNE",pWARN) 
    << Form("Dataset name doesn't follow `detector%sbeam%ssample%sobservable' convention",
     delimiter.c_str(),delimiter.c_str(),delimiter.c_str());

  return "";
}
//________________________________________________________________________________________
//________________________________________________________________________________________
string valor::util::nm::MdlName(
  string det_nm, string beam_nm, string sample_nm, string obsrv_nm)
{
  return valor::util::nm::DSetName(det_nm,beam_nm,sample_nm,obsrv_nm);
}
//________________________________________________________________________________________
string valor::util::nm::MdlName(
  valor::conv::Detector_t det,  valor::conv::BeamConf_t beam, 
  valor::conv::Sample_t sample, valor::conv::KineSpace_t obsrv)
{
  return valor::util::nm::DSetName(det,beam,sample,obsrv);
}
//________________________________________________________________________________________
string valor::util::nm::MdlDetectorName(string dataset)
{
  return valor::util::nm::DSetDetectorName(dataset);
}
//________________________________________________________________________________________
valor::conv::Detector_t valor::util::nm::MdlDetector(string dataset)
{
  return valor::util::nm::DSetDetector(dataset);
}
//________________________________________________________________________________________
string valor::util::nm::MdlBeamName(string dataset)
{
  return valor::util::nm::DSetBeamName(dataset);
}
//________________________________________________________________________________________
valor::conv::BeamConf_t valor::util::nm::MdlBeam(string dataset)
{
  return valor::util::nm::DSetBeam(dataset);
}
//________________________________________________________________________________________
string valor::util::nm::MdlSampleName(string dataset)
{
  return valor::util::nm::DSetSampleName(dataset);
}
//________________________________________________________________________________________
valor::conv::Sample_t valor::util::nm::MdlSample(string dataset)
{
  return valor::util::nm::DSetSample(dataset);
}
//________________________________________________________________________________________
/*
string valor::util::nm::MdlObsrvName(string dataset)
{

}
//________________________________________________________________________________________
KSpace_t? valor::util::nm::MdlObsrv(string dataset)
{

}
//________________________________________________________________________________________
*/
string valor::util::nm::MdlToken(string dataset, int i)
{
  return valor::util::nm::DSetToken(dataset, i);
}
//________________________________________________________________________________________
//________________________________________________________________________________________
string valor::util::nm::DatumName(
  string tag, string mother_obj_name, string quantity, string format)
{
  string datum_name = (tag.size() == 0) ?
     (                   mother_obj_name + delimiter + quantity + delimiter + format ):
     ( tag + delimiter + mother_obj_name + delimiter + quantity + delimiter + format );

  return datum_name;
}
//________________________________________________________________________________________


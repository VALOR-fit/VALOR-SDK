//________________________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

ChangeLog:

*/
//________________________________________________________________________________________

#include <iostream>
#include <map>
#include <vector>

#include "TClass.h"
#include "TFile.h"
#include "TObjArray.h"
#include "TSystem.h"

#include "valor/Conventions/Build.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/UtlXML.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/VThreadPool.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/VArray/VALORArray.h"
#include "valor/FitCore/CompoundFitModel.h"
#include "valor/FitElements/GenericSingleSampleFitModel.h"

using namespace valor;
using namespace valor::conv;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

//________________________________________________________________________________________
CompoundFitModel::CompoundFitModel() :
 fSystematics(0),
 fOwnerOfSystematics(false),
 fDefVALORArrayFormat("pdft_vf")
{
  fName = "UninitialisedCompoundFitModel";
}
//________________________________________________________________________________________
CompoundFitModel::~CompoundFitModel() 
{
  if (fSystematics && fOwnerOfSystematics) { delete fSystematics; }
  for (FitModelABC *& m : fSubModels) {
    delete m; m=0;
  }
}
//________________________________________________________________________________________
std::string CompoundFitModel::CompoundName() const
{
  std::string name = "--";
  for (std::vector<FitModelABC*>::const_iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    name += (*it)->Name() + "--";
  }
  return name;
}
//________________________________________________________________________________________
std::string CompoundFitModel::Name() const
{
  return fName;
}
//________________________________________________________________________________________
bool CompoundFitModel::Read(string file, Option_t * opt)
{
  bool status = true;
  
  // Parse the input XML configuration file
  TXMLEngine* xml = new TXMLEngine;   
  XMLDocPointer_t xmldoc = xml->ParseFile(file.c_str());
  if (xmldoc==0) {
    VLOG("CompoundFitModel",pERROR) << "Can not parse input XML file [" << file << "]";
    delete xml;
    util::Die(1); return false;
  }
  XMLNodePointer_t root_node = xml->DocGetRootElement(xmldoc);
  
  for ( XMLNodePointer_t curr_node = util::FindNode(xml,root_node,"model");
        (!(xml->IsEmptyNode(curr_node)));
        curr_node = xml->GetNext(curr_node) )
  {
    string model_name = util::XML2String(xml,curr_node,"name");
    string class_name = util::XML2String(xml,curr_node,"class");
    TString config_path = util::XML2String(xml,curr_node,"config");
    gSystem->ExpandPathName(config_path);
    
    // Use factory class to get proper model
    TClass * curr_class = TClass::GetClass(class_name.c_str());
    if (!curr_class) {
      VLOG("CompoundFitModel",pERROR) << "Cannot find class "<<class_name;
      status = false;
      continue;
    }
    void * curr_obj = curr_class->New();
    FitModelABC * curr_model = static_cast<FitModelABC*>(curr_obj);
    
    if (!curr_model) {
      VLOG("CompoundFitModel",pERROR) << "Failed to properly turn class "<<class_name;
      status = false;
      continue;
    }
    
    status = curr_model->Read(config_path.Data(), opt);
    
    fSubModels.push_back(curr_model);
    
  }
  
  if (not status) { util::Die(-1); }
  
  VLOG("CompoundFitModel",pNOTICE) << "Created fit model from "<<fSubModels.size()<<" submodels.";
  
  return status;
}
//________________________________________________________________________________________
bool CompoundFitModel::Read(util::ToyStream* stream)
{
   bool status = true;
   for(std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it)
   {
      status = status && (*it)->Read(stream);
   }

   return status;
}
//________________________________________________________________________________________
long CompoundFitModel::StreamSize(Option_t* opt) const
{
  long size = 0;
  for(std::vector<FitModelABC*>::const_iterator it = fSubModels.begin(); it != fSubModels.end(); ++it)
  {
    size += (*it)->StreamSize(opt);
  }

  return size;
}
//________________________________________________________________________________________
bool CompoundFitModel::Generate(const PhysHypothesis & ph, Option_t * opt)
{
  bool status = true;

#ifdef __VALOR_FMWK_PARALLEL__

  // For now, do JUNO PID path serially.
  // Multi-sample PID needs all four model templates before UpdatePDFs(),
  // so avoid parallel finalization until the flow is validated.

#endif

  std::vector<GenericSingleSampleFitModel *> gssfm_models;
  bool has_juno_detresp_pid = false;

  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin();
       it != fSubModels.end(); ++it) {

    GenericSingleSampleFitModel * gssfm =
      dynamic_cast<GenericSingleSampleFitModel *>(*it);

    if (gssfm) {
      gssfm_models.push_back(gssfm);

      if (gssfm->UseJunoDetectorResponse() &&
          gssfm->JunoApplyPID()) {
        has_juno_detresp_pid = true;
      }
    }
  }

  if (has_juno_detresp_pid) {

    VLOG("CompoundFitModel", pNOTICE)
    << "[JUNO PID] Using split GeneratePrePDF -> "
    << "ApplyJunoPID -> FinalizePDF flow";

    for (std::vector<FitModelABC*>::iterator it = fSubModels.begin();
         it != fSubModels.end(); ++it) {

      GenericSingleSampleFitModel * gssfm =
        dynamic_cast<GenericSingleSampleFitModel *>(*it);

      if (gssfm) {
        status = status && gssfm->GeneratePrePDF(ph, opt);
      }
      else {
        status = status && (*it)->Generate(ph, opt);
      }
    }

    if (!status) return false;

    this->ApplyJunoPID();

    for (std::vector<GenericSingleSampleFitModel *>::iterator it =
           gssfm_models.begin();
         it != gssfm_models.end(); ++it) {

      status = status && (*it)->FinalizePDF(opt);
    }

    return status;
  }

  // Original behavior when JUNO PID is disabled.
#ifdef __VALOR_FMWK_PARALLEL__
  std::atomic_bool astat(status);

  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin();
       it != fSubModels.end(); ++it) {

    FitModelABC & m = **it;
    std::function<void(void)> f =
      [&]{ astat = astat && m.Generate(ph, opt); };

    util::VThreadPool::Instance()->AddTask(f);
  }

  util::VThreadPool::Instance()->Wait();
  status = astat;
#else
  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin();
       it != fSubModels.end(); ++it) {

    status = status && (*it)->Generate(ph, opt);
  }
#endif

  return status;
}
//________________________________________________________________________________________
// bool CompoundFitModel::Generate (const FitParamList & fp, Option_t * opt)
// {
//   bool status = true;
//   if (fSystematics) {
//     fSystematics->UpdateFrom(fp);
//     status = this->Generate(PhysHypothesis(fp),opt);
//   }
//   else {
// #ifdef __VALOR_FMWK_PARALLEL__
//     std::atomic_bool astat(status);
//     for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
//       FitModelABC & m = **it;
//       std::function<void(void)> f = [&]{astat = astat && m.Generate(fp,opt);};
//       util::VThreadPool::Instance()->AddTask(f);
//     }
//     util::VThreadPool::Instance()->Wait();
//     status = astat;
// #else
//     for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
//       status = status && (*it)->Generate(fp,opt);
//     }
// #endif //__VALOR_FMWK_PARALLEL__
//   }
//   return status;
// }
//________________________________________________________________________________________
bool CompoundFitModel::Generate(const FitParamList & fp, Option_t * opt)
{
  bool status = true;

  if (fSystematics) {
    fSystematics->UpdateFrom(fp);
  }

  bool has_juno_pid = false;

  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin();
       it != fSubModels.end(); ++it) {

    GenericSingleSampleFitModel * gssfm =
      dynamic_cast<GenericSingleSampleFitModel *>(*it);

    if (!gssfm) continue;

    if (gssfm->UseJunoDetectorResponse() &&
        gssfm->JunoApplyPID()) {
      has_juno_pid = true;
      break;
    }
  }

  // Use the split PID-aware flow only when needed.
  if (has_juno_pid) {
    return this->Generate(PhysHypothesis(fp), opt);
  }

  // Otherwise preserve original behavior.
#ifdef __VALOR_FMWK_PARALLEL__
  std::atomic_bool astat(status);

  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin();
       it != fSubModels.end(); ++it) {

    FitModelABC & m = **it;

    std::function<void(void)> f =
      [&]{ astat = astat && m.Generate(fp, opt); };

    util::VThreadPool::Instance()->AddTask(f);
  }

  util::VThreadPool::Instance()->Wait();
  status = astat;

#else

  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin();
       it != fSubModels.end(); ++it) {

    status = status && (*it)->Generate(fp, opt);
  }

#endif

  return status;
}
//________________________________________________________________________________________
void CompoundFitModel::RandomizeSystematics (FitParamList& fpl, Option_t * opt)
{
  if (fSystematics) {  // If we have our own systematic model, tweak that
    fSystematics->Tweak(false, NULL, fpl, opt);
  }
  else {// otherwise loop over all submodels.
    for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
      (*it)->RandomizeSystematics(fpl, opt);
    }
  }
}
//________________________________________________________________________________________
void CompoundFitModel::RandomizeSystematics (bool (*should_randomize)(conv::PhysParamId_t), FitParamList& fpl, Option_t * opt)
{
  if (fSystematics) {  // If we have our own systematic model, tweak that
    fSystematics->Tweak(false, NULL, should_randomize, fpl, opt);
  }
  else {// otherwise loop over all submodels.
    for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
      (*it)->RandomizeSystematics( should_randomize, fpl, opt);
    }
  }
}
//________________________________________________________________________________________
void CompoundFitModel::RandomizeSystematics (std::map<valor::conv::PhysParamId_t, bool> should_randomize, FitParamList& fpl, Option_t * opt)
{
  if (fSystematics) {  // If we have our own systematic model, tweak that
    fSystematics->Tweak(false, NULL, should_randomize, fpl, opt);
  }
  else {// otherwise loop over all submodels.
    for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
      (*it)->RandomizeSystematics(should_randomize, fpl, opt);
    }
  }
}
//________________________________________________________________________________________
void CompoundFitModel::TweakSystematic (string syst_name, double twkdial, Option_t * opt)
{
  PhysParamId_t syst = util::NameRegistry::Instance()->PhysParamFromString(syst_name);
  if(syst == conv::kPhypUndefined) {
    VLOG("CompoundFitModel",pERROR) << "Could not recognise parameter with name \'" << syst_name << "\'";
    util::Die(-1);
  }
  this->TweakSystematic(syst,twkdial,opt);
}
//________________________________________________________________________________________
void CompoundFitModel::TweakSystematic (conv::PhysParamId_t syst, double twkdial, Option_t * opt)
{
  // If we have our own systematic model, tweak that
  if (fSystematics) {
    fSystematics->SetCurrentValue(syst, twkdial);
  }
  else { // otherwise loop over all submodels.
    for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
      (*it)->TweakSystematic(syst, twkdial, opt);
    }
  }
}
//________________________________________________________________________________________
void CompoundFitModel::ResetSystematics (FitParamList& fpl, Option_t * opt) {
  if (fSystematics) {
    fSystematics->Tweak(true,NULL, fpl);
  }
  else {
    for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
      (*it)->ResetSystematics(fpl, opt);
    }
  }
  return;
}
//________________________________________________________________________________________
void CompoundFitModel::ForceSystematicsToUntuned (FitParamList& fpl, Option_t * opt) {
  if (fSystematics) {
    fSystematics->Tweak(true,&(fSystematics->UntunedValues()), fpl);
  }
  else {
    for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
      (*it)->ForceSystematicsToUntuned(fpl, opt);
    }
  }
  return;
}
//________________________________________________________________________________________
void CompoundFitModel::PrintSystematics (std::ostream & stream, Option_t * opt) const
{
  if (fSystematics) {
    fSystematics->Print(opt);
  }
  else {
    for (std::vector<FitModelABC*>::const_iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
      (*it)->PrintSystematics(stream, opt);
    }
  }
  return;
}
//________________________________________________________________________________________
PhysParamList CompoundFitModel::Tweaks(Option_t * opt) const
{
  PhysParamList ppl;
  if (fSystematics) {
    fSystematics->CopyTweaksTo(ppl);
  }
  else {
    for (std::vector<FitModelABC*>::const_iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
      PhysParamList ppl2 = (*it)->Tweaks();
      ppl.MergeFrom(ppl2,opt);
    }
  }
  return ppl;
}
//________________________________________________________________________________________
bool CompoundFitModel::Write (TObjArray* collection, const char * tag, Option_t * write_opt)
{
  bool status = true;
  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    status = status && (*it)->Write(collection,tag,write_opt);
  }
  return status;
}
//________________________________________________________________________________________
bool CompoundFitModel::Write (const char * filename, const char * tag, Option_t * write_opt, Option_t * file_opt)
{
  VLOG("CompoundFitModel",pNOTICE) << "Writing results to file: [" << filename << "]";

  TObjArray * array = new TObjArray();
  bool status = this->Write(array,tag,write_opt);

  Option_t * used_file_opt = (file_opt == 0) ? "recreate" : file_opt;
  TFile f(filename, used_file_opt);
  array->Write();
  f.Close();

  return status;
}
//________________________________________________________________________________________
bool CompoundFitModel::Write(util::ToyStream* stream)
{
   bool status = true;
   for(std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it)
   {
      status = status && (*it)->Write(stream);
   }

   return status;
}
//________________________________________________________________________________________
double CompoundFitModel::GetFitStatisticPenalty(Option_t * opt) const
{
  if (fSystematics) {
    return fSystematics->FitStatisticPenalty(opt);
  }
  VLOG("CompoundFitModel",pNOTICE) << "No global covariance defined. Using submodel covariances for fit statistic penalty - may be wrong.";
  double fitstat = 0;
  for (std::vector<FitModelABC*>::const_iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    fitstat+= (*it)->GetFitStatisticPenalty(opt);
  }
  return fitstat;
}
//________________________________________________________________________________________
void CompoundFitModel::AddSubModel(FitModelABC * model, Option_t * /*opt*/)
{
  fSubModels.push_back(model);
  return;
}
//________________________________________________________________________________________
const FitModelABC * CompoundFitModel::GetSubModel(int model_idx, Option_t * /*opt*/) const
{
  return fSubModels.at(model_idx);
}
//________________________________________________________________________________________
FitModelABC * CompoundFitModel::GetSubModel(int model_idx, Option_t * /*opt*/)
{
  return fSubModels.at(model_idx);
}
//________________________________________________________________________________________
const std::vector<FitModelABC*> CompoundFitModel::GetSubModels(Option_t * /*opt*/) const
{
  return fSubModels;
}
//________________________________________________________________________________________
std::vector<FitModelABC*> CompoundFitModel::GetSubModels(Option_t * /*opt*/)
{
  return fSubModels;
}
//________________________________________________________________________________________
unsigned int CompoundFitModel::GetNSubModels(Option_t * /*opt*/) const
{
  return fSubModels.size();
}
//________________________________________________________________________________________
CorrelatedSystematics * CompoundFitModel::GetSystematics(Option_t * /*opt*/)
{
  return fSystematics;
}
//________________________________________________________________________________________
const CorrelatedSystematics * CompoundFitModel::GetSystematics(Option_t * /*opt*/) const
{
  return fSystematics;
}
//________________________________________________________________________________________
void CompoundFitModel::SetSystConf(CorrelatedSystematics * syst, Option_t * opt)
{
  TString ts(opt);
  if (ts.Contains("copy") ) {
    fSystematics = dynamic_cast<CorrelatedSystematics *>(syst->Clone());
    fOwnerOfSystematics = true;
  }
  else {
    fSystematics = syst;
    fOwnerOfSystematics = ts.Contains("adopt");
  }
  
  // Set the same matrix for the submodels
  // Submodels will never own
  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    (*it)->SetSystConf(syst,"");
  }
  
  return;
}
//________________________________________________________________________________________
void CompoundFitModel::SetName(std::string name)
{
  fName = name;
}
//________________________________________________________________________________________
std::string CompoundFitModel::GetPDFIntegralAsString(Option_t * opt)
{
  std::string to_print = "";

  for (std::vector<FitModelABC*>::iterator it = fSubModels.begin(); it != fSubModels.end(); ++it) {
    to_print += (*it)->GetPDFIntegralAsString(opt) + "\n";
  }

  return to_print;
}
//________________________________________________________________________________________
util::ToyStream* CompoundFitModel::InitToyStream(const string file_name, const string read_or_write, string valor_array_format)
{
  valor_array_format = util::RemoveAllWhitespace(valor_array_format);
  if(valor_array_format.empty()) {
    valor_array_format = fDefVALORArrayFormat;
  }

  return new util::ToyStream(file_name, read_or_write, typeid(this), this->CompoundName(),
                             valor_array_format, this->StreamSize());
}
//________________________________________________________________________________________
int JunoPIDLabel(conv::Sample_t s)
{
  if (s == conv::ksJUNO_FC_CC_Nue)    return 0;
  if (s == conv::ksJUNO_FC_CC_Numu)   return 1;
  if (s == conv::ksJUNO_FC_CC_Nuebar) return 2;
  if (s == conv::ksJUNO_FC_CC_Numubar)return 3;
  return -1;
}
//________________________________________________________________________________________
void CompoundFitModel::ApplyJunoPID()
{
  std::vector<GenericSingleSampleFitModel *> models;

  for (FitModelABC * m : fSubModels) {
    GenericSingleSampleFitModel * g =
      dynamic_cast<GenericSingleSampleFitModel *>(m);

    if (!g) continue;
    if (!g->UseJunoDetectorResponse()) continue;
    if (!g->JunoApplyPID()) continue;
    if (JunoPIDLabel(g->Sample()) < 0) continue;

    models.push_back(g);
  }

  if (models.size() != 4) {
    VLOG("CompoundFitModel", pWARN)
      << "[JUNO PID] Expected 4 JUNO CC samples, found "
      << models.size() << ". Skipping.";
    return;
  }

  const VALORBinning * reco_binning = models.front()->RecoBinning();
  if (!reco_binning) return;

  const std::vector<KineVar_t> & reco_vars = reco_binning->KineVars();
  if (reco_vars.empty()) return;

  const KineVar_t reco_E_var = reco_vars[0];
  const int n_reco_bins = reco_binning->NBins();

  auto pid_group_from_reco_energy = [](double Ereco) -> int {
    const double e_edges[11] = {
      1.08508435, 1.41421357, 1.84317469, 2.40224887, 3.13090217,
      4.08057156, 5.31829591, 6.93144844, 9.03390453,
      11.77408038, 20.0
    };

    int ebin = 9;
    for (int i = 0; i < 10; ++i) {
      if (Ereco < e_edges[i + 1]) {
        ebin = i;
        break;
      }
    }

    if (ebin <= 2) return 0;
    if (ebin == 3) return 1;
    if (ebin == 4) return 2;
    if (ebin == 5) return 3;
    if (ebin == 6) return 4;
    if (ebin == 7) return 5;
    if (ebin == 8) return 6;
    return 7;
  };

  auto holder_mode_index = [](GenericSingleSampleFitModel * g) -> int {
    for (unsigned int i = 0; i < g->NReactionModes(); ++i) {
      if (g->IsNullMode(i)) continue;
      if (!g->MCTFinOb(i)) continue;
      return static_cast<int>(i);
    }
    return -1;
  };

  std::map<GenericSingleSampleFitModel *, std::vector<VALORArray> > before;
  std::map<GenericSingleSampleFitModel *, int> holder;

  for (GenericSingleSampleFitModel * g : models) {
    const int iholder = holder_mode_index(g);

    if (iholder < 0) {
      VLOG("CompoundFitModel", pWARN)
        << "[JUNO PID] No valid holder mode for sample "
        << util::AsString(g->Sample())
        << ". Skipping PID.";
      return;
    }

    holder[g] = iholder;

    std::vector<VALORArray> copies;

    for (unsigned int imode = 0; imode < g->NReactionModes(); ++imode) {
      if (g->IsNullMode(imode) || !g->MCTFinOb(imode)) {
        copies.push_back(VALORArray());
        continue;
      }

      copies.push_back(*(g->MCTFinOb(imode)));
      g->MCTFinOb(imode)->SetAll(0.0);
    }

    before[g] = copies;
  }

  for (GenericSingleSampleFitModel * true_model : models) {
    const int true_label = JunoPIDLabel(true_model->Sample());
    if (true_label < 0) continue;

    for (unsigned int imode_true = 0;
         imode_true < true_model->NReactionModes();
         ++imode_true) {

      if (true_model->IsNullMode(imode_true)) continue;
      if (!true_model->MCTFinOb(imode_true)) continue;

      const VALORArray & src = before[true_model][imode_true];
      const int n_true_bins = src.GetSize() / n_reco_bins;

      for (GenericSingleSampleFitModel * reco_model : models) {
        const int reco_label = JunoPIDLabel(reco_model->Sample());
        if (reco_label < 0) continue;

        const int imode_reco = holder[reco_model];

        VALORArray * dst =
          reco_model->MCTFinOb(static_cast<unsigned int>(imode_reco));

        if (!dst) continue;

        for (int ireco = 0; ireco < n_reco_bins; ++ireco) {
          const double e_low =
            reco_binning->BinMinimum(ireco, reco_E_var);
          const double e_high =
            reco_binning->BinMaximum(ireco, reco_E_var);
          const double Ereco = 0.5 * (e_low + e_high);

          const int group = pid_group_from_reco_energy(Ereco);

          const double pid =
            true_model->JunoPIDProb(group, true_label, reco_label);

          if (pid <= 0.0) continue;

          for (int itrue = 0; itrue < n_true_bins; ++itrue) {
            const double v = src.GetEntry(ireco, itrue);
            if (v == 0.0) continue;

            const double old = dst->GetEntry(ireco, itrue);
            dst->SetEntry(old + v * pid, ireco, itrue);
          }
        }
      }
    }
  }

  VLOG("CompoundFitModel", pNOTICE)
    << "[JUNO PID] Applied multi-sample PID using holder modes.";
}
//________________________________________________________________________________________

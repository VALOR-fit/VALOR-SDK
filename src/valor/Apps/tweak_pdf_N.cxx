//____________________________________________________________________________
/*!

\app       valor-tweak_pdf

\brief     Vary the MC spectra by throwing sets of systematics

\options     --output              : Where to save tweaked histograms to. [def: tweaks_[PARAMS].root]
             --ntoys               : Number of toy-MC experiments [def: 200]
             --physics-hypothesis  : physics hypothesis [def: "3f_osc"]
             --fit-param-opt       : Options used when setting up the FitParamList
                                     [def: "NONE"]
             --config              : Analysis configuration file [required]
             --mesg-thresholds     : Message thresholds [def: "verbosity.xml"]
             --enu-reco-max        : Maximum Ereco to show on the plot [def: 10]
             --params              : Which parameters to tweak [def: allsyst]

             --input-mixing12,etc  : Option to give input values to osc params (in .arg)

             --load-prior-from-file : Option for file to give constrained prior matrix [ use default prior ]
             --prior-fit-number     : Option to specify the entry in the tree for above file [ 0 ]

             [--nthreads ]         : Number of CPU threads to use [ std::thread::hardware_concurrency ]


\author    Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           Steve Dennis <s.r.dennis \at liverpool.ac.uk>
           University of Liverpool

\created   October 2016

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

// ROOT
#include "TCanvas.h"
#include "TFile.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TString.h"
#include "TSystem.h"
// VALOR/SDK
#include "valor/Conventions/Opt.h"
#include "valor/CmdLine/CmdLnArgList.h"
#include "valor/FitCore/CorrelatedSystematics.h"
#include "valor/FitCore/PhysParamList.h"
#include "valor/FitCore/FitParamList.h"
#include "valor/FitCore/FitModelABC.h"
#include "valor/FitCore/FitDataABC.h"
//#include "valor/FitCore/Likelihood.h"
#include "valor/FitCore/FitterABC.h"
#include "valor/FitCore/UtlFit.h"
#include "valor/FitCore/CompoundFitElements.h"
#include "valor/FitCore/SingleSampleFitModelABC.h"
#include "valor/FitCore/UtlFitParamList.h"
#include "valor/Utilities/OscParamValues.h"
#include "valor/Utilities/UtlStyle.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/Env.h"
#include "valor/Utilities/VThreadPool.h"
#include "valor/Utilities/UtlParam.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Ntuple/StdFitNtp.h"

using namespace std;
using namespace valor;
using namespace valor::conv;

const double enu_reco_max = 10;

const char * fit_result_tree_name = "valor_fit_ntp";

const KineVar_t kv = conv::kvEreco;

int main(int argc, char ** argv)
{
  valor::util::PrintBanner(Env::Instance()->Banner(),500);

  CmdLnArgList arg;
  arg.Read(argc,argv,"$VALOR_FMWK/config/apps/tweak_pdf_N.arg:$VALOR_FMWK/config/apps/seeds.arg");
  LOG("valor", pNOTICE) << arg;

  // Read the command line
  int ntoys               = arg.GetInt("ntoys");
  int n_threads           = arg.GetInt("nthreads");
  string hypname          = arg.GetString("physics-hypothesis");
  string fplopt           = arg.GetString("fit-param-opt");
  string output_rootfile  = arg.GetString("output");
  string params_to_tweak  = arg.GetString("params");

  string read_prior_from_file = arg.GetString("load-prior-from-file");
  int prior_fit_number = arg.GetInt("prior-fit-number");

  std::string mesg_thresholds = util::CompleteConfigPath(arg.GetString("mesg-thresholds"));
  Messenger::Instance()->Configure(mesg_thresholds);

  // Set up the thread pool
  if(n_threads>0) {//without n-cores specified, use as many as the host has
    util::VThreadPool::Instance()->SetNThreads(n_threads);
  }

  // Set the random seeds
  Rnd::Instance()->SetSeeds(arg);

  // And OA parameters
  Opt::Instance()->gMassHierarchyAssumed = ( (arg.GetBool("inverted"))     ? conv::kNmhInverted      : conv::kNmhNormal);

  conv::NuOscDmsqScheme_t dmsq_scheme = util::DmsqSchemeFromString(arg.GetString("dmsq-scheme"));
  if(dmsq_scheme == kNuDmsqUndefined) {
    LOG("valor", pFATAL) << "Could not parse the input Dmsq scheme: [" << arg.GetString("dmsq-scheme") << "]";
    util::Die(-1);
  }
  Opt::Instance()->gOscDmsqScheme = dmsq_scheme;

  double mixing12 = arg.GetDouble("input-mixing12");
  double mixing13 = arg.GetDouble("input-mixing13");
  double mixing23 = arg.GetDouble("input-mixing23");
  double dm12sq   = arg.GetDouble("input-dm12sq");
  double dm23sq   = arg.GetDouble("input-dm23sq");
  double deltacp  = arg.GetDouble("input-deltaCP");

  // Config
  std::string config_path = util::CompleteConfigPath(arg.GetString("config"));

  // Print settings
  cout << "\n[Job inputs]" << endl;
  cout << "- Reading analysis configuration from: " << config_path << endl;
  cout << "- Model physics hypothesis : " << hypname << endl;

  CompoundFitElements * elements = new CompoundFitElements();
  elements->Read(config_path.c_str());
  FitModelABC * model = elements->FitModel();
  CompoundFitModel * cm = dynamic_cast<CompoundFitModel*>(model);
  assert(cm);

  // Arrays to store the spectra for the default, +1 sigma and -1 sigma variations

  vector<TH1D *> nom_pdf_werr;
  vector<TH1D *> nom_pdf     ;
  vector<TH1D *> err_p1      ;
  vector<TH1D *> err_m1      ;

  vector<double> max(cm->GetNSubModels());
  vector<vector<TH1D *> > twk_pdf(cm->GetNSubModels());
  CorrelatedSystematics * syst = cm->GetSystematics();

  // If necessary, update the systematics prior from a previous fit
  TFile * postfit_matrix_file = NULL;
  if (read_prior_from_file!="-" && read_prior_from_file!="none") {
    postfit_matrix_file = new TFile(read_prior_from_file.c_str());
    FitParamList * fp = new FitParamList;
    cout << "Using postfit matrix "<<prior_fit_number<<" from "<<read_prior_from_file<<endl;
    TTree * fit_tree = (TTree*)postfit_matrix_file->Get(fit_result_tree_name);
    assert(fit_tree);
    fit_tree->SetBranchAddress("best_fit_params",&fp);
    fit_tree->GetEntry(prior_fit_number);
    syst->UpdateCentralValuesAndCovarianceFromFPL(*fp);
  }
  else {
    cout << "Using prefit"<<endl;
  }

  // Generate pdfs
  for(int itoy = -1; itoy < ntoys; itoy++) {
    cout << "** Toy-MC: #" << itoy << endl;

    // (toy-MC #-1 is special: default)
    FitParamList * fp = util::GetFitParameterList(hypname.c_str(), fplopt.c_str());

    if (fp->Exists(conv::kPhypMixing12)) { fp->Update(conv::kPhypMixing12,mixing12); }
    if (fp->Exists(conv::kPhypMixing13)) { fp->Update(conv::kPhypMixing13,mixing13); }
    if (fp->Exists(conv::kPhypMixing23)) { fp->Update(conv::kPhypMixing23,mixing23); }
    if (fp->Exists(conv::kPhypDm12sq  )) { fp->Update(conv::kPhypDm12sq  ,dm12sq  ); }
    if (fp->Exists(conv::kPhypDm23sq  )) { fp->Update(conv::kPhypDm23sq  ,dm23sq  ); }
    if (fp->Exists(conv::kPhypCPV     )) { fp->Update(conv::kPhypCPV     ,deltacp ); }


    model->ResetSystematics("all");
    // Randomise the systematics in the model, and copy them to the FPL
    std::vector<PhysParamId_t> param_set = util::GetParameterSet(params_to_tweak);
    std::map<PhysParamId_t,bool> param_map;

    assert(param_set.size());

    for (unsigned int ip = 0 ; ip < param_set.size() ; ip++) {
      PhysParamId_t p = param_set[ip];
      if(!util::IsSystematic(p)) { continue; }
      param_map[p] = true;
    }

    if (itoy > -1) {
      model->RandomizeSystematics(param_map);
    }
    fp->FromPhysParamList(model->Tweaks(),"current");

    // Generate predictions
    model->Generate(*fp,"+SB");

    // Array to store results
    TObjArray * array = new TObjArray();
    array->SetOwner(true);

    // Write stuff out
    model->Write(array,"curr","pdfts_rf +SB");

    // Pick results wanted
    for(unsigned int imodel = 0; imodel < cm->GetNSubModels(); imodel++) {
      SingleSampleFitModelABC * this_model = dynamic_cast<SingleSampleFitModelABC*>(cm->GetSubModel(imodel));
      BeamConf_t b = this_model->GetBeam();
      Sample_t   s = this_model->GetSample();
      TString name = TString::Format("curr_pdfts_sb_rf_%s_%s",
         this_model->Name().c_str(),util::AsString(kv).c_str());
      TH1D * curr_pdf = (TH1D*) array->FindObject(name.Data());
      if (!curr_pdf) {
        cerr<<"Could not find histogram "<<name<<endl;
        exit(-1);
      }
      if(itoy < 0) {
         nom_pdf_werr.push_back(new TH1D(*curr_pdf));
         nom_pdf     .push_back(new TH1D(*curr_pdf));
      } else {
         twk_pdf[imodel].push_back(new TH1D(*curr_pdf));
      }
    } //imodel
    array->Clear("C");
    delete array;
  }//itoy

  for(unsigned int imodel = 0; imodel < cm->GetNSubModels(); imodel++) {
    err_m1.push_back(new TH1D(*nom_pdf[imodel]));
    err_p1.push_back(new TH1D(*nom_pdf[imodel]));
    err_m1.back()->Reset();
    err_p1.back()->Reset();
    for(int ebin = 1; ebin <= nom_pdf[imodel]->GetNbinsX(); ebin++) {
       double nom_val = nom_pdf[imodel]->GetBinContent(ebin);
       double sumsq   = 0;
       for(int itoy = 0; itoy < ntoys; itoy++) {
          double curr_val = twk_pdf[imodel][itoy]->GetBinContent(ebin);
          sumsq += TMath::Power(curr_val-nom_val,2.);
       }//itoy
       double one_sigma = TMath::Sqrt(sumsq/ntoys);
       err_p1[imodel]->SetBinContent(ebin, nom_val + one_sigma);
       err_m1[imodel]->SetBinContent(ebin, nom_val - one_sigma);
       nom_pdf_werr [imodel] -> SetBinError(ebin,one_sigma);
       if( nom_pdf[imodel]->GetBinCenter(ebin) < enu_reco_max ) {
          max [imodel] = TMath::Max(max[imodel], nom_val + one_sigma);
       }
    }//ebin
  }//imodel


  // Draw and (if necessary), write to file
  TFile * outfile = NULL;
  if (output_rootfile!="-" && output_rootfile!="none") {
    output_rootfile = util::ReplaceAll(output_rootfile,"[PARAMS]",params_to_tweak);
    outfile = new TFile(output_rootfile.c_str(),"RECREATE");
    LOG("valor",pNOTICE)<<"Writing to file: "<<output_rootfile;
  }

  for(unsigned int imodel = 0; imodel < cm->GetNSubModels(); imodel++) {
    SingleSampleFitModelABC * this_model = dynamic_cast<SingleSampleFitModelABC*>(cm->GetSubModel(imodel));
    BeamConf_t b = this_model->GetBeam();
    Sample_t   s = this_model->GetSample();
    //~ ic++;
    //~ c[ibeam]->cd(ic);
    //~ TH1F * hframe = c[ibeam]->GetPad(ic)->DrawFrame(0.,0.,enu_reco_max, 1.24*max[imodel]);
    //~ hframe -> GetXaxis() -> SetTitle("Reco. E_{#nu} (GeV)");
    //~ hframe -> GetYaxis() -> SetTitle("");
  //~ //hframe -> GetYaxis() -> SetTitle("Events per bin");
    //~ hframe -> Draw();
    //~ nom_pdf_werr [imodel] -> SetFillColor(kRed);
    //~ nom_pdf_werr [imodel] -> SetLineColor(kRed);
    //~ nom_pdf_werr [imodel] -> SetLineWidth(0);
    //~ nom_pdf_werr [imodel] -> SetMarkerSize(0);
    //~ nom_pdf_werr [imodel] -> Draw("E2SAME");
    //~ nom_pdf      [imodel] -> SetLineColor(kBlack);
    //~ nom_pdf      [imodel] -> SetMarkerSize(0);
    //~ nom_pdf      [imodel] -> SetLineWidth(1);
    //~ nom_pdf      [imodel] -> Draw("SAME");
    //~ TLatex * title = new TLatex(1,1.03*max[imodel],Form("%s / %s", util::AsString(b).c_str(), util::AsStringTeX(s,true).c_str()));
    //~ title->SetTextSize(0.08);
    //~ title->Draw();
    //~ TLegend * leg = new TLegend(0.50, 0.50, 0.92, 0.80);
    //~ leg->SetFillColor(0);
    //~ //leg->SetHeader(Form("%s / %s", util::AsString(b).c_str(), util::AsStringTeX(s,true).c_str()));
    //~ leg->AddEntry(nom_pdf     [imodel], "nominal",        "L");
    //~ leg->AddEntry(nom_pdf_werr[imodel], "a-priori error", "F");
    //~ leg->Draw();

    if (outfile && outfile->IsOpen()) {
      TString name = TString::Format("env_sb_%s_%s",
         util::AsString(b).c_str(), util::AsString(s).c_str() );
      nom_pdf[imodel]->Write((name+"_nom").Data());
      err_p1 [imodel]->Write((name+"_p1" ).Data());
      err_m1 [imodel]->Write((name+"_m1" ).Data());
    }
  }//imodel
  //~ c[ibeam]->Update();

  if (outfile) {
    outfile->Close();
    delete outfile;
  }

  std::cout <<"Done!"<<std::endl;
}

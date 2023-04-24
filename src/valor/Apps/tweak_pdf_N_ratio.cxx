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
             --mean,nominal         : Option to calculate the standard deviation from the mean value or the nominal value, default nominal

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
#include "valor/FitElements/GenericSingleSampleFitModel.h"
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

//const KineVar_t kv = conv::kvEreco;

int main(int argc, char ** argv)
{
  valor::util::PrintBanner(Env::Instance()->Banner(),500);

  CmdLnArgList arg;
  arg.Read(argc,argv,"$VALOR_FMWK/config/apps/tweak_pdf_N.arg:$VALOR_FMWK/config/apps/seeds.arg:$VALOR_FMWK/config/apps/oscparams.arg");
  LOG("valor", pNOTICE) << arg;

  // Read the command line
  int ntoys               = arg.GetInt("ntoys");
  int n_threads           = arg.GetInt("nthreads");
  string hypname          = arg.GetString("physics-hypothesis");
  string fplopt           = arg.GetString("fit-param-opt");
  string output_rootfile  = arg.GetString("output");
  string params_to_tweak  = arg.GetString("params");
  string kine_var         = arg.GetString("kinevar");
  bool mean               = arg.GetBool("mean");
  string detector         = arg.GetString("detector");

  // Should we check the detector in the config?
  bool checkDetector = false;
  Detector_t d = kdUndefined;
  if(detector != "none" && detector != "-"){
    checkDetector = true;
    d = util::DetectorFromString(detector);
  }

  // Get the kinematic variable we're tweaking
  const KineVar_t kv = util::KineVarFromString(kine_var);

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

  double mixingmumu = arg.GetDouble("input-sinsq2thMuMu");
  double mixingmue  = arg.GetDouble("input-sinsq2thMuE");
  double mixingee   = arg.GetDouble("input-sinsq2thEE");
  double dm41sq     = arg.GetDouble("input-dm41sq");
  bool   fixmixmumu = arg.GetBool("fix-sinsq2thMuMu");
  bool   fixmixmue  = arg.GetBool("fix-sinsq2thMuE");
  bool   fixmixee   = arg.GetBool("fix-sinsq2thEE");
  bool   fixdm41    = arg.GetBool("fix-dm41sq");
  double fixatmixmumu = arg.GetDouble("fix-at-sinsq2thMuMu");
  double fixatmixmue  = arg.GetDouble("fix-at-sinsq2thMuE");
  double fixatmixee   = arg.GetDouble("fix-at-sinsq2thEE");
  double fixatdm41    = arg.GetDouble("fix-at-dm41sq");
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
  //vector<TH1D *> sig_pdf     ;
  //vector<TH1D *> err_p1_sig      ;
  //vector<TH1D *> err_m1_sig      ;
  //vector<TH1D *> bkg_pdf     ;
  //vector<TH1D *> err_p1_bkg      ;
  //vector<TH1D *> err_m1_bkg      ;
  vector<TH1D *> ratio_pdf     ;
  vector<TH1D *> err_p1_ratio      ;
  vector<TH1D *> err_m1_ratio      ;

  vector<double> max(cm->GetNSubModels());
  vector<vector<TH1D *> > twk_pdf(cm->GetNSubModels());
  CorrelatedSystematics * syst = cm->GetSystematics();

  vector<vector<TH1D *> > nom_contribs     ;
//  vector<vector<vector<TH1D *> > > twk_contribs;
  vector<vector<vector<TH1D *> > > twk_contribs(cm->GetNSubModels());
  
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
//  std::vector<TObjArray*> vec_array;
  // Generate pdfs
  for(int itoy = -1; itoy < ntoys; itoy++) {
    cout << "** Toy-MC: #" << itoy << endl;

    // (toy-MC #-1 is special: default)
    FitParamList * fp = util::GetFitParameterList(hypname.c_str(), fplopt.c_str());

    if(hypname == "eff_sterile_osc"){
        if(dm41sq == -999){//if hyp set to sterile osc and no input params set(at least dm41 should be set, should include all 12 (4x (input, fix, fix-at))
          LOG("valor",pWARN) << " Not set input sterile oscillation parameters, using defaults of Dm41sq = 100, mixMuMu = 0, mixMuE = 1, mixEE = 0";
          fp->Update(conv::kPhypDm41sq,   100);
          fp->Update(conv::kPhypMixingMuMu, 0);
          fp->Update(conv::kPhypMixingMuE,  1);
          fp->Update(conv::kPhypMixingEE,   0);
        }
        else{
          if (fp->Exists(conv::kPhypDm41sq    )) {//if exists 
            fp->Update(conv::kPhypDm41sq    ,dm41sq    ); //set param to initial value
            if(fixdm41){//if to be fixed
              fp->FixParameter(conv::kPhypDm41sq);//fix param
              if(fixatdm41!=-999){fp->Update(conv::kPhypDm41sq, fixatdm41); fp->SetInitValue(conv::kPhypDm41sq, fixatdm41); }//if fix-at value not default (oscpara.arg) then set value and initval to fix-at
            }
          }
          if (fp->Exists(conv::kPhypMixingMuMu    )) { 
            fp->Update(conv::kPhypMixingMuMu    ,mixingmumu    ); 
            if(fixmixmumu){
              fp->FixParameter(conv::kPhypMixingMuMu);
              if(fixatmixmumu!=-999){fp->Update(conv::kPhypMixingMuMu, fixatmixmumu); fp->SetInitValue(conv::kPhypMixingMuMu, fixatmixmumu); }
            }
          }
          if (fp->Exists(conv::kPhypMixingMuE    )) { 
            fp->Update(conv::kPhypMixingMuE    ,mixingmue    ); 
            if(fixmixmue){
              fp->FixParameter(conv::kPhypMixingMuE);
              if(fixatmixmue!=-999){fp->Update(conv::kPhypMixingMuE, fixatmixmue); fp->SetInitValue(conv::kPhypMixingMuE, fixatmixmue); }
            }
          }
          if (fp->Exists(conv::kPhypMixingEE    )) { 
            fp->Update(conv::kPhypMixingEE    ,mixingee    ); 
            if(fixmixee){
              fp->FixParameter(conv::kPhypMixingEE);
              if(fixatmixee!=-999){fp->Update(conv::kPhypMixingEE, fixatmixee); fp->SetInitValue(conv::kPhypMixingEE, fixatmixee); }
            }
          }
        }
    }
    else{

      if (fp->Exists(conv::kPhypMixing12)) { fp->Update(conv::kPhypMixing12,mixing12); }
      if (fp->Exists(conv::kPhypMixing13)) { fp->Update(conv::kPhypMixing13,mixing13); }
      if (fp->Exists(conv::kPhypMixing23)) { fp->Update(conv::kPhypMixing23,mixing23); }
      if (fp->Exists(conv::kPhypDm12sq  )) { fp->Update(conv::kPhypDm12sq  ,dm12sq  ); }
      if (fp->Exists(conv::kPhypDm23sq  )) { fp->Update(conv::kPhypDm23sq  ,dm23sq  ); }
      if (fp->Exists(conv::kPhypCPV     )) { fp->Update(conv::kPhypCPV     ,deltacp ); }
    }

    model->ResetSystematics(*fp, "all");
    
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
      model->RandomizeSystematics(param_map, *fp);
    }
    fp->FromPhysParamList(model->Tweaks(),"current");

    // Generate predictions
    model->Generate(*fp,"+SB");

    // Array to store results
    TObjArray * array = new TObjArray();
    array->SetOwner(true);

    // Array to store results
    TObjArray * array1 = new TObjArray();
    array1->SetOwner(true);
    // Write stuff out
    model->Write(array,"curr","pdfts_rf+SB");
    model->Write(array1,"curr","pdfcs_rf+SB");
    
    // Pick results wanted
    for(unsigned int imodel = 0; imodel < cm->GetNSubModels(); imodel++) {
      //std::cout<<" cm->GetNSubModels() = "<< cm->GetNSubModels()<<std::endl;
      //SingleSampleFitModelABC * this_model = dynamic_cast<SingleSampleFitModelABC*>(cm->GetSubModel(imodel));
      GenericSingleSampleFitModel * this_model = dynamic_cast<GenericSingleSampleFitModel*>(cm->GetSubModel(imodel));
      BeamConf_t b = this_model->GetBeam();
      Sample_t   s = this_model->GetSample();
      TString name = TString::Format("curr_pdfts_sb_rf_%s_%s",
         this_model->Name().c_str(),util::AsString(kv).c_str());
      TH1D * curr_pdf = (TH1D*) array->FindObject(name.Data());
      if (!curr_pdf) {
        cerr<<"Could not find histogram "<<name<<" 1st"<<endl;
        exit(-1);
      }
      /*if(itoy < 0) {
         nom_pdf_werr.push_back(new TH1D(*curr_pdf));
         nom_pdf     .push_back(new TH1D(*curr_pdf));
      } else {
         twk_pdf[imodel].push_back(new TH1D(*curr_pdf));
      }*/
      //std::cout<<"Start of reac mode loop"<<std::endl;
      vector<TH1D *> contribs;
      for (conv::ReacMode_t ireac : this_model->GetReactionModes()) {
        TString name_contrib = TString::Format("curr_pdfcs_sb_rf_%s_%s_%s", this_model->Name().c_str(), util::AsString(kv).c_str(), util::AsString(ireac).c_str());
        TH1D * curr_contrib = (TH1D*) array1->FindObject(name_contrib.Data());
        if (!curr_contrib) {
          cerr<<"Could not find histogram "<<name_contrib<<" 1st*****"<<endl;
          exit(-1);
        }
        contribs.push_back(new TH1D(*curr_contrib));
      }//ireac
      //std::cout<<"size of contribs = "<<contribs.size()<<std::endl;
      //std::cout<<"End of reac mode loop"<<std::endl;
      if(itoy<0){
         nom_pdf_werr.push_back(new TH1D(*curr_pdf));
         nom_pdf     .push_back(new TH1D(*curr_pdf));
        nom_contribs.push_back(contribs);
      } else {
         twk_pdf[imodel].push_back(new TH1D(*curr_pdf));
         twk_contribs[imodel].push_back(contribs);
      }
    } //imodel
    //vec_array.push_back(array1);
    array->Clear("C");
    delete array;
    array1->Clear("C");
    delete array1;
  }//itoy


  for(unsigned int imodel = 0; imodel < cm->GetNSubModels(); imodel++) {
    std::cout<<"Started imodel loop 1: "<< imodel<<std::endl;
    //std::cout<<" cm->GetNSubModels() = "<< cm->GetNSubModels()<<std::endl;
    err_m1.push_back(new TH1D(*nom_pdf[imodel]));
    err_p1.push_back(new TH1D(*nom_pdf[imodel]));
    err_m1.back()->Reset();
    err_p1.back()->Reset();

    //err_m1_sig.push_back(new TH1D(*nom_pdf[imodel]));
    //err_p1_sig.push_back(new TH1D(*nom_pdf[imodel]));
    //err_m1_sig.back()->Reset();
    //err_p1_sig.back()->Reset();

    //err_m1_bkg.push_back(new TH1D(*nom_pdf[imodel]));
    //err_p1_bkg.push_back(new TH1D(*nom_pdf[imodel]));
    //err_m1_bkg.back()->Reset();
    //err_p1_bkg.back()->Reset();

    err_m1_ratio.push_back(new TH1D(*nom_pdf[imodel]));
    err_p1_ratio.push_back(new TH1D(*nom_pdf[imodel]));
    err_m1_ratio.back()->Reset();
    err_p1_ratio.back()->Reset();
    
    //sig_pdf.push_back(new TH1D(*nom_pdf[imodel]));
    //bkg_pdf.push_back(new TH1D(*nom_pdf[imodel]));
    //sig_pdf.back()->Reset();
    //bkg_pdf.back()->Reset();
    ratio_pdf.push_back(new TH1D(*nom_pdf[imodel]));
    ratio_pdf.back()->Reset();
      
    //std::cout<<"Done push_back"<<std::endl;
    GenericSingleSampleFitModel * this_model = dynamic_cast<GenericSingleSampleFitModel*>(cm->GetSubModel(imodel));
    for(int ebin = 1; ebin <= nom_pdf[imodel]->GetNbinsX(); ebin++) {
       double nom_val=0.;
       double nom_val_sig=0.;
       double nom_val_bkg=0.;
       double nom_ratio = 0.;
       double curr_ratio = 0.;
       int icount = -1;
       for (conv::ReacMode_t m : this_model->GetReactionModes()){ 
       //for (int m = 0; m < nom_contribs[imodel].size(); m++){ 
         icount++;
         //std::cout<<"start reac mode "<<icount<<" mode = "<<m<<std::endl;
    //     TString name_contrib = TString::Format("curr_pdfcs_sb_rf_%s_%s_%s", this_model->Name().c_str(), util::AsString(kv).c_str(), util::AsString(m).c_str());
    //     TH1D * curr_pdf_contrib = (TH1D*) vec_array[0]->FindObject(name_contrib.Data());
    //     if (!curr_pdf_contrib) {
     //      cerr<<"Could not find histogram "<<name_contrib<<" 2nd"<<endl;
      //     exit(-1);
        // }
         TH1D * curr_pdf_contrib = new TH1D(*nom_contribs[imodel][icount]);
	 nom_val += curr_pdf_contrib->GetBinContent(ebin);
         //std::cout<<"got bin value"<<std::endl;
	 if (util::IsNuE(m) && util::IsCC(m)){//if signal
	   nom_val_sig += curr_pdf_contrib->GetBinContent(ebin);
         } 
         else {
	   nom_val_bkg += curr_pdf_contrib->GetBinContent(ebin);
	 }
       } //mode
       //std::cout<<"done nom"<<std::endl;
//       double nom_val   = nom_pdf[imodel]->GetBinContent(ebin);
       double one_sigma = 1.;
      // double one_sigma_sig = 1.;
      // double one_sigma_bkg = 1.;
       double one_sigma_ratio = 1.;
       double sumsq     = 0;
      // double sumsqsig     = 0;
      // double sumsqbkg     = 0;
       double sumsqratio     = 0;
       double mean_val      = 0.;

       if(mean){
         for(int itoy = 0; itoy < ntoys; itoy++) {
           mean_val += twk_pdf[imodel][itoy]->GetBinContent(ebin);
         }
       }
       mean = mean/static_cast<double>(ntoys);
       for(int itoy = 0; itoy < ntoys; itoy++) {
          //std::cout<<"itoy = "<<itoy<<std::endl;
	  double curr_val=0.;
	  double curr_val_sig=0.;
	  double curr_val_bkg=0.;
          int jcount = -1;
          for (conv::ReacMode_t m : this_model->GetReactionModes()) {
            jcount++;
     //       TString name_contrib = TString::Format("curr_pdfcs_sb_rf_%s_%s_%s", this_model->Name().c_str(), util::AsString(kv).c_str(), util::AsString(m).c_str());
     //       TH1D * curr_pdf_contrib = (TH1D*) vec_array[itoy+1]->FindObject(name_contrib.Data());
     //       if (!curr_pdf_contrib) {
     //         cerr<<"Could not find histogram "<<name_contrib<<" 3rd"<<endl;
     //         exit(-1);
     //       }
            TH1D * curr_pdf_contrib = new TH1D(*twk_contribs[imodel][itoy][jcount]);
            curr_val += curr_pdf_contrib->GetBinContent(ebin);
            if (util::IsNuE(m) && util::IsCC(m)){//if signal
              curr_val_sig += curr_pdf_contrib->GetBinContent(ebin);
            }
            else {
              curr_val_bkg += curr_pdf_contrib->GetBinContent(ebin);
            }
	  }
	  nom_ratio = nom_val_sig/nom_val_bkg;
	  curr_ratio = curr_val_sig/curr_val_bkg;
          //double curr_val = twk_pdf[imodel][itoy]->GetBinContent(ebin);
          if(mean){
            if(mean_val<std::numeric_limits<double>::epsilon()){
              LOG("valor",pERROR) << " Trying to divide by a mean value of 0, reverting to nominal";
              sumsq += TMath::Power(curr_val-nom_val,2.);
              LOG("valor",pINFO) << " Calculating the standard deviation using the nominal";
            }
            else{
              sumsq += TMath::Power(curr_val-mean_val,2.);
              LOG("valor",pINFO) << " Calculating the standard deviation using the mean";
            }
          }
          else{
            sumsq += TMath::Power(curr_val-nom_val,2.);
            //sumsqsig += TMath::Power(curr_val_sig-nom_val_sig,2.);
            //sumsqbkg += TMath::Power(curr_val_bkg-nom_val_bkg,2.);
            sumsqratio += TMath::Power(curr_ratio-nom_ratio,2.);
            LOG("valor",pINFO) << " Calculating the standard deviation using the nominal";
          }
       }//itoy
       //sig_pdf[imodel]->SetBinContent(ebin, nom_val_sig);
       //bkg_pdf[imodel]->SetBinContent(ebin, nom_val_bkg);
       ratio_pdf[imodel]->SetBinContent(ebin, nom_ratio);
       
       one_sigma = TMath::Sqrt(sumsq/static_cast<double>(ntoys));
       //one_sigma_sig = TMath::Sqrt(sumsqsig/static_cast<double>(ntoys));
       //one_sigma_bkg = TMath::Sqrt(sumsqbkg/static_cast<double>(ntoys));
       one_sigma_ratio = TMath::Sqrt(sumsqratio/static_cast<double>(ntoys));
       
       err_p1[imodel]->SetBinContent(ebin, nom_val + one_sigma);
       err_m1[imodel]->SetBinContent(ebin, nom_val - one_sigma);
       nom_pdf_werr [imodel] -> SetBinError(ebin,one_sigma);
       if( nom_pdf[imodel]->GetBinCenter(ebin) < enu_reco_max ) {
          max [imodel] = TMath::Max(max[imodel], nom_val + one_sigma);
       }
       //err_p1_sig[imodel]->SetBinContent(ebin, nom_val_sig + one_sigma_sig);
       //err_m1_sig[imodel]->SetBinContent(ebin, nom_val_sig - one_sigma_sig);
       //err_p1_bkg[imodel]->SetBinContent(ebin, nom_val_bkg + one_sigma_bkg);
       //err_m1_bkg[imodel]->SetBinContent(ebin, nom_val_bkg - one_sigma_bkg);
       err_p1_ratio[imodel]->SetBinContent(ebin, nom_ratio + one_sigma_ratio);
       err_m1_ratio[imodel]->SetBinContent(ebin, nom_ratio - one_sigma_ratio);
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
    if(checkDetector && d != this_model->GetDetector()) continue;
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
      //sig_pdf[imodel]->Write((name+"_sig_nom").Data());
      //err_p1_sig [imodel]->Write((name+"_sig_p1" ).Data());
      //err_m1_sig [imodel]->Write((name+"_sig_m1" ).Data());
      //bkg_pdf[imodel]->Write((name+"_bkg_nom").Data());
      //err_p1_bkg [imodel]->Write((name+"_bkg_p1" ).Data());
      //err_m1_bkg [imodel]->Write((name+"_bkg_m1" ).Data());
      ratio_pdf[imodel]->Write((name+"_ratio_nom").Data());
      err_p1_ratio [imodel]->Write((name+"_ratio_p1" ).Data());
      err_m1_ratio [imodel]->Write((name+"_ratio_m1" ).Data());
    }
  }//imodel
  //~ c[ibeam]->Update();

  if (outfile) {

    outfile->Close();
    delete outfile;
  }
  std::cout <<"Done!"<<std::endl;
}

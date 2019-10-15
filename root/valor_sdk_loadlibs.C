Long_t valor_load_lib(const char * lib, bool warn_on_fail = true, bool verbose = false)
{
  if (verbose) { std::cerr<<lib<<std::endl; }
  Long_t result = gSystem->Load(lib);
  if (result && warn_on_fail) {std::cerr<<"Failed to load: "<<lib<<std::endl;}
  return result;
}

int valor_sdk_loadlibs ()
{
  TString vf_libs0 = gSystem->GetDynamicPath();
  TString vf_libs  = vf_libs0 + ":/usr/lib:/usr/local/lib:/opt/lib:/opt/local/lib:/usr/lib64:$VALOR_FMWK/lib";
  gSystem->SetDynamicPath(vf_libs.Data());
  
  TString features(gROOT->GetConfigFeatures());

  // extra ROOT libs
  valor_load_lib("libGpad"     , false);
  valor_load_lib("libPhysics"  , false);
  valor_load_lib("libMatrix"   , false);
  valor_load_lib("libHist"     , false);
  valor_load_lib("libGraf"     , false);
  valor_load_lib("libTree"     , false);
  valor_load_lib("libRIO"      , false);
  valor_load_lib("libXMLIO"    , false);
  valor_load_lib("libMinuit"   , false);
  valor_load_lib("libMinuit2"  , false);
  valor_load_lib("libMathMore"); 

  // log4cpp lib
  valor_load_lib("liblog4cpp", false);

  // VALOR libs
  Long_t valor_lib_status = 0;
  valor_lib_status = valor_lib_status || valor_load_lib("libValorMesg.so");
  valor_lib_status = valor_lib_status || valor_load_lib("libValorRegistry.so");
  valor_lib_status = valor_lib_status || valor_load_lib("libValorConv.so");
  valor_lib_status = valor_lib_status || valor_load_lib("libValorArray.so");
  valor_lib_status = valor_lib_status || valor_load_lib("libValorFitCore.so");
  valor_lib_status = valor_lib_status || valor_load_lib("libValorNtuple.so");
  valor_lib_status = valor_lib_status || valor_load_lib("libValorOsc.so");
  valor_lib_status = valor_lib_status || valor_load_lib("libValorCmdLine.so");
  valor_lib_status = valor_lib_status || valor_load_lib("libValorUtil.so");
  valor_lib_status = valor_lib_status || valor_load_lib("libValorKBin.so");
  valor_lib_status = valor_lib_status || valor_load_lib("libValorFitElements.so");
  
  if (valor_lib_status) {
    std::cerr<<"Did not successfully load all libraries"<<std::endl;
    exit(-1);
  }
  
  return 0;
}


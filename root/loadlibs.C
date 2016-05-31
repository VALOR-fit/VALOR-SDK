void valor_load_lib(const char * lib)
{
  //~ std::cerr<<lib<<":\t"<<
  gSystem->Load(lib);
  //~ std::cerr<<std::endl
}

void loadlibs ()
{
  TString vf_libs0 = gSystem->GetDynamicPath();
  TString vf_libs  = vf_libs0 + ":/usr/lib:/usr/local/lib:/opt/lib:/opt/local/lib:/usr/lib64";
  gSystem->SetDynamicPath(vf_libs.Data());
  
  TString features(gROOT->GetConfigFeatures());

  // extra ROOT libs
  valor_load_lib("libGpad");
  valor_load_lib("libPhysics");
  valor_load_lib("libMatrix");
  valor_load_lib("libHist");
  valor_load_lib("libGraf");
  valor_load_lib("libTree");
  valor_load_lib("libRIO");
  valor_load_lib("libXMLIO");
  valor_load_lib("libMinuit");
  valor_load_lib("libMinuit2");
  if (features.Contains("mathmore")) {valor_load_lib("libMathMore"); }

  // log4cpp lib
  valor_load_lib("liblog4cpp");

  // VALOR libs
  valor_load_lib("libValorMesg.so");
  valor_load_lib("libValorRegistry.so");
  valor_load_lib("libValorConv.so");
  valor_load_lib("libValorArray.so");
  valor_load_lib("libValorFitCore.so");
  valor_load_lib("libValorNtuple.so");
  valor_load_lib("libValorOsc.so");
  valor_load_lib("libValorUtil.so");
  valor_load_lib("libValorCmdLine.so");
}


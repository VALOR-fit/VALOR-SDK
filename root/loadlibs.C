{
  TString libs0 = gSystem->GetDynamicPath();
  TString libs  = libs0 + ":/usr/lib:/usr/local/lib:/opt/lib:/opt/local/lib:/usr/lib64";
  gSystem->SetDynamicPath(libs.Data());

  // extra ROOT libs
  gSystem->Load("libGpad");
  gSystem->Load("libPhysics");
  gSystem->Load("libMatrix");
  gSystem->Load("libHist");
  gSystem->Load("libGraf");
  gSystem->Load("libTree");
  gSystem->Load("libRIO");
  gSystem->Load("libXMLIO");
  gSystem->Load("libMinuit");
  gSystem->Load("libMinuit2");
  gSystem->Load("libMathMore"); 

  // log4cpp lib
  gSystem->Load("liblog4cpp");

  // VALOR libs
  gSystem->Load("libValorMesg.so");
  gSystem->Load("libValorRegistry.so");
  gSystem->Load("libValorConv.so");
  gSystem->Load("libValorArray.so");
  gSystem->Load("libValorFitCore.so");
  gSystem->Load("libValorNtuple.so");
  gSystem->Load("libValorOsc.so");
  gSystem->Load("libValorUtil.so");
  gSystem->Load("libValorCmdLine.so");
}


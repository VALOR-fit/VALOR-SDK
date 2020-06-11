// 
// This macro reads in the MINOS data and translates it from
// sinsqtheta to sinsq2theta
//
// April 2020
// R.J.
//

{

  TFile  * f = new TFile("./sterile_3p1_limits.root","READ");
  const char * name = "lim_numudis_3p1_minosp_90pctCL";
  TGraph * g = static_cast<TGraph *>(f->Get(name));
  f->Close();

  // Get the x-axis bin edges and redefine
  const int nPoints  = g->GetN();
  double * xPointsArr = g->GetX();
  double * yPointsArr = g->GetY();
  std::vector<double> xPoints(xPointsArr, xPointsArr+nPoints);
  std::vector<double> yPoints(yPointsArr, yPointsArr+nPoints);
  std::vector<double> newXPoints;

  if(nPoints != xPoints.size() || nPoints != yPoints.size()){
    std::cerr << " Error: Number of points don't match: " << std::endl;
    std::cerr << " N Points: " << nPoints << ", X Points: " << xPoints.size() << ", Y Points: " << yPoints << std::endl;
    exit(1);
  }
  for(int i = 0; i < nPoints; ++i){
    // Get point on the x-axis
    double sinsqtheta  = xPoints.at(i);
    double sinsq2theta = 4 * sinsqtheta * (1-sinsqtheta);
    newXPoints.push_back(sinsq2theta);
  }

  // Fill the new TGraph and write to new file
  TVectorD xPointsTVD(nPoints,newXPoints.data());
  TVectorD yPointsTVD(nPoints,yPoints.data());
  TGraph *gNew = new TGraph(xPointsTVD, yPointsTVD);
  std::string newName(name);
  newName += "_sinsq2theta_corrected";
  gNew->SetName(newName.c_str());

  // New file to save the TGraph to
  TFile fNew("./sterile_3p1_limits.root","UPDATE");
  gNew->Write();
  fNew.Close();

}

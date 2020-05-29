//
//
//
//
// Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>

void test_cp_asymmetry
(
  double Emin         =    0.0,       /* GeV     */
  double Emax         =    3.0,       /* GeV     */
  double L            =  300.0,       /* km      */
  double deltaCP_deg  =  -45.0,       /* degrees */
  double sinsqtheta12 =    0.306,
  double sinsqtheta13 =    0.025,
  double sinsqtheta23 =    0.500,
  double dmsq12       =    7.5E-5,
  double dmsq23       =    2.5E-3 
)
{
  using namespace valor;

  const int nbins = 10000;

  double deltaCP = (deltaCP_deg/180.) * TMath::Pi();

  valor::NuOsc3Flavour osc;
  osc.SetOscParametersSinSqTAll (sinsqtheta12, sinsqtheta13, sinsqtheta23, dmsq12, dmsq23, deltaCP);
  osc.SetMassHierarchy(valor::conv::kNmhNormal);
  //osc.SetMassHierarchy(valor::conv::kNmhInverted);
  osc.SetBaseline(L);

  TH1D * hAcp_mue   = new TH1D("hAcp_mue",  "", nbins, Emin, Emax);
  TH1D * hAcp_mutau = new TH1D("hAcp_mutau","", nbins, Emin, Emax);

  for(int ibin = 1; ibin <= nbins; ibin++) {

     double E = hAcp_mue->GetXaxis()->GetBinCenter(ibin);

     double p_numu_nue         = osc.CalcMatterOscProb(valor::conv::kPdgNuMu,    valor::conv::kPdgNuE,      E);     
     double p_numu_nutau       = osc.CalcMatterOscProb(valor::conv::kPdgNuMu,    valor::conv::kPdgNuTau,    E);     
     double p_numubar_nuebar   = osc.CalcMatterOscProb(valor::conv::kPdgNuMuBar, valor::conv::kPdgNuEBar,   E);     
     double p_numubar_nutaubar = osc.CalcMatterOscProb(valor::conv::kPdgNuMuBar, valor::conv::kPdgNuTauBar, E);     

/*
     cout << "E/GeV = " << E 
          << ", P(numu->nue) = " << p_numu_nue
          << ", P(numubar->nuebar) = " << p_numubar_nuebar
          << ", P(numu->nutau) = " << p_numu_nutau
          << ", P(numubar->nutaubar) = " << p_numubar_nutaubar
          << endl;
*/

     double Acp_mue   = (p_numu_nue   - p_numubar_nuebar  ) / (p_numu_nue   + p_numubar_nuebar  );
     double Acp_mutau = (p_numu_nutau - p_numubar_nutaubar) / (p_numu_nutau + p_numubar_nutaubar);

     hAcp_mue   -> SetBinContent (ibin, Acp_mue);
     hAcp_mutau -> SetBinContent (ibin, Acp_mutau);
  }

  TCanvas * c = new TCanvas();

  hAcp_mue   -> SetLineColor(kBlue);
  hAcp_mutau -> SetLineColor(kRed);

  hAcp_mue   -> Draw();
  hAcp_mutau -> Draw("same");

  hAcp_mue->GetXaxis()->SetTitle("E_{#nu} (GeV)");
  hAcp_mue->GetYaxis()->SetTitle("CP asymmetry");

  TLegend * leg = new TLegend(0.5, 0.7, 0.9, 0.9);
  leg->SetFillColor(0); 
  leg->SetBorderSize(0); 
  leg->SetHeader( Form("L = %.1f km, NH, #delta_{CP} = %.1f^{o}", L, deltaCP_deg) ); 
  leg->AddEntry(hAcp_mue,   "#mu #rightarrow e", "L");
  leg->AddEntry(hAcp_mutau, "#mu #rightarrow #tau", "L");
  leg->Draw();

  c->Update();

  c->SaveAs("Acp.png");
}

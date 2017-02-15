//
// Constants
//

const double pi         = 3.14159265; 
const double pmin       = 0.02; 
const double pmax       = 0.09; 
const double diag_scale = 0.6; 

const int npoints_m = 4; 
const double deltaCP_value       [npoints_m] = { 0.0              , 0.5*pi               , pi                 , 1.5*pi                 };
const int    deltaCP_marker_style[npoints_m] = { 24               , 26                   , 20                 , 22                     };
const int    deltaCP_marker_size [npoints_m] = { 2                , 2                    , 2                  , 2                      };
const char*  deltaCP_legend_text [npoints_m] = { "#delta_{CP} = 0", "#delta_{CP} = #pi/2", "#delta_{CP} = #pi", "#delta_{CP} = 3#pi/2" };

const int kWideDashed = 10;
//
// Function prototypes
//

void FormatGraphLong (TGraph *graph, int colour, int width, int style);
void FormatGraph     (TGraph *graph, int colour, int width, int style);

TGraph * make_loop(
  bool   useMatterProb,
  double energy,  
  double sinsqtheta12,  
  double sinsqtheta13,  
  double sinsqtheta23,
  double dmsq12, 
  double modDmsq32
);

TMarker * make_point(
  bool   useMatterProb,
  double energy,  
  double sinsqtheta12,  
  double sinsqtheta13,  
  double sinsqtheta23,
  double dmsq12, 
  double modDmsq32,
  double dCP,
//  double sinsqtheta13Error,
//  double sinsqtheta23PlusError,
//  double sinsqtheta23MinusError,
//  double modDmsq32PlusError,
//  double modDmsq32MinusError,
  int marker_style,
  int marker_size
);

valor::NuOsc3Flavour * nuOsc = new valor::NuOsc3Flavour;


void make_biprobability_plot (
 bool   showNH                 = true,
 bool   showIH                 = true,
 bool   showMirrorTheta23      = false,
 bool   showDiagonal           = true,
 bool   showLegend             = true,
 bool   showEnergyAndBaseline  = true,
 bool   showOscillationParams  = true,
 bool   useMatterProb          = true,
 double energy                 = 0.6, // GeV
 double baseline               = 295, // km
 double sinsqtheta12           = 0.306,
 double sinsqtheta13           = 0.025,
 double sinsqtheta23           = 0.55,
 double dmsq12                 = 7.5e-5,
 double modDmsq32              = 2.5e-3
// double sinsqtheta13Error      = 0.025,    //current value 0.02, Daya Bay + Reno 0.01, T2K LOI 0.006
// double sinsqtheta23PlusError  = 0.060,    //current value 0.06 (since best fit is 1.0), T2K LOI 0.01
// double sinsqtheta23MinusError = 0.060,    //current value 0.06, T2K LOI 0.01 
// double modDmsq32PlusError     = 0.12e-3,  //current value 0.12e-3, T2K LOI 0.1e-3
// double modDmsq32MinusError    = 0.08e-3,  //current value 0.08e-3, T2K LOI 0.1e-3 
)
{
  using namespace std;
  using namespace valor;
  using namespace valor::conv;

  // Global options
  Opt::Instance()->gOscMixingParamChoice = kNuMixSinsqTheta;
  nuOsc->SetBaseline(baseline);

  double sinsqtheta23_mirror = 0.5;
  if( TMath::Abs(sinsqtheta23-0.5) < 0.001) {
     showMirrorTheta23 = false;
  } else {
     if(showMirrorTheta23) {
        double dsinsqtheta23 = TMath::Abs(sinsqtheta23-0.5);
        if(sinsqtheta23 < 0.5) sinsqtheta23_mirror = 0.5 + dsinsqtheta23;
        if(sinsqtheta23 > 0.5) sinsqtheta23_mirror = 0.5 - dsinsqtheta23;
     }
  }

  // 
  // Calculate P(numu->nue) and P(numubar->nuebar) for different values of deltaCP 
  // Normal hierarchy
  //

  nuOsc->SetMassHierarchy(kNmhNormal);

  TGraph * T2KNH_loop = 0;
  TMarker * T2KNH_points[npoints_m] = {0};
  T2KNH_loop = make_loop(
    useMatterProb, energy, sinsqtheta12, sinsqtheta13, sinsqtheta23, dmsq12, modDmsq32);
  FormatGraphLong(T2KNH_loop, kBlue, 2, kSolid);
  for(int i=0; i < npoints_m; i++) {
    T2KNH_points[i] = 
      make_point(useMatterProb, energy, 
                 sinsqtheta12, sinsqtheta13, sinsqtheta23, dmsq12, modDmsq32, deltaCP_value[i],
                 deltaCP_marker_style[i], deltaCP_marker_size[i]);
  }

  // repeat for mirror theta23, if requested

  TGraph * T2KNH_mirror23_loop = 0;
  TMarker * T2KNH_mirror23_points[npoints_m] = {0};
  if(showMirrorTheta23) {
    T2KNH_mirror23_loop = make_loop(
       useMatterProb, energy, sinsqtheta12, sinsqtheta13, sinsqtheta23_mirror, dmsq12, modDmsq32);
       FormatGraphLong(T2KNH_mirror23_loop, kBlue, 2, kWideDashed);
    for(int i=0; i < npoints_m; i++) {
      T2KNH_mirror23_points[i] = make_point(
         useMatterProb, energy, 
         sinsqtheta12, sinsqtheta13, sinsqtheta23_mirror, dmsq12, modDmsq32, deltaCP_value[i],
         deltaCP_marker_style[i], deltaCP_marker_size[i]);
    }
  }

  // 
  // Calculate P(numu->nue) and P(numubar->nuebar) for different values of deltaCP 
  // Inverted hierarchy
  //

  nuOsc->SetMassHierarchy(kNmhInverted);

  TGraph * T2KIH_loop = 
     make_loop(useMatterProb, energy, sinsqtheta12, sinsqtheta13, sinsqtheta23, dmsq12, modDmsq32);
  FormatGraph(T2KIH_loop, kRed, 2, kSolid);

  TMarker * T2KIH_points[npoints_m] = {0};
  for(int i=0; i < npoints_m; i++) {
    T2KIH_points[i] = 
      make_point(useMatterProb, energy, 
                 sinsqtheta12, sinsqtheta13, sinsqtheta23, dmsq12, modDmsq32, deltaCP_value[i],
                 deltaCP_marker_style[i], deltaCP_marker_size[i]);
  }

  // repeat for mirror theta23, if requested

  TGraph * T2KIH_mirror23_loop = 0;
  TMarker * T2KIH_mirror23_points[npoints_m] = {0};
  if(showMirrorTheta23) {
    T2KIH_mirror23_loop = make_loop(
       useMatterProb, energy, sinsqtheta12, sinsqtheta13, sinsqtheta23_mirror, dmsq12, modDmsq32);
       FormatGraphLong(T2KIH_mirror23_loop, kRed, 2, kWideDashed);
    for(int i=0; i < npoints_m; i++) {
      T2KIH_mirror23_points[i] = make_point(
         useMatterProb, energy, 
         sinsqtheta12, sinsqtheta13, sinsqtheta23_mirror, dmsq12, modDmsq32, deltaCP_value[i],
         deltaCP_marker_style[i], deltaCP_marker_size[i]);
    }
  }

  //
  // Plot
  //

  TCanvas *c = new TCanvas("c", "c", 800, 750);
  gPad->SetLeftMargin(0.2);
  TH2F * hframe = (TH2F*) c->DrawFrame(pmin,pmin,pmax,pmax);
  hframe->GetXaxis()->SetTitle("P(#nu_{#mu} #rightarrow #nu_{e})");
  hframe->GetYaxis()->SetTitle("P(#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{e})");
  hframe->GetXaxis()->SetTitleOffset(1.1);
  hframe->GetYaxis()->SetTitleOffset(1.3);
  hframe->GetXaxis()->SetLabelSize(0.04);
  hframe->GetYaxis()->SetLabelSize(0.04);
  hframe->Draw();
  if(showNH) {
    T2KNH_loop->Draw("L");
    for(int i=0; i < npoints_m; i++) {
      T2KNH_points[i]->Draw();
    }
    if(showMirrorTheta23) {
       T2KNH_mirror23_loop->Draw("L");
       for(int i=0; i < npoints_m; i++) {
         T2KNH_mirror23_points[i]->Draw();
       }
    }
  }
  if(showIH) {
    T2KIH_loop->Draw("L");
    for(int i=0; i < npoints_m; i++) {
      T2KIH_points[i]->Draw();
    }
    if(showMirrorTheta23) {
       T2KIH_mirror23_loop->Draw("L");
       for(int i=0; i < npoints_m; i++) {
         T2KIH_mirror23_points[i]->Draw();
       }
    }
  }


  TLegend *leg = new TLegend(0.70, 0.50, 0.91, 0.89);
  if(showNH) leg->AddEntry(T2KNH_loop, "NH (#Delta m^{2}_{32} > 0)", "L");
  if(showIH) leg->AddEntry(T2KIH_loop, "IH (#Delta m^{2}_{32} < 0)", "L");
  if(showMirrorTheta23) {
//     leg->AddEntry(T2KNH_loop, "NH (#Delta m^{2}_{32} > 0)", "L");
//     leg->AddEntry(T2KIH_loop, "IH (#Delta m^{2}_{32} < 0)", "L");
  }
  for(int i=0; i < npoints_m; i++) {
    leg->AddEntry(T2KNH_points[i], deltaCP_legend_text[i], "P");
  }
  leg->SetBorderSize(0);
  if(showLegend) leg->Draw();

  if(showDiagonal) {
    TLine * l = new TLine(pmin,pmin,diag_scale*pmax,diag_scale*pmax);
    l->SetLineStyle(kDashed);
    l->Draw();
  }

  TLatex * txtEL = new TLatex(1.2*pmin,0.95*pmax,Form("E = %.1f GeV, L = %.0f km",energy,baseline) );
  txtEL->SetTextSize(0.03);
  if(showEnergyAndBaseline) txtEL->Draw();

  TLatex * txtDmsq32        = new TLatex(1.2*pmin,0.90*pmax,Form("|#Delta m^{2}_{32}| = %.2e eV^{2}/c^{4}", modDmsq32) );  
  TLatex * txtTheta13       = new TLatex(1.2*pmin,0.87*pmax,Form("sin^{2}#theta_{13} = %.3f", sinsqtheta13) );
  TLatex * txtTheta23       = new TLatex(1.2*pmin,0.84*pmax,Form("sin^{2}#theta_{23} = %.3f", sinsqtheta23) );
  TLatex * txtTheta23Mirror = new TLatex(1.2*pmin,0.81*pmax,Form("sin^{2}#theta_{23} = %.3f", sinsqtheta23_mirror) );
  txtDmsq32       ->SetTextSize(0.025);
  txtTheta13      ->SetTextSize(0.025);
  txtTheta23      ->SetTextSize(0.025);
  txtTheta23Mirror->SetTextSize(0.025);
  if(showOscillationParams) txtDmsq32  -> Draw();
  if(showOscillationParams) txtTheta13 -> Draw();
  if(showOscillationParams) txtTheta23 -> Draw();
  if(showMirrorTheta23) {
     if(showOscillationParams) txtTheta23Mirror -> Draw();
     TLine * ltheta23        = new TLine(1.8*pmin,0.848*pmax,2.3*pmin,0.848*pmax);
     TLine * ltheta23mirror  = new TLine(1.8*pmin,0.818*pmax,2.3*pmin,0.818*pmax);
     ltheta23       -> SetLineStyle(kSolid);
     ltheta23mirror -> SetLineStyle(kWideDashed);
     if(showOscillationParams) ltheta23       -> Draw();
     if(showOscillationParams) ltheta23mirror -> Draw();
  }

  // Example experimental point for illustration purposes
/*
  // high prec
  //double expt_pnue        = 0.042;
  //double expt_pnuebar     = 0.056;
  //double frac_err_pnue    = 0.05;
  //double frac_err_pnuebar = 0.05;
  // low prec
  double expt_pnue        = 0.06;
  double expt_pnuebar     = 0.04;
  double frac_err_pnue    = 0.30;
  double frac_err_pnuebar = 0.44;
  TMarker * expt_point = new TMarker(expt_pnue, expt_pnuebar, 20);
  expt_point->SetMarkerColor(kMagenta);
  expt_point->SetMarkerSize(1);
  expt_point->Draw();
  TLine * dpnue    = new TLine(expt_pnue*(1-frac_err_pnue), expt_pnuebar,  expt_pnue*(1+frac_err_pnue), expt_pnuebar);
  TLine * dpnuebar = new TLine(expt_pnue, expt_pnuebar*(1-frac_err_pnuebar), expt_pnue, expt_pnuebar*(1+frac_err_pnuebar));
  dpnue->SetLineColor(kMagenta);
  dpnuebar->SetLineColor(kMagenta);
  dpnue->Draw();
  dpnuebar->Draw();
*/
  c->SaveAs("c.png");
}
//_________________________________________________________________________________
TGraph * make_loop (
  bool   useMatterProb,
  double energy,  
  double sinsqtheta12,  
  double sinsqtheta13,  
  double sinsqtheta23,
  double dmsq12, 
  double modDmsq32
)
{
  cout << "Building loop" << endl;

  const int npoints_loop = 5000;

  double pNumuToNue      [npoints_loop] = {0};
  double pNumubarToNuebar[npoints_loop] = {0};

  for(int i=0; i<npoints_loop; i++)
  {
      double deltaCP = i * 2*pi / npoints_loop;
      nuOsc->SetOscParametersDmsqAtmSol(
          sinsqtheta12, sinsqtheta13, sinsqtheta23, dmsq12, modDmsq32, deltaCP);
      if(useMatterProb) {
        pNumuToNue[i] = 
          nuOsc->CalcMatterOscProb(kPdgNuMu,    kPdgNuE,    energy);
        pNumubarToNuebar[i] = 
          nuOsc->CalcMatterOscProb(kPdgNuMuBar, kPdgNuEBar, energy);
      }
      else { 
        pNumuToNue[i] = 
          nuOsc->CalcVacuumOscProb(kPdgNuMu,    kPdgNuE,    energy);
        pNumubarToNuebar[i] = 
          nuOsc->CalcVacuumOscProb(kPdgNuMuBar, kPdgNuEBar, energy);
      }
  }

  TGraph * gr = new TGraph(npoints_loop, pNumuToNue, pNumubarToNuebar);
  return gr;
}
//_________________________________________________________________________________
TMarker * make_point (
  bool   useMatterProb,
  double energy,  
  double sinsqtheta12,  
  double sinsqtheta13,  
  double sinsqtheta23,
  double dmsq12, 
  double modDmsq32,
  double dCP,
//  double sinsqtheta13Error,
//  double sinsqtheta23PlusError,
//  double sinsqtheta23MinusError,
//  double modDmsq32PlusError,
//  double modDmsq32MinusError,
  int marker_style,
  int marker_size
)
{
  double OscProb    = 0;
  double OscProbBar = 0;

  nuOsc->SetOscParametersDmsqAtmSol(     
     sinsqtheta12, sinsqtheta13, sinsqtheta23, dmsq12, modDmsq32, dCP);
  if(useMatterProb) {
     OscProb     = nuOsc->CalcMatterOscProb(kPdgNuMu,    kPdgNuE,    energy);
     OscProbBar  = nuOsc->CalcMatterOscProb(kPdgNuMuBar, kPdgNuEBar, energy);
  }
  else {
     OscProb    = nuOsc->CalcVacuumOscProb(kPdgNuMu,    kPdgNuE,    energy);
     OscProbBar = nuOsc->CalcVacuumOscProb(kPdgNuMuBar, kPdgNuEBar, energy);
  }

/*
  nuOsc->SetOscParametersDmsqAtmSol(
    sinsqtheta12, sinsqtheta13+sinsqtheta13Error, sinsqtheta23, dmsq12, modDmsq32, dCP);
  double OscProbTwk = nuOsc->CalcMatterOscProb(kPdgNuMu, kPdgNuE, energy);
  double ErrorS13Plus = fabs(OscProbTwk - OscProb);

  nuOsc->SetOscParametersDmsqAtmSol(
    sinsqtheta12, sinsqtheta13-sinsqtheta13Error, sinsqtheta23, dmsq12, modDmsq32, dCP);
  double OscProbTwk = nuOsc->CalcMatterOscProb(kPdgNuMu, kPdgNuE, energy);
  double ErrorS13Minus = fabs(OscProbTwk - OscProb);

  nuOsc->SetOscParametersDmsqAtmSol(
    sinsqtheta12, sinsqtheta13, sinsqtheta23+sinsqtheta23PlusError, dmsq12, modDmsq32, dCP);
  double OscProbTwk = nuOsc->CalcMatterOscProb(kPdgNuMu, kPdgNuE, energy);
  double ErrorS23Plus = fabs(OscProbTwk - OscProb);

  nuOsc->SetOscParametersDmsqAtmSol(
    sinsqtheta12, sinsqtheta13, sinsqtheta23-sinsqtheta23MinusError, dmsq12, modDmsq32, dCP);
  double OscProbTwk = nuOsc->CalcMatterOscProb(kPdgNuMu, kPdgNuE, energy);
  double ErrorS23Minus = fabs(OscProbTwk - OscProb);

  nuOsc->SetOscParametersDmsqAtmSol(
     sinsqtheta12, sinsqtheta13, sinsqtheta23, dmsq12, modDmsq32+modDmsq32PlusError, dCP);
  double OscProbTwk = nuOsc->CalcMatterOscProb(kPdgNuMu, kPdgNuE, energy);
  double ErrorDmsq32Plus = fabs(OscProbTwk - OscProb);

  nuOsc->SetOscParametersDmsqAtmSol(
     sinsqtheta12, sinsqtheta13, sinsqtheta23, dmsq12, modDmsq32-modDmsq32MinusError, dCP);
  double OscProbTwk = nuOsc->CalcMatterOscProb(kPdgNuMu, kPdgNuE, energy);
  double ErrorDmsq32Minus = fabs(OscProbTwk - OscProb);
*/

  TMarker * p = new TMarker(OscProb, OscProbBar, marker_style);
  p->SetMarkerSize(marker_size);
  return p;
}
//_________________________________________________________________________________
void FormatGraphLong(TGraph *graph, int colour, int width, int style)
{
  graph->SetTitle("");
  graph->SetLineColor(colour);
  graph->SetLineWidth(width);
  graph->SetLineStyle(style);
  graph->GetXaxis()->SetLimits(0.02, 0.08);
  graph->GetYaxis()->SetRangeUser(0.020001, 0.08);
  graph->GetXaxis()->SetTitleSize(0.06);
  graph->GetYaxis()->SetTitleSize(0.06);
  graph->GetXaxis()->SetLabelSize(0.06);
  graph->GetYaxis()->SetLabelSize(0.06);
  graph->GetXaxis()->SetTitleOffset(1.15);
  graph->GetYaxis()->SetTitleOffset(1.4);
  graph->GetXaxis()->CenterTitle();
  graph->GetYaxis()->CenterTitle();
}
//_________________________________________________________________________________
void FormatGraph(TGraph *graph, int colour, int width, int style)
{
  graph->SetTitle("");
  graph->SetLineColor(colour);
  graph->SetLineWidth(width);
  graph->SetLineStyle(style);
}
//_________________________________________________________________________________

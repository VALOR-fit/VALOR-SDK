//
// -----------------------------------------------------------------------------
// Using a multi-layer perceptron (MLP) neural net (ANN)
// for a regression analysis of oscillation probabilities.
//
// Given a set of input vectors:
//   x_i = (sinsqtheta13_i, sinsqtheta23_i, dmsq23_i, deltaCP_i),
// and a set of output vectors
//   y_i = P(\nu_a -> \nu_b),
// approximate the unknown function f(x_i)=y_i using an ANN.
// In the comments below, g(x_i) denotes the approximate function.
//
// Costas Andreopoulos < costas.andreopoulos@stfc.ac.uk >
// -----------------------------------------------------------------------------
//

const int n       = 50000; // training + test data (1:1)
const int nepochs = 1000;  // training epochs

// Baseline, neutrino energy (current script works for 1 energy bin)
// and other constants
const double L  = 300; // km
const double E  = 0.6; // GeV
const double pi = TMath::Pi();

// Inition and final neutrino - Specifies which oscillation probability to test
int nu_init = valor::conv::kPdgNuMu;
int nu_fin  = valor::conv::kPdgNuE;

// Number of parameters in oscillation probability function
// Others dependencies are fixed.
const int nparams = 4;  // (sinsqtheta13, sinsqtheta23, dmsq23, deltaCP)
const char * param_label [nparams] = {
  "sin^{2}#theta_{13}","sin^{2}#theta_{23}","|#Deltam^{2}_{23}|","#delta_{CP}"};
const char * param_units [nparams] = {
  0, 0, "eV^{2}/c^{4}",0};

// In training and test data, use transformed osc parameters so
// that they all span the [0,1] range. Switch to false to use the
// actual oscillation parameter values
const bool use_transformed = true;

// Oscillation parameters and parameter ranges
const double sinsqtheta12_fixed =  0.306;
const double dmsq12_fixed       =  7.5E-5;
const double sinsqtheta13_min   =  0.017;
const double sinsqtheta23_min   =  0.3;
const double dmsq23_min         =  1.5E-3;
const double deltaCP_min        = -pi;
const double sinsqtheta13_range =  0.01;
const double sinsqtheta23_range =  0.4;
const double dmsq23_range       =  2.0E-3;
const double deltaCP_range      =  2*pi;

// For validation, produce plots of oscillation probabilities vs each
// one of the 4 oscillation parameters, fixing the values of the other
// parameters. Use the following ncases.
const int ncases = 3;
const double sinsqtheta13_vec[ncases]   =  {0.017,  0.022,  0.026};
const double sinsqtheta23_vec[ncases]  =   {0.350,  0.500,  0.650};
const double dmsq23_vec[ncases]         =  {2.0E-3, 2.5E-3, 3.0E-3};
const double deltaCP_vec[ncases]        =  {-pi/2,  0,      pi/2};
const int graph_colors[ncases] = {kMagenta, kGreen+1, kBlue};

//..............................................................................
// func prototypes
//
TGraph * FuncErr1ParamScan(
  int scan_param_id, int icase,
  valor::NuOsc3Flavour & osc, TMultiLayerPerceptron* mlp);
//..............................................................................
void mlp_regression_test_1()
{
   TRandom r;

   // Setup osc. probability calculator
   valor::NuOsc3Flavour osc;
   osc.SetMassHierarchy(valor::conv::kNmhNormal  );
// osc.SetMassHierarchy(valor::conv::kNmhInverted);
   osc.SetBaseline(L);

   //
   // Create a tree with train and test data.
   //
   // We have 4 input parameters:
   //   sinsqtheta13, sinsqtheta23, dmsq23, deltaCP
   // and 1 output value:
   //   P(numu->nue)
   //

   cout << "Generating training and test data" << endl;

   valor::conv::OscChannelId_t osch =
        valor::util::GetOscChannel(nu_init, nu_fin);

   TNtuple* t=new TNtuple("training_data","Training and testing data",
     "sinsqtheta13:sinsqtheta23:dmsq23:deltaCP:posc");

   for (int i = 0; i < n; i++)
   {
      double rnd_sinsqtheta13 = r.Rndm();
      double rnd_sinsqtheta23 = r.Rndm();
      double rnd_dmsq23       = r.Rndm();
      double rnd_deltaCP      = r.Rndm();

      double sinsqtheta13 = sinsqtheta13_min + sinsqtheta13_range * rnd_sinsqtheta13;
      double sinsqtheta23 = sinsqtheta23_min + sinsqtheta23_range * rnd_sinsqtheta23;
      double dmsq23       = dmsq23_min       + dmsq23_range       * rnd_dmsq23;
      double deltaCP      = deltaCP_min      + deltaCP_range      * rnd_deltaCP;

      osc.SetOscParametersSinSqTAll(
           sinsqtheta12_fixed, sinsqtheta13, sinsqtheta23, dmsq12_fixed, dmsq23, deltaCP);
      double posc = osc.CalcMatterOscProb(nu_init, nu_fin, E);

      cout << "#" << i
           << " : P [" << valor::util::AsString(osch) << "]"
           << " (sinsqtheta13 = " << sinsqtheta13
           << ", sinsqtheta23 = " << sinsqtheta23
           << ", dmsq23 = " << dmsq23
           << ", deltaCP = " << deltaCP
           << ") = " << posc << endl;

      if(use_transformed) {
        // Fill-in the training and test data (test using transformed osc.
        // parameter values so that they all span the [0,1] range)
        t->Fill(rnd_sinsqtheta13,rnd_sinsqtheta23,rnd_dmsq23,rnd_deltaCP,posc);
      } else {
        // Fill-in the training and test data (actual oscillation parameter values)
        t->Fill(sinsqtheta13,sinsqtheta23,dmsq23,deltaCP,posc);
      }
   }

   //
   // Create and train ANN
   //

   TMultiLayerPerceptron* mlp = new TMultiLayerPerceptron(
      "sinsqtheta13,sinsqtheta23,dmsq23,deltaCP:30:15:posc",
      t, "Entry$%2","(Entry$%2)==0");
   mlp->Train(nepochs,"text update=10");

   //
   // Analyze ANN
   //

   TMLPAnalyzer* mlpa=new TMLPAnalyzer(mlp);
   mlpa->GatherInformations();
   mlpa->CheckNetwork();
   mlpa->DrawDInputs();

   //
   // Draw statistics to show the quality of the ANN's approximation
   //

   TCanvas* c1 = new TCanvas("TruthDeviation1", "Diagnostic plots #1",20,20,500,630);
   c1->Divide(1,3);

   c1->cd(1);
   // Draw the difference between the ANN's output g(x_i),
   // for an input vector x_i, and the true value f(x_i),
   // vs. f(x_i), as TProfiles
   mlpa->DrawTruthDeviations();

   c1->cd(2);
   // draw the difference between g(x_i) and f(x_i)
   // vs. each x_i element, as TProfiles
   mlpa->DrawTruthDeviationInsOut();

   c1->cd(3);
   // draw a box plot g(x_i) - f(x_i) vs. f(x_i)
   mlpa->GetIOTree()->Draw(
       "100*(Out.Out0-True.True0)/True.True0:True.True0>>hDelta",
       "True.True0 > 0","goff");
   TH2F* hDelta=(TH2F*)gDirectory->Get("hDelta");
   hDelta->GetYaxis()->SetTitle("(output-truth)/truth (%)");
   hDelta->GetXaxis()->SetTitle("truth");
   hDelta->Draw("BOX");

   // Draw difference of g(x_i) vs f(x_i) as function of each one of
   // the x_i elements, for a number of cases for the remaining elements

   TGraph * vgr[nparams][ncases] = {0};
   for(int scan_param_id = 0; scan_param_id < nparams; scan_param_id++) {
     for(int icase = 0; icase < ncases; icase++) {
       TGraph * gr = FuncErr1ParamScan(scan_param_id, icase, osc, mlp);
       vgr[scan_param_id][icase] = gr;
     }
   }

   TCanvas* c2 = new TCanvas("TruthDeviation2", "Diagnostic plots #2");
   c2->Divide(2,2);

   for(int scan_param_id = 0; scan_param_id < nparams; scan_param_id++)
   {
     c2->cd(scan_param_id+1);
     double xmin =  9999999;
     double xmax = -9999999;
     double ymin =  9999999;
     double ymax = -9999999;
     for(int icase = 0; icase < ncases; icase++) {
       TGraph * gr = vgr[scan_param_id][icase];
       const int np = gr->GetN();
       for(int ip=0; ip<np; ip++) {
         xmin = TMath::Min(xmin, gr->GetX()[ip]);
         xmax = TMath::Max(xmax, gr->GetX()[ip]);
         ymin = TMath::Min(ymin, gr->GetY()[ip]);
         ymax = TMath::Max(ymax, gr->GetY()[ip]);
       }
       double yrange = ymax - ymin;
       ymax += 0.6*yrange;
       ymin -= 0.1*yrange;
     }
     TH1F * hframe = (TH1F*)
        c2->GetPad(scan_param_id+1)->DrawFrame(xmin,ymin,xmax,ymax);
     ostringstream yaxtitle;
     yaxtitle << "P(" << valor::util::AsString(osch)
              << "): (output - truth)/truth (%)";
     hframe->GetYaxis()->SetTitle(yaxtitle.str().c_str());
     ostringstream xaxtitle;
     xaxtitle << param_label[scan_param_id];
     if(param_units[scan_param_id]) {
        xaxtitle << " (" << param_units[scan_param_id] << ")";
      }
     hframe->GetXaxis()->SetTitle(xaxtitle.str().c_str());
     TLegend * leg = new TLegend(0.20, 0.70, 0.90, 0.90);
     leg->SetFillColor(0);
     leg->SetBorderSize(0);
     for(int icase = 0; icase < ncases; icase++) {
       ostringstream label;
       int nparams_in_label = 0;
       for(int ip = 0; ip < nparams; ip++) {
         if(ip == scan_param_id) continue;
         label << param_label[ip] << " = ";
         if      (ip == 0) label << sinsqtheta13_vec[icase];
         else if (ip == 1) label << sinsqtheta23_vec[icase];
         else if (ip == 2) label << dmsq23_vec[icase] << " " << param_units[2];
         else if (ip == 3) label << deltaCP_vec[icase];
         nparams_in_label++;
         if(nparams_in_label < nparams-1) label << ", ";
       }//nparams
       vgr[scan_param_id][icase]-> SetLineColor(graph_colors[icase]);
       vgr[scan_param_id][icase]-> Draw("l");
       leg -> AddEntry(vgr[scan_param_id][icase], label.str().c_str(), "L");
     }//icase
     leg->Draw();
   }//scan_param_id

   c1->SaveAs("c1.png");
   c2->SaveAs("c2.png");
}
//..............................................................................
TGraph * FuncErr1ParamScan(
  int scan_param_id, int icase,
  valor::NuOsc3Flavour & osc, TMultiLayerPerceptron* mlp)
{
  assert(icase >= 0 && icase < ncases);
  assert(scan_param_id >= 0 && scan_param_id < nparams);

  const int npoints = 30;
  double vx  [npoints]; // vector of scanned param values x
  double vdy [npoints]; // vector of g(x) - f(x) for each x value

  // scanned param values
  for (int i = 0; i < npoints; i++) {

      double sinsqtheta13 = (scan_param_id == 0) ?
          sinsqtheta13_min + i * sinsqtheta13_range/npoints :
          sinsqtheta13_vec [icase];
      double sinsqtheta23 = (scan_param_id == 1) ?
          sinsqtheta23_min + i * sinsqtheta23_range/npoints :
          sinsqtheta23_vec [icase];
      double dmsq23 = (scan_param_id == 2) ?
          dmsq23_min + i * dmsq23_range/npoints :
          dmsq23_vec  [icase];
      double deltaCP = (scan_param_id == 3) ?
         deltaCP_min + i * deltaCP_range/npoints :
         deltaCP_vec [icase];

      vx[i] = 0;
      if      (scan_param_id == 0) vx[i] = sinsqtheta13;
      else if (scan_param_id == 1) vx[i] = sinsqtheta23;
      else if (scan_param_id == 2) vx[i] = dmsq23;
      else if (scan_param_id == 3) vx[i] = deltaCP;

      // calculate true function
      osc.SetOscParametersSinSqTAll(
        sinsqtheta12_fixed, sinsqtheta13, sinsqtheta23, dmsq12_fixed, dmsq23, deltaCP);
      double posc = osc.CalcMatterOscProb(nu_init, nu_fin,E);

      // calculate approximate function using the ANN
      double x_mlp[nparams] = {0};
      if(use_transformed) {
        x_mlp[0] = (sinsqtheta13 - sinsqtheta13_min) / sinsqtheta13_range;
        x_mlp[1] = (sinsqtheta23 - sinsqtheta23_min) / sinsqtheta23_range;
        x_mlp[2] = (dmsq23       - dmsq23_min      ) / dmsq23_range;
        x_mlp[3] = (deltaCP      - deltaCP_min     ) / deltaCP_range;
      }
      else {
        x_mlp[0] = sinsqtheta13;
        x_mlp[1] = sinsqtheta23;
        x_mlp[2] = dmsq23;
        x_mlp[3] = deltaCP;
      }
      double posc_approx = mlp->Evaluate(0, x_mlp);

      // get the difference
      vdy [i] = (posc > 0) ? 100*(posc_approx - posc)/posc : 0;
  }//i

  // create graph: g(x) - f(x) vs x_scanned for current case
  TGraph * gr = new TGraph(npoints, vx, vdy);
  return gr;
}
//..............................................................................

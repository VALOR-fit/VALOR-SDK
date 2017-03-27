//
// playing....
//
//
// Costas Andreopoulos <costas.andreopoulos@stfc.ac.uk>
//

#include <complex>

void test_making_unitarity_triangles(void)
{
  const int nthrows = 1000;

  valor::NuOsc3Flavour osc3f;

  double sinsqtheta12_bestfit     = 0.304;
  double sinsqtheta13_bestfit     = 0.0217;
  double sinsqtheta23_bestfit     = 0.5;
  double sinsqtheta12_fracerr1sig = 0.025;
  double sinsqtheta13_fracerr1sig = 0.10;
  double sinsqtheta23_fracerr1sig = 0.20;

  double dmsq12  = 5.5E-6; // irrelevant, just use an acceptable value
  double dmsq23  = 2.0E-4; // irrelevant, just use an acceptable value 

  TCanvas * c = new TCanvas();
  c->Divide(2,3);

  TH2D * h1 = new TH2D("h1","", 100, -0.5, 0.5, 100, -0.5, 0.5);
  TH2D * h2 = new TH2D("h2","", 100, -0.5, 0.5, 100, -0.5, 0.5);
  TH2D * h3 = new TH2D("h3","", 100, -0.5, 0.5, 100, -0.5, 0.5);
  TH2D * h4 = new TH2D("h4","", 100, -0.5, 0.5, 100, -0.5, 0.5);
  TH2D * h5 = new TH2D("h5","", 100, -0.5, 0.5, 100, -0.5, 0.5);
  TH2D * h6 = new TH2D("h6","", 100, -0.5, 0.5, 100, -0.5, 0.5);

  TRandom3 rand;

  for(int i = 0; i < nthrows; i++) {

     double sinsqtheta12 = rand.Gaus(sinsqtheta12_bestfit, sinsqtheta12_bestfit*sinsqtheta12_fracerr1sig);
     double sinsqtheta13 = rand.Gaus(sinsqtheta13_bestfit, sinsqtheta13_bestfit*sinsqtheta13_fracerr1sig);
     double sinsqtheta23 = rand.Gaus(sinsqtheta23_bestfit, sinsqtheta23_bestfit*sinsqtheta23_fracerr1sig);
     double deltaCP      = rand.Gaus(-TMath::Pi()/2, TMath::Pi()/2);
   //double deltaCP      = -TMath::Pi() + 2 * TMath::Pi() * rand.Uniform();

     osc3f.SetOscParametersSinSqTAll (sinsqtheta12, sinsqtheta13, sinsqtheta23, dmsq12, dmsq23, deltaCP);

     std::complex<double> Ue1   = osc3f.GetPMNSElement(0, 0);
     std::complex<double> Ue2   = osc3f.GetPMNSElement(0, 1);
     std::complex<double> Ue3   = osc3f.GetPMNSElement(0, 2);
     std::complex<double> Umu1  = osc3f.GetPMNSElement(1, 0);
     std::complex<double> Umu2  = osc3f.GetPMNSElement(1, 1);
     std::complex<double> Umu3  = osc3f.GetPMNSElement(1, 2);
     std::complex<double> Utau1 = osc3f.GetPMNSElement(2, 0);
     std::complex<double> Utau2 = osc3f.GetPMNSElement(2, 1);
     std::complex<double> Utau3 = osc3f.GetPMNSElement(2, 2);

     std::complex<double> z1 = - Ue3  * std::conj(Umu3)  / Ue2   * std::conj(Umu2);  
     std::complex<double> z2 = - Umu1 * std::conj(Umu2)  / Ue1   * std::conj(Ue2);  
     std::complex<double> z3 = - Ue3  * std::conj(Utau3) / Ue2   * std::conj(Utau2);  
     std::complex<double> z4 = - Ue1  * std::conj(Ue3)   / Umu1  * std::conj(Umu3);  
     std::complex<double> z5 = - Umu1 * std::conj(Utau1) / Umu3  * std::conj(Utau3);  
     std::complex<double> z6 = - Ue2  * std::conj(Ue3)   / Utau2 * std::conj(Utau3);  
 
     h1->Fill(std::real(z1), std::imag(z1));
     h2->Fill(std::real(z2), std::imag(z2));
     h3->Fill(std::real(z3), std::imag(z3));
     h4->Fill(std::real(z4), std::imag(z4));
     h5->Fill(std::real(z5), std::imag(z5));
     h6->Fill(std::real(z6), std::imag(z6));
  }

  c->cd(1); h1->Draw("colz");
  c->cd(2); h2->Draw("colz");
  c->cd(3); h3->Draw("colz");
  c->cd(4); h4->Draw("colz");
  c->cd(5); h5->Draw("colz");
  c->cd(6); h6->Draw("colz");

  c->Update();
}

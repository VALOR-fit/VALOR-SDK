#include <iostream>

#include "TFile.h"
#include "TH2D.h"

#include "valor/Conventions/Constants.h"
#include "valor/Conventions/ReacMode.h"
#include "valor/Conventions/Opt.h"
#include "valor/Oscillation/NuOsc3Flavour.h"

#include "Osc3Flav.h"


// const int kPdgNuTau = 16;
// const int kPdgNuTauBar = -16;
// const int kPdgNuMu = 14;
// const int kPdgNuMuBar = -14;
// const int kPdgNuE = 12;
// const int kPdgNuEBar = -12;

// Create an instance of NuOsc3Flavour
valor::Osc3Flav otf;


std::array<double, 3> calprop(int nu_start , double energy, double cosz);


void propagate(){

    // double dcp_in =  0.;
    // double h_in   = 1.0;
    // int    v_in   = 1.0;

    // h_in = ( h_in > 0 ? 1.0 : -1.0 );

    double cosineZ, energy;
    double e_start, e_end, e_step, cz_start, cz_end, cz_step;
    int i, j ;

    //// Types of Averaging	
    int NBinsEnergy = 200;
    int Dm12ZenithNbin = 200; 

   
    // Set oscillation parameters (example values, adjust according to your needs)
    // double density       = 2.6; // in g/cm^3
    double sinsqtheta12  = 0.310;
    double sinsqtheta13  = 0.0224;
    double sinsqtheta23  = 0.558;
    double dmsq12        = 7.4e-5;
    double modDmsq32     = 2.45e-3;
    double dCP           = 0;
    // nuOsc.SetDensity(density);
    otf.SetMassHierarchy(valor::conv::kNmhNormal); // kNmhNormal for normal hierarchy, kNmhInverted for inverted hierarchy
    otf.SetOscParametersSinSqTAll(sinsqtheta12, sinsqtheta13, sinsqtheta23, dmsq12, modDmsq32, dCP); // Using sin^2 values for angles and deltaCP=0


    // // Energy Range in log space
    // double EnergyBins[NBinsEnergy+1];
    // e_start = 0.100000001;
    // e_end = 10.;
    // e_step = log10(e_end/e_start)/double(NBinsEnergy);

    // double Entry = e_start;
    // for(i=0; i<NBinsEnergy; i++ ){
    //              Entry = e_start*pow( 10.0 , double(i)*e_step );
    //              EnergyBins[i] = Entry;
    // }
    // EnergyBins[NBinsEnergy] = EnergyBins[NBinsEnergy-1]*1.001;

    // Energy Range in linear space
    double EnergyBins[NBinsEnergy+1];
    e_start = 0.100000001;
    e_end = 10.;
    e_step = ( e_end - e_start )/double(NBinsEnergy);

    double Entry = e_start;
    for(i=0; i<NBinsEnergy; i++ ){
                 EnergyBins[i] = Entry + double(i)*e_step;
    }
    EnergyBins[NBinsEnergy] = EnergyBins[NBinsEnergy-1]*1.001;


    // Zenith Angle Range
    double Dm12ZenithEdge[Dm12ZenithNbin+1];
    cz_start = -1.001;
    cz_end = 1.101;
    cz_step = ( cz_end - cz_start)/double(Dm12ZenithNbin);
   
    Dm12ZenithEdge[0]= cz_start*0.9999;
    for ( i=1; i<Dm12ZenithNbin ; i++ ){
    	Dm12ZenithEdge[i] = Dm12ZenithEdge[0] + double(i)*cz_step;
    }
    Dm12ZenithEdge[Dm12ZenithNbin] = Dm12ZenithEdge[Dm12ZenithNbin-1]*1.001;

   
    TH2D *NuEToNuE3f 	=  new TH2D("NuEToNuE3f","3 Flavor P_{#nu_{e}#rightarrow#nu_{e}}",
                    NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    TH2D *NuEToNuMu3f 	=  new TH2D("NuEToNuMu3f","3 Flavor P_{#nu_{e}#rightarrow#nu_{#mu}}",
   					NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    TH2D *NuEToNuTau3f 	=  new TH2D("NuEToNuTau3f","3 Flavor P_{#nu_{e}#rightarrow#nu_{#tau}}",
   					NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    TH2D *NuEToNuX3f 	=  new TH2D("NuEToNuX3f","3 Flavor P_{#nu_{e}#rightarrow#nu_{x}}",
   					NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
   

   
    TH2D *NuMuToNuE3f 	=  new TH2D("NuMuToNuE3f","3 Flavor P_{#nu_{#mu}#rightarrow#nu_{e}}",
   					NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    TH2D *NuMuToNuMu3f 	=  new TH2D("NuMuToNuMu3f","3 Flavor P_{#nu_{#mu}#rightarrow#nu_{#mu}}",
   					NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    TH2D *NuMuToNuTau3f 	=  new TH2D("NuMuToNuTau3f","3 Flavor P_{#nu_{#mu}#rightarrow#nu_{#tau}}",
   					NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    TH2D *NuMuToNuX3f 	=  new TH2D("NuMuToNuX3f","3 Flavor P_{#nu_{#mu}#rightarrow#nu_{x}}",
   					NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);

   
    TH2D *NuTauToNuE3f 	=  new TH2D("NuTauToNuE3f","3 Flavor P_{#nu_{#tau}#rightarrow#nu_{e}}",
   					NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    TH2D *NuTauToNuMu3f 	=  new TH2D("NuTauToNuMu3f","3 Flavor P_{#nu_{#tau}#rightarrow#nu_{#mu}}",
   					NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    TH2D *NuTauToNuTau3f 	=  new TH2D("NuTauToNuTau3f","3 Flavor P_{#nu_{#tau}#rightarrow#nu_{#tau}}",
   					NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    TH2D *NuTauToNuX3f 	=  new TH2D("NuTauToNuX3f","3 Flavor P_{#nu_{#tau}#rightarrow#nu_{x}}",
   					NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);


    // anti-neutrino titles
    // TH2D *NuEToNuE3f 	=  new TH2D("NuEToNuE3f","3 Flavor P_{#bar{#nu}_{e}#rightarrow#bar{#nu}_{e}}",
    //                 NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    // TH2D *NuEToNuMu3f 	=  new TH2D("NuEToNuMu3f","3 Flavor P_{#bar#nu_{e}#rightarrow#bar#nu_{#mu}}",
   	// 				NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    // TH2D *NuEToNuTau3f 	=  new TH2D("NuEToNuTau3f","3 Flavor P_{#bar#nu_{e}#rightarrow#bar#nu_{#tau}}",
   	// 				NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    // TH2D *NuEToNuX3f 	=  new TH2D("NuEToNuX3f","3 Flavor P_{#bar#nu_{e}#rightarrow#bar#nu_{x}}",
   	// 				NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
   

   
    // TH2D *NuMuToNuE3f 	=  new TH2D("NuMuToNuE3f","3 Flavor P_{#bar#nu_{#mu}#rightarrow#bar#nu_{e}}",
   	// 				NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    // TH2D *NuMuToNuMu3f 	=  new TH2D("NuMuToNuMu3f","3 Flavor P_{#bar#nu_{#mu}#rightarrow#bar#nu_{#mu}}",
   	// 				NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    // TH2D *NuMuToNuTau3f 	=  new TH2D("NuMuToNuTau3f","3 Flavor P_{#bar#nu_{#mu}#rightarrow#bar#nu_{#tau}}",
   	// 				NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    // TH2D *NuMuToNuX3f 	=  new TH2D("NuMuToNuX3f","3 Flavor P_{#bar#nu_{#mu}#rightarrow#bar#nu_{x}}",
   	// 				NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);

   
    // TH2D *NuTauToNuE3f 	=  new TH2D("NuTauToNuE3f","3 Flavor P_{#bar#nu_{#tau}#rightarrow#bar#nu_{e}}",
   	// 				NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    // TH2D *NuTauToNuMu3f 	=  new TH2D("NuTauToNuMu3f","3 Flavor P_{#bar#nu_{#tau}#rightarrow#bar#nu_{#mu}}",
   	// 				NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    // TH2D *NuTauToNuTau3f 	=  new TH2D("NuTauToNuTau3f","3 Flavor P_{#bar#nu_{#tau}#rightarrow#bar#nu_{#tau}}",
   	// 				NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);
    // TH2D *NuTauToNuX3f 	=  new TH2D("NuTauToNuX3f","3 Flavor P_{#bar#nu_{#tau}#rightarrow#bar#nu_{x}}",
   	// 				NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);


   
//    TH2D *NuMuToNuTau2f 	=  new TH2D("NuMuToNuTau2f","2 Flavor P_{#nu_{#mu}#rightarrow#nu_{#tau}}",
//    					NBinsEnergy  -1 , EnergyBins, Dm12ZenithNbin -1, Dm12ZenithEdge);

//------------------- End of Raw Probability Plots	


    // double total = 0.0;
    // fill the probabilities
    for ( i = 0 ; i <= NBinsEnergy ; i ++ ) {


        // // energy in log space
        // energy = e_start*pow(10.0, double(i)*e_step);

        // energy in linear space
        energy = e_start + double(i)*e_step;

        for ( j = 0 ; j <= Dm12ZenithNbin ; j++ ) {


            cosineZ = cz_start + double(j)*cz_step;


            otf.DefinePath( cosineZ, 15.0  , true);
        
            // total = 0.0;
	        // total += myNu->GetProb(1,1);
	        // total += myNu->GetProb(1,2);
	        // total += myNu->GetProb(1,3);

	        // if( fabs( 1.00 - total ) > 1.0e-7 ) abort();
	
	
            // kPdgNuEBar, kPdgNuMuBar, kPdgNuTauBar for anti-nu
            std::array<double, 3> Probability = calprop(kPdgNuE, energy, cosineZ);
            NuEToNuE3f 	->Fill( energy, cosineZ, Probability[0]) ;
            NuEToNuMu3f 	->Fill( energy, cosineZ, Probability[1]);
            NuEToNuTau3f 	->Fill( energy, cosineZ, Probability[2]) ;
            NuEToNuX3f 	->Fill( energy, cosineZ, 1.0 - Probability[0]
             - Probability[1] - Probability[2] ) ;

            Probability = calprop(kPdgNuMu, energy, cosineZ);
            NuMuToNuE3f 	->Fill( energy, cosineZ, Probability[0] ) ;
            NuMuToNuMu3f 	->Fill( energy, cosineZ, Probability[1] ) ;
            NuMuToNuTau3f 	->Fill( energy, cosineZ, Probability[2] ) ;
            NuMuToNuX3f 	->Fill( energy, cosineZ, 1.0 - Probability[0]
             - Probability[1] - Probability[2] ) ;
            
            Probability = calprop(kPdgNuTau, energy, cosineZ);
            NuTauToNuE3f 	->Fill( energy, cosineZ, Probability[0] ) ;
            NuTauToNuMu3f 	->Fill( energy, cosineZ, Probability[1] ) ;
            NuTauToNuTau3f 	->Fill( energy, cosineZ, Probability[2] ) ;
            NuTauToNuX3f 	->Fill( energy, cosineZ, 1.0 - Probability[0]
             - Probability[1] - Probability[2] ) ;
        } // End Zenith Angle Loop //
   
    } // End Energy Loop //


    TFile *tmp = new TFile("LineProb_ICm5p_normal.root", "recreate");
    tmp->cd();

    NuEToNuE3f    ->Write();
    NuEToNuMu3f   ->Write();
    NuEToNuTau3f  ->Write();
    NuEToNuX3f    ->Write();
   
    NuMuToNuE3f   ->Write();
    NuMuToNuMu3f  ->Write();
    NuMuToNuTau3f ->Write();
    NuMuToNuX3f   ->Write();

    NuTauToNuE3f  ->Write();
    NuTauToNuMu3f ->Write();
    NuTauToNuTau3f->Write();
    NuTauToNuX3f  ->Write();

//    NuMuToNuTau2f ->Write();

    tmp->Close();
   	
    std::cout << std::endl<<"Done Jake!" << std::endl;
    return 0;
}






std::array<double, 3> calprop( int nu_start , double energy, double cosz) {

    std::array<std::array<double, 2>, 3> outputFlavourState;
    std::array<std::array<double, 2>, 3> initialFlavourState;
    enum {
        re,
        im
    };

    for(int i=0; i<3; i++)
    {
        initialFlavourState[i][re] = 0.0;
        initialFlavourState[i][im] = 0.0;
    }

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<2; j++)
        {
        outputFlavourState[i][j] = 0.0;
        }
    }

    std::array<double, 3> Probability;

    std::vector<double> Path = otf.getTraverseDistance();
    std::vector<double> DensityProfile = otf.getTraverseRhos();
    std::vector<double> ElectronProfile = otf.getYp();
    int nLayer = otf.getLayers();
	
    for (int i = 0; i < nLayer ; i++ )
    {
        otf.SetBaseline(Path[i]);
        otf.SetEDensity(DensityProfile[i], ElectronProfile[i]);
        
        if ( i == 0 ) {
            // The neutrino is formed in a definite flavour state
            if(nu_start == kPdgNuE || nu_start == kPdgNuEBar)
            {
                initialFlavourState[0][re] = 1.0;
            }
            else 
            if(nu_start == kPdgNuMu || nu_start == kPdgNuMuBar)
            {
                initialFlavourState[1][re] = 1.0;
            }
            else 
            if(nu_start == kPdgNuTau || nu_start == kPdgNuTauBar)
            {
                initialFlavourState[2][re] = 1.0;
            }

            outputFlavourState = otf.MatterOscProb(nu_start, energy, initialFlavourState);
        }

        else if ( i >0 ) {
            outputFlavourState = otf.MatterOscProb(nu_start, energy, outputFlavourState);
        }//for other layers
    }// end of layer loop


    double total = 0.0;
    for (int i=0; i<3; ++i){
        Probability[i] = outputFlavourState[i][re]*outputFlavourState[i][re] 
        + outputFlavourState[i][im]*outputFlavourState[i][im];
    }

    return Probability;
}
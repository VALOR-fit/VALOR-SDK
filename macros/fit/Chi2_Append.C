//____________________________________________________________________________
/*!

\brief     Attach chi2_best_fit_toy from existing fit file onto output parameter and spectra tree

\options

\author    Jaiden Parlone <j.parlone \at liverpool.ac.uk>
           University of Liverpool

\created   October 2020

\copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

//System
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
//Root
#include "TMath.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include <TTree.h>
//VALOR

    void Chi2_Append(
            const char* ParameterFile = "", //Output from saveparamthrows during toy generation.
            const char* FitFile = "", //Output file from fitting the toys.
            const char* OutputFile = "parameter_values_with_ChiSquare.root",
            const char* BaseTTree = "marg_parameter_values",
            const char* BestFitToy = "chi2_best_fit_toy"
            ) {
        TFile *f1 = new TFile(ParameterFile, "read");
        TTree *t1 = (TTree *) f1->Get(BaseTTree); // base tree to append to

        TFile *f2 = new TFile(FitFile, "read");
        std::vector<double> *best_fit_toy = (std::vector<double> *) f2->Get(BestFitToy);

        TFile *newFile = new TFile(OutputFile,"recreate");
        TTree *newTree = (TTree *)t1->CloneTree(); // TODO: Look into this creating TH1 error.
        newFile->cd();
        std::string vector_name = BestFitToy; // chi2_best_fit_toy is of type vector<double>
        std::string branch_info = vector_name + "/D";
        double toy_branch = -999;

       // std::cout << "Size of vector" << best_fit_toy->size() << std::endl; //TODO: set this up as an error check
        auto branch = newTree->Branch(vector_name.c_str(), &(toy_branch), branch_info.c_str()); //Create Branch for best fit toy
        for (std::vector<double>::iterator i = best_fit_toy->begin(); i != best_fit_toy->end(); i++) {
            toy_branch = *i;
            branch->BackFill();
        }
        newTree->Write();
    }

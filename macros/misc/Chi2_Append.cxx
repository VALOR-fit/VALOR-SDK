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

//Use a TChain
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

    void Chi2_Append() {
        TFile *f1 = new TFile("neural_net_test_normal_cpv_reactor_0_1_2_toy_throws.root", "read");
        TTree *t1 = (TTree *) f1->Get("marg_parameter_values"); // base tree to append to

        TFile *f2 = new TFile("neuraltestfit_cpv_t2k_0_1_result.root", "read");
        std::vector<double> *best_fit_toy = (std::vector<double> *) f2->Get("chi2_best_fit_toy");

        TFile *newFile = new TFile("neural_net_test_normal_cpv_reactor_0_1_2_toy_throws_with_ChiSquare.root","recreate");
        TTree *newTree = (TTree *)t1->CloneTree(); // TODO: Look into this creating TH1 error.
        newFile->cd();
        std::string vector_name = "chi2_best_fit_toy"; // chi2_best_fit_toy is of type vector<double>
        std::string branch_info = vector_name + "/D";
        double toy_branch = -999;

        newTree->Branch(vector_name.c_str(), &(toy_branch), branch_info.c_str()); //Create Branch for best fit toy

  //  std::cout << "Size of vector" << best_fit_toy->size() << std::endl; //TODO: set this up as an error check

    newTree->SetBranchStatus("*",0);
    newTree->SetBranchStatus("chi2_best_fit_toy",1); //Only Fill ChiSquared branch

    for (std::vector<double>::iterator i = best_fit_toy->begin(); i != best_fit_toy->end(); i++) {
        toy_branch = *i;

    }
    newTree->Fill();
    newTree->SetBranchStatus("*",1);
        newTree->Write();
    }


    //Create new TTree with vector then use TChain.
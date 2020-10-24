//____________________________________________________________________________
/*!

\app       valor-TBranchAppend

\brief     Attatch existing TBranches together to form a TTree. Currently in prototype stage where a vector can be attatched to a set of TBranches.

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

//difference of this and using namespace
    int main() {

        // TODO: (Generalise) Get inputs from command line: File1; Branches from file 1, File 2, Branches from file 2, etc.

        // TODO: Open copy of TFile

        TFile *f1 = new TFile("neural_net_test_normal_cpv_reactor_0_1_2_toy_throws.root", "read");

        //  TString outfilename = "neural_net_test_normal_cpv_reactor_0_1_2_toy_throws_with_ChiSquare.root";

        // TODO: Get TTree to append branch to

        //TODO: (Generalise) Create array of TTrees wanting to combine with names from command line
        /*
        TList *list = new TList;
        for(int i = 0, i < NTrees+1, i++) {
            list->Add(TreeArray[i]);
        }
         */

        TTree *t1 = (TTree *) f1->Get("marg_parameter_values"); // base tree to append to

        // TODO: Which branches do you want to clone?
        /*
        for (const auto activeBranchName : {"event", "fNtrack", "fNseg", "fH"}) //generalise activeBranchName with branch string
            oldtree->SetBranchStatus(activeBranchName, 1);

        */
        // TODO: Open read only version of file 2 that you want branch from.

        TFile *f2 = new TFile("neuraltestfit_cpv_t2k_0_1_result.root", "read");
        std::vector<double> *best_fit_toy = (std::vector<double> *) f2->Get("chi2_best_fit_toy");


        // TODO: Copy branch from from file 2 onto final file.


        TFile *newFile = new TFile("neural_net_test_normal_cpv_reactor_0_1_2_toy_throws_with_ChiSquare.root","recreate");
        TTree *newTree = (TTree *)t1->CloneTree(); // TODO: Look into this creating TH1 error.
        newFile->cd();
        std::string vector_name = "chi2_best_fit_toy"; // chi2_best_fit_toy is of type vector<double>
        std::string branch_info = vector_name + "/D";
        double toy_branch = -999;

      newTree->Branch(vector_name.c_str(), &(toy_branch), branch_info.c_str()); //Create Branch for best fit toy

    std::cout << "Size of vector" << best_fit_toy->size() << std::endl; //TODO: set this up as an error check

   // newTree->SetBranchStatus("*",0);
   // newTree->SetBranchStatus("chi2_best_fit_toy",1); //Only Fill ChiSquared branch

    for (std::vector<double>::iterator i = best_fit_toy->begin(); i != best_fit_toy->end(); i++) {
        //  std::cout << *i << std::endl;
        toy_branch = *i;
        newTree->Fill();
    }
  //  newTree->SetBranchStatus("*",1);
        newTree->Write();

    return 0;
    }

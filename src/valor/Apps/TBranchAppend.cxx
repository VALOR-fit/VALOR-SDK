#include <iostream>
#include <sstream>
#include <string>
#include "TMath.h"

void TBranchAppend() {

    // TODO: (Generalise) Get inputs from command line: File1; Branches from file 1, File 2, Branches from file 2, etc.

    // TODO: Open copy of TFile

    TFile *f = new TFile("14MayTH2DNoBurnin.root", "read");

    // TODO: Get TTree to append branch to

    TTree *tree_sys_params = (TTree *) f->Get("posteriors");

    // TODO: Open read only version of file 2 that you want branch from.

    // TODO: Copy branch from from file 2 onto final file.

    // TODO: aaWrite output file.
}

// For ROOT 6 with LLVM, none of our own classes can be referenced in this file
// Put anything you need in valor_utils.C

void valor_interpreter()
{   
    TString curr_dir = gSystem->pwd();
    gSystem->cd(curr_dir.Data());

    TString top_dir = gSystem->Getenv("VALOR_FMWK");

    TString loadincs = ".x " + top_dir + "/root/loadincs.C";
    TString loadlibs = ".x " + top_dir + "/root/loadlibs.C";

    gROOT->ProcessLine(loadincs.Data());    
    gROOT->ProcessLine(loadlibs.Data());    
    
    TString loadutils = ".x " + top_dir + "/root/valor_utils.C";
}

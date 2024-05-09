//________________________________________________________________________________________
/*

 Copyright (c) 2010-2020, The VALOR group. All rights reserved

 ChangeLog:

 */
//________________________________________________________________________________________

#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <math.h>

#include <TSystem.h>
#include "TMath.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TROOT.h"

#include "valor/Messenger/Messenger.h"
#include "valor/Conventions/Constants.h"
#include "valor/Conventions/PhysParamId.h"
#include "valor/Utilities/ConfidenceInterval.h"
#include "valor/FitCore/FitParamList.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/UtlSystem.h"

using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

using namespace valor;
using namespace valor::conv;

ClassImp(ConfidenceInterval)
//________________________________________________________________________________________
ConfidenceInterval::ConfidenceInterval() :
        TNamed()
{

}


//________________________________________________________________________________________
ConfidenceInterval::ConfidenceInterval(string in_file_name, string confidence, PhysParamId_t par_x, PhysParamId_t par_y, bool avg_or_min_delta_chi2, bool find_delta_chi2) :
        TNamed()
{
    param_x = par_x;
    param_y = par_y;

    bin_low_x = conv::kALargeNum;
    bin_high_x = -conv::kALargeNum;
    bin_low_y = conv::kALargeNum;
    bin_high_y = -conv::kALargeNum;

    n_bins_x = 1; n_bins_y = 1; n_bins = 1;
    min_chi2 = 0; bin_low_x = 0; bin_high_x = 1; bin_low_y = 0; bin_high_y = 1;
    scale_limits = 0.0000000001;

    output_graphs = new TObjArray();
    chi2_2D = new TH2D("chi2_2D", "chi2_2D", 1, 0, 1, 1, 0, 1);
    n_entries_2D = new TH2D("n_entries_2D", "n_entries_2D", 1, 0, 1, 1, 0, 1);
    chi2_1D = new TH1D;

    this->Generate(in_file_name, confidence, avg_or_min_delta_chi2, find_delta_chi2);
}


//________________________________________________________________________________________
ConfidenceInterval::~ConfidenceInterval()
{
    delete chi2_2D;
    delete n_entries_2D;
}


//________________________________________________________________________________________
void ConfidenceInterval::Generate(string in_file_name, string confidence, bool avg_or_min_delta_chi2, bool find_delta_chi2)
{
    // Setup a TChain containing the input files
    VLOG("Messenger", pINFO) << "Adding files to TChain";
    TChain* chain = new TChain("valor_dune_nd_ntp");
    chain->Add(in_file_name.c_str());
    if(chain->GetEntries() == 0) VLOG("Messenger", pFATAL) << "No input root files detected.";
    VLOG("Messenger", pINFO) << "Finished adding files to TChain";

    // Loop over the input files to find the number of bins and the lowest and highest bin edges
    GetBinningAndData(chain);

    // Setup histogram bins
    chi2_2D->SetBins	 (n_bins_x, bin_low_x, bin_high_x, n_bins_y, bin_low_y, bin_high_y);
    n_entries_2D->SetBins(n_bins_x, bin_low_x, bin_high_x, n_bins_y, bin_low_y, bin_high_y);

    // Add the data into their respective histograms
    FillHists(avg_or_min_delta_chi2);

    // Check for missing runs, underflows and overflows, and calculate the min chi^2
    Checks();

    // Calculate delta chi^2 (If requested to do so by the user)
    if(find_delta_chi2) FindDelta();

    // If we don't have a y parameter, take the projection to obtain a 1D hist
    if(param_y == kPhypUndefined){
        chi2_1D = chi2_2D->ProjectionX("chi2_1D", 1, 1);
        hist_limits[2] = chi2_1D->GetMinimum();
        hist_limits[3] = chi2_1D->GetMaximum();
    }

    // Parse the input confidence and calculate the critical delta chi^2 required to give the confidence
    double chi2_crit = FindCriticalChi2(confidence, find_delta_chi2);

    // Bins where the fit failed have no content, so when drawing a contour they would be included as having Delta chi^2 < critical chi^2.
    // Try to bilinerally interpolate the Delta chi^2 value in that bin. If that fails add a chi^2 penalty instead.
    FixFailure(chi2_crit);

    // Generate the constant delta chi^2 contour
    GenerateContour(chi2_crit);
}



//__________________________________ Private functions: __________________________________
//________________________________________________________________________________________

// Works out the histogram binning based on the input data
void ConfidenceInterval::GetBinningAndData(TChain* chain)
{
    VLOG("Messenger", pINFO) << "Calculating Binning and fetching input data";
    long n_entries = chain->GetEntries();
    std::vector<double> unique_x, unique_y;
    FitParamList *  best_fit_param_array = 0;
    chain->SetBranchAddress("best_fit_params", &best_fit_param_array);

    // If we have no y parameter, we'll need to manually define some arbitary binning
    if(param_y == kPhypUndefined){
        bin_low_y = 0;
        bin_high_y = 1;
    }

    for(long entry = 0; entry < n_entries; ++entry){
        chain->GetEntry(entry);
        bool fit_failed = false;

        if(!best_fit_param_array->IsFixed(param_x)) VLOG("Messenger", pWARN) << "X parameter  is not fixed";
        if(param_y != kPhypUndefined && !best_fit_param_array->IsFixed(param_y)) VLOG("Messenger", pWARN) << "Y parameter is not fixed";

        double x_val = best_fit_param_array->BestFitValue(param_x);
        if(x_val <= -99999999){
            VLOG("Messenger", pWARN) << "x best fit val <= -99999999 for file " << entry << ". Did the fit fail? Skipping this entry.";
            fit_failed = true;

            // In order to get the limits and bin numbering correct, we need to see what the value was meant to be (which is fine as the value is fixed during the fit)
            x_val = best_fit_param_array->InitValue(param_x);
        }

        // The number of unique x values will give how many bins we need in the x-direction
        if(entry == 0) unique_x.push_back(x_val);
        else{
            bool unique = true;
            for(unsigned int i = 0; i < unique_x.size(); ++i){
                if(unique_x[i] == x_val) unique = false;
            }
            if(unique) unique_x.push_back(x_val);
        }
        double y_val = 0;
        if(param_y != kPhypUndefined){
            y_val = best_fit_param_array->BestFitValue(param_y);
            if(y_val <= -99999999){
                VLOG("Messenger", pWARN) << "y best fit val <= -99999999 for file " << entry << ". Did the fit fail? Skipping this entry.";
                fit_failed = true;

                // In order to get the limits and bin numbering correct, we need to see what the value was meant to be (which is fine as the value is fixed during the fit)
                y_val = best_fit_param_array->InitValue(param_y);
            }
            if(entry == 0) unique_y.push_back(y_val);
            else{
                bool unique = true;
                for(unsigned int i = 0; i < unique_y.size(); ++i){
                    if(unique_y[i] == y_val) unique = false;
                }
                if(unique) unique_y.push_back(y_val);
            }
        }
        else if(entry == 0) unique_y.push_back(y_val);

        if(!fit_failed){
            x_vals.push_back(x_val);
            y_vals.push_back(y_val);
            double chi2 = best_fit_param_array->GetBestFitStatistic();
            chi2_vals.push_back(chi2);

            VLOG("Messenger", pDEBUG) << "file " << entry << ": ";
            VLOG("Messenger", pDEBUG) << "x_val = " << x_val;
            VLOG("Messenger", pDEBUG) << "y_val = " << y_val;
            VLOG("Messenger", pDEBUG) << "chi^2 = " << chi2 << "\n";
        }
    }

    // Calculate the bin limits and number of bins
    bin_low_x = unique_x[0];
    bin_high_x = unique_x[0];
    for(unsigned int i = 1; i < unique_x.size(); ++i){
        bin_low_x = std::min(bin_low_x, unique_x[i]);
        bin_high_x = std::max(bin_high_x, unique_x[i]);
    }
    if(param_y != kPhypUndefined){
        bin_low_y = unique_y[0];
        bin_high_y = unique_y[0];
        for(unsigned int i = 1; i < unique_y.size(); ++i){
            bin_low_y = std::min(bin_low_y, unique_y[i]);
            bin_high_y = std::max(bin_high_y, unique_y[i]);
        }
    }

    bin_low_x *= 1 - Sign(bin_low_x) * scale_limits;
    bin_high_x *= 1 + Sign(bin_high_x) * scale_limits;
    bin_low_y *= 1 - Sign(bin_low_y) * scale_limits;
    bin_high_y *= 1 + Sign(bin_high_y) * scale_limits;
    n_bins_x = (unsigned int) unique_x.size();
    n_bins_y = (unsigned int) unique_y.size();
    if(n_bins_y == 0) ++n_bins_y;

    hist_limits[0] = bin_low_x;
    hist_limits[1] = bin_high_x;
    hist_limits[2] = bin_low_y;
    hist_limits[3] = bin_high_y;

    VLOG("Messenger", pDEBUG) << "par_x ID = " << param_x;
    VLOG("Messenger", pDEBUG) << "min par_x val = " << bin_low_x << "; max par_x val = " << bin_high_x << "; n_bins_x = " << n_bins_x;
    VLOG("Messenger", pDEBUG) << "par_y ID = " << param_y;
    VLOG("Messenger", pDEBUG) << "min par_y val = " << bin_low_y << "; max par_y val = " << bin_high_y << "; n_bins_y = " << n_bins_y;
    VLOG("Messenger", pINFO) << "Finished calculating Binning and fetching input data";
}


//________________________________________________________________________________________
// Add the data to two histograms, one to keep track of the number of entries per bin and another to record the chi^2 per bin
void ConfidenceInterval::FillHists(bool avg_or_min_delta_chi2)
{
    VLOG("Messenger", pINFO) << "Filling Histograms";
    if(x_vals.size() < y_vals.size()) VLOG("Messenger", pERROR) << "x_vals.size() < y_vals.size(). This should never happen!";

    // Add the number of repeat runs for each (par_x, par_y) point into a hist
    for(unsigned int i = 0; i < x_vals.size(); ++i){
        n_entries_2D->Fill(x_vals[i], y_vals[i]);
    }

    // Add the values of param_x, param_y and the associated mean or min chi^2 to another hist
    if(avg_or_min_delta_chi2){
        // Sum the chi^2 at first when calculating the mean chi^2
        for(unsigned int i = 0; i < x_vals.size(); ++i){
            chi2_2D->Fill(x_vals[i], y_vals[i], chi2_vals[i]);
        }
        // Loop over each bin, dividing by the number of entries
        n_bins = chi2_2D->GetSize();
        if(n_bins != (unsigned int)n_entries_2D->GetSize()) VLOG("Messenger", pERROR) << "Different number of bins in chi2_2D and n_entries_2D";
        for(unsigned int bin = 1; bin < n_bins +1; ++bin){
            if(chi2_2D->IsBinUnderflow(bin) || chi2_2D->IsBinOverflow(bin)) continue;
            double n_bin_entries = n_entries_2D->GetBinContent(bin);
            double sum = chi2_2D->GetBinContent(bin);
            if(n_bin_entries == 0 && sum != 0) VLOG("Messenger", pERROR) <<  "n_bin_entries = 0 but sum != 0. This should never happen!";
            if(n_bin_entries == 0) continue;
            chi2_2D->SetBinContent(bin, sum / n_bin_entries);
        }
    }
    else{
        // Find the min chi^2 value for each bin
        for(unsigned int i = 0; i < x_vals.size(); ++i) {
            int bin_x = chi2_2D->GetXaxis()->FindBin(x_vals[i]);
            int bin_y = chi2_2D->GetYaxis()->FindBin(y_vals[i]);
            if ((chi2_vals[i] < chi2_2D->GetBinContent(bin_x, bin_y)) ||
                (chi2_2D->GetBinContent(bin_x, bin_y) <= 0)){
                chi2_2D->SetBinContent(bin_x, bin_y, chi2_vals[i]);
            }
        }
    }
    VLOG("Messenger", pINFO) << "Finished filling Histograms";
}


//________________________________________________________________________________________
// Checks for underflows/overflows, whether each bin has the expected number of entries and finds the best fit point
void ConfidenceInterval::Checks()
{
    VLOG("Messenger", pINFO) << "Performing checks on the histograms";
    int n_entries_max = n_entries_2D->GetBinContent(n_entries_2D->GetMaximumBin());
    bool initial_val = false;

    for(unsigned int bin = 0; bin < n_bins + 2; ++bin){
        int content = n_entries_2D->GetBinContent(bin);
        if(!n_entries_2D->IsBinUnderflow(bin) && !n_entries_2D->IsBinOverflow(bin)) {
            // Check for missing runs
            if(content != n_entries_max) {
                int bin_x, bin_y, bin_z;
                n_entries_2D->GetBinXYZ(bin, bin_x, bin_y, bin_z);
                VLOG("Messenger", pWARN) <<  content << " runs found for x-axis bin centre = " << n_entries_2D->GetXaxis()->GetBinCenter(bin_x)
                                        << ", y-axis bin centre = " << n_entries_2D->GetYaxis()->GetBinCenter(bin_y)
                                        << ". Expected " << n_entries_max << " runs.";
            }

            if(content != 0){
                // Check for underflows and overflows (shouldn't happen)
                if (chi2_2D->IsBinUnderflow(bin) && chi2_2D->GetBinContent(bin) > 0 ) 				VLOG("Messenger", pERROR) <<  "> 0 underflows detected in chi2_2D at global bin # " << bin;
                else if (chi2_2D->IsBinOverflow(bin) && chi2_2D->GetBinContent(bin) > 0 ) 			VLOG("Messenger", pERROR) << "> 0 overflows detected in chi2_2D at global bin # " << bin;
                if (n_entries_2D->IsBinUnderflow(bin) && n_entries_2D->GetBinContent(bin) > 0 ) 	VLOG("Messenger", pERROR) <<  "> 0 underflows detected in n_entries_2D at global bin # " << bin;
                else if (n_entries_2D->IsBinOverflow(bin) && n_entries_2D->GetBinContent(bin) > 0 ) VLOG("Messenger", pERROR) << "> 0 overflows detected in n_entries_2D at global bin # " << bin;

                // Calc. min chi^2
                double chi2 = chi2_2D->GetBinContent(bin);
                if(!initial_val) {
                    min_chi2 = chi2;
                    initial_val = true;
                }
                else{
                    if(chi2 < min_chi2){
                        min_chi2 = chi2;
                        //min_chi2_bin = bin;
                        int bin_x, bin_y, bin_z;
                        chi2_2D->GetBinXYZ(bin, bin_x, bin_y, bin_z);
                        best_fit_point[0] = chi2_2D->GetXaxis()->GetBinCenter(bin_x);
                        best_fit_point[1] = chi2_2D->GetYaxis()->GetBinCenter(bin_y);
                        best_fit_point[2] = min_chi2;
                    }
                }
            }
        }
    }
    VLOG("Messenger", pDEBUG) <<  "min_chi2 = " 	<< best_fit_point[2];
    VLOG("Messenger", pDEBUG) <<  "x_bf = " 		<< best_fit_point[0];
    VLOG("Messenger", pDEBUG) <<  "y_bf = " 		<< best_fit_point[1];
    VLOG("Messenger", pINFO) << "Finished performing checks on the histograms";
}


//________________________________________________________________________________________
// Converts the chi^2 histogram into a delta chi^2 histogram
void ConfidenceInterval::FindDelta()
{
    VLOG("Messenger", pINFO) << "Finding delta chi^2";
    for(unsigned int bin = 1; bin < n_bins + 1; ++bin){
        int content = n_entries_2D->GetBinContent(bin);
        if(!n_entries_2D->IsBinUnderflow(bin) && !n_entries_2D->IsBinOverflow(bin) && content != 0) {
            double chi2 = chi2_2D->GetBinContent(bin);
            chi2_2D->SetBinContent(bin, chi2 - min_chi2);
        }
    }
    VLOG("Messenger", pINFO) << "Finished finding delta chi^2";
}


//________________________________________________________________________________________
// Converts the input confidence level (in units of % or standard deviations) into a critical delta chi^2
double ConfidenceInterval::FindCriticalChi2(string confidence, bool find_delta_chi2)
{
    // Parse the input confidence
    VLOG("Messenger", pINFO) << "Calculating the critical delta chi^2";
    double conf_level = 0.90;
    std::string conf_type = "%";
    std::string buf;
    std::stringstream ss(confidence);
    vector<std::string> conf_info;
    while (ss >> buf)  conf_info.push_back(buf);
    if(conf_info.size() != 2) VLOG("Messenger", pERROR) << "Input confidence level should have 2 entries separated with whitespace";
    bool conf_info_valid = true;
    if(conf_info[0].find_first_not_of("0123456789.") != std::string::npos) conf_info_valid = false;     // The confidence level should only be a number
    if(!((conf_info[1] == "%") || (conf_info[1] == "sigma"))) conf_info_valid = false;     // The confidence level type should only be % or sigma
    if(!conf_info_valid) VLOG("Messenger", pERROR) << "Input confidence should be of the form <confidence> <% or sigma>, e.g. 0.90 %";
    conf_level = std::atof(conf_info[0].c_str());
    conf_type = conf_info[1];
    if(conf_type == "sigma") conf_level = 0.5*(TMath::Erf(conf_level/sqrt(2)) - TMath::Erf(-conf_level/sqrt(2)));
    else conf_level /= 100;     // Convert from % to decimal

    // Calculate the delta chi^2 required to give the confidence
    double chi2_crit = 0;
    if(param_y == kPhypUndefined) chi2_crit = TMath::ChisquareQuantile(conf_level, 1);
    else chi2_crit = TMath::ChisquareQuantile(conf_level, 2);
    if(!find_delta_chi2) chi2_crit += min_chi2;
    VLOG("Messenger", pDEBUG) << "Confidence level = " << conf_level;
    VLOG("Messenger", pDEBUG) << "Critical delta chi^2 = " << chi2_crit;
    VLOG("Messenger", pINFO) << "Finished calculating the critical delta chi^2";
    return chi2_crit;
}



//________________________________________________________________________________________
// Accounts for bins where all fits failed or where there are no data. This prevents them from being considered as delta chi^2 = 0 bins
void ConfidenceInterval::FixFailure(double chi2_crit)
{
    VLOG("Messenger", pINFO) << "Fixing bins with no entries";
    double penalty = 1.01 * chi2_crit;
    vector<int> penalised_bins;
    for(unsigned int bin = 1; bin < n_bins + 1; ++bin){
        int content = n_entries_2D->GetBinContent(bin);
        double chi2 = chi2_2D->GetBinContent(bin);
        if(!n_entries_2D->IsBinUnderflow(bin) && !n_entries_2D->IsBinOverflow(bin) && content == 0 && chi2 == 0){
            // There is no data in this bin, so fix it:
            // If the adjacent bins contain data, use bilinear interpolation, otherwise add a penalty term.
            int bins_per_row = n_bins_x + 2;	// Because of underflows/overflows
            int bin_s = (int)bin;
            int check_bins[8] = {bin_s + bins_per_row, bin_s - bins_per_row, bin_s + 1, bin_s - 1, bin_s + bins_per_row - 1, bin_s + bins_per_row + 1, bin_s - bins_per_row - 1, bin_s - bins_per_row + 1};		// Bin numbers of the adjacent bins
            bool can_interpolate = true;
            for(unsigned int i = 0; i < 8; ++i){
                int check = check_bins[i];

                // Don't interpolate if an adjacent bin was penalised, as this would skew the interpolation
                bool penalsied_adjacent = false;
                if(penalised_bins.size() != 0){
                    for(unsigned int j = 0; j < penalised_bins.size(); ++j){
                        if(penalised_bins[j] == check){
                            penalsied_adjacent = true;
                            break;
                        }
                    }
                }

                if(chi2_2D->IsBinUnderflow(check) || chi2_2D->IsBinOverflow(check)
                   || check < 1 || check > (int)n_bins || penalsied_adjacent
                   || n_entries_2D->GetBinContent(check) == 0){
                    can_interpolate = false;
                    break;
                }
            }

            int bin_x, bin_y, bin_z;
            chi2_2D->GetBinXYZ(bin, bin_x, bin_y, bin_z);
            double x = chi2_2D->GetXaxis()->GetBinCenter(bin_x);
            double y = chi2_2D->GetYaxis()->GetBinCenter(bin_y);

            double interpolated_val = -1;
            if(can_interpolate){
                interpolated_val = chi2_2D->Interpolate(x ,y);		// TODO: Interpolation seems to fail unexpectedly (returns 0), look into this
                if(interpolated_val != 0){
                    chi2_2D->SetBinContent(bin, interpolated_val);
                    VLOG("Messenger", pNOTICE) << "Chi^2 value at x = " << x << ", y = " << y << " has been interpolated and set to " << interpolated_val;
                }
                else VLOG("Messenger", pNOTICE) << "Interpolation failed at x = " << x << ", y = " << y;
            }
            if(!can_interpolate || interpolated_val == 0){
                chi2_2D->SetBinContent(bin, penalty);
                penalised_bins.push_back(bin);
                VLOG("Messenger", pNOTICE) << "Chi^2 value at x = " << x << ", y = " << y << " could not be interpolated so has been set to 1.01 * critical Delta chi^2, which is " << penalty;
            }
        }
    }
    VLOG("Messenger", pINFO) << "Finished fixing bins with no entries";
}


//________________________________________________________________________________________
// Generates the constant delta chi^2 contour graphs
void ConfidenceInterval::GenerateContour(double chi2_crit)
{
    VLOG("Messenger", pINFO) << "Generating the confidence interval";
    TCanvas *c = new TCanvas("c", "Contour List", 0, 0, 600, 600);

    if(param_y != kPhypUndefined){
        chi2_2D->SetContour(1, &chi2_crit);    // chi2_crit may need to be an array
        chi2_2D->Draw("CONT LIST");   // This generates a list of TGraph objects for each contour
        c->Update();    // This is needed to retrieve the contours
        TObjArray *conts = dynamic_cast<TObjArray *>(gROOT->GetListOfSpecials()->FindObject("contours"));
        if (conts == NULL) {
          util::Die(-1);
          VLOG("Messenger", pERROR) << "No Contours Found";
          return;
        }

        /* // For testing
        chi2_2D->Draw("text");
        TGraph *best_fit_marker = new TGraph(1, &best_fit_point[0], &best_fit_point[1]);
        best_fit_marker->SetMarkerColor(4);
        best_fit_marker->SetMarkerSize(3);
        best_fit_marker->Draw("*");
        c->Update();
        string boo;
        cout << "press any key to continue" << endl;
        std::cin >> boo;
        c->Update();
        // */


        TList *contLevel = (TList *) conts->At(0);
        TGraph *curv = (TGraph *) contLevel->First();
        TGraph *gc = NULL;
        Double_t x0, y0;
        for (int j = 0; j < contLevel->GetSize(); j++) {
            curv->GetPoint(0, x0, y0);
            gc = (TGraph *) curv->Clone();   // Draw clones of the graphs to avoid deletions in case the 1st pad is redrawn.
            output_graphs->Add(gc);
            //gc->Draw("C");        // For testing
            curv = (TGraph *) contLevel->After(curv); // Get Next graph
        }

    }
    else{
        double bin_width = (bin_high_x - bin_low_x) / n_bins_x;
        double line_x_vals [2] = {bin_low_x - bin_width, bin_high_x};
        double line_y_vals [2] = {chi2_crit, chi2_crit};
        TGraph *conf_limit = new TGraph(2, line_x_vals, line_y_vals);
        output_graphs->Add(conf_limit);

        /*	// For testing
        //c->DrawFrame(bin_low_x, chi2_1D->GetMinimum(), bin_high_x, chi2_1D->GetMaximum());
        //chi2_1D->Draw("same");
        //conf_limit->Draw("same");
        c->Update();
        // */
    }
    c->Close();
    VLOG("Messenger", pINFO) << "Finished generating the confidence interval";
}

//________________________________________________________________________________________
double ConfidenceInterval::Sign(double num)
{
    if(num >= 0) return 1;
    return -1;
}

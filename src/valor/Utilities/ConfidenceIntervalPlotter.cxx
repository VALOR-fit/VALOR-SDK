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
#include "TGraph.h"
#include "TROOT.h"

#include "valor/Messenger/Messenger.h"
#include "valor/Conventions/Constants.h"
#include "valor/Conventions/PhysParamId.h"
#include "valor/Utilities/ConfidenceIntervalPlotter.h"
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

ClassImp(ConfidenceIntervalPlotter)
//________________________________________________________________________________________
ConfidenceIntervalPlotter::ConfidenceIntervalPlotter() :
        TNamed()
{

}


//________________________________________________________________________________________
ConfidenceIntervalPlotter::ConfidenceIntervalPlotter(PhysParamId_t par_x, PhysParamId_t par_y, bool avg_or_min_delta_chi2, bool find_delta_chi2, string input_titles) :
        TNamed()
{
    param_x = par_x;
    param_y = par_y;
    avg_delta_chi2 = avg_or_min_delta_chi2;
    find_delta = find_delta_chi2;
    titles = input_titles;

    limits[0] =  conv::kALargeNum;
    limits[1] = -conv::kALargeNum;
    limits[2] =  conv::kALargeNum;
    limits[3] = -conv::kALargeNum;

    line_colour = 1; bf_colour = 1; hist_colour = 2;
    line_style = 1; bf_style = 29; hist_style = 1;
    bf_size = 1.5;
    label = "Confidence Limit"; hist_label = "Data";
    bf_marker_added = false; manual_range = false;
    graphs_to_draw = new TObjArray();
    bfp_to_draw = new TObjArray();
    hists_to_draw = new TObjArray();
    leg = new TLegend(0.6, 0.75, 0.9, 0.95);
}


//________________________________________________________________________________________
ConfidenceIntervalPlotter::~ConfidenceIntervalPlotter()
{

}

//__________________________________ Public functions: __________________________________
//_______________________________________________________________________________________
// Add a dataset to the plot, generate the confidence interval and set the plotting information
void ConfidenceIntervalPlotter::Add(string in_file_name = "", string confidence = "90 %")
{
    LOG("Messenger", pNOTICE) << " \n Adding dataset " << in_file_name;
    ConfidenceInterval *conf = new ConfidenceInterval(in_file_name.c_str(), confidence, param_x, param_y, avg_delta_chi2, find_delta);
    TObjArray *graph_array = new TObjArray();
    graph_array = conf->GetConfidenceInterval();

    // Change the axis limits, if needed
    if(!manual_range){
        double *temp_limits = conf->GetLimits();
        limits[0] = TMath::Min(limits[0], temp_limits[0]);
        limits[1] = TMath::Max(limits[1], temp_limits[1]);
        limits[2] = TMath::Min(limits[2], temp_limits[2]);
        limits[3] = TMath::Max(limits[3], temp_limits[3]);
    }

    // Set up the best-fit point marker and contour graphs. Don't draw them yet, as the axis limits may not be finalised
    if(param_y != conv::kPhypUndefined){
        double *best_fit_point = conf->GetBestFitPoint();
        TGraph *best_fit_marker = new TGraph(1, &best_fit_point[0], &best_fit_point[1]);
        best_fit_marker->SetMarkerColor(bf_colour);
        best_fit_marker->SetMarkerSize(bf_size);
        best_fit_marker->SetMarkerStyle(bf_style);

        bfp_to_draw->Add(best_fit_marker);
        if(!bf_marker_added){
            bf_marker_added = true;
            leg->AddEntry(best_fit_marker, "Best-fit", "p");
        }
    }
    else{
        TH1D *chi2_1D = new TH1D();
        chi2_1D = conf->GetChi2Data_1D();

        TH1D *hist = new TH1D();
        hist = (TH1D *) chi2_1D->Clone();

        hist->SetLineColor(hist_colour);
        hist->SetLineStyle(hist_style);
        hists_to_draw->Add(hist);

        if(!CheckLegendLabel(hist_labels, hist_label)){
            leg->AddEntry(hist, hist_label.c_str(), "l");
            hist_labels.push_back(hist_label);
        }
    }

    TGraph *graph = (TGraph *) graph_array->First();
    TGraph *gc = NULL;
    for (int j = 0; j < graph_array->GetSize(); j++) {
        gc = (TGraph *) graph->Clone();

        gc->SetLineColor(line_colour);
        gc->SetLineStyle(line_style);
        graphs_to_draw->Add(gc);

        if(!CheckLegendLabel(line_labels, label)){
            leg->AddEntry(gc, label.c_str(), "l");
            line_labels.push_back(label);
        }

        graph = (TGraph *) graph_array->After(graph); // Get Next graph
        if(graph == 0) break;
    }

    delete conf;
    LOG("Messenger", pINFO) << "Finished adding dataset " << in_file_name;
}


//________________________________________________________________________________________
// Draw the collection of confidence intervals and datasets
void ConfidenceIntervalPlotter::Draw(TCanvas *canvas)
{
    // Draw the axes and titles
    LOG("Messenger", pNOTICE) << "Drawing all added confidence intervals";
    canvas->cd();
    frame = canvas->DrawFrame(limits[0], limits[2], limits[1], limits[3], titles.c_str());

    TGraph *graph;
    TGraph *gc;

    if(param_y != conv::kPhypUndefined){
        // Draw the best-fit points
        graph = (TGraph *) bfp_to_draw->First();
        gc = NULL;
        for (int i = 0; i < bfp_to_draw->GetSize(); i++) {
            gc = (TGraph *) graph->Clone();
            gc->Draw("P");
            graph = (TGraph *) bfp_to_draw->After(graph);
            if(graph == 0) break;
        }
    }
    else{
        // Draw the 1D histograms
        TH1D *hist = (TH1D*) hists_to_draw->First();
        TH1D *hc = NULL;
        for (int i = 0; i < hists_to_draw->GetSize(); i++) {
            hc = (TH1D *) hist->Clone();
            hc->Draw("same");
            hist = (TH1D *) hists_to_draw->After(hist);
            if(hist == 0) break;
        }
    }

    // Draw the constant Delta chi^2 contours
    graph = (TGraph *) graphs_to_draw->First();
    gc = NULL;
    for (int i = 0; i < graphs_to_draw->GetSize(); i++) {
        gc = (TGraph *) graph->Clone();
        gc->Draw("C");
        graph = (TGraph *) graphs_to_draw->After(graph);
        if(graph == 0) break;
    }

    // Draw the legend
    leg->Draw();

    canvas->Update();
    LOG("Messenger", pINFO) << "Finished drawing all added confidence intervals";
}


//________________________________________________________________________________________
// The axis ranges use the range of input data by default, but can be set to something else
void ConfidenceIntervalPlotter::SetRangeManually(double x_low, double y_low, double x_high, double y_high)
{
    manual_range = true;
    limits[0] = x_low;
    limits[1] = x_high;
    limits[2] = y_low;
    limits[3] = y_high;
}


//__________________________________ Private functions: __________________________________
//________________________________________________________________________________________

// Check the legend for a matching label. Returns true if the legend already contains an entry that has a matching label
bool ConfidenceIntervalPlotter::CheckLegendLabel(vector<string> vec, string lab)
{
    bool match = false;

    if(vec.size() != 0){
        for(unsigned int i = 0; i < vec.size(); ++i){
            if(vec[i] == lab){
                match = true;
                break;
            }
        }
    }

    return match;
}


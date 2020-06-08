//____________________________________________________________________________
/*!

 \class     valor::ConfidenceIntervalPlotter

 \brief     Creates constant chi^2 contour graphs by combining the graphs created by the ConfidenceInterval class

 \author    Francis Bench
 University of Liverpool

 \created   December 20, 2016

 \copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

 */
//____________________________________________________________________________

#ifndef _valor_confidence_interval_plotter_h_
#define _valor_confidence_interval_plotter_h_

#include <iostream>
#include <string>
#include <vector>

#include <TNamed.h>
#include <TObjString.h>
#include "TH1D.h"
#include "TH2.h"
#include "TH1.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "valor/Conventions/PhysParamId.h"

using std::string;

namespace valor {
    class ConfidenceIntervalPlotter;

    class ConfidenceIntervalPlotter: public TNamed {
    public:
        // Construct with information which must be consistent across all confidence intervals in the same plot
        ConfidenceIntervalPlotter(conv::PhysParamId_t par_x, conv::PhysParamId_t par_y, bool avg_or_min_delta_chi2, bool find_delta_chi2, string input_titles);
        ~ConfidenceIntervalPlotter();

        // Add a dataset to the plot, generate the confidence interval and set the plotting information
        void Add(string data_file, string confidence);

        // Draw the collection of confidence intervals and datasets
        void Draw(TCanvas *canvas);

        // The axis ranges use the range of input data by default, but can be set to something else
        void SetRangeManually(double x_low, double y_low, double x_high, double y_high);

        // Set the (optional) cosmetic settings for the graphs and histograms
        void SetChiSqLineColor(short int col) 	{line_colour 	= col;}
        void SetChiSqLineStyle(Style_t sty) 	{line_style 	= sty;}
        void SetChiSqLineLabel(string lab)		{label			= lab;}
        void SetBestFitColor(short int col)		{bf_colour 		= col;}		// Only used for 2D plots
        void SetBestFitStyle(Style_t sty)		{bf_style 		= sty;}		// Only used for 2D plots
        void SetBestFitSize(Size_t siz)			{bf_size		= siz;}		// Only used for 2D plots
        void SetDataLineColor(short int col) 	{hist_colour 	= col;}		// Only used for 1D plots
        void SetDataLineStyle(Style_t sty) 		{hist_style 	= sty;}		// Only used for 1D plots
        void SetDataLineLabel(string lab)		{hist_label		= lab;}		// Only used for 1D plots

        TH1F *frame;
        TLegend *leg;

        ClassDef(ConfidenceIntervalPlotter,1);


    private:
        ConfidenceIntervalPlotter();

        // Check the legend for a matching label. Returns true if the legend already contains an entry that has a matching label
        bool CheckLegendLabel(std::vector<string> vec, string label);

        bool bf_marker_added, manual_range;
        double limits[4];
        std::vector<string> line_labels, hist_labels;
        TObjArray *graphs_to_draw, *bfp_to_draw, *hists_to_draw;
        string titles;	// The graph and axis titles to be drawn. Expected format is "title;x_title;y_title"

        // Cosmetic settings for the graphs and histograms
        short int line_colour, bf_colour, hist_colour;
        Style_t line_style, bf_style, hist_style;
        Size_t bf_size;
        string label, hist_label;

        // Parameters common to each confidence interval to be drawn. These are passed to ConfidenceInterval
        conv::PhysParamId_t param_x;
        conv::PhysParamId_t param_y;
        bool avg_delta_chi2;
        bool find_delta;
    };
}
#endif

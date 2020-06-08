//____________________________________________________________________________
/*!

 \class     valor::ConfidenceInterval

 \brief     Creates a constant chi^2 confidence interval for a given dataset

 \author    Francis Bench
 University of Liverpool

 \created   November 28, 2016

 \copyright Copyright (c) 2010-2020, The VALOR group. All rights reserved

 */
//____________________________________________________________________________

#ifndef _valor_confidence_interval_h_
#define _valor_confidence_interval_h_

#include <iostream>
#include <string>
#include <vector>

#include <TNamed.h>
#include <TObjString.h>
#include "TH1D.h"
#include "TH2.h"
#include "TH1.h"
#include "TChain.h"

#include "valor/Conventions/PhysParamId.h"

using std::string;

namespace valor {
    class ConfidenceInterval;

    class ConfidenceInterval: public TNamed {
    public:
        ConfidenceInterval(string in_file_name, string confidence, conv::PhysParamId_t par_x, conv::PhysParamId_t par_y, bool avg_or_min_delta_chi2, bool find_delta_chi2);
        ~ConfidenceInterval();

        // Get the coordinates of the best-fit point as (param x coord, param y coord, delta chi^2). Histogram bin centres are used to approximate the coordinates
        double *GetBestFitPoint() {return best_fit_point;}

        // Get the axis limits of the histograms
        double *GetLimits() {return hist_limits;}

        // Get an array containing the TGraphs that make the 1D or 2D confidence intervals
        TObjArray *GetConfidenceInterval() const {return output_graphs;}

        // Get the 1D or 2D delta chi^2 histograms
        TH2D *GetChi2Data_2D() const {return chi2_2D;}
        TH1D *GetChi2Data_1D() const {return chi2_1D;}

        ClassDef(ConfidenceInterval,1);

    private:
        ConfidenceInterval();
        void Generate(string in_file_name, string confidence, bool avg_or_min_delta_chi2, bool find_delta_chi2);

        // Works out the histogram binning based on the input data
        void GetBinningAndData(TChain* chain);

        // Add the data to two histograms, one to keep track of the number of entries per bin and another to record the chi^2 per bin
        void FillHists(bool avg_or_min_delta_chi2);

        // Checks for underflows/overflows, whether each bin has the expected number of entries and finds the best fit point
        void Checks();

        // Converts the chi^2 histogram into a delta chi^2 histogram
        void FindDelta();

        // Converts the input confidence level (in units of % or standard deviations) into a critical delta chi^2
        double FindCriticalChi2(string confidence, bool find_delta_chi2);

        // Accounts for bins where all fits failed or where there are no data. This prevents them from being considered as delta chi^2 = 0 bins
        void FixFailure(double chi2_crit);

        // Generates the constant delta chi^2 contour graphs
        void GenerateContour(double chi2_crit);

        double Sign(double num);

        conv::PhysParamId_t param_x;
        conv::PhysParamId_t param_y;
        unsigned int n_bins_x, n_bins_y, n_bins;
        double min_chi2, bin_low_x, bin_high_x, bin_low_y, bin_high_y;
        double scale_limits;     //TODO: Maybe it would be better to just add on an extra bin to the end of all histograms, but this works for now
        std::vector<double> x_vals, y_vals, chi2_vals;
        double best_fit_point[3];   // (x, y, delta chi^2 min)
        double hist_limits[4];		// (x_low, x_high, y_low, y_high)
        TObjArray *output_graphs;

        // Setup the histograms with arbitrary binning (to be defined later)
        TH2D *chi2_2D;
        TH2D *n_entries_2D;
        TH1D * chi2_1D;

    };
}
#endif

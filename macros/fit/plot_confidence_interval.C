//
// ------------------------------------------------------------------------------------------------
// Plots constant chi^2 contours in 1D or 2D from root files output from run-fit.
//
// Francis Bench
// ------------------------------------------------------------------------------------------------
//

void plot_confidence_interval()
{
    using namespace valor;
	using namespace valor::conv;
	
	PhysParamId_t param_x = kPhypMixing23;		// Choose either kPhypMixing23, kPhypMixing13, kPhypMixing12, kPhypCPV, kPhypDm23sq, kPhypDm12sq or kPhypUndefined
	PhysParamId_t param_y = kPhypDm23sq;		// Set to kPhypUndefined for 1D contours. Input a different phys param ID (see above) to get 2D contours
	bool avg_or_min_delta_chi2 = true;          // If true, uses the mean delta chi^2 value for each point (averaged across runs). If false, uses the lowest delta chi^2.
    bool find_delta_chi2 = true;         		// If true, makes delta chi^2 contours instead of chi^2 contours.
	
	// 2D:
	ConfidenceIntervalPlotter *plotter = new ConfidenceIntervalPlotter(param_x, param_y, avg_or_min_delta_chi2, find_delta_chi2, "Confidence Intervals;sin^{2}(#theta_{23});#Delta m^{2}_{23} (eV^{2} c^{-4})");

	plotter->SetChiSqLineLabel("FastMC FHC NH - 90% CL");					// Set options to be used in the drawing of the next added contour (optional)
	plotter->Add("/hepstore/fbench/data/2016-12-22/*.root", "90 %");	// Add the data to the ConfidenceIntervalPlotter and specify the confidence limit. This generates the contour

	plotter->SetChiSqLineStyle(2);
	plotter->SetChiSqLineLabel("FastMC FHC NH - 2#sigma CL");
	plotter->Add("/hepstore/fbench/data/2016-12-22/*.root", "2 sigma");

	plotter->SetChiSqLineStyle(1);
	plotter->SetChiSqLineColor(2);
	plotter->SetBestFitColor(2);
	plotter->SetChiSqLineLabel("FastMC FHC IH - 90% CL");
	plotter->Add("/hepstore/fbench/data/2016-12-22-a/*.root", "90 %");

	plotter->SetChiSqLineStyle(2);
	plotter->SetChiSqLineLabel("FastMC FHC IH - 2#sigma CL");
	plotter->Add("/hepstore/fbench/data/2016-12-22-a/*.root", "2 sigma");

	TCanvas *c1 = new TCanvas("c1", "2D Confidence Limits", 800, 600);
	//plotter->SetRangeManually(0.01, -1.0, 0.03, 1.5);		// Optional. Do this before drawing
	plotter->Draw(c1);										// Draws all of the added confidence intervals (& the Delta chi^2 histgram if it's a 1D plot)
	plotter->frame->SetTitleSize(0.05, "Y");
	plotter->frame->SetTitleSize(0.05, "X");
	plotter->frame->SetTitleOffset(1.2, "Y");
	plotter->frame->SetLabelSize(0.04, "Y");
	plotter->frame->SetLabelSize(0.04, "X");
	TGaxis::SetMaxDigits(3);
	c1->Update();


	// 1D:
	param_y = kPhypUndefined;	// Setting param_y to kPhypUndefined is required to obtain 1D confidence intervals
	ConfidenceIntervalPlotter *plotter2 = new ConfidenceIntervalPlotter(param_x, param_y, avg_or_min_delta_chi2, find_delta_chi2, "Confidence Intervals;sin^{2}(#theta_{23});#Delta#chi^{2}");

	plotter2->SetDataLineColor(1);
	plotter2->SetChiSqLineStyle(2);
	plotter2->SetChiSqLineLabel("90 % CL");
	plotter2->SetDataLineLabel("FastMC FHC NH");
	plotter2->Add("/hepstore/fbench/data/2016-12-22/*.root", "90 %");

	plotter2->SetChiSqLineColor(2);
	plotter2->SetChiSqLineLabel("2#sigma CL");
	plotter2->Add("/hepstore/fbench/data/2016-12-22/*.root", "2 sigma");

	plotter2->SetChiSqLineColor(1);
	plotter2->SetDataLineColor(3);
	plotter2->SetChiSqLineLabel("90 % CL");
	plotter2->SetDataLineLabel("FastMC FHC IH");
	plotter2->Add("/hepstore/fbench/data/2016-12-22-a/*.root", "90 %");

	TCanvas *c2 = new TCanvas("c2", "1D Confidence Limits", 800, 600);
	plotter2->Draw(c2);


	//____________________________________________________________________________________________
	// sin^2(theta13) and deltaCP graphs
	// 2D:
	param_x = kPhypMixing13;
	param_y = kPhypCPV;
	// /*
	ConfidenceIntervalPlotter *plotter3 = new ConfidenceIntervalPlotter(param_x, param_y, avg_or_min_delta_chi2, find_delta_chi2, "Confidence Intervals;sin^{2}(#theta_{13});#delta_{CP} (radians)");

	plotter3->SetChiSqLineLabel("FastMC FHC NH - 90% CL");
	plotter3->Add("/hepstore/fbench/data/2016-11-30/*.root", "90 %");

	plotter3->SetChiSqLineStyle(2);
	plotter3->SetChiSqLineLabel("FastMC FHC NH - 2#sigma CL");
	plotter3->Add("/hepstore/fbench/data/2016-11-30/*.root", "2 sigma");

	TCanvas *c3 = new TCanvas("c3", "2D Confidence Limits", 800, 600);
	plotter3->Draw(c3);
	// */


	// 1D:
	// /*
	param_y = kPhypUndefined;
	ConfidenceIntervalPlotter *plotter4 = new ConfidenceIntervalPlotter(param_x, param_y, avg_or_min_delta_chi2, find_delta_chi2, "Confidence Intervals;sin^{2}(#theta_{13});#Delta#chi^{2}");
	plotter4->SetDataLineColor(1);
	plotter4->SetChiSqLineStyle(2);
	plotter4->SetChiSqLineLabel("90 % CL");
	plotter4->SetDataLineLabel("FastMC FHC NH");
	plotter4->Add("/hepstore/fbench/data/2016-11-30/*.root", "90 %");
	
	plotter4->SetChiSqLineColor(2);
	plotter4->SetChiSqLineLabel("2#sigma CL");
	plotter4->Add("/hepstore/fbench/data/2016-11-30/*.root", "2 sigma");
	
	TCanvas *c4 = new TCanvas("c4", "1D Confidence Limits", 800, 600);
	plotter4->SetRangeManually(0.0, 0.0, 0.06, 50.0);
	plotter4->Draw(c4);
	// */
}

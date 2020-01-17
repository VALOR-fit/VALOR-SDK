//____________________________________________________________________________
/*!

\class     valor::FitResult

\brief     A class to hold the results from a fit.

           Collects together the best fit point, likelihood and chi2 values,
           along with the likelihood and chi2 surfaces.

\author    Andy Chappell <A.Chappell \at warwick.ac.uk>
           University of Warwick

\created   January 31, 2019

\copyright (c) 2010-2019, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_fit_result_h_
#define _valor_fit_result_h_

#include "TH1.h"
#include "TH2.h"

#include <vector>
using std::vector;

namespace valor
{

class FitResult
{
public:
   /**
      Constructs a FitResult object.

      Stores the basic results of a 1D fit without likelihood and chi2 surfaces.

      \param param The value of the fit parameter at the best-fit.
      \param chi2 The chi2 value at the best-fit.
      \param likelihood The likelihood at the best-fit.
   */
   FitResult(const double param, double chi2, double likelihood);

   /**
      Constructs a FitResult object.

      Stores the results of a 1D fit from likelihood and chi2 surfaces.

      \param chi2 The chi2 surface from the best-fit.
      \param likelihood The likelihood surface from the best-fit.
   */
   FitResult(const TH1D& chi2, const TH1D& likelihood, vector<double> * toy_likelihoods);

   /**
      Constructs a FitResult object.

      Stores the basic results of a 2D fit without likelihood and chi2 surfaces.

      \param param1 The value of the first fit parameter at the best-fit.
      \param param1 The value of the second fit parameter at the best-fit.
      \param chi2 The chi2 value at the best-fit.
      \param likelihood The likelihood at the best-fit.
   */
   FitResult(const double param1, const double param2, double chi2,
         double likelihood);

   /**
      Constructs a FitResult object.

      Stores the results of a 2D fit from likelihood and chi2 surfaces.

      \param chi2 The chi2 surface from the best-fit.
      \param likelihood The likelihood surface from the best-fit.
   */
   FitResult(const TH2D& chi2, const TH2D& likelihood);

   /**
      Destructor.
   */
   virtual ~FitResult();

   /**
      Retrieve the first best-fit parameter.

      \return The first best-fit parameter.
   */
   double GetParam1BestFit();

   /**
      Retrieve the second best-fit parameter.

      \return The second best-fit parameter.
   */
   double GetParam2BestFit();

   /**
      Retrieve the chi2 value at the best-fit point.

      \return The chi2 value at the best-fit.
   */
   double GetChi2BestFit();

   /**
      Retrieve the likelihood value at the best-fit point.

      \return The likelihood value at the best-fit parameter.
   */
   double GetLikelihoodBestFit();

   /**
      Retrieve the 1D chi2 surface.

      This function will exit if the surface doesn't exist, that is, if the fit
      was 2D, or if the FitResult was initialised with only the basic statistics
      and parameter values.

      \return The chi2 surface.
   */
   TH1D* GetChi2Surface1D();

   /**
      Retrieve the 1D likelihood surface.

      This function will exit if the surface doesn't exist, that is, if the fit
      was 2D, or if the FitResult was initialised with only the basic statistics
      and parameter values.

      \return The likelihood surface.
   */
   TH1D* GetLikelihoodSurface1D();

   /**
      Retrieve the 2D chi2 surface.

      This function will exit if the surface doesn't exist, that is, if the fit
      was 1D, or if the FitResult was initialised with only the basic statistics
      and parameter values.

      \return The chi2 surface.
   */
   TH2D* GetChi2Surface2D();

   /**
      Retrieve the 2D likelihood surface.

      This function will exit if the surface doesn't exist, that is, if the fit
      was 1D, or if the FitResult was initialised with only the basic statistics
      and parameter values.

      \return The likelihood surface.
   */
   TH2D* GetLikelihoodSurface2D();

   /**
      Retrieve a vector containing the chisq value for each marginalisation toy.

      \return The likelihood surface.
   */
   vector<double> * GetToyLikelihoods() { return _toy_likelihoods; }

private:
   double _param1;
   double _param2;
   double _likelihood;
   double _chi2;
   bool _isFit2D;
   TH1D* _likelihood_1D;
   TH2D* _likelihood_2D;
   TH1D* _chi2_1D;
   TH2D* _chi2_2D;
   vector<double> * _toy_likelihoods;
};

} // valor

#endif


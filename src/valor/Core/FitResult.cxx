#include "valor/Core/FitResult.h"

#include <limits>
#include <sstream>
#include <string>

#include "valor/Messenger/Messenger.h"

namespace valor
{

FitResult::FitResult(const double param, double chi2,
      double likelihood):
   _param1(param), _param2(0.0), _likelihood(likelihood), _chi2(chi2),
   _isFit2D(false), _likelihood_1D(nullptr), _likelihood_2D(nullptr),
   _chi2_1D(nullptr), _chi2_2D(nullptr)
{
}
//______________________________________________________________________________
FitResult::FitResult(const TH1D& chi2, const TH1D& likelihood):
   _param2(0.0), _isFit2D(false), _likelihood_2D(nullptr), _chi2_2D(nullptr)
{
   std::ostringstream oss;
   oss << likelihood.GetName() << "_fit";
   this->_likelihood_1D = static_cast<TH1D*>(likelihood.Clone(oss.str().c_str()));
   this->_likelihood_1D->SetDirectory(0);

   oss.str("");
   oss.clear();
   oss << chi2.GetName() << "_fit";
   this->_chi2_1D = static_cast<TH1D*>(chi2.Clone(oss.str().c_str()));
   this->_chi2_1D->SetDirectory(0);
   // "Fix" unpopulated bins - chi2 bins are initialised to
   // std::numeric_limits<double>::max(), which root cannot plot. When running
   // an optimised fit that does not populate the full chi2 surface, these
   // values are left behind in the histogram, so here we adjust them to be
   // equal to 1 greater than the largest chi2 value found in the fit.
   int N = this->_chi2_1D->GetNbinsX();
   // Find the largest "real" chi2 value
   double chi2Max = 0.0;
   for(int i = 1; i <= N; ++i)
   {
      double chi2Val = this->_chi2_1D->GetBinContent(i);
      if(chi2Val != std::numeric_limits<double>::max() && chi2Val > chi2Max)
      {
         chi2Max = chi2Val;
      }
   }
   // Replace std::numeric_limits<double>::max() with the new value
   for(int i = 1; i <= N; ++i)
   {
      if(this->_chi2_1D->GetBinContent(i) == std::numeric_limits<double>::max())
      {
         this->_chi2_1D->SetBinContent(i, 1.0 + chi2Max);
      }
   }

   int bin_bestfit = this->_chi2_1D->GetMinimumBin();
   this->_chi2 = this->_chi2_1D->GetBinContent(bin_bestfit);
   this->_likelihood = this->_likelihood_1D->GetBinContent(bin_bestfit);
   this->_param1 = this->_chi2_1D->GetXaxis()->GetBinCenter(bin_bestfit);
}
//______________________________________________________________________________
FitResult::FitResult(const double param1, const double param2,
      double chi2, double likelihood):
   _param1(param1), _param2(param2), _likelihood(likelihood), _chi2(chi2),
   _isFit2D(true), _likelihood_1D(nullptr), _likelihood_2D(nullptr),
   _chi2_1D(nullptr), _chi2_2D(nullptr)
{
}
//______________________________________________________________________________
FitResult::FitResult(const TH2D& chi2, const TH2D& likelihood):
   _isFit2D(true), _likelihood_1D(nullptr), _chi2_1D(nullptr)
{
   std::ostringstream oss;
   oss << likelihood.GetName() << "_fit";
   this->_likelihood_2D = static_cast<TH2D*>(likelihood.Clone(oss.str().c_str()));
   this->_likelihood_2D->SetDirectory(0);

   oss.str("");
   oss.clear();
   oss << chi2.GetName() << "_fit";
   this->_chi2_2D = static_cast<TH2D*>(chi2.Clone(oss.str().c_str()));
   this->_chi2_2D->SetDirectory(0);
   // "Fix" unpopulated bins - chi2 bins are initialised to
   // std::numeric_limits<double>::max(), which root cannot plot. When running
   // an optimised fit that does not populate the full chi2 surface, these
   // values are left behind in the histogram, so here we adjust them to be
   // equal to 1 greater than the largest chi2 value found in the fit.
   int Nx = this->_chi2_2D->GetNbinsX();
   int Ny = this->_chi2_2D->GetNbinsY();
   // Find the largest "real" chi2 value
   double chi2Max = 0.0;
   for(int i = 1; i <= Nx; ++i)
   {
      for(int j = 1; j <= Ny; ++j)
      {
         double chi2Val = this->_chi2_2D->GetBinContent(i, j);
         if(chi2Val != std::numeric_limits<double>::max() && chi2Val > chi2Max)
         {
            chi2Max = chi2Val;
         }
      }
   }
   // Replace std::numeric_limits<double>::max() with the new value
   for(int i = 1; i <= Nx; ++i)
   {
      for(int j = 1; j <= Ny; ++j)
      {
         if(this->_chi2_2D->GetBinContent(i, j) ==
               std::numeric_limits<double>::max())
         {
            this->_chi2_2D->SetBinContent(i, j, 1.0 + chi2Max);
         }
      }
   }


   int x_bin_bestfit; int y_bin_bestfit; int dummy;
   this->_chi2_2D->GetMinimumBin(x_bin_bestfit, y_bin_bestfit, dummy);
   this->_chi2 = this->_chi2_2D->GetBinContent(x_bin_bestfit, y_bin_bestfit);
   this->_likelihood = this->_likelihood_2D->GetBinContent(x_bin_bestfit,
         y_bin_bestfit);
   this->_param1 = this->_chi2_2D->GetXaxis()->GetBinCenter(x_bin_bestfit);
   this->_param2 = this->_chi2_2D->GetYaxis()->GetBinCenter(y_bin_bestfit);
}
//______________________________________________________________________________
FitResult::~FitResult()
{
   delete _likelihood_1D;
   delete _likelihood_2D;
   delete _chi2_1D;
   delete _chi2_2D;
}
//______________________________________________________________________________
double FitResult::GetParam1BestFit()
{
   return this->_param1;
}
//______________________________________________________________________________
double FitResult::GetParam2BestFit()
{
   if(!this->_isFit2D)
   {
      VLOG("FitResult", pERROR) << "Requested the second parameter from a 1D " <<
         "FitResult";
      exit(1);
   }

   return this->_param2;
}
//______________________________________________________________________________
double FitResult::GetChi2BestFit()
{
   return this->_chi2;
}
//______________________________________________________________________________
double FitResult::GetLikelihoodBestFit()
{
   return this->_likelihood;
}
//______________________________________________________________________________
TH1D* FitResult::GetChi2Surface1D()
{
   if(this->_isFit2D)
   {
      VLOG("FitResult", pERROR) << "Requested a 1D chi2 surface from a 2D " <<
         "FitResult";
      exit(1);
   }
   else if(this->_chi2_1D == nullptr)
   {
      VLOG("FitResult", pERROR) << "Requested a chi2 surface, but none was " <<
         "provided on initialisation of the FitResult";
   }

   return this->_chi2_1D;
}
//______________________________________________________________________________
TH1D* FitResult::GetLikelihoodSurface1D()
{
   if(this->_isFit2D)
   {
      VLOG("FitResult", pERROR) << "Requested a 1D likelihood surface from a " <<
         "2D FitResult";
      exit(1);
   }
   else if(this->_likelihood_1D == nullptr)
   {
      VLOG("FitResult", pERROR) << "Requested a likelihood surface, but none " <<
         "was provided on initialisation of the FitResult";
   }

   return this->_likelihood_1D;
}
//______________________________________________________________________________
TH2D* FitResult::GetChi2Surface2D()
{
   if(!this->_isFit2D)
   {
      VLOG("FitResult", pERROR) << "Requested a 2D chi2 surface from a 1D " <<
         "FitResult";
      exit(1);
   }
   else if(this->_chi2_2D == nullptr)
   {
      VLOG("FitResult", pERROR) << "Requested a chi2 surface, but none was " <<
         "provided on initialisation of the FitResult";
   }

   return this->_chi2_2D;

}
//______________________________________________________________________________
TH2D* FitResult::GetLikelihoodSurface2D()
{
   if(!this->_isFit2D)
   {
      VLOG("FitResult", pERROR) << "Requested a 2D likelihood surface from a " <<
         "1D FitResult";
      exit(1);
   }
   else if(this->_likelihood_2D == nullptr)
   {
      VLOG("FitResult", pERROR) << "Requested a likelihood surface, but none " <<
         "was provided on initialisation of the FitResult";
   }

   return this->_likelihood_2D;

}

} // valor


#include "valor/FitCore/FitResult.h"

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
      LOG("FitResult", pERROR) << "Requested the second parameter from a 1D " <<
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
      LOG("FitResult", pERROR) << "Requested a 1D chi2 surface from a 2D " <<
         "FitResult";
      exit(1);
   }
   else if(this->_chi2_1D == nullptr)
   {
      LOG("FitResult", pERROR) << "Requested a chi2 surface, but none was " <<
         "provided on initialisation of the FitResult";
   }

   return this->_chi2_1D;
}
//______________________________________________________________________________
TH1D* FitResult::GetLikelihoodSurface1D()
{
   if(this->_isFit2D)
   {
      LOG("FitResult", pERROR) << "Requested a 1D likelihood surface from a " <<
         "2D FitResult";
      exit(1);
   }
   else if(this->_likelihood_1D == nullptr)
   {
      LOG("FitResult", pERROR) << "Requested a likelihood surface, but none " <<
         "was provided on initialisation of the FitResult";
   }

   return this->_likelihood_1D;
}
//______________________________________________________________________________
TH2D* FitResult::GetChi2Surface2D()
{
   if(!this->_isFit2D)
   {
      LOG("FitResult", pERROR) << "Requested a 2D chi2 surface from a 1D " <<
         "FitResult";
      exit(1);
   }
   else if(this->_chi2_2D == nullptr)
   {
      LOG("FitResult", pERROR) << "Requested a chi2 surface, but none was " <<
         "provided on initialisation of the FitResult";
   }

   return this->_chi2_2D;

}
//______________________________________________________________________________
TH2D* FitResult::GetLikelihoodSurface2D()
{
   if(!this->_isFit2D)
   {
      LOG("FitResult", pERROR) << "Requested a 2D likelihood surface from a " <<
         "1D FitResult";
      exit(1);
   }
   else if(this->_likelihood_2D == nullptr)
   {
      LOG("FitResult", pERROR) << "Requested a likelihood surface, but none " <<
         "was provided on initialisation of the FitResult";
   }

   return this->_likelihood_2D;

}

} // valor


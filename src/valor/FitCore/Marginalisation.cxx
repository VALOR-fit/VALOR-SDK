#include "valor/FitCore/Marginalisation.h"

#include <limits>
#include <sstream>
#include <string>
#include "TMath.h"

#include "valor/Messenger/Messenger.h"
#include "valor/Conventions/Constants.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/UtlStr.h"
#include "valor/Utilities/ToyStream.h"
#include "valor/FitCore/UtlOscillation.h"
#include "valor/Conventions/PhysParamId.h"


//#define VALOR_EXPT_LOW_LEVEL_MESG_ENABLED

namespace valor
{

Marginalisation::Marginalisation(CompoundFitElements& elements, string filePrefix,  bool use_simple_binning,
                                 bool save_throws_to_tree, string va_format):
   Marginalisation(*elements.FitModel(), filePrefix, use_simple_binning, save_throws_to_tree, va_format)
{
}
//______________________________________________________________________________
Marginalisation::Marginalisation(FitModelABC& model, string filePrefix, bool use_simple_binning,
                                 bool save_throws_to_tree, string va_format):
   _filePrefix(filePrefix), _model(&model), _likelihood(0.0), _chi2(0.0),
   _isFit2D(false), _likelihood_1D(nullptr), _likelihood_2D(nullptr),
   _chi2_1D(nullptr), _chi2_2D(nullptr), _fitResult(nullptr),
   _use_simple_binning(use_simple_binning),
   _param1_start_bin(0), _param2_start_bin(-1), _va_format(va_format)
{
}
//______________________________________________________________________________
Marginalisation::~Marginalisation()
{
   this->_model = nullptr;
   delete this->_likelihood_1D;
   delete this->_likelihood_2D;
   delete this->_chi2_1D;
   delete this->_chi2_2D;
   delete this->_fitResult;
}
//______________________________________________________________________________
void Marginalisation::ComputeStatistics(const int fitParamBin, FitDataABC* data,
      const int Ntoys, const int Nbatches, const int firstBatch)
{
   this->ComputeStatistics(fitParamBin, -1, data, Ntoys, Nbatches, firstBatch);
}
//______________________________________________________________________________
void Marginalisation::ComputeStatistics(const int fitParam1Bin,
      const int fitParam2Bin, FitDataABC* data, const int Ntoys,
      const int Nbatches, const int firstBatch)
{
   const int lastBatch = firstBatch + Nbatches - 1;
   const int N = Nbatches * Ntoys;
   vector<double> vec_chi2_toy(N);  // Index constructed (ibatch - firstBatch) * Ntoys + itoy
   std::string read_opt = _va_format;
   if (_use_simple_binning) { read_opt += "+SB"; }
   for(int b = firstBatch; b <= lastBatch; ++b)
   {
      std::ostringstream oss;
      oss << this->_filePrefix << "_" << _param1_start_bin + fitParam1Bin;
      if(fitParam2Bin > -1)
      {  // File name for 2D fit
         oss << "_v_" << _param2_start_bin + fitParam2Bin;
      }
      oss << "_" << b << "_" << Ntoys << ".bin";
      std::string filename = oss.str();
      LOG("Marginalisation", pINFO) << "Filename: " << filename;

      util::ToyStream* toy_stream = this->_model->InitToyStream(filename, "read", read_opt);
      for(int t = 0; t < Ntoys; ++t)
      {
         // Read the chi2 of each toy-MC experiment into a vector
#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
         LOG("Marginalisation", pDEBUG) << "Reading toy-MC data";
#endif
         this->_model->Read(toy_stream);
         double chi2 = data->GetFitStatistic(this->_model, "likelihood-ratio");
         vec_chi2_toy[(b - firstBatch) * Ntoys + t] = chi2;

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
         LOG("Marginalisation", pNOTICE) << "Marg toy #" << t << " event rates:\n" << this->_model->GetPDFIntegralAsString();
#endif

#ifdef VALOR_EXPT_LOW_LEVEL_MESG_ENABLED
         LOG("Marginalisation", pDEBUG) << "chi2(" << t << "): " << chi2 <<
            " L(" << t << "): " << exp(-chi2/2.);
#endif
      }
      delete toy_stream;
   }

   // Now sort the vector of chi2 values in descending order. This is so that the contributions from tiny
   // likelihood values are summed (and could become significant), instead of being lost when added to relatively
   // large likelihood values. (Note: rbegin/rend is the reverse beginning/end, i.e. the end/beginning).
   std::sort(vec_chi2_toy.rbegin(),vec_chi2_toy.rend());

   // Use the median chisq as a scale factor to guard against precision issues
   double median_index = (vec_chi2_toy.size() + 1) / 2.;
   double diff_chi2 = (vec_chi2_toy[(int)std::floor(median_index)] + vec_chi2_toy[(int)std::ceil(median_index)]) / 2.;
   double scale = diff_chi2;
   if(diff_chi2 > _scale_precision) scale = _scale_precision;
   scale = TMath::Min(_scale_precision, scale);

   // Calculate the scaled likelihood sum
   double likelihood_sum = 0.0;
   for (std::vector<double>::iterator it=vec_chi2_toy.begin(); it!=vec_chi2_toy.end(); ++it){
      double arg = -*it/2. + scale;
      likelihood_sum += exp(arg) / N;
   }
   this->_chi2 = 2*scale - 2*log(likelihood_sum);  // The scale cancels out at this stage of the calculation, so this is the un-scaled chi2
   this->_likelihood = exp(-this->_chi2/2.);
}

void Marginalisation::InitFit(const int param1Nbins, const int param1StartBin,
      const double param1MinValue, const double param1MaxValue,
      const int param2Nbins, const int param2StartBin,
      const double param2MinValue, const double param2MaxValue)
{
   if(param2Nbins > 0)
   {
      this->_isFit2D = true;
      double step = 0.0;
      if(param1Nbins > 1) { step = (param1MaxValue - param1MinValue) / (param1Nbins - 1); }
      double shift = 0.5 * step;
      double xMin = param1MinValue - shift;
      double xMax = param1MaxValue + shift;
      step = 0.0;
      if(param2Nbins > 1) { step = (param2MaxValue - param2MinValue) / (param2Nbins - 1); }
      shift = 0.5 * step;
      double yMin = param2MinValue - shift;
      double yMax = param2MaxValue + shift;
      this->_likelihood_2D = new TH2D("likelihood_2d", "Likelihood 2D",
            param1Nbins, xMin, xMax, param2Nbins, yMin, yMax);
      this->_chi2_2D = new TH2D("chi2_2d", "#chi^{2} 2D", param1Nbins, xMin,
            xMax, param2Nbins, yMin, yMax);

      // Initialise chi2 grid to maximum value - likelihood initialised to zero
      // by default
      for(int i = 1; i <= param1Nbins; ++i)
      {
         for(int j = 1; j <= param2Nbins; ++j)
         {
            this->_chi2_2D->SetBinContent(i, j, std::numeric_limits<double>::max());
         }
      }
      _param1_start_bin = param1StartBin;
      _param2_start_bin = param2StartBin;
   }
   else
   {
      this->_isFit2D = false;
      double step = 0.0;
      if(param1Nbins > 1) { step = (param1MaxValue - param1MinValue) / (param1Nbins - 1); }
      double shift = 0.5 * step;
      double xMin = param1MinValue - shift;
      double xMax = param1MaxValue + shift;
      this->_likelihood_1D = new TH1D("likelihood_1d", "Likelihood 1D",
            param1Nbins, xMin, xMax);
      this->_chi2_1D = new TH1D("chi2_1d", "#chi^{2} 1D", param1Nbins, xMin,
            xMax);

      // Initialise chi2 grid to maximum value - likelihood initialised to zero
      // by default
      for(int i = 1; i <= param1Nbins; ++i)
      {
         this->_chi2_1D->SetBinContent(i, std::numeric_limits<double>::max());
      }
      _param1_start_bin = param1StartBin;
   }
}

FitResult* Marginalisation::Fit(FitDataABC* data, const int Ntoys, const int Nbatches, util::oscillation::PenaltyTerm penaltyset, const int firstBatch)
{
    if(this->_isFit2D)
    {
        const int Nx =  this->_chi2_2D->GetXaxis()->GetNbins();
        const int Ny =  this->_chi2_2D->GetYaxis()->GetNbins();
        // Get the likelihood and chi2 statistics for the entire grid
        for(int i = 0; i < Nx; ++i)
        {
            for(int j = 0; j < Ny; ++j)
            {
                this->ComputeStatistics(i, j, data, Ntoys, Nbatches, firstBatch);
                this->_likelihood_2D->SetBinContent(i + 1, j + 1, this->_likelihood);
                if (penaltyset.param_names != ""){
                    this->CalcPenaltyTerm(this->_chi2_2D->GetXaxis()->GetBinCenter(i + 1), penaltyset);
                    this->_chi2_2D->SetBinContent(i + 1, j + 1, this->_chi2 + this->_penaltyterm);
                }
                else{
                    this->_chi2_2D->SetBinContent(i + 1, j + 1, this->_chi2);
                }
            }
        }

        // Store the result of the fit
        this->_fitResult = new FitResult(*this->_chi2_2D, *this->_likelihood_2D);

        return this->_fitResult;
    }
    else{
        const int N =  this->_chi2_1D->GetXaxis()->GetNbins();
        // Get the likelihood and chi2 statistics for the entire grid
        for(int i = 0; i < N; ++i)
        {
            this->ComputeStatistics(i, data, Ntoys, Nbatches, firstBatch);
            this->_likelihood_1D->SetBinContent(i + 1, this->_likelihood);

            if (penaltyset.param_names != ""){
                this->CalcPenaltyTerm(this->_chi2_1D->GetBinCenter(i + 1), penaltyset);
                this->_chi2_1D->SetBinContent(i + 1, this->_chi2 + this->_penaltyterm);
            }
            else{
                this->_chi2_1D->SetBinContent(i + 1, this->_chi2);
            }
        }
        // Store the result of the fit
        this->_fitResult = new FitResult(*this->_chi2_1D, *this->_likelihood_1D);
        return this->_fitResult;
    }
}

FitResult* Marginalisation::Fit(const double initValue, FitDataABC* data,
      const int Ntoys, const int Nbatches, const int firstBatch)
{
   double value = initValue;
   int binInit = this->_chi2_1D->FindBin(value);
   const int N =  this->_chi2_1D->GetXaxis()->GetNbins();
   bool minimised = false;

   while(!minimised)
   {
      // Get the likelihood and chi2 statistics for the local grid (3x3 region
      // around the current bin)
      int binMin = CheckLocalMinimum(binInit, 3, N, data, Ntoys, Nbatches,
            firstBatch);
      if(binMin == binInit)
      {  // This is the (very) local minimum, check a larger region to see if
         // this is likely to be the true global minimum
         binMin = CheckLocalMinimum(binInit, 7, N, data, Ntoys, Nbatches,
               firstBatch);
         if(binMin == binInit)
         {  // Global minimum probably found, we're done
            minimised = true;
         }
         else
         {  // Update central bin and iterate, minimum not found.
            binInit = binMin;
         }
      }
      else
      {  // Update central bin and iterate, minimum not found.
         binInit = binMin;
      }
   }

   // Store the result of the fit
   this->_fitResult = new FitResult(*this->_chi2_1D, *this->_likelihood_1D);

   return this->_fitResult;
}

FitResult* Marginalisation::Fit(const double initValue1,
      const double initValue2, FitDataABC* data, const int Ntoys,
      const int Nbatches, const int firstBatch)
{
   double valueX = initValue1;
   double valueY = initValue2;
   int binInitX = this->_chi2_2D->GetXaxis()->FindBin(valueX);
   int binInitY = this->_chi2_2D->GetYaxis()->FindBin(valueY);
   const int Nx =  this->_chi2_2D->GetXaxis()->GetNbins();
   const int Ny =  this->_chi2_2D->GetYaxis()->GetNbins();
   bool minimised = false;

   while(!minimised)
   {
      // Get the likelihood and chi2 statistics for the local grid (3x3 region
      // around the current bin)
      int binMinX = binInitX;
      int binMinY = binInitY;
      CheckLocalMinimum(binMinX, binMinY, 3, Nx, Ny, data, Ntoys, Nbatches,
            firstBatch);
      if(binMinX == binInitX && binMinY == binInitY)
      {  // This is the (very) local minimum, check a larger region to see if
         // this is likely to be the true global minimum
         CheckLocalMinimum(binMinX, binMinY, 7, Nx, Ny, data, Ntoys, Nbatches,
               firstBatch);
         if(binMinX == binInitX && binMinY == binInitY)
         {  // Global minimum probably found, we're done
            minimised = true;
         }
         else
         {  // Update central bin and iterate, minimum not found.
            binInitX = binMinX;
            binInitY = binMinY;
         }
      }
      else
      {  // Update central bin and iterate, minimum not found.
         binInitX = binMinX;
         binInitY = binMinY;
      }
   }

   // Store the result of the fit
   this->_fitResult = new FitResult(*this->_chi2_2D, *this->_likelihood_2D);

   return this->_fitResult;
}

int Marginalisation::CheckLocalMinimum(const int binInit, const int windowSize,
      const int gridSize, FitDataABC* data, const int Ntoys, const int Nbatches,
      const int firstBatch)
{
   int binMin = binInit;
   const int limit = (windowSize - 1) / 2;
   double chi2Min = std::numeric_limits<double>::max();
   for(int di = -limit; di <= limit; ++di)
   {
      int i = binInit + di;
      // Check still within fit bounds
      if(i < 1 || i > gridSize)
      {
         continue;
      }
      // Check if the chi2 value has already been loaded
      if(this->_likelihood_1D->GetBinContent(i) == 0.0)
      {  // Load the chi2 value from file
         this->ComputeStatistics(i - 1, data, Ntoys, Nbatches, firstBatch);
         this->_likelihood_1D->SetBinContent(i, this->_likelihood);
         this->_chi2_1D->SetBinContent(i, this->_chi2);
      }
      else
      {  // Load the chi2 value from the existing surface
         this->_chi2 = this->_chi2_1D->GetBinContent(i);
      }
      if(this->_chi2 < chi2Min)
      {
         chi2Min = this->_chi2;
         binMin = i;
      }
   }

   return binMin;
}

void Marginalisation::CheckLocalMinimum(int& binX, int& binY,
      const int windowSize, const int gridSizeX, const int gridSizeY,
      FitDataABC* data, const int Ntoys, const int Nbatches,
      const int firstBatch)
{
   int binMinX = binX;
   int binMinY = binY;
   const int limit = (windowSize - 1) / 2;
   double chi2Min = std::numeric_limits<double>::max();
   for(int di = -limit; di <= limit; ++di)
   {
      int i = binX + di;
      // Check still within fit bounds
      if(i < 1 || i > gridSizeX)
      {
         continue;
      }
      for(int dj = -limit; dj <= limit; ++dj)
      {
         int j = binY + dj;
         // Check still within fit bounds
         if(j < 1 || j > gridSizeY)
         {
            continue;
         }
         // Check if the chi2 value has already been loaded
         if(this->_likelihood_2D->GetBinContent(i, j) == 0.0)
         {  // Load the chi2 value from file
            this->ComputeStatistics(i - 1, j - 1, data, Ntoys, Nbatches,
                  firstBatch);
            this->_likelihood_2D->SetBinContent(i, j, this->_likelihood);
            this->_chi2_2D->SetBinContent(i, j, this->_chi2);
         }
         else
         {  // Load the chi2 value from the existing surface
            this->_chi2 = this->_chi2_2D->GetBinContent(i, j);
         }
         if(this->_chi2 < chi2Min)
         {
            chi2Min = this->_chi2;
            binMinX = i;
            binMinY = j;
         }
      }
   }

   // Set the location of the minimum bin in the reference variables
   binX = binMinX;
   binY = binMinY;
}

void Marginalisation::CalcPenaltyTerm(const double par0,util::oscillation::PenaltyTerm penaltyset){
    double fpenalty_mean0  = penaltyset.mean ;
    double fpenalty_sigma0 = penaltyset.sigma;
    std::string penalty_scheme = penaltyset.scheme;
    std::string parameter_scheme_name_p0 = ""; // Leave this as an empty string to allow util::ConvertOscParamToUsedScheme to set the target scheme according to the values set in Opt::Instance
    conv::PhysParamId_t par0_id = util::NameRegistry::Instance()->PhysParamFromString(penaltyset.param_names);
    
    //Convert parameter value to teh same scheme as penalty
    double parval0 = util::ConvertOscParamToUsedScheme(par0_id, par0 , parameter_scheme_name_p0, penalty_scheme);

    double penalty0 = (parval0 - fpenalty_mean0)*(parval0 - fpenalty_mean0) /  ( fpenalty_sigma0 * fpenalty_sigma0);
  
    this->_penaltyterm = penalty0; 

}
} // valor


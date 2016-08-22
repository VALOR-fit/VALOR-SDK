#include "valor/Marginalisation/MCMarginaliser.h"
#include "valor/Messenger/Messenger.h"
#include "valor/FitCore/FitDataABC.h"
#include "valor/FitCore/FitModelABC.h"
#include "valor/FitCore/FitParamList.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/NameRegistry.h"

namespace valor
{
namespace marginalisation
{

MCMarginaliser::MCMarginaliser(FitInputs* fitInputs):
   fNumToys(0), fNumDimensions(1), fParam0(conv::kPhypUndefined),
   fParam1(conv::kPhypUndefined), fFitInputs(fitInputs), fLikelihood(0)
{
}

MCMarginaliser::MCMarginaliser(const MCMarginaliser& original):
   fNumToys(original.fNumToys), fNumDimensions(original.fNumDimensions),
   fParam0(original.fParam0), fParam1(original.fParam1),
   fFitInputs(original.fFitInputs)
{
   fLikelihood = new VALORArrayD(*original.fLikelihood);
}

MCMarginaliser::~MCMarginaliser()
{
   if(fLikelihood)
   {
      delete fLikelihood;
   }
}

double MCMarginaliser::Marginalise()
{
   LOG("MCMarginaliser", pNOTICE) << "Performing " << fNumDimensions << "D fit.";
   if(fNumDimensions == 1)
   {
      return Marginalise1D();
   }
   else
   {
      return Marginalise2D();
   }
}

double MCMarginaliser::Marginalise1D()
{
   FitDataABC& data = fFitInputs->Observation();
   FitModelABC& model = fFitInputs->Expectation();
   FitParamList& fp = fFitInputs->FitParams();

   double minValue = fp.MinValue(fParam0);
   double maxValue = fp.MaxValue(fParam0);
   // Quick hack for testing purposes
   double stepSize = (maxValue - minValue) / 1.0;
   // double step_size = fitInputs->FitParams().StepSize(fParam0);

   fp.SetCurrentValue(fParam0, minValue);
   Rnd::Instance()->SetSeeds(*fArg);
   data.Generate(&model, fp, false);

   for(int i = 0; i < fp.Size(); ++i)
   {
       conv::PhysParamId_t p = fp.WhichParam(i);
      if(p == fParam0)
      {
         fp.SetNuisance(p, false);
         fp.FreeParameter(p);
      }
      else
      {
         // Need to determine appropriate combination for free/fixed and
         // nuisance/marginalisation parameters
         fp.SetNuisance(p, true);
         fp.SetDoMarg(p, true);
         fp.FixParameter(p);
      }
  }

   int numBins = 1 + static_cast<int>((maxValue - minValue) / stepSize);
   fLikelihood = new VALORArray(numBins);
   fLikelihood->SetAll(0.0);
   int i = 0;
   for(double value = minValue; value <= maxValue; value += stepSize, ++i)
   {
      // Ensure that each set of toys is equivalent to the last set
      Rnd::Instance()->SetSeeds(*fArg);
      fp.SetCurrentValue(fParam0, value);
      // Marginalise for this value
      double totalLikelihood = 0.0;
      for(int n = 0; n < fNumToys; ++n)
      {
         RandomizeNuisance();
         LOG("MCMarginaliser", pDEBUG) << "Toy " << n;
         int ndof = data.NumOfBins() - fp.GetNumberFreeParameters();
         double chi2 = data.GetFitStatistic(&model, fp, NULL);
         totalLikelihood += exp(-chi2 / 2);
         LOG("MCMarginaliser", pDEBUG) << "Chi2[" << ndof << "] (" << n <<
            ": " << value << ") = " << chi2 << "   L = " << totalLikelihood;
      }
      fLikelihood->SetEntry(totalLikelihood / fNumToys, i);
      LOG("MCMarginaliser", pNOTICE) << "L(" << value << ") = " <<
         (totalLikelihood / fNumToys);
   }

   return 0.0;
}

double MCMarginaliser::Marginalise2D()
{
   FitDataABC& data = fFitInputs->Observation();
   FitModelABC& model = fFitInputs->Expectation();
   FitParamList& fp = fFitInputs->FitParams();

   double minValue0 = fp.MinValueByIndex(fParam0);
   double maxValue0 = fp.MaxValueByIndex(fParam0);
   double stepSize0 = fp.StepSizeByIndex(fParam0);
   double minValue1 = fp.MinValueByIndex(fParam1);
   double maxValue1 = fp.MaxValueByIndex(fParam1);
   double stepSize1 = fp.StepSizeByIndex(fParam1);

   fp.SetCurrentValue(fParam0, minValue0);
   fp.SetCurrentValue(fParam1, minValue1);
   Rnd::Instance()->SetSeeds(*fArg);
   data.Generate(&model, fp);

   for(int i = 0; i < fp.Size(); ++i)
   {
      conv::PhysParamId_t p = fp.WhichParam(i);
      if(p == fParam0 || p == fParam1)
      {
         fp.SetNuisance(p, false);
         fp.FreeParameter(p);
      }
      else
      {
         // Need to determine appropriate combination for free/fixed and
         // nuisance/marginalisation parameters
         fp.SetNuisance(p, true);
         fp.SetDoMarg(p, true);
         fp.FixParameter(p);
      }
   }

   for(double value0 = minValue0; value0 <= maxValue0; value0 += stepSize0)
   {
      fp.SetCurrentValue(fParam0, value0);
      for(double value1 = minValue1; value1 <= maxValue1; value1 += stepSize1)
      {
         // Ensure that each set of toys is equivalent to the last set
         Rnd::Instance()->SetSeeds(*fArg);
         fp.SetCurrentValue(fParam1, value1);
         // Marginalise for this value
         double totLikelihood = 0.0;
         for(int i = 0; i < fNumToys; ++i)
         {
            RandomizeNuisance();
            totLikelihood += exp(-data.GetFitStatistic(&model, fp, NULL) / 2);
         }
         double likelihood = totLikelihood / fNumToys;
         LOG("MCMarginaliser", pNOTICE) << "Likelihood (" << value0 << "," <<
            value1 << ") = " << likelihood;
      }
   }

   return 0.0;
}

void MCMarginaliser::Configure(const CmdLnArgList& arg)
{
   fArg = &arg;
   fNumToys = arg.GetInt("num-marginalisation-toys");
   if(fNumToys <= 0)
   {
      LOG("MCMarginaliser", pERROR) <<
         "Number of marginalisation toys must be positive";   
   }

   fParam0 = GetPhysParamId(arg.GetString("fit-param-0"));
   fParam1 = GetPhysParamId(arg.GetString("fit-param-1"));
   fNumDimensions = (fParam1 == conv::kPhypUndefined) ? 1 : 2;
}

void MCMarginaliser::RandomizeNuisance()
{
   FitModelABC& model = fFitInputs->Expectation();
   FitParamList& fp = fFitInputs->FitParams();
   TString options(fArg->GetString("param-values-option"));

   // Loop until all parameters are within physical limits
   do
   {
      model.ResetSystematics("all");
      model.RandomizeSystematics(options.Data());
      PhysParamList tweaks = model.Tweaks();
      fp.FromPhysParamList(tweaks, "current");
   }
   while(!AllParametersWithinBounds());
   do
   {
      RandomizeOscillation();
   }
   while(!AllParametersWithinBounds());
}

void MCMarginaliser::RandomizeOscillation()
{
   FitParamList& fp = fFitInputs->FitParams();
   for(int i = 0; i < fp.Size(); ++i)
   {
      conv::PhysParamId_t p = fp.WhichParam(i);
      if(p!= fParam0 && p != fParam1)
      {
         if(!util::IsSystematic(p) && !util::IsNormalization(p))
         {
            std::string prior = fp.GetPriorModeName(p);
            if(prior == "flat")
            {
               double min = fp.GetMinValueFlat(p);
               double max = fp.GetMaxValueFlat(p);
               double val = Rnd::Instance()->RndToyMCosc().Uniform(min, max);
               fp.SetCurrentValue(p, val);
            }
            else if(prior == "gaus")
            {
               double mean = fp.GetMeanGaus(p);
               double sigma = fp.GetSigmaGaus(p);
               double val = Rnd::Instance()->RndToyMCosc().Gaus(mean, sigma);
               fp.SetCurrentValue(p, val);
            }
            else if(prior == "user")
            {
               double val = fp.GetPriorHisto(p)->GetRandom();
               fp.SetCurrentValue(p, val);
            }
            else
            {
               LOG("MCMarginaliser", pFATAL) << "Undefined prior for " <<
                  util::NameRegistry::Instance()->AsString(p) << ": " << prior;
               exit(1);
            }
         }
      }
   }
}

conv::PhysParamId_t MCMarginaliser::GetPhysParamId(const std::string& name)
{
   if(name == "NONE")
   {
      return conv::kPhypUndefined;
   }
   else if(name == "mixing23Bar")
   {
      return conv::kPhypMixing23Bar;
   }
   else if(name=="mixing13Bar")
   {
      return conv::kPhypMixing13Bar;
   }
   else if(name=="dmsq23Bar")
   {
      return conv::kPhypDm23sqBar;
   }
   else if(name=="dmsq12Bar")
   {
      return conv::kPhypDm12sqBar;
   }
   else if(name=="mixing12Bar")
   {
      return conv::kPhypMixing12Bar;
   }
   else if(name=="deltacpBar")
   {
      return conv::kPhypCPVBar;
   }
   else if(name=="betaBar")
   {
      return conv::kPhypBetaBar;
   }
   else if(name=="mixing23")
   {
      return conv::kPhypMixing23;
   }
   else if(name=="mixing13")
   {
      return conv::kPhypMixing13;
   }
   else if(name=="dmsq23")
   {
      return conv::kPhypDm23sq;
   }
   else if(name=="dmsq12")
   {
      return conv::kPhypDm12sq;
   }
   else if(name=="mixing12")
   {
      return conv::kPhypMixing12;
   }
   else if(name=="deltacp")
   {
      return conv::kPhypCPV;
   }
   else
   {
      LOG("MCMarginaliser", pERROR) << "Parameter \'" << name <<
         "\' does not exist";
      valor::util::Die(-1);
   }

   return conv::kPhypUndefined;
}

bool MCMarginaliser::AllParametersWithinBounds()
{
   FitParamList& fp = fFitInputs->FitParams();
   for(int i = 0; i < fp.Size(); ++i)
   {
      conv::PhysParamId_t p = fp.WhichParam(i);
      if(p != fParam0 && p != fParam1)
      {
         if(fp.CurrValue(p) > fp.MaxValue(p) ||
               fp.CurrValue(p) < fp.MinValue(p))
         {
/*            LOG("MCMarginaliser", pNOTICE) <<
               util::NameRegistry::Instance()->AsString(p) <<
               " out of bounds. Has value " << fp.CurrValue(p) <<
               " require " << fp.MinValue(p) << "-" << fp.MaxValue(p);*/
            return false;
         }
      }
   }

   return true;
} 

}
}

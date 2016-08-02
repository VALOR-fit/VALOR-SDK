#include "valor/Marginalisation/MCMarginaliser.h"
#include "valor/Messenger/Messenger.h"
#include "valor/FitCore/FitDataABC.h"
#include "valor/FitCore/FitModelABC.h"
#include "valor/FitCore/FitParamList.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/UtlSystem.h"

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
   //data.Generate(&model, fp); 

   double minValue = fp.MinValue(fParam0);
   double maxValue = fp.MaxValue(fParam0);
   // Quick hack for testing purposes
   double stepSize = (maxValue - minValue) / 4.0;
   // double step_size = fitInputs->FitParams().StepSize(fParam0);

   int numBins = 1 + static_cast<int>((maxValue - minValue) / stepSize);
   fLikelihood = new VALORArray(numBins);
   fLikelihood->SetAll(0.0);
   int i = 0;
   for(double value = minValue; value <= maxValue; value += stepSize, ++i)
   {
      RandomizeNuisance();
      fp.SetCurrentValue(fParam0, value);
      // Marginalise for this value
      double totalLikelihood = 0.0;
      for(int n = 0; n < fNumToys; ++n)
      {
         double chi2 = data.GetFitStatistic(&model, fp, NULL);
         LOG("valor_dune", pNOTICE) << "Chi2 (" << n << ": " << value <<
            ") = " << chi2;
         totalLikelihood += exp(-chi2 / 2);
      }
      fLikelihood->SetEntry(totalLikelihood / fNumToys, i);
   }

   return 0.0;
}

double MCMarginaliser::Marginalise2D()
{
   FitDataABC& data = fFitInputs->Observation();
   FitModelABC& model = fFitInputs->Expectation();
   FitParamList& fp = fFitInputs->FitParams();
   data.Generate(&model, fp);

   double minValue0 = fp.MinValueByIndex(fParam0);
   double maxValue0 = fp.MaxValueByIndex(fParam0);
   double stepSize0 = fp.StepSizeByIndex(fParam0);
   double minValue1 = fp.MinValueByIndex(fParam1);
   double maxValue1 = fp.MaxValueByIndex(fParam1);
   double stepSize1 = fp.StepSizeByIndex(fParam1);

   for(double value0 = minValue0; value0 <= maxValue0; value0 += stepSize0)
   {
      fp.SetCurrentValue(fParam0, value0);
      for(double value1 = minValue1; value1 <= maxValue1; value1 += stepSize1)
      {
         RandomizeNuisance();
         fp.SetCurrentValue(fParam1, value1);
         // Marginalise for this value
         double totLikelihood = 0.0;
         for(int i = 0; i < fNumToys; ++i)
         {
            totLikelihood += exp(-data.GetFitStatistic(&model, fp, NULL) / 2);
         }
         double likelihood = totLikelihood / fNumToys;
         LOG("valor_dune", pNOTICE) << "Likelihood (" << value0 << "," <<
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
      LOG("valor_dune", pERROR) <<
         "Number of marginalisation toys must be positive";   
   }

   fParam0 = GetPhysParamId(arg.GetString("fit-param-0"));
   fParam1 = GetPhysParamId(arg.GetString("fit-param-1"));
   fNumDimensions = (fParam1 == conv::kPhypUndefined) ? 1 : 2;
}

void MCMarginaliser::RandomizeNuisance()
{
   Rnd::Instance()->SetSeeds(*fArg);

   FitModelABC& model = fFitInputs->Expectation();
   FitParamList& fp = fFitInputs->FitParams();
   TString options(fArg->GetString("param-values-option"));

   // Loop until all parameters are within physical limits
   // Currently the loop is short-circuited to assume all is well
   do
   {
      model.ResetSystematics("all");
      model.RandomizeSystematics(options.Data());
      PhysParamList tweaks = model.Tweaks();
      fp.FromPhysParamList(tweaks, "current");
   }
   while(false);
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
      LOG("valor_dune", pNOTICE) << "Parameter \'" << name <<
         "\' does not exist";
      valor::util::Die(-1);
   }

   return conv::kPhypUndefined;
}

}
}

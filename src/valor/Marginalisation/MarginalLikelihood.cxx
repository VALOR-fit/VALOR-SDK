#include "valor/Marginalisation/MarginalLikelihood.h"
#include "valor/Marginalisation/MCMarginaliser.h"
#include "valor/Messenger/Messenger.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/NameRegistry.h"

#include "TFile.h"
#include "TH1D.h"
#include "TMath.h"
#include "TObjArray.h"

namespace valor
{
namespace marginalisation
{

MarginalLikelihood::MarginalLikelihood(MarginaliserABC* marginaliser) :
   LikelihoodABC(), fMarginaliser(marginaliser), computedMarginalValues(false),
   _validationFilename(""), _validate(false)
{
}

MarginalLikelihood::~MarginalLikelihood()
{
/*   std::for_each(marginalValues.begin(), marginalValues.end,
         marginalValueDeleter());*/
   marginalValues.clear();
}

double MarginalLikelihood::Likelihood(FitDataABC* data, FitParamList* params)
{
   MCMarginaliser* marginaliser = dynamic_cast<MCMarginaliser*>(fMarginaliser);
   assert(marginaliser);
   std::vector<double> chi2Values;

   if(!computedMarginalValues)
   {
      PreCalcMarginalValues(*params);
   }

   TObjArray array;

   // Want to build an object holding the tuples of marginal values
   // and then iterate over this object to retrieve for each toy
   // rather than iterator over several vectors and indexing
   int i = 0;
   FitModelABC* m = marginaliser->Begin();
   for( ; m != marginaliser->End(); )
   {
      map<conv::PhysParamId_t, std::vector<double>*>::iterator v =
         marginalValues.begin();
      for( ;v != marginalValues.end(); ++v)
      {
         conv::PhysParamId_t p = v->first;
         std::vector<double>* randvals = v->second;
         params->SetInitValue(p, randvals->at(i));
      }
      PhysHypothesis hypothesis(*params);

      // Consider adding unoscillated spectra to validation file here
      if(_validate)
      {
         // Generate the final PDFs without applying oscillation
         m->Generate(hypothesis, "");
         TString tag = TString::Format("toy_%d_unosc", i);
         m->Write(&array, tag.Data(), "pdft_rf");
      }

      // Apply oscillation to the marginalisation toy
      m->Generate(hypothesis, "+OSC");
      // Calculate the likelihood
      double chi2 = data->GetFitStatistic(m, NULL);
      std::cout << "Fit statistic toy " << (i + 1) << " " << chi2 << std::endl;
      chi2Values.push_back(chi2);

      if(_validate)
      {
         TString tag = TString::Format("toy_%d_osc", i);
         m->Write(&array, tag.Data(), "pdft_rf");
      }

      FitModelABC* next = marginaliser->Next();
      if(_validate && !next)
      {
         TFile file(_validationFilename.c_str(), "RECREATE");
         array.Write("", TObject::kOverwrite);
         file.Close();
      }
      m = next;
      ++i;
   }

   std::sort(chi2Values.begin(), chi2Values.end());
   double avgChi2 = 0.5 * (chi2Values[0] + chi2Values[chi2Values.size() - 1]);
   double halfAvgChi2 = 0.5 * avgChi2;
   double scale = halfAvgChi2 <= PRECISION_SCALE ? halfAvgChi2 : PRECISION_SCALE;
   double totalLikelihood = 0;
   i = 0;
   for(std::vector<double>::iterator it = chi2Values.begin();
         it != chi2Values.end(); ++it, i++)
   {
      double likelihood = exp(-0.5 * *it + scale);
      totalLikelihood += likelihood;
   }

   double averageLikelihood = totalLikelihood / marginaliser->GetNumToys();
   double lnLikelihood = 2 * scale - 2 * log(averageLikelihood);
   FitParamList* free = params->FreeParameterSubset();
   LOG("MarginalLikelihood", pNOTICE) << "FP: " << free->CurrValueByIndex(0) <<
      " L: " << averageLikelihood << " -2lnL: " << lnLikelihood;

   delete free;

   return lnLikelihood;
}

void MarginalLikelihood::EnableValidation(std::string filename)
{
   _validate = true;
   _validationFilename = filename;
}

void MarginalLikelihood::PreCalcMarginalValues(FitParamList& fp)
{
   MCMarginaliser* marginaliser = dynamic_cast<MCMarginaliser*>(fMarginaliser);
   for(int i = 0; i < fp.Size(); ++i)
   {
      conv::PhysParamId_t p = fp.WhichParam(i);
      if(fp.IsMarginalized(p))
      {
         marginalValues.insert(std::map<conv::PhysParamId_t,
               std::vector<double>*>::value_type(p, new std::vector<double>()));
         std::string prior = fp.GetPriorModeName(p);
         if(prior == "flat")
         {
            double min = fp.GetMinValueFlat(p);
            double max = fp.GetMaxValueFlat(p);
            for(FitModelABC* m = marginaliser->Begin(); m != marginaliser->End();
                  m = marginaliser->Next())
            {
               double val = Rnd::Instance()->RndToyMCosc().Uniform(min, max);
               marginalValues[p]->push_back(val);
            }
         }
         else if(prior == "gaus")
         {
            double mean = fp.GetMeanGaus(p);
            double sigma = fp.GetSigmaGaus(p);
            for(FitModelABC* m = marginaliser->Begin(); m != marginaliser->End();
                  m = marginaliser->Next())
            {
               double val = Rnd::Instance()->RndToyMCosc().Gaus(mean, sigma);
               marginalValues[p]->push_back(val);
            }
         }
         else if(prior == "user")
         {
            for(FitModelABC* m = marginaliser->Begin(); m != marginaliser->End();
                  m = marginaliser->Next())
            {
               double val = fp.GetPriorHisto(p)->GetRandom();
               marginalValues[p]->push_back(val);
            }
         }
         else
         {
            LOG("MarginalLikelihood", pERROR) << "Undefined prior for " <<
               util::NameRegistry::Instance()->AsString(p) << ": " << prior;
            exit(1);
         }
      }
   }
   computedMarginalValues = true;
}

}
}

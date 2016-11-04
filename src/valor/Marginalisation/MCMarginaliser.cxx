#include "valor/Marginalisation/MCMarginaliser.h"
#include "valor/Messenger/Messenger.h"
#include "valor/FitCore/FitDataABC.h"
#include "valor/FitCore/FitModelABC.h"
#include "valor/FitCore/FitParamList.h"
#include "valor/Utilities/Rnd.h"
#include "valor/Utilities/UtlSystem.h"
#include "valor/Utilities/NameRegistry.h"
#include "valor/Utilities/XMLConfig.h"

// Temp include - can remove when generate doesn't depend on TMath.Pi()
#include "TMath.h"

namespace valor
{
namespace marginalisation
{

MCMarginaliser::MCMarginaliser(FitDataABC* data, FitModelABC* model,
      FitParamList* parameters):
   fNumToys(0), fNumDimensions(1), fParam0(conv::kPhypUndefined),
   fParam1(conv::kPhypUndefined), fData(data), fModel(model),
   fParams(parameters)
{
}

MCMarginaliser::~MCMarginaliser()
{
   std::vector<FitModelABC*>::iterator it = fToys.begin();
   while(it != fToys.end())
   {
      delete *it;
      it = fToys.erase(it);
   }
}

void MCMarginaliser::Marginalise(FitDataABC* data, FitModelABC* model)
{
   Rnd::Instance()->SetSeeds(*fArg);
   TString tag(fArg->GetString("tag"));

   for(int n = 0; n < fNumToys; ++n)
   {
      LOG("MCMarginaliser", pNOTICE) << "Generating toy " << (n + 1) <<
         " of " << fNumToys;
      //RandomizeNuisance();
      model->GenerateUnoscillated(*fParams, "+SB");

      // Temporary in memory storage
      fToys.push_back(model);
      TString filename = TString::Format("%s_%d.root", tag.Data(), n);
      model->Write(filename.Data());

      // Write the xml configuration asscoiated with this compound fit model to
      // file
      std::string filestr(filename.Data());
      valor::util::XMLConfig xmlconf(filestr);
      TXMLEngine* xml = xmlconf.GetXMLEngine();
      XMLNodePointer_t root = xml->NewChild(NULL, NULL, "analysis_inputs");
      xml->DocSetRootElement(xmlconf.GetXMLDocument(), root);
      model->WriteXML(xmlconf);
      xml->SaveDoc(xmlconf.GetXMLDocument(),
            xmlconf.GetMasterFilename().c_str());
   }
}

std::vector<FitModelABC*>::iterator MCMarginaliser::Begin()
{
   return fToys.begin();
}

std::vector<FitModelABC*>::iterator MCMarginaliser::End()
{
   return fToys.end();
}

int MCMarginaliser::GetNumToys() const
{
   return fNumToys;
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
   // Temporary hack to set systematic as free parameter
//   LOG("MCMarginaliser", pWARN) << "Hacked free parameter used";
//   fParam0 = fParams->WhichParam(159);
   fParam1 = GetPhysParamId(arg.GetString("fit-param-1"));
   fNumDimensions = (fParam1 == conv::kPhypUndefined) ? 1 : 2;
}

void MCMarginaliser::RandomizeNuisance()
{
   TString options(fArg->GetString("param-values-option"));

   // Loop until all parameters are within physical limits
   do
   {
      fModel->ResetSystematics("all");
      fModel->RandomizeSystematics(options.Data());
      PhysParamList tweaks = fModel->Tweaks();
      fParams->FromPhysParamList(tweaks, "current");
//      RandomizeOscillation();
   }
   while(!AllParametersWithinBounds());
}

void MCMarginaliser::RandomizeOscillation()
{
   for(int i = 0; i < fParams->Size(); ++i)
   {
      conv::PhysParamId_t p = fParams->WhichParam(i);
      if(p!= fParam0 && p != fParam1)
      {
         if(!util::IsSystematic(p) && !util::IsNormalization(p))
         {
            std::string prior = fParams->GetPriorModeName(p);
            if(prior == "flat")
            {
               double min = fParams->GetMinValueFlat(p);
               double max = fParams->GetMaxValueFlat(p);
               double val = Rnd::Instance()->RndToyMCosc().Uniform(min, max);
               fParams->SetCurrentValue(p, val);
            }
            else if(prior == "gaus")
            {
               double mean = fParams->GetMeanGaus(p);
               double sigma = fParams->GetSigmaGaus(p);
               double val = Rnd::Instance()->RndToyMCosc().Gaus(mean, sigma);
               fParams->SetCurrentValue(p, val);
            }
            else if(prior == "user")
            {
               double val = fParams->GetPriorHisto(p)->GetRandom();
               fParams->SetCurrentValue(p, val);
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
   for(int i = 0; i < fParams->Size(); ++i)
   {
      conv::PhysParamId_t p = fParams->WhichParam(i);
      if(p != fParam0 && p != fParam1)
      {
         if(fParams->CurrValue(p) > fParams->MaxValue(p) ||
               fParams->CurrValue(p) < fParams->MinValue(p))
         {
            LOG("MCMarginaliser", pDEBUG) << "Nuisance " <<
               util::NameRegistry::Instance()->AsString(p) << " = " <<
               fParams->CurrValue(p) << " from [" <<
               fParams->MinValue(p) << "," <<
               fParams->MaxValue(p)  << "]";
            return false;
         }
      }
   }

   return true;
} 

}
}

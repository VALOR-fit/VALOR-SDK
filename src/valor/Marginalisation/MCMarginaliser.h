#ifndef MCMARGINALISER_H
#define MCMARGINALISER_H

#include "valor/Conventions/PhysParamId.h"
#include "valor/Marginalisation/MarginaliserABC.h"
#include "valor/FitCore/FitDataABC.h"
#include "valor/FitCore/FitModelABC.h"
#include "valor/FitCore/FitParamList.h"
#include "valor/VArray/VALORArrayD.h"

namespace valor
{
namespace marginalisation
{
   class MCMarginaliser : public MarginaliserABC
   {
   public:
      MCMarginaliser(FitDataABC* data, FitModelABC* model,
            FitParamList* parameters);
      virtual ~MCMarginaliser();

      /**
         Marginalises over nuisance parameters.
      */
      virtual void Marginalise();

      /**
         Marginalises over nuisance parameters.

         \param data
            The data against which the fit should be performed.
         \param model
            The model over which to marginalise.
      */
      virtual void Marginalise(FitDataABC* data, FitModelABC* model);

      /*
         Retrieves an iterator representing the beginning of the set of models.
      */
      virtual std::vector<FitModelABC*>::iterator Begin();

      /*
         Retrieves an interator representing the end of the set of models.
      */
      virtual std::vector<FitModelABC*>::iterator End();

      /**
         Retrieves the average likelihood calculated during a 1D
         marginalisation.

         \return The average likelihood over the marginalization toys.
      */
      virtual TH1D* GetAverageLikelihood1D();

      /**
         Retrieves the average likelihood calculated during a 2D
         marginalisation.

         \return The average likelihood over the marginalization toys.
      */
      virtual TH2D* GetAverageLikelihood2D();

      /**
         Returns the number of marginalisation toys.

         \return The number of marginalisation toys.
      */
      int GetNumToys() const;

      /**
         Configures the marginaliser.

         \param arg
            The CmdLnArgList from which configuration parameters are to be read.
      */
      virtual void Configure(const CmdLnArgList& arg);

      static conv::PhysParamId_t GetPhysParamId(const std::string& name);

   private:
      void RandomizeNuisance();
      void RandomizeOscillation();
      void MarginaliseNew();
      void Marginalise1D();
      void Marginalise2D();
      bool AllParametersWithinBounds();

   private:
      int fNumToys;
      int fNumDimensions;
      conv::PhysParamId_t fParam0;
      conv::PhysParamId_t fParam1;
      FitDataABC* fData;
      FitModelABC* fModel;
      FitParamList* fParams;
      VALORArrayD* fLikelihood;
      TH1D* fAvgLikelihood1D;
      TH2D* fAvgLikelihood2D;
      // This is temporary until the binary file read/write is working
      // Note that the Root dictionary does not correctly resolve FitModelABC
      // as belonging to the valor namespace when used as the template to a
      // vector within the valor::marginalisation namespace, so it must have its
      // namespace explicitly specified.
      std::vector<valor::FitModelABC*> fToys;
   };
}
}

#endif

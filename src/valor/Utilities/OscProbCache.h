#ifndef OSCPROBCACHE_H
#define OSCPROBCACHE_H

#include <map>
#include "valor/Oscillation/NuOsc3Flavour.h"

namespace valor
{
namespace util
{
   struct OscProbs
   {
      double ee;
      double emu;
      double etau;
      double mue;
      double mumu;
      double mutau;
      double taue;
      double taumu;
      double tautau;
   };

   class OscProbCache
   {
   public:
      virtual ~OscProbCache(){ };

      /**
         Gets the oscillation probability.

         \param model
            The model under which the probability is being calculated.
         \param isAntineutrino
            Whether the probability is being calculated for antineutrino mode.
         \param energy
            The true neutrino energy for which the probability is to be
            calculated.
         \param p
            The matrix into which the probabilities will be stored (3x3 for
            3 flavour).
      */
      static void GetOscProb(NuOsc3Flavour& model, bool isAntineutrino, double energy, double p[3][3]);

   private:
      OscProbCache(){ };
      static std::map<double, OscProbs> sOscProbs;
      static std::map<double, OscProbs> sOscProbsBar;
      static std::map<double, bool> sOscCached;
      static std::map<double, bool> sOscCachedBar;
   };
}
}

#endif

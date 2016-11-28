#include "valor/Utilities/OscProbCache.h"
#include "valor/Messenger/Messenger.h"
#include <cstring>

namespace valor
{
namespace util
{
   std::map<double, OscProbs> OscProbCache::sOscProbs;
   std::map<double, OscProbs> OscProbCache::sOscProbsBar;
   std::map<double, bool> OscProbCache::sOscCached;
   std::map<double, bool> OscProbCache::sOscCachedBar;

   void OscProbCache::GetOscProb(NuOsc3Flavour& model, bool isAntineutrino,
         double energy, double p[3][3])
   {
      if(!isAntineutrino)
      {
         if(sOscCached[energy])
         {
            OscProbs op = sOscProbs[energy];
            p[0][0] = op.ee;
            p[0][1] = op.emu;
            p[0][2] = op.etau;
            p[1][0] = op.mue;
            p[1][1] = op.mumu;
            p[1][2] = op.mutau;
            p[2][0] = op.taue;
            p[2][1] = op.taumu;
            p[2][2] = op.tautau;
         }
         else
         {
            OscProbs op;
            model.CalcMatterOscProbAll(isAntineutrino, energy, p);
            op.ee = p[0][0];
            op.emu = p[0][1];
            op.etau = p[0][2];
            op.mue = p[1][0];
            op.mumu = p[1][1];
            op.mutau = p[1][2];
            op.taue = p[2][0];
            op.taumu = p[2][1];
            op.tautau = p[2][2];
            sOscProbs[energy] = op;
            sOscCached[energy] = true;
         }
      }
      else
      {
         if(sOscCachedBar[energy])
         {
            OscProbs op = sOscProbsBar[energy];
            p[0][0] = op.ee;
            p[0][1] = op.emu;
            p[0][2] = op.etau;
            p[1][0] = op.mue;
            p[1][1] = op.mumu;
            p[1][2] = op.mutau;
            p[2][0] = op.taue;
            p[2][1] = op.taumu;
            p[2][2] = op.tautau;
         }
         else
         {
            OscProbs op;
            model.CalcMatterOscProbAll(isAntineutrino, energy, p);
            op.ee = p[0][0];
            op.emu = p[0][1];
            op.etau = p[0][2];
            op.mue = p[1][0];
            op.mumu = p[1][1];
            op.mutau = p[1][2];
            op.taue = p[2][0];
            op.taumu = p[2][1];
            op.tautau = p[2][2];
            sOscProbsBar[energy] = op;
            sOscCachedBar[energy] = true;
         }
      }
   }
}
}


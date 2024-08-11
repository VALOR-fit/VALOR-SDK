//____________________________________________________________________________
/*

Copyright (c) 2010-2020, The VALOR group. All rights reserved

*/
//____________________________________________________________________________

#include <cassert>

#include <TMinuit.h>
#include <TString.h>
#include <TMath.h>
#include "Math/FitMethodFunction.h"
#include "HFitInterface.h"

#include "valor/Messenger/Messenger.h"
#include "valor/Core/Likelihood.h"
#include "valor/Core/LikelihoodMinuit2.h"

using namespace valor;
using namespace valor::conv;

ClassImp(LikelihoodMinuit2)

//____________________________________________________________________________
LikelihoodMinuit2::LikelihoodMinuit2(Likelihood * likelihood) :
fLikelihood(likelihood),
fErrDef(0)
{

}
//____________________________________________________________________________
LikelihoodMinuit2::~LikelihoodMinuit2()
{

}
//____________________________________________________________________________
double LikelihoodMinuit2::operator() (const std::vector<double>& x) const
{
 this->Kernel()->FitParams().Sync(x);
 return this->Kernel()->Statistic("poisson");
}
//____________________________________________________________________________

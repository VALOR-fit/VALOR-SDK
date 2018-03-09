//____________________________________________________________________________
/*!

\class     valor::utils

\brief     Utilities for randomising oscillation parameters

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory

           Steve Dennis <sdennis@liverpool.ac.uk>
           University of Liverpool

           Francis Bench <fbench@liverpool.ac.uk>
           University of Liverpool

\created   Mar 9, 2018

\copyright (c) 2010-2018, The VALOR Fitting Group, All Rights Reserved

*/
//____________________________________________________________________________

#ifndef _valor_osc_utils_h_
#define _valor_osc_utils_h_

#include "valor/Conventions/ConfLevel.h"
#include "valor/Conventions/PhysParamId.h"

#include "TH1D.h"

namespace valor {
    namespace util {

        double RandomizeParameter(conv::PhysParamId_t p);

        double RandomizeMixing23 (void); ///< pull from gaussian with mean & sigma from global fit (mixing param is either sinsq2theta or sinsqtheta depending on config)
        double RandomizeMixing12 (void); ///< pull from gaussian with mean & sigma from global fit ( -//- )
        double RandomizeMixing13 (void); ///< uniform from 0 up to ~ small_factor x CHOOZ limit    ( -//- )
        double RandomizeCPV      (void); ///< uniform from -pi to +pi (or -1 to +1)
        double RandomizeDm12sq   (void); ///< pull from gaussian with mean & sigma from global fit
        double RandomizeDm23sq   (void); ///< pull from gaussian with mean & sigma from global fit

        double RandomizeMixing23Bar (void); //
        double RandomizeMixing12Bar (void); //
        double RandomizeMixing13Bar (void); //
        double RandomizeCPVBar      (void); //
        double RandomizeDm12sqBar   (void); //
        double RandomizeDm23sqBar   (void); //

        double RandomizeUser        (TH1D *hPrior); // Extract random from a prior
        int    RandomizeMH          (const double probNH);

        // NEW BETABAR
        double RandomizeBetaBar   (void); //
        //

        // NEW OSC PRIOR
        double FromSinsq2thToSinsqth(const double sinsq2th);
        double FromSinsqthToSinsq2th(const double sinsqth);
        //
    }
}

#endif
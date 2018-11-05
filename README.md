# The VALOR Software Development Kit (SDK)

VALOR SDK contains software **modules enabling the implementation** of the **VALOR neutrino oscillation fits** for sereval experiments (SBN, DUNE, T2K and HyperK). 

The SDK is a **highly-abstracted** neutrino oscillation fit. Specific applications built on top of the SDK can specify, almost entirely via XML configuration files:
- A list event samples to be included in the fit (each a specific combination of beam configuration, detector, and observed topology).
- The reconstructed kinematical distribution (variables and binning scheme) which is fit for each sample.
- The types of contributions (typically, corresponding to different generator-level MC-truth interaction modes) which are kept track for each fit distribution.
- The MC-truth kinematical distribution (variables and binning scheme) associated with each single reconstructed bin in the contribution of every mode to each fit reconstructed distribution. Together multi-dimensional distribution, for each mode of every sample, that spans both reconstructed and MC-truth kinematical variables is referred to as a MC template.
- The systematic parameters to be included in the fit.
- The prior errors for systematic parameters as well as input correlations between systematic parameters.
- A specification of how each systematic parameter affects a kinematical bin in each MC template (for each mode of every fit sample).
- A physics hypothesis (e.g. 3-flavour oscillations, 3+1 oscillations, etc) and a list of the parameters of interest, their ranges and external constraints, if any.

## Authors

* **Costas Andreopoulos**, *University of Liverpool & STFC/RAL*
* **Maria Antonova**, *IFIC Valencia*
* **Chistopher Barry**, *University of Liverpool*
* **Francis Bench**, *University of Liverpool*
* **Andrew Chappell**, *University of Warwick*
* **Thomas Dealtry**, *Lancaster University*
* **Steve Dennis**, *University of Liverpool*
* **Lorena Escudero**, *University of Cambridge*
* **Jaggar Henzerling**, *University of Liverpool*
* **Rhiannon Jones**, *University of Liverpool*
* **Nick Grant**, *University of Warwick*
* **Marco Roda**, *University of Liverpool*
* **Davide Sgalaberna**, *CERN*
* **Julia Tena Vidal**, *University of Liverpool*

Past authors:

* **Raj Shah**, *University of Oxford * STFC/RAL*

## Contact

Dr. **Costas Andreopoulos** < constantinos.andreopoulos \at cern.ch >
 
**University of Liverpool** | Faculty of Science & Engineering | School of Physical Sciences | Dept. of Physics <br>
*Oliver Lodge Lab 316, Liverpool L69 7ZE, UK (tel: +44-(0)1517-943201)* <br>
**U.K. Research & Innovation** | Science & Technology Facilities Council | Rutherford Appleton Lab | Particle Physics Dept. <br>
*Harwell Oxford Campus, R1 2.89, Oxfordshire OX11 0QX, UK (tel: +44-(0)1235-445091)* <br>

## Copyright

Copyright (c) 2010-2018, The VALOR Fitting Group. **All Rights Reserved.**

# The VALOR Software Development Kit (VALOR-SDK)

VALOR SDK contains software **modules enabling the implementation** of the **VALOR neutrino oscillation fits** for sereval experiments (SBN, DUNE, T2K and HyperK). 

The SDK is a **highly-abstracted** neutrino oscillation fit. Specific applications built on top of the SDK can specify, almost entirely via XML configuration files:
- A list event samples to be included in the fit (each a specific combination of beam configuration, detector, and observed topology).
- The reconstructed kinematical distribution (variables and binning scheme) which is fit for each sample.
- The list of contributions, or **modes** (typically, corresponding to different generator-level MC-truth interaction modes) which are explicitly kept track of for each fit distribution. An appropriately chosen selection of modes enables the correct application of all specified systematics and physics hypotheses.
- The MC-truth kinematical distribution (variables and binning scheme) associated with each single reconstructed bin in the contribution of every mode to each fit reconstructed distribution. An appropriately chosen selection of MC-truth kinematical variables enables the correct application of all specified systematics and physics hypotheses.Together multi-dimensional distribution, for each mode of every sample, that spans both reconstructed and MC-truth kinematical variables is referred to as a MC template.
- The systematic parameters to be included in the fit.
- The prior errors for systematic parameters as well as input correlations between systematic parameters.
- A specification of how each systematic parameter affects a kinematical bin in each MC template (for each mode of every fit sample).
- A physics hypothesis (e.g. 3-flavour oscillations, 3+1 oscillations, etc) and a list of the parameters of interest, their ranges and external constraints, if any.

## Copyright

Copyright (c) 2010-2019, The VALOR Fitting Group. **All Rights Reserved.**

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

Professor **Costas Andreopoulos** < constantinos.andreopoulos \at cern.ch >
 
**University of Liverpool** | Dept. of Physics <br>
*Oliver Lodge Lab 316, Liverpool L69 7ZE, UK* | *Tel: +44-(0)1517-943201* <br>
**STFC Rutherford Appleton Laboratory** | Particle Physics Dept. <br>
*Harwell Oxford Campus, R1 2.89, Oxfordshire OX11 0QX, UK* | *Tel: +44-(0)1235-445091* <br>


# Using the VALOR-SDK: An abbreviated User's Guide

## Configuration, installation and post-installation testing

External dependency setup:

- **colorgcc** (makes g++ output more readable)
   - To install colorgcc (optional), type: *shell%* <span style="color: red"> make color</span>
   - To uninstall, type : *shell%* <span style="color: red"> make colorclean</span>
- **python virtualenv, with ipython and numpy** (ensure support for the VALOR python scripts)
   - To install, type: *shell%* <span style="color: red"> make python</span>
   - To uninstall, type *shell%* <span style="color: red"> make pythonclean</span>

Configuring the VALOR-SDK:

*shell%* ./configure

Building the VALOR-SDK:

*shell%* make

## Setting up a neutrino fit

### How to specify a list event samples to be included in the fit 

### How to specify the binning scheme for the true and reconstructed kinematical distributions in the MC templates

### How to specify the list of true reaction models that are kept track of separately for each fit sample

### How to populate the VALOR MC templates using your analysis DSTs

### How to plot nominal event rate distributions and write out corresponding event rate tables

### How to specify the list of systematic parameters to be included in the fit.

### How to specify the action of each systematic parameter.

### How to specify prior constraints for systematic parameters.

### How to run a neutrino fit

### Format of the standard output VALOR fit file



# The VALOR Software Development Kit (VALOR-SDK)

VALOR SDK contains the core, expertiment-agnostic software modules enabling the implementation of the **VALOR neutrino oscillation analyses** on sereval experiments. 

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

Copyright (c) 2010-2023, The VALOR Fitting Group. **All Rights Reserved.**

## Authors

* **Costas Andreopoulos**, *University of Liverpool*
* **Maria Antonova**, *IFIC Valencia*
* **Thomas Dealtry**, *Lancaster University*
* **Rhiannon Jones**, *University of Sheffield*
* **Jaiden Parlone**, *University of Liverpool*
* **Davide Sgalaberna**, *ETHZ*
* **Beth Slater**, *University of Liverpool*

Past authors:

* **Dom Barker**, *University of Sheffield*
* **Chistopher Barry**, *University of Liverpool*
* **Francis Bench**, *University of Liverpool*
* **Andrew Chappell**, *University of Warwick*
* **Steve Dennis**, *University of Warwick & STFC/RAL; University of Liverpool; Universirt of Cambridge*
* **Lorena Escudero**, *University of Cambridge; IFIC Valencia*
* **Pablo Fernandez Menendez**, *IFIC Valencia*
* **Nick Grant**, *STFC/RAL; Lancaster University; University of Warwick*
* **Thomas Ham**, *University of Liverpool*
* **Glória de Sá Pereira**, *University of Liverpool*
* **Raj Shah**, *University of Oxford & STFC/RAL*

## Contact

Professor **Costas Andreopoulos** < c.andreopoulos \at cern.ch >
 
**University of Liverpool** | Dept. of Physics <br>
*Oliver Lodge Lab 316, Liverpool L69 7ZE, UK* | *Tel: +44-(0)1517-943201* <br>


# Using the VALOR-SDK

https://www.overleaf.com/read/ygmrxrjtmwyv





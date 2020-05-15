#
# Makefile for VALOR/DUNE apps
#
# Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
#

SHELL = /bin/sh
NAME = all
MAKEFILE = Makefile

# Include machine specific flags and locations (inc. files & libs)
#
include $(VALOR_FMWK)/make/Make.include

TGT = adaptive_mcmc \
      fitnt_conv \
      make_response_functions \
      make_table_systematic_effects \
      run_build_toys \
      run_fit \
      run_fit_marg \
      tweak_pdf_N \
      tweak_pdf_sigma_var \
      write_pdf \
      run_tests \
      batch_tests 

#      run_tests \ # Need to check why these are borking
#      batch_tests \

#      make_dummy_samples \

all: $(TGT)

# Generate GENIE response functions
#
make_response_functions: FORCE
ifdef GENIE
	$(CXX) $(CXXFLAGS) -c make_response_functions.cxx $(GINCLUDES)
	$(LD) $(LDFLAGS) make_response_functions.o $(GLIBRARIES) $(VALOR_GLIBRARIES) -o $(VALOR_FMWK)/bin/valor-make_response_functions
else
	@echo "Skipping make_response_functions due to no GENIE"
endif

# Throw a user-specified parameter set N times and output a table containing the mean, RMS and RMS/mean for all samples
#
make_table_systematic_effects: FORCE
	$(CXX) $(CXXFLAGS) -c make_table_systematic_effects.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) make_table_systematic_effects.o $(LIBRARIES) -o $(VALOR_FMWK)/bin/valor-make_table_systematic_effects

# Run a generic fit
#
run_fit: FORCE
	$(CXX) $(CXXFLAGS) -c run_fit.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) run_fit.o $(LIBRARIES) -o $(VALOR_FMWK)/bin/valor-run_fit

# Write out pdf
# 
write_pdf: FORCE
	$(CXX) $(CXXFLAGS) -c write_pdf.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) write_pdf.o $(LIBRARIES) -o $(VALOR_FMWK)/bin/valor-write_pdf
	
# Randomly throw the pdf t o make envelopes
# 
tweak_pdf_N: FORCE
	$(CXX) $(CXXFLAGS) -c tweak_pdf_N.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) tweak_pdf_N.o $(LIBRARIES) -o $(VALOR_FMWK)/bin/valor-tweak_pdf_N

# Make +/- 1 and 3 sigma variations for each param
#
tweak_pdf_sigma_var: FORCE
	$(CXX) $(CXXFLAGS) -c tweak_pdf_sigma_var.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) tweak_pdf_sigma_var.o $(LIBRARIES) -o $(VALOR_FMWK)/bin/valor-tweak_pdf_sigma_var

# Run an Adaptive Metropolis Hastings around the results of a fit
#
adaptive_mcmc: FORCE
	$(CXX) $(CXXFLAGS) -c adaptive_mcmc.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) adaptive_mcmc.o $(LIBRARIES) -o $(VALOR_FMWK)/bin/valor-adaptive_mcmc

# Pick info from the VALOR fit n-tuple and convert to bare ROOT format
#
fitnt_conv: FORCE
	$(CXX) $(CXXFLAGS) -c fitnt_conv.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) fitnt_conv.o $(LIBRARIES) -o $(VALOR_FMWK)/bin/valor-fitnt_conv

run_build_margtoys: FORCE
	$(CXX) $(CXXFLAGS) -c run_build_margtoys.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) run_build_margtoys.o $(LIBRARIES) -o $(VALOR_FMWK)/bin/valor-run_build_margtoys

run_fit_marg: FORCE
	$(CXX) $(CXXFLAGS) -c run_fit_marg.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) run_fit_marg.o $(LIBRARIES) -o $(VALOR_FMWK)/bin/valor-run_fit_marg

run_build_toys: FORCE
	$(CXX) $(CXXFLAGS) -c run_build_toys.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) run_build_toys.o $(LIBRARIES) -o $(VALOR_FMWK)/bin/valor-run_build_toys


make_dummy_samples: FORCE
	$(CXX) $(CXXFLAGS) -c make_dummy_samples.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) make_dummy_samples.o $(LIBRARIES) -o $(VALOR_FMWK)/bin/valor-make_dummy_samples

run_tests: FORCE
ifdef GENIE
	$(CXX) $(CXXFLAGS) -c run_tests.cxx $(GINCLUDES)
	$(LD) $(LDFLAGS) run_tests.o $(GLIBRARIES)  $(VALOR_GLIBRARIES) -o $(VALOR_FMWK)/bin/valor-run_tests
else
	@echo "Skipping run_tests due to no GENIE"
endif

batch_tests: FORCE
ifdef GENIE
	$(CXX) $(CXXFLAGS) -c batch_tests.cxx $(GINCLUDES)
	$(LD) $(LDFLAGS) batch_tests.o $(GLIBRARIES)  $(VALOR_GLIBRARIES) -o $(VALOR_FMWK)/bin/valor-batch_tests
else
	@echo "Skipping run_tests due to no GENIE"
endif

#################### CLEANING

purge: FORCE
	$(RM) *.o *~ core 

clean: FORCE
	$(RM) *.o *~ core
	$(RM) $(VALOR_FMWK)/bin/valor-make_response_functions
	$(RM) $(VALOR_FMWK)/bin/valor-make_table_systematic_effects
	$(RM) $(VALOR_FMWK)/bin/valor-run_fit
	$(RM) $(VALOR_FMWK)/bin/valor-write_pdf
	$(RM) $(VALOR_FMWK)/bin/valor-tweak_pdf_N
	$(RM) $(VALOR_FMWK)/bin/valor-tweak_pdf_sigma_var
	$(RM) $(VALOR_FMWK)/bin/valor-fitnt_conv
	$(RM) $(VALOR_FMWK)/bin/valor-adaptive_mcmc
	$(RM) $(VALOR_FMWK)/bin/valor-run_tests
	$(RM) $(VALOR_FMWK)/bin/valor-run_build_margtoys
	$(RM) $(VALOR_FMWK)/bin/valor-run_fit_marg
	$(RM) $(VALOR_FMWK)/bin/valor-make_dummy_samples
	$(RM) $(VALOR_FMWK)/bin/valor-batch_tests

distclean: FORCE
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-make_response_functions
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-make_table_systematic_effects
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-run_fit
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/write_pdf
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/tweak_pdf_N
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/tweak_pdf_sigma_var
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-fitnt_conv
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-adaptive_mcmc
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-run_tests
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-run_build_margtoys
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-run_fit_marg
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-make_dummy_samples
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-batch_tests

FORCE:
	
# DO NOT DELETE

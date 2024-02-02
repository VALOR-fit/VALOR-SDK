#
# Makefile for VALOR/SDK apps
#
# Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
#

SHELL = /bin/sh
NAME = all
MAKEFILE = Makefile

# Include machine specific flags and locations (inc. files & libs)
#
include $(VALOR_FMWK)/make/Make.include

TGT = adaptive_mcmc \
      fitnt_conv \
      make_table_systematic_effects \
      run_build_toys \
      run_fit \
      run_fit_marg \
      tweak_pdf_N \
      tweak_pdf_sigma_var \
      write_pdf \
      make_response_functions 
     # run_tests \
      batch_tests 

#      run_tests \ # Need to check why these are borking
#      batch_tests \

#      make_dummy_samples \

all: $(TGT)

# Generate GENIE response functions
#
make_response_functions: FORCE
ifeq ("YES","$(OPT_ENABLE_GENIE)")
	$(CXX) $(CXXFLAGS) -c make_response_functions.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) make_response_functions.o $(LIBRARIES) -o $(VALOR_FMWK)/bin/valor-make_response_functions
else
	@echo "Skipping make_response_functions due to no GENIE"
endif

# Throw a user-specified parameter set N times and output a table containing the mean, RMS and RMS/mean for all samples
#
make_table_systematic_effects: FORCE
ifeq ("YES","$(OPT_ENABLE_GENIE)")
	$(CXX) $(CXXFLAGS) -c make_table_systematic_effects.cxx $(GINCLUDES)
	$(LD) $(LDFLAGS) make_table_systematic_effects.o $(GLIBRARIES) $(VALOR_GLIBRARIES) -o $(VALOR_FMWK)/bin/valor-make_table_systematic_effects
else
	@echo "Skipping make_table_systematic_effects due to no GENIE"
endif

# Run a generic fit
#
run_fit: FORCE
ifeq ("YES","$(OPT_ENABLE_GENIE)")
	$(CXX) $(CXXFLAGS) -c run_fit.cxx $(GINCLUDES)
	$(LD) $(LDFLAGS) run_fit.o $(GLIBRARIES) $(VALOR_GLIBRARIES) -o $(VALOR_FMWK)/bin/valor-run_fit
else
	@echo "Skipping run_fit due to no GENIE"
endif

# Write out pdf
# 
write_pdf: FORCE
ifeq ("YES","$(OPT_ENABLE_GENIE)")
	$(CXX) $(CXXFLAGS) -c write_pdf.cxx $(GINCLUDES)
	$(LD) $(LDFLAGS) write_pdf.o $(GLIBRARIES) $(VALOR_GLIBRARIES) -o $(VALOR_FMWK)/bin/valor-write_pdf
else
	@echo "Skipping write_pdf due to no GENIE"
endif
	
# Randomly throw the pdf t o make envelopes
# 
tweak_pdf_N: FORCE
ifeq ("YES","$(OPT_ENABLE_GENIE)")
	$(CXX) $(CXXFLAGS) -c tweak_pdf_N.cxx $(GINCLUDES)
	$(LD) $(LDFLAGS) tweak_pdf_N.o $(GLIBRARIES) $(VALOR_GLIBRARIES) -o $(VALOR_FMWK)/bin/valor-tweak_pdf_N
else
	@echo "Skipping tweak_pdf_N due to no GENIE"
endif

# Make +/- 1 and 3 sigma variations for each param
#
tweak_pdf_sigma_var: FORCE
ifeq ("YES","$(OPT_ENABLE_GENIE)")
	$(CXX) $(CXXFLAGS) -c tweak_pdf_sigma_var.cxx $(GINCLUDES)
	$(LD) $(LDFLAGS) tweak_pdf_sigma_var.o $(GLIBRARIES) $(VALOR_GLIBRARIES) -o $(VALOR_FMWK)/bin/valor-tweak_pdf_sigma_var
else
	@echo "Skipping tweak_pdf_sigma_var due to no GENIE"
endif

# Run an Adaptive Metropolis Hastings around the results of a fit
#
adaptive_mcmc: FORCE
ifeq ("YES","$(OPT_ENABLE_GENIE)")
	$(CXX) $(CXXFLAGS) -c adaptive_mcmc.cxx $(GINCLUDES)
	$(LD) $(LDFLAGS) adaptive_mcmc.o $(GLIBRARIES) $(VALOR_GLIBRARIES) -o $(VALOR_FMWK)/bin/valor-adaptive_mcmc
else
	@echo "Skipping adaptive_mcmc due to no GENIE"
endif

# Pick info from the VALOR fit n-tuple and convert to bare ROOT format
#
fitnt_conv: FORCE
ifeq ("YES","$(OPT_ENABLE_GENIE)")
	$(CXX) $(CXXFLAGS) -c fitnt_conv.cxx $(GINCLUDES)
	$(LD) $(LDFLAGS) fitnt_conv.o $(GLIBRARIES) $(VALOR_GLIBRARIES) -o $(VALOR_FMWK)/bin/valor-fitnt_conv
else
	@echo "Skipping fitnt_conv due to no GENIE"
endif

run_build_margtoys: FORCE
ifeq ("YES","$(OPT_ENABLE_GENIE)")
	$(CXX) $(CXXFLAGS) -c run_build_margtoys.cxx $(GINCLUDES)
	$(LD) $(LDFLAGS) run_build_margtoys.o $(GLIBRARIES) $(VALOR_GLIBRARIES) -o $(VALOR_FMWK)/bin/valor-run_build_margtoys
else
	@echo "Skipping run_build_margtoys due to no GENIE"
endif

run_fit_marg: FORCE
ifeq ("YES","$(OPT_ENABLE_GENIE)")
	$(CXX) $(CXXFLAGS) -c run_fit_marg.cxx $(GINCLUDES)
	$(LD) $(LDFLAGS) run_fit_marg.o $(GLIBRARIES) $(VALOR_GLIBRARIES) -o $(VALOR_FMWK)/bin/valor-run_fit_marg
else
	@echo "Skipping run_fit_marg due to no GENIE"
endif

run_build_toys: FORCE
ifeq ("YES","$(OPT_ENABLE_GENIE)")
	$(CXX) $(CXXFLAGS) -c run_build_toys.cxx $(GINCLUDES)
	$(LD) $(LDFLAGS) run_build_toys.o $(GLIBRARIES) $(VALOR_GLIBRARIES) -o $(VALOR_FMWK)/bin/valor-run_build_toys
else
	@echo "Skipping run_build_toys due to no GENIE"
endif


make_dummy_samples: FORCE
ifeq ("YES","$(OPT_ENABLE_GENIE)")
	$(CXX) $(CXXFLAGS) -c make_dummy_samples.cxx $(GINCLUDES)
	$(LD) $(LDFLAGS) make_dummy_samples.o $(GLIBRARIES) $(VALOR_GLIBRARIES) -o $(VALOR_FMWK)/bin/valor-make_dummy_samples
else
	@echo "Skipping make_dummy_samples due to no GENIE"
endif

run_tests: FORCE
ifdef GENIE
	$(CXX) $(CXXFLAGS) -c run_tests.cxx $(GINCLUDES)
	$(LD) $(LDFLAGS) run_tests.o $(GLIBRARIES)  $(VALOR_GLIBRARIES) -o $(VALOR_FMWK)/bin/valor-run_tests
else
	@echo "Skipping run_tests due to no GENIE"
endif

batch_tests: FORCE
ifdef GENIE
	$(CXX) $(CXXFLAGS) -c batch_tests.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) batch_tests.o $(LIBRARIES) -o $(VALOR_FMWK)/bin/valor-batch_tests
else
	@echo "Skipping batch_tests due to no GENIE"
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
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-write_pdf
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-weak_pdf_N
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-tweak_pdf_sigma_var
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-fitnt_conv
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-adaptive_mcmc
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-run_tests
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-run_build_margtoys
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-run_fit_marg
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-make_dummy_samples
	$(RM) $(VALOR_FMWK_INSTALLATION_PATH)/bin/valor-batch_tests

FORCE:
	
# DO NOT DELETE

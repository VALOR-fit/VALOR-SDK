
/*

This macro makes a "selected" event sample from LBNE fast MC files with an efficiency and purity that are input parameters.
It uses truth information to make the selection, but omits a fraction of signal events equal to 1 - efficiency. It also 
adds enough background events (i.e. events whose true interaction types are different from the signal) to the sample to 
give the required purity. 

The following samples can be made:

"CC0Pi": CC with no pion in final state
"CC1ChPi": CC with exactly 1 charged pion and no neutral pion in final state
"CC1Pi0": CC with no charged pion and exactly 1 neutral pion in final state
"CC2ChPi": CC with exactly 2 charged pions and no neutral pion in final state
"CC2ChPi1Pi0": CC with exactly 2 charged pions and exactly 1 neutral pion in final state
"CCOther": other CC
"NC1Pi0": NC with no charged pion and exactly 1 neutral pion in final state
"NCOther": other NC

CC event samples can be normalised using the scale factor

2.20471e-12 * POT * fiducial mass (kt) / nCCEvents

where 

2.20471e-12 is number of true CC interactions / POT / kt in near detector - this is NuMuCCEventRate / POTperYear 
in /lbne/data/users/lblpwg_tools/FastMC_Data/flux_files/v3r0p10/nominal/g4lbne_v3r0p10_FHC_ND.root. 

nCCEvents is the number of true CC events in event sample before selection - this is in the gst tree.

NC event samples can be normalised in a similar way using the scale factor

7.44098e-13 * POT * fiducial mass (kt) / nNCEvents

where 

7.44098e-13 is the number of true NC interactions / POT / kt in near detector - this is NuMuNCEventRate / POTperYear 
in /lbne/data/users/lblpwg_tools/FastMC_Data/flux_files/v3r0p10/nominal/g4lbne_v3r0p10_FHC_ND.root. 

nNCEvents is the number of true NC events in event sample before selection - this is in the gst tree.

Nick Grant 30 April 2014

*/

void make_event_sample_from_truth
(
  TString sample = "CC2ChPi", 
  double efficiency = 0.8, 
  double purity = 0.7
)
{

  TChain *fastMCchain = new TChain("gst");
  fastMCchain->Add("/lbne/data/users/lblpwg_tools/FastMC_Data/outputs/nigrant/fastMC_ND_FHC_numuflux_numu_1.root");
  fastMCchain->Add("/lbne/data/users/lblpwg_tools/FastMC_Data/outputs/nigrant/fastMC_ND_FHC_numuflux_numu_2.root");
  fastMCchain->Add("/lbne/data/users/lblpwg_tools/FastMC_Data/outputs/nigrant/fastMC_ND_FHC_numuflux_numu_3.root");
  fastMCchain->Add("/lbne/data/users/lblpwg_tools/FastMC_Data/outputs/nigrant/fastMC_ND_FHC_numuflux_numu_4.root");
  fastMCchain->Add("/lbne/data/users/lblpwg_tools/FastMC_Data/outputs/nigrant/fastMC_ND_FHC_numuflux_numu_5.root");
  fastMCchain->Add("/lbne/data/users/lblpwg_tools/FastMC_Data/outputs/nigrant/fastMC_ND_FHC_numuflux_numu_6.root");
  fastMCchain->Add("/lbne/data/users/lblpwg_tools/FastMC_Data/outputs/nigrant/fastMC_ND_FHC_numuflux_numu_7.root");
  fastMCchain->Add("/lbne/data/users/lblpwg_tools/FastMC_Data/outputs/nigrant/fastMC_ND_FHC_numuflux_numu_8.root");
  fastMCchain->Add("/lbne/data/users/lblpwg_tools/FastMC_Data/outputs/nigrant/fastMC_ND_FHC_numuflux_numu_9.root");
  fastMCchain->Add("/lbne/data/users/lblpwg_tools/FastMC_Data/outputs/nigrant/fastMC_ND_FHC_numuflux_numu_10.root");
  
  bool cc;
  bool nc;
  //number of CC + NC interactions per kt per POT - according to https://cdcvs.fnal.gov/redmine/projects/fast_mc/wiki/Nominal_Flux_Normalizations
  //or number of CC + NC interactions per POT - according to comment in gNtpConv_FastMC.cxx
  double POTWeight;

  //number of initial state pi plus; "i" means "initial", i.e. after primary interaction but before intranuclear rescattering
  int nipip;
  int nipim;
  int nipi0;
  //number of pi plus in final state; "f" means "final", i.e. after intranuclear rescattering
  int nfpip;
  int nfpim;
  int nfpi0;

  gst->SetBranchAddress("cc",&cc);
  gst->SetBranchAddress("nc",&nc);
  gst->SetBranchAddress("POTWeight",&POTWeight);
  gst->SetBranchAddress("nipip",&nipip);
  gst->SetBranchAddress("nipim",&nipim);
  gst->SetBranchAddress("nipi0",&nipi0);
  gst->SetBranchAddress("nfpip",&nfpip);
  gst->SetBranchAddress("nfpim",&nfpim);
  gst->SetBranchAddress("nfpi0",&nfpi0);

  TString fileName = sample;
  fileName += "_Eff";
  fileName += Form("%d", (int)(efficiency * 100.0));
  fileName += "_Pur";
  fileName += Form("%d", (int)(purity * 100.0));
  fileName += ".root";

  TFile *sampleFile = new TFile(fileName, "RECREATE");
  //clone tree from input fast MC file but don't write any events yet
  TTree *sampleTree = gst->CloneTree(0);

  int nEvents, nCCEvents, nNCEvents, br_nEvents, br_nCCEvents, br_nNCEvents;

  //add some braches to tree containing selected events
  sampleTree->Branch("nEvents", &br_nEvents, "nEvents/I");
  sampleTree->Branch("nCCEvents", &br_nCCEvents, "nCCEvents/I");
  sampleTree->Branch("nNCEvents", &br_nNCEvents, "nNCEvents/I");

  TRandom3 rand(0);
  double uniformRand;

  int nEntries = gst->GetEntries();

  nEvents = 0;
  nCCEvents = 0;
  nNCEvents = 0;

  //number of true events in the selection called "sample"
  int nTrueEvents = 0;

  for(Long64_t i = 0; i < nEntries; i++)
    {
      gst->GetEntry(i);
      if(i%10000 == 0)
        cout<<"Doing event "<<i<<endl;

      nEvents++;

      if(cc)
	nCCEvents++;

      if(nc)
        nNCEvents++;

      if(sample == "CC0Pi")
        {
	if(cc && (nfpip + nfpim == 0) && nfpi0 == 0)
	  nTrueEvents++;
	}

      if(sample == "CC1ChPi")
	{
	if(cc && (nfpip + nfpim == 1) && nfpi0 == 0)
	  nTrueEvents++;
	}

      if(sample == "CC1Pi0")
        {
	if(cc && (nfpip + nfpim == 0) && nfpi0 == 1)
	  nTrueEvents++;
        }

      if(sample == "CC2ChPi")
        {
	if(cc && (nfpip + nfpim == 2) && nfpi0 == 0)
	  nTrueEvents++;
        }

      if(sample == "CC2ChPi1Pi0")
	{
        if(cc && (nfpip + nfpim == 2) && nfpi0 == 1)
          nTrueEvents++;
        }

      if(sample == "CCOther")
	{
	if(cc && ((nfpip + nfpim > 2) || nfpi0 > 1))
          nTrueEvents++;
        }

      if(sample == "NC1Pi0")
	{
	if(nc && (nfpip + nfpim == 0) && nfpi0 == 1)
          nTrueEvents++;
        }

      if(sample == "NCOther")
	{
	if(nc && ((nfpip + nfpim > 0) || nfpi0 > 1))
          nTrueEvents++;
        }

    }//end of loop over events

  br_nEvents = nEvents;
  br_nCCEvents = nCCEvents;
  br_nNCEvents = nNCEvents;

  for(Long64_t i = 0; i < nEntries; i++)
    {
      gst->GetEntry(i);
      if(i%10000 == 0)
	cout<<"Doing event "<<i<<endl;

      if(sample == "CC0Pi")
        {
	if(cc && (nfpip + nfpim == 0) && nfpi0 == 0)
	  {
	  uniformRand = rand.Uniform(0,1);
	  //add event to sample if it is signal (true CC0pi in this case), but omit a fraction equal to 1 - efficiency
	  if(uniformRand < efficiency)
	    sampleTree->Fill();
	  }
	else
	  {
	  uniformRand = rand.Uniform(0,1);
	  //add background events (everything except true CC0pi in this case) to sample to obtain the required purity 
	  if(uniformRand < (((double)nTrueEvents / (nEntries - nTrueEvents)) * efficiency * (1.0 - purity)) / purity)
	    sampleTree->Fill();
	  }
	}

      if(sample == "CC1ChPi")
        {
	if(cc && (nfpip + nfpim == 1) && nfpi0 == 0)
	  {
	  uniformRand = rand.Uniform(0,1);
	  if(uniformRand < efficiency)
	    sampleTree->Fill();
	  }
	else
	  {
	  uniformRand = rand.Uniform(0,1);
	  if(uniformRand < (((double)nTrueEvents / (nEntries - nTrueEvents)) * efficiency * (1.0 - purity)) / purity)
	    sampleTree->Fill();
	  }
	}

      if(sample == "CC1Pi0")
	{
	if(cc && (nfpip + nfpim == 0) && nfpi0 == 1)
	  {
	  uniformRand = rand.Uniform(0,1);
	  if(uniformRand < efficiency)
	    sampleTree->Fill();
	  }
        else
	  {
	  uniformRand = rand.Uniform(0,1);
	  if(uniformRand < (((double)nTrueEvents / (nEntries - nTrueEvents)) * efficiency * (1.0 - purity)) / purity)
	    sampleTree->Fill();
	  }
	}

      if(sample == "CC2ChPi")
	{
        if(cc && (nfpip + nfpim == 2) && nfpi0 == 0)
          {
          uniformRand = rand.Uniform(0,1);
          if(uniformRand < efficiency)
	    sampleTree->Fill();
          }
        else
          {
          uniformRand = rand.Uniform(0,1);
          if(uniformRand < (((double)nTrueEvents / (nEntries - nTrueEvents)) * efficiency * (1.0 - purity)) / purity)
	    sampleTree->Fill();
          }
        }

      if(sample == "CC2ChPi1Pi0")
	{
        if(cc && (nfpip + nfpim == 2) && nfpi0 == 1)
          {
          uniformRand = rand.Uniform(0,1);
          if(uniformRand < efficiency)
	    sampleTree->Fill();
          }
        else
          {
          uniformRand = rand.Uniform(0,1);
          if(uniformRand < (((double)nTrueEvents / (nEntries - nTrueEvents)) * efficiency * (1.0 - purity)) / purity)
	    sampleTree->Fill();
          }
        }

      if(sample == "NC1Pi0")
	{
        if(nc && (nfpip + nfpim == 0) && nfpi0 == 1)
          {
          uniformRand = rand.Uniform(0,1);
          if(uniformRand < efficiency)
	    sampleTree->Fill();
          }
        else
          {
          uniformRand = rand.Uniform(0,1);
          if(uniformRand < (((double)nTrueEvents / (nEntries - nTrueEvents)) * efficiency * (1.0 - purity)) / purity)
	    sampleTree->Fill();
          }
        }

      if(sample == "NCOther")
	{
        if(nc && ((nfpip + nfpim > 0) || nfpi0 > 1))
          {
          uniformRand = rand.Uniform(0,1);
          if(uniformRand < efficiency)
	    sampleTree->Fill();
          }
        else
          {
          uniformRand = rand.Uniform(0,1);
          if(uniformRand < (((double)nTrueEvents / (nEntries - nTrueEvents)) * efficiency * (1.0 - purity)) / purity)
	    sampleTree->Fill();
          }
        }

    }//end of loop over events

  sampleTree->Write();

}//end of make_event_sample_from_truth()



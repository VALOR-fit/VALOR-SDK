
/*

This macro makes selected event samples from LBNE fast MC files using reconstructed information only. 
The following samples can be made:

"CCInc": reconstructed as CC inclusive (this is the same as CC 1 track)
"CC2Track": reconstructed as CC with exactly 1 track in hadronic system with length > trackLengthCut
"CC1ChPi": reconstructed as CC with exactly 2 tracks in hadronic system with length > trackLengthCut
"CC1Pi0": reconstructed as CC with >= 2 tracks in hadronic system and exactly 2 EM objects; longest track has 
          length > trackLengthCut, second longest track has length < trackLengthCut, and shortest track has 
          length < trackLengthCut
"CC2ChPi": reconstructed as CC with exactly 3 tracks in hadronic system with length > trackLengthCut
"CCOther": other CC
"NC1Pi0": reconstructed as NC with >= 1 track in hadronic system and exactly 2 EM objects; both longest and 
          shortest tracks have length < trackLengthCut
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

Nick Grant 5 May 2014

*/

void make_event_sample_from_reco
(
 TString sample = "NCOther",
 double trackLengthCut = 0.05
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
  int nip;
  int nipip;
  int nipim;
  int nipi0;
  //number of pi plus in final state; "f" means "final", i.e. after intranuclear rescattering
  int nfp;
  int nfpip;
  int nfpim;
  int nfpi0;

  //EVClass_reco is reco type of event: 0 = numu CC, 1 = nue CC, 2 = NC
  int EvClass_reco;

  //Upper case L refers to the particle reconstructed as the primary lepton.
  //It is not necessarily the true primary lepton.
  int PdgL;
  double pL_reco;
  double pxL_reco;
  double pyL_reco;
  double pzL_reco;
  double Ehad_reco;
  double EhadB_reco;
  double cthL_reco;
  double TrkL_reco;
  bool IsContL_reco;

  const int maxParticles = 100;

  const int nf;
  int pdgf[maxParticles];
  double Ef[maxParticles];
  double pxf[maxParticles];
  double pyf[maxParticles];
  double pzf[maxParticles];
  double pf[maxParticles];
  double cthf[maxParticles];
  double Ef_reco[maxParticles];
  double pxf_reco[maxParticles];
  double pyf_reco[maxParticles];
  double pzf_reco[maxParticles];
  double Trkf_reco[maxParticles];
  double cthf_reco[maxParticles];

  const int nEMf;
  int EMpdgf[maxParticles];
  double EMpxf[maxParticles];
  double EMpyf[maxParticles];
  double EMpzf[maxParticles];
  double EMpf[maxParticles];
  double EMcthf[maxParticles];
  double EMEf[maxParticles];
  double Ef_reco[maxParticles];
  double EMpxf_reco[maxParticles];
  double EMpyf_reco[maxParticles];
  double EMpzf_reco[maxParticles];
  double EMTrkf_reco[maxParticles];
  double EMcthf_reco[maxParticles];
  double EMPizMass_reco[maxParticles];
  double EMElPizM_reco[maxParticles];

  gst->SetBranchAddress("cc",&cc);
  gst->SetBranchAddress("nc",&nc);
  gst->SetBranchAddress("POTWeight",&POTWeight);
  gst->SetBranchAddress("nip",&nip);
  gst->SetBranchAddress("nipip",&nipip);
  gst->SetBranchAddress("nipim",&nipim);
  gst->SetBranchAddress("nipi0",&nipi0);
  gst->SetBranchAddress("nfp",&nfp);
  gst->SetBranchAddress("nfpip",&nfpip);
  gst->SetBranchAddress("nfpim",&nfpim);
  gst->SetBranchAddress("nfpi0",&nfpi0);

  gst->SetBranchAddress("EvClass_reco",&EvClass_reco);

  gst->SetBranchAddress("PdgL",&PdgL);
  gst->SetBranchAddress("pL_reco",&pL_reco);
  gst->SetBranchAddress("pxL_reco",&pxL_reco);
  gst->SetBranchAddress("pyL_reco",&pyL_reco);
  gst->SetBranchAddress("pzL_reco",&pzL_reco);
  gst->SetBranchAddress("cthL_reco",&cthL_reco);
  gst->SetBranchAddress("TrkL_reco",&TrkL_reco);
  gst->SetBranchAddress("IsContL_reco",&IsContL_reco);

  gst->SetBranchAddress("nf",&nf);
  gst->SetBranchAddress("pdgf",pdgf);
  gst->SetBranchAddress("Ef",Ef);
  gst->SetBranchAddress("pxf",pxf);
  gst->SetBranchAddress("pyf",pyf);
  gst->SetBranchAddress("pzf",pzf);
  gst->SetBranchAddress("pf",pf);
  gst->SetBranchAddress("cthf",cthf);
  gst->SetBranchAddress("Ef_reco",Ef_reco);
  gst->SetBranchAddress("pxf_reco",pxf_reco);
  gst->SetBranchAddress("pyf_reco",pyf_reco);
  gst->SetBranchAddress("pzf_reco",pzf_reco);
  gst->SetBranchAddress("Trkf_reco",Trkf_reco);
  gst->SetBranchAddress("cthf_reco",cthf_reco);
  gst->SetBranchAddress("nEMf",&nEMf);
  gst->SetBranchAddress("EMpdgf",EMpdgf);
  gst->SetBranchAddress("EMEf",EMEf);
  gst->SetBranchAddress("EMpxf",EMpxf);
  gst->SetBranchAddress("EMpyf",EMpyf);
  gst->SetBranchAddress("EMpzf",EMpzf);
  gst->SetBranchAddress("EMpf",EMpf);
  gst->SetBranchAddress("EMcthf",EMcthf);
  gst->SetBranchAddress("Ef_reco",Ef_reco);
  gst->SetBranchAddress("EMpxf_reco",EMpxf_reco);
  gst->SetBranchAddress("EMpyf_reco",EMpyf_reco);
  gst->SetBranchAddress("EMpzf_reco",EMpzf_reco);
  gst->SetBranchAddress("EMTrkf_reco",EMTrkf_reco);
  gst->SetBranchAddress("EMcthf_reco",EMcthf_reco);
  gst->SetBranchAddress("EMPizMass_reco",EMPizMass_reco);
  gst->SetBranchAddress("EMElPizM_reco",EMElPizM_reco);

  TString fileName = sample;
  fileName += ".root";

  TFile *sampleFile = new TFile(fileName, "RECREATE");
  //clone tree from input fast MC file but don't write any events yet
  TTree *sampleTree = gst->CloneTree(0);

  int nEvents, nCCEvents, nNCEvents, br_nEvents, br_nCCEvents, br_nNCEvents;
  double maxTrackLength, maxTrackLength2, minTrackLength;

  //add some braches to tree containing selected events
  sampleTree->Branch("nEvents", &br_nEvents, "nEvents/I");
  sampleTree->Branch("nCCEvents", &br_nCCEvents, "nCCEvents/I");
  sampleTree->Branch("nNCEvents", &br_nNCEvents, "nNCEvents/I");

  nEvents = 0;
  nCCEvents = 0;
  nNCEvents = 0;

  //number of true events in the selection called "sample"
  int nTrueEvents = 0;
  int nTrueEventsPassSel = 0;
  int nEventsPassSel = 0;

  for(Long64_t i = 0; i < gst->GetEntries(); i++)
    //for(Long64_t i = 0; i < 2000; i++)
    {
      gst->GetEntry(i);
      if(i%10000 == 0)
        cout<<"Doing event "<<i<<endl;

      nEvents++;

      if(cc)
	nCCEvents++;

      if(nc)
        nNCEvents++;

      if(sample == "CCInc")
	{
	if(cc)
	  nTrueEvents++;
	if(EvClass_reco == 0)
	  {
	  nEventsPassSel++;
	  sampleTree->Fill();
	  if(cc)
	    nTrueEventsPassSel++;
	  }
	}

      if(sample == "CC2Track")
        {
	if(cc && nfp == 1 && (nfpip + nfpim == 0) && nfpi0 == 0)
	  nTrueEvents++;
	if(EvClass_reco == 0 && nf == 1 && Trkf_reco[0] > trackLengthCut)
	  {
	  nEventsPassSel++;
	  sampleTree->Fill();
	if(cc && nfp == 1 && (nfpip + nfpim == 0) && nfpi0 == 0)
	    nTrueEventsPassSel++;
	  }
	}

      if(sample == "CC1ChPi")
	{
	if(cc && (nfpip + nfpim == 1) && nfpi0 == 0)
	  nTrueEvents++;
	if(EvClass_reco == 0 && nf == 2 && Trkf_reco[0] > trackLengthCut && Trkf_reco[1] > trackLengthCut)
	  {
	  nEventsPassSel++;
	  sampleTree->Fill();
	  if(cc && (nfpip + nfpim == 1) && nfpi0 == 0)
	    nTrueEventsPassSel++;
	  }
	}

      if(sample == "CC1Pi0")
        {
	if(cc && (nfpip + nfpim == 0) && nfpi0 == 1)
          nTrueEvents++;
	maxTrackLength = -999.9;
        maxTrackLength2 = -999.9;
	minTrackLength = 99999.9;
	for(int j=0; j<nf; j++)
	  {
	  if(Trkf_reco[j] > maxTrackLength)
	    {
	    maxTrackLength2 = maxTrackLength;
	    maxTrackLength = Trkf_reco[j];
	    }
	  if(Trkf_reco[j] < maxTrackLength && Trkf_reco[j] > maxTrackLength2)
	     maxTrackLength2 = Trkf_reco[j];
	  if(Trkf_reco[j] > 0.0 && Trkf_reco[j] < minTrackLength)
            minTrackLength = Trkf_reco[j];
	  }
        if(EvClass_reco == 0 && nf >= 2 && maxTrackLength > trackLengthCut && maxTrackLength2 < trackLengthCut && minTrackLength < trackLengthCut && nEMf == 2)
          {
          nEventsPassSel++;
          sampleTree->Fill();
          if(cc && (nfpip + nfpim == 0) && nfpi0 == 1)
            nTrueEventsPassSel++;
          }
	}

      if(sample == "CC2ChPi")
        {
	if(cc && (nfpip + nfpim == 2) && nfpi0 == 0)
	  nTrueEvents++;
	if(EvClass_reco == 0 && nf == 3 && Trkf_reco[0] > trackLengthCut && Trkf_reco[1] > trackLengthCut && Trkf_reco[2] > trackLengthCut)
	  {
	  nEventsPassSel++;
	  sampleTree->Fill();
	  if(cc && (nfpip + nfpim == 2) && nfpi0 == 0)
	    nTrueEventsPassSel++;
	  }
        }

      if(sample == "CCOther")
        {
        if(cc && ((nfpip + nfpim > 2) || nfpi0 > 1))
	  nTrueEvents++;
	if(EvClass_reco == 0 && nf > 3 && nEMf != 2)
	  {
	  nEventsPassSel++;
	  sampleTree->Fill();
	if(cc && ((nfpip + nfpim > 2) || nfpi0 > 1))
	    nTrueEventsPassSel++;
	  }
        }

      if(sample == "NC1Pi0")
        {
	if(nc && (nfpip + nfpim == 0) && nfpi0 == 1)
	  nTrueEvents++;
	maxTrackLength = -999.9;
	minTrackLength = 99999.9;
	for(int j=0; j<nf; j++)
          {
	  if(Trkf_reco[j] > maxTrackLength)
	    maxTrackLength = Trkf_reco[j];
	  if(Trkf_reco[j] > 0.0 && Trkf_reco[j] < minTrackLength)
	    minTrackLength = Trkf_reco[j];
          }
	if(EvClass_reco == 2 && nf >= 1 && minTrackLength < trackLengthCut && maxTrackLength < trackLengthCut && nEMf == 2)
	  {
	  nEventsPassSel++;
	  sampleTree->Fill();
	  if(nc && (nfpip + nfpim == 0) && nfpi0 == 1)
	    nTrueEventsPassSel++;
	  }
        }

      if(sample == "NCOther")
	{
	if(nc && nfpi0 != 1)
	  nTrueEvents++;
	if(EvClass_reco == 2 && nEMf != 2)
	  {
	  nEventsPassSel++;
	  sampleTree->Fill();
	  if(nc && nfpi0 != 1)
	    nTrueEventsPassSel++;
	  }
        }
    }//end of loop over events

  cout<<setprecision(3);
  cout<<endl;
  cout<<"Efficiency / purity of sample "<<sample<<" is "<<(double)nTrueEventsPassSel / nTrueEvents<<" / "<<(double)nTrueEventsPassSel / nEventsPassSel<<endl; 
  cout<<endl;

  br_nEvents = nEvents;
  br_nCCEvents = nCCEvents;
  br_nNCEvents = nNCEvents;

  sampleTree->Write();

}//end of make_event_sample_from_reco()



#!/usr/bin/python

# Plots numu -> numu, nue, nutau oscillation probabilities as a function of E, at fixed oscillation parameter values and fixed L
# Steve Dennis, September 2014
# Francis Bench, November 2019
# Tom Dealtry, August 2022

import ROOT
import sys

R=ROOT

env_var = "VALOR_FMWK"
try:
    ROOT.valor
except:
    ROOT.gROOT.ProcessLine(".x $VALOR_FMWK/root/valor_interpreter.C")

ROOT.gROOT.SetBatch(1)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptTitle(0)

valor = ROOT.valor
util =  valor.util
conv =  valor.conv
opt  =  valor.Opt.Instance()
env  =  valor.Env.Instance()
rnd  =  valor.Rnd.Instance()

sinsq_theta23 = 0.528
sinsq_theta12 = 0.304
sinsq_theta13 = 0.0212
dm23sq = 2.509E-3
dm12sq = 7.53E-5
deltacp = -1.601

opt.gMassHierarchyAssumed = conv.kNmhNormal
opt.gOscMixingParamChoice = conv.kNuMixSinsqTheta
opt.gOscOctantChoice      = conv.kNuOctantTwo
opt.gOscDmsqScheme        = conv.kNuDmsqAuto
opt.SetDefaultParameterValues()

nbins = 1000

baseline = 295.


def main():

    fout = ROOT.TFile("oscillation_probability_vs_E.root", "RECREATE")
    
    all_hists = dict()

    osc = valor.NuOsc3Flavour()
    osc.SetBaseline(baseline)

    canv = ROOT.TCanvas("canv","canv",1280,1280)
    canv.Divide(2,2)

    maxE = 30
    for ilimit,[initial_nu, initial_nu_string, initial_nu_string_short] in enumerate([[conv.kPdgNuMu, "#nu_{#mu}", "numu"],
                                                                                      [conv.kPdgNuE, "#nu_{e}", "nue"],
                                                                                      [conv.kPdgNuMuBar, "#bar{#nu}_{#mu}", "numubar"],
                                                                                      [conv.kPdgNuEBar, "#bar{#nu}_e", "nuebar"]
                                                                                  ]):

        endbar = 'bar' if "bar" in initial_nu_string_short else ''
        hname = "osc_prob_"+initial_nu_string_short+"_to_"+"FINALFLAV" +endbar+"_"+`int(baseline)`+'km'
        h_mu  = R.TH1D(hname.replace('FINALFLAV', 'numu' ), hname.replace('FINALFLAV', 'numu' ), nbins,0,maxE)
        h_e   = R.TH1D(hname.replace('FINALFLAV', 'nue'  ), hname.replace('FINALFLAV', 'nue'  ), nbins,0,maxE)
        h_tau = R.TH1D(hname.replace('FINALFLAV', 'nutau'), hname.replace('FINALFLAV', 'nutau'), nbins,0,maxE)

        print h_mu.GetName()
        
        hists = [h_mu,h_e,h_tau]

        for h in hists:
            all_hists[h.GetName()] = h
            h.GetXaxis().SetTitle("E (GeV)")
            if "bar" in initial_nu_string_short:
                h.GetYaxis().SetTitle("P_{" + initial_nu_string + " #rightarrow #bar{#nu}_{#alpha}}")
            else:
                h.GetYaxis().SetTitle("P_{" + initial_nu_string + " #rightarrow #nu_{#alpha}}")
            h.SetMaximum(1)
            h.SetMinimum(0)
            h.GetXaxis().SetTitleSize(0.045)
            h.GetYaxis().SetTitleSize(0.045)
            h.SetLineWidth(1)

        h_mu .SetLineColor(ROOT.kRed+1)
        h_e  .SetLineColor(ROOT.kBlue+2)
        h_tau.SetLineColor(ROOT.kGreen+2)

        for ii in xrange(1,nbins+1):
            energy = h_mu.GetBinCenter(ii)
            osc.SetOscParametersSinSqTAll(sinsq_theta12,sinsq_theta13,sinsq_theta23,
                                          dm12sq, dm23sq, deltacp)

            if "bar" in initial_nu_string_short:
                h_mu .Fill(energy,osc.CalcMatterOscProb(initial_nu, conv.kPdgNuMuBar , energy))
                h_e  .Fill(energy,osc.CalcMatterOscProb(initial_nu, conv.kPdgNuEBar  , energy))
                h_tau.Fill(energy,osc.CalcMatterOscProb(initial_nu, conv.kPdgNuTauBar, energy))
            else:
                h_mu .Fill(energy,osc.CalcMatterOscProb(initial_nu, conv.kPdgNuMu , energy))
                h_e  .Fill(energy,osc.CalcMatterOscProb(initial_nu, conv.kPdgNuE  , energy))
                h_tau.Fill(energy,osc.CalcMatterOscProb(initial_nu, conv.kPdgNuTau, energy))

        canv.cd(ilimit+1)

        h_mu .Draw("hist")
        h_e  .Draw("hist,same")
        h_tau.Draw("hist,same")

        for h in hists:
            h.Write()


    legend = R.TLegend(0.47,0.6,0.508,0.9)
    legend.AddEntry(h_mu , "#nu_{#mu}" , "l")
    legend.AddEntry(h_e  , "#nu_{e}"   , "l")
    legend.AddEntry(h_tau, "#nu_{#tau}", "l")
    legend.SetFillStyle(0)
    canv.cd(0)

    legend.Draw()

    canv.Print("oscillation_patterns_E.pdf")

    canv.Write()
    fout.ls()
    fout.Close()
    
if __name__=="__main__":
    main()


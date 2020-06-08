import ROOT
import sys
import os
import glob
import math
import itertools

# If GENIE available, load that first
genie_enabled = "OPT_ENABLE_GENIE=YES" in open(os.environ["VALOR_FMWK"]+"/make/Make.config").read()
if genie_enabled:
  ROOT.gROOT.ProcessLine(".x ${GENIE}/src/scripts/gcint/loadlibs.C");

ROOT.gROOT.ProcessLine(".x ${VALOR_FMWK}/root/valor_interpreter.C");


ROOT.gROOT.ProcessLine("using namespace valor;");
ROOT.gROOT.ProcessLine("using namespace valor::util;");
ROOT.gROOT.ProcessLine("using namespace valor::conv;");

R = ROOT

valor = ROOT.valor
conv  = valor.conv
util  = valor.util
opt   = valor.Opt.Instance()
rnd   = valor.Rnd.Instance()
nr    = util.NameRegistry.Instance()

print("\n==============================")
print("Loading VALOR Python"+(" with GENIE" if genie_enabled else ""))
print("==============================\n")

try:
  import numpy as np
except:
  print("Warning: Numpy not loaded")

opt.gDoNotDie = True # We're interactive, so don't die unless you have to.

# Make float formatting strings
for n in range(1,10):
  globals()["to%ddp"%n] = "{0:.%df}"%n
  globals()["to%dsf"%n] = "{0:.%de}"%(n-1)
  
  
# Touch every VALOR object so they appear in tab completion
import os
folders = [os.environ['VALOR_FMWK']]
walk = os.walk(*folders)
for root, dirs, files in itertools.chain(walk):
  for f in files:
    if f.endswith(".cxx"):
      classname = f[:-4]
      try:
        getattr(valor,classname)
        getattr(util ,classname)
        getattr(conv ,classname)
      except:
        pass
  
# Quick and dirty function to get every object in a file
def fileburst(f):
  for k in f.GetListOfKeys():
    obj = k.ReadObj()
    if (obj.ClassName()=="TTree"):
      if obj.GetEntries():
        obj.GetEntry(0)
    globals()[k.GetName()] = obj
  return
  
rootargs = [x for x in sys.argv if x.endswith(".root")]
i=0
for f in rootargs:
  varname = "_file%d"%i
  print("Attaching file ",f," as ",varname)
  globals()[varname] = ROOT.TFile(f)
  i+=1
  if i > 999: break

if (i==1):
  fileburst(_file0)

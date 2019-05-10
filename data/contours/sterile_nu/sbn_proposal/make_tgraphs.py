#!/usr/bin/env python

import ROOT
import sys
import numpy as np

R=ROOT

files = sys.argv[1:]

output_file = R.TFile("tgraph_from_data.root","RECREATE")

for filename in files:
  
  f = open(filename)
  
  name = filename.split("/")[-1].replace(".data","")
  
  content = []
  
  for l in f:
    l = l.strip()
    if not l: continue
    if l[0] == "#": continue
    
    x,y = map(float,filter(bool,l.split(" ")))
    
    content.append((x,y))
  
  # Sort them by y and make arrays
  reversed_indices = [(y,x) for x,y in content]
  reversed_indices.sort()
  x_sorted = [x for (y,x) in reversed_indices]
  y_sorted = [y for (y,x) in reversed_indices]
  
  graph = R.TGraph(len(x_sorted),np.array(x_sorted),np.array(y_sorted))
  
  graph.SetName(name)
  graph.Write()
    
  print "Made",name

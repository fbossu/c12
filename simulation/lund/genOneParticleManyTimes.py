#!/usr/bin/env python3



# -----------------------------------------------------------------------
#  LUND FORMAT
# https://gemc.jlab.org/gemc/html/documentation/generator/lund.html#lund
"""
::: Header :::
   1   2 3 4     5     6 7 8 9 10
 Npart - - -  Bea Pol  - - - -  -

::: Particle :::

 1  2    3           4      5 6   7  8  9  10 11  12 13 14
ID  -  type       Part ID   - -  px py pz   -  -  vx vy vz
      1=active
"""
# all the not useful fields will be filled with 99
# -----------------------------------------------------------------------

import sys, getopt
import math
from random import random as rndm
from random import gauss
from random import seed

##############################################
""" 
Set SEED
"""
###############################################
seed(43)


##############################################
""" 
Set Number of events
"""
###############################################
NEVENTS=10000


##############################################
""" 
Set Outout file name
"""
###############################################
ofilename = "one.lund"

##############################################
""" 
Set the list of particles you want to generate
by writing the list of PDG codes
"""
###############################################

PDGs = [ 2212 ] 
#PDGs = [ 2212, 13, -13, 211, -211, 11, -11 ] 




###############################################
# generate
##########
BeamPolarization = -1 # not useful

with open( ofilename ,"w") as fout:

  # loop on events
  for i in range(1,NEVENTS+1):
    header = [99]*10
    header[0] = len(PDGs)
    header[4] = BeamPolarization

    for h in header:
      fout.write(str(h))
      fout.write(" ")
    fout.write("\n")
    vx= 0.0
    vy= 0.0
    vz= 0.0

    for j,p in enumerate(PDGs):
      particle = [99]*14
      particle[0] = j + 1;
      particle[2] = 1
      particle[3] = p

      particle[6] = 0.2391
      particle[7] = 0.1983
      particle[8] = 0.338
      
  
      particle[11] = vx
      particle[12] = vy
      particle[13] = vz


      for h in particle:
        #fout.write(str(round(h,6)))
        fout.write(str(h))
        fout.write(" ")
      fout.write("\n")



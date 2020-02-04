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
ofilename = "test.lund"

##############################################
""" 
Set the list of particles you want to generate
by writing the list of PDG codes
"""
###############################################

PDGs = [ 2212 ] 
#PDGs = [ 2212, 13, -13, 211, -211, 11, -11 ] 



##############################################
""" 
Set Max and Min of the ranges
"""
###############################################

# generate flat in ranges of P, theta, phi
PMin=0.25
PMax=2

PhiMin=0.0
PhiMax=2.0*math.pi

ThetaMin=math.radians(35.0)
ThetaMax=math.radians(130.0)

# vertex 
VR = 0.03

Vx = -0.171

Vy = -0.1

VzMin = -2.5
VzMax =  2.5

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
    r = abs( gauss( 0., VR ) )
    f = rndm()*2*math.pi
    vx= Vx + r * math.cos( f )
    vy= Vy + r * math.sin( f )
    vz= VzMin + rndm()*(VzMax - VzMin)

    for j,p in enumerate(PDGs):
      particle = [99]*14
      particle[0] = j + 1;
      particle[2] = 1
      particle[3] = p

      P = PMin + rndm()*(PMax - PMin)
      Phi = PhiMin + rndm()*(PhiMax - PhiMin)
      Theta = ThetaMin + rndm()*(ThetaMax - ThetaMin)
      
      particle[6] = P*math.sin(Theta)*math.cos(Phi)  # px
      particle[7] = P*math.sin(Theta)*math.sin(Phi)  # py
      particle[8] = P*math.cos(Theta)  # pz
      
  
      particle[11] = vx
      particle[12] = vy
      particle[13] = vz


      for h in particle:
        #fout.write(str(round(h,6)))
        fout.write(str(h))
        fout.write(" ")
      fout.write("\n")



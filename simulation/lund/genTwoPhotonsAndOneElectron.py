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

from ROOT import TVector3

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
ofilename = "photons.lund"

##############################################
""" 
Set the list of particles you want to generate
by writing the list of PDG codes
"""
###############################################

PDGs = [ 2212 ] 
#PDGs = [ 2212, 13, -13, 211, -211, 11, -11 ] 


def storeParticle(  pdg, vm, vertex, state ):
  particle = [99]*14
  particle[0] = counter;
  particle[2] = state
  particle[3] = pdg

  particle[6] = vm.Px()
  particle[7] = vm.Py()
  particle[8] = vm.Pz()
  particle[11] = vertex[0] 
  particle[12] = vertex[1] 
  particle[13] = vertex[2] 
  return particle


def writeParticles( outf, particles ):
  # write mother
  for i,p in enumerate( particles ):
    p[0] = i+1
    for h in p:
      outf.write(str(h))
      outf.write(" ")
    outf.write("\n")



##############################################
# set gamma energy
# set angle between gammas
##############################################

Eg = 4.
Phi = 1.

###############################################
# generate
##########
BeamPolarization = -1 # not useful

with open( ofilename ,"w") as fout:

  # loop on events
  for i in range(1,NEVENTS+1):
   
    vertex = (
        0.0,
        0.0,
        0.0
    )
 
    counter = 0 
    parts = []

  
    pelectron = TVector3()
    pelectron.SetMagThetaPhi( 5, math.radians( 15 ), math.radians( 180 ) )



    phi = 180*rndm()
    
    pgammaC  = TVector3()
    pgammaC.SetMagThetaPhi( Eg, math.radians( 15 ), math.radians( 0 ) )


    vnorm = pgammaC.Orthogonal()

    
    pgamma1  = TVector3()
    pgamma1.SetMagThetaPhi( Eg, math.radians( 15 ), math.radians( 0 ) )
    pgamma1.Rotate( math.radians( Phi/2. ), vnorm )
    pgamma1.Rotate( math.radians( phi ), pgammaC )

    pgamma2  = TVector3()
    pgamma2.SetMagThetaPhi( Eg, math.radians( 15 ), math.radians( 0 ) )
    pgamma2.Rotate( math.radians( Phi/2. ), vnorm )
    pgamma2.Rotate( math.radians( phi - 180 ), pgammaC )


    parts.append( storeParticle( 11, pelectron, vertex, 1 ) )
    parts.append( storeParticle( 22, pgamma1, vertex, 1 ) )
    parts.append( storeParticle( 22, pgamma2, vertex, 1 ) )


    ##############################
    header = [99]*10
    header[0] = len(parts)
    header[4] = BeamPolarization

    for h in header:
      fout.write(str(h))
      fout.write(" ")
    fout.write("\n")
    writeParticles( fout, parts )

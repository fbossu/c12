#!/bin/env python
#
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

from ROOT import TGenPhaseSpace, TLorentzVector, TDatabasePDG, TMath, TVector3

import math
from random import random as rndm
from array import array
from random import gauss

dbpdg = TDatabasePDG()

# chose motherParticle
PDGmother=3122
MotherMass = dbpdg.GetParticle(PDGmother).Mass()

# chose daughters
PDGs = [ 2212, -211 ]
DaughterMass = [ dbpdg.GetParticle(i).Mass() for i in PDGs ]


# Settings
# ========

NEVENTS=50000
BeamPolarization = -1 # not useful

# generate flat in ranges of P, theta, phi
PMin=0.5
PMax=2.0

PhiMin=0.0
PhiMax=2.0*math.pi

ThetaMin=math.radians(60.0)
ThetaMax=math.radians(100.0)

# vertex 
VR = 2.0

Vx = 0.0

Vy = 0.0

VzMin = -5.5
VzMax = -1.5


## FileName
fname = "phasespace_"
fname += dbpdg.GetParticle(PDGmother).GetName()
for p in PDGs:
  fname += "_" +  dbpdg.GetParticle(p).GetName()
fname += ".lund"

####################################################
####################################################
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

def applyCuts( particles ):
  test = True
  pdic = {}
  for p in particles:
    if p[3] in pdic:
      pdic[ p[3]+1000 ] = TVector3( p[6], p[7], p[8] ) 
    else:
      pdic[ p[3] ] = TVector3( p[6], p[7], p[8] ) 


  ## check electron angle
  #electron = pdic[ 11 ]
  #if electron.Theta()  < ThetaMin:
    #return False
  #if electron.Theta() > ThetaMax:
    #return False 

  pi = pdic[ 2212 ]
  if pi.Theta()  < ThetaMin:
    return False
  if pi.Theta() > ThetaMax:
    return False 

  pi = pdic[ -211 ]
  if pi.Theta()  < ThetaMin:
    return False
  if pi.Theta() > ThetaMax:
    return False 


  return test
####################################################
####################################################

# generate
with open(fname,"w") as fout:

  # loop on events
  i = 0
  while i  < NEVENTS:
    header = [99]*10
    header[0] = len(PDGs) + 1  # daughters + mother
    header[4] = BeamPolarization

    # array to store all the particles in the event    
    counter = 1
    parts = []

    # generate mother 
    P = PMin + rndm()*(PMax - PMin)
    Phi = PhiMin + rndm()*(PhiMax - PhiMin)
    Theta = ThetaMin + rndm()*(ThetaMax - ThetaMin)


    vm = TLorentzVector( P * math.sin(Theta) * math.cos(Phi),
                         P * math.sin(Theta) * math.sin(Phi),
                         P * math.cos(Theta), 
                         math.sqrt( P*P + MotherMass*MotherMass ) )

    
    r = abs( gauss( 0., VR ) )
    f = rndm()*2*math.pi
    vertex = (
              Vx + r * math.cos( f ),
              Vy + r * math.sin( f ),
              VzMin + rndm()*(VzMax - VzMin)
            )

    parts.append( storeParticle( PDGmother, vm, vertex, 0 ) )


    # prepare decay
    Decay = TGenPhaseSpace()
    Decay.SetDecay(vm,len(DaughterMass),array('d',DaughterMass))

    test = True
    Decay.Generate()
    for j,p in enumerate(PDGs):
    
      vp = Decay.GetDecay(j)
      
      parts.append( storeParticle( p, vp, vertex, 1 ) )
      if math.isnan( vp.Px() ) :
        test = False

    
    if test == False:
      continue

    if applyCuts( parts ) == False:
      continue


    i += 1

    for h in header:
      fout.write(str(h))
      fout.write(" ")
    fout.write("\n")
    writeParticles(fout,parts)

  fout.close()

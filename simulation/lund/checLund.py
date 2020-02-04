from ROOT import TLorentzVector, TDatabasePDG, TH1F, TH2F, TCanvas, TVector3
from math import *
import sys

with open( sys.argv[-1] ) as fin:
  content = fin.readlines()

pdg = TDatabasePDG()

h0 = TH1F( "m0", "", 200,0,3)
h1 = TH1F( "m", "", 200,0,3)

h2t = TH2F( "theta", "", 180,0,180,90,0,180)
h2p = TH2F( "phi",  "",  180,-180,180,180,-180,180)

i = 0
while i < len( content):

  line = content[i].strip().split()
  if len(line) == 10:
    g0line = content[i+1].strip().split()
    g1line = content[i+2].strip().split()
    g2line = content[i+3].strip().split()

    g0d = [ float(a) for a in g0line[6:9] ]
    g1d = [ float(a) for a in g1line[6:9] ]
    g2d = [ float(a) for a in g2line[6:9] ]

    m0 = pdg.GetParticle( int(g0line[3]) ).Mass()
    m1 = pdg.GetParticle( int(g1line[3]) ).Mass()
    m2 = pdg.GetParticle( int(g2line[3]) ).Mass()

    v0 = TLorentzVector( g0d[0], g0d[1],  g0d[2], sqrt(g0d[0]**2+g0d[1]**2+g0d[2]**2 + m0**2) )
    v1 = TLorentzVector( g1d[0], g1d[1],  g1d[2], sqrt(g1d[0]**2+g1d[1]**2+g1d[2]**2 + m1**2) )
    v2 = TLorentzVector( g2d[0], g2d[1],  g2d[2], sqrt(g2d[0]**2+g2d[1]**2+g2d[2]**2 + m2**2) )

    m = v1 + v2
    h1.Fill( m0 )
    h0.Fill( m.M() )
    #v0 = TVector3( g0d[0], g0d[1], g0d[2] )
    #v1 = TVector3( g1d[0], g1d[1], g1d[2] )

    h2t.Fill( degrees( v1.Theta() ), degrees( v2.Theta() ) )
    h2p.Fill( degrees( v1.Phi() ), degrees( v2.Phi() ) )  
  i += int(line[0]) + 1

c0 = TCanvas()
h1.Draw()
h0.SetLineColor(2)
h0.Draw("same")

c1 = TCanvas("theta","theta")
h2t.Draw("colz")

c2 = TCanvas("phi","phi")
h2p.Draw("colz")

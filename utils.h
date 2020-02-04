#include <TH1F.h>
#include <TFile.h>
#include <TH2F.h>

#include <TLorentzVector.h>
#include <TDatabasePDG.h>
#include <TMath.h>


#include "H4.h"

struct axis {
  const char *title; int nbins; float binl; float binh;
};

axis* createAxis( const char *title, int nbins, float binl, float binh ){
  axis *x = new axis();
  x->title = title;
  x->nbins = nbins;
  x->binl  = binl;
  x->binh  = binh;
  return x;
}


TH1F* createHisto( const char *name, const char *title, axis *x ){
  TH1F *h = new TH1F(name,title,x->nbins,x->binl,x->binh);
  h->SetXTitle(x->title);
  return h;
}

TH2F* create2DHisto( const char *name, const char *title, axis *x, axis *y ){
  TH2F *h = new TH2F(name,title,x->nbins,x->binl,x->binh,y->nbins,y->binl,y->binh);
  h->SetXTitle(x->title);
  h->SetYTitle(y->title);
  return h;
}

//// PDG database
// --------------
TDatabasePDG pdg;


TLorentzVector getVector( RecParticle &p ){

  float m = 0;
  if( pdg.GetParticle(p.pid) != 0x0 ){
    m = pdg.GetParticle(p.pid)->Mass();
  }
  else {
    printf( " ERROR, PDG value %d not present in database \n", p.pid );
  }
  
  TLorentzVector v( p.px, p.py, p.pz, TMath::Sqrt(  p.px*p.px + p.py*p.py + p.pz*p.pz + m*m ) );
  return v;
}

TLorentzVector getVector( MCPart &p ){

  float m = 0;
  if( pdg.GetParticle(p.pid) != 0x0 ){
    m = pdg.GetParticle(p.pid)->Mass();
  }
  else {
    printf( " ERROR, PDG value %d not present in database \n", p.pid );
  }
  
  TLorentzVector v( p.px, p.py, p.pz, TMath::Sqrt(  p.px*p.px + p.py*p.py + p.pz*p.pz + m*m ) );
  return v;
}


TLorentzVector getQ( TLorentzVector &beam, TLorentzVector &eout ) {
  return beam - eout;
}

TLorentzVector getW(  TLorentzVector &beam, TLorentzVector &eout , TLorentzVector &target ) {
  return getQ(beam, eout) + target;
}

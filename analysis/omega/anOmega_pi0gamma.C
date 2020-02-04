#include <cstdlib>
#include <iostream>
#include <cmath>
#include <map>

#include "reader.h"
#include "dictionary.h"
#include "bank.h"

#include <TMath.h>
#include <TH1F.h>
#include <TFile.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TNtuple.h>

#include "H4.h"

#include "utils.h"


//=========================================
// MAIN
//=========================================
int main(int argc, char** argv) {
  std::cout << " reading file example program (HIPO) " << std::endl;
  char inputFile[256];

  if(argc>1) {
    sprintf(inputFile,"%s",argv[1]);
  } else {
    std::cout << " *** please provide a file name..." << std::endl;
    exit(0);
  }



  // open reader
  // ------------
  hipo::reader  reader;
  reader.open(inputFile);

  // load bank dictionary
  // --------------------
  hipo::dictionary factory;
  reader.readDictionary( factory );



  // hipo event
  // ----------
  hipo::event event;


  // output file
  // -----------
  TFile *fout = TFile::Open("omega_pi0g.root","recreate");

  // output ntuple
  // ------------
  enum { kQ2,kW,keE,keTheta,kePhi,kMMep,kMM,kpi0M,kpi0Theta,kpi0Phi,kpi0P,kgTheta,kgPhi,kgP,
         kMomega,komegaTheta,komegaPhi,kMMepTheta,kMMepPhi,kpTheta,kpPhi,kpP,kMMpi0,kMMg,
         kNVARS };

  TNtuple *nt = new TNtuple("nt","foreff","Q2:W:eE:eTheta:ePhi:MMep:MM:pi0M:pi0Theta:pi0Phi:pi0P:gTheta:gPhi:gP:"\
"Momega:omegaTheta:omegaPhi:MMepTheta:MMepPhi:pTheta:pPhi:pP:MMpi0:MMg"
);

  // output histogram
  // ----------------
  axis *axbeta  = createAxis("beta", 100,-0.2,2);
  axis *axp     = createAxis("momentum", 100,0,10);

  TH2F *hBetaPip = create2DHisto( "hBetaPip", "pi+", axp, axbeta );
  TH2F *hBetaPim = create2DHisto( "hBetaPim", "pi-", axp, axbeta );

  //// PDG database
  TDatabasePDG pdg;


  // load banks
  // ----------
  RecTrack rectrk( "REC::Track", factory );
  RecParticle recp( "REC::Particle", factory );


  // initial state: e+p
  // ------------------
  TLorentzVector Ein( 0., 0., 10.56 , 10.56 );
  TLorentzVector Pin( 0., 0., 0., pdg.GetParticle(2212)->Mass() );

  // loop over events;
  // ----------------
  int entry = -1;
  while(reader.next()==true){
    entry++;
    //if( entry>5) return 0;
    if( entry%20000==0) std::cout << "event # " << entry << std::endl;

    // load event
    reader.read( event );
  
    // load banks
    event.getStructure( rectrk );
    event.getStructure( recp );

    std::map<int,std::vector<int>> partPerPid;
    for( int i = 0 ; i < recp.getSize() ; i++ ){
      recp.get(i);
      partPerPid[ recp.pid ].push_back( i );
    }

    // skip events with more than one electron
    if ( partPerPid[11].size() != 1 ) continue;
    if ( partPerPid[2212].size() != 1 ) continue;

    if ( partPerPid[22].size() < 3 ) continue;

    // output electron
    recp.get( partPerPid[11][0] );
    float Me = pdg.GetParticle(11)->Mass();
    float t = sqrt( recp.px*recp.px + recp.py*recp.py + recp.pz*recp.pz + Me*Me );
    TLorentzVector Eout = getVector( recp );

    // output proton
    recp.get( partPerPid[2212][0] );
    float Mp = pdg.GetParticle(2212)->Mass();
    t = sqrt( recp.px*recp.px + recp.py*recp.py + recp.pz*recp.pz + Mp*Mp );
    TLorentzVector Pout( recp.px, recp.py, recp.pz, t );

    // DIS variables
    TLorentzVector Q = Ein - Eout;
    TLorentzVector W = Q + Pin;

    if ( -Q.Mag2() < 1. ) continue;
    if ( W.Mag()   < 1.8 ) continue;

    // photons
    for( int i = 0; i < partPerPid[22].size(); i++ ){
      recp.get( partPerPid[22][i] );
      t = sqrt( recp.px*recp.px + recp.py*recp.py + recp.pz*recp.pz );
      if( t < 0.5 ) continue;
      TLorentzVector g1(  recp.px, recp.py, recp.pz, t );

      for( int j = i+1; j < partPerPid[22].size(); j++ ){
        recp.get( partPerPid[22][j] );
        t = sqrt( recp.px*recp.px + recp.py*recp.py + recp.pz*recp.pz );
        if( t < 0.5 ) continue;
        TLorentzVector g2(  recp.px, recp.py, recp.pz, t );
      
        auto pi0 = (g1 + g2 );
        if( pi0.M() < 0.075 || pi0.M() > .2 ) continue; 


        for( int k = 0; k < partPerPid[22].size(); k++ ){
          if( k == i ) continue;
          if( k == j ) continue;

          recp.get( partPerPid[22][k] );
          t = sqrt( recp.px*recp.px + recp.py*recp.py + recp.pz*recp.pz );
          if( t < 0.5 ) continue;
          TLorentzVector g3(  recp.px, recp.py, recp.pz, t );
        
          TLorentzVector MMep    = Ein + Pin - Eout - Pout ;
          TLorentzVector MM      = Ein + Pin - Eout - Pout  - g3 - pi0;
          TLorentzVector MMpi0   = Ein + Pin - Eout - Pout  - pi0;
          TLorentzVector MMg     = Ein + Pin - Eout - Pout  - g3;

          TLorentzVector Momega = g3 + pi0;

          if( Momega.M() > 2.5 || Momega.M() < .26 ) continue;
          if( MM.M2() > 1.5 || MM.M2() < -1.5 ) continue;
          if( MMep.M() < 0.1 || MMep.M() > 2.5 ) continue;

          //if( MM.Mag2() 

          float data[kNVARS];
          data[kQ2] = -Q.Mag2();
          data[kW] = W.Mag();
          data[keE] = Eout.Energy();
          data[keTheta] = Eout.Theta();
          data[kePhi] = Eout.Phi();
          data[kMMep] = MMep.M2();
          data[kMM] = MM.M2();
          data[kpi0M] = pi0.M();
          data[kpi0Theta] = pi0.Theta();
          data[kpi0Phi] = pi0.Phi();
          data[kpi0P] = pi0.P();
          data[kgTheta] = g3.Theta();
          data[kgPhi] = g3.Phi();
          data[kgP] = g3.P();
          data[kMomega] = Momega.M();
          data[komegaTheta] = Momega.Theta();
          data[komegaPhi] = Momega.Phi();
          data[kMMepTheta] = MMep.Theta();
          data[kMMepPhi] = MMep.Phi();
          data[kpTheta] = Pout.Theta();
          data[kpPhi] = Pout.Phi();
          data[kpP] = Pout.P();
          data[kMMpi0] = MMpi0.M2();
          data[kMMg] = MMg.M2();
         
          nt->Fill(data); 
        }

      }
    }



  } 
  //nmc->SaveAs( "tracks.root");
  fout->Write();
  fout->Close();
  return 0;
}


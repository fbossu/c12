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
  TFile *fout = TFile::Open("omega_pipipi0.root","recreate");

  // output ntuple
  // ------------
  enum { kP,kPT,kphi,ktheta,kZ,kd0,kndf,kchi2,kq,kPathLength,knSVT,knBMT,
        kNVARS };

  TNtuple *nt = new TNtuple("nt","foreff","Q2:W:eE:eTheta:ePhi:MMep:MM:MMpip:Momega:pi0M:pi0Theta:pipTheta:pipPhi:pipP:pimTheta:pimPhi:pimP"\
":omegaTheta:omegaPhi:MMepTheta:MMepPhi:pTheta:pPhi:pP:MMpim"
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

    // skip events without pi plus
    if ( partPerPid[211].size() < 1 ) continue;
    if ( partPerPid[-211].size() < 1 ) continue;

    if ( partPerPid[22].size() < 2 ) continue;

    // skip events with too many pions
    if ( partPerPid[211].size() > 1 || partPerPid[-211].size() > 1 ) continue;


    // output electron
    recp.get( partPerPid[11][0] );
    float Me = pdg.GetParticle(11)->Mass();
    float t = sqrt( recp.px*recp.px + recp.py*recp.py + recp.pz*recp.pz + Me*Me );
    //TLorentzVector Eout( recp.px, recp.py, recp.pz, t );
    TLorentzVector Eout = getVector( recp );

    // output proton
    recp.get( partPerPid[2212][0] );
    float Mp = pdg.GetParticle(2212)->Mass();
    t = sqrt( recp.px*recp.px + recp.py*recp.py + recp.pz*recp.pz + Mp*Mp );
    TLorentzVector Pout( recp.px, recp.py, recp.pz, t );


    // output pi plus
    recp.get( partPerPid[211][0] );
    float Mpi = pdg.GetParticle(211)->Mass();
    t = sqrt( recp.px*recp.px + recp.py*recp.py + recp.pz*recp.pz + Mpi*Mpi );
    TLorentzVector PiPlus( recp.px, recp.py, recp.pz, t );

    hBetaPip->Fill(  PiPlus.P(), recp.beta );


    recp.get( partPerPid[-211][0] );
    t = sqrt( recp.px*recp.px + recp.py*recp.py + recp.pz*recp.pz + Mpi*Mpi );
    TLorentzVector PiMinus( recp.px, recp.py, recp.pz, t );

    hBetaPim->Fill( PiMinus.P(), recp.beta );
    //TLorentzVector IM = PiPlus + PiMinusout;
 

    // photons
    std::vector<TLorentzVector> pi0s;
    
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
      
        auto tot = (g1 + g2 );
        if( tot.M() < 0.075 || tot.M() > .2 ) continue; 
        pi0s.push_back( (g1+g2) ); 
      }
    }

    if( pi0s.size() == 0 ) continue;

    // DIS variables
    TLorentzVector Q = Ein - Eout;
    TLorentzVector W = Q + Pin;

    if ( -Q.Mag2() < 1. ) continue;
    if ( W.Mag()   < 1.8 ) continue;

    for( auto pi0 : pi0s ){
    // missing mass
      TLorentzVector MMep    = Ein + Pin - Eout - Pout ;
      TLorentzVector MM      = Ein + Pin - Eout - Pout  - PiPlus - PiMinus - pi0;
      TLorentzVector MMpip   = Ein + Pin - Eout - Pout  - PiPlus;
      TLorentzVector MMpim   = Ein + Pin - Eout - Pout  - PiMinus;
      TLorentzVector MMpipi  = Ein + Pin - Eout - Pout  - PiPlus - PiMinus;

      TLorentzVector Momega = PiPlus + PiMinus + pi0;

      if( Momega.M() > 2.5 || Momega.M() < .05 ) continue;
      if( MM.M2() > 1.5 || MM.M2() < -1.5 ) continue;
      if( MMep.M() < 0.1 || MMep.M() > 2.5 ) continue;

      //if( MM.Mag2() 

      float data[25];
      data[0] = -Q.Mag2();
      data[1] = W.Mag();
      data[2] = Eout.Energy();
      data[3] = Eout.Theta();
      data[4] = Eout.Phi();
      data[5] = MMep.M2();
      data[6] = MM.M2();
      data[7] = MMpip.M2();
      data[8] = Momega.M();
      data[9] = pi0.M();
      data[10] = pi0.Theta();
      data[11] = PiPlus.Theta();
      data[12] = PiPlus.Phi();
      data[13] = PiPlus.P();
      data[14] = PiMinus.Theta();
      data[15] = PiMinus.Phi();
      data[16] = PiMinus.P();
      data[17] = Momega.Theta();
      data[18] = Momega.Phi();
      data[19] = MMep.Theta();
      data[20] = MMep.Phi();
      data[21] = Pout.Theta();
      data[22] = Pout.Phi();
      data[23] = Pout.P();
      data[24] = MMpim.M2();
     
      nt->Fill(data); 
    }
  } 
  //nmc->SaveAs( "tracks.root");
  fout->Write();
  fout->Close();
  return 0;
}


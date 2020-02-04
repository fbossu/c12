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
  TFile *fout = TFile::Open("lambda.root","recreate");

  // output ntuple
  // ------------
  enum { 
        kPp,kPTp,kphip,kthetap,kZp,
        kPpi,kPTpi,kphipi,kthetapi,kZpi,
        kMl, kPl,kPTl, kphil,kthetal,
        kNVARS };

  TNtuple *nt = new TNtuple("nt","lambda",
        "Pp:PTp:phip:thetap:Zp:"\
        "Ppi:PTpi:phipi:thetapi:Zpi:"\
        "Ml:Pl:PTl:phil:thetal"
);

  // output histogram
  // ----------------
  axis *axbeta  = createAxis("beta", 100,-0.2,2);
  axis *axp     = createAxis("momentum", 100,0,10);

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
    if( entry%2000==0) std::cout << "event # " << entry << std::endl;

    // load event
    reader.read( event );
  
    // load banks
    event.getStructure( rectrk );
    event.getStructure( recp );


    if( recp.getSize() < 2 ) continue;

    TVector3 vMCp;
    TVector3 vMCpi;

    std::vector<int> neg;
    std::vector<int> pos;
    for( int i = 0; i < recp.getSize(); i++ ){
      recp.get(i);
      if( recp.charge > 0 ) pos.push_back( i );
      if( recp.charge < 0 ) neg.push_back( i );
    }

    if( pos.size() < 1 ) continue;
    if( neg.size() < 1 ) continue;

    float Mp  = pdg.GetParticle(2212)->Mass();
    float Mpi = pdg.GetParticle(211)->Mass();
    for( int p : pos ){
      // output proton
      recp.get( p );
      float t = sqrt( recp.px*recp.px + recp.py*recp.py + recp.pz*recp.pz + Mp*Mp );
      TLorentzVector Pout;
      Pout.SetXYZM( recp.px, recp.py, recp.pz, Mp );
      float Zp = recp.vz;

      for( int n : neg ){
        recp.get( n );
        t = sqrt( recp.px*recp.px + recp.py*recp.py + recp.pz*recp.pz + Mpi*Mpi );
        TLorentzVector Pi;
        Pi.SetXYZM( recp.px, recp.py, recp.pz, Mpi );
        float Zpi = recp.vz;

        TLorentzVector l0 = Pi + Pout ;
      
        float data[kNVARS];
        data[kPp]     = Pout.P();
        data[kPTp]    = Pout.Pt();
        data[kphip]   = Pout.Phi();
        data[kthetap] = Pout.Theta();
        data[kZp]     = Zp;
        data[kPpi]     = Pi.P();
        data[kPTpi]    = Pi.Pt();
        data[kphipi]   = Pi.Phi();
        data[kthetapi] = Pi.Theta();
        data[kZpi]     = Zpi;
        data[kMl]     = l0.M();
        data[kPl]     = l0.P();
        data[kPTl]    = l0.Pt();
        data[kphil]   = l0.Phi();
        data[kthetal] = l0.Theta();
        nt->Fill(data); 
      }

    }




    
  } 
  //nmc->SaveAs( "tracks.root");
  fout->Write();
  fout->Close();
  return 0;
}


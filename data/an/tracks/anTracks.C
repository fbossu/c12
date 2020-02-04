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
  TFile *fout = TFile::Open("tracks.root","recreate");

  // output ntuple
  // ------------
  enum { kP,kPT,kphi,ktheta,kZ,kd0,kndf,kchi2,kq,kPathLength,knSVT,knBMT,
        kNVARS };

  TNtuple *nmc = new TNtuple("nt","foreff","P:PT:phi:theta:Z:d0:ndf:chi2:q:path:nSVT:nBMT"
                                            );

  // output histogram
  // ----------------
  axis *ntrks  = createAxis("ntrks", 10,0,10);
  TH1F *htrks = createHisto( "htrks","num of tracks",  ntrks );

  //// PDG database
  // --------------
  TDatabasePDG pdg;

  // load banks
  // ----------
    CVTTrack trk( "CVTRec::Tracks", factory );
    //RecTrack rectrk( "REC::Track", factory );
    Cross bmtC( "BMTRec::Crosses", factory );
    Cross svtC( "BSTRec::Crosses", factory );

  // loop over events;
  // ----------------
  int entry = -1;
  while(reader.next()==true){
    entry++;
    //if( entry>5) return 0;
    if( entry%500==0) std::cout << "event # " << entry << std::endl;

    // load event
    reader.read( event );
  
    // load banks
    event.getStructure( trk );
    //event.getStructure( rectrk );
    event.getStructure( bmtC );
    event.getStructure( svtC );


    //trk.show();
    
    htrks->Fill( trk.getSize() );

    //if( entry > 100 ) break;
    //skip events without CVT tracks
    if( trk.getSize() < 1 ) continue;
    //if( bmtC.getSize() < 1 ) continue;
    //if( svtC.getSize() < 1 ) continue;

    // first of all, create all the maps
    // ---------------------------------

    //// map REC Track to CVTRec Track
    //std::map<int,int> RecTrkToCvtTrk;
    //for( int i = 0; i < rectrk.getSize(); i++ ){
      //rectrk.get( i );
      //if( rectrk.detector/1000 == 4 ){ // this is CVT
        //RecTrkToCvtTrk[i] = rectrk.index;
      //}
    //}

    // maps crosses
    std::map<int,std::vector<int>> svtCtoTrk;
    for( int i = 0; i < svtC.getSize(); i++ ){
      svtC.get(i);
      svtCtoTrk[ svtC.tid ].push_back( svtC.id );
    }
    std::map<int,std::vector<int>> bmtCtoTrk;
    for( int i = 0; i < bmtC.getSize(); i++ ){
      bmtC.get(i);
      bmtCtoTrk[ bmtC.tid ].push_back( bmtC.id );
    }


    // loop over cvt tracks
    for( int i = 0; i < trk.getSize(); i++ ){
      trk.get( i );
      

      // create output buffer for:"P:PT:phi:theta:Z:d0:ndf:chi2:q:nSVT:nBMT:match:trueP:truePhi:trueTheta:trueZ:trueQ"
      float data[ kNVARS ];
      for( int k = 0; k < kNVARS; k++ ) data[k] = -9999.;

      data[ kP     ] = trk.p;
      data[ kPT    ] = trk.pt;
      data[ kphi   ] = trk.phi;
      data[ ktheta ] = trk.theta;
      data[ kZ     ] = trk.z0;
      data[ kd0    ] = trk.d0;
      data[ kndf   ] = trk.ndf;
      data[ kchi2  ] = trk.chi2;
      data[ kq     ] = trk.q;
      data[ kPathLength ] = trk.path;
      data[ knSVT  ] = svtCtoTrk[ i + 1 ].size() ;
      data[ knBMT  ] = bmtCtoTrk[ i + 1 ].size() ;
      nmc->Fill( data );
    }
    
    //if( iev > 100 ) break;
  } 
  //nmc->SaveAs( "tracks.root");
  fout->Write();
  fout->Close();
  return 0;
}


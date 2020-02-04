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

  // load banks
  // ----------
  RunConfig run("RUN::config", factory );
  MCPart mcp( "MC::Particle", factory );
  CVTTrack trk( "CVTRec::Tracks", factory );
  RecTrack rectrk( "REC::Track", factory );
  MCisMatch isM( "MC::IsParticleMatched", factory );
  MCMatchedCluster mcCls("MC::MatchedClusters", factory );
  
  Cross bmtC( "BMTRec::Crosses", factory );
  Cross svtC( "BSTRec::Crosses", factory );


  // hipo event
  // ----------
  hipo::event event;


  // output ntuple
  // ------------
  enum { kP,kPT,kphi,ktheta,kZ,kd0,kndf,kchi2,kq,knSVT,knBMT,
        kmatch,ktrueP,ktruePhi,ktrueTheta,ktrueZ,ktrueQ,
        kfraction,ktrueSVT,ktrueBMT,ktruePID,kNVARS};
  TNtuple *nmc = new TNtuple("nmc","foreff","P:PT:phi:theta:Z:d0:ndf:chi2:q:nSVT:nBMT"\
                                            ":match:trueP:truePhi:trueTheta:trueZ:trueQ"\
                                            ":fraction:trueSVT:trueBMT:truePID");

  //// PDG database
  // --------------
  TDatabasePDG pdg;

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
    event.getStructure( run );
    event.getStructure( mcp );
    event.getStructure( trk );
    event.getStructure( rectrk );
    event.getStructure( isM );
    event.getStructure( mcCls );
    event.getStructure( bmtC );
    event.getStructure( svtC );

    //if( trk.getSize() < 1 ) continue;
    run.get(0);
    // first of all, create all the maps
    // ---------------------------------

    // map REC Track to CVTRec Track
    std::map<int,int> RecTrkToCvtTrk;
    for( int i = 0; i < rectrk.getSize(); i++ ){
      rectrk.get( i );
      if( rectrk.detector/1000 == 4 ){ // this is CVT
        RecTrkToCvtTrk[i] = rectrk.index;
      }
    }
    
    // map isMatch to MC tid
    std::map<int,int> mcToIsMatch;
    for( int i = 0; i < isM.getSize(); i++ ){
      isM.get(i);
      mcToIsMatch[ isM.mctid ] = i;
    } 

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

    // count BMT and BST clusters to MC tid
    std::map<int,std::pair<int,int>> mcToMatchCls;
    for( int i = 0; i < mcCls.getSize(); i++ ){
      mcCls.get(i);
      if( mcToMatchCls.count( mcCls.mctid ) == 0 ){
        mcToMatchCls[ mcCls.mctid ].first  = 0;
        mcToMatchCls[ mcCls.mctid ].second = 0;
      }
      if( mcCls.detector == 1 ){
        mcToMatchCls[ mcCls.mctid ].first += 1;
      }
      else if( mcCls.detector == 2 ){
        mcToMatchCls[ mcCls.mctid ].second += 1;
      }
      else { }

    }


    // check if mcp exist
    if( mcp.getSize() == 0 ) continue;

    // loop over generated particles
    for( int i = 0; i < mcp.getSize(); i++ ){
      mcp.get( i );
      
      // get the isParticleMatched corresponding to this mc particle
      isM.get(  mcToIsMatch[ (i+1)] );

      if( isM.isInAcc < 1 ) continue;

      // create output buffer for:"P:PT:phi:theta:Z:d0:ndf:chi2:q:nSVT:nBMT:match:trueP:truePhi:trueTheta:trueZ:trueQ"
      float data[ kNVARS ];
      for( int k = 0; k < kNVARS; k++ ) data[k] = -9999.;
 
      // compute and fill "true" info
      TVector3 P( mcp.px, mcp.py, mcp.pz );
      data[ ktrueP     ] = P.Mag();
      data[ ktruePhi   ] = TMath::RadToDeg() * P.Phi();
      data[ ktrueTheta ] = TMath::RadToDeg() * P.Theta();
      data[ ktrueZ     ] = mcp.vz;
      data[ ktrueQ     ] = pdg.GetParticle( mcp.pid )->Charge()/3;
      data[ kfraction  ] = isM.fraction;
      data[ ktrueSVT   ] = mcToMatchCls[ i+1 ].second;
      data[ ktrueBMT   ] = mcToMatchCls[ i+1 ].first;
      data[ ktruePID   ] = mcp.pid;
      
      if( isM.rectid >= 0 ) {
        data[ kmatch ] = 1.;

        int cvtTid =  RecTrkToCvtTrk[ isM.rectid ] ;
        trk.get( cvtTid );
        
        data[ kP     ] = trk.p;
        data[ kPT    ] = trk.pt;
        data[ kphi   ] = trk.phi;
        data[ ktheta ] = trk.theta;
        data[ kZ     ] = trk.z0;
        data[ kd0    ] = trk.d0;
        data[ kndf   ] = trk.ndf;
        data[ kchi2  ] = trk.chi2;
        data[ kq     ] = trk.q;
        data[ knSVT  ] = svtCtoTrk[ cvtTid + 1 ].size() ;
        data[ knBMT  ] = bmtCtoTrk[ cvtTid + 1 ].size() ;

        if( data[knBMT] < 2 ) std::cout<<" [WARNING] too few bmt in event: " << run.event << std::endl; 
        if( data[knSVT] < 1 ) std::cout<<" [WARNING] too few svt in event: " << run.event << std::endl; 
      }
      nmc->Fill( data );
    }
    
    //if( iev > 100 ) break;
  } 
  nmc->SaveAs( "eff4.root");
  //fout->Close();
  return 0;
}


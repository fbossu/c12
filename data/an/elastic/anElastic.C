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
pdg.AddParticle("deuteron","deuteron", 1.8791300, true, 0,1, "", 45);


  // Beam and Target
  TLorentzVector beam( 0., 0., 6.5, 6.5);
  TLorentzVector target( 0., 0., 0., 0.938);


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
  TFile *fout = TFile::Open("el.root","recreate");

  // output ntuple
  // ------------
  enum { kP,kphi,ktheta,kZ,kd0,kndf,kchi2,kq,kPathLength,knSVT,knBMT,
        kNVARS };

  //TNtuple *nmc = new TNtuple("nt","foreff","P:PT:phi:theta:Z:d0:ndf:chi2:q:path:nSVT:nBMT"
  //);

  // output histogram
  // ----------------
  axis *ntrks  = createAxis("ntrks", 10,0,10);
  TH1F *htrks = createHisto( "htrks","num of tracks",  ntrks );


  axis *phi    = createAxis( "phi", 180, -180, 180);
  axis *thetaP = createAxis( "#theta_{p}", 180, 20, 100);
  axis *thetaE = createAxis( "#theta_{e}", 180, 0, 50);
  axis *momentum = createAxis( "p", 100, 0, 5);

  TH2F *h2TeTp   = create2DHisto( "h2TeTp", "#theta_{e} vs #theta_{p}", thetaP, thetaE );
  TH2F *h2FeFp   = create2DHisto( "h2FeFp", "#varphi_{e} vs #varphi_{p}", phi, phi );
 
  axis *dtheta = createAxis("#Delta #theta(Q-p)", 200,-10,10);
  TH1F *h1dTheta = createHisto( "h1dTheta", "#Delta #theta Q - p", dtheta );

  axis *dphi = createAxis("#Delta #varphi(Q-p)", 200,-10,10);
  TH1F *h1dPhi = createHisto( "h1dPhi", "#Delta #varphi Q - p", dphi );

  TH1F *h1dPhiSec[6];
  for( int i=0;i<6;i++) h1dPhiSec[i] =  createHisto( Form("h1dPhiSec%d",i+1), Form("#Delta #varphi Q - p; Sec %d",i+1), dphi ); 
  
  TH1F *h1dThetaSec[6];
  for( int i=0;i<6;i++) h1dThetaSec[i] =  createHisto( Form("h1dThetaSec%d",i+1), Form("#Delta #theta Q - p; Sec %d",i+1), dtheta ); 

  TH2F *h2ThetaVsP[6];
  for( int i=0;i<6;i++) h2ThetaVsP[i] =  create2DHisto( Form("h2ThetaVsP%d",i+1), Form("#theta vs P; Sec %d",i+1), momentum, thetaP ); 

  TH2F *h2MCThetaVsP = create2DHisto( "h2MCThetaVsP","",momentum,thetaP);

  
  axis *qaxis = createAxis( "Q^{2}", 100,0,12);
  axis *waxis = createAxis( "W", 100,0,5);
 
  TH2F *hQ2W = create2DHisto( "hQ2W","Q2 vs W",waxis, qaxis); 


  axis *zaxis = createAxis("z (cm)", 200, -10,10);
  TH2F *hVeVpZ = create2DHisto( "hVeVpZ"," Ve vs Vp", zaxis, zaxis);

  axis *xaxis = createAxis("x (cm)", 200, -3,3);
  TH2F *hVeVpX = create2DHisto( "hVeVpX"," Ve vs Vp", xaxis, xaxis);

  axis *yaxis = createAxis("y (cm)", 200, -3,3);
  TH2F *hVeVpY = create2DHisto( "hVeVpY"," Ve vs Vp", yaxis, yaxis);

  axis *d0axis = createAxis("d0 (cm)", 200, -3,3);
  TH2F *hVeVpd0 = create2DHisto( "hVeVpd0","d0 Ve vs Vp", d0axis, d0axis);
  // load banks
  // ----------
  //CVTTrack trk( "CVTRec::Tracks", factory );
  RecTrack rectrk( "REC::Track", factory );
  RecParticle recpart( "REC::Particle", factory );
  //Cross bmtC( "BMTRec::Crosses", factory );
  //Cross svtC( "BSTRec::Crosses", factory );

  MCPart mcp( "MC::Particle", factory );
  // loop over events;
  // ----------------
  int entry = -1;
  while(reader.next()==true){
    entry++;
    //if( entry>5) return 0;
    if( entry%5000==0) std::cout << "event # " << entry << std::endl;

    // load event
    reader.read( event );
  
    // load banks
    //event.getStructure( trk );
    event.getStructure( rectrk );
    event.getStructure( recpart );
    event.getStructure( mcp );
    //event.getStructure( bmtC );
    //event.getStructure( svtC );


    //std::cout<< trk.getSize() << " == "  << entry <<std::endl;
    //trk.show();
    
    //htrks->Fill( trk.getSize() );

    //if( entry > 100 ) break;
    //skip events without CVT tracks
    if( recpart.getSize() < 1 ) continue;
    //if( trk.getSize() < 1 ) continue;

    // first of all, create all the maps
    // ---------------------------------

    std::map<int,int> recPartToTrk;
    std::map<int,int> recTrkToPart;

    for( int i=0; i< rectrk.getSize(); i++ ){
      rectrk.get( i );
      recPartToTrk[ rectrk.pindex ] = i; 
      recTrkToPart[ i ] = rectrk.pindex ; 
    }

    //// loop over tracks to look for negative in the FD and positive in the CD
    std::vector<int> etrk;
    std::vector<int> ptrk;
    for( int i = 0; i < rectrk.getSize(); i++ ){
      rectrk.get( i );
      switch( rectrk.detector){
        case 5:
          if( rectrk.q > 0 ) ptrk.push_back(i);
          break;
        case 6:
          if( rectrk.q < 0 ) etrk.push_back(i);
          break;
        default:
          break;
      }
    }

    // if MC
    // -----
    if( mcp.getSize() > 0 ){
      for( int a = 0; a < mcp.getSize(); a++ ){
        mcp.get(a);
        if( mcp.pid == 2212 ){
          TLorentzVector pmc = getVector( mcp );
          h2MCThetaVsP->Fill( pmc.P(), TMath::RadToDeg() * pmc.Theta() );
        }
      }
    }     
    // -----



    // loop over all the combinations
    for( int ie : etrk ){
      rectrk.get(ie);
      recpart.get( rectrk.pindex );

      bool id = false;
      if( TMath::Abs(recpart.pid) == 11 ) id = true;
      if( id == false ) continue;

      TVector3 ve( recpart.vx, recpart.vy,recpart.vz );
      TLorentzVector pe = getVector( recpart );

      auto Q = getQ( beam, pe );
      auto W = getW( beam, pe, target);

      hQ2W->Fill( W.Mag(), -Q.Mag2() );

      if( W.Mag() > 1.1 ) continue;
    
      int esector = rectrk.sector;
  
      for( int ip : ptrk ){
        rectrk.get(ip);
        recpart.get( rectrk.pindex );
        TVector3 vp( recpart.vx, recpart.vy,recpart.vz );
        TLorentzVector pp = getVector( recpart );

        if( TMath::Abs(recpart.pid) == 2212 ) id &= true;
        else id &= false;
        //if( id == false ) continue;

       
        h2FeFp->Fill( TMath::RadToDeg() * pp.Phi(),   TMath::RadToDeg() * pe.Phi() );
        h2TeTp->Fill( TMath::RadToDeg() * pp.Theta(),   TMath::RadToDeg() * pe.Theta() );
        h1dTheta->Fill( TMath::RadToDeg() * ( Q.Theta() - pp.Theta() ) );
        h1dPhi->Fill( TMath::RadToDeg() * ( Q.Phi() - pp.Phi() ) );
        
        
        h1dPhiSec[esector-1]->Fill( TMath::RadToDeg() * ( Q.Phi() - pp.Phi() ) );
        h1dThetaSec[esector-1]->Fill( TMath::RadToDeg() * ( Q.Theta() - pp.Theta() ) );


        h2ThetaVsP[esector-1]->Fill( pp.P(),  TMath::RadToDeg() *pp.Theta() );

        hVeVpZ->Fill( vp.Z(), ve.Z());
        hVeVpX->Fill( vp.X(), ve.X());
        hVeVpY->Fill( vp.Y(), ve.Y());
        hVeVpd0->Fill( vp.Perp(), ve.Perp());
      }
      
    }

 
      

      //// create output buffer for:"P:PT:phi:theta:Z:d0:ndf:chi2:q:nSVT:nBMT:match:trueP:truePhi:trueTheta:trueZ:trueQ"
      //float data[ kNVARS ];
      //for( int k = 0; k < kNVARS; k++ ) data[k] = -9999.;

      //data[ kP     ] = trk.p;
      //data[ kPT    ] = trk.pt;
      //data[ kphi   ] = trk.phi;
      //data[ ktheta ] = trk.theta;
      //data[ kZ     ] = trk.z0;
      //data[ kd0    ] = trk.d0;
      //data[ kndf   ] = trk.ndf;
      //data[ kchi2  ] = trk.chi2;
      //data[ kq     ] = trk.q;
      //data[ kPathLength ] = trk.path;
      //data[ knSVT  ] = svtCtoTrk[ i + 1 ].size() ;
      //data[ knBMT  ] = bmtCtoTrk[ i + 1 ].size() ;
      //nmc->Fill( data );
      //}
    
    //if( iev > 100 ) break;
  } 
  //nmc->SaveAs( "tracks.root");
  fout->Write();
  fout->Close();
  return 0;
}


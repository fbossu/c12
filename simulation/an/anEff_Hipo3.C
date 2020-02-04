#include <cstdlib>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "reader.h"
#include "node.h"

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

#include "H3.h"
enum { kP,kPT,kphi,ktheta,kZ,kd0,kndf,kchi2,kq,knSVT,knBMT,kmatch,ktrueP,ktruePhi,ktrueTheta,ktrueZ,ktrueQ};

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

  int NSIGMA = 4;
  if( argc>2 ) NSIGMA = atoi( argv[2] );

  std::cout << " NSIGMA " << NSIGMA << std::endl;

  hipo::reader  reader;
  reader.open(inputFile);

 
  MCPart mcp;
  mcp.init( "MC::Particle", reader );

  CVTTrack trk;
  trk.init( "CVTRec::Tracks", reader );

  RecTrack rectrk;
  rectrk.init( "REC::Track", reader );

  MCisMatch isM;
  isM.init( "MC::IsParticleMatched", reader );

  TNtuple *nmc = new TNtuple("nmc","foreff","P:PT:phi:theta:Z:d0:ndf:chi2:q:nSVT:nBMT:match:trueP:truePhi:trueTheta:trueZ:trueQ");
  //// PDG database
  TDatabasePDG pdg;

  int iev = -1;
  while( reader.next() == true ){
    iev++;
    // first of all, create all the maps
    std::map<int,int> RecTrkToCvtTrk;
    for( int i = 0; i < rectrk.getSize(); i++ ){
      rectrk.get( i );
      if( rectrk.detector/1000 == 4 ){ // this is CVT
        RecTrkToCvtTrk[i] = rectrk.index;
      }
    }
    
    std::map<int,int> mcToIsMatch;
    for( int i = 0; i < isM.getSize(); i++ ){
      isM.get(i);
      mcToIsMatch[ isM.mctid ] = i;
    } 

    // check if mcp exist
    if( mcp.getSize() == 0 ) continue;

    // loop over generated particles
    for( int i = 0; i < mcp.getSize(); i++ ){
      mcp.get( i );
      
      // get the isParticleMatched corresponding to this mc particle
      isM.get(  mcToIsMatch[ (i+1)] );

      // create output buffer for:"P:PT:phi:theta:Z:d0:ndf:chi2:q:nSVT:nBMT:match:trueP:truePhi:trueTheta:trueZ:trueQ"
      float data[17];
      for( int k = 0; k<17; k++ ) data[k] = -9999.;
 
      // compute and fill "true" info
      TVector3 P( mcp.px, mcp.py, mcp.pz );
      data[ ktrueP     ] = P.Mag();
      data[ ktruePhi   ] = TMath::RadToDeg() * P.Phi();
      data[ ktrueTheta ] = TMath::RadToDeg() * P.Theta();
      data[ ktrueZ     ] = mcp.vz;
      data[ ktrueQ     ] = pdg.GetParticle( mcp.pid )->Charge()/3;
      
      if( isM.rectid >= 0 ) {
        data[ kmatch ] = 1.;

        trk.get(  RecTrkToCvtTrk[ isM.rectid ] );
        
        data[ kP     ] = trk.p;
        data[ kPT    ] = trk.pt;
        data[ kphi   ] = trk.phi;
        data[ ktheta ] = trk.theta;
        data[ kZ     ] = trk.z0;
        data[ kd0    ] = trk.d0;
        data[ kndf   ] = trk.ndf;
        data[ kchi2  ] = trk.chi2;
        data[ kq     ] = trk.q;
        data[ knSVT  ] = trk.nSVT;
        data[ knBMT  ] = trk.nBMT;
      }
      nmc->Fill( data );
    }
    
    //if( iev > 100 ) break;
  }
  nmc->SaveAs( "eff.root" ); 
  return 0;
}



  //TFile *fout = TFile::Open(sout,"recreate"); 
  //TNtuple *ntr = new TNtuple("ntr","rec","P:PT:phi:theta:Z:d0:ndf:chi2:nSVT:nBMT:match:trueP:truePhi:trueTheta:trueZ");
  //TNtuple *nmc = new TNtuple("nmc","foreff","P:PT:phi:theta:Z:d0:ndf:chi2:nSVT:nBMT:match:trueP:truePhi:trueTheta:trueZ:trueQ");

  //// PDG database
  //TDatabasePDG pdg;

  ////----------------------------------------------------
  ////--  Main LOOP running through events and printing
  ////--  values of the first decalred branch
  ////----------------------------------------------------
  //int entry = 0;
  //while(reader.next()==true){
    //entry++;
    //if( entry%500==0) std::cout << "event # " << entry << std::endl;


    //float P_True;
    //float Theta_True;
    //float Phi_True;
    //float Z_True;
    //float PID_True;
    //// fill MC particle info
    //int n_mcp_pid = mcp_pid->getLength();
    //for(int b = 0; b < n_mcp_pid; b++){
      //TVector3 P( mcp_px->getValue(b), mcp_py->getValue(b), mcp_pz->getValue(b));
      //hmcp->Fill( P.Mag() );
      //hmcpxpy->Fill( P.Mag(), P.Perp() );
      //P_True = P.Mag();
      //Theta_True = TMath::RadToDeg() * P.Theta();
      //Phi_True =   TMath::RadToDeg() * P.Phi();
      //Z_True = mcp_vz->getValue(b);
      //PID_True = (float) mcp_pid->getValue(b);

    

      //// reconsturcted track
      //// -------------------
      //bool matchedForEfficiency = false;
      //float data[15];
          //// MC info
          //data[11] = P_True;
          //data[12] = Phi_True;
          //data[13] = Theta_True;
          //data[14] = Z_True;
          //data[15] = pdg.GetParticle(PID_True)->Charge()/3;

      //int n_trk = trk_ID->getLength();
      //for( int i=0;i<n_trk;i++){

        //float nsvt = 0;
        //float nbmt = 0;
        //// count crosses
        //for( int j=0;j<9;j++){
          //int c = crs[j]->getValue(i);
          //if( c == 0 ) continue;
          //if( c <1000 ) nsvt++;
          //else nbmt++;
        //}
        //// fill output dat afor ntuple
        //data[0] = trk_p->getValue(i);
        //data[1] = trk_pt->getValue(i);
        ////data[2] = TMath::RadToDeg() * std::remainder( trk_phi0->getValue(i), 2*M_PI);
        //data[2] = TMath::RadToDeg() * trk_phi0->getValue(i);
////        if( data[2] > 180. )
////          data[2] = TMath::RadToDeg() * ( trk_phi0->getValue(i) - 2. * TMath::Pi() );
        //data[3] = TMath::RadToDeg()*(TMath::Pi()/2-TMath::ATan(trk_tandip->getValue(i)));
        //data[4] = 10.* trk_z0->getValue(i);
        //data[5] = 10.* trk_d0->getValue(i);
        //data[6] = (float)trk_ndf->getValue(i);
        //data[7] = trk_chi2->getValue(i);
        //data[8] = (float)nsvt;
        //data[9] = (float)nbmt;

        //if ( trk_q->getValue(i) != pdg.GetParticle(PID_True)->Charge()/3 )  continue;   // charge match

        //// match?
        //data[10] = 0.;
        //if( 
           //( TMath::Abs( data[0] - P_True)/P_True < NSIGMA * 0.03 ) && // P matching
           //( TMath::Abs( data[2] - Phi_True) < NSIGMA * 0.118 )     && // phi match
           //( TMath::Abs( data[3] - Theta_True) < NSIGMA * 0.205 )   && // theta match
           //( trk_q->getValue(i) == pdg.GetParticle(PID_True)->Charge()/3 )         // charge match
          //){
          //if( matchedForEfficiency == false ){
            //data[10]  = 1.;
            //nmc->Fill(data);
            //matchedForEfficiency = true;
          //}
        //}
        //ntr->Fill(data);
      //} // loop over rec track
      //if( matchedForEfficiency == false ){
        //for( int k=0;k<10;k++) data[k] = -99999; 
        //nmc->Fill(data);
      //}
    

    //}
  //}
  ////ntr->Write();
  ////nmc->Write();
  ////ntr->SaveAs("ntr.root"); 
  //fout->Write();
  //fout->Close();
////  hmcpxpy->Draw("colz");
  ////hrecp->Draw();
////  std::cout << ">";
////  int a;
////  std::cin >> a;
////  exit(0);
////  theApp.Run();

  ////----------------------------------------------------
//}
////###### ENF OF GENERATED FILE #######


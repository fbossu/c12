#include <cstdlib>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "reader.h"
#include "node.h"

#include <TMath.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TNtuple.h>

#include "H3.h"
#include "htools.h"



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

  int NSECTORS = 3;
  int NLAYERS  = 6;
  axis *secAxis  = createAxis( "sector", NSECTORS,0.5,NSECTORS+0.5 );
  axis *layAxis  = createAxis( "layer",  NLAYERS,0.5, NLAYERS+0.5 );
  axis *sizeAxis = createAxis( "size", 20,0.5,20.5 );
  axis *timeAxis = createAxis( "time (ns)", 75,0,500 );
  axis *countsAxis = createAxis( "counts", 21,-0.5,20.5 );
  axis *centroidAxis   = createAxis( "centroid", 750,0,1500 );
  axis *residualAxis   = createAxis( "residual (mm)", 500,-2,2 );

  TH2F *H2TIME[NSECTORS];
  for( int i = 0; i < NSECTORS ; i++ ) H2TIME[i] = create2DHisto( Form("timeL%d",i+1),"", layAxis, timeAxis );

  TH2F *H2BKGTIME[NSECTORS];
  for( int i = 0; i < NSECTORS ; i++ ) H2BKGTIME[i] = create2DHisto( Form("bkgtimeL%d",i+1),"", layAxis, timeAxis );

  TH2F *H2[NLAYERS];
  for( int i = 0; i < NLAYERS ; i++ ) H2[i] = create2DHisto( Form("L%d",i+1),"", secAxis, countsAxis );

  TH2F *H2S[NLAYERS];
  for( int i = 0; i < NLAYERS ; i++ ) H2S[i] = create2DHisto( Form("sizeL%d",i+1),"", secAxis, sizeAxis );

  TH1F *HCent[NLAYERS][NSECTORS];
  for( int j=0; j<NSECTORS;j++) 
    for( int i = 0; i < NLAYERS ; i++ ) HCent[i][j] = createHisto( Form("centL%dS%d",i+1,j+1),"", centroidAxis );

  TH1F *hResidual[NLAYERS][NSECTORS];
  for( int j=0; j<NSECTORS;j++) 
    for( int i = 0; i < NLAYERS ; i++ ) hResidual[i][j] = createHisto( Form("residualL%dS%d",i+1,j+1),"", residualAxis );
  

  hipo::reader  reader;
  reader.open(inputFile);

  Cluster rcl;
  //rcl.init( "BSTRec::Clusters", reader);
  rcl.init( "BMTRec::Clusters", reader);

  CVTTrack trk;
  trk.init( "CVTRec::Tracks", reader );

  //CVTCosmic trk;
  //trk.init( "CVTRec::Cosmics", reader );

  int iev = -1;
  while( reader.next() == true ){
    iev++;
    if( iev == 0 ) continue;

    int count[NLAYERS][NSECTORS];
    for( int i = 0; i < NLAYERS; i++){
      for( int l = 0; l < NSECTORS; l++){
        count[i][l] = 0;
      }
    }
    if( rcl.getSize() < 1 ) continue;
    if( iev%1000 == 0 ) std::cout << " EVENT  " << iev << std::endl;
    for( int j = 0; j < rcl.getSize(); j++ ){
      rcl.get(j);

      //if( rcl.sector == 3 && rcl.layer == 4 && abs(rcl.centroid - 757) <0.15) continue;  

      count[ rcl.layer-1 ][ rcl.sector-1 ] += 1;


      HCent[ rcl.layer-1 ][ rcl.sector-1 ]->Fill( rcl.centroid );
      H2S[ rcl.layer-1 ]->Fill( rcl.sector, rcl.size );

      if( rcl.tid > 0 ){
        
        
        trk.get( rcl.tid-1 );
        if( trk.nBMT < 6 ) continue;
        
        hResidual[ rcl.layer-1 ][ rcl.sector-1 ]->Fill( rcl.centroidResidual );
      }
      if( rcl.tid < 0 ){
        H2BKGTIME[ rcl.sector-1 ]->Fill( rcl.layer, rcl.Tmin );
      }
      else{
        H2TIME[ rcl.sector-1 ]->Fill( rcl.layer, rcl.Tmin );
      }
      for( int i = 0; i < NLAYERS; i++){
        for( int l = 0; l < NSECTORS; l++){
          if( count[i][l] > 100 ) std::cout<< "** " << i << " " << l << "  -  "<< count[i][l]  << std::endl;
          H2[ i ]->Fill( l+1, count[i][l] );
        }
      }
    }
  

    //if( iev > 15 ) break;
  }

  TFile f("out.root", "recreate");
  for( int i = 0; i < NLAYERS ; i++ ) H2[i]->SetDirectory(&f);
  for( int i = 0; i < NSECTORS ; i++ ) H2TIME[i]->SetDirectory(&f);
  for( int i = 0; i < NSECTORS ; i++ ) H2BKGTIME[i]->SetDirectory(&f);
  for( int i = 0; i < NLAYERS ; i++ ) H2S[i]->SetDirectory(&f);
  
  for( int j=0; j<NSECTORS;j++) 
    for( int i = 0; i < NLAYERS ; i++ ){
       HCent[i][j]->SetDirectory(&f);
       hResidual[i][j]->SetDirectory(&f);
    }
  f.Write();
  f.Close();

  return 0;
}



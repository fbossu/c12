#include <cstdlib>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "reader.h"

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

//class Cls : public Bank {

  //public:
    //Cls( std::string s, hipo::dictionary d) : Bank(s,d) {} ;
    //void get( int i ){
      //sector = getInt( "sector", i );
      //layer  = getInt( "layer", i );
      //size   = getInt( "size", i );
      //tid  = getInt( "trkID", i );
      //centroid  = getFloat( "centroid", i );
      //centroidResidual = getFloat( "centroidResidual", i );
      //Tmin = getFloat( "Tmin", i );
      //Tmax = getFloat( "Tmax", i );
    //}
    //int sector;
    //int layer;
    //int size;
    //int tid;
    //float centroid;
    //float Tmin;
    //float Tmax;
    //float centroidResidual;
//};


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
  axis *countsAxis = createAxis( "counts", 25,-0.5,24.5 );
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
  Cls rcl("BMTRec::Clusters", factory );
  CVTTrack trk( "CVTRec::Tracks", factory );

  // hipo event
  // ----------
  hipo::event event;


  int iev = -1;
  while( reader.next() == true ){
    iev++;
    if( iev%500==0) std::cout << "event # " << iev << std::endl;
    // load event
    reader.read( event );
  
    // load banks
    event.getStructure( rcl );
    event.getStructure( trk );

    if( rcl.getSize() < 1 ) continue;
    int count[NLAYERS][NSECTORS];
    for( int i = 0; i < NLAYERS; i++){
      for( int l = 0; l < NSECTORS; l++){
        count[i][l] = 0;
      }
    }

    for( int j = 0; j < rcl.getSize(); j++ ){
      rcl.get(j);
      count[ rcl.layer-1 ][ rcl.sector-1 ] += 1;
  
      HCent[ rcl.layer-1 ][ rcl.sector-1 ]->Fill( rcl.centroid );
    
      //if( rcl.tid > -1 ){
        //trk.get( rcl.tid );
        //hResidual[ rcl.layer-1 ][ rcl.sector-1 ]->Fill( rcl.centroidResidual );
        //if( trk.ndf < 4 ) continue;
        //if( trk.q < 0 )
          //H2SM[ rcl.layer-1 ]->Fill( rcl.sector, rcl.size );
        //else
          //H2SP[ rcl.layer-1 ]->Fill( rcl.sector, rcl.size );
      //}

      H2S[ rcl.layer-1 ]->Fill( rcl.sector, rcl.size );

      if( rcl.tid > 0 ){
        trk.get( rcl.tid );
        hResidual[ rcl.layer-1 ][ rcl.sector-1 ]->Fill( rcl.centroidResidual );
        H2TIME[ rcl.sector-1 ]->Fill( rcl.layer, rcl.Tmin );
      }
      else{
        H2BKGTIME[ rcl.sector-1 ]->Fill( rcl.layer, rcl.Tmin );
      }

    }
  
    for( int i = 0; i < NLAYERS; i++){
      for( int l = 0; l < NSECTORS; l++){
        if( count[i][l] > 100 ) std::cout<< "** " << i << " " << l << "  -  "<< count[i][l]  << std::endl;
        H2[ i ]->Fill( l+1, count[i][l] );
      }
    }
    //if( iev > 15 ) break;
  }

  TFile f("out.root", "recreate");
  for( int i = 0; i < NLAYERS ; i++ ) H2[i]->SetDirectory(&f);
  //for( int i = 0; i < NLAYERS ; i++ ) H2SM[i]->SetDirectory(&f);
  //for( int i = 0; i < NLAYERS ; i++ ) H2SP[i]->SetDirectory(&f);
  for( int i = 0; i < NLAYERS ; i++ ) H2S[i]->SetDirectory(&f);
  
  for( int i = 0; i < NSECTORS ; i++ ) H2TIME[i]->SetDirectory(&f);
  for( int i = 0; i < NSECTORS ; i++ ) H2BKGTIME[i]->SetDirectory(&f);

  for( int j=0; j<NSECTORS;j++) 
    for( int i = 0; i < NLAYERS ; i++ ){
       HCent[i][j]->SetDirectory(&f);
       hResidual[i][j]->SetDirectory(&f);
    }

  f.Write();
  f.Close();

  return 0;
}



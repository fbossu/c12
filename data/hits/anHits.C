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

  int NLAYERS  = 6;
  axis *layAxis  = createAxis( "layer",  NLAYERS,0.5, NLAYERS+0.5 );
  axis *timeAxis = createAxis( "time (ns)", 75,0,500 );
  axis *stripAxis  = createAxis( "strip",  1200,0.5, 1200+0.5 );

  TH2F *h2Time;
  h2Time = create2DHisto( "htime","time of max", layAxis, timeAxis );

  TH1F *hStrip[NLAYERS];
  for( int i = 0; i < NLAYERS ; i++ ) hStrip[i] = createHisto( Form("stripL%d",i+1),"", stripAxis );

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
  Hit rhit("FMT::adc", factory );

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
    event.getStructure( rhit );

    for( int j = 0; j < rhit.getSize() ; j++ ){
      // load the i-th row of the bank
      rhit.get( j ) ;

      int layer = rhit.layer;
      int strip = rhit.component;
      float t   = rhit.time;

      hStrip[ layer-1 ]->Fill( strip );
      h2Time->Fill( layer, t );   
    }

    //if( iev > 15 ) break;
  }

  TFile f("out.root", "recreate");
  for( int i = 0; i < NLAYERS ; i++ ) hStrip[i]->SetDirectory(&f);
  h2Time->SetDirectory(&f);

  f.Write();
  f.Close();

  return 0;
}



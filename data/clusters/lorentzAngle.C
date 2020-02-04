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
  axis *phiAxis  = createAxis( "phi (deg)", 90,0,60 );


  TH2F *hLA[NLAYERS][NSECTORS];
  for( int j=0; j<NSECTORS;j++) 
    for( int i = 0; i < NLAYERS ; i++ ) hLA[i][j] = create2DHisto( Form("laL%dS%d",i+1,j+1),"", phiAxis, sizeAxis );
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
  CVTTraj traj( "CVTRec::Trajectory", factory );

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
    event.getStructure( traj );

    // skip if there are no BMT clsuters
    if( rcl.getSize() < 1 ) continue;

    // maps
    ///////
    // this maps the tid to a map of BMT related points in the trajectory (100*layer+sector) and itsp phi
    std::map<int,std::map<int,float>> trjPhiMap;
    for( int j = 0; j < traj.getSize(); j++ ){
      traj.get(j);
      if( traj.detector != 1 ) continue;

      //std::cout << " " 
                //<< traj.id << " " 
                //<< traj.layer << " " 
                //<< traj.detector << " " 
                //<< traj.sector << " " 
                //<< traj.phi << "\n"; 
      trjPhiMap[traj.id][ (100*traj.layer + traj.sector) ] = TMath::RadToDeg()*traj.phi; 
    }
    

    // loop over clusters
    for( int j = 0; j < rcl.getSize(); j++ ){
      rcl.get(j);

      if( rcl.tid <= 0 ){
        continue;
      }
      trk.get( rcl.tid );
      if( trk.q > 0 ) continue;

      hLA[rcl.layer-1][rcl.sector-1]->Fill( trjPhiMap[rcl.tid][ (100*(rcl.layer+6) +rcl.sector)], rcl.size   );
      //std::cout << trjPhiMap[rcl.tid][ (100*(rcl.layer+6) +rcl.sector)] << std::endl;
    }
  
    //if( iev > 15 ) break;
  }

  TFile f("outLA.root", "recreate");

  for( int j=0; j<NSECTORS;j++) 
    for( int i = 0; i < NLAYERS ; i++ ){
       hLA[i][j]->SetDirectory(&f);
    }

  f.Write();
  f.Close();

  return 0;
}



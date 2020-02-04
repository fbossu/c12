#pragma once
#include <string>

#include "dictionary.h"
#include "bank.h"


class Bank : public hipo::bank {
  public:
    Bank( std::string s, hipo::dictionary d ) :
      hipo::bank( d.getSchema( s.c_str() ) ) {} ;

    virtual void get( int ) = 0;

    int getSize() { return getRows(); }
    
};

class MCPart : public Bank {

  public:
    MCPart( std::string s, hipo::dictionary d) : Bank(s,d) {} ;

    
    void get( int i ){
      pid =  getInt( "pid", i );
      px =   getFloat( "px", i );
      py =   getFloat( "py", i );
      pz =   getFloat( "pz", i );
      vx = 10. * getFloat( "vx", i );
      vy = 10. * getFloat( "vy", i );
      vz = 10. * getFloat( "vz", i );
    } 

    int pid;
    float px;
    float py;
    float pz;
    float vx;
    float vy;
    float vz;


};

class RecParticle : public Bank {
  public:
    RecParticle( std::string s, hipo::dictionary d) : Bank(s,d) {} ;
  
  void get( int i ){
    pid   = getInt(        "pid", i);
    px    = getFloat(       "px", i);
    py    = getFloat(       "py", i);
    pz    = getFloat(       "pz", i);
    vx    = getFloat(       "vx", i);
    vy    = getFloat(       "vy", i);
    vz    = getFloat(       "vz", i);
    charge= getInt(     "charge", i);
    beta  = getFloat(     "beta", i);
    chi2pid=getFloat( "chi2pid", i);
  }
  int pid;
  float px;
  float py;
  float pz;
  float vx;
  float vy;
  float vz;
  int charge;
  float beta;
  float chi2pid;
};

class RecTrack : public Bank {
  public:
    RecTrack( std::string s, hipo::dictionary d) : Bank(s,d) {} ;
  
  void get( int i ){
     index    = getInt(   "index", i );
     pindex   = getInt(  "pindex", i );
     detector = getInt("detector", i );
     sector   = getInt(  "sector", i );
     status   = getInt(  "status", i );
     q        = getInt(       "q", i );
     chi2     = getFloat(  "chi2", i );
     NDF      = getInt(     "NDF", i );
  }

  int index    ;
  int pindex   ;
  int detector ;
  int sector   ;
  int status   ;
  int q        ;
  float chi2   ;
  int NDF      ;
};


class MCisMatch : public Bank {
  public:
    MCisMatch( std::string s, hipo::dictionary d) : Bank(s,d) {} ;

    void get( int i ) {
      mctid       = getInt("mcTindex"  , i );
      rectid      = getInt("recTindex"  , i );
      isInAcc     = getInt("isInAcc"  , i );
      nMCclusters = getInt("nMCclusters"  , i ); 
      fraction    = getFloat("fraction"  , i );
    }

    int mctid;
    int rectid;
    int isInAcc;
    int nMCclusters;
    float fraction;
};

class MCMatchedCluster : public Bank {
  public:
    MCMatchedCluster( std::string s, hipo::dictionary d) : Bank(s,d) {} ;

    void get( int i ) {
      mctid       = getInt("mcTindex"  , i );
      rectid      = getInt("recTindex"  , i );
      detector    = getInt("detector"  , i );
    }

    int mctid;
    int rectid;
    int detector;
};


class CVTTrack : public Bank {
public:
    CVTTrack( std::string s, hipo::dictionary d) : Bank(s,d) {} ;

  void get( int i ){
    reset(); 
    for( int j=1;j<=9;j++ ) {
      char buffer[50];
      sprintf(buffer,"Cross%d_ID",j );  
      int v = getInt( buffer, i );
      if( v>0 ){
        if( v < 1000 ) nSVT++;
        else nBMT++;
      }
    }
    d0     = 10. * getFloat( "d0"  , i );
    chi2   = getFloat( "chi2"  , i );
    p      = getFloat( "p" , i );
    pt     = getFloat( "pt" , i );
    z0     = 10.* getFloat( "z0" , i );
    tandip = getFloat( "tandip" , i );
    q      = getInt( "q" , i );         
    ndf    = getInt( "ndf" , i );

    phi    = 180.0/M_PI * getFloat( "phi0" , i );

    theta  = 180.0/M_PI *  (M_PI/2. - atan( tandip ) );

    path   = getFloat( "pathlength", i );

  }

    cov_d02 = getFloat("cov_d02",i);
    cov_d0phi0 = getFloat("cov_d0phi0",i);
    cov_d0rho = getFloat("cov_d0rho",i);
    cov_phi02 = getFloat("cov_phi02",i);
    cov_phi0rho = getFloat("cov_phi0rho",i);
    cov_rho2 = getFloat("cov_rho2",i);
    cov_z02 = getFloat("cov_z02",i);
    cov_tandip2 = getFloat("cov_tandip2",i);
  }

  void reset() { 
    nSVT = 0;
    nBMT = 0;
  }

  int nSVT;
  int nBMT;

  float phi;
  float theta;

  float d0;
  float chi2;
  int   ndf;
  float p;
  float pt;
  float z0;
  float tandip;
  int   q;

  float path;

  float cov_d02;
  float cov_d0phi0;
  float cov_d0rho;
  float cov_phi02;
  float cov_phi0rho;
  float cov_rho2;
  float cov_z02;
  float cov_tandip2;
  
};

class CVTTraj : public Bank {
public:
    CVTTraj( std::string s, hipo::dictionary d) : Bank(s,d) {} ;

  void get( int i ){
    id = getInt("id", i );
    detector = getInt("detector", i );
    sector = getInt("sector", i );
    layer  = getInt("layer", i );
    phi = getFloat("phi", i );
  }

  int id;
  int detector;
  int sector;
  int layer;
  float phi;
};

class Cross : public Bank {
  public:
    Cross( std::string s, hipo::dictionary d) : Bank(s,d) {} ;

    void get( int i ){
      id  = getInt("ID",i);
      tid = getInt("trkID",i);
      sec = getInt("sector",i);
      reg = getInt("region",i);
    }

    int id;
    int tid;
    int sec;
    int reg;
};




class Cls : public Bank {

  public:
    Cls( std::string s, hipo::dictionary d) : Bank(s,d) {} ;
    void get( int i ){
      id = getInt( "ID", i );
      sector = getInt( "sector", i );
      layer  = getInt( "layer", i );
      size   = getInt( "size", i );
      tid  = getInt( "trkID", i );
      centroid  = getFloat( "centroid", i );
      centroidResidual = getFloat( "centroidResidual", i );
      Tmin = getFloat( "Tmin", i );
      Tmax = getFloat( "Tmax", i );
    }
    int sector;
    int layer;
    int size;
    int tid;
    float centroid;
    float Tmin;
    float Tmax;
    float centroidResidual;
};

class RunConfig : public Bank {

  public:
    RunConfig( std::string s, hipo::dictionary d) : Bank(s,d) {} ;
    void get( int i ){
      event = getInt( "event", 0 );
      trigger = getLong( "event",0);
      run = getInt("run",0);
    }
  int event;
  int run;
  long trigger;
};


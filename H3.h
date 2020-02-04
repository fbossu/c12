#include "bank.h"
#include "clas12bank.h"


class Bank {
  public:
    Bank() {} ;
    virtual void init( std::string s, hipo::reader& r){
      _bank.init( s.c_str(), r );

      addBranches();
    }

    std::vector<std::string> bnames;
    clas12bank _bank;
    
    virtual int getSize() { return _bank.getSize(); }
  private:

    virtual void addBranches(){
      for( auto s : bnames ) _bank.addBranch( s.c_str() ); 
    }
};

class RecPart : public Bank {
  public:
    
    RecPart() {
      bnames.push_back( "pid");
      bnames.push_back( "px" );
      bnames.push_back( "py" );
      bnames.push_back( "pz" );
      bnames.push_back( "vx" );
      bnames.push_back( "vy" );
      bnames.push_back( "vz" );
      bnames.push_back( "charge" );
      bnames.push_back( "beta" );
      bnames.push_back( "status" );
    }                   
   
    void get( int i ){
      pid    = _bank.getInt( "pid", i );
      q      = _bank.getInt( "charge", i );
      status = _bank.getInt( "status", i );
      px = _bank.getFloat( "px", i );
      py = _bank.getFloat( "py", i );
      pz = _bank.getFloat( "pz", i );
      vx = _bank.getFloat( "vx", i );
      vy = _bank.getFloat( "vy", i );
      vz = _bank.getFloat( "vz", i );
      beta = _bank.getFloat( "beta", i );
    } 

    int pid;
    float px;
    float py;
    float pz;
    float vx;
    float vy;
    float vz;
    int   q;
    int   status;
    float beta;
};

class MCPart : public Bank {
  public:
    
    MCPart() {
      bnames.push_back( "pid");
      bnames.push_back( "px" );
      bnames.push_back( "py" );
      bnames.push_back( "pz" );
      bnames.push_back( "vx" );
      bnames.push_back( "vy" );
      bnames.push_back( "vz" );
    }                   
   
    void get( int i ){
      pid = _bank.getInt( "pid", i );
      px = _bank.getFloat( "px", i );
      py = _bank.getFloat( "py", i );
      pz = _bank.getFloat( "pz", i );
      vx = 10. * _bank.getFloat( "vx", i );
      vy = 10. * _bank.getFloat( "vy", i );
      vz = 10. * _bank.getFloat( "vz", i );
    } 

    int pid;
    float px;
    float py;
    float pz;
    float vx;
    float vy;
    float vz;
 
};

class RecTrack : public Bank {
  public:
    RecTrack() {
      bnames.push_back( "index" ); 
      bnames.push_back( "detector" ); 
    }
  
  void get( int i ){
     index    = _bank.getInt( "index", i );
     detector = _bank.getInt( "index", i );
  }

  int index;
  int detector;
};


class MCisMatch : public Bank {
  public:
    MCisMatch() {
      bnames.push_back( "mcTindex" );
      bnames.push_back( "recTindex" );
      bnames.push_back( "isInAcc" );
      bnames.push_back( "nMCclusters" );
      bnames.push_back( "fraction" );
    }

    int get( int i ) {
      mctid       = _bank.getInt("mcTindex"  , i );
      rectid      = _bank.getInt("recTindex"  , i );
      isInAcc     = _bank.getInt("isInAcc"  , i );
      nMCclusters = _bank.getInt("nMCclusters"  , i ); 
      fraction    = _bank.getFloat("fraction"  , i );
    }

    int mctid;
    int rectid;
    int isInAcc;
    int nMCclusters;
    float fraction;
};


class CVTTrack : public Bank {
public:
  CVTTrack() {
    bnames.push_back( "Cross1_ID" );
    bnames.push_back( "Cross2_ID" );
    bnames.push_back( "Cross3_ID" );
    bnames.push_back( "Cross4_ID" );
    bnames.push_back( "Cross5_ID" );
    bnames.push_back( "Cross6_ID" );
    bnames.push_back( "Cross7_ID" );
    bnames.push_back( "Cross8_ID" );
    bnames.push_back( "Cross9_ID" );

    bnames.push_back( "d0" );
    bnames.push_back( "chi2" );
    bnames.push_back( "ndf");
    bnames.push_back( "p");
    bnames.push_back( "pt");
    bnames.push_back( "z0");
    bnames.push_back( "phi0");
    bnames.push_back( "tandip");
    bnames.push_back( "q");    

  }

  void get( int i ){
    reset(); 
    for( int j=1;j<=9;j++ ) {
      char buffer[50];
      sprintf(buffer,"Cross%d_ID",j );  
      int v = _bank.getInt( buffer, i );
      if( v>0 ){
        if( v < 1000 ) nSVT++;
        else nBMT++;
      }
    }
    d0     = _bank.getFloat( "d0"  , i );
    chi2   = _bank.getFloat( "chi2"  , i );
    p      = _bank.getFloat( "p" , i );
    pt     = _bank.getFloat( "pt" , i );
    z0     = _bank.getFloat( "z0" , i );
    tandip = _bank.getFloat( "tandip" , i );
    q      = _bank.getInt( "q" , i );         
    ndf    = _bank.getInt( "ndf" , i );

    phi    = 180.0/M_PI * _bank.getFloat( "phi0" , i );

    theta  = 180.0/M_PI *  (M_PI/2. - atan( tandip ) );
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
};



class Cluster : public Bank {

  public:
    Cluster() {
      bnames.push_back( "sector");
      bnames.push_back( "layer");
      bnames.push_back( "size");
      bnames.push_back( "trkID");
      bnames.push_back( "centroid");
      bnames.push_back( "centroidResidual");
      bnames.push_back( "Tmin");
      bnames.push_back( "Tmax");
    }
    void get( int i ){
      sector = _bank.getInt( "sector", i );
      layer = _bank.getInt( "layer", i );
      size = _bank.getInt( "size", i );
      tid = _bank.getInt( "trkID", i );
      centroid = _bank.getFloat( "centroid", i );
      centroidResidual = _bank.getFloat( "centroidResidual", i );
      Tmin = _bank.getFloat( "Tmin", i );
      Tmax = _bank.getFloat( "Tmax", i );
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

class CVTCosmic : public Bank {

  public:
    CVTCosmic() {
      bnames.push_back( "NbBMTHits");
    }
  void get( int i ){
    nBMT = _bank.getInt( "NbBMTHits", i );
  }

  int nBMT;
};


import org.jlab.io.hipo.*;
import org.jlab.io.hipo.HipoDataSource;
import org.jlab.io.base.DataBank;
import org.jlab.io.base.DataEvent;

import org.jlab.io.hipo3.Hipo3DataSource;



// function to loop over 100 events of background
// ----------------------------------------------
N = 1;
DataEvent getBkgEvent( Hipo3DataSource r ){
  if( N > 100 ) N = 1;
  N++;
  return r.gotoEvent( N );
}




// begin
// -----


// create the output file
System.out.println("  ------- " );
HipoDataSync writer = new HipoDataSync();
writer.open( "test.hipo" );


// reader for the bkg events, in hipo3
Hipo3DataSource reader0 = new Hipo3DataSource();
reader0.open(args[0]);


// reader for the simulation  part, in hipo4
HipoDataSource reader1 = new HipoDataSource();
reader1.open(args[1]);



// loop over the simulation events
// -------------------------------
//int evnumber = 0;
while( reader1.hasEvent() ){
  //if( evnumber > 200 ) break;
  //evnumber++;

  // get simu event
  DataEvent ev1 = reader1.getNextEvent();

  // create the output event
  DataEvent merged = writer.createEvent();
 

  // append the useful banks
  if(ev1.hasBank("RUN::config")) merged.appendBank(ev1.getBank("RUN::config"));
  if(ev1.hasBank("MC::Header")) merged.appendBank(ev1.getBank("MC::Header"));
  if(ev1.hasBank("MC::Event")) merged.appendBank(ev1.getBank("MC::Event"));
  if(ev1.hasBank("MC::Particle")) merged.appendBank(ev1.getBank("MC::Particle"));
  if(ev1.hasBank("MC::Lund")) merged.appendBank(ev1.getBank("MC::Lund"));
  if(ev1.hasBank("MC::True")) merged.appendBank(ev1.getBank("MC::True"));
  if(ev1.hasBank("CTOF::tdc")) merged.appendBank(ev1.getBank("CTOF::tdc"));


  // get background event
  DataEvent ev0 =  getBkgEvent( reader0 );


  /////// BST
  /// ------------------------------------

  // get background bank
  DataBank bBSTbkg = ev0.getBank( "BST::adc");
  int Nbkg = 0;
  if( bBSTbkg != null ) Nbkg = bBSTbkg.rows();

  // get sim bank
  DataBank bBSTsim = ev1.getBank( "BST::adc");
  int Nsim = 0;
  if( bBSTsim != null ) Nsim = bBSTsim.rows();

  DataBank bBSTmerged = merged.createBank( "BST::adc", Nsim + Nbkg );
  for( int i = 0; i <  Nsim ; i++ ){
    bBSTmerged.setByte(  "sector"    , i , bBSTsim.getByte(  "sector"    , i ) );
    bBSTmerged.setByte(  "layer"     , i , bBSTsim.getByte(  "layer"     , i ) );
    bBSTmerged.setShort( "component" , i , bBSTsim.getShort( "component" , i ) );
    bBSTmerged.setByte(  "order"     , i , bBSTsim.getByte(  "order"     , i ) );
    bBSTmerged.setInt(   "ADC"       , i , bBSTsim.getInt(   "ADC"       , i ) );
    bBSTmerged.setFloat( "time"      , i , bBSTsim.getFloat( "time"      , i ) );
    bBSTmerged.setShort( "ped"       , i , bBSTsim.getShort( "ped"       , i ) );
    //bBSTmerged.setInt(   "integral"  , i , bBSTsim.getInt(   "integral"  , i  ) );
    bBSTmerged.setLong(  "timestamp" , i , bBSTsim.getLong(  "timestamp" , i  ) );
  }

  for( int i = Nsim; i <  Nsim + Nbkg; i++ ){
    bBSTmerged.setByte(  "sector"    , i , bBSTbkg.getByte(  "sector"    , i - Nsim ) );
    bBSTmerged.setByte(  "layer"     , i , bBSTbkg.getByte(  "layer"     , i - Nsim ) );
    bBSTmerged.setShort( "component" , i , bBSTbkg.getShort( "component" , i - Nsim ) );
    bBSTmerged.setByte(  "order"     , i , bBSTbkg.getByte(  "order"     , i - Nsim ) );
    bBSTmerged.setInt(   "ADC"       , i , bBSTbkg.getInt(   "ADC"       , i - Nsim ) );
    bBSTmerged.setFloat( "time"      , i , bBSTbkg.getFloat( "time"      , i - Nsim ) );
    bBSTmerged.setShort( "ped"       , i , bBSTbkg.getShort( "ped"       , i - Nsim ) );
    //bBSTmerged.setInt(   "integral"  , i , bBSTbkg.getInt(   "integral"  , i - Nsim ) );
    bBSTmerged.setLong(  "timestamp" , i , bBSTbkg.getLong(  "timestamp" , i - Nsim ) );
  }

  // append bank
  merged.appendBank( bBSTmerged );


  /////// BMT
  /// ------------------------------------

  // get background bank
  DataBank bBMTbkg = ev0.getBank( "BMT::adc");
  Nbkg = 0;
  if( bBMTbkg != null ) Nbkg = bBMTbkg.rows();

  // get sim bank
  DataBank bBMTsim = ev1.getBank( "BMT::adc");
  Nsim = 0;
  if( bBMTsim != null ) Nsim = bBMTsim.rows();

  DataBank bBMTmerged = merged.createBank( "BMT::adc", Nsim + Nbkg );
  for( int i = 0; i <  Nsim ; i++ ){
    bBMTmerged.setByte(  "sector"    , i , bBMTsim.getByte(  "sector"    , i ) );
    bBMTmerged.setByte(  "layer"     , i , bBMTsim.getByte(  "layer"     , i ) );
    bBMTmerged.setShort( "component" , i , bBMTsim.getShort( "component" , i ) );
    bBMTmerged.setByte(  "order"     , i , bBMTsim.getByte(  "order"     , i ) );
    bBMTmerged.setInt(   "ADC"       , i , bBMTsim.getInt(   "ADC"       , i ) );
    bBMTmerged.setFloat( "time"      , i , bBMTsim.getFloat( "time"      , i ) );
    bBMTmerged.setShort( "ped"       , i , bBMTsim.getShort( "ped"       , i ) );
    bBMTmerged.setInt(   "integral"  , i , bBMTsim.getInt(   "integral"  , i  ) );
    bBMTmerged.setLong(  "timestamp" , i , bBMTsim.getLong(  "timestamp" , i  ) );
  }

  for( int i = Nsim; i <  Nsim + Nbkg; i++ ){
    bBMTmerged.setByte(  "sector"    , i , bBMTbkg.getByte(  "sector"    , i - Nsim ) );
    bBMTmerged.setByte(  "layer"     , i , bBMTbkg.getByte(  "layer"     , i - Nsim ) );
    bBMTmerged.setShort( "component" , i , bBMTbkg.getShort( "component" , i - Nsim ) );
    bBMTmerged.setByte(  "order"     , i , bBMTbkg.getByte(  "order"     , i - Nsim ) );
    bBMTmerged.setInt(   "ADC"       , i , bBMTbkg.getInt(   "ADC"       , i - Nsim ) );
    bBMTmerged.setFloat( "time"      , i , bBMTbkg.getFloat( "time"      , i - Nsim ) );
    bBMTmerged.setShort( "ped"       , i , bBMTbkg.getShort( "ped"       , i - Nsim ) );
    bBMTmerged.setInt(   "integral"  , i , bBMTbkg.getInt(   "integral"  , i - Nsim ) );
    bBMTmerged.setLong(  "timestamp" , i , bBMTbkg.getLong(  "timestamp" , i - Nsim ) );
  }

  merged.appendBank( bBMTmerged );


  // write output event
  // ------------------
  writer.writeEvent( merged );
}




reader0.close();
reader1.close();
writer.close();

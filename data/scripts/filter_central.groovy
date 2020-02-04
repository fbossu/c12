import org.jlab.io.hipo.*;
import org.jlab.io.hipo.HipoDataSource;
import org.jlab.io.base.DataBank;
import org.jlab.io.base.DataEvent;

HipoDataSync writer = new HipoDataSync();
writer.open(args[0]);

HipoDataSource reader = new HipoDataSource();
reader.open(args[1]);


String[] banks = [
"BMT::adc",
"BST::adc",
  //"BMTRec::Clusters",
  //"BMTRec::Crosses",
  //"BSTRec::Clusters",
  //"BSTRec::Crosses",
  //"BSTRec::Hits",
  //"BMTRec::Hits",
"RUN::config",
"RUN::trigger",
"CND::adc",
"CND::tdc",
"CTOF::adc",
"CTOF::tdc"
  //"CVTRec::Tracks",
  //"CVTRec::Trajectory"
]


if(reader)while(reader.hasEvent())
{
	DataEvent event = reader.getNextEvent();

  if( event == null ) continue;

  if( event.hasBank( "RUN::config") == false ) continue;

  trg = event.getBank( "RUN::config").getLong( "trigger",0 )

  if( trg % 2 == 0 ) continue;

  DataEvent skimmedEvent = writer.createEvent();

  for( String b : banks ){
    if(event.hasBank( b )) skimmedEvent.appendBank(event.getBank( b ));
  }
  writer.writeEvent(skimmedEvent);
}

reader.close();
writer.close();

import org.jlab.io.hipo.*;
import org.jlab.io.hipo.HipoDataSource;
import org.jlab.io.base.DataBank;
import org.jlab.io.base.DataEvent;

HipoDataSync writer = new HipoDataSync();
writer.open(args[0]);

HipoDataSource reader = new HipoDataSource();
reader.open(args[1]);


//int[] evList = [ 5, 30, 55, 131, 146, 161, 179, 194, 200, 211, 228, 230, 254, 260, 267, 272, 307, 308, 310, 313, 320, 327, 360, 367, 385, 464, 481, 510, 520, 576, 594, 674, 696, 716, 718, 739, 746, 749, 754, 764, 765, 781, 846, 861, 874, 888, 946, 947]
int[] evList = [ 57,59,64,356,642,692,882,968,1161,1168,1365,1434,1540,1638,1830,1893,2241,2262,2264,2297,2317,2382,2399,2514,2560,2572,2576,2602,2889,2924,2951,3067,3305,3432,3872,3876,4059,4463,4610,4682,4786 ]

if(reader)while(reader.hasEvent())
{
	DataEvent event = reader.getNextEvent();

  if( event == null ) continue;
  if(event.hasBank("RUN::config")) { 
    int evnum = event.getBank("RUN::config").getInt("event",0);
    if( evList.contains( evnum ) == false ) continue;
  }
  else continue;



  DataEvent skimmedEvent = writer.createEvent();
  if(event.hasBank("RUN::config")) skimmedEvent.appendBank(event.getBank("RUN::config"));
  if(event.hasBank("BMT::adc")) skimmedEvent.appendBank(event.getBank("BMT::adc"));
  if(event.hasBank("BST::adc")) skimmedEvent.appendBank(event.getBank("BST::adc"));
  if(event.hasBank("CTOF::tdc")) skimmedEvent.appendBank(event.getBank("CTOF::tdc"));
  if(event.hasBank("MC::Header")) skimmedEvent.appendBank(event.getBank("MC::Header"));
  if(event.hasBank("MC::Event")) skimmedEvent.appendBank(event.getBank("MC::Event"));
  if(event.hasBank("MC::Particle")) skimmedEvent.appendBank(event.getBank("MC::Particle"));
  if(event.hasBank("MC::Lund")) skimmedEvent.appendBank(event.getBank("MC::Lund"));
  if(event.hasBank("MC::True")) skimmedEvent.appendBank(event.getBank("MC::True"));
  if(event.hasBank("CVTRec::Tracks")) skimmedEvent.appendBank(event.getBank("CVTRec::Tracks"));
  if(event.hasBank("BMTRec::Clusters")) skimmedEvent.appendBank(event.getBank("BMTRec::Clusters"));
  if(event.hasBank("BSTRec::Clusters")) skimmedEvent.appendBank(event.getBank("BSTRec::Clusters"));
  if(event.hasBank("CVTRec::Trajectory")) skimmedEvent.appendBank(event.getBank("CVTRec::Trajectory"));
  writer.writeEvent(skimmedEvent);
}

reader.close();
writer.close();



#include "LendaEvent.hh"
#include <iostream>


using namespace std;


#define BAD_NUM -1008
LendaEvent::LendaEvent()
{
  sdt1=0;
  sdt2=0;
  Clear();
  

}

void LendaEvent::Clear(){

  TOF=BAD_NUM;
  ShiftDt=BAD_NUM;
  ShiftTOF=BAD_NUM;
  Dt=BAD_NUM;
  PulseShape=BAD_NUM;
  times.clear();
  energies.clear();
  energiesCor.clear();
  channels.clear();
  traces.clear();
  
  shortGates.clear();
  longGates.clear();

  shiftCorrectedTimes.clear();
  liqCorrectedTimes.clear();
  

}

void LendaEvent::pushTime(Double_t t){
  times.push_back(t);

}
void LendaEvent::pushEnergy(Double_t e){
  energies.push_back(e);

}
void LendaEvent::pushChannel(Double_t c){
  channels.push_back(c);
}

void LendaEvent::pushTrace(vector <UShort_t> in){
  traces.push_back(in);
}



void LendaEvent::pushLiqCorrections(Double_t c1,Double_t c2){
  //Liq1 = channel 8
  //Time_Diff  timeDiff = 0.5*(events[0]->time + events[1]->time) - events[2]->time;
 
  if (shiftCorrectedTimes.size() == 0 ){
    cout<<"***Warning preform shift correction first***"<<endl;
  }
  liqCorrectedTimes.push_back(shiftCorrectedTimes[0]);//0 is unchanged
  liqCorrectedTimes.push_back(shiftCorrectedTimes[1]);//1 is unchanged
  
  if (channels[0]==0 && channels[1]==1){
    liqCorrectedTimes.push_back(shiftCorrectedTimes[2]+c1);
  } else if (channels[0] ==2 && channels[1]==3){
    liqCorrectedTimes.push_back(shiftCorrectedTimes[2]+c2);
  }
  
}



void LendaEvent::pushLongGate(Double_t lg){
  longGates.push_back(lg);
}
void LendaEvent::pushShortGate(Double_t sg){
  shortGates.push_back(sg);
}

void LendaEvent::shiftCor(){

  //correction determined from time[0]-time[1]
  //shift applied to time[0]
  if (channels[0]==0 && channels[1] ==1 ) {//one bar
    shiftCorrectedTimes.push_back(times[0]-sdt1);
    //      cout<<"channels "<<channels[0]<< " "<<channels[1]<<endl;
    //      cout<<"times[0] "<<times[0]<<" -sdt1 "<<times[0]-sdt1<<endl;
  } else if (channels[0]==2 && channels[1]==3) { //the other one
    shiftCorrectedTimes.push_back(times[0]-sdt2);
    //      cout<<"channels "<<channels[0]<< " "<<channels[1]<<endl;
    //      cout<<"times[0] "<<times[0]<<" -sdt2 "<<times[0]-sdt2<<endl;
  }

  shiftCorrectedTimes.push_back(times[1]); //time[1] is unchanged
  shiftCorrectedTimes.push_back(times[2]);//time[2] is unchanged

}

void LendaEvent::gainCor(){

  energiesCor.resize(energies.size());
  for (int i=0;i<(int)energies.size();i++){
    if (channels[i] == 0)
      energiesCor[i]= energies[i]*gainCorrections[0];
    else if (channels[i] == 1)
      energiesCor[i]= energies[i]*gainCorrections[1];
    else if (channels[i] == 2)
      energiesCor[i]= energies[i]*gainCorrections[2];
    else if (channels[i] == 3)
      energiesCor[i]= energies[i]*gainCorrections[3];
  }


}


void LendaEvent::Finalize(){

  shiftCor();
  
  if (gainCorrections.size()!=0)//only apply gain correctins if 
    gainCor();                   //they have be provided

  TOF = 0.5*(times[0]+times[1])-times[2];
  ShiftDt=(shiftCorrectedTimes[0]-shiftCorrectedTimes[1]);
  ShiftTOF=0.5*(shiftCorrectedTimes[0]+shiftCorrectedTimes[1]) -shiftCorrectedTimes[2];

  PulseShape = longGates[2]/shortGates[2];

  Dt = times[0]-times[1];

  
  
}




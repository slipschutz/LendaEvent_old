

#include "LendaEvent.hh"
#include <iostream>


using namespace std;


#define BAD_NUM -1008
LendaEvent::LendaEvent()
{

 Time_Diff=BAD_NUM;

}

void LendaEvent::Clear(){

  Time_Diff=BAD_NUM;
  times.clear();
  energies.clear();
  channels.clear();
  traces.clear();

  shortGates.clear();
  longGates.clear();

  shiftCorrectedTimes.clear();


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

void LendaEvent:: pushShiftCorrections(Double_t sdt1,Double_t sdt2){
  
  if ( times.size() == 0 || channels.size()==0){
    cout<<"***Warning***"<<endl;
    cout<<"LendaEvent has no times in it\n push times and channels before pushing time corrections"<<endl;
  } else {
    //correction determined from time[0]-time[1]
    //shift applied to time[0]
    if (channels[0]==0 && channels[1] ==1 ) //one bar
      shiftCorrectedTimes.push_back(times[0]-sdt1);
    else if (channels[0]==2 && channels[1]==3) //the other one
      shiftCorrectedTimes.push_back(times[0]-sdt2);
    
    shiftCorrectedTimes.push_back(times[1]); //time[1] is unchanged
    shiftCorrectedTimes.push_back(times[2]);//time[2] is unchanged
  }
}

void LendaEvent::pushLongGate(Double_t lg){
  longGates.push_back(lg);
}
void LendaEvent::pushShortGate(Double_t sg){
  shortGates.push_back(sg);
}


void LendaEvent::Finalize(){
  for (int i=0;i<3;i++){
    times2[i]=times[i];
    channels2[i]=channels[i];
    energies2[i]=energies[i];
  }

}




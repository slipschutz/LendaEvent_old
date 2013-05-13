

#include "LendaEvent.hh"
#include <iostream>
#include "TMath.h"
#include <sstream>

using namespace std;


#define BAD_NUM -1008
LendaEvent::LendaEvent()
{
 
  fPosForm="chan";
  sdt1=0;
  sdt2=0;
  fgainCorrections.clear();
  fwalkCorrections.clear();
  fnumOfWalkCorrections=0;
  fnumOfGainCorrections=0;
  fnumOfPositionCorrections=0;
  CTrace=0;
  Clear();

 

}

void LendaEvent::setWalkCorrections(vector <Double_t> in){
  fwalkCorrections.clear();
  fwalkCorrections.push_back(in);
  fnumOfWalkCorrections=0;
}

void LendaEvent::setWalkCorrections(vector <Double_t> in,Int_t channel){

  if (channel >= (Int_t)fwalkCorrections.size() ){
    int diff = channel-fwalkCorrections.size();
    fwalkCorrections.resize( fwalkCorrections.size()+diff +1);
  }
  
  fwalkCorrections[channel]=in;


  fnumOfWalkCorrections++;
}

void LendaEvent::setGainCorrections(vector <Double_t> in ){
  for (int i=0;i<(int)in.size();i++)
    setGainCorrections(in[i],i);
}

void LendaEvent::setGainCorrections(Double_t in,Int_t channel){

  if (channel >= (Int_t)fgainCorrections.size()){
    int diff = channel - fgainCorrections.size();
    fgainCorrections.resize(fgainCorrections.size()+diff+1,1.0);
  }
  fgainCorrections[channel]=in;


  fnumOfGainCorrections++;
}


void LendaEvent::Clear(){
  ////REMEBER TO CLEAR THINGS THAT were thing.push_Back!!!!
  TOF=BAD_NUM;
  Dt=BAD_NUM;

  ShiftDt=BAD_NUM;
  ShiftTOF=BAD_NUM;

  GOE=BAD_NUM;
  CorGOE=BAD_NUM;
  PulseShape=BAD_NUM;

  TOFW.clear();
  TOFP.clear();

  times.clear();
  energies.clear();
  energiesCor.clear();
  channels.clear();
  traces.clear();
  
  shortGates.clear();
  longGates.clear();


  shiftCorrectedTimes.clear();



  NumOfChannelsInEvent=0;

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




void LendaEvent::pushLongGate(Double_t lg){
  longGates.push_back(lg);
}
void LendaEvent::pushShortGate(Double_t sg){
  shortGates.push_back(sg);
}

void LendaEvent::shiftCor(){

  // correction to move the time difference spectrum seen in a lenda bar
  // to be around 0
  //correction determined from time[0]-time[1]
  //shift applied to time[0]
  if (channels[0]==0 && channels[1] ==1 ) {//one bar
    shiftCorrectedTimes.push_back(times[0]-sdt1);


  } else if (channels[0]==2 && channels[1]==3) { //the other one
    shiftCorrectedTimes.push_back(times[0]-sdt2);


  }

  shiftCorrectedTimes.push_back(times[1]); //time[1] is unchanged
  shiftCorrectedTimes.push_back(times[2]);//time[2] is unchanged

}

void LendaEvent::gainCor(){

  //Applying gain correction to each of the channels for Lenda bars
  
  for (int i=0;i<(int)energies.size();i++){
    energiesCor[i]=energies[i]*fgainCorrections[channels[i]];

  }
  /*

    if (channels[i] == 0)
      energiesCor[i]= energies[i]*fgainCorrections[0];
    else if (channels[i] == 1)
      enez rgiesCor[i]= energies[i]*fgainCorrections[1];
    else if (channels[i] == 2)
      energiesCor[i]= energies[i]*fgainCorrections[2];
    else if (channels[i] == 3)
      energiesCor[i]= energies[i]*fgainCorrections[3];
    else if (channels[i]==8)
      energiesCor[i]=energies[i]*fgainCorrections[4];//For the liqscints
      }*/
}

void LendaEvent::walkCor(){
  Double_t total[fnumOfWalkCorrections];

  for (int i=0;i<fnumOfWalkCorrections;i++)
    total[i]=0;

  
  for (int j=0;j<fnumOfWalkCorrections;j++){
    if (channels[j]<fwalkCorrections.size()){
      for (int i=0;i<(int)fwalkCorrections[channels[j]].size();i++){
	
	//	if (energiesCor[j]<600)
	  total[j]=total[j]+fwalkCorrections[channels[j]][i]*TMath::Power(energiesCor[j],i+1);

      }
    }
  }


  Double_t runningTotal=0;
  for (int j=0;j<fnumOfWalkCorrections;j++){
     runningTotal = runningTotal +total[j];
     //     cout<<"This is runningTotal "<<runningTotal<<" this is J "<<j<<endl;
     // int t ;cin>>t;
     TOFW[j]=fTimeAfterPosCor-runningTotal;
  }
    
}


void LendaEvent::Finalize(){

  energiesCor.resize(energies.size());
  TOFW.resize(fnumOfWalkCorrections);
  TOFP.resize(fnumOfPositionCorrections);
  shiftCor();//make the shiftCorrectedTimes
 
  if (fgainCorrections.size()!=0)//only apply gain correctins if 
    gainCor();                   //they have be provided



  TOF = 0.5*(times[0]+times[1])-times[2];
  ShiftDt=(shiftCorrectedTimes[0]-shiftCorrectedTimes[1]);
  ShiftTOF=0.5*(shiftCorrectedTimes[0]+shiftCorrectedTimes[1]) -shiftCorrectedTimes[2];


  NumOfChannelsInEvent = times.size();//the number of channels pushed to event
  PulseShape = longGates[2]/shortGates[2];

  Dt = times[0]-times[1];

  GOE = (energies[0]-energies[1])/(energies[0]+energies[1]);
  CorGOE = (energiesCor[0]-energiesCor[1])/(energiesCor[0]+energiesCor[1]);
  posCor();  

  if (fwalkCorrections.size()!=0)
    walkCor();

}

void LendaEvent::setPositionCorrections(vector <Double_t> coef,Int_t channel ){

  stringstream key;
  key<<fPosForm<<"_"<<channel;

  if (fPositionCorrections.find(key.str()) == fPositionCorrections.end()){
    fPositionCorrections[key.str()]=coef;
    fnumOfPositionCorrections++;
  }else {
    cout<<"***Warning correction with key "<<key.str()<<" has already exists***"<<endl;
    cout<<"***Choose different key***"<<endl;
  }
}

void LendaEvent::posCor(){
  //Apply the position correctioins

  

  Double_t total[fnumOfPositionCorrections];

  for (int i=0;i<fnumOfPositionCorrections;i++)
    total[i]=0;

  stringstream key;
  vector <Double_t> theCoef;
  for (int j=0;j<fnumOfPositionCorrections;j++){
    key.str("");
    key<<fPosForm<<"_"<<channels[j];
    if (fPositionCorrections.find(key.str()) != fPositionCorrections.end()){
      theCoef =fPositionCorrections[key.str()];
      for (int i=0;i<theCoef.size();++i){
	total[j]=total[j]+ theCoef[i]*TMath::Power(CorGOE,i+1);


      }
    }
  }


  Double_t runningTotal=0;
  for (int j=0;j<fnumOfPositionCorrections;j++){
    runningTotal=runningTotal+total[j];
    TOFP[j]=ShiftTOF-runningTotal;    

  }


  if (TOFP.size()!=0)
    fTimeAfterPosCor=TOFP[TOFP.size()-1];
  else
    fTimeAfterPosCor=ShiftTOF;

  /*
    if (channels[j]<fwalkCorrections.size()){
      for (int i=0;i<(int)fwalkCorrections[channels[j]].size();i++){

        //      if (energiesCor[j]<600)
	total[j]=total[j]+fwalkCorrections[channels[j]][i]*TMath::Power(energiesCor[j],i+1);

      }
    }
  }

  Double_t runningTotal=0;
  for (int j=0;j<fnumOfWalkCorrections;j++){
    runningTotal = runningTotal +total[j];
    //     cout<<"This is runningTotal "<<runningTotal<<" this is J "<<j<<endl;
    // int t ;cin>>t;
    TOFW[j]=ShiftTOF-runningTotal;
  }


  */


}

/*
void LendaEvent::checkKey(string key){


 istringstream iss( key );
 vector<string> parts;
 string result;
  while (std::getline( iss, result , '_') )
    {
      parts.push_back(result);
    }   

  if (parts.size() != 2){
    cout<<"***Warning incorect position Correction key***"<<endl;
    cout<<"***Must be something_degreeOfCoef***"<<endl;
  }

  if (parts[0]!=fPosForm && fnumOfPositionCorrections==0){//if the given key is not like the expexted form
    fnumOfPositionCorrections++;                          //but it is the first key make that the form
    fPosForm=parts[0];
  }else if (parts[0]!=fPosForm && fnumOfPositionCorrections!=0){
    cout<<"***Warning given key does no match the expected form for keys***"<<endl;
    cout<<"***Expected form is "<<fPosForm<<"_degreeOfCoef***"<<endl;
    cout<<"***You gave "<<key<<"***"<<endl;
  } else {
    //nothing wrong
    fnumOfPositionCorrections++;
  }


}
*/

void LendaEvent::DumpWalkCorrections(){
  cout<<"\n***Dump walk corrections***"<<endl;

  for (int j=0;j<(int)fwalkCorrections.size();++j){
    int max_i = fwalkCorrections[j].size();
    cout<<"walkCorrection for channel "<<j<<endl;
    for (int i=0;i<max_i;++i){
      cout<<"   c"<<i+1<<" "<<fwalkCorrections[j][i]<<endl;
      //i+1 because the coefficents don't include the constant term
    }
  }
  

}

void LendaEvent::DumpGainCorrections(){
  cout<<"\n***Dump gain Corrections***"<<endl;
  for (int i=0;i<(int)fgainCorrections.size();++i){
    cout<<"gain correction for channel "<<i<<" "<<fgainCorrections[i]<<endl;
  }
}

void LendaEvent::DumpAllCorrections(){
  DumpGainCorrections();
  DumpWalkCorrections();
  DumpPositionCorrections();
}


void LendaEvent:: DumpPositionCorrections(){
  
for( map<string,vector<double> >::iterator ii=fPositionCorrections.begin(); ii!=fPositionCorrections.end(); ++ii)
    {
      for (int i=0;i<(int)((*ii).second).size();++i)
	cout << (*ii).first<<" "<<i << ": " << (*ii).second[i] << endl;
    }


}

void LendaEvent::Fatal(){
  cout<<"This Method should not exist.  Don't call it"<<endl;
}

void LendaEvent::MakeC(int spot){

  //  CTrace = calloc(sizeof(UShort_t)*traces[0].size());

  if (traces.size() == 0 ){
    cout<<"There are no traces to copy"<<endl;
    return;
  }

  if (CTrace != 0 )
    delete CTrace;

  CTrace = new UShort_t[traces[spot].size()];

  for (int i=0;i<traces[spot].size();++i){
    CTrace[i]=traces[spot][i];
  }

}

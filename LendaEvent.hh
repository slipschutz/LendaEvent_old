#ifndef __LENDA_EVENT_HH
#define __LENDA_EVENT_HH

#include "TObject.h"
#include "TFile.h"
#include <vector>
#include <map>




using namespace std;
class LendaEvent : public TObject {
public:
  
  LendaEvent();
  void Clear(); //Clear the Event.  Should be called after every write to a tree

  void Finalize(); //Applies intenral corrections and calculates convient Branches


  //Convenient quantities
  Double_t TOF;
  
  Double_t ShiftDt;
  Double_t ShiftTOF;
  Double_t Dt;
  Double_t PulseShape; //long gate over short gate
  Double_t GOE;
  Double_t CorGOE;
  vector <Double_t> TOFW; // the Walk corrections to TOFW
  vector <Double_t> TOFP;

  //Main information holders
  vector <Double_t> energiesCor;
  vector <Double_t> times;
  vector <Double_t> energies;
  vector <Double_t> channels;
  vector < vector <UShort_t> > traces;


  vector <Double_t> shiftCorrectedTimes;
  vector <Double_t> liqCorrectedTimes;


  vector <Double_t> shortGates;
  vector <Double_t> longGates;


  //pushing methods
  void pushLongGate(Double_t);
  void pushShortGate(Double_t);

  void pushEnergy(Double_t);
  void pushTime(Double_t);
  void pushChannel(Double_t);
  void pushTrace (vector <UShort_t>);


  //Corrections setting methods
  inline void setShiftCorrections(Double_t c1,Double_t c2){sdt1=c1;sdt2=c2;}
  void setGainCorrections(Double_t in,Int_t channel);//sets the gain correction for a channel
  void setGainCorrections(vector <Double_t> );//wrapper to the above assumes the vector has channel 0 at 
                                              //index 0 and channel 1 at index 1 ...


  void setWalkCorrections(vector <Double_t> in); //This sets input walkcorrection as the first 
                                                 //correction in the set walkCorrections[0][i] i goes 
                                                 //from 1 to degree of correction

  void setWalkCorrections(vector <Double_t> in,Int_t channel); // sets the correction and puts it in the 
                                                               //walkCorrections[channel][i]

  void setPositionCorrections(vector <Double_t>,Int_t channel );


  void dumpWalkCorrections();
  void dumpGainCorrections();
  void dumpAllCorrections();
  void dumpPositionCorrections(); 

  void pushLiqCorrections(Double_t,Double_t);

  void Fatal();


private:
  void shiftCor();
  void gainCor();
  void walkCor();  
  void posCor();

  //Corecction 
  Double_t sdt1,sdt2;

  vector <Double_t> fgainCorrections; //the gain corrections 
  vector < vector <Double_t> > fwalkCorrections; // set of walkCorrections

  map <string,vector<Double_t> > fPositionCorrections;//map for the position corrections 

  int fnumOfWalkCorrections;
  int fnumOfGainCorrections;
  int fnumOfPositionCorrections;

  string fPosForm;//defualt form of the string stored in the posCor map
  
  Double_t *currentCorrectionReference;
  

public:
  ClassDef(LendaEvent, 7);
};

#endif




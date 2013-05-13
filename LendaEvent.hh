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


  //Convenient quantities calculated in Finalize
  Double_t TOF; // Time of flight
  Double_t Dt; //Diference between Top and botom PMTs
  Double_t ShiftTOF; //TOF after shift correction
  Double_t ShiftDt;  //Dt after shift correction

  Double_t PulseShape; //long gate over short gate 
  Double_t GOE;   //Center of Gravity (E1-E2)/(E1+E2) 
  Double_t CorGOE;//Center of Gravity after gain matching
  vector <Double_t> TOFW; // the Walk corrections to TOF
  vector <Double_t> TOFP; //The position Corrections to TOF

  //Main information holders
  vector <Double_t> energiesCor; //the corrected energies 
  vector <Double_t> times; //the times
  vector <Double_t> energies; // the raw energies
  vector <Double_t> channels; // the channels
  vector < vector <UShort_t> > traces; // a vector of vectors holding the traces if present


  vector <Double_t> shiftCorrectedTimes; //The shift corrected times
  vector <Double_t> shortGates;  //short gate energies
  vector <Double_t> longGates;  //long gate energies



  //C Traces thing
  //root secissions like c arrays more then vectors of vectors
  void MakeC(int spot=0);  //copy trace data for the spot-th trace into C-array
  UShort_t *CTrace;  //the c-array to store the trace in



  //pushing methods expects the series of times and energies to be ordered by channel
  void pushLongGate(Double_t);
  void pushShortGate(Double_t);
  void pushEnergy(Double_t);
  void pushTime(Double_t);
  void pushChannel(Double_t);
  void pushTrace (vector <UShort_t>);


  //Corrections setting methods
  inline void setShiftCorrections(Double_t c1,Double_t c2){sdt1=c1;sdt2=c2;}//set shift Cor
  void setGainCorrections(Double_t in,Int_t channel);//sets the gain correction for a channel
  void setGainCorrections(vector <Double_t> );//wrapper to the above assumes the vector has channel 0 at 
                                              //index 0 and channel 1 at index 1 ...


  void setWalkCorrections(vector <Double_t> in); //This sets input walkcorrection as the first 
                                                 //correction in the set walkCorrections[0][i] i goes 
                                                 //from 1 to degree of correction

  void setWalkCorrections(vector <Double_t> in,Int_t channel); // sets the correction and puts it in the 
                                                               //walkCorrections[channel][i]


  void setPositionCorrections(vector <Double_t>,Int_t channel );//sets the position correction a channel



  //Printing Methods
  void DumpWalkCorrections();
  void DumpGainCorrections();
  void DumpAllCorrections();
  void DumpPositionCorrections(); 



  void Fatal();//overload this stupid inherited method


private:
  void shiftCor(); //method to apply the shift corections
  void gainCor(); //method to apply the gain corrections
  void walkCor(); //method to apply the walk corrections
  void posCor(); //method to apply the position corrections

  //Corecction 
  Double_t sdt1,sdt2;

  vector <Double_t> fgainCorrections; //the gain corrections 
  vector < vector <Double_t> > fwalkCorrections; // set of walkCorrections

  map <string,vector<Double_t> > fPositionCorrections;//map for the position corrections 

  int fnumOfWalkCorrections;
  int fnumOfGainCorrections;
  int fnumOfPositionCorrections;

  string fPosForm;
  
  double fTimeAfterPosCor;

public:
  ClassDef(LendaEvent, 9);
};

#endif




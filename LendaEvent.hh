#ifndef __LENDA_EVENT_HH
#define __LENDA_EVENT_HH

#include "TObject.h"
#include "TFile.h"
#include <vector>





using namespace std;
class LendaEvent : public TObject {
public:
  
  LendaEvent();
  void Clear();
  void Finalize();
  //Convenient quantities
  Double_t TOF;
  Double_t ShiftDt;
  Double_t ShiftTOF;
  Double_t Dt;
  Double_t PulseShape; //long gate over short gate

  //Corecction 
  Double_t sdt1,sdt2;
  vector <Double_t> gainCorrections;
  

  void MakeC();

  UShort_t *CTrace;

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

  //pushign methods
  void pushLongGate(Double_t);
  void pushShortGate(Double_t);

  void pushEnergy(Double_t);
  void pushTime(Double_t);
  void pushChannel(Double_t);
  void pushTrace (vector <UShort_t>);

  inline void setShiftCorrections(Double_t c1,Double_t c2){sdt1=c1;sdt2=c2;}
  inline void setGainCorrections(vector <Double_t> in){gainCorrections=in;}
  void pushLiqCorrections(Double_t,Double_t);
  
private:
  void shiftCor();
  void gainCor();
  

public:
  ClassDef(LendaEvent, 4);
};

#endif




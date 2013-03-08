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

  Double_t Time_Diff;
  Double_t ShiftT_Diff;

  vector <Double_t> times;
  vector <Double_t> energies;
  vector <Double_t> channels;

  vector < vector <UShort_t> > traces;
  

  vector <Double_t> shiftCorrectedTimes;
  vector <Double_t> liqCorrectedTimes;


  

  vector <Double_t> shortGates;
  vector <Double_t> longGates;




  void pushLongGate(Double_t);
  void pushShortGate(Double_t);

  void pushEnergy(Double_t);
  void pushTime(Double_t);
  void pushChannel(Double_t);
  void pushTrace (vector <UShort_t>);

  void pushShiftCorrections(Double_t,Double_t);

  void pushLiqCorrections(Double_t,Double_t);



public:
  ClassDef(LendaEvent, 2);
};

#endif




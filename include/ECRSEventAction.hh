// 6/19/2014 Hexc, Olesya: We defined the atm layer structure
// 2/10/2015 Hexc, Olesya: added current event number
// 6/9/2015  Hexc, Olesya: implement event timer
#ifndef ECRSEventAction_h
#define ECRSEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4Timer.hh"

class ECRSEventActionMessenger;

class ECRSEventAction : public G4UserEventAction
{
public:
  ECRSEventAction();
  virtual ~ECRSEventAction();
  
  virtual void  BeginOfEventAction(const G4Event*);
  virtual void  EndOfEventAction(const G4Event*);
  G4int getCurrentEventID() { return evtNb; }
  
  void SetDrawFlag (G4String val)  {drawFlag = val;};
  void SetPrintModulo (G4int val)  {printModulo = val;};
  
private:
  G4int			atmosphereCollID;
  G4String			drawFlag;
  ECRSEventActionMessenger*   eventMessenger;
  G4int			printModulo;
  G4int                 evtNb;
  G4Timer* evtTimer;
};

#endif

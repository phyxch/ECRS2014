// 2/12/2015 Hexc, Olesya: add event action to this class
#ifndef ECRSTrackingAction_h
#define ECRSTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"
#include "ECRSSingleton.hh"
#include "ECRSRunAction.hh"
#include "ECRSEventAction.hh"

class ECRSTrackingMessenger;

class ECRSTrackingAction : public G4UserTrackingAction
{
public:
  ECRSTrackingAction(ECRSRunAction*, ECRSEventAction*);
  ~ECRSTrackingAction();
  
  void PreUserTrackingAction(const G4Track*);
  void PostUserTrackingAction(const G4Track*);
  
private:
  ECRSSingleton*		fileOut;
  ECRSTrackingMessenger*	trackMessenger;
  ECRSRunAction*                run_action;
  ECRSEventAction*              evt_action;
};

#endif

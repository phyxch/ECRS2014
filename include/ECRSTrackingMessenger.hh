#ifndef ECRSTrackingMessenger_h
#define ECRSTrackingMessenger_h 1

#include "G4UImessenger.hh"

class ECRSTrackingAction;

class ECRSTrackingMessenger : public G4UImessenger
{
  public:
    ECRSTrackingMessenger(ECRSTrackingAction*);
    ~ECRSTrackingMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    ECRSTrackingAction*	trackAction;
};

#endif

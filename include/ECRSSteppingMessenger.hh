#ifndef ECRSSteppingMessenger_h
#define ECRSSteppingMessenger_h 1

#include "G4UImessenger.hh"

class ECRSSteppingAction;

class ECRSSteppingMessenger : public G4UImessenger
{
  public:
    ECRSSteppingMessenger(ECRSSteppingAction*);
    ~ECRSSteppingMessenger();

    void SetNewValue(G4UIcommand*,G4String);

  private:
    ECRSSteppingAction*	stepAction;
};

#endif

#ifndef ECRSEventActionMessenger_h
#define ECRSEventActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class ECRSEventAction;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

class ECRSEventActionMessenger : public G4UImessenger
{
  public:
    ECRSEventActionMessenger(ECRSEventAction*);
    ~ECRSEventActionMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    ECRSEventAction* eventAction;
    G4UIcmdWithAString* DrawCmd;
    G4UIcmdWithAnInteger* PrintCmd;
};

#endif

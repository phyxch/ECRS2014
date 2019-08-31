#ifndef ECRSStackingMessenger_h
#define ECRSStackingMessenger_h 1

class ECRSStackingAction;
class G4UIcmdWithADoubleAndUnit;

#include "G4UImessenger.hh"
#include "globals.hh"

class ECRSStackingMessenger: public G4UImessenger
{
  public:
    ECRSStackingMessenger(ECRSStackingAction* msa);
    ~ECRSStackingMessenger();

  public:
    void SetNewValue(G4UIcommand* command,G4String newValues);

  private:
    ECRSStackingAction* myAction;
    G4UIcmdWithADoubleAndUnit* minEKEcmd;
    G4UIcmdWithADoubleAndUnit* minGammaKEcmd;
};

#endif

#include "ECRSSteppingMessenger.hh"
#include "ECRSSteppingAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"

ECRSSteppingMessenger::ECRSSteppingMessenger(ECRSSteppingAction* act)
:stepAction(act)
{
}

ECRSSteppingMessenger::~ECRSSteppingMessenger()
{
}

void ECRSSteppingMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
}

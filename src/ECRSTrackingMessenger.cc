#include "ECRSTrackingMessenger.hh"
#include "ECRSTrackingAction.hh"

#include "G4UIcmdWithAnInteger.hh"

ECRSTrackingMessenger::ECRSTrackingMessenger(ECRSTrackingAction* TrAct)
:trackAction(TrAct)
{
}

ECRSTrackingMessenger::~ECRSTrackingMessenger()
{
}

void ECRSTrackingMessenger::SetNewValue
		(G4UIcommand* command, G4String newValue)
{
}

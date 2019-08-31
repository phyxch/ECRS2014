#include "ECRSEventActionMessenger.hh"
#include "ECRSEventAction.hh"

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "globals.hh"

ECRSEventActionMessenger::ECRSEventActionMessenger(ECRSEventAction* EvAct)
:eventAction(EvAct)
{
  DrawCmd = new G4UIcmdWithAString("/event/drawTracks",this);
  DrawCmd->SetGuidance("Draw the tracks in the event");
  DrawCmd->SetGuidance("  Choice: none, charged, neutral, all (default)");
  DrawCmd->SetParameterName("choice",true);
  DrawCmd->SetDefaultValue("all");
  DrawCmd->SetCandidates("none charged neutral all muons no_e no_e&gamma");
  DrawCmd->AvailableForStates(G4State_Idle);

  PrintCmd = new G4UIcmdWithAnInteger("/event/printModulo",this);
  PrintCmd->SetGuidance("Print events modulo n");
  PrintCmd->SetParameterName("EventNb",false);
  PrintCmd->SetRange("EventNb>0");
  PrintCmd->AvailableForStates(G4State_Idle);
}

ECRSEventActionMessenger::~ECRSEventActionMessenger()
{
  delete DrawCmd;
  delete PrintCmd;
}

void ECRSEventActionMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{
  if (command == DrawCmd)  {eventAction->SetDrawFlag(newValue);}

  if (command == PrintCmd)
	{eventAction->SetPrintModulo(PrintCmd->GetNewIntValue(newValue));}
}

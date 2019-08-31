#include "ECRSStackingMessenger.hh"
#include "ECRSStackingAction.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4ios.hh"

ECRSStackingMessenger::ECRSStackingMessenger(ECRSStackingAction* 
		msa):myAction(msa)
{
  minEKEcmd = new G4UIcmdWithADoubleAndUnit("/tracking/minEKE",this);
  minEKEcmd->SetGuidance("Minimum kinetic energy for secondary electron tracking.");
  minEKEcmd->SetParameterName("minEKE",true);
  minEKEcmd->SetDefaultUnit("MeV");
  minEKEcmd->SetDefaultValue(0.5);
  minEKEcmd->SetRange("minEKE>=0");
  minEKEcmd->AvailableForStates(G4State_Idle);

  minGammaKEcmd = new G4UIcmdWithADoubleAndUnit("/tracking/minGammaKE",this);
  minGammaKEcmd->SetGuidance("Minimum kinetic energy for secondary gamma traking.");
  minGammaKEcmd->SetParameterName("minGKE",true);
  minGammaKEcmd->SetDefaultUnit("MeV");
  minGammaKEcmd->SetDefaultValue(0.3);
  minGammaKEcmd->SetRange("minGKE>0");
  minGammaKEcmd->AvailableForStates(G4State_Idle);
}

ECRSStackingMessenger::~ECRSStackingMessenger()
{
  delete minEKEcmd;
  delete minGammaKEcmd;
}

void ECRSStackingMessenger::SetNewValue(G4UIcommand* command,
		G4String newValue)
{
  if (command==minEKEcmd)
  {myAction->SetMinEKE(minEKEcmd->GetNewDoubleValue(newValue));}

  if (command==minGammaKEcmd)
  {myAction->SetMinGammaKE(minGammaKEcmd->GetNewDoubleValue(newValue));}
}

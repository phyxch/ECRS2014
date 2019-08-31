#include "ECRSDetectorMessenger.hh"
#include "ECRSDetectorConstruction.hh"
#include "globals.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"

ECRSDetectorMessenger::ECRSDetectorMessenger(
ECRSDetectorConstruction* detector)
:ECRSDetector(detector)
{
  MultCmd = new G4UIcmdWithADouble("/geometry/multiplier",this);
  MultCmd->SetGuidance("Parameter to multiply all dimensions by.");
  MultCmd->SetParameterName("Multiplier",true);
  MultCmd->SetDefaultValue(1.0);
  MultCmd->SetRange("Multiplier>0");
  MultCmd->AvailableForStates(G4State_Idle);

  UpdateCmd = new G4UIcmdWithoutParameter("/geometry/update",this);
  UpdateCmd->SetGuidance("Update geometry.");
  UpdateCmd->SetGuidance("This command must be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you used the multiplier command.");
  UpdateCmd->AvailableForStates(G4State_Idle);

  DefineCmd = new G4UIcmdWithoutParameter("/geometry/updateMaterials",this);
  DefineCmd->SetGuidance("Update the materials.");
  DefineCmd->AvailableForStates(G4State_Idle);

  PrintParCmd = new G4UIcmdWithoutParameter("/geometry/PrintParameters",this);
  PrintParCmd->SetGuidance("Prints current sizes of Universe and ECRS,");
  PrintParCmd->SetGuidance("and layers of the atmospheres.");
  PrintParCmd->AvailableForStates(G4State_Idle);

  AtmoVisCmd = new G4UIcmdWithABool("/geometry/AtmVisibility",this);
  AtmoVisCmd->SetGuidance("Set visibility of inner atmospheric layers.");
  AtmoVisCmd->SetGuidance("Update geometry after application of this command");
  AtmoVisCmd->SetParameterName("VisFlag",true);
  AtmoVisCmd->SetDefaultValue(false);
  AtmoVisCmd->AvailableForStates(G4State_Idle);

  DensityCmd = new G4UIcmdWithADouble("/geometry/densityMultiplier",this);
  DensityCmd->SetGuidance("Set the density multiplier factor.");
  DensityCmd->SetGuidance("Update geometry after application of this command.");
  DensityCmd->SetParameterName("multDensity",true);
  DensityCmd->SetDefaultValue(1.0);
  DensityCmd->SetRange("multDensity>0");
  DensityCmd->AvailableForStates(G4State_Idle);

  ExtMCmd = new G4UIcmdWithABool("/geometry/externalMag",this);
  ExtMCmd->SetGuidance("Flag for External magnetic field.");
  ExtMCmd->SetParameterName("ExtMag",true);
  ExtMCmd->SetDefaultValue(false);
  ExtMCmd->AvailableForStates(G4State_Idle);

}

ECRSDetectorMessenger::~ECRSDetectorMessenger()
{
  delete MultCmd;
  delete UpdateCmd;
  delete DefineCmd;
  delete PrintParCmd;
  delete AtmoVisCmd;
  delete DensityCmd;
  delete ExtMCmd;
}

void ECRSDetectorMessenger::SetNewValue(G4UIcommand* command,
G4String newValue)
{
  if (command == MultCmd)
    {ECRSDetector->SetMultiplier(MultCmd->GetNewDoubleValue(newValue));}

  if (command == UpdateCmd)
    {ECRSDetector->UpdateGeometry();}

  if (command == DefineCmd)
    {ECRSDetector->UpdateMaterials();}

  if (command == PrintParCmd)
    {ECRSDetector->PrintAllParameters();}

  if (command == AtmoVisCmd)
    {ECRSDetector->SetVisFlag(AtmoVisCmd->GetNewBoolValue(newValue));}

  if (command == DensityCmd)
    {ECRSDetector->SetDensityMult(DensityCmd->GetNewDoubleValue(newValue));}


   if (command == ExtMCmd)
    {ECRSDetector->SetExtMag(newValue);}
  

//   if (command == ExtMCmd)
//    {ECRSDetector->SetExtMag(ExtMCmd->GetNewBoolValue(newValue));}

}

// 9/10/2014, Hexc, Olesya: Add more command line parameters for defining the
//   direction of the primary particles
// 3/12/2015, Hexc, Olesya: Add messenger for running cosmic ray events
//
#include "ECRSPrimaryGeneratorMessenger.hh"

#include "ECRSPrimaryGeneratorAction.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UnitsTable.hh"

ECRSPrimaryGeneratorMessenger::ECRSPrimaryGeneratorMessenger(ECRSPrimaryGeneratorAction* ECRSGun)
:ECRSAction(ECRSGun)
{
  PosCmd = new G4UIcmdWith3VectorAndUnit("/gun/pos",this);
  PosCmd->SetGuidance("Select launch position in the world volume in meters.");
  PosCmd->SetGuidance("Enter the position as a normal 3 vector.");
  PosCmd->SetParameterName("Position x","Position y","Position z",false);
  PosCmd->SetDefaultUnit("m");
  PosCmd->AvailableForStates(G4State_Idle);

  KECmd = new G4UIcmdWithADoubleAndUnit("/gun/KE",this);
  KECmd->SetGuidance("Set kinetic energy of primary particle.");
  KECmd->SetParameterName("energy",false);
  KECmd->SetUnitCategory("Energy");
  KECmd->SetRange("energy>=0");

  RndECmd = new G4UIcmdWithAString("/gun/rndEnergy",this);
  RndECmd->SetGuidance("Flag for random energy generation.");
  RndECmd->SetParameterName("RndEnergy",true);
  RndECmd->SetDefaultValue("off");
  RndECmd->SetCandidates("on off");

  lowKECmd = new G4UIcmdWithADoubleAndUnit("/gun/lowKE",this);
  lowKECmd->SetGuidance("Set the low end of the kinetic energy distribution.");
  lowKECmd->SetParameterName("LowKE",false);
  lowKECmd->SetUnitCategory("Energy");
  lowKECmd->SetRange("LowKE>0");

  highKECmd = new G4UIcmdWithADoubleAndUnit("/gun/highKE",this);
  highKECmd->SetGuidance("Set the high end of the kinetic energy distribution.");
  highKECmd->SetParameterName("HighKE",false);
  highKECmd->SetUnitCategory("Energy");
  highKECmd->SetRange("HighKE>0");

  PosFlagCmd = new G4UIcmdWithAString("/gun/rndPos",this);
  PosFlagCmd->SetGuidance("Set the random position flag.");
  PosFlagCmd->SetGuidance("Launches particle randomly from a sphere.");
  PosFlagCmd->SetParameterName("RndPosition",true);
  PosFlagCmd->SetDefaultValue("off");
  PosFlagCmd->SetCandidates("on off");

  AngFlagCmd = new G4UIcmdWithAString("/gun/rndAng",this);
  AngFlagCmd->SetGuidance("Set the random launch angle flag.");
  AngFlagCmd->SetGuidance("This is only effective if rndPos is set to on.");
  AngFlagCmd->SetParameterName("RndAngle",true);
  AngFlagCmd->SetDefaultValue("off");
  AngFlagCmd->SetCandidates("on off");

  // Define "CosmicRay" option
  CosmicRay = new G4UIcmdWithAString("/gun/cosmicRay",this);
  CosmicRay->SetGuidance("Incident primary cosmic protons (based on PDG).");
  CosmicRay->SetGuidance("  Choice : on, off(default)");
  CosmicRay->SetParameterName("choice",true);
  CosmicRay->SetDefaultValue("off");
  CosmicRay->SetCandidates("on off");
  CosmicRay->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Define "Rigidity cut" option
  RigiCutCmd = new G4UIcmdWithADouble("/gun/RigidityCut", this);
  RigiCutCmd->SetGuidance("Set the cut off rigitity value in the location");
  RigiCutCmd->SetGuidance("Unit is GeV. Default value is 1.0");
  RigiCutCmd->SetGuidance("This option can only be used in the cosmicRay on mode");
  RigiCutCmd->SetParameterName("RigiCutCmd",true);
  RigiCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  G4UIparameter* length_unit_param = new G4UIparameter("Length Unit",'s',false);
  length_unit_param->SetParameterCandidates("Re re RE km m");
  G4UIparameter* angle_unit_param = new G4UIparameter("Angle Unit",'s',false);
  angle_unit_param->SetParameterCandidates("degree deg rad radian mrad milliradian");
  
  G4UIparameter* altitude_param = new G4UIparameter("Altitude",'d',false);
  G4UIparameter* longitude_param = new G4UIparameter("Longitude",'d',false);
  G4UIparameter* latitude_param = new G4UIparameter("Latitude",'d',false);
  
  SetPositionCmd = new G4UIcommand("/gun/SetPosition",this);
  SetPositionCmd->SetGuidance("Set the altitude, latitude and longitude defining the start position ");
  SetPositionCmd->SetGuidance( "in your selected coordinate system");
  SetPositionCmd->SetGuidance("[usage] /gun/SetPosition ");
  SetPositionCmd->SetGuidance("altitude length_unit latitude longitude angle_unit");
  SetPositionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  SetPositionCmd->SetParameter(altitude_param);
  SetPositionCmd->SetParameter(length_unit_param);
  SetPositionCmd->SetParameter(latitude_param);
  SetPositionCmd->SetParameter(longitude_param);
  SetPositionCmd->SetParameter(angle_unit_param);
  
}

ECRSPrimaryGeneratorMessenger::~ECRSPrimaryGeneratorMessenger()
{
  delete PosCmd;
  delete KECmd;
  delete RndECmd;
  delete lowKECmd;
  delete highKECmd;
  delete PosFlagCmd;
  delete AngFlagCmd;
  delete CosmicRay;
  delete RigiCutCmd;
}

void ECRSPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == PosCmd)
    {ECRSAction->SetPosValue(PosCmd->GetNew3VectorValue(newValue));}

  if (command == KECmd)
    {ECRSAction->SetKE(KECmd->GetNewDoubleValue(newValue));}

  if (command == RndECmd)
    {ECRSAction->SetRndEFlag(newValue);}

  if (command == lowKECmd)
    {ECRSAction->SetLowKE(lowKECmd->GetNewDoubleValue(newValue));}

  if (command == highKECmd)
    {ECRSAction->SetHighKE(highKECmd->GetNewDoubleValue(newValue));}

  if (command == PosFlagCmd)
    {ECRSAction->SetRndPosFlag(newValue);}

  if (command == AngFlagCmd)
    {ECRSAction->SetRndAngFlag(newValue);}

  if( command == CosmicRay )
   { ECRSAction->SetCosmicRayFlag(newValue);}

  if( command == RigiCutCmd )
   { ECRSAction->SetRigiCut(RigiCutCmd->GetNewDoubleValue(newValue));}
 
  if (command == SetPositionCmd )
    { 
      G4double altitude, latitude, longitude;
      G4String  length_unit, angle_unit;
      
      /* this example is copied from the Geant4 source: G4VVisCommands.cc
      std::istringstream is(paramString);
      is >> x >> y >> unts;
      G4String unt = unts;
      
      xval = x*G4UIcommand::ValueOf(unt);
      yval = y*G4UIcommand::ValueOf(unt);
      */

      std::istringstream is(newValue);
      is >>  altitude >> length_unit >> latitude >> longitude >> angle_unit ;

      altitude *=G4UnitDefinition::GetValueOf(length_unit);
      longitude *=G4UnitDefinition::GetValueOf(angle_unit);
      latitude *=G4UnitDefinition::GetValueOf(angle_unit);

      G4cout << "altitude = " << altitude << "  length_unit = " << length_unit << "  latitude = " << latitude << "  longitude = " << longitude << "   angle_unit = " << angle_unit << G4endl;

      ECRSAction->SetPosition(altitude,longitude,latitude);
    }
}

// 9/10/2014, Hexc, Olesya: Add more command line parameters for defining the
//   direction of the primary particles
// 3/12/2015, Hexc, Olesya: Add messenger for running cosmic ray events
//
#ifndef ECRSPrimaryGeneratorMessenger_h
#define ECRSPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class ECRSPrimaryGeneratorAction;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;

class ECRSPrimaryGeneratorMessenger: public G4UImessenger
{
public:
  ECRSPrimaryGeneratorMessenger(ECRSPrimaryGeneratorAction*);
  ~ECRSPrimaryGeneratorMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);
  
private:
  ECRSPrimaryGeneratorAction*	ECRSAction;
  G4UIcmdWith3VectorAndUnit*	PosCmd;
  G4UIcmdWithADoubleAndUnit*	KECmd;
  G4UIcmdWithAString*		RndECmd;
  G4UIcmdWithADoubleAndUnit*	lowKECmd;
  G4UIcmdWithADoubleAndUnit*	highKECmd;
  G4UIcmdWithAString*		PosFlagCmd;
  G4UIcmdWithAString*		AngFlagCmd;
  G4UIcmdWithAString*           CosmicRay;
  G4UIcmdWithADouble*           RigiCutCmd; 

  G4UIcommand*                  SetPositionCmd;  // Define the primary particle position
};

#endif

#ifndef ECRSFieldMessenger_h
#define ECRSFieldMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class ECRSMagneticField;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithADouble;

class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;

class ECRSFieldMessenger : public G4UImessenger
{public:
  ECRSFieldMessenger(ECRSMagneticField* aField);
  ~ECRSFieldMessenger();
  
  void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:
  ECRSMagneticField* theField;
  G4UIdirectory*     mainDir;
  G4UIdirectory*     IntegrationDir;
  G4UIdirectory*     MagnetoDir;  
  
  //integration command
  G4UIcmdWithADouble* SetEpsilonCmd;
  G4UIcmdWithADoubleAndUnit* SetG4DeltaChordCmd; 
  G4UIcmdWithADoubleAndUnit* SetDeltaIntersectionCmd;
  G4UIcmdWithADoubleAndUnit* SetBSMaxStepCmd;
  G4UIcmdWithoutParameter*  ResetIntegrationParametersCmd;
  G4UIcmdWithoutParameter*  SelectBulirshStoerMethodCmd;
  G4UIcmdWithoutParameter*  SelectG4IntegrationMethodCmd;
  G4UIcmdWithAString* SetStepperCmd;
  
  // magnetic field model command
  
  G4UIcmdWithADoubleAndUnit* SetTimeOfBCmd;
  G4UIcommand* SetStartDateCmd;
  G4UIcmdWithAnInteger* Setnm_igrfCmd;
  G4UIcmdWithAString* SetExternalFieldCmd;
  G4UIcmdWithAString* SetInternalFieldCmd;
  G4UIcmdWithAString* SetMagnetopauseModelCmd;
  G4UIcmdWithADoubleAndUnit* SetDipoleB0Cmd;
  G4UIcmdWithADoubleAndUnit* SetDipolePSCmd;
  G4UIcmdWith3VectorAndUnit* SetDipoleShiftCmd; 
  G4UIcommand* SetDipoleAxisCmd; 
  G4UIcmdWithABool* SetConsiderDipoleShiftCmd; 
  G4UIcmdWithoutParameter* SetTiltedDipoleParameterFromIGRFCmd;
  G4UIcmdWithoutParameter* SetEccentricDipoleParameterFromIGRFCmd;
  
  
  // Magnetic Activity Command
  
  G4UIcmdWithAnInteger* SetIoptCmd;
  G4UIcmdWithADouble* SetPdynCmd;
  G4UIcmdWithADoubleAndUnit* SetDstCmd;
  G4UIcmdWithADoubleAndUnit* SetImfyCmd;
  G4UIcmdWithADoubleAndUnit* SetImfzCmd;
  G4UIcmdWithADouble* SetG1Cmd;
  G4UIcmdWithADouble* SetG2Cmd;
  G4UIcmdWithAString* ReadTSYParameterCmd;
  G4UIcmdWithoutParameter* PrintTSYParameterCmd; 
};

#endif

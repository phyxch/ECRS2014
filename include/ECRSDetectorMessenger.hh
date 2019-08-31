#ifndef ECRSDetectorMessenger_h
#define ECRSDetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class G4UIcmdWithAString;
class ECRSDetectorConstruction;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

class ECRSDetectorMessenger : public G4UImessenger
{
  public:
    ECRSDetectorMessenger(ECRSDetectorConstruction*);
    ~ECRSDetectorMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    ECRSDetectorConstruction*		ECRSDetector;
    G4UIcmdWithADouble*			MultCmd;
    G4UIcmdWithoutParameter*		UpdateCmd;
    G4UIcmdWithoutParameter*		DefineCmd;
    G4UIcmdWithoutParameter*		PrintParCmd;
    G4UIcmdWithABool*			AtmoVisCmd;
    G4UIcmdWithADouble*			DensityCmd;
    G4UIcmdWithABool*                 ExtMCmd;
 
};

#endif

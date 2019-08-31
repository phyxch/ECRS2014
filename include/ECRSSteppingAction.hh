// Dec 8, 2014: Hexc, Olesya - Adding code for filling the hits ntuple for storing hits at
//   the surface of the earth.
// 2/11/2015: Hexc, Olesya - Add eventAction and primaryGeneratorAction 
// 2/12/2015: Hexc, Olesya - Removed primary particle info from the output since it takes up too large disk space.
#ifndef ECRSSteppingAction_h
#define ECRSSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "ECRSRunAction.hh"
#include "ECRSEventAction.hh"
//#include "ECRSPrimaryGeneratorAction.hh"


class ECRSDetectorConstruction;
class ECRSSingleton;
class ECRSSteppingMessenger;
class G4Track;
class G4ParticleDefinition;
class G4VPhysicalVolume;
class G4StepPoint;

class ECRSSteppingAction : public G4UserSteppingAction
{
public:
  ECRSSteppingAction(ECRSDetectorConstruction*, ECRSRunAction*, ECRSEventAction*);
  ~ECRSSteppingAction();
  
  void UserSteppingAction(const G4Step*);
  
private:
  ECRSRunAction*                run_action;
  ECRSEventAction*              evt_action;
  ECRSDetectorConstruction*	detector;
  ECRSSingleton*		fileOut;
  ECRSSteppingMessenger*	theMessenger;
  G4Track*			aTrack;
  G4ParticleDefinition*		particle;
  G4VPhysicalVolume*		currentVolume;
  G4VPhysicalVolume*		nextVolume;
  G4StepPoint*			point;
  G4double			KE;
  G4String			particleName;
};

#endif

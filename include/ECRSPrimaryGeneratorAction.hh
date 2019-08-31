// 9/10/2014, Hexc, Olesya: Add more command line parameters for defining the
//   direction of the primary particles
//
#ifndef ECRSPrimaryGeneratorAction_h
#define ECRSPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleGun.hh"
#include "ECRSRunAction.hh"
#include "ECRSEventAction.hh"

class G4Event;
class ECRSPrimaryGeneratorMessenger;
class ECRSDetectorConstruction;

class ECRSPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  ECRSPrimaryGeneratorAction(ECRSDetectorConstruction*, ECRSRunAction*, ECRSEventAction*);
  ~ECRSPrimaryGeneratorAction();
  void GeneratePrimaries(G4Event* anEvent);
  void SetKE(G4double val)
  {KE = val;};
  void SetPosValue(G4ThreeVector val)
  {pos = val;};
  void SetRndEFlag(G4String newValue)
  {rndEFlag = newValue;};
  void SetLowKE(G4double newValue)
  {lowKE = newValue;};
  void SetHighKE(G4double newValue)
  {highKE = newValue;};
  void SetRndPosFlag(G4String newValue)
  {rndPosFlag = newValue;};
  void SetRndAngFlag(G4String newValue)
  {rndAngFlag = newValue;};
  void SetCosmicRayFlag(G4String val) 
  { cosmicRay = val;};
  void SetRigiCut(G4double val) 
  { RigiVal = val;};
  void SetPosition(G4double Alt, G4double Long, G4double Lat);
  G4ParticleGun* getParticleGun() 
  {return particleGun;};
  G4double probProtonKE(G4double energy, G4double NormCnt);
  
private:
  
  G4ParticleGun*		        particleGun;
  G4ThreeVector			        pos;
  ECRSRunAction*                        run_action;
  ECRSEventAction*                      evt_action;
  ECRSPrimaryGeneratorMessenger*	gunMessenger;
  ECRSDetectorConstruction*		detector;
  G4String				rndEFlag;
  G4String                              cosmicRay;   //flag for cosmic ray
  G4double				KE;
  G4double				lowKE;
  G4double				highKE;
  G4String				rndPosFlag;
  G4String				rndAngFlag;
  G4double                              RigiVal;  //rigitity value
  G4double                              NormConst;
};

#endif

// Cleaning up and reformatting, May 25, 2009, hexc
//
// Update Date: Hexc, Olesya, 6/19/2014: Making major updates for refining the ATM layer definition. 
//
#ifndef ECRSAtmosphereSD_h
#define ECRSAtmosphereSD_h 1

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

#include "ECRSAtmoHit.hh"

class ECRSDetectorConstruction;
class G4HCofThisEvent;
class G4Step;

class ECRSAtmosphereSD : public G4VSensitiveDetector
{
public:
  ECRSAtmosphereSD(G4String, ECRSDetectorConstruction*);
  ~ECRSAtmosphereSD();
  
  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);
  void clear();
  void DrawAll();
  void PrintAll();
  
private:
  AtmoHitsCollection*	AtmoCollection;
  ECRSDetectorConstruction*	Detector;
  G4int*			HitID;
};

#endif

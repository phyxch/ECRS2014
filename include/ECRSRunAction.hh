// Oct 1, 2015: Hexc, Olesya - Adding code to write root output for data
//    analysis.
// Dec 8, 2014: Hexc, Olesya - Adding array of ntuple entry IDs
#ifndef ECRSRunAction_h
#define ECRSRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Timer.hh"
#include "globals.hh"

class G4Run;

class ECRSRunAction : public G4UserRunAction
{
public:
  ECRSRunAction();
  ~ECRSRunAction();
  
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
  G4int GetNtColID(G4int id) { return fNtColID[id];};

private:
  G4Timer* timer;
  G4int fNtColID[32];  // The length of the array should be increased if more variables are added.
};

#endif

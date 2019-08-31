#ifndef ECRSStackingAction_h
#define ECRSStackingAction_h 1

#include "G4UserStackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "globals.hh"

class ECRSStackingMessenger;

class ECRSStackingAction: public G4UserStackingAction
{
  public:
    ECRSStackingAction();
    ~ECRSStackingAction();
    
  protected:
    G4StackManager* stackManager;

  public:
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
    void SetMinEKE(G4double val)
	{minEminusKE = val;}
    void SetMinGammaKE(G4double val)
	{minGammaKE = val;}

  private:
    G4double minEminusKE;
    G4double minGammaKE;
    ECRSStackingMessenger*	StackMessenger;
};

#endif

// 9/4/2014, Hexc, Olesya
// We are not sure about exactly the function of this class. It will not be registered in the run manager
#include "ECRSStackingAction.hh"
#include "ECRSStackingMessenger.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

using namespace CLHEP;

ECRSStackingAction::ECRSStackingAction()
{
  minEminusKE = 0.5*MeV;
  minGammaKE = 0.3*MeV;
  StackMessenger = new ECRSStackingMessenger(this);
}

ECRSStackingAction::~ECRSStackingAction()
{
  delete StackMessenger;
}

G4ClassificationOfNewTrack ECRSStackingAction::ClassifyNewTrack(const
								G4Track* aTrack)
{
  G4ClassificationOfNewTrack classification = fWaiting;
  G4ParticleDefinition* particleType = aTrack->GetDefinition();
  
  // The following kills the tracking of secondary electrons from the primary
  // and all neutrinos
  if (((aTrack->GetKineticEnergy()<minEminusKE) && 
       (particleType==G4Electron::ElectronDefinition())) ||
      ((aTrack->GetKineticEnergy()<minGammaKE) &&
       (particleType==G4Gamma::GammaDefinition())) ||
      (particleType==G4NeutrinoE::NeutrinoEDefinition()) ||
      (particleType==G4AntiNeutrinoE::AntiNeutrinoEDefinition()) ||
      (particleType==G4NeutrinoMu::NeutrinoMuDefinition()) ||
      (particleType==G4AntiNeutrinoMu::AntiNeutrinoMuDefinition()) ||
      (particleType==G4NeutrinoTau::NeutrinoTauDefinition()) ||
      (particleType==G4AntiNeutrinoTau::AntiNeutrinoTauDefinition()))
    {classification = fKill;}
  else classification = fUrgent;
  
  return classification;
}

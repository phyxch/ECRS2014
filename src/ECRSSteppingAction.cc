// 7/21/2014: Hexc, Olesya:
// Modified the output format to write out hits info when the hits are close to 
// the surface of the Earth.
// 2/11/2015: Hexc, Olesya - Add eventAction and the primaryGeneratorAction in order to 
//              output the event number and the primary particle info.
// 4/28/2015: Hexc, Olesya - Add aTrack->SetTrackStatus(fStopAndKill); when the hit is below the surface.
// 10/31/2017: Hexc - Stop tracking a particle after 10 seconds. 
//
#include "ECRSSteppingAction.hh"
#include "ECRSSteppingMessenger.hh"
#include "ECRSDetectorConstruction.hh"
#include "ECRSSingleton.hh"

#include "G4StepPoint.hh"
#include "G4ParticleDefinition.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"

#include "g4root.hh"

using namespace CLHEP;

ECRSSteppingAction::ECRSSteppingAction(ECRSDetectorConstruction* det, 
				       ECRSRunAction* run_action_in, 
				       ECRSEventAction* evt_action_in)
  :detector(det),run_action(run_action_in),evt_action(evt_action_in),
   aTrack(NULL),particle(NULL),
   currentVolume(NULL),nextVolume(NULL),point(NULL)
{
  fileOut = ECRSSingleton::instance();
  KE = 0;
  particleName = "";
  theMessenger = new ECRSSteppingMessenger(this);
}

ECRSSteppingAction::~ECRSSteppingAction()
{
  delete theMessenger;
}

void ECRSSteppingAction::UserSteppingAction(const G4Step* aStep)
{
  aTrack = aStep->GetTrack();
  currentVolume = aTrack->GetVolume();
  nextVolume = aTrack->GetNextVolume();
  G4double Global_time = aTrack->GetGlobalTime();

  // Stop tracking when the global time exceeds 10 seconds;
  if (Global_time > 10.0*s){
    aTrack->SetTrackStatus(fStopAndKill);
    return;
  }
  
  if ((currentVolume != detector->GetECRS()) &&
      (nextVolume == detector->GetECRS()))         // output the hits info while crossing the earth surface boundary
    {
      //  G4int PID = particle->GetPDGEncoding();
 
      G4double Local_time = aTrack->GetLocalTime();
      G4double Proper_time = aTrack->GetProperTime();
      KE = aTrack->GetKineticEnergy();
      particle = aTrack->GetDefinition();
      G4int PID = particle->GetPDGEncoding();
      particleName = particle->GetParticleName();

      // find and set event ID
      G4int evtID = evt_action->getCurrentEventID();

      G4ThreeVector position = aTrack->GetPosition();
      G4ThreeVector mom = aTrack->GetMomentum();
      
      // For now we are going to write out the hits information close to the 
      // surface of the earth.
      // We comment this text output since the data is stored in the hits ntuple 
      fileOut->fout << 0 << "  " <<particle->GetParticleName() <<"  "<<PID<<"  "<< "  " << KE/MeV << "  "  
		    << position.getX()/m << "  " << position.getY()/m << "  "
		    << position.getZ()/m <<  "   "  << Global_time/ms << "   "  
		    << Local_time/ms << "   "  << Proper_time/ms << G4endl;      
      

      G4int flagParticle = 99;   // Initialize 
            
      if (particleName == "proton") { flagParticle = 1; }
      else if (particleName == "neutron")  { flagParticle = 2; }
      else if (particleName == "mu+") { flagParticle = 3; }
      else if (particleName == "mu-") { flagParticle = 4; }
      else if (particleName == "e+") { flagParticle = 5; }
      else if (particleName == "e-") { flagParticle = 6; }
      else if (particleName == "gamma") {flagParticle = 7; }
      else if (particleName == "pi+") { flagParticle = 8; }
      else if (particleName == "pi-") { flagParticle = 9; }
      else if (particleName == "C12")  { flagParticle = 10; }
      else if (particleName == "C13")  { flagParticle = 11; }
      else if (particleName == "He3")  { flagParticle = 12; }
      else if (particleName == "deutron") { flagParticle = 13; }
      else if (particleName == "N14") { flagParticle = 14; }
      else if (particleName == "anti_proton") { flagParticle = 15; }
      else if (particleName == "anti_neutron") { flagParticle = 16; }
      else if (particleName == "triton") { flagParticle = 17; }
      else 
	{
	  flagParticle = 99;
	}
      
      // get analysis manager
      G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
      
      // fill ntuple (ntup[le ID = 2)
      analysisManager->FillNtupleDColumn(2, run_action->GetNtColID(12), flagParticle);
      analysisManager->FillNtupleDColumn(2, run_action->GetNtColID(13), KE/MeV);
      analysisManager->FillNtupleDColumn(2, run_action->GetNtColID(14), position.getX()/m);
      analysisManager->FillNtupleDColumn(2, run_action->GetNtColID(15), position.getY()/m);
      analysisManager->FillNtupleDColumn(2, run_action->GetNtColID(16), position.getZ()/m);
      analysisManager->FillNtupleDColumn(2, run_action->GetNtColID(17), mom.getX()/MeV);
      analysisManager->FillNtupleDColumn(2, run_action->GetNtColID(18), mom.getY()/MeV);
      analysisManager->FillNtupleDColumn(2, run_action->GetNtColID(19), mom.getZ()/MeV);
      analysisManager->FillNtupleDColumn(2, run_action->GetNtColID(20), Global_time/ms);
      analysisManager->FillNtupleDColumn(2, run_action->GetNtColID(21), Local_time/ms);  
      analysisManager->FillNtupleDColumn(2, run_action->GetNtColID(22), evtID);  // added on 2/10/2015

      analysisManager->AddNtupleRow(2);

      // A quick solution for stopping tracks below the surface
      // 
      aTrack->SetTrackStatus(fStopAndKill);  
      
    } 
}

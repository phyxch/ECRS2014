// 6/19/2014 Hexc, Olesya: We defined the atm layer structure
#include "ECRSEventAction.hh"
#include "ECRSEventActionMessenger.hh"
#include "ECRSAtmoHit.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"


ECRSEventAction::ECRSEventAction()
:atmosphereCollID(-1),drawFlag("all"),eventMessenger(NULL),printModulo(1)
{
  eventMessenger = new ECRSEventActionMessenger(this);
  evtTimer = new G4Timer();
}

ECRSEventAction::~ECRSEventAction()
{
  delete eventMessenger;
}

void ECRSEventAction::BeginOfEventAction(const G4Event* evt)
{
  evtNb = evt->GetEventID();
  if (evtNb % printModulo == 0)
    {
      G4cout << "\n---> Begin of event: " << evtNb << G4endl;
      CLHEP::HepRandom::showEngineStatus();
    }
  
  // Start the event timer
  evtTimer->Start();

  if (atmosphereCollID==-1)
    {
      G4SDManager* SDman = G4SDManager::GetSDMpointer();
      atmosphereCollID = SDman->GetCollectionID("AtmoCollection");
    }
}

void ECRSEventAction::EndOfEventAction(const G4Event* evt)
{
  // evtNb = evt->GetEventID();
  
  /*  // this section of the code is obsolete
    G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
    AtmoHitsCollection* CHC = 0;
    G4int n_hit = 0;
    
    G4double totEdepLayer[NUMBER_LAYER], totTrackLengthLayer[NUMBER_LAYER];
    for (G4int i = 0; i < NUMBER_LAYER; i++)
    {
    totEdepLayer[i] = 0.;
    totTrackLengthLayer[i] = 0.;
    }
    
    if (HCE) CHC = (AtmoHitsCollection*)(HCE->GetHC(atmosphereCollID));
    
    G4int iLayer = -1;
    
    if (CHC)
    {
    n_hit = CHC->entries();
    for (G4int i=0; i<n_hit; i++)
    {
    iLayer =  (*CHC)[i]->GetCurrentLayerNumber();
    totEdepLayer[iLayer] =(*CHC)[i]->GetEdepAtmLayer(iLayer);
    totTrackLengthLayer[iLayer] =(*CHC)[i]->GetTrackLengthAtmLayer(iLayer);
    }
    }
    
    if (evtNb % printModulo == 0)
    {
    G4cout << "---> End of event: " << evtNb << G4endl;
    G4cout << "\n    " << n_hit
    << " hits are stored in AtmoHitsCollection." << G4endl;
    }
    
  */
  
  // extract the trajectories and draw them
  
  if (G4VVisManager::GetConcreteInstance())
    {
      G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
      G4int n_trajectories = 0;
      if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
      
      for (G4int i=0; i<n_trajectories; i++)
	{
	  G4Trajectory* trj = (G4Trajectory*)((*(evt->GetTrajectoryContainer()))[i]);
	  if (drawFlag == "all") trj->DrawTrajectory();  // changed from:  trj->DrawTrajectory(50);
	  else if ((drawFlag == "charged") && (trj->GetCharge() != 0.))
	    trj->DrawTrajectory();   // changed from:  trj->DrawTrajectory(50);
	  else if ((drawFlag == "neutral") && (trj->GetCharge() == 0.))
	    trj->DrawTrajectory();   // changed from:  trj->DrawTrajectory(50);
	  else if ((drawFlag == "muons") && 
		   ((trj->GetParticleName() == "mu-") ||
		    (trj->GetParticleName() == "mu+")))
	    trj->DrawTrajectory();  // changed from:  trj->DrawTrajectory(50);
	  else if ((drawFlag == "no_e") &&
		   (!(trj->GetParticleName() == "e-") &&
		    !(trj->GetParticleName() == "e+")))
	    trj->DrawTrajectory();  // changed from:  trj->DrawTrajectory(50);
	  else if ((drawFlag == "no_e&gamma") &&
		   (!(trj->GetParticleName() == "e-") &&
		    !(trj->GetParticleName() == "e+") &&
		    !(trj->GetParticleName() == "gamma")))
	    trj->DrawTrajectory();   // changed from:  trj->DrawTrajectory(50);
	}  // end for
    }  // end if visManager  

  // Stop the event timer
  evtTimer->Stop();

  G4cout << " The elapsed user time of this event (in seconds) : " << evtTimer->GetUserElapsed() << G4endl;
}

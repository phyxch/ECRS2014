// 6/20/2014, Hexc, Olesya, Uodated the primary generator code
//            to reflect the spherical shape of the universe (i.e., world)
// 9/10/2014, Hexc, Olesya: Add more command line parameters for defining the
//   direction of the primary particles
// 12/1/2014, Hexc, Olesya: Fixed a bug in setting the starting position
// 3/11/2015, Hexc, Olesya: Add code to store primary particle info into a ntuple
//
#include "ECRSPrimaryGeneratorAction.hh"
#include "ECRSPrimaryGeneratorMessenger.hh"
#include "ECRSDetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "g4root.hh"

using namespace CLHEP;

ECRSPrimaryGeneratorAction::ECRSPrimaryGeneratorAction(ECRSDetectorConstruction* det,
						       ECRSRunAction* run_action_in, 
						       ECRSEventAction* evt_action_in)
  :detector(det),run_action(run_action_in),evt_action(evt_action_in),
   rndEFlag("off"),KE(10.0*GeV),lowKE(50*MeV),highKE(10*GeV),
   rndPosFlag("off"),rndAngFlag("off"),cosmicRay("off"),RigiVal(1.0)
{
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);

  gunMessenger = new ECRSPrimaryGeneratorMessenger(this);
  // We pick the zinith direction relative to Atlanta
  // Latitude: 33.75 degree (north) 
  // Longgitude: 84.39 degree (west) 

  //  G4double r_sin_theta = sin(33.75*pi/180.);
  G4double r_sin_theta = sin(90.*pi/180.);

  //  G4double r_cos_theta = cos(33.75*pi/180.);
  G4double r_cos_theta = cos(90.*pi/180.);

  //  G4double r_cos_phi = cos(84.38*pi/180.);
  G4double r_cos_phi = cos(0.*pi/180.);

  //  G4double r_sin_phi = sin(84.38*pi/180.);
  G4double r_sin_phi = sin(0.*pi/180.);

  G4double uRadius = 0.4*detector->GetUniverseSize();   // This is the universe radius
  pos = G4ThreeVector(uRadius*r_cos_theta*r_cos_phi, uRadius*r_cos_theta*r_sin_phi, uRadius*r_sin_theta);
  particleGun->SetParticleDefinition(G4Proton::ProtonDefinition());
  particleGun->SetParticleEnergy(KE);
  particleGun->SetParticlePosition(pos);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(-r_cos_theta*r_cos_theta*r_cos_phi*r_cos_phi,
							  -r_cos_theta*r_cos_theta*r_sin_phi*r_sin_phi,
							  -r_sin_theta*r_sin_theta));
}

ECRSPrimaryGeneratorAction::~ECRSPrimaryGeneratorAction()
{
  delete particleGun;
  delete gunMessenger;
}

void ECRSPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  
  if (rndEFlag == "on")
    {
      if (highKE<lowKE)
	{
	  G4cout << "\nHighKE is less than LowKE!!\n";
	  G4RunManager::GetRunManager()->AbortRun();
	}
      KE = lowKE + (highKE - lowKE)*G4UniformRand();
      particleGun->SetParticleEnergy(KE);
    }
  
  // "rndPosFlag = on" launches the primary particle randomly from a sphere
  // around the planet
  if (rndPosFlag == "on")
    {
      G4double x, y, z;
      G4double px, py, pz;
      G4double x0 = 0.99*detector->GetUniverseSize();
      G4double z0 = 0.99*detector->GetUniverseSize();
      G4ThreeVector p;
      G4ThreeVector n, norm;
      G4int test = 1;
      while (test)
	{
	  x = (2*x0*G4UniformRand() - x0);
	  y = (2*x0*G4UniformRand() - x0);
	  z = 0;
	  if ((x*x + y*y) <= (x0*x0))
	    {
	      z = -z0 + sqrt(x0*x0 - x*x - y*y);
	      test = 0;
	    }
	}
      pos = G4ThreeVector(x,y,z);
      norm = G4ThreeVector(-x,-y,-(z+z0));
      n = norm/norm.mag();
      
      // rndAngFlag = on will launch the particle from the sphere at a random
      // angle from the inward normal on the sphere
      if (rndAngFlag == "on")
	{
	  G4double scalar = 0.;
	  G4double p_dot_n = 0;
	  G4double r = detector->GetAtmHeight() + detector->GetECRSRadius();
	  G4double costheta = sqrt(x0*x0 - r*r)/x0;
	  // The following while block requires that the primary momentum be in
	  // a direction that will enter the atmosphere
	  while (!(scalar > costheta))
	    {        
	      px = 2*G4UniformRand() - 1;
	      py = 2*G4UniformRand() - 1;
	      pz = 2*G4UniformRand() - 1;
	      p = G4ThreeVector(px,py,pz);
	      p_dot_n = n.getX()*px + n.getY()*py + n.getZ()*pz;
	      scalar = p_dot_n/(p.mag()*n.mag());
	    }
	}
      // rndAngFlag = off will launch the particle toward the center of the ECRS
      else
	{
	  p = norm;
	}
      particleGun->SetParticleEnergy(KE);
      particleGun->SetParticlePosition(pos);      
      particleGun->SetParticleMomentumDirection(p);
    }
  
  if (cosmicRay == "on") 
    {
      G4ParticleDefinition* particle
	= particleTable->FindParticle(particleName="proton");
      particleGun->SetParticleDefinition(particle);

      NormConst = ((18./17.)*pow(RigiVal*GeV,-1.7)) - ((18./17.)*pow(100.0*GeV,-1.7));
      //fileOut->fout <<"***************************Rigi Val & Norm Cosnt " <<RigiVal << "  "<< NormConst << G4endl;
      
      while(1) {
	KE = RigiVal*GeV + ((100.0*GeV-RigiVal*GeV) * G4UniformRand());
	if (probProtonKE(KE,NormConst) > G4UniformRand()) break;
	// G4cout << "KE: " << G4BestUnit(KE,"Energy") << G4endl;
      }

      particleGun->SetParticleEnergy(KE);
      
    } 
  
  /*
  if (cosmicRay == "off") 
    {
      G4ParticleDefinition* particle
	= particleTable->FindParticle(particleName="mu-");
      particleGun->SetParticleDefinition(particle);
      particleGun->SetParticleMomentum(5.*GeV);
      
    }
  */

  G4cout << "\nPrimary particle KE = " << G4BestUnit(KE,"Energy") << "   Cutoff energy (GeV): " << RigiVal << G4endl;
  G4cout << "Starting position: " << G4BestUnit(pos,"Length") << G4endl;

  // find and set the primary particle position and momentum 
  G4int primID = particleGun->GetParticleDefinition()->GetPDGEncoding();
  G4double primP_mass = particleGun->GetParticleDefinition()->GetPDGMass();
  G4double eP =  particleGun->GetParticleEnergy();
  
  //G4ThreeVector primP_pos = particleGun->GetParticlePosition();
  G4ThreeVector primP_mom =  sqrt(eP*eP - primP_mass*primP_mass) * particleGun->GetParticleMomentumDirection();
  
  G4double xP = pos.getX();
  G4double yP = pos.getY();
  G4double zP = pos.getZ();
  
  G4double pxP = primP_mom.getX();
  G4double pyP = primP_mom.getY();
  G4double pzP = primP_mom.getZ();

  // find and set event ID
  G4int evtID = anEvent->GetEventID();

  // get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  // fill ntuple (ntup[le ID = 3)
  analysisManager->FillNtupleDColumn(3, run_action->GetNtColID(23), primID);
  analysisManager->FillNtupleDColumn(3, run_action->GetNtColID(24), eP/MeV);
  analysisManager->FillNtupleDColumn(3, run_action->GetNtColID(25), xP/m);
  analysisManager->FillNtupleDColumn(3, run_action->GetNtColID(26), yP/m);
  analysisManager->FillNtupleDColumn(3, run_action->GetNtColID(27), zP/m);
  analysisManager->FillNtupleDColumn(3, run_action->GetNtColID(28), pxP/MeV);
  analysisManager->FillNtupleDColumn(3, run_action->GetNtColID(29), pyP/MeV);
  analysisManager->FillNtupleDColumn(3, run_action->GetNtColID(30), pzP/MeV);
  analysisManager->FillNtupleDColumn(3, run_action->GetNtColID(31), evtID);

  analysisManager->AddNtupleRow(3);
  
  particleGun->GeneratePrimaryVertex(anEvent);
}

void ECRSPrimaryGeneratorAction::SetPosition(G4double Alt, G4double Long, G4double Lat)
{

  G4double x,y,z;
  G4double earthRadius = 6.3712e6*m;
  G4ThreeVector p;
  x = (earthRadius + Alt)*cos(Lat)*cos(Long);
  y = (earthRadius + Alt)*cos(Lat)*sin(Long);
  z = (earthRadius + Alt)*sin(Lat);
  G4cout << "  Lat = " << Lat << "  Long = " << Long << G4endl;
  G4cout << "x = " << G4BestUnit(x,"Length") <<",  y = "<< G4BestUnit(y,"Length") <<",  z = "<< G4BestUnit(z,"Length")<< G4endl;
  pos = G4ThreeVector(x,y,z);
  p   =  G4ThreeVector(-x,-y,-z);
  
  G4cout << "\nPrimary particle KE = " << G4BestUnit(KE,"Energy") << G4endl;
  G4cout << "Starting position: " << G4BestUnit(pos,"Length") << G4endl;
  particleGun->SetParticlePosition(pos);
  particleGun->SetParticleEnergy(KE);
  particleGun->SetParticleMomentumDirection(p);
}


G4double ECRSPrimaryGeneratorAction::probProtonKE(G4double energy, G4double NormCnt)
{
  // We used the primary proton KE distribution from PDG for selecting proton KE
  // The constant, 1.0584, was determined from a normalization.  This is only valid for choosing KE in a
  // range from 1 to 100 GeV.  For higher KE, one has to determine this constant again.  See the code
  // "primaryProtonDist.C".

  //I modified the code such that now it calculates the normalization constant for the given rigitity cut off value. 
  //The default rigitity value is 1.0 GeV. 
  //But still this is only valid for choosing KE in the range from given rigitity value to 100 GeV.
  //Kanishka 10/19/12.
  
  return 1.8*pow(energy, -2.7) / NormCnt;
}

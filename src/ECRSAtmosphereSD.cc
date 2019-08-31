// Cleaning up.  May 25, 2009, hexc
//
// Update Date: Hexc, Olesya, 6/19/2014: Making major updates for refining the ATM layer definition. 
//
#include "ECRSAtmosphereSD.hh"

#include "ECRSAtmoHit.hh"
#include "ECRSDetectorConstruction.hh"

#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"

#include "G4ios.hh"

ECRSAtmosphereSD::ECRSAtmosphereSD(G4String name,
				   ECRSDetectorConstruction* det)
  :G4VSensitiveDetector(name),AtmoCollection(NULL),Detector(det)
{
  collectionName.insert("AtmoCollection");
  HitID = new G4int[NUMBER_LAYER];   // Define HitID for each atm layer
}

ECRSAtmosphereSD::~ECRSAtmosphereSD()
{
  delete [] HitID;
}

void ECRSAtmosphereSD::Initialize(G4HCofThisEvent* HCE)
{
  AtmoCollection = new AtmoHitsCollection
    (SensitiveDetectorName,collectionName[0]);
  for (G4int j=0; j<NUMBER_LAYER; j++) {HitID[j] = -1;};
}

G4bool ECRSAtmosphereSD::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
  
  G4double edep = aStep->GetTotalEnergyDeposit();
  
  G4double stepl = 0.;
 
  // Need to check this statement for tracking charge-neutral particles !!!  5/25/2009, hexc
  // We still need to deal this part at a later time.  6/19/2014 Hexc, Olesya
  if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.)
    stepl = aStep->GetStepLength();
  
  if ((edep==0.)&&(stepl==0.)) return false;
  
  G4TouchableHistory* theTouchable =
    (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
  
  G4VPhysicalVolume* physVol = theTouchable->GetVolume();

  // Extract the layer number from the volume name
  G4int iLayer;
  G4String LayerName;

  LayerName = physVol->GetName();
  //G4cout << " ******* LayerName " << LayerName;

  iLayer = atoi((LayerName.substr(5)).c_str());   // The layer name is like:  Layer02
  //G4cout << "   Layer number " << iLayer << G4endl;

  if (HitID[iLayer]==-1)
    {
      ECRSAtmoHit* atmoHit = new ECRSAtmoHit();
      atmoHit->SetCurrentLayerNumber(iLayer);
      atmoHit->AddAtmHits(iLayer, edep, stepl);

      HitID[iLayer] = AtmoCollection->insert(atmoHit) - 1;

      if (verboseLevel>0)
	G4cout << " New Hit on Atmosphere Layer: " << iLayer+1 << G4endl;
    }
  else
    {
      (*AtmoCollection)[HitID[iLayer]]->AddAtmHits(iLayer, edep, stepl);

      if (verboseLevel>0)
	G4cout << " Energy added to Layer: " << iLayer+1 << G4endl;
    }
  
  return true;
}

void ECRSAtmosphereSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  static G4int HCID = -1;
  if (HCID<0)
    { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection(HCID,AtmoCollection);
}

void ECRSAtmosphereSD::clear()
{
  delete AtmoCollection;
}

void ECRSAtmosphereSD::DrawAll()
{}

void ECRSAtmosphereSD::PrintAll()
{}

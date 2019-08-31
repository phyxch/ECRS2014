// 6/18/2014: Hexc, Olesya: Making major upgrade of the detector contruction by using 
// template vectors for describing atm layers.
//
#ifndef ECRSDetectorConstruction_H
#define ECRSDetectorConstruction_H 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "ECRSLayerNumber.hh"

// #define NUMBER_LAYER 99

//class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Sphere;
class ECRSDetectorMessenger;
class ECRSAtmosphereSD;
class G4VisAttributes;
class ECRSMagneticField;

class ECRSDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  G4double mult;          // used for varying overall dimensions
  G4double multDensity;   // used for varying atmospheric densities
  G4bool visibility;      // used for visualization of the atmosphere
  G4bool externalMag;
  
public:
  ECRSDetectorConstruction();
  ~ECRSDetectorConstruction();
  G4VPhysicalVolume* Construct();
  G4VPhysicalVolume* ConstructUniverse();
  void PrintUniverseParameters();
  void DefineMaterials();
  void SetMultiplier(G4double);
  void SetDensityMult(G4double);
  void SetVisFlag(G4bool);
  void UpdateGeometry();
  void UpdateMaterials();
  void PrintAllParameters();
  void SetExtMag(G4bool newValue)
  // void SetExtMag (G4String newValue)
  {externalMag = newValue;};
  
  G4double GetAtmHeight()	{return upperATMHeight;};
  G4double GetECRSRadius()	{return earthRadius;};
  G4double GetUniverseSize()	{return universeSize;};

  G4int GetNbOfLayers()         {return NbOfLayers;}; 

  G4double GetAirDensity( G4double,G4int);
  
  const G4VPhysicalVolume*    GetECRS()      {return ECRS_phys;};
  
private:
  // Set detector sizes
  G4double universeSize;  // variables for defining world volume size

  G4double earthRadius;   // radius of the ECRS
  G4double oceanDepth;    // the depth of water on earth
  //G4double atmHeight[38];
  G4double upperATMHeight, airLayerThickness;
  G4int atmType;  // control parameter for selecting different atm models
  G4double denPer;

  G4Sphere*		solidUniverse;
  G4LogicalVolume*	logicUniverse;
  G4VPhysicalVolume*	physiUniverse;
  
  G4Sphere*		ECRS_sphere;
  G4LogicalVolume*	ECRS_log;
  G4VPhysicalVolume*	ECRS_phys;
  
  G4int    NbOfLayers;
  
  std::vector<G4Material*> airLayer_Matt;
  std::vector<G4VPhysicalVolume*> airLayer_phys;
  std::vector<G4LogicalVolume*> airLayer_logic;
  std::vector<G4Sphere*> airLayer;
  std::vector<G4double> LayerHeight;
  std::vector<G4double> LayerDensity;  

  // Other parameters
  G4double density, temperature, pressure, fractionmass;
  G4int ncomponents, natoms;
  G4String name, symbol;
    
  // Other Materials
  G4Material* Si;         // ECRS is currently made of Si
  G4Material* space;      // pointer to space material
  G4Material* earth;
  
  G4Element  *elN, *elO, *elAr, *elC;
  G4Material *N2, *O2, *Ar, *CO2, *H2O;
  
  ECRSDetectorMessenger*  ECRSDetector;		           
        
  ECRSAtmosphereSD*	atmosphereSD;
  ECRSMagneticField* theMagneticField;
  
};

#endif


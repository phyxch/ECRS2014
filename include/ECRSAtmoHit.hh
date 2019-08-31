// Cleaning up and reformatting, May 25, 2009, hexc
//
// Update Date: Hexc, Olesya, 6/19/2014: Making major updates for refining the ATM layer definition. 
//
#ifndef ECRSAtmoHit_h
#define ECRSAtmoHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

#include "ECRSLayerNumber.hh"

class ECRSAtmoHit : public G4VHit
{
public:
  ECRSAtmoHit();
  ~ECRSAtmoHit();
  ECRSAtmoHit(const ECRSAtmoHit&);
  const ECRSAtmoHit& operator=(const ECRSAtmoHit&);
  int operator==(const ECRSAtmoHit&) const;
  
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  
  void Draw();
  void Print();
  
public:
  
  void AddAtmHits(G4int nLayer, G4double de, G4double dl)
  {EdepAtmLayer[nLayer] += de; TrackLengthAtmLayer[nLayer] += dl;};
  
  G4double GetEdepAtmLayer(G4int nLayer)	{return EdepAtmLayer[nLayer];};
  
  G4double GetTrackLengthAtmLayer(G4int nLayer)	{return TrackLengthAtmLayer[nLayer];};

  void SetCurrentLayerNumber(G4int n) {currentLayerNumberOfHits = n;}
  G4int GetCurrentLayerNumber() {return currentLayerNumberOfHits;}
  
private:
  G4int currentLayerNumberOfHits;
  G4double EdepAtmLayer[NUMBER_LAYER], TrackLengthAtmLayer[NUMBER_LAYER];
  
};

typedef G4THitsCollection<ECRSAtmoHit> AtmoHitsCollection;

extern G4Allocator<ECRSAtmoHit> ECRSAtmoHitAllocator;

inline void* ECRSAtmoHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*) ECRSAtmoHitAllocator.MallocSingle();
  return aHit;
}

inline void ECRSAtmoHit::operator delete(void* aHit)
{
  ECRSAtmoHitAllocator.FreeSingle((ECRSAtmoHit*) aHit);
}

#endif

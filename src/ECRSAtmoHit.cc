// Update Date: Hexc, Olesya, 6/19/2014: Making major updates for refining the ATM layer definition. 
//
#include "ECRSAtmoHit.hh"

G4Allocator<ECRSAtmoHit> ECRSAtmoHitAllocator;

ECRSAtmoHit::ECRSAtmoHit()
{
  // Initialize all hits to zero
  for (G4int i = 0; i<NUMBER_LAYER; i++)
    {
      EdepAtmLayer[i] = 0.;
      TrackLengthAtmLayer[i] = 0.;
    }
  currentLayerNumberOfHits = 0;      // Initialize it to the top layer
}

ECRSAtmoHit::~ECRSAtmoHit()
{}

ECRSAtmoHit::ECRSAtmoHit(const ECRSAtmoHit& right)
{

  for (G4int i = 0; i<NUMBER_LAYER; i++)
    {
      EdepAtmLayer[i] = right.EdepAtmLayer[i];
      TrackLengthAtmLayer[i] = right.TrackLengthAtmLayer[i];
    }

}

const ECRSAtmoHit& ECRSAtmoHit::operator=(const ECRSAtmoHit& right)
{

  for (G4int i = 0; i<NUMBER_LAYER; i++)
    {
      EdepAtmLayer[i] = right.EdepAtmLayer[i];
      TrackLengthAtmLayer[i] = right.TrackLengthAtmLayer[i];
    }

  return *this;
}

int ECRSAtmoHit::operator==(const ECRSAtmoHit& right) const
{
  return 0;
}

void ECRSAtmoHit::Draw()
{}

void ECRSAtmoHit::Print()
{}

#ifndef ECRSECRSHit_h
#define ECRSECRSHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class ECRSECRSHit : public G4VHit
{
  public:
    ECRSECRSHit();
    ~ECRSECRSHit();
    ECRSECRSHit(const ECRSECRSHit&);
    const ECRSECRSHit& operator=(const ECRSECRSHit&);
    int operator==(const ECRSECRSHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    void Draw();
    void Print();

  public:
    void AddECRS(G4double de, G4double dl)
      {EdepECRS += de; TrackLengthECRS += dl;};

    G4double GetEdepECRS()	{return EdepECRS;};

    G4double GetTrakECRS()	{return TrackLengthECRS;};

  private:
    G4double EdepECRS, TrackLengthECRS;
};

typedef G4THitsCollection<ECRSECRSHit> ECRSHitsCollection;

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

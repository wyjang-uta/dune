#ifndef LArHit_h
#define LArHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

/// LAr hit class
///
/// It defines data members to store the energy deposit and track lengths
/// of charged particles in a selected volume:
/// - fEdep, fTrackLength

class LArHit : public G4VHit
{
  public:
    LArHit();
    LArHit(const LArHit&);
    virtual ~LArHit();

    // operators
    const LArHit& operator=(const LArHit&);
    G4bool operator==(const LArHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print();

    // methods to handle data
    void Add(G4double de, G4double dl);

    // get methods
    G4double GetEdep() const;
    G4double GetTrackLength() const;

  private:
    G4double fEdep;           ///< Energy deposit in the sensitive volume
    G4double fTrackLength;    ///< Track length in the sensitive volume
};

using LArHitsCollection = G4THitsCollection<LArHit>;

extern G4ThreadLocal G4Allocator<LArHit>* LArHitAllocator;

inline void* LArHit::operator new(size_t)
{
  if( !LArHitAllocator) {
    LArHitAllocator = new G4Allocator<LArHit>;
  }
  void *hit;
  hit = (void *)LArHitAllocator->MallocSingle();
  return hit;
}

inline void LArHit::operator delete(void *hit)
{
  if( !LArHitAllocator ) {
    LArHitAllocator = new G4Allocator<LArHit>;
  }
  LArHitAllocator->FreeSingle((LArHit*) hit);
}

inline void LArHit::Add(G4double de, G4double dl)
{
  fEdep += de;
  fTrackLength += dl;
}

inline G4double LArHit::GetEdep() const
{
  return fEdep;
}

inline G4double LArHit::GetTrackLength() const
{
  return fTrackLength;
}

#endif

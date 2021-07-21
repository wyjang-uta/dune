#ifndef TAMUALP_CalorHit_h
#define TAMUALP_CalorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

/// Calorimeter hit class
/// It defines data members to store the the energy deposit and track lengths
/// of charged particles in a selected volume:
/// - fEdep, fTrackLength

class TAMUALP_CalorHit : public G4VHit
{
  public:
    TAMUALP_CalorHit();
    TAMUALP_CalorHit(const TAMUALP_CalorHit&);
    virtual ~TAMUALP_CalorHit();

    // operators
    const TAMUALP_CalorHit& operator=(const TAMUALP_CalorHit&);
    G4bool operator==(const TAMUALP_CalorHit&) const;

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
    G4double fEdep;         ///< Energy deposit in the sensitive volume
    G4double fTrackLength;  ///< Track length in the sensitive volume
};

using TAMUALP_CalorHitsCollection = G4THitsCollection<TAMUALP_CalorHit>;

extern G4ThreadLocal G4Allocator<TAMUALP_CalorHit>* TAMUALP_CalorHitAllocator;

inline void* TAMUALP_CalorHit::operator new(size_t) {
  if(!TAMUALP_CalorHitAllocator) {
    TAMUALP_CalorHitAllocator = new G4Allocator<TAMUALP_CalorHit>;
  }
  void *hit;
  hit = (void *) TAMUALP_CalorHitAllocator->MallocSingle();
  return hit;
}

inline void TAMUALP_CalorHit::operator delete(void *hit)
{
  if (!TAMUALP_CalorHitAllocator) {
    TAMUALP_CalorHitAllocator = new G4Allocator<TAMUALP_CalorHit>;
  }
  TAMUALP_CalorHitAllocator->FreeSingle((TAMUALP_CalorHit*) hit);
}

inline void TAMUALP_CalorHit::Add(G4double de, G4double dl) {
  fEdep += de;
  fTrackLength += dl;
}

inline G4double TAMUALP_CalorHit::GetEdep() const {
  return fEdep;
}

inline G4double TAMUALP_CalorHit::GetTrackLength() const {
  return fTrackLength;
}

#endif

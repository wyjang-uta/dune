#include "TAMUALP_CalorHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<TAMUALP_CalorHit>* TAMUALP_CalorHitAllocator = 0;

TAMUALP_CalorHit::TAMUALP_CalorHit()
  : G4VHit(),
  fEdep(0.),
  fTrackLength(0.)
{}

TAMUALP_CalorHit::~TAMUALP_CalorHit()
{}

TAMUALP_CalorHit::TAMUALP_CalorHit(const TAMUALP_CalorHit& right)
  : G4VHit()
{
  fEdep         = right.fEdep;
  fTrackLength  = right.fTrackLength;
}

const TAMUALP_CalorHit& TAMUALP_CalorHit::operator=(const TAMUALP_CalorHit& right)
{
  fEdep         = right.fEdep;
  fTrackLength  = right.fTrackLength;

  return *this;
}

G4bool TAMUALP_CalorHit::operator==(const TAMUALP_CalorHit& right) const
{
  return ( this == &right ) ? true : false;
}

void TAMUALP_CalorHit::Print()
{
  G4cout
    << "Edep: "
    << std::setw(7) << G4BestUnit(fEdep, "Energy")
    << " track length: "
    << std::setw(7) << G4BestUnit(fTrackLength, "Length")
    << G4endl;
}

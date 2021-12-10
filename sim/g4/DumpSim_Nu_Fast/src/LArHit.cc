#include "LArHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<LArHit>* LArHitAllocator = 0;

LArHit::LArHit()
  : G4VHit(),
  fEdep(0.),
  fTrackLength(0.)
{}

LArHit::~LArHit()
{}

LArHit::LArHit(const LArHit& right)
  : G4VHit()
{
  fEdep = right.fEdep;
  fTrackLength = right.fTrackLength;
}

const LArHit& LArHit::operator=(const LArHit& right)
{
  fEdep = right.fEdep;
  fTrackLength = right.fTrackLength;

  return *this;
}

G4bool LArHit::operator==(const LArHit& right) const
{
  return ( this == &right ) ? true : false;
}

void LArHit::Print()
{
  G4cout
    << "Edep: "
    << std::setw(7) << G4BestUnit( fEdep, "Energy")
    << " track length: "
    << std::setw(7) << G4BestUnit( fTrackLength, "Length")
    << G4endl;
}

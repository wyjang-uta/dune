#include "TAMUALP_CalorimeterSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

TAMUALP_CalorimeterSD::TAMUALP_CalorimeterSD(
    const G4String& name,
    const G4String& hitsCollectionName,
    G4int nofCells)
  : G4VSensitiveDetector(name),
  fHitsCollection(nullptr),
  fNofCells(nofCells)
{
  collectionName.insert(hitsCollectionName);
}

TAMUALP_CalorimeterSD::~TAMUALP_CalorimeterSD()
{
}

void TAMUALP_CalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection
    = new TAMUALP_CalorHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  auto hcID
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection );

  // Create hits
  // fNofCells for cells + one more for total sums
  for ( G4int i = 0; i < fNofCells+1; i++ ) {
    fHitsCollection->insert(new TAMUALP_CalorHit());
  }
}

G4bool TAMUALP_CalorimeterSD::ProcessHits(G4Step* step,
                                          G4TouchableHistory*)
{
  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();

  // step length
  G4double stepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    stepLength = step->GetStepLength();
  }

  if ( edep == 0. && stepLength == 0. ) return false;

  auto touchable = (step->GetPreStepPoint()->GetTouchable());

  // Get calorimeter cell id
  auto layerNumber = touchable->GetReplicaNumber(1);

  // Get hit accounting data for this cell
  auto hit = (*fHitsCollection)[layerNumber];
  if( ! hit ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hit " << layerNumber;
    G4Exception("TAMUALP_CalorimeterSD::ProcessHits()",
        "MyCode0004", FatalException, msg);
  }

  // Get hit for total accounting
  auto hitTotal = (*fHitsCollection)[fHitsCollection->entries()-1];

  // Add values
  hit->Add(edep, stepLength);
  hitTotal->Add(edep, stepLength);

  return true;
}

void TAMUALP_CalorimeterSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel > 1 ) {
    auto nofHits = fHitsCollection->entries();
    G4cout
      << G4endl
      << "-------->Hits Collection: in this event they are " << nofHits
      << " hits in the tracking chambers: " << G4endl;
    for ( std::size_t i = 0; i < nofHits; ++i ) (*fHitsCollection)[i]->Print();
  }
}

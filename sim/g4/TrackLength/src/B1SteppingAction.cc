//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleTypes.hh"
#include "g4root.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction* )
: G4UserSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track* track = step->GetTrack();

  /*
  G4cout <<
    track->GetTrackID() << " " <<
    track->GetParentID() << " " <<
    track->GetParticleDefinition()->GetParticleName() << " " <<
    track->GetGlobalTime() << " " <<
    track->GetVolume()->GetName() << " " <<
    track->GetKineticEnergy() << " " <<
    track->GetTrackStatus() << " " <<
    track->GetTrackLength() // unit is mm
    << G4endl;
    */

  auto analysisManager = G4AnalysisManager::Instance();
  G4int trackStatus = track->GetTrackStatus();
  if( trackStatus == 2 )
  {
    analysisManager->FillNtupleSColumn(0, track->GetParticleDefinition()->GetParticleName() );
    analysisManager->FillNtupleDColumn(1, (G4double)track->GetTrackID() );
    analysisManager->FillNtupleDColumn(2, (G4double)track->GetKineticEnergy()/CLHEP::GeV );
    analysisManager->FillNtupleDColumn(3, (G4double)track->GetTrackLength()/CLHEP::cm );      // the default unit is mm.
    analysisManager->FillNtupleDColumn(4, (G4double)track->
    analysisManager->AddNtupleRow();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


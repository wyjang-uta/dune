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
/// \file TargetSim_SteppingAction.cc
/// \brief Implementation of the TargetSim_SteppingAction class

#include "TargetSim_SteppingAction.hh"
#include "TargetSim_EventAction.hh"
#include "TargetSim_DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleTypes.hh"
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TargetSim_SteppingAction::TargetSim_SteppingAction(TargetSim_EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fWorldVolume(0),
  fTargetVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TargetSim_SteppingAction::~TargetSim_SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TargetSim_SteppingAction::UserSteppingAction(const G4Step* step)
{
  auto analysisManager = G4AnalysisManager::Instance();

  G4String particleName;
  G4int pdgEncoding;

  const std::vector<const G4Track*>* secondary = step->GetSecondaryInCurrentStep();
  for( size_t lp=0; lp < (*secondary).size(); lp++ )
  {
    particleName = (*secondary)[lp]->GetDefinition()->GetParticleName();
    pdgEncoding = (*secondary)[lp]->GetDefinition()->GetPDGEncoding();

    // choose only neutrinos (nu_e and nu_mu or their antiparticles)
    if( abs(pdgEncoding) == 12 || abs(pdgEncoding) == 14 )
    {
      analysisManager->FillNtupleSColumn(0, (*secondary)[lp]->GetCreatorProcess()->GetProcessName());
      analysisManager->FillNtupleDColumn(1, (G4double)(*secondary)[lp]->GetKineticEnergy()/CLHEP::GeV);
      analysisManager->FillNtupleDColumn(2, (G4double)(*secondary)[lp]->GetTotalEnergy()/CLHEP::GeV);
      analysisManager->FillNtupleDColumn(3, (G4double)(*secondary)[lp]->GetPosition().getX()/CLHEP::cm);
      analysisManager->FillNtupleDColumn(4, (G4double)(*secondary)[lp]->GetPosition().getY()/CLHEP::cm);
      analysisManager->FillNtupleDColumn(5, (G4double)(*secondary)[lp]->GetPosition().getZ()/CLHEP::cm);
      analysisManager->FillNtupleDColumn(6, (G4double)(*secondary)[lp]->GetMomentum().getX()/CLHEP::GeV);
      analysisManager->FillNtupleDColumn(7, (G4double)(*secondary)[lp]->GetMomentum().getY()/CLHEP::GeV);
      analysisManager->FillNtupleDColumn(8, (G4double)(*secondary)[lp]->GetMomentum().getZ()/CLHEP::GeV);
      analysisManager->FillNtupleDColumn(9, (G4double)(*secondary)[lp]->GetMomentumDirection().getX());
      analysisManager->FillNtupleDColumn(10, (G4double)(*secondary)[lp]->GetMomentumDirection().getY());
      analysisManager->FillNtupleDColumn(11, (G4double)(*secondary)[lp]->GetMomentumDirection().getZ());
      analysisManager->FillNtupleSColumn(12, (*secondary)[lp]->GetDefinition()->GetParticleName() );
      analysisManager->FillNtupleSColumn(13, step->GetTrack()->GetParticleDefinition()->GetParticleName());
      analysisManager->AddNtupleRow();
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


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
/// \file TargetSim_DetectorConstruction.cc
/// \brief Implementation of the TargetSim_DetectorConstruction class

#include <cmath>

#include "TargetSim_DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4FieldManager.hh"
#include "G4MagneticField.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4UniformMagField.hh"
#include "G4SDManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//TargetSim_DetectorConstruction::TargetSim_DetectorConstruction(G4double input_radius, G4double world_scale)
TargetSim_DetectorConstruction::TargetSim_DetectorConstruction()
: G4VUserDetectorConstruction()//,
  /*
  tgt_radius(input_radius),
  wld_scale(world_scale)
  */
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TargetSim_DetectorConstruction::~TargetSim_DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* TargetSim_DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Parameters of the target geometry
  //	- a cylindrical target with radius of 0.85 cm and 1.5 m length
  //	  composed by graphite material.
  //G4double radius = tgt_radius;
  G4double radius = 0.85*cm;
  G4cout << "building geometry with radius: " << tgt_radius/CLHEP::cm << " cm." << G4endl;
  G4double length = 1.5 * m;
  G4Material* target_mat = nist->FindOrBuildMaterial("G4_GRAPHITE");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  //G4double world_sizeXY = (2.*radius)*wld_scale;
  //G4double world_sizeZ  = length*wld_scale;
  G4double world_sizeXY = 2.0*radius;
  G4double world_sizeZ  = length;
  G4cout << "World size in XY = " << world_sizeXY/CLHEP::cm << " cm" << G4endl;
  G4cout << "World size in Z  = " << world_sizeZ/CLHEP::cm  << " cm" << G4endl;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  G4Box* solidWorld =
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size

  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  //
  // Graphite target
  //
  G4Tubs* target =
    new G4Tubs("target",
        0,              // inner radius
        radius,     	// outer radius
        0.5*length,  	// z half-length
        0,
        2.*M_PI);

  G4LogicalVolume* logicTarget =
    new G4LogicalVolume(target,            //its solid
                        target_mat,        //its material
                        "Target");         //its name

  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicTarget,             //its logical volume
                    "Target",                //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  //
  //always return the physical World
  //
  return physWorld;
}

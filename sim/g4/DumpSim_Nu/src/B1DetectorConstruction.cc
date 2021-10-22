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
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

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

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Elements
  //G4Element* elementH  = new G4Element("Hydrogen",  "H",  1.,  1.0079*g/mole);
  //G4Element* elementC  = new G4Element("Carbon",    "C",  6.,  12.011*g/mole);
  //G4Element* elementN  = new G4Element("Nitrogen",  "N",  7.,  14.007*g/mole);
  //G4Element* elementO  = new G4Element("Oxygen",    "O",  8., 15.9994*g/mole);
  G4Element* elementMg = new G4Element("Magnesium", "Mg", 12., 24.3050*g/mole);
  G4Element* elementAl = new G4Element("Aluminium", "Al", 13., 26.9815*g/mole);
  G4Element* elementSi = new G4Element("Silicon",   "Si", 14., 28.0855*g/mole);
  //G4Element* elementTi = new G4Element("Titanium",  "Ti",22.,   47.90*g/mole);
  //G4Element* elementV  = new G4Element("Vanadium",  "V", 23., 50.9415*g/mole);
  G4Element* elementFe = new G4Element("Iron",      "Fe", 26.,  55.845*g/mole);
  //G4Element* elementMo = new G4Element("Molybdenum","Mo",42.,   95.94*g/mole);
  //G4Element* elementPt = new G4Element("Platinum",  "Pt",78.,  195.08*g/mole);
  //G4Element* elementAu = new G4Element("Gold",      "Au",79.,  196.97*g/mole);

  // DUNE hadron absorber - aluminum alloy 6061-T6
  G4Material* Al6061 = new G4Material(
      "Al6061",   // its name
      2.70*g/cm3, // its density
      4           // number of component elements
      );
  Al6061->AddElement(elementAl, 0.980);
  Al6061->AddElement(elementMg, 0.010);
  Al6061->AddElement(elementSi, 0.006);
  Al6061->AddElement(elementFe, 0.004);

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 4.0 * m * 1.1;
  G4double world_sizeZ  = 4.0 * m * 1.1;
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
  // Al hadron absorber
  G4double abso_x = 4*m;
  G4double abso_y = 4*m;
  G4double abso_z = 4*m;
  G4Box* absorber =
    new G4Box("absorber", 0.5*abso_x, 0.5*abso_y, 0.5*abso_z);

  G4LogicalVolume* logicDump =
    new G4LogicalVolume(absorber,
        Al6061, "absorber");

  new G4PVPlacement(0,
      G4ThreeVector(),
      logicDump,
      "absorber",
      logicWorld,
      false,
      0,
      checkOverlaps);



  //
  //always return the physical World
  //
  return physWorld;
}


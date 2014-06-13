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
// $Id: B2aDetectorConstruction.cc 77603 2013-11-26 17:11:49Z gcosmo $
//
/// \file B2aDetectorConstruction.cc
/// \brief Implementation of the B2aDetectorConstruction class
 
#include "B2aDetectorConstruction.hh"
#include "B2aDetectorMessenger.hh"
#include "B2TrackerSD.hh"
#include "B2CalorimeterSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4SDManager.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4ThreadLocal 
G4GlobalMagFieldMessenger* B2aDetectorConstruction::fMagFieldMessenger = 0;

B2aDetectorConstruction::B2aDetectorConstruction()
:G4VUserDetectorConstruction(), 
 fNbOfChambers(0),
 fLogicTarget(NULL), fLogicChamber(NULL),
 fLogicCalor(NULL), //CJC 6.6.14
 fTargetMaterial(NULL), fChamberMaterial(NULL), 
 fCalorMaterial(NULL), //CJC 6.6.14
 fStepLimit(NULL),
 fCheckOverlaps(true)
{
  fMessenger = new B2aDetectorMessenger(this);

  fNbOfChambers = 2; // 1 will give hits = 0 for tracker hits CJC
  fLogicChamber = new G4LogicalVolume*[fNbOfChambers];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
B2aDetectorConstruction::~B2aDetectorConstruction()
{
  delete [] fLogicChamber; 
  delete fStepLimit;
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* B2aDetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2aDetectorConstruction::DefineMaterials()
{
  // Material definition 

  G4NistManager* nistManager = G4NistManager::Instance();

  // Air defined using NIST Manager
  nistManager->FindOrBuildMaterial("G4_AIR");

  // Liquid Hydrogen defined for target
  //CJC 6.6.14
  G4double z, a, density;
  G4String name, symbol;
  G4int nComp, nAtom;

  a=1.01*g/mole;
  G4Element* ele_H = new G4Element(name="Hydrogen", symbol="H", z=1., a);

  density = .07085*g/cm3; //density of Liquid Hydrogen
  G4Material* lh2 = new G4Material(name="Liquid Hydrogen", density, nComp=1);

  lh2->AddElement(ele_H, nAtom=2);
  fTargetMaterial = lh2;
  
  //Build Cesium Iodide for calorimeters
  //CJC 6.6.14
  a=132.9*g/mole;
  G4Element* ele_Cs = new G4Element(name="Cesium", symbol="Cs", z=55., a);

  a=126.9*g/mole;
  G4Element* ele_I = new G4Element(name="Iodine", symbol="I", z=53., a);

  density = 4.51*g/cm3;
  G4Material* CsI = new G4Material(name="Cesium Iodide", density, nComp=2);
  CsI->AddElement(ele_Cs, 1);
  CsI->AddElement(ele_I, 1); 

  fCalorMaterial = CsI;

  //Xenon for chambers
  fChamberMaterial = nistManager->FindOrBuildMaterial("G4_Xe");


  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B2aDetectorConstruction::DefineVolumes()
{
  G4Material* air  = G4Material::GetMaterial("G4_AIR");

  // Sizes of the principal geometrical components (solids)
  
  G4double chamberSpacing = 80*cm; // from chamber center to center!

  G4double chamberWidth = 20.0*cm; // width of the chambers
  G4double targetLength =  50.0*cm; // full length of Target
  
  G4double trackerLength = (fNbOfChambers+1)*chamberSpacing;

  G4double worldLength = 1.2 * (2*targetLength + trackerLength);

  G4double targetRadius  = 0.5*targetLength;   // Radius of Target
  targetLength = 0.5*targetLength;             // Half length of the Target  
  G4double trackerSize   = 0.5*trackerLength;  // Half length of the Tracker

  G4double calorSide = 200.0*cm;
  G4double calorDepth = 2.0*cm;

  // Definitions of Solids, Logical Volumes, Physical Volumes

  // World

  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);

  G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
         << " mm" << G4endl;

  G4Box* worldS
    = new G4Box("world",                                    //its name
                worldLength/2,worldLength/2,worldLength/2); //its size
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,   //its solid
                 air,      //its material
                 "World"); //its name
  
  //  Must place the World Physical volume unrotated at (0,0,0).
  // 
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(), // at (0,0,0)
                 worldLV,         // its logical volume
                 "World",         // its name
                 0,               // its mother  volume
                 false,           // no boolean operations
                 0,               // copy number
                 fCheckOverlaps); // checking overlaps 

 

 // Target
  
  G4ThreeVector positionTarget = G4ThreeVector(0,0,-(targetLength+0.5*trackerLength));

  G4Tubs* targetS
    = new G4Tubs("target",0.,targetRadius,targetLength,0.*deg,360.*deg);
  fLogicTarget
    = new G4LogicalVolume(targetS, fTargetMaterial,"Target",0,0,0);
  new G4PVPlacement(0,               // no rotation
                    positionTarget,  // at (x,y,z)
                    fLogicTarget,    // its logical volume
                    "Target",        // its name
                    worldLV,         // its mother volume
                    false,           // no boolean operations
                    0,               // copy number
                    fCheckOverlaps); // checking overlaps 

  G4cout << "Target is " << 2*targetLength/cm << " cm of "
         << fTargetMaterial->GetName() << G4endl;

//Photo Detector (Calorimeter)
  //CJC 6.6.14

  G4ThreeVector positionCalor = G4ThreeVector(0,0,130.0*cm); 

G4Box* calorS = 
  new G4Box("calorimeter", 
	    calorSide/2, calorSide/2, calorDepth/2);

fLogicCalor = 
  new G4LogicalVolume(calorS, 
		      fCalorMaterial, 
		      "Calorimeter");

 new G4PVPlacement(0,
		   positionCalor, 
		   fLogicCalor, 
		   "Calorimeter Physical", 
		   worldLV, 
		   false,
		   0, 
		   fCheckOverlaps);

 G4cout << "The calorimeter is " << calorSide/cm << " cm of "
	<< fCalorMaterial->GetName() << G4endl;
 /*
 //Adding in sensitive detector
//Alternative way provided in the ConstructSDandField method below
 //CJC 6.7.14

 G4VSensitiveDetector* detector = new B2CalorimeterSD("Calorimeter", "CalorimeterHitsCollection");

 // Get pointer to detector manager
 G4SDManager* SDman = G4SDManager::GetSDMpointer();

 // Register detector with manager
 SDman->AddNewDetector(detector);

 // Attach to volume defining calorimeter
 fLogicCalor->SetSensitiveDetector(detector);
 */


  // Tracker

  G4ThreeVector positionTracker = G4ThreeVector(0,0,0);

  G4Tubs* trackerS
    = new G4Tubs("tracker",0,trackerSize,trackerSize, 0.*deg, 360.*deg);
  G4LogicalVolume* trackerLV
    = new G4LogicalVolume(trackerS, air, "Tracker",0,0,0);  
   new G4PVPlacement(0,               // no rotation
                    positionTracker, // at (x,y,z)
                    trackerLV,       // its logical volume
                    "Tracker",       // its name
                    worldLV,         // its mother  volume
                    false,           // no boolean operations
                    0,               // copy number
                    fCheckOverlaps); // checking overlaps 
 

  // Visualization attributes

  G4VisAttributes* boxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  G4VisAttributes* calorVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0)); //CJC 6.6.14

  worldLV      ->SetVisAttributes(boxVisAtt);
  fLogicTarget ->SetVisAttributes(boxVisAtt);
  trackerLV    ->SetVisAttributes(boxVisAtt);
  fLogicCalor  ->SetVisAttributes(calorVisAtt); //CJC 6.6.14


  // Tracker segments

  
  G4double firstPosition = -trackerSize + chamberSpacing;
  G4double firstLength   = trackerLength/10;
  G4double lastLength    = trackerLength;

  G4double halfWidth = 0.5*chamberWidth;
  G4double rmaxFirst = 0.5 * firstLength;
  
  G4double rmaxIncr = 0.0;
  if (fNbOfChambers > 0 ){
    rmaxIncr = 0.5 * (lastLength-firstLength)/(fNbOfChambers-1);
    if (chamberSpacing < chamberWidth) {
      G4Exception("B2aDetectorConstruction::DefineVolume()",
		  "InvalidSetup", FatalException,
		  "Width>Spacing");
    }
    }

  for (G4int copyNo=0; copyNo<fNbOfChambers; copyNo++) {

      G4double Zposition = firstPosition+ copyNo*chamberSpacing;
      G4double rmax =  rmaxFirst+copyNo*rmaxIncr;

      G4Tubs* chamberS
        = new G4Tubs("Chamber_solid", 0, rmax, halfWidth, 0.*deg, 360.*deg);

      fLogicChamber[copyNo] =
              new G4LogicalVolume(chamberS,fChamberMaterial,"Chamber_LV",0,0,0);

      fLogicChamber[copyNo]->SetVisAttributes(chamberVisAtt);

      new G4PVPlacement(0,                            // no rotation
                        G4ThreeVector(0,0,Zposition), // at (x,y,z)
                        fLogicChamber[copyNo],        // its logical volume
                        "Chamber_PV",                 // its name
                        trackerLV,                    // its mother  volume
                        false,                        // no boolean operations
                        copyNo,                       // copy number
                        fCheckOverlaps);              // checking overlaps 
 



  }

  // Example of User Limits
  //
  // Below is an example of how to set tracking constraints in a given
  // logical volume
  //
  // Sets a max step length in the tracker region, with G4StepLimiter

  G4double maxStep = 100.0*cm;
  fStepLimit = new G4UserLimits(maxStep);
  trackerLV->SetUserLimits(fStepLimit);
 
  /// Set additional contraints on the track, with G4UserSpecialCuts
  ///
  /// G4double maxLength = 2*trackerLength, maxTime = 0.1*ns, minEkin = 10*MeV;
  /// trackerLV->SetUserLimits(new G4UserLimits(maxStep,
  ///                                           maxLength,
  ///                                           maxTime,
  ///                                           minEkin));

  // Always return the physical world

  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void B2aDetectorConstruction::ConstructSDandField()
{
  // Sensitive detectors
  
  G4cout << "Placing the SDs" << G4endl;
   G4String calorimeterSDname = "CalorimeterSD";
  B2CalorimeterSD* calorimSD = new B2CalorimeterSD(calorimeterSDname, "CalorimeterHitsCollection");

   SetSensitiveDetector("Calorimeter", calorimSD, true);

  G4cout << "Calorimeter SD........okay! \n" <<
    " Hits Collection is CalorimeterHitsCollection " << G4endl;
  
  

  
  G4String trackerChamberSDname = "B2/TrackerChamberSD";
  B2TrackerSD* aTrackerSD = new B2TrackerSD(trackerChamberSDname,
                                   "TrackerHitsCollection");
  
  // Setting aTrackerSD to all logical volumes with the same name 
  // of "Chamber_LV".
  SetSensitiveDetector("Chamber_LV", aTrackerSD, true);
  

  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue = G4ThreeVector();
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void B2aDetectorConstruction::SetTargetMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();

  G4Material* pttoMaterial = 
              nistManager->FindOrBuildMaterial(materialName);

  if (fTargetMaterial != pttoMaterial) {
     if ( pttoMaterial ) {
        fTargetMaterial = pttoMaterial;
        if (fLogicTarget) fLogicTarget->SetMaterial(fTargetMaterial);
        G4cout << "\n----> The target is made of " << materialName << G4endl;
     } else {
        G4cout << "\n-->  WARNING from SetTargetMaterial : "
               << materialName << " not found" << G4endl;
     }
  }
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2aDetectorConstruction::SetChamberMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();

  G4Material* pttoMaterial =
              nistManager->FindOrBuildMaterial(materialName);

  if (fChamberMaterial != pttoMaterial) {
     if ( pttoMaterial ) {
        fChamberMaterial = pttoMaterial;
	for (G4int copyNo=0; copyNo<fNbOfChambers; copyNo++) {
           if (fLogicChamber[copyNo]) fLogicChamber[copyNo]-> 
                                      SetMaterial(fChamberMaterial);
        }
        G4cout << "\n----> The chamber is made of " << materialName << G4endl;
     } else {
        G4cout << "\n-->  WARNING from SetChamberMaterial : "
               << materialName << " not found" << G4endl;
     }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2aDetectorConstruction::SetCalorMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();

G4Material* pttoMaterial = 
  nistManager->FindOrBuildMaterial(materialName);
 if (fCalorMaterial !=pttoMaterial) {
   if ( pttoMaterial ) {
     fChamberMaterial = pttoMaterial;
     if (fLogicCalor) fLogicCalor->SetMaterial(fCalorMaterial);
   }
   G4cout << "\n----> The calorimeter is made of " << materialName << G4endl;
 } else {
   G4cout << "\n----> WARNING from SetChamberMaterial : " <<
     materialName << " not found " << G4endl;
 }
}
    


void B2aDetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2aDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
  G4cout << "Checking overlaps .... " << G4endl;
  fCheckOverlaps = checkOverlaps;
  G4cout << fCheckOverlaps << G4endl;
}  

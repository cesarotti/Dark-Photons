/*
 * Dark Photon Detector Construction
 * !!!History:
 *    CJC 6.15.14 created
 *    CJC 6.18.14 changed calorimeter into tube
 *    BYS 6.26.14 added target sensitive detector
 *
 * file: DetectorConstruction.cc
 */

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "CalorimeterSD.hh"
#include "TargetSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SDManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "math.h"



//G4ThreadLocal
//G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = 0;

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),
fLogicCalor(NULL), //logical volume for calorimeter
  fLogicTarget(NULL), //logical volume for target
  fTargetMaterial(NULL), //material of target
  fCalorMaterial(NULL), //material of calorimeter
    fWorldMaterial(NULL),
  fStepLimit(NULL), 
    fCheckOverlaps(true) 
    //  fCenterToFront(0.)
{
 fMessenger = new DetectorMessenger(this);
 fLogicCalor = new G4LogicalVolume*[10];
}

DetectorConstruction::~DetectorConstruction()
{
  delete fStepLimit;
  delete fMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  //Define the materials
  DefineMaterials();

  //Define volumes
  return DefineVolumes();
}

/*
 * Method used to set the materials of the experiment
 * Liquid hydrogen for the target has to be defined
 * as well as Cesium Iodide for the crystals. Other materials
 * can be found in the NIST directory
 * CJC 6.18.14
 * vacuum for tunnel 
 */

void DetectorConstruction::DefineMaterials()
{

  G4NistManager* nistManager = G4NistManager::Instance();

  nistManager->FindOrBuildMaterial("G4_AIR");
 
  G4double z, a, density, pressure, temperature;
  G4String name, symbol;
  G4int nComp, nAtom;


  //Vacuum for chamber
  density = universe_mean_density;
  pressure = 1.e-19*pascal;
  temperature = 0.1*kelvin;
  G4Material* vacuum = new G4Material(name="Vacuum", z=1., a=1.01*g/mole, 
					  density, kStateGas, temperature, 
					  pressure);

  fWorldMaterial = vacuum;



  //Liquid hydrogen for the target
  a = 1.01*g/mole;
  G4Element* ele_H = new G4Element(
				   name="Hydrogen", //name
				   symbol="H", //symbol
				   z=1.,//atomic number
				   a); // mass / mole

  density = .07085*g/cm3; //density of Liquid Hydrogen
  G4Material* lh2 = new G4Material(name = "Liquid Hydrogen", density, nComp =1);

  lh2->AddElement(ele_H, nAtom=2);
  fTargetMaterial = lh2; // Target material is now liquid hydrogen

  //Cesium Iodide for the crystals

  a = 132.9*g/mole;
  G4Element* ele_Cs = new G4Element(name = "Cesium", symbol = "Cs", z = 55., a);

  a =126.9*g/mole;
  G4Element* ele_I = new G4Element(name="Iodide", symbol = "I", z = 53., a);

  density = 4.51*g/cm3;
  G4Material* CsI = new G4Material(name="Cesium Iodide", density, nComp=2);
  CsI->AddElement(ele_Cs, 1);
  CsI->AddElement(ele_I, 1); 

  fCalorMaterial = CsI;

  //Print Materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;


}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{

  G4Material* Air = G4Material::GetMaterial("G4_AIR");

  //Sizes and lengths

  G4double targetLength = 10.0*cm; // depth of target
  G4double targetFace = 10.0*cm; //lengths of sides of face of target

  G4double crystalLength = 2.54*12.0*cm; 
  G4double calorLength = crystalLength;

  G4double calorSpacing = 10*m; //distance from target to calorimeter
  G4double targetPos = -(.5*calorSpacing); //position of Z coordinate of target
  G4double calorDist = 10*m + .5*targetLength;
  G4double calorPos = calorDist + targetPos; //position of calorimeter

  G4double calorOuterRad = calorSpacing*tan(10 * pi /180); //outer radius of calorimeter
  G4double calorInnerRad = calorSpacing*tan(0 * pi / 180); //inner radius of calorimeter

  G4double worldLength = 1.2*(calorDist+crystalLength+targetLength-targetPos);

  //fCenterToFront = calorDist-0.5*calorLength;


  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);


  G4cout << "Computed tolerance = "
	 << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
	 << " mm" << G4endl;


  //World

G4Box* worldS = 
  new G4Box("world", 
	    worldLength/2, worldLength/2, worldLength/2);
G4LogicalVolume* worldLV
  = new G4LogicalVolume(
			worldS, // solid
			fWorldMaterial,  // material
			"World"); //logical volume's name

// Place the world

G4VPhysicalVolume* worldPV
  = new G4PVPlacement(
		      0, //no rotation
		      G4ThreeVector(), // at origin
		      worldLV, //logical volume
		      "World", // name
		      0, // no mother volume
		      false, //no booleans
		      0, //copy number
		      fCheckOverlaps); // true

//!!!
//Target


 G4ThreeVector positionTarget = G4ThreeVector(0, 0, targetPos); 

G4Box* targetS = 
  new G4Box("target", targetFace/2, targetFace/2, targetLength/2);

 fLogicTarget = 
   new G4LogicalVolume(targetS, fTargetMaterial, "Target", 0,0,0);

 new G4PVPlacement(0, // no rotation
		   positionTarget, // at (x,y,z)
		   fLogicTarget, // logical volume
		   "Target", //name
		   worldLV, //mother volume
		   false, //no booleans
		   0, // copy number
		   fCheckOverlaps); //true

 G4cout << "Target is " << targetLength/cm << " cm of " <<
   fTargetMaterial->GetName() << G4endl;



// ##############################################################################
// ############## Brian Shin ####################################################
// ##############################################################################
//
// put detector behind the target
//

  G4Tubs* TargetDetectorS = new G4Tubs("TargetDetectorS", // name
                                       calorInnerRad,     // inner radius
                                       calorOuterRad,     // outer radius
                                       20*cm,              // depth
                                       0*rad,             // starting angle
                                       2*pi*rad);         // ending angle

  fLogicTargetDetector = new G4LogicalVolume(TargetDetectorS, Air, "TargetDetectorLV", 0, 0, 0);

  new G4PVPlacement(0,
                    G4ThreeVector(0*cm, 0*cm, targetPos + calorSpacing),
                    fLogicTargetDetector,
                    "TargetDetector",
                    worldLV,
                    false,
                    0,
                    fCheckOverlaps);
  

// ##############################################################################
// ############## Brian Shin ####################################################
// ##############################################################################

//  //!!!
//  //Calorimeter 

 
//  G4ThreeVector posCal = G4ThreeVector(0,0, calorPos); 
// /*
//  //Calorimeter Solid as a tube
// G4Tubs* calorimeterS = 
//   new G4Tubs("calorimeter",
// 	     calorInnerRad,
// 	     calorOuterRad,
// 	     calorLength/2, 
// 	     0.,
// 	     2*pi);
// fLogicCalor = 
//   new G4LogicalVolume(calorimeterS, fCalorMaterial, "Calorimeter", 0,0,0);


// new G4PVPlacement(0, 
// 		   posCal, 
// 		   fLogicCalor, 
// 		   "Calorimeter", 
// 		   worldLV, 
// 		   false, 
// 		   0, 
// 		   fCheckOverlaps);
// */


//  double arc = 2*pi*rad;
//  double calorSeg = (calorOuterRad-calorInnerRad)/10.;

//  for (int copyNum=1; copyNum<11; copyNum++) //ten rings, start at 1
// {
//   double newRad = calorInnerRad+((copyNum-1)*calorSeg);
//   int angleDiv = 60; //int( pi*2*newRad/50.); //finds best number of slices

//   G4Tubs* calorimeterS = new G4Tubs("calorimeter", 
// 				    newRad, 
// 				    newRad+calorSeg, 
// 				    calorLength/2, 
// 				    0.,
// 				    arc);

//   fLogicCalor[copyNum] = new G4LogicalVolume(calorimeterS, 
// 					     Air, 
// 					     "CalorimeterLV", 
// 					     0, 0, 0);

//   fLogicCalor[copyNum] ->SetVisAttributes(G4Colour(0.1*(copyNum-1),1.0-(copyNum-1)*.1, 1.0)); 
//   // G4VPhysicalVolume* calorPV = 
//   new G4PVPlacement(0, 
// 		    posCal, 
// 		    fLogicCalor[copyNum],
// 		    "Calorimeter_MV", 
// 		    worldLV, 
// 		    false, 
// 		    copyNum, 
// 		    fCheckOverlaps);

// G4Tubs* crystalS = 
//   new G4Tubs("crystal",
// 	     newRad, newRad+calorSeg, calorLength/2, 
// 	     0., arc/angleDiv);

// G4LogicalVolume* crystalLV = 
//   new G4LogicalVolume(crystalS, fCalorMaterial, "CrystalLV", 0,0,0);

// //G4VPhysicalVolume* crystalRep = 
//   new G4PVReplica("crystalRep", crystalLV, fLogicCalor[copyNum],
// 		kPhi, angleDiv, arc/angleDiv);


//  }


 //Visualization

 G4VisAttributes* pink = new G4VisAttributes(G4Colour(1.0, 0.4, 0.8));
 G4VisAttributes* color  = new G4VisAttributes(G4Colour(0.9, 0.7, 0.2));

 worldLV ->SetVisAttributes(new G4VisAttributes(G4Colour(1.0,1.0,1.0)));

 color->SetVisibility(true);

 fLogicTarget ->SetVisAttributes(pink);
 //fLogicCalor ->SetVisAttributes(color);

 //Setting user Limits

 G4double maxStep = 1.0*cm;
 fStepLimit = new G4UserLimits(maxStep);



 return worldPV;

}

void DetectorConstruction::ConstructSDandField()
{
  // //!!!
  // //Create a sensitive detector and put it with logical volumes
  // G4String calorimeterSDname = "CalorimeterSD";
  // CalorimeterSD* calorimeterSD =
  //   new CalorimeterSD(calorimeterSDname, "CalorimeterHitsCollection");

  // SetSensitiveDetector("CrystalLV", calorimeterSD, true); //sets SD to all logical volumes with the name CrystalLV

  // G4cout << "SD Construction.....Complete!" << G4endl;

  // ##############################################################################
  // ############## Brian Shin ####################################################
  // ##############################################################################
  //
  // sensitive detector behind the target

  TargetSD* targetSD = new TargetSD("TargetDetectorSD", "TargetHitsCollection");

  SetSensitiveDetector("TargetDetectorLV", targetSD, true);

  G4cout << "BRIAN: my sensitive detector is complete!" << G4endl;

  // ##############################################################################
  // ############## Brian Shin ####################################################
  // ##############################################################################
}

void DetectorConstruction::SetTargetMaterial(G4String materialName)
{
  G4NistManager* nistMan = G4NistManager::Instance();

G4Material* pttoMaterial = 
  nistMan->FindOrBuildMaterial(materialName);

 if(fTargetMaterial != pttoMaterial) {
   if ( pttoMaterial) {
     fTargetMaterial = pttoMaterial; 
     if (fLogicTarget) fLogicTarget->SetMaterial(fTargetMaterial);
     G4cout << "\n-----> The target is made of " << materialName << G4endl;
   }
   else {
     G4cout << "\n --> Warning from SetTargetMaterial: " << 
       materialName << " not found" << G4endl;
   }
 }
}

void DetectorConstruction::SetCalorMaterial(G4String materialName)
{
  G4NistManager* nistMan = G4NistManager::Instance();

G4Material* pttoMaterial = 
  nistMan->FindOrBuildMaterial(materialName);

 if(fTargetMaterial != pttoMaterial) {
   if ( pttoMaterial) {
     fCalorMaterial = pttoMaterial; 
     for (G4int copyNum=0; copyNum<10; copyNum++){
     if (fLogicCalor[copyNum]) fLogicCalor[copyNum]->SetMaterial(fCalorMaterial);
     G4cout << "\n-----> The calorimeter is made of " << materialName << G4endl;
     }
   }
   else {
     G4cout << "\n --> Warning from SetCalorMaterial: " << 
       materialName << " not found" << G4endl;
   }
 }
}


void DetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

void DetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
  G4cout << "Checking overlaps....." <<G4endl;
  fCheckOverlaps = checkOverlaps; 
  G4cout << fCheckOverlaps <<G4endl;
}

//Implement later
/*
G4double DetectorConstruction::GetCalorDistance()
{
  return fCenterToFront;
}
*/
       




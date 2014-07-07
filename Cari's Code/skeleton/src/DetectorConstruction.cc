/*
 * Dark Photon Detector Construction
 * !!!History:
 *    CJC 6.15.14 created
 *
 * file: DetectorConstruction.cc
 */

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//G4ThreadLocal
//G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = 0;

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),
fLogicCrystal(NULL), //logical volume for calorimeter
  fLogicTarget(NULL), //logical volume for target
  fTargetMaterial(NULL), //material of target
  fCalorMaterial(NULL), //material of calorimeter
  fStepLimit(NULL), 
  fCheckOverlaps(true)
{
 fMessenger = new DetectorMessenger(this);
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
 */

void DetectorConstruction::DefineMaterials()
{

  G4NistManager* nistManager = G4NistManager::Instance();

  //Air defined using NIST
  nistManager->FindOrBuildMaterial("G4_AIR");

  //Liquid Hydrogen for the target
  G4double z, a, density;
  G4String name, symbol;
  G4int nComp, nAtom;

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
  G4Material* air = G4Material::GetMaterial("G4_AIR");

  //Sizes and lengths

  G4double targetLength = 10.0*cm; // depth of target
  G4double targetFace = 10.0*cm; //lengths of sides of face of target
  G4double targetPos = -50.0*cm; // Z coordinate of center of target
  //!!!
  G4double calorDist = 200.0*cm; //distance from target to calorimeter
  G4double calorPos = calorDist + targetPos; //position of calorimeter

  G4int numArray = 50; //number of crystals along a side of the calorimeter
  G4double crystalFace = 3.0*cm; // length of one side of the cross section of crystal
  G4double crystalLength = 12.0*cm; //length of crystal
  G4double motherFace = numArray*crystalFace; //defining mother volumes for crystals
  G4double motherLength = crystalLength;

  G4double worldLength = 2*(calorDist+crystalLength+targetLength);

  //Definitions of geometries

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
			air,  // material
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
  new G4Box("target", targetFace/2, targetFace/2, targetLength);

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

 //!!!
 //Using replicas to generate crystal array
 //Calorimeter Array

 G4ThreeVector posCalMother = G4ThreeVector(0,0, calorPos); 

 //Mother volume
G4Box* motherS = 
  new G4Box("mother solid", motherFace/2, motherFace/2, motherLength/2);
G4LogicalVolume* motherLV = 
  new G4LogicalVolume(motherS, fCalorMaterial, "Mother", 0,0,0);

//Mother volume along x axis 
G4Box* motherXS = 
  new G4Box("mother solid x", motherFace/2, crystalFace/2, motherLength/2);
G4LogicalVolume* motherXLV = 
  new G4LogicalVolume(motherXS, fCalorMaterial, "MotherX", 0,0,0);

//Crystals
G4Box* crystalS = 
  new G4Box("crystal", crystalFace/2, crystalFace/2, crystalLength/2);
fLogicCrystal = 
 new  G4LogicalVolume(crystalS, fCalorMaterial, "Crystal", 0,0,0);

new G4PVPlacement(0, 
		   posCalMother, 
		   motherLV, 
		   "Mother of Calorimeter", 
		   worldLV, 
		   false, 
		   0, 
		   fCheckOverlaps);


 //X-Array by replicas

 G4PVReplica repX("LinearArrayX", 
		  fLogicCrystal, 
		  motherXLV, 
		  kXAxis, numArray, crystalFace, 0);

 // Y-Array by replicas

 G4PVReplica repY("LinearArrayY", 
		  motherXLV, 
		  motherLV, 
		  kYAxis, numArray, crystalFace, 0);



 //Visualization

 G4VisAttributes* color = new G4VisAttributes(G4Colour(1.0, 0.4, 0.8));
 G4VisAttributes* white = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));

 worldLV ->SetVisAttributes(white);

 color->SetVisibility(true);

 fLogicTarget ->SetVisAttributes(color);
 fLogicCrystal ->SetVisAttributes(color);

 //Setting user Limits

 G4double maxStep = 1.0*cm;
 fStepLimit = new G4UserLimits(maxStep);



 return worldPV;

}

void DetectorConstruction::ConstructSDandField()
{
  //!!!
  //Create a sensitive detector and put it with logical volumes
  G4cout << "SD Construction" << G4endl;
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
     if (fLogicCrystal) fLogicCrystal->SetMaterial(fCalorMaterial);
     G4cout << "\n-----> The target is made of " << materialName << G4endl;
   }
   else {
     G4cout << "\n --> Warning from SetTargetMaterial: " << 
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

       



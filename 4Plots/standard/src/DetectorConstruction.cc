/*
 * Dark Photon Detector Construction
 * !!!History:
 *    CJC 6.15.14 created
 *    CJC 6.18.14 changed calorimeter into tube
 *
 * file: DetectorConstruction.cc
 */

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "BasicHitSD.hh"

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
#include "G4UnionSolid.hh"

#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"



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
 fLogicCalor = new G4LogicalVolume*[35];
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
  nistManager->FindOrBuildMaterial("G4_LIQUID_HYDROGEN");
 
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
  fCalorMaterial = Air;
  //Sizes and lengths

  G4double targetLength = 10.0*cm; // depth of target
  G4double targetFace = 10.0*cm; //lengths of sides of face of target

  G4double crystalLength = 2.54*12.0*cm; 

  G4double calorSpacing = 10*m; //distance from target to calorimeter
  G4double targetPos = -(.5*calorSpacing); //position of Z coordinate of target
  G4double calorDist = 10*m + .5*targetLength;
  G4double calorPos = calorDist + targetPos; //position of calorimeter

  G4double worldLength = 2*calorSpacing;

  G4double centerToFront = calorDist-0.5*crystalLength;


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
   new G4LogicalVolume(targetS, fTargetMaterial, "CrystalLV", 0,0,0);

 new G4PVPlacement(0, 
		   positionTarget,
		   fLogicTarget,
		   "Target", 
		   worldLV, 
		   false, 
		   0,
		   fCheckOverlaps);

 G4cout << "Target is " << targetLength/cm << " cm of " <<
   fTargetMaterial->GetName() << G4endl;


 //!!!
 //Calorimeter 

 G4double theta1 = 2.*deg;
 G4double theta2 = 5.*deg;

 G4double innerRad = ThetaToDistance(theta1, centerToFront);
 G4double outerRad = ThetaToDistance(theta2, centerToFront);

G4cout << "The target is centered at: " << targetPos << G4endl;
 G4cout << "The calorimeter is centered at : " << calorPos << G4endl;
 G4cout << "Inner radius is: " << innerRad << G4endl;
 G4cout << "Outer radius is: " << outerRad << G4endl;


G4Tubs* calorimeterS = 
  new G4Tubs("calorimeterS", innerRad,
	     outerRad,
	     crystalLength/2, 0.*deg, 360.*deg);

G4LogicalVolume* calorimeterLV = 
  new G4LogicalVolume( calorimeterS, fCalorMaterial, "CrystalLV", 0,0,0);

 new G4PVPlacement(0,
	       G4ThreeVector(0., 0., calorPos),
	       calorimeterLV, 
	       "Calorimeter",
	       worldLV, 
	       false, 
	       0, 
	       fCheckOverlaps);
 /*
 
G4VSolid* beamS = 
  new G4Box("beamS", targetLength, targetLength, 1.*cm);

 G4LogicalVolume* beamLV = 
   new G4LogicalVolume (beamS, fWorldMaterial, "CrystalLV", 0, 0, 0);
 
 new G4PVPlacement(0, 
		   G4ThreeVector(0., 0., targetPos+6.*cm),
		   beamLV, 
		   "BeamCheck", 
		   worldLV, 
		   false, 
		   0, 
		   fCheckOverlaps);
 */



 //Visualization

 G4VisAttributes* pink = new G4VisAttributes(G4Colour(1.0, 0.4, 0.8));
 G4VisAttributes* color  = new G4VisAttributes(G4Colour(0.9, 0.7, 0.2));

 worldLV ->SetVisAttributes(new G4VisAttributes(G4Colour(1.0,1.0,1.0)));

 color->SetVisibility(true);

 fLogicTarget ->SetVisAttributes(pink);
 //Setting user Limits

 G4double maxStep = 1.0*cm;
 fStepLimit = new G4UserLimits(maxStep);



 return worldPV;

}

void DetectorConstruction::ConstructSDandField()
{

  G4String basicSDname = "BasicHitSD";
  BasicHitSD* basicSD =
    new BasicHitSD(basicSDname, "BasicHitsCollection");

  //SMEARING
  basicSD->SetSmearing(true);

  SetSensitiveDetector("CrystalLV", basicSD, true); 

  G4cout << "SD Construction.....Complete!" << G4endl;
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

G4double DetectorConstruction::ThetaToDistance(G4double theta, G4double dist)
{

  return std::tan(theta)*dist; 

}
 




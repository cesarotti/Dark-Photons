/*
 * Dark Photon Detector Construction
 * !!!History:
 *    CJC 6.15.14 created
 *    CJC 6.18.14 changed calorimeter into tube
 *    CJC 7.08.14 completely new set up
 *
 * file: DetectorConstruction.cc
 */

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "TestSD.hh"
#include "OmniSD.hh"
#include "G4SDManager.hh"

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

#include "SquareParameterisation.hh"
#include "G4PVParameterised.hh"
#include "G4SubtractionSolid.hh"



//G4ThreadLocal
//G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = 0;

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),
fLogicCalor(NULL), //logical volume for calorimeter
    fLogicTarget(NULL),
  fTargetMaterial(NULL), //material of target
  fCalorMaterial(NULL), //material of calorimeter
    fWorldMaterial(NULL),
  fStepLimit(NULL), 
    fCheckOverlaps(true), 
    fLiningMaterial(NULL)
    //  fCenterToFront(0.)
{
 fMessenger = new DetectorMessenger(this);
 fLogicCalor = new G4LogicalVolume*[1225];
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
  density*=100.;
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

  //Lead for lining calorimeter
  a = 207.20*g/mole;
  G4Element* elPb = new G4Element(name="lead",symbol="Pb", z=82., a);

  density = 11.34*g/cm3;
  G4Material* Pb = new G4Material(name="Lead", density, nComp=1);
  Pb->AddElement(elPb, 1);
  fLiningMaterial = Pb;

  //Print Materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;


}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{


  //Sizes and lengths

  G4double targetLength = 10.0*cm; // depth of target
  G4double targetFace = 10.0*cm; //lengths of sides of face of target

  G4double crystalLength = 2.54*12.0*cm; 

  G4double calorSpacing = 10.*m; //distance from target to calorimeter
  G4double targetPos = -(.5*calorSpacing); //position of Z coordinate of target
  G4double calorDist = .5*calorSpacing + .5*targetLength;


  G4double worldLength = 3*(calorDist+crystalLength+targetLength-targetPos);


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
 
 /*
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

 */

 //!!!
 //Calorimeter 

 G4int crysLength = 5.*cm;

G4VSolid* boxS =
  new G4Box("boxS", crysLength/2, crysLength/2, crystalLength/2);

 G4double xPos[1225] = {};
 G4double yPos[1225] = {};

 G4ThreeVector position = G4ThreeVector(); 

 for (G4int i=0; i<1225; i++)
   {
     xPos[i] = (i%35-17)*crysLength; 
     yPos[i] = (i/35-17)*crysLength;

     position = G4ThreeVector(xPos[i], yPos[i], calorDist);
     fLogicCalor[i] = new G4LogicalVolume(boxS,
					  fCalorMaterial,
					  "CrystalLV", 
					  0, 0, 0);
    
     if (i<360 || (i>374 && i <395) ||(i>409 && i <430)||(i>444 && i<465) || 
	 (i>479 && i<500) || (i>514 && i<535) || (i>549 && i<570) ||
	 (i>584 && i<605) || (i>619 && i<640) || (i>654 && i<675) ||
	 (i>689 && i<710) || (i>724 && i<745) || (i>759 && i<780) ||
	 (i>794 && i<815) || (i>829 && i<850) || (i>864))
       {
	 new G4PVPlacement(0, 
			   position, 
			   fLogicCalor[i], 
			   "ClusterPV", 
			   worldLV, 
			   false, 
			   i, 
			   fCheckOverlaps);
       		
     }			  
   }
 // Lining
 
 G4VSolid* outerS =
   new G4Box("outerS", 5.*cm+17.5*crysLength, 5.*cm+17.5*crysLength, crystalLength/2);
 
G4VSolid* innerS = 
  new G4Box("innerS", 17.5*crysLength+.5*mm, 17.5*crysLength+.5*mm, crystalLength/2);
 
 G4SubtractionSolid* liningS = new G4SubtractionSolid("Lining", outerS, innerS);

G4LogicalVolume* liningLV =
  new G4LogicalVolume ( liningS, 
			fLiningMaterial, 
			"LiningLV",
			0, 0, 0);

 new G4PVPlacement(0, 
		   G4ThreeVector(0., 0., calorDist),
		   liningLV,
		   "LiningPV", 
		   worldLV, 
		   false,
		   0, 
		   fCheckOverlaps);

 G4VSolid* holeOutS =
   new G4Box("holeOutS", 7.5*crysLength-.5*mm, 7.5*crysLength-.5*mm, crystalLength/2);
 
G4VSolid* holeInS = 
  new G4Box("holeInS", 7.5*crysLength - 5.*cm, 7.5*crysLength - 5.*cm, crystalLength/2);
 
 G4SubtractionSolid* liningHS = 
   new G4SubtractionSolid("LiningH", holeOutS, holeInS);

G4LogicalVolume* liningHLV =
  new G4LogicalVolume ( liningHS, 
			fLiningMaterial, 
			"LiningHLV",
			0, 0, 0);

 new G4PVPlacement(0, 
		   G4ThreeVector(0., 0., calorDist),
		   liningHLV,
		   "LiningHPV", 
		   worldLV, 
		   false,
		   0, 
		   fCheckOverlaps);

 G4double crysConst = 5.*cm;
 G4double startRad = 7.5; 
     G4VSolid* omni = 
       new G4Tubs("omniS", (startRad)*crysConst, (startRad+(15))*crysConst,
		  .005*cm, 0.*deg, 360.*deg);

     G4LogicalVolume* omniLV = 
       new G4LogicalVolume(omni, fWorldMaterial, "OmniLV");

     new G4PVPlacement(0, 
		   G4ThreeVector(0., 0.,calorDist-.5*crystalLength-.01*cm), 
		   omniLV, 
		   "Omni", 
		   worldLV, 
		   false, 
		   0, 
		   fCheckOverlaps);
   



 //Visualization

 G4VisAttributes* color  = new G4VisAttributes(G4Colour(0.9, 0.7, 0.2));

 worldLV ->SetVisAttributes(new G4VisAttributes(G4Colour(1.0,1.0,1.0)));
 fLogicTarget ->SetVisAttributes(color);

 liningHLV->SetVisAttributes(new G4VisAttributes(G4Colour(.7, .7, .7)));
 liningLV->SetVisAttributes(new G4VisAttributes(G4Colour(.7, .7, .7)));



 color->SetVisibility(true);

 //Setting user Limits

 G4double maxStep = 1.0*cm;
 fStepLimit = new G4UserLimits(maxStep);



 return worldPV;

   }


void DetectorConstruction::ConstructSDandField()
{
  //!!!
  //Create a sensitive detector and put it with logical volumes
  G4SDManager* sdMan = G4SDManager::GetSDMpointer();
  G4String SDname;


  SDname = "/calorimeterSD";
  TestSD* calorimeterSD =
    new TestSD(SDname, "TestHitsCollection");
  sdMan->AddNewDetector(calorimeterSD);

  SetSensitiveDetector("CrystalLV", calorimeterSD, true); //sets SD to all logical volumes with the name CrystalLV


  SDname = "/omniSD";
  OmniSD* omniSD = 
    new OmniSD(SDname, "OmniHitsCollection");
  sdMan->AddNewDetector(omniSD);
  SetSensitiveDetector("OmniLV", omniSD, true);
  
 
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

 if(fCalorMaterial != pttoMaterial) {
   if ( pttoMaterial) {
     fCalorMaterial = pttoMaterial; 
     for (G4int copyNum=0; copyNum<49; copyNum++){
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
  fCheckOverlaps = checkOverlaps; 
}





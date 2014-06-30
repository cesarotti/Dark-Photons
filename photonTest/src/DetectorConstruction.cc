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
#include "TestSD.hh"

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
  fCalorMaterial(NULL), //material of calorimeter
    fWorldMaterial(NULL),
  fStepLimit(NULL), 
    fCheckOverlaps(true) 
    //  fCenterToFront(0.)
{
 fMessenger = new DetectorMessenger(this);
 fLogicCalor = new G4LogicalVolume*[30];
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
  G4int nComp;


  //Vacuum for chamber
  density = universe_mean_density;
  pressure = 1.e-19*pascal;
  temperature = 0.1*kelvin;
  G4Material* vacuum = new G4Material(name="Vacuum", z=1., a=1.01*g/mole, 
					  density, kStateGas, temperature, 
					  pressure);

  fWorldMaterial = vacuum;


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

  G4int size = 30; 

  G4double crystalLength = 2.54*12.0*cm; 
  G4double calorLength = crystalLength;
  G4double crystalFace = 5.0*cm;

  G4double calorPos = 10*m; //position of calorimeter

  G4double worldLength = 20*m+12*2.54*cm;

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

 //Calorimeter 

 G4double hFace = size*5.0*cm;

 for (int copyNum =0; copyNum<size; copyNum++)
   {
 G4Box* detector = 
   new G4Box("horizontal array", hFace/2, crystalFace/2, calorLength/2);

 fLogicCalor[copyNum] = new G4LogicalVolume(detector,
					Air, 
					"CalorimeterLV", 
					0,0,0);

 fLogicCalor[copyNum] ->SetVisAttributes(G4Colour(1.0,1.0, 1.0));

 G4ThreeVector posCal = G4ThreeVector(0,.612*m+ (copyNum-15)*5.0*cm, 0);

 new G4PVPlacement(0, 
		   posCal, 
		   fLogicCalor[copyNum], 
		   "Calorimeter_MV",
		   worldLV, 
		   false, 
		   copyNum+1, 
		   fCheckOverlaps);

G4Box* crystalS = 
  new G4Box("crystal", crystalFace/2, crystalFace/2, crystalLength/2);

G4LogicalVolume* crystalLV = 
  new G4LogicalVolume(crystalS, fCalorMaterial, "CrystalLV", 0,0,0);

new G4PVReplica("crystalRep", crystalLV, fLogicCalor[copyNum], 
		 kXAxis,size, 5.0*cm);

 crystalLV->SetVisAttributes(G4Colour(1.0, 1.0, 1.0));

   }


 //Visualization

 // G4VisAttributes* pink = new G4VisAttributes(G4Colour(1.0, 0.4, 0.8));
 G4VisAttributes* color  = new G4VisAttributes(G4Colour(0.9, 0.7, 0.2));

 worldLV ->SetVisAttributes(new G4VisAttributes(G4Colour(1.0,1.0,1.0)));

 color->SetVisibility(true);


 G4double maxStep = 1.0*cm;
 fStepLimit = new G4UserLimits(maxStep);



 return worldPV;

}

void DetectorConstruction::ConstructSDandField()
{
  
  //!!!
  //Create a sensitive detector and put it with logical volumes
  
   G4String testSDname = "TestSD";
  TestSD* testSD =
    new TestSD(testSDname, "TestHitsCollection");

  SetSensitiveDetector("CrystalLV", testSD, true); //sets SD to all logical volumes with the name CrystalLV
  

  G4cout << "SD Construction.....Complete!" << G4endl;
}


void DetectorConstruction::SetCalorMaterial(G4String materialName)
{
  G4NistManager* nistMan = G4NistManager::Instance();

G4Material* pttoMaterial = 
  nistMan->FindOrBuildMaterial(materialName);

 if(fCalorMaterial != pttoMaterial) {
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





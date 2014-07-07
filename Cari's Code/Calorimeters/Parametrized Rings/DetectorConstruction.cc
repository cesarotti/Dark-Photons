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

#include "G4PVParameterised.hh"
#include "RingParam.hh"



//G4ThreadLocal
//G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = 0;

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),
  fCalorMaterial(NULL), //material of calorimeter
    fWorldMaterial(NULL),
  fStepLimit(NULL), 
    fCheckOverlaps(true) 
    //  fCenterToFront(0.)
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

  nistManager->FindOrBuildMaterial("G4_CESIUM_IODIDE");


  //Print Materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;


}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{

  G4Material* Air = G4Material::GetMaterial("G4_AIR");

  G4Material* csI = G4Material::GetMaterial("G4_CESIUM_IODIDE");

  fCalorMaterial = csI;

  //Sizes and lengths

  G4int size = 30; 

  G4double crystalLength = 2.54*12.0*cm; 
  G4double crystalFace = 5.0*cm;


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

 G4int numRing = 20;

G4VSolid* calorimeterS = 
  new G4Tubs("calorimeterS", 0., numRing*crystalFace,
	     crystalLength/2, 0.*deg, 360.*deg);

G4LogicalVolume* calorimeterLV = 
  new G4LogicalVolume(calorimeterS, Air, "CalorimeterLV");

 new G4PVPlacement (0, 
		    G4ThreeVector(), 
		    calorimeterLV, 
		    "Calorimeter", 
		    worldLV,
		    false, 
		    0, 
		    fCheckOverlaps); 



G4VSolid* crystalS = 
  new G4Tubs("crystalS", 0., 5.0*cm, crystalLength, 0.*deg, 360.*deg);

G4LogicalVolume* crystalLV = 
  new G4LogicalVolume(crystalS, 
		      fCalorMaterial,
		      "CrystalLV");

 G4double delRad = 2.0*cm;

 G4VPVParameterisation* crysParam = new RingParam(numRing, 0,
						  delRad, crystalLength);

 new G4PVParameterised("CrystalPV",
		       crystalLV, 
		       calorimeterLV, 
		       kZAxis,
		       numRing, 
		       crysParam, 
		       fCheckOverlaps);



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





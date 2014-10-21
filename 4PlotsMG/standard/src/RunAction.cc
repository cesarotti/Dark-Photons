/*
 * Dark Photon Run Action
 * !!!History 
 *    CJC 6.14.15 created
 *
 * file RunAction.cc
 * Description: 
 * Run Action is called for every run. Consider what data should be collected
 * over long runs
 */



#include "RunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

#include "Analysis.hh"
#include "G4SystemOfUnits.hh"

#include "G4EmCalculator.hh"

#include "G4Positron.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4ProcessManager.hh"
#include "G4VEmProcess.hh"
#include "G4eplusAnnihilation.hh"
#include <G4UnitsTable.hh>


//!!!
//Initialize analysis storing i.e. ntuples or histograms
RunAction::RunAction()
  : G4UserRunAction()
{
  //set printing event number for each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);

  // Data storage and analysis
  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();
  G4cout << "Using" << analysisMan->GetType() << G4endl;
  analysisMan->SetVerboseLevel(2);

  //Creating directories
  
   analysisMan->CreateNtuple("Signal", "Hits");
   analysisMan->CreateNtupleIColumn("numHits"); // 0
   analysisMan->CreateNtupleDColumn("energyTot"); // 1
   analysisMan->CreateNtupleDColumn("XPosition"); // 2
   analysisMan->CreateNtupleDColumn("YPosition"); // 3
   analysisMan->CreateNtupleIColumn("Particle_ID"); // 4
   analysisMan->CreateNtupleDColumn("Theta"); // 5
   analysisMan->CreateNtupleDColumn("zPosition"); //6
   analysisMan->FinishNtuple();

   //Keep track of number of events that reach the detector
   analysisMan->CreateNtuple("Event", "Hits");
   analysisMan->CreateNtupleIColumn("Hit_Detector_Bool"); // 1
   analysisMan->FinishNtuple();
  

}


//delete your analysis manager
RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();
}
 
//!!!
//Data storage
void RunAction::BeginOfRunAction(const G4Run*)
{

  //Save random number seed
  // G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  
  // Data storage and analysis
  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();
 
   analysisMan->OpenFile("Signal");

   G4cout << "BEGINNING OF RUN LOADED" << G4endl;



   //Cross Section
   /*
    * IF YOU WANT TO CALCULATE THIS FOR YOUR PROCESSES, ALL YOU HAVE TO DO IS
    * ADD IN THE APPROPRIATE PROCESSES
    */
   
   /*

   G4EmCalculator emCal;

   G4ParticleDefinition* positron = G4Positron::PositronDefinition();
   G4String particleName = positron->GetParticleName();
   G4double charge = positron->GetPDGCharge();
   G4double energy = 5.*GeV;

  G4double z, a, density;
  G4String name, symbol;
  G4int nComp, nAtom;

  a = 1.01*g/mole;
  G4Element* ele_H = new G4Element(
				   name="Liquid Hydrogen", //name
				   symbol="H", //symbol
				   z=1.,//atomic number
				   a); // mass / mole

  density = .07085*g/cm3; //density of Liquid Hydrogen
  G4Material* material= new G4Material(name = "Liquid Hydrogen",
				       density,
				       nComp =1);

  material->AddElement(ele_H, nAtom=2);

   
   G4String matName = material->GetName();
   density = material->GetDensity();
   G4double radl = material->GetRadlen();
   G4double Z = material->GetZ();
   G4double A = material->GetA();
   

   G4VEmProcess* eplusProc = new G4eplusAnnihilation();
   G4String procName = eplusProc->GetProcessName();

   G4double sigma = emCal.ComputeCrossSectionPerAtom(energy, 
						      positron, 
						      procName, 
						      Z,
						      A,
						      526.62*keV);

   G4cout << "Cross Section Per Atom: "
	  << G4BestUnit(sigma, "Surface") << G4endl;

   G4double sigmaV = emCal.ComputeCrossSectionPerVolume(energy,
							positron,
							procName,
							material);

   G4cout << "Cross Section Per Volume: " << sigmaV*cm << "cm^-1" << G4endl;
   
*/

}

//!!!
//Finish and close data file
void RunAction::EndOfRunAction(const G4Run* )
{
  
  G4cout << "END OF RUN ACTION" << G4endl;
  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();
  analysisMan->Write();
  analysisMan->CloseFile();
}


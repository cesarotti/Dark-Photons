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
/*
  //Creating directories
   analysisMan->SetNtupleDirectoryName("ntuples");

   analysisMan->CreateH1("Photon_Hits", 
			 "Photon Hits", 
			 10, 
			 -.5,
			 3., 
			 "none",
			 "none",
			 "linear");


   analysisMan->CreateNtuple("Gamma_Info", "Photon hits");
   analysisMan->CreateNtupleIColumn("numHits"); // 0
   analysisMan->CreateNtupleDColumn("photon1xPos"); // 1
   analysisMan->CreateNtupleDColumn("photo1yPos"); // 2
   analysisMan->CreateNtupleDColumn("totEnergyPhoton1"); // 3
   analysisMan->CreateNtupleDColumn("photon2xPos"); //4
   analysisMan->CreateNtupleDColumn("photon2yPos"); //5
   analysisMan->CreateNtupleDColumn("totEnergyPhoton2"); // 6
   analysisMan->CreateNtupleDColumn("theta1"); //7
   analysisMan->CreateNtupleDColumn("theta2"); //8
   analysisMan->CreateNtupleIColumn("crystalNum1"); //9
   analysisMan->CreateNtupleIColumn("cyrstalNum2"); //10
   analysisMan->CreateNtupleIColumn("ringNum1"); // 11
   analysisMan->CreateNtupleIColumn("ringNum2"); // 12 
   analysisMan->FinishNtuple();
  
*/

   //analysisMan->SetNtupleDirectoryName("ntuples");

   analysisMan->CreateNtuple("Hits_Info", "Particle ID and Energy and Theta");
   analysisMan->CreateNtupleIColumn("PDGID");     // 0
   analysisMan->CreateNtupleDColumn("Energy");    // 1
   analysisMan->CreateNtupleDColumn("theta");     // 2
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
 
   analysisMan->OpenFile("InelasticHitsData");

}

//!!!
//Finish and close data file
void RunAction::EndOfRunAction(const G4Run* )
{
  
  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();
  analysisMan->Write();
  analysisMan->CloseFile();
}
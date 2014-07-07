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

  //Creating directories
  
   analysisMan->CreateNtuple("Gamma_Info", "Photon hits");
   analysisMan->CreateNtupleIColumn("numHits");
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
 
   analysisMan->OpenFile("TwoGamma");

}

//!!!
//Finish and close data file
void RunAction::EndOfRunAction(const G4Run* )
{
  
  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();
  analysisMan->Write();
  analysisMan->CloseFile();
}

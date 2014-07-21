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

  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();
  analysisMan->SetVerboseLevel(2);

  analysisMan->CreateH1("0", "Bool_of_hits", 20, 0, 19);

  analysisMan->CreateNtuple("CalorimeterData", "Energy_In_Crystals"); 
  //Create a container for each ring
  analysisMan->CreateNtupleDColumn("Energy_Dep0");
  analysisMan->CreateNtupleDColumn("Energy_Dep1");
  analysisMan->CreateNtupleDColumn("Energy_Dep2");
  analysisMan->CreateNtupleDColumn("Energy_Dep3");
  analysisMan->CreateNtupleDColumn("Energy_Dep4");
  analysisMan->CreateNtupleDColumn("Energy_Dep5");
  analysisMan->CreateNtupleDColumn("Energy_Dep6");
  analysisMan->CreateNtupleDColumn("Energy_Dep7");
  analysisMan->CreateNtupleDColumn("Energy_Dep8");
  analysisMan->CreateNtupleDColumn("Energy_Dep9");
  analysisMan->CreateNtupleDColumn("Energy_Dep10");
  analysisMan->CreateNtupleDColumn("Energy_Dep11");
  analysisMan->CreateNtupleDColumn("Energy_Dep12");
  analysisMan->CreateNtupleDColumn("Energy_Dep13");
  analysisMan->CreateNtupleDColumn("Energy_Dep14");
  analysisMan->CreateNtupleDColumn("Energy_Dep15");
  analysisMan->CreateNtupleDColumn("Energy_Dep16");
  analysisMan->CreateNtupleDColumn("Energy_Dep17");
  analysisMan->CreateNtupleDColumn("Energy_Dep18");
  analysisMan->CreateNtupleDColumn("Energy_Dep19");

  analysisMan->CreateNtupleIColumn("numHits");  //20
  analysisMan->CreateNtupleDColumn("Energy_Dep"); //21


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
  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();

  analysisMan->OpenFile("Calorimeter");

}

//!!!
//Finish and close data file
void RunAction::EndOfRunAction(const G4Run* )
{
  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();
  analysisMan->Write();
  analysisMan->CloseFile();
}

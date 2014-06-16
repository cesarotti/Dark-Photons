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

RunAction::RunAction()
  : G4UserRunAction()
{
  /*
HERE IS WHERE YOU INITIALIZE ALL NTUPLES AND HISTOGRAMS
   */
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  //Save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  /*
HERE IS WHERE YOU DO DATA STORAGE FOR YOUR ANALYSIS
   */
}

void RunAction::EndOfRunAction(const G4Run* )
{
}

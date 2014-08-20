#include "DarkPhysicsList.hh"
#include "EmStandardPhysics.hh"
#include "G4Positron.hh"
#include "G4VEmProcess.hh"
#include "G4eplusAnnihilation.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProcessManager.hh"

DarkPhysicsList::DarkPhysicsList() :
G4VModularPhysicsList() 
{
  SetVerboseLevel(0);
  RegisterPhysics(new EmStandardPhysics());  
}

DarkPhysicsList::~DarkPhysicsList()
{}
/*
void DarkPhysicsList::ConstructParticle()
{
  G4Positron::PositronDefinition();
}

void DarkPhysicsList::ConstructProcess()
{
  
  AddTransportation();
  ConstructEM();
}

void DarkPhysicsList::ConstructEM()
{
  
  G4ParticleDefinition* positron = G4Positron::PositronDefinition();
  G4ProcessManager* pman = positron->GetProcessManager();

  G4VEmProcess* eplusProc = new G4eplusAnnihilation();
  eplusProc->SetCrossSectionBiasingFactor(1e+04, true);
  eplusProc->SetMinKinEnergy(4.8*GeV);
  pman->AddProcess(eplusProc, 0, -1, 4);
}
*/

void DarkPhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}

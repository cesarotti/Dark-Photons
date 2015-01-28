//
/*
 Physics list for positron-electron annihilation events
 Background specific
 Dark Photon group
 
 !!!History
    CJC 6.11.14 Created
    cJC 6.18.14 Modified likelihood of collision

 */

#include "PositronPhysicsList1.hh"


#include "G4ProcessManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Positron.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4Gamma.hh"

#include "G4VUserPhysicsList.hh"

#include "G4hIonisation.hh"
#include "G4eIonisation.hh"

#include "G4hBremsstrahlung.hh"
#include "G4eBremsstrahlung.hh"

#include "G4eplusAnnihilation.hh"

#include "G4VEmProcess.hh"
#include "AlwaysTwoGamma.hh"
#include "AlwaysTwoGammaMessenger.hh"
#include "DarkPhoton.hh"


PositronPhysicsList1::PositronPhysicsList1()
{
  defaultCutValue = 1.0*mm; //As me if you need energy cut offs, too
  SetVerboseLevel(0);
}

PositronPhysicsList1::~PositronPhysicsList1()
{}

void PositronPhysicsList1::ConstructParticle()
{
  G4Gamma::GammaDefinition();
  G4Positron::PositronDefinition();
  G4Electron::ElectronDefinition();
  G4Proton::ProtonDefinition();
}

void PositronPhysicsList1::ConstructProcess()
{
  AddTransportation();
  ConstructEM();

}

void PositronPhysicsList1::ConstructEM()
{
  G4ParticleDefinition* positron = G4Positron::PositronDefinition();
  pman = positron->GetProcessManager();
  G4cout << "About to register messenger" << G4endl;

  //ModelMessenger = new AlwaysTwoGammaMessenger(this);


  //CHANGED TO ALLOW FOR MANUAL INSERTION OF EVENTS
  //G4cout << "Registered messenger" << G4endl;

  //G4VEmProcess* new_process = new AlwaysTwoGamma("twogamma");
  //new_process->SetCrossSectionBiasingFactor(1e+06, true); 

  //pman->AddProcess(new_process, 0, -1, 4);
 
  G4VEmProcess *always = new DarkPhoton();
  always->SetCrossSectionBiasingFactor(1e+04, true); 

  pman->AddProcess(always, 0, -1, 4);
  




 //G4VEmProcess* eplusProc = new G4eplusAnnihilation();
  //How to bias
 //eplusProc->SetCrossSectionBiasingFactor(1e+06, true); 
 //pman->AddProcess(eplusProc, 0, -1, 4);
 
  
}


void PositronPhysicsList1::SetCuts()
{
  SetCutValue(defaultCutValue, "gamma");
  SetCutValue(defaultCutValue, "e-");
  SetCutValue(defaultCutValue, "e+");
  SetCutValue(defaultCutValue, "proton");
}




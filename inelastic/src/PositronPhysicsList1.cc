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

// ####################################### BRIAN SHIN ####################################
#include "G4PionZero.hh"
#include "G4Decay.hh"
#include "G4DecayTable.hh"
#include "G4PhaseSpaceDecayChannel.hh"
#include "G4DalitzDecayChannel.hh"

#include "G4PositronNuclearProcess.hh"
#include "G4ElectroVDNuclearModel.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
// ####################################### BRIAN SHIN ####################################

#include "G4VEmProcess.hh"


PositronPhysicsList1::PositronPhysicsList1()
{
  defaultCutValue = 1.0*mm;
  SetVerboseLevel(0);
}

PositronPhysicsList1::~PositronPhysicsList1()
{}

void PositronPhysicsList1::ConstructParticle()
{
  //G4Gamma::GammaDefinition();
  //G4Positron::PositronDefinition();
  //G4Electron::ElectronDefinition();
  //G4Proton::ProtonDefinition();

  G4BosonConstructor bosonCons;
  bosonCons.ConstructParticle();

  G4LeptonConstructor lepCons;
  lepCons.ConstructParticle();

  G4MesonConstructor mesonCons;
  mesonCons.ConstructParticle();

  G4IonConstructor ionCons;
  ionCons.ConstructParticle();

  G4BaryonConstructor baryonCons;
  baryonCons.ConstructParticle();

  G4ShortLivedConstructor slCons;
  slCons.ConstructParticle();
}

void PositronPhysicsList1::ConstructProcess()
{
  AddTransportation();
  //ConstructEM(); // commented out to isolate positron-nuclear process
  ConstructHadronic();
  ConstructDecay();

}

void PositronPhysicsList1::ConstructEM()
{
  G4ParticleDefinition* positron = G4Positron::PositronDefinition();
  G4ProcessManager* pman = positron->GetProcessManager();
  


  G4VEmProcess* eplusProc = new G4eplusAnnihilation();
  eplusProc->SetCrossSectionBiasingFactor(1e+06, true);
  pman->AddProcess(eplusProc, 0, -1, 4);
  

  //pman->AddProcess(new G4eplusAnnihilation(), 0, -1, 4);
 

}

// ####################################### BRIAN SHIN ####################################
void PositronPhysicsList1::ConstructHadronic()
{
  // get pointer to positron process manager
  G4ParticleDefinition* positron = G4Positron::PositronDefinition();
  G4ProcessManager* pman = positron->GetProcessManager();

  // get pointer to positron-nuclear process
  G4PositronNuclearProcess* pnproc = new G4PositronNuclearProcess("PositronNuclear");
  
  // register positron-nuclear model
  G4ElectroVDNuclearModel* pnmodel = new G4ElectroVDNuclearModel();
  pnproc->RegisterMe(pnmodel);

  // set biasing
  pnproc->BiasCrossSectionByFactor(10000.);

  // add process to process manager
  pman->AddDiscreteProcess(pnproc);
}

void PositronPhysicsList1::ConstructDecay() {
  G4PionZero* pionZero = G4PionZero::PionZeroDefinition();
  G4ProcessManager* pman = pionZero->GetProcessManager();

  G4Decay* decayProc = new G4Decay();

  pman->AddProcess(decayProc);
  pman->SetProcessOrdering(decayProc, idxPostStep);
  pman->SetProcessOrdering(decayProc, idxAtRest);

  G4DecayTable* table = pionZero->GetDecayTable();

  // create a decay channel
  G4VDecayChannel* mode;

  // pi0 -> gamma + gamma
  mode = new G4PhaseSpaceDecayChannel("pi0",0.988,2,"gamma","gamma");
  table->Insert(mode);

  // pi0 -> gamma + e+ + e-
  mode = new G4DalitzDecayChannel("pi0",0.012,"e-","e+");
  table->Insert(mode);
}
// ###################################### BRIAN SHIN ####################################

void PositronPhysicsList1::SetCuts()
{
  SetCutValue(defaultCutValue, "gamma");
  SetCutValue(defaultCutValue, "e-");
  SetCutValue(defaultCutValue, "e+");
  SetCutValue(defaultCutValue, "proton");
}

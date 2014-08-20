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

#include "G4PionZero.hh"
#include "G4Decay.hh"
#include "G4DecayTable.hh"
#include "G4PhaseSpaceDecayChannel.hh"
#include "G4DalitzDecayChannel.hh"

#include "G4PositronNuclearProcess.hh"
#include "G4ElectronNuclearProcess.hh"
#include "G4ElectroVDNuclearModel.hh"
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4PhotoElectricEffect.hh"


#include "G4ProcessManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Positron.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4Gamma.hh"

#include "G4VUserPhysicsList.hh"

#include "G4hIonisation.hh"
#include "G4eIonisation.hh"
#include "G4ionIonisation.hh"

#include "G4hBremsstrahlung.hh"
#include "G4eBremsstrahlung.hh"

#include "G4eplusAnnihilation.hh"

#include "G4VEmProcess.hh"
#include "G4VEmModel.hh"

#include "G4eeToHadrons.hh"
#include "G4eeToHadronsMultiModel.hh"
#include "G4AnnihiToMuPair.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"

#include "G4PionZero.hh"
#include "G4Decay.hh"
#include "G4DecayTable.hh"
#include "G4PhaseSpaceDecayChannel.hh"
#include "G4DalitzDecayChannel.hh"

#include "G4PositronNuclearProcess.hh"
#include "G4ElectronNuclearProcess.hh"
#include "G4ElectroVDNuclearModel.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"



PositronPhysicsList1::PositronPhysicsList1()
{
  defaultCutValue = 1.0*mm;
  SetVerboseLevel(0);
}

PositronPhysicsList1::~PositronPhysicsList1()
{}

void PositronPhysicsList1::ConstructParticle()
{
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
  ConstructEM();
  //Construct2Gamma();
  //ConstructBremsstrahlung();
  ConstructPositronNuclear();
  //ConstructElectronNuclear();

}

void PositronPhysicsList1::ConstructEM()
{
  G4ParticleDefinition* positron = G4Positron::PositronDefinition();
  G4ProcessManager* pman = positron->GetProcessManager();

  G4ParticleDefinition* gamma = G4Gamma::GammaDefinition();
  G4ProcessManager* pman1 = gamma->GetProcessManager();

  G4ParticleDefinition* proton = G4Proton::ProtonDefinition();
  G4ProcessManager* pman2 = proton->GetProcessManager();

  G4VEmProcess* eplusProc = new G4eplusAnnihilation();
  eplusProc->SetCrossSectionBiasingFactor(1e+06, true);
  pman->AddProcess(eplusProc, 0, -1, 4);
  // pman->AddProcess(new G4MultipleScattering(), -1, 1, 1);
  pman->AddProcess(new G4eIonisation(),-1, 2, 2);
  pman->AddProcess(new G4eBremsstrahlung(),-1, 3, 3);
  
  /*
  G4VEmProcess* eTHMproc = new G4eeToHadrons();
  G4VEmModel* eTHMmodel = new G4eeToHadronsMultiModel();
  
  eTHMproc->RegisterMe(eTHMmodel);
  pman->AddProcess(eTHMproc);
  */
  
  pman->AddDiscreteProcess(new G4AnnihiToMuPair());
  
  pman2->AddProcess(new G4hIonisation(),-1, 2, 2);

  pman1->AddDiscreteProcess(new G4ComptonScattering);
  
  pman1->AddDiscreteProcess(new G4PhotoElectricEffect);
  pman1->AddDiscreteProcess(new G4GammaConversion);
  
  //pman->AddContinuousProcess(new G4Cerenkov);

  pman1->AddProcess(new G4Scintillation, 4, -1, 4); 
 
}

void PositronPhysicsList1::ConstructBremsstrahlung() {
  G4ParticleDefinition* electron = G4Electron::ElectronDefinition();
  G4ProcessManager* pman = electron->GetProcessManager();
  


  G4eBremsstrahlung* brems = new G4eBremsstrahlung();
  brems->SetCrossSectionBiasingFactor(100, true);
  pman->AddProcess(brems, -1, 3, 3);
}

void PositronPhysicsList1::ConstructPositronNuclear()
{
  // get pointer to positron process manager
  G4ParticleDefinition* positron = G4Positron::PositronDefinition();
  G4ProcessManager* pman = positron->GetProcessManager();

  // get pointer to positron-nuclear process
  G4PositronNuclearProcess* pnproc = new G4PositronNuclearProcess("PositronNuclear");
  
  // register positron-nuclear model
  G4ElectroVDNuclearModel* pnmodel = new G4ElectroVDNuclearModel();
  pnproc->RegisterMe(pnmodel);

  // add process to process manager
  pman->AddDiscreteProcess(pnproc);
}

void PositronPhysicsList1::ConstructElectronNuclear()
{
  // get pointer to positron process manager
  G4ParticleDefinition* electron = G4Electron::ElectronDefinition();
  G4ProcessManager* pman = electron->GetProcessManager();

  // get pointer to positron-nuclear process
  G4ElectronNuclearProcess* pnproc = new G4ElectronNuclearProcess("ElectroNuclear");
  
  // register positron-nuclear model
  G4ElectroVDNuclearModel* pnmodel = new G4ElectroVDNuclearModel();
  pnproc->RegisterMe(pnmodel);

  // add process to process manager
  pman->AddDiscreteProcess(pnproc);
}


void PositronPhysicsList1::SetCuts()
{
  SetCutValue(defaultCutValue, "gamma");
  SetCutValue(defaultCutValue, "e-");
  SetCutValue(defaultCutValue, "e+");
  SetCutValue(defaultCutValue, "proton");
}





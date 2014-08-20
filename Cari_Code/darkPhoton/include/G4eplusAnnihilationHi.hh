#ifndef G4eplusAnnihilationHi_h
#define G4eplusAnnihilationHi_h 1

#include "G4VEmProcess.hh"
#include "G4Positron.hh"
#include "G4VEmModel.hh"

class G4ParticleDefinition;

class G4eplusAnnihilationHi : public G4VEmProcess
{

public:

  G4eplusAnnihilationHi(const G4String& name = "annihil");

  virtual ~G4eplusAnnihilationHi();

  virtual G4bool IsApplicable(const G4ParticleDefinition& p);

  virtual G4VParticleChange* AtRestDoIt(
                             const G4Track& track,
                             const G4Step& stepData);

  virtual G4double AtRestGetPhysicalInteractionLength(
                             const G4Track& track,
                             G4ForceCondition* condition
                            );

  // Print out of the class parameters
  virtual void PrintInfo();

protected:

  virtual void InitialiseProcess(const G4ParticleDefinition*);

private:
  
  G4bool  isInitialised;
  const G4ParticleDefinition* theGamma;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

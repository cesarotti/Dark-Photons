
#ifndef G4eeToTwoGammaModel_h
#define G4eeToTwoGammaModel_h 1

#include "G4VEmModel.hh"

class G4ParticleChangeForGamma;

class G4eeToTwoGammaModel : public G4VEmModel
{

public:

  G4eeToTwoGammaModel(const G4ParticleDefinition* p = 0,
                      const G4String& nam = "eplus2gg");

  virtual ~G4eeToTwoGammaModel();

  virtual void Initialise(const G4ParticleDefinition*, const G4DataVector&);
  
  virtual G4double ComputeCrossSectionPerElectron(
                                const G4ParticleDefinition*,
                                      G4double kinEnergy, 
                                      G4double cutEnergy = 0.,
                                      G4double maxEnergy = DBL_MAX);
  
  virtual G4double ComputeCrossSectionPerAtom(
                                const G4ParticleDefinition*,
                                      G4double kinEnergy, 
                                      G4double Z, 
                                      G4double A = 0., 
                                      G4double cutEnergy = 0.,
                                      G4double maxEnergy = DBL_MAX);

  virtual G4double CrossSectionPerVolume(const G4Material*,
					 const G4ParticleDefinition*,
					 G4double kineticEnergy,
					 G4double cutEnergy,
					 G4double maxEnergy);

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
				 const G4MaterialCutsCouple*,
				 const G4DynamicParticle*,
				 G4double tmin,
				 G4double maxEnergy);

private:

  // hide assignment operator
  G4eeToTwoGammaModel & operator=(const  G4eeToTwoGammaModel &right);
  G4eeToTwoGammaModel(const  G4eeToTwoGammaModel&);

  G4double pi_rcl2;
  G4ParticleDefinition*  theGamma;
  G4ParticleChangeForGamma* fParticleChange;
  G4bool isInitialised;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

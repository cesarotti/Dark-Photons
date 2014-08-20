nclude "G4eeToTwoGammaModel.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4TrackStatus.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "Randomize.hh"
#include "G4ParticleChangeForGamma.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

G4eeToTwoGammaModel::G4eeToTwoGammaModel(const G4ParticleDefinition*,
                                         const G4String& nam)
  : G4VEmModel(nam),
    pi_rcl2(pi*classic_electr_radius*classic_electr_radius),
    isInitialised(false)
{
  theGamma = G4Gamma::Gamma();
  fParticleChange = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4eeToTwoGammaModel::~G4eeToTwoGammaModel()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4eeToTwoGammaModel::Initialise(const G4ParticleDefinition*,
                                     const G4DataVector&)
{
  if(isInitialised) { return; }
  fParticleChange = GetParticleChangeForGamma();
  isInitialised = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4eeToTwoGammaModel::ComputeCrossSectionPerElectron(
                                       const G4ParticleDefinition*,
                                       G4double kineticEnergy,
				       G4double, G4double)
{
  // Calculates the cross section per electron of annihilation into two photons
  // from the Heilter formula.

  G4double ekin  = std::max(eV,kineticEnergy);   

  G4double tau   = ekin/electron_mass_c2;
  G4double gam   = tau + 1.0;
  G4double gamma2= gam*gam;
  G4double bg2   = tau * (tau+2.0);
  G4double bg    = sqrt(bg2);

  G4double cross = pi_rcl2*((gamma2+4*gam+1.)*log(gam+bg) - (gam+3.)*bg)
                 / (bg2*(gam+1.));
  return cross;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4eeToTwoGammaModel::ComputeCrossSectionPerAtom(
                                    const G4ParticleDefinition* p,
                                    G4double kineticEnergy, G4double Z,
				    G4double, G4double, G4double)
{
  // Calculates the cross section per atom of annihilation into two photons
  
  G4double cross = Z*ComputeCrossSectionPerElectron(p,kineticEnergy);
  return cross;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double G4eeToTwoGammaModel::CrossSectionPerVolume(
					const G4Material* material,
					const G4ParticleDefinition* p,
					      G4double kineticEnergy,
					      G4double, G4double)
{
  // Calculates the cross section per volume of annihilation into two photons
  
  G4double eDensity = material->GetElectronDensity();
  G4double cross = eDensity*ComputeCrossSectionPerElectron(p,kineticEnergy);
  return cross;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4eeToTwoGammaModel::SampleSecondaries(vector<G4DynamicParticle*>* vdp,
					    const G4MaterialCutsCouple*,
					    const G4DynamicParticle* dp,
					    G4double,
					    G4double)
{
  G4double PositKinEnergy = dp->GetKineticEnergy();
  G4DynamicParticle *aGamma1, *aGamma2;
   
  // Case at rest
  if(PositKinEnergy == 0.0) {
    G4double cost = 2.*G4UniformRand()-1.;
    G4double sint = sqrt((1. - cost)*(1. + cost));
    G4double phi  = twopi * G4UniformRand();
    G4ThreeVector dir(sint*cos(phi), sint*sin(phi), cost);
    phi = twopi * G4UniformRand();
    G4ThreeVector pol(cos(phi), sin(phi), 0.0);
    pol.rotateUz(dir);
    aGamma1 = new G4DynamicParticle(theGamma, dir, electron_mass_c2);
    aGamma1->SetPolarization(pol.x(),pol.y(),pol.z());
    aGamma2 = new G4DynamicParticle(theGamma,-dir, electron_mass_c2);
    aGamma1->SetPolarization(-pol.x(),-pol.y(),-pol.z());

  } else {

    G4ThreeVector PositDirection = dp->GetMomentumDirection();

    G4double tau     = PositKinEnergy/electron_mass_c2;
    G4double gam     = tau + 1.0;
    G4double tau2    = tau + 2.0;
    G4double sqgrate = sqrt(tau/tau2)*0.5;
    G4double sqg2m1  = sqrt(tau*tau2);

    // limits of the energy sampling
    G4double epsilmin = 0.5 - sqgrate;
    G4double epsilmax = 0.5 + sqgrate;
    G4double epsilqot = epsilmax/epsilmin;

    //
    // sample the energy rate of the created gammas
    //
    G4double epsil, greject;

    do {
      epsil = epsilmin*pow(epsilqot,G4UniformRand());
      greject = 1. - epsil + (2.*gam*epsil-1.)/(epsil*tau2*tau2);
    } while( greject < G4UniformRand() );

    //
    // scattered Gamma angles. ( Z - axis along the parent positron)
    //

    G4double cost = (epsil*tau2-1.)/(epsil*sqg2m1);
    if(std::abs(cost) > 1.0) {
      G4cout << "### G4eeToTwoGammaModel WARNING cost= " << cost
	     << " positron Ekin(MeV)= " << PositKinEnergy
	     << " gamma epsil= " << epsil
	     << G4endl;
      if(cost > 1.0) cost = 1.0;
      else cost = -1.0; 
    }
    G4double sint = sqrt((1.+cost)*(1.-cost));
    G4double phi  = twopi * G4UniformRand();

    //
    // kinematic of the created pair
    //

    G4double TotalAvailableEnergy = PositKinEnergy + 2.0*electron_mass_c2;
    G4double Phot1Energy = epsil*TotalAvailableEnergy;

    G4ThreeVector Phot1Direction(sint*cos(phi), sint*sin(phi), cost);
    Phot1Direction.rotateUz(PositDirection);
    aGamma1 = new G4DynamicParticle (theGamma,Phot1Direction, Phot1Energy);
    phi = twopi * G4UniformRand();
    G4ThreeVector pol1(cos(phi), sin(phi), 0.0);
    pol1.rotateUz(Phot1Direction);
    aGamma1->SetPolarization(pol1.x(),pol1.y(),pol1.z());

    G4double Phot2Energy =(1.-epsil)*TotalAvailableEnergy;
    G4double PositP= sqrt(PositKinEnergy*(PositKinEnergy+2.*electron_mass_c2));
    G4ThreeVector dir = PositDirection*PositP - Phot1Direction*Phot1Energy;
    G4ThreeVector Phot2Direction = dir.unit();

    // create G4DynamicParticle object for the particle2
    aGamma2 = new G4DynamicParticle (theGamma,Phot2Direction, Phot2Energy);

    //!!! likely problematic direction to be checked
    aGamma2->SetPolarization(-pol1.x(),-pol1.y(),-pol1.z());
  }
  /*
    G4cout << "Annihilation in fly: e0= " << PositKinEnergy
    << " m= " << electron_mass_c2
    << " e1= " << Phot1Energy 
    << " e2= " << Phot2Energy << " dir= " <<  dir 
    << " -> " << Phot1Direction << " " 
    << Phot2Direction << G4endl;
  */
 
  vdp->push_back(aGamma1);
  vdp->push_back(aGamma2);
  fParticleChange->SetProposedKineticEnergy(0.);
  fParticleChange->ProposeTrackStatus(fStopAndKill);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

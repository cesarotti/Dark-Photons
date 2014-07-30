//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: DarkPhotonModel.cc 74581 2013-10-15 12:03:25Z gcosmo $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:   DarkPhotonModel
//
// Author:        Vladimir Ivanchenko on base of Michel Maire code
//
// Creation date: 02.08.2004
//
// Modifications:
// 08-04-05 Major optimisation of internal interfaces (V.Ivanchenko)
// 18-04-05 Compute CrossSectionPerVolume (V.Ivanchenko)
// 06-02-06 ComputeCrossSectionPerElectron, ComputeCrossSectionPerAtom (mma)
// 29-06-06 Fix problem for zero energy incident positron (V.Ivanchenko) 
// 20-10-06 Add theGamma as a member (V.Ivanchenko)
//
//
// Class Description:
//
// Implementation of e+ annihilation into 2 gamma
//
// The secondaries Gamma energies are sampled using the Heitler cross section.
//
// A modified version of the random number techniques of Butcher & Messel
// is used (Nuc Phys 20(1960),15).
//
// GEANT4 internal units.
//
// Note 1: The initial electron is assumed free and at rest.
//
// Note 2: The annihilation processes producing one or more than two photons are
//         ignored, as negligible compared to the two photons process.



//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "DarkPhotonModel.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4TrackStatus.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "Randomize.hh"
#include "G4ParticleChangeForGamma.hh"
#include "G4Log.hh"
#include "G4Exp.hh"

#ifndef M_a //Dark photon mass in MeV
#define M_a 50
#endif

#ifndef alpha_a //Dark photon coupling constant
#define alpha_a 0.050
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

DarkPhotonModel::DarkPhotonModel(const G4ParticleDefinition*,
                                         const G4String& nam)
  : G4VEmModel(nam),
    pi_rcl2(pi*classic_electr_radius*classic_electr_radius),
    isInitialised(false)
{
  theGamma = G4Gamma::Gamma();
  fParticleChange = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

DarkPhotonModel::~DarkPhotonModel()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void DarkPhotonModel::Initialise(const G4ParticleDefinition*,
                                     const G4DataVector&)
{
  if(isInitialised) { return; }
  fParticleChange = GetParticleChangeForGamma();
  isInitialised = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double DarkPhotonModel::ComputeCrossSectionPerElectron(
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
  //G4double multiplier = 10^6;


  G4double gp = kineticEnergy/electron_mass_c2;
  G4double r0 = 0.282;
  G4cout << "gp for calc xsec is: " << gp <<G4endl;

  G4double cross = pi_rcl2*((gamma2+4*gam+1.)*G4Log(gam+bg) - (gam+3.)*bg)
                 / (bg2*(gam+1.));

  /*G4double cross = - (((twopi/2.)*std::pow(r0,2)*(sqrt((-1 + gp)/(1 + gp))*(4*(1 + gp)*(3 + gp) + std::pow(M_a,4)) - 
         2*(4*gp*(4 + gp) + std::pow(-2 + std::pow(M_a,2),2))*atanh(sqrt((-1 + gp)/(1 + gp)))))/
     ((-1 + std::pow(gp,2))*(2 + 2*gp - std::pow(M_a,2))));
*/
  //pi_rcl2*((gamma2+4*gam+1.)*G4Log(gam+bg) - (gam+3.)*bg)
  //               / (bg2*(gam+1.));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double DarkPhotonModel::ComputeCrossSectionPerAtom(
                                    const G4ParticleDefinition* p,
                                    G4double kineticEnergy, G4double Z,
            G4double, G4double, G4double)
{
  // Calculates the cross section per atom of annihilation into two photons
  
  G4double cross = Z*ComputeCrossSectionPerElectron(p,kineticEnergy);
  return cross;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double DarkPhotonModel::CrossSectionPerVolume(
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

void DarkPhotonModel::SampleSecondaries(vector<G4DynamicParticle*>* vdp,
              const G4MaterialCutsCouple*,
              const G4DynamicParticle* dp,
              G4double,
              G4double)
{
  G4double PositKinEnergy = dp->GetKineticEnergy();
  G4DynamicParticle *aGamma1, *aGamma2;
   

  G4double gp = PositKinEnergy/dp->GetMass();
  G4cout << "Positron kinetic energy is " << PositKinEnergy << "MeV and mass is " << dp->GetMass() << "MeV!" << G4endl;
  G4double rand1 = G4UniformRand();
  G4double electron_mass = dp->GetMass();
  G4double sss = 2. * electron_mass * (electron_mass + dp->GetKineticEnergy());
  
  G4double beta = sqrt((gp - 1.)/(gp+1.));

  G4cout << "Random value is: " << rand1 << " and s is " << sss << " and gp is " << gp << " and beta is " << beta << G4endl;

 //G4double energy = -(5000 + 0.511) * (2500.26*(-1.+gp)*sqrt((-1.+gp)/(1. +gp))*(2. +2.*gp-1.*std::pow(M_a,2))*(-((1.*(-1.+std::pow(gp,2))*(1. +sqrt((-1.+gp)/(1. +gp)))*(1. -(0.5*std::pow(M_a,2))/(1. +gp)))/((-1.+gp)*sqrt((-1.+gp)/(1. +gp))*(2. +2.*gp-1.*std::pow(M_a,2)))+1.*rand1))/(-1.+std::pow(gp,2));
  G4double energy =  (-2500.2555*(-1. + gp)*sqrt((-1. + gp)/(1. + gp))*(2. + 2.*gp - 1.*std::pow(M_a,2))*
     ((-1.*(-1. + std::pow(gp,2))*(1. + sqrt((-1. + gp)/(1. + gp)))*
          (1. - (0.5*std::pow(M_a,2))/(1. + gp)))/
        ((-1. + gp)*sqrt((-1. + gp)/(1. + gp))*(2. + 2.*gp - 1.*std::pow(M_a,2))) + 1.*rand1))/
   (-1. + std::pow(gp,2));
  //G4double energy = -(5000 + 0.511) * (sqrt(-1.+gp)*(2.+2.*gp-std::pow(M_a,2.))*(-((std::pow((1.+gp),(3./2.))*(1.+sqrt((-1.+gp)/(1.+gp)))*(1-std::pow(M_a,2.)/(2.*(1.+gp))))/(sqrt(-1.+gp)*(2.+2.*gp-std::pow(M_a,2.))))+rand1))/(2.*std::pow((1.+gp),3./2.));

  G4double theta_lab = acos(  (energy - dp->GetMass() * (1.-(std::pow(M_a,2.)/sss)))  /(beta*energy));
  G4double theta_cm = (cos(theta_lab) - beta)/(1.-beta*cos(theta_lab));

   G4double sint = sqrt((1.+cos(theta_lab))*(1.-cos(theta_lab)));
    G4double phi  = twopi * G4UniformRand();

  //G4ThreeVector PositDirection = dp->GetMomentumDirection();
  G4ThreeVector Phot1Direction(sint*cos(phi), sint*sin(phi), cos(theta_lab));
  //Phot1Direction.rotateUz(PositDirection);

  G4cout << "New gamma from darkphoton process has properties: " <<  theta_lab << " angle from beampipe and " << energy << "energy!" << G4endl;
    aGamma1 = new G4DynamicParticle (theGamma,Phot1Direction, energy);
    phi = twopi * G4UniformRand();
    G4ThreeVector pol1(cos(phi), sin(phi), 0.0);
    pol1.rotateUz(Phot1Direction);
    aGamma1->SetPolarization(pol1.x(),pol1.y(),pol1.z());

      vdp->push_back(aGamma1);
  fParticleChange->SetProposedKineticEnergy(0.);
  fParticleChange->ProposeTrackStatus(fStopAndKill);



  /*
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
      epsil = epsilmin*G4Exp(G4Log(epsilqot)*G4UniformRand());
      greject = 1. - epsil + (2.*gam*epsil-1.)/(epsil*tau2*tau2);
    } while( greject < G4UniformRand() );

    //
    // scattered Gamma angles. ( Z - axis along the parent positron)
    //

    G4double cost = (epsil*tau2-1.)/(epsil*sqg2m1);
    if(std::abs(cost) > 1.0) {
      G4cout << "### DarkPhotonModel WARNING cost= " << cost
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
  /*
  vdp->push_back(aGamma1);
  vdp->push_back(aGamma2);
  fParticleChange->SetProposedKineticEnergy(0.);
  fParticleChange->ProposeTrackStatus(fStopAndKill);
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

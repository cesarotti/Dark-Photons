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
// $Id: AlwaysTwoGammaModel.cc 74581 2013-10-15 12:03:25Z gcosmo $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:   AlwaysTwoGammaModel
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

#include "AlwaysTwoGammaModel.hh"
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
#include "HepMCG4AsciiReader.hh"
#include "HepMCG4AsciiReaderMessenger.hh"
#include "G4LorentzVector.hh"
#include "G4Event.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4TransportationManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

AlwaysTwoGammaModel::AlwaysTwoGammaModel(const G4ParticleDefinition*,
                                         const G4String& nam)
  : G4VEmModel(nam),
    pi_rcl2(pi*classic_electr_radius*classic_electr_radius),
    isInitialised(false)
{
  G4cout << "Constructor AlwaysTwoGammaModel " << G4endl;
  fParticleChange = 0;
  name_of_model = &nam;
  G4cout << "Name is" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

AlwaysTwoGammaModel::~AlwaysTwoGammaModel()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void AlwaysTwoGammaModel::Initialise(const G4ParticleDefinition*,
                                     const G4DataVector&)
{
  if(isInitialised) { return; }
  G4cout << "Initializing AlwaysTwoGamma " << G4endl;

  asciiInput = new HepMC::IO_GenEvent(*name_of_model, std::ios::in);

  fParticleChange = GetParticleChangeForGamma();
  isInitialised = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double AlwaysTwoGammaModel::ComputeCrossSectionPerElectron(
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
  G4double multiplier = 10^6;

  G4double cross = 261e-28; // * 10^(−30);//* 10^(-28);

  G4double real_cross = pi_rcl2*((gamma2+4*gam+1.)*G4Log(gam+bg) - (gam+3.)*bg)
                 / (bg2*(gam+1.));
                 
  cout << "AlwaysTwoGammaModel REAL CROSS SECTION IS " << real_cross << G4endl;
  cout << "AlwaysTwoGammaModel CROSS SECTION IS " << cross << G4endl;
  return cross; //Changed this.  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double AlwaysTwoGammaModel::ComputeCrossSectionPerAtom(
                                    const G4ParticleDefinition* p,
                                    G4double kineticEnergy, G4double Z,
				    G4double, G4double, G4double)
{
  // Calculates the cross section per atom of annihilation into two photons
  
  G4double cross = Z*ComputeCrossSectionPerElectron(p,kineticEnergy);
  G4cout << "AlwaysTwoGammaModel2 CROSS SECTION IS " << cross << G4endl;
  return cross;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double AlwaysTwoGammaModel::CrossSectionPerVolume(
					const G4Material* material,
					const G4ParticleDefinition* p,
					      G4double kineticEnergy,
					      G4double, G4double)
{
  // Calculates the cross section per volume of annihilation into two photons
  
  G4double eDensity = material->GetElectronDensity();
  G4double cross = eDensity*ComputeCrossSectionPerElectron(p,kineticEnergy);
  G4cout << "AlwaysTwoGammaModel3 CROSS SECTION IS " << cross << G4endl;
  return cross;
}


G4bool AlwaysTwoGammaModel::CheckVertexInsideWorld(const G4ThreeVector& pos) const
{
  G4Navigator* navigator= G4TransportationManager::GetTransportationManager()
                                                 -> GetNavigatorForTracking();

  G4VPhysicalVolume* world= navigator-> GetWorldVolume();
  G4VSolid* solid= world-> GetLogicalVolume()-> GetSolid();
  EInside qinside= solid-> Inside(pos);

  if( qinside != kInside) return false;
  else return true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AlwaysTwoGammaModel::SampleSecondaries(vector<G4DynamicParticle*>* vdp,
					    const G4MaterialCutsCouple*,
					    const G4DynamicParticle* dp,
					    G4double,
					    G4double)
{

  G4cout << "PROCESSING A NEW EVENT!!!" << G4endl;

  HepMC::GenEvent* aevent= asciiInput-> read_next_event();


  for(HepMC::GenEvent::vertex_const_iterator vitr= aevent->vertices_begin();
      vitr != aevent->vertices_end(); ++vitr ) { // loop for vertex ...

G4cout << "Reading vertex" << G4endl;
    // real vertex?
    G4bool qvtx=false;
    for (HepMC::GenVertex::particle_iterator
           pitr= (*vitr)->particles_begin(HepMC::children);
         pitr != (*vitr)->particles_end(HepMC::children); ++pitr) {

      if (!(*pitr)->end_vertex() && (*pitr)->status()==1) {
        qvtx=true;
        break;
      }
    }
    if (!qvtx) continue;

    // check world boundary
    HepMC::FourVector pos= (*vitr)-> position();
    G4LorentzVector xvtx(pos.x(), pos.y(), pos.z(), pos.t());
    if (! CheckVertexInsideWorld(xvtx.vect()*mm)) continue;

    // create G4PrimaryVertex and associated G4PrimaryParticles

    G4double calorSpacing = 10*m; //distance from target to calorimeter
    G4double targetPos = -(.5*calorSpacing); //position of Z coordinate of target

    G4PrimaryVertex* g4vtx=
      new G4PrimaryVertex(xvtx.x()*mm, xvtx.y()*mm, targetPos + xvtx.z()*mm,
                          xvtx.t()*mm/c_light);

    for (HepMC::GenVertex::particle_iterator
           vpitr= (*vitr)->particles_begin(HepMC::children);
         vpitr != (*vitr)->particles_end(HepMC::children); ++vpitr) {

      if( (*vpitr)->status() != 1 ) continue;

      G4int pdgcode= (*vpitr)-> pdg_id();
      pos= (*vpitr)-> momentum();
      G4LorentzVector p(pos.px(), pos.py(), pos.pz(), pos.e());

  G4cout << "Values are " << pos.px() << "," << pos.py() << "," << pos.pz() << G4endl;
      
      //Apply the lorentz boost for the equivalent 35MeV COM frame
      p.boost(0, 0, 0.9998934147671376709550178210287064659912158297191052);

      G4DynamicParticle* g4dyn=
        new G4DynamicParticle(G4ParticleTable::GetParticleTable()->FindParticle(pdgcode), p);
//Inject it into our process
      vdp->push_back(g4dyn);
      
    }
  }


  fParticleChange->SetProposedKineticEnergy(0.);
  fParticleChange->ProposeTrackStatus(fStopAndKill);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

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
// $Id: AlwaysTwoGammaModel.hh 66241 2012-12-13 18:34:42Z gunter $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     AlwaysTwoGammaModel
//
// Author:        Vladimir Ivanchenko on base of Michel Maire code
//
// Creation date: 02.08.2004
//
// Modifications:
// 08-04-05 Major optimisation of internal interfaces (V.Ivanchenko)
// 18-04-05 Compute CrossSectionPerVolume (V.Ivanchenko)
// 06-02-06 ComputeCrossSectionPerElectron, ComputeCrossSectionPerAtom (mma)
// 20-10-06 Add theGamma as a member (V.Ivanchenko)
//
//
// Class Description:
//
// Implementation of e+ annihilation into 2 gamma

// -------------------------------------------------------------------
//

#ifndef AlwaysTwoGammaModel_h
#define AlwaysTwoGammaModel_h 1

#include "G4VEmModel.hh"
#include "HepMCG4AsciiReader.hh"
#include "HepMCG4AsciiReaderMessenger.hh"


class G4ParticleChangeForGamma;

class AlwaysTwoGammaModel : public G4VEmModel
{

public:

 

  AlwaysTwoGammaModel(const G4ParticleDefinition* p = 0,
                      const G4String& nam = "eplus2gg");

  virtual G4bool CheckVertexInsideWorld(const G4ThreeVector& pos) const;

  virtual ~AlwaysTwoGammaModel();

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
  AlwaysTwoGammaModel & operator=(const  AlwaysTwoGammaModel &right);
  AlwaysTwoGammaModel(const  AlwaysTwoGammaModel&);

  HepMC::IO_GenEvent* asciiInput;
  G4double count;

  const G4String* name_of_model;
  G4double pi_rcl2;
  G4ParticleDefinition*  theGamma;
  G4ParticleChangeForGamma* fParticleChange;
  G4bool isInitialised;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

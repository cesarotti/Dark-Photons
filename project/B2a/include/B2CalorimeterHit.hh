//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// $Id:$
//
// Jane Tinslay - adapted from A01 example
//
#ifndef B2CALORIMETERHIT_HH
#define B2CALORIMETERHIT_HH

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"
#include "G4Transform3D.hh"
#include "G4VHit.hh"

class G4AttDef;
class G4AttValue;

class B2CalorimeterHit : public G4VHit {

public:
  
  // Constructors
  B2CalorimeterHit();
  B2CalorimeterHit(G4int id);

  // Destructor
  virtual ~B2CalorimeterHit();
  
  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  // Methods
  virtual void Draw();

  virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
  virtual std::vector<G4AttValue>* CreateAttValues() const;

  virtual void Print();

  // Deposited energy
  inline void AddDepositedEnergy(G4double energy) {fDepositedEnergy += energy;}
  inline G4double GetDepositedEnergy() const {return fDepositedEnergy;}

  // Position vector
  inline void SetPosition(G4ThreeVector position) {fPosition = position;}
  inline G4ThreeVector GetPosition() const {return fPosition;}

  // Rotation matrix
  inline void SetRotation(G4RotationMatrix rotation) {fRotation = rotation;}
  inline G4RotationMatrix GetRotation() const {return fRotation;}

  // Logical volume
  inline void SetLogicalVolume(G4LogicalVolume* volume) {pLogicalVolume = volume;}
  inline const G4LogicalVolume* GetLogicalVolume() const {return pLogicalVolume;}
  
private:
  
  // Data members
  G4int fCellID;
  G4double fDepositedEnergy;
  G4ThreeVector fPosition;
  G4RotationMatrix fRotation;
  const G4LogicalVolume* pLogicalVolume;
  
};

typedef G4THitsCollection<B2CalorimeterHit> B2CalorimeterHitsCollection;

extern G4Allocator<B2CalorimeterHit> B2CalorimeterHitAllocator;

inline void* B2CalorimeterHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)B2CalorimeterHitAllocator.MallocSingle();
  return aHit;
}

inline void B2CalorimeterHit::operator delete(void* aHit)
{
  B2CalorimeterHitAllocator.FreeSingle((B2CalorimeterHit*) aHit);
}

#endif



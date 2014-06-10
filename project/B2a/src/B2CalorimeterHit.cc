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
// Tatsumi Koi   Minor modifications for McGill Univ.  tutorial
//
#include "B2CalorimeterHit.hh"
#include "B2PrimaryGeneratorAction.hh"
#include "G4AttDef.hh"
#include "G4AttDefStore.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
// HandsOn4: Draw box

#include "G4VVisManager.hh"

G4Allocator<B2CalorimeterHit> B2CalorimeterHitAllocator;

B2CalorimeterHit::B2CalorimeterHit()
  :fCellID(-1)
  ,fDepositedEnergy(0)
  ,fPosition()
  ,fRotation()
  ,pLogicalVolume(0)
{}

B2CalorimeterHit::B2CalorimeterHit(G4int id)
  :fCellID(id)
  ,fDepositedEnergy(0)
  ,fPosition()
  ,fRotation()
  ,pLogicalVolume(0)
{}

B2CalorimeterHit::~B2CalorimeterHit() {}

void B2CalorimeterHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  
  if(pVVisManager && (fDepositedEnergy>0.)) {

    // HandsOn4: Draw a box with depth propotional to the energy deposition
  }
}

const std::map<G4String,G4AttDef>* B2CalorimeterHit::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String,G4AttDef>* store 
    = G4AttDefStore::GetInstance("B2CalorimeterHit",isNew);
  
  if (isNew) {
    G4String HitType("HitType");
    (*store)[HitType] = G4AttDef(HitType,"Hit Type", "Bookkeeping", "", "G4String");
    
    G4String ID("ID");
    (*store)[ID] = G4AttDef(ID, "ID", "Bookkeeping", "", "G4int");

    G4String Column("Column");
    (*store)[Column] = G4AttDef(Column, "Column ID", "Bookkeeping", "", "G4int");

    G4String Row("Row");
    (*store)[Row] = G4AttDef(Row, "Row ID", "Bookkeeping", "", "G4int");

    G4String Energy("Energy");
    (*store)[Energy] = G4AttDef(Energy, "Energy Deposited", "Physics", 
				"G4BestUnit", "G4double");

    G4String Pos("Pos");
    (*store)[Pos] = G4AttDef(Pos, "Position", "Physics", "G4BestUnit", "G4ThreeVector");
    
    G4String LVol("LVol");
    (*store)[LVol] = G4AttDef(LVol, "Logical Volume", "Bookkeeping", "", "G4String");
  }
  
  return store;
}

std::vector<G4AttValue>* B2CalorimeterHit::CreateAttValues() const
{
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;
  
  values->push_back(G4AttValue("HitType", "CalorimeterHit", ""));
  
  values->push_back
    (G4AttValue("ID", G4UIcommand::ConvertToString(fCellID), ""));

  values->push_back(G4AttValue("Column", " ", ""));

  values->push_back(G4AttValue("Row", " ", ""));

  values->push_back(G4AttValue("Energy", G4BestUnit(fDepositedEnergy, "Energy"), ""));

  values->push_back(G4AttValue("Pos", G4BestUnit(fPosition,"Length"), ""));

  if (pLogicalVolume) values->push_back(G4AttValue("LVol", pLogicalVolume->GetName(), ""));
  else values->push_back(G4AttValue("LVol", " ", ""));
  
  return values;
}

void B2CalorimeterHit::Print()
{
  // G4cout << "  Cell[" << fCellID << "] " << fDepositedEnergy/MeV << " (MeV)" << G4endl;
}



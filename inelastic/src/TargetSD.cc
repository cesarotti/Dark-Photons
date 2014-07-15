/*
 * Dark Photon TargetSD
 *
 * History
 *     BYS 6.26.14 created
 */

#include "TargetSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4ParticleDefinition.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

TargetSD::TargetSD(const G4String& name,
               const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name), 
    fHitsCollection(NULL),
    thresholdEnergy(0.) // minimal energy to count as a hit
{
  collectionName.insert(hitsCollectionName);
  //!!!
  thresholdEnergy = 1.0*MeV;
}


TargetSD::~TargetSD()
{
}

void TargetSD::Initialize(G4HCofThisEvent* hce)
{
  //Create hits collection

  fHitsCollection = 
    new TargetHitsCollection(SensitiveDetectorName, collectionName[0]);

  //Add collection in hit collection of the event

  G4int hceID = 
    G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); 
  hce -> AddHitsCollection( hceID, fHitsCollection);

}

G4bool TargetSD::ProcessHits(G4Step* aStep, 
                  G4TouchableHistory* )
{
 
  //Only looks for certain particles
  G4int PDGID = aStep -> GetTrack()->GetDefinition()->GetPDGEncoding();


  //!!!
  //Defining energy threshold

  if(aStep->GetTrack()->GetTotalEnergy() < thresholdEnergy) return false;

  //checking for only certain particles

  if (PDGID==22) { // gamma

    TargetHit* newHit = new TargetHit();
    //enters fields in Target Hit class
    newHit->SetTrackID (aStep->GetTrack()->GetTrackID());
    newHit->SetTotalEnergy(aStep->GetTrack()->GetTotalEnergy()); 
    newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
    newHit->SetMomentum(aStep->GetTrack()->GetMomentum());
    newHit->SetPDGID(PDGID);
   
    fHitsCollection->insert(newHit);

  } else if (PDGID == -11) { // positron

    TargetHit* newHit = new TargetHit();
    //enters fields in Target Hit class
    newHit->SetTrackID (aStep->GetTrack()->GetTrackID());
    newHit->SetTotalEnergy(aStep->GetTrack()->GetTotalEnergy()); 
    newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
    newHit->SetMomentum(aStep->GetTrack()->GetMomentum());
    newHit->SetPDGID(PDGID);
   
    fHitsCollection->insert(newHit);

  } else if (PDGID == 211) { // positive pion

    TargetHit* newHit = new TargetHit();
    //enters fields in Target Hit class
    newHit->SetTrackID (aStep->GetTrack()->GetTrackID());
    newHit->SetTotalEnergy(aStep->GetTrack()->GetTotalEnergy()); 
    newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
    newHit->SetMomentum(aStep->GetTrack()->GetMomentum());
    newHit->SetPDGID(PDGID);
   
    fHitsCollection->insert(newHit);

  } else if (PDGID == -211) { // negative pion

    TargetHit* newHit = new TargetHit();
    //enters fields in Target Hit class
    newHit->SetTrackID (aStep->GetTrack()->GetTrackID());
    newHit->SetTotalEnergy(aStep->GetTrack()->GetTotalEnergy()); 
    newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
    newHit->SetMomentum(aStep->GetTrack()->GetMomentum());
    newHit->SetPDGID(PDGID);
   
    fHitsCollection->insert(newHit);

  } else if (PDGID == 111) { // pion zero

    TargetHit* newHit = new TargetHit();
    //enters fields in Target Hit class
    newHit->SetTrackID (aStep->GetTrack()->GetTrackID());
    newHit->SetTotalEnergy(aStep->GetTrack()->GetTotalEnergy()); 
    newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
    newHit->SetMomentum(aStep->GetTrack()->GetMomentum());
    newHit->SetPDGID(PDGID);
   
    fHitsCollection->insert(newHit);

  } else if (PDGID == 2212) { // proton

    TargetHit* newHit = new TargetHit();
    //enters fields in Target Hit class
    newHit->SetTrackID (aStep->GetTrack()->GetTrackID());
    newHit->SetTotalEnergy(aStep->GetTrack()->GetTotalEnergy()); 
    newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
    newHit->SetMomentum(aStep->GetTrack()->GetMomentum());
    newHit->SetPDGID(PDGID);
   
    fHitsCollection->insert(newHit);

  } else if (PDGID == 2112) { // neutron

    TargetHit* newHit = new TargetHit();
    //enters fields in Target Hit class
    newHit->SetTrackID (aStep->GetTrack()->GetTrackID());
    newHit->SetTotalEnergy(aStep->GetTrack()->GetTotalEnergy()); 
    newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
    newHit->SetMomentum(aStep->GetTrack()->GetMomentum());
    newHit->SetPDGID(PDGID);
   
    fHitsCollection->insert(newHit);

  }


  return true;
}

void TargetSD::EndOfEvent(G4HCofThisEvent*)
{
    G4int nofHits = fHitsCollection->entries();
    for (G4int i =0; i<nofHits; i++) (*fHitsCollection)[i]->Print();
}





/*
 * Dark Photon Target Hit
 *
 * History
 *     BYS 6.26.14 created
 */

#include "TargetHit.hh"

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<TargetHit>* TargetHitAllocator;

TargetHit::TargetHit()
  : G4VHit(),
    fTrackID(-1), 
    fTotalEnergy(0.),
    fPos(G4ThreeVector()), 
    fMomentum(G4ThreeVector()),
    fPDGID(0)
{}

TargetHit::~TargetHit()
{}

const TargetHit& TargetHit::operator=(const TargetHit& hit)
{
  fTrackID = hit.fTrackID;
  fTotalEnergy = hit.fTotalEnergy;
  fPos = hit.fPos;
  fMomentum = hit.fMomentum;
  fPDGID = hit.fPDGID;

  return *this;

}

G4int TargetHit::operator==(const TargetHit& hit) const
{
  return (this==&hit) ? 1 : 0;
}


void TargetHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
    {
      G4Circle circle(fPos);
      circle.SetScreenSize(4.);
      circle.SetFillStyle(G4Circle::filled);
      G4Colour colour(0.,1.0,0.);
      G4VisAttributes attribs(colour);
      circle.SetVisAttributes(attribs);
      pVVisManager->Draw(circle);
    }
}

void TargetHit::Print()
{
  G4cout << "Target hit registered. " << G4endl;
}





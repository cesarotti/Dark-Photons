/*
 * Dark Photon Calor Hit
 *
 *!!!History
 *   CJC 6.17.14 created
 */

#include "CalorHit.hh"

#include "G4THitsCollection.hh"
#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<CalorHit>* CalorimeterHitAllocator;

CalorHit::CalorHit()
  : G4VHit(),
    fTrackID(-1), 
    fTotalEnergy(0.),
    fPos(G4ThreeVector()), 
    fMomentum(G4ThreeVector())
{}

CalorHit::~CalorHit()
{}

//Create a hit that exactly mirrors another hit
CalorHit::CalorHit(const CalorHit& hit)
  : G4VHit()
{ 
  fTrackID = hit.fTrackID;
  fTotalEnergy = hit.fTotalEnergy;
  fPos = hit.fPos;
  fMomentum = hit.fMomentum;
}

const CalorHit& CalorHit::operator=(const CalorHit& hit)
{
  fTrackID = hit.fTrackID;
  fTotalEnergy = hit.fTotalEnergy;
  fPos = hit.fPos;
  fMomentum = hit.fMomentum;

  return *this;

}

G4int CalorHit::operator==(const CalorHit& hit) const
{
  return (this==&hit) ? 1 : 0;
}


void CalorHit::Draw()
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

void CalorHit::Print()
{
  G4cout << "Calorimeter hit registered. " << G4endl;
}





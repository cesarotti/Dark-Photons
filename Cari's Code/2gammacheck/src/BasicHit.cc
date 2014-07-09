/*
 * Dark Photon Calor Hit
 *
 *!!!History
 *   CJC 6.17.14 created
 */

#include "BasicHit.hh"

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

G4ThreadLocal G4Allocator<BasicHit>* BasicHitAllocator;

BasicHit::BasicHit()
  : G4VHit(),
    fTotalEnergy(0.)
{}

BasicHit::~BasicHit()
{}

BasicHit::BasicHit(const BasicHit& hit)
  : G4VHit()
{ 
  fTotalEnergy = hit.fTotalEnergy;
}

const BasicHit& BasicHit::operator=(const BasicHit& hit)
{
  fTotalEnergy = hit.fTotalEnergy;

  return *this;
}

G4int BasicHit::operator==(const BasicHit& hit) const
{
  return (this==&hit) ? 1 : 0;
}


void BasicHit::Draw()
{
  /*
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
  */
}

void BasicHit::Print()
{
}






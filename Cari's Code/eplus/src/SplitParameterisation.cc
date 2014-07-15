

#include "SplitParameterisation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

SplitParameterisation::SplitParameterisation()
  : G4VPVParameterisation(), 
    fRegion(0)
{}

SplitParameterisation::SplitParameterisation(G4int region)
  : G4VPVParameterisation(),
  fRegion(0)
{
    for (G4int copyNo=0;copyNo<308;copyNo++)
    {
        G4int column = copyNo / 14;
        G4int row = copyNo % 14;
        fXCell[copyNo] = (column-10)*5.*cm - 2.5*cm;
        fYCell[copyNo] = (row-6)*5*cm - 2.5*cm;
    }
    fRegion = region;
}

SplitParameterisation::~SplitParameterisation()
{}

void SplitParameterisation::ComputeTransformation
(const G4int copyNo,G4VPhysicalVolume *physVol) const
{
  G4int xShift = 7*5.0*cm;
  if (fXCell[copyNo] < 11)
    {
    physVol->SetTranslation(G4ThreeVector(fXCell[copyNo]-xShift,fYCell[copyNo],0.));
    }
  else 
    physVol->SetTranslation(G4ThreeVector(fXCell[copyNo]+xShift, fYCell[copyNo], 0.));
 

}

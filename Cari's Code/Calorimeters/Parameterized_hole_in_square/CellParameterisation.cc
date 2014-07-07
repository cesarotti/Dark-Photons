

#include "CellParameterisation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

CellParameterisation::CellParameterisation()
  : G4VPVParameterisation(), 
    fRegion(0)
{}

CellParameterisation::CellParameterisation(G4int region)
  : G4VPVParameterisation(),
  fRegion(0)
{
    for (G4int copyNo=0;copyNo<396;copyNo++)
    {
        G4int column = copyNo % 36;
        G4int row = copyNo / 36;
        fXCell[copyNo] = (column-17)*5.*cm - 2.5*cm;
        fYCell[copyNo] = (row-5)*5*cm - 2.5*cm;
    }
    fRegion = region;
}

CellParameterisation::~CellParameterisation()
{}

void CellParameterisation::ComputeTransformation
(const G4int copyNo,G4VPhysicalVolume *physVol) const
{
    physVol->SetTranslation(G4ThreeVector(fXCell[copyNo],fYCell[copyNo],0.));

}

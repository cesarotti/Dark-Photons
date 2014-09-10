
#include "SquareParameterisation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

SquareParameterisation::SquareParameterisation()
  : G4VPVParameterisation()
{}

SquareParameterisation::SquareParameterisation(G4int square)
  : G4VPVParameterisation(), 
    fSquare(square)
{  
  for (G4int copyNo = 0; copyNo<25; copyNo++)
    {
      G4int col = copyNo%5; 
      G4int row = copyNo/5; 
      fXCell[copyNo] = (col-2)*5.0*cm;
      fYCell[copyNo] = (row-2)*5.0*cm;
    }
}

SquareParameterisation::~SquareParameterisation()
{}

void SquareParameterisation::ComputeTransformation
(const G4int copyNo, G4VPhysicalVolume *physVol) const
{
  physVol->SetTranslation(G4ThreeVector(fXCell[copyNo], fYCell[copyNo], 0.));
} 


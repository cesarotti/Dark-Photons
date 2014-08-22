#ifndef SquareParameterisation_h
#define SquareParameterisation_h 1

#include "globals.hh"
#include "G4VPVParameterisation.hh"

class G4VPhysicalVolume;
class G4Box;

// No warnings plz
class G4Trd;
class G4Trap;
class G4Cons;
class G4Orb;
class G4Sphere;
class G4Ellipsoid;
class G4Torus;
class G4Para;
class G4Hype;
class G4Tubs; 
class G4Polycone;
class G4Polyhedra;

class SquareParameterisation : public G4VPVParameterisation
{ 
  public:
    SquareParameterisation();
    SquareParameterisation(G4int square); //odds only 

    virtual ~SquareParameterisation();
   
    void ComputeTransformation (const G4int copyNo,
                                G4VPhysicalVolume* physVol) const;
    
  private:  // Dummy declarations to get rid of warnings ...

    void ComputeDimensions (G4Box&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Trd&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Trap&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Cons&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Sphere&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Orb&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Ellipsoid&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Torus&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Para&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Hype&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Polycone&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Polyhedra&,const G4int,
                            const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Tubs&, const G4int, 
			    const G4VPhysicalVolume*) const {}

  private:

  G4int    fSquare;
  G4double fXCell[25]; 
  G4double fYCell[25];
};

#endif

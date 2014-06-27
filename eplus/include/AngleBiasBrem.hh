#ifndef AngleBiasBrem_h
#define AngleBiasBrem_h 1

#include "G4ParticleChangeForLoss.hh"
#include "G4WrapperProcess.hh"
class G4VParticleChange;

class AngleBiasBrem : public G4WrapperProcess{
public:
	AngleBiasBrem();
	virtual ~AngleBiasBrem();
	G4VParticleChange* PostStepDoIt(const G4Track& track, const G4Step& step);

};

#endif

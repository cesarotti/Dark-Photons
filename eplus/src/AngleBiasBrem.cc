#include "AngleBiasBrem.hh"

AngleBiasBrem::AngleBiasBrem()
{
	  G4cout << __LINE__ << G4endl; 

}

AngleBiasBrem::~AngleBiasBrem()
{
	  G4cout << __LINE__ << G4endl; 

}

G4VParticleChange* AngleBiasBrem::PostStepDoIt(const G4Track& track, const G4Step& step){
	G4int lowerLimitAngle = 3.0;
	G4int upperLimitAngle = 4.0;

	lowerLimitAngle = lowerLimitAngle/180*3.14159265359;
	upperLimitAngle = upperLimitAngle/180*3.14159265359;

	while (1) {

		G4VParticleChange* temp = pRegProcess->PostStepDoIt(track, step);
		G4int nsec= temp->GetNumberOfSecondaries();
		G4double angle=0;

		for (G4int i=0; i<nsec; i++){
			G4Track* particle = temp->GetSecondary(i);
			G4int type = particle->GetDynamicParticle()->GetPDGcode();
			if (type==22) angle = particle->GetMomentumDirection().theta();
		}

		if ((angle>lowerLimitAngle) && (angle<upperLimitAngle)) {
			pParticleChange=temp;
			return pParticleChange;
		} //else temp->~G4VParticleChange();

	}	
	
}

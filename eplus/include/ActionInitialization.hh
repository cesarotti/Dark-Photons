/*
 *Dark Photon Action Initialization
 *!!!History
 *  CJC 6.15.14 created
 */

#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "DetectorConstruction.hh"
 
class DetectorConstruction;

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(DetectorConstruction *detConstruction);
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
  private:
    DetectorConstruction* fDetConstruction;
};

#endif

    

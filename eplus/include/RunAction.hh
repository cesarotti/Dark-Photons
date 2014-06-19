/*
 * Dark Photon Run Action
 * !!!History
 *    CJC 6.14.15 created
 *    
 */

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class RunAction : public G4UserRunAction
{
public:
  RunAction();
  virtual ~RunAction();

  virtual G4Run* GenerateRun();
  virtual void BeginOfRunAction(const G4Run* run);
  virtual void EndOfRunAction(const G4Run* run);
  
};


#endif


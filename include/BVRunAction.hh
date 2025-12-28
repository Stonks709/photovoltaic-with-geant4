#ifndef BVRunAction_h
#define BVRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class BVRunAction : public G4UserRunAction
{
  public:
    BVRunAction();
    virtual ~BVRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
};

#endif
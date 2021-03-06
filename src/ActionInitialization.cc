#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction);
}

void ActionInitialization::Build() const
{
  DetectorConstruction* det = new DetectorConstruction;
  PrimaryGeneratorAction* prim = new PrimaryGeneratorAction;
  RunAction* runAction = new RunAction;
  

  SetUserAction(new PrimaryGeneratorAction);

  //RunAction* runAction = new RunAction;
  SetUserAction(runAction);

  EventAction* eventAction = new EventAction();
  SetUserAction(eventAction);

  SetUserAction(new SteppingAction(det,prim,eventAction));
}  

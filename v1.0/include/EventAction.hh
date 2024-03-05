#ifndef EventAction_h
#define EventAction_h 1

#define  NOTRIGGER 0
#define  PINTRIGGER 1

#include "G4UserEventAction.hh"
#include "TrackerIonSD.hh"
#include "TrackerPINSD.hh"
#include "TrackerPINHit.hh"
#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"
#include "Results.hh"
#include <sys/time.h>
#include "RunAction.hh"

class EventAction : public G4UserEventAction
{
  public:
  EventAction(Results*,RunAction*,Projectile*,Recoil*);
   ~EventAction();

  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);
  
  void SetNoTrigger(){setTrigger=NOTRIGGER;printf("Trigger set to NONE\n");};
  void SetTriggerPIN(){setTrigger=PINTRIGGER;printf("Trigger set to PIN\n");};
  
  private:
  Results* results;
  G4int ionCollectionID;
  G4int PINCollectionID;
  timeval tc,ts,td;
  double t,rate;
  RunAction* run_action;
  

  Projectile* theProjectile;
  Recoil*     theRecoil;
  
  G4int eventTrigger,setTrigger;
};

#endif //EVENTACTION_H

    

#include "EventAction_Messenger.hh"

EventAction_Messenger::EventAction_Messenger(EventAction* EvtAct)
:aEventAction(EvtAct)
{ 
  EventActionDir = new G4UIdirectory("/Trigger/");
  EventActionDir->SetGuidance("Trigger control.");

  TPCmd = new G4UIcmdWithoutParameter("/Trigger/PIN",this);
  TPCmd->SetGuidance("Select PIN trigger");  

  TNCmd = new G4UIcmdWithoutParameter("/Trigger/NONE",this);
  TNCmd->SetGuidance("Select no trigger");
}
EventAction_Messenger::~EventAction_Messenger()
{

  delete EventActionDir;
  delete TPCmd;
  delete TNCmd;
 
}

void EventAction_Messenger::SetNewValue(G4UIcommand* command,G4String)
{ 
 
  if( command == TPCmd )
    { aEventAction->SetTriggerPIN();}

  if( command == TNCmd )
    { aEventAction->SetNoTrigger();}

}

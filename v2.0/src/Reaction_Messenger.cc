#include "Reaction_Messenger.hh"


Reaction_Messenger::Reaction_Messenger(Reaction* React)
  :aReaction(React)
{ 
  ReactionDir = new G4UIdirectory("/Reaction/");
  ReactionDir->SetGuidance("Reaction control.");

  SetBmaxCmd = new G4UIcmdWithADouble("/Reaction/SetBmax",this);
  SetBmaxCmd->SetGuidance("Set the value for the maximum impact parameter in fm.");
  SetBmaxCmd->SetParameterName("choice",false);
  SetBmaxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  RepBmaxCmd = new G4UIcmdWithoutParameter("/Reaction/ReportBmax",this);
  RepBmaxCmd->SetGuidance("Report the value for the maximum impact parameter in fm");  
}



Reaction_Messenger::~Reaction_Messenger()
{
  delete SetBmaxCmd;
  delete RepBmaxCmd;
}


void Reaction_Messenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
 if( command == SetBmaxCmd )
   { aReaction->SetBmax(SetBmaxCmd->GetNewDoubleValue(newValue));} 

  if( command == RepBmaxCmd )
   { aReaction->ReportBmax();}
}


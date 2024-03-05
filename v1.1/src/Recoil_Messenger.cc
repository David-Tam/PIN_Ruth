#include "Recoil_Messenger.hh"


Recoil_Messenger::Recoil_Messenger(Recoil* Rec)
:theRecoil(Rec)
{ 
  RecoilDir = new G4UIdirectory("/Recoil/");
  RecoilDir->SetGuidance("Recoil control.");
 

  RACmd = new G4UIcmdWithAnInteger("/Recoil/A",this);
  RACmd->SetGuidance("Set the mass number for the recoil");
  RACmd->SetParameterName("Recoil A",false);
  RACmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  RZCmd = new G4UIcmdWithAnInteger("/Recoil/Z",this);
  RZCmd->SetGuidance("Set the atomic number for the recoil");
  RZCmd->SetParameterName("Recoil Z",false);
  RZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

 
  RRepCmd = new G4UIcmdWithoutParameter("/Recoil/Report",this);
  RRepCmd->SetGuidance("Report recoil parameters");  

}



Recoil_Messenger::~Recoil_Messenger()
{
 

  delete RACmd;
  delete RZCmd; 
  delete RRepCmd;

}


void Recoil_Messenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
 
  if( command == RACmd )
   { theRecoil->SetRecoilA(RACmd->GetNewIntValue(newValue));} 

 if( command == RZCmd )
   { theRecoil->SetRecoilZ(RZCmd->GetNewIntValue(newValue));} 

 if( command == RRepCmd )
   { theRecoil->Report();}
}


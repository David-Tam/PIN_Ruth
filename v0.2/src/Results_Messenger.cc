#include "Results_Messenger.hh"


Results_Messenger::Results_Messenger(Results* Res) :results(Res) 
{ 

  ResDir = new G4UIdirectory("/Results/");
  ResDir->SetGuidance("Output control.");
 
  TreDir = new G4UIdirectory("/Results/Tree/");
  TreDir->SetGuidance("Root tree control.");

  ProjDir = new G4UIdirectory("/Results/Projectile/");
  ProjDir->SetGuidance("Projectile spectra");
 
  RecDir = new G4UIdirectory("/Results/Recoil/");
  RecDir->SetGuidance("Recoil spectra");

  CSDir = new G4UIdirectory("/Results/CrossSection/");
  CSDir->SetGuidance("CrossSection spectra");

  GDir = new G4UIdirectory("/Results/Gamma/");
  GDir->SetGuidance("Gamma-ray spectra");

  PDir = new G4UIdirectory("/Results/PIN/");
  PDir->SetGuidance("PIN diode spectra");

  GRDir = new G4UIdirectory("/Results/Groups/");
  GRDir->SetGuidance("Gamma-ray - PIN detector grouping");

  TRepCmd = new G4UIcmdWithoutParameter("/Results/Tree/Report",this);
  TRepCmd->SetGuidance("Report parameters for the Root tree"); 

  TSCmd = new G4UIcmdWithAString("/Results/Tree/Save",this);
  TSCmd->SetGuidance("Save the ROOT tree of simulated parameters");
  TSCmd->SetParameterName("file name",false);
  TSCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  TSCmd->SetDefaultValue("test.root");


  PCSPCmd = new G4UIcmdWithoutParameter("/Results/Groups/CalculatePINWallPositions",this);
  PCSPCmd->SetGuidance("Calculates position of PIN wall diodes");

  PRSPCmd = new G4UIcmdWithoutParameter("/Results/Groups/ReportPINWallPositions",this);
  PRSPCmd->SetGuidance("Reports position of PIN wall diodes");

 
}

Results_Messenger::~Results_Messenger()
{

  // this is for the directories
  delete ResDir;
  delete TreDir;
  delete ProjDir;
  delete RecDir;
  delete CSDir;
  delete GDir;
  delete PDir;
  delete GRDir;
  // this is for the tree access
  delete TRepCmd;
  delete TSCmd;
 

  delete PCSPCmd;
  delete PRSPCmd;
 
}



void Results_Messenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 

  // this is for the tree access
  if( command == TRepCmd )
    { results->TreeReport();}
 
  if( command == TSCmd )
    { results->TreeSave(newValue);}

  if( command == PCSPCmd )
    { results->GetPINWallPositions();}

  if( command == PRSPCmd )
    { results->ReportPINWallPositions();}

 


}

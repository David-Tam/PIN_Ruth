#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction()
{
 // use G4-NIST materials data base
  //
  G4NistManager* man = G4NistManager::Instance();
  man->FindOrBuildMaterial("G4_Galactic");
  man->FindOrBuildMaterial("G4_Pb");
  man->FindOrBuildMaterial("G4_lAr");
  man->FindOrBuildMaterial("G4_STAINLESS-STEEL");  
  man->FindOrBuildMaterial("G4_Al");
  man->FindOrBuildMaterial("G4_POLYETHYLENE");
  man->FindOrBuildMaterial("G4_RUBBER_NEOPRENE");
  man->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  man->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  man->FindOrBuildMaterial("G4_BGO");
  man->FindOrBuildMaterial("G4_CESIUM_IODIDE");
  man->FindOrBuildMaterial("G4_Ge");
  man->FindOrBuildMaterial("G4_Cu");
  man->FindOrBuildMaterial("G4_AIR");
  // parameters to suppress:
  
  
}

DetectorConstruction::~DetectorConstruction()
{
  delete ExperimentalHallMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  Materials* materials=new Materials();
  
  //Experimental Hall
  Experimental_Hall* ExperimentalHall = new Experimental_Hall(materials);
  ExpHall_phys=ExperimentalHall->Construct();
  ExpHall_log=ExperimentalHall->GetLogVolume();
  //  ExperimentalHall->Report();
  ExperimentalHallMessenger = new Experimental_Hall_Messenger(ExperimentalHall);
  
  theTarget = new Target(ExpHall_log,materials);
  theTarget->Construct();
  //  theTarget->Report();
  TargetMessenger = new Target_Messenger(theTarget);
  
  aPIN_array = new PIN_array(ExpHall_log,materials);
  aPIN_array->Construct();
  //  aPIN_array->Report();
  
  theChamber = new Chamber(ExpHall_log,materials);
  theChamber->Construct();
  //  theChamber->Report();
  ChamberMessenger = new Chamber_Messenger(theChamber); 
  
  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 
  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  
  //------------------------------------------------
  // Detectors sensitive for ion tracking
  //------------------------------------------------
  TrackerIon = new TrackerIonSD("IonTracker");
  TrackerIonSDMessenger = new TrackerIonSD_Messenger(TrackerIon);
  SDman->AddNewDetector( TrackerIon );
  
  theTarget->GetTargetLog()->SetSensitiveDetector(TrackerIon);
  theTarget->GetBackingLog()->SetSensitiveDetector(TrackerIon);
  ExpHall_log->SetSensitiveDetector(TrackerIon);
  
  TrackerPIN = new TrackerPINSD("PINTracker");
  SDman->AddNewDetector( TrackerPIN );
  aPIN_array->MakeSensitive(TrackerPIN);  
  
  return ExpHall_phys;
}
/*====================================================================*/
void DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

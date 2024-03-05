#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

#include "G4AssemblyVolume.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4SDManager.hh"
#include "Materials.hh"
#include "Experimental_Hall.hh"
#include "Experimental_Hall_Messenger.hh"
#include "Target.hh"
#include "Target_Messenger.hh"
#include "TrackerIonSD.hh"
#include "TrackerIonSD_Messenger.hh"
#include "PIN_array.hh"
#include "TrackerPINSD.hh"
#include "G4NistManager.hh"
#include "Chamber.hh"
#include "Chamber_Messenger.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();
    ~DetectorConstruction();

    G4VPhysicalVolume* Construct();
 
  Target* GetTarget(){return theTarget;};
  PIN_array* GetPINWall(){return aPIN_array;};
  
  private:
  
  G4LogicalVolume* ExpHall_log;
  G4VPhysicalVolume* ExpHall_phys;
  Target* theTarget;
  PIN_array *aPIN_array; 
  TrackerPINSD* TrackerPIN;
  Chamber* theChamber;
 
  Experimental_Hall_Messenger* ExperimentalHallMessenger;
  Chamber_Messenger* ChamberMessenger;
  Target_Messenger*    TargetMessenger;

  TrackerIonSD* TrackerIon;
  TrackerIonSD_Messenger* TrackerIonSDMessenger;

  void UpdateGeometry();
};

#endif


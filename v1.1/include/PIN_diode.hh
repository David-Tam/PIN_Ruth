#ifndef PIN_diode_H
#define PIN_diode_H 1

#include "G4Material.hh"
#include "Materials.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4UserLimits.hh"

class PIN_diode 
{
  public:

  G4LogicalVolume *expHall_log;

  PIN_diode(G4LogicalVolume*,Materials*);
  ~PIN_diode();
  
  G4VPhysicalVolume *Construct();
  void setId(G4int i){Id=i;};
  void setRingId(G4int i){RingId=i;};
  void setName();
  void Report();
  void Place(G4ThreeVector);
  G4LogicalVolume* GetLog(){return PIN_diode_log;};
 
  private:
  // dimensions
  G4double PIN_diode_side_x;
  G4double PIN_diode_side_y;
  G4double PIN_diode_dl_side_x;
  G4double PIN_diode_dl_side_y;
  G4double PIN_diode_thickness;
  G4double PIN_diode_dead_thickness;

  //materials
  Materials* materials;
  G4Material* PIN_Material;

  //default position
  G4RotationMatrix NoRot;
  G4ThreeVector *Pos;

  G4Box* aPIN_diode;
  G4Box* aPIN_diode_dead_layer;
  
  //logical volume
  G4LogicalVolume* PIN_diode_log;
  G4LogicalVolume* PIN_diode_dl_log;
  //physical volume
  G4VPhysicalVolume* PIN_diode_phys;
  G4VPhysicalVolume* PIN_diode_dl_phys;
  G4int Id,RingId;
};

#endif


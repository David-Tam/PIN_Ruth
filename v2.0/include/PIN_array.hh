#ifndef PIN_array_H
#define PIN_array_H 1

#define NPIN 44
#include "G4Material.hh"
#include "Materials.hh"
#include "G4ThreeVector.hh"
#include "PIN_diode.hh"
#include "globals.hh"
#include "TrackerPINSD.hh"
#include "TrackerIonSD.hh"
#include <iostream>
#include <vector>
using namespace std;


class PIN_array 
{
  public:

  G4LogicalVolume *expHall_log;
  
  PIN_array(G4LogicalVolume*,Materials*);
  ~PIN_array();
  
  void Construct();
  void Report();
  void MakeSensitive(TrackerPINSD*);
  void setZPos(G4double z){zpos=z;Construct();};
  G4double GetZPos(){return zpos;};
  G4double GetXPos(G4int id){return pos[id-1].getX();}; //detector position = id-1
  G4double GetYPos(G4int id){return pos[id-1].getY();};
  G4int getRingID(G4int i){return ring[i];};
    private:
 
  vector<PIN_diode*> aPIN_array;
  //materials
  Materials* materials;
  G4int id[NPIN];
  G4int ring[NPIN];
  G4ThreeVector pos[NPIN];
  G4double zpos;
 

};

#endif


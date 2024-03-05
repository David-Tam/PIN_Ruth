#include "PIN_diode.hh"

PIN_diode::PIN_diode(G4LogicalVolume* experimentalHall_log,Materials* mat)
{
  materials=mat;
  expHall_log=experimentalHall_log;
  PIN_diode_side_x=8.84*mm;
  PIN_diode_side_y=9.78*mm;
  PIN_diode_dl_side_x= PIN_diode_side_x+2.*mm;
  PIN_diode_dl_side_y= PIN_diode_side_y+2.*mm;
  PIN_diode_thickness=0.5*mm;
  PIN_diode_dead_thickness=0.001*mm;
  PIN_Material = materials->FindMaterial("Silicon");
  Pos = new G4ThreeVector(0.,0.,0.);
}

PIN_diode::~PIN_diode()
{;}
//-----------------------------------------------------------------------------
G4VPhysicalVolume* PIN_diode::Construct()
{
 

 
  aPIN_diode = new G4Box("PIN",PIN_diode_side_x/2.,PIN_diode_side_y/2.,PIN_diode_thickness/2.);

  PIN_diode_log = new G4LogicalVolume(aPIN_diode,PIN_Material,"PIN_log",0,0,0);
 
  PIN_diode_phys = new G4PVPlacement(G4Transform3D(NoRot,*Pos),PIN_diode_log,"PIN",expHall_log,false,0);

  aPIN_diode_dead_layer = new G4Box("DL",PIN_diode_dl_side_x/2.,PIN_diode_dl_side_y/2.,PIN_diode_dead_thickness/2.);

  PIN_diode_dl_log = new G4LogicalVolume(aPIN_diode_dead_layer,PIN_Material,"DL_log",0,0,0);
  G4UserLimits* limits=new G4UserLimits();
  limits->SetMaxAllowedStep(PIN_diode_dead_thickness/50.);
  PIN_diode_dl_log->SetUserLimits(limits);
  
  Pos->setZ(Pos->getZ()-PIN_diode_thickness/2.-PIN_diode_dead_thickness/2.*(1+0.001));
  PIN_diode_dl_phys = new G4PVPlacement(G4Transform3D(NoRot,*Pos),PIN_diode_dl_log,"DL",expHall_log,false,0);
  
  G4Colour red (1.0,1.0, 0.0); 
  G4VisAttributes* Vis_6 = new G4VisAttributes(red);
  Vis_6->SetVisibility(true);
  Vis_6->SetForceSolid(true);

  G4Colour blue (0.0,1.0, 0.0); 
  G4VisAttributes* Vis_5 = new G4VisAttributes(blue);
  Vis_5->SetVisibility(true);
  Vis_5->SetForceSolid(true);
  
  PIN_diode_log->SetVisAttributes(Vis_6);
  PIN_diode_dl_log->SetVisAttributes(Vis_5);
    
  return PIN_diode_phys;
}


//-----------------------------------------------------------------------------
void PIN_diode::Report()
{
  G4ThreeVector pos;
  
  G4cout<<"----> PIN_diode material set to     "<<PIN_diode_log->GetMaterial()->GetName()<< G4endl;   
  G4cout<<"----> PIN_diode side X is set to "<<G4BestUnit(2.*aPIN_diode->GetXHalfLength(),"Length")<<G4endl;
  G4cout<<"----> PIN_diode side Y is set to "<<G4BestUnit(2.*aPIN_diode->GetYHalfLength(),"Length")<<G4endl;  
  G4cout<<"----> PIN_diode thickness is set to "<<G4BestUnit(2.*aPIN_diode->GetZHalfLength(),"Length")<<G4endl;
  G4cout<<"----> PIN_diode dead layer thickness is set to "<<G4BestUnit(2.*aPIN_diode_dead_layer->GetZHalfLength(),"Length")<<G4endl;

 
}

//-----------------------------------------------------------------------------
void PIN_diode::Place(G4ThreeVector shift)
{
  PIN_diode_phys->SetTranslation(shift);
  shift.setZ(shift.getZ()-PIN_diode_thickness/2.-PIN_diode_dead_thickness/2.*(1.+0.001));
  PIN_diode_dl_phys->SetTranslation(shift);
  G4RunManager::GetRunManager()->GeometryHasBeenModified();  
}
//-----------------------------------------------------------------------------
void PIN_diode::setName()
{
  char s[20];
  sprintf(s,"PIN %3d %1d",Id,RingId);
  PIN_diode_phys->SetName(s);
  sprintf(s,"PIN DL %3d %1d",Id,RingId);
  PIN_diode_dl_phys->SetName(s);
}
//---------------------------------------------------------------------                                                                                                                                                          

#include "Recoil.hh"
#include "G4ProcessManager.hh"

Recoil::Recoil()
{
  RecoilA=12;
  RecoilZ=6;

}

Recoil::~Recoil()
{
  ;
}

//-----------------------------------------------------------------------------
void Recoil::Report()
{
  G4cout<<"----> Recoil A set to              "<<RecoilA<< G4endl; 
  G4cout<<"----> Recoil Z set to              "<<RecoilZ<< G4endl; 
 
}



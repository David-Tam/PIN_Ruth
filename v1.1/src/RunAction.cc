#include "RunAction.hh"

 
RunAction::RunAction(PhysicsList* PL,Results* Res,DetectorConstruction* Det): thePhysicsList(PL), theResults(Res), theDetector(Det)
{
  
}


RunAction::~RunAction()
{

}

void RunAction::BeginOfRunAction(const G4Run*)
{

  // check to see if its grabbing custom stopping data
  // thePhysicsList->getStoppingData()->DumpMap();
  // G4cout << "For NIST materials" << G4endl;
  // G4int Zp = 13;
  // G4double E = 1.66666666667;
  // G4String mat = "G4_Al";
  // G4cout << Zp << " in " << mat << " stopping at " << E << " MeV/A: " 
  // 	 << (thePhysicsList->getStoppingData()->GetDEDX(1.66666666667,Zp,mat)) / (MeV*cm2/mg) 
  // 	 << " MeV*cm2/mg" << G4endl;
  // mat = "G4_Cu";
  // G4cout << Zp << " in " << mat << " stopping at " << E << " MeV/A: " 
  // 	 << (thePhysicsList->getStoppingData()->GetDEDX(1.66666666667,Zp,mat)) / (MeV*cm2/mg) 
  // 	 << " MeV*cm2/mg" << G4endl;
  // mat = "G4_MYLAR";
  // G4cout << Zp << " in " << mat << " stopping at " << E << " MeV/A: " 
  // 	 << (thePhysicsList->getStoppingData()->GetDEDX(1.66666666667,Zp,mat)) / (MeV*cm2/mg) 
  // 	 << " MeV*cm2/mg" << G4endl;
  // mat = "G4_CESIUM_IODIDE";
  // G4cout << Zp << " in " << mat << " stopping at " << E << " MeV/A: " 
  // 	 << (thePhysicsList->getStoppingData()->GetDEDX(1.66666666667,Zp,mat)) / (MeV*cm2/mg) 
  // 	 << " MeV*cm2/mg" << G4endl;
  // Zp = 36;
  // mat = "G4_Al";
  // G4cout << Zp << " in " << mat << " stopping at " << E << " MeV/A: " 
  // 	 << (thePhysicsList->getStoppingData()->GetDEDX(1.66666666667,Zp,mat)) / (MeV*cm2/mg) 
  // 	 << " MeV*cm2/mg" << G4endl;
  // mat = "G4_Cu";
  // G4cout << Zp << " in " << mat << " stopping at " << E << " MeV/A: " 
  // 	 << (thePhysicsList->getStoppingData()->GetDEDX(1.66666666667,Zp,mat)) / (MeV*cm2/mg) 
  // 	 << " MeV*cm2/mg" << G4endl;
  // mat = "G4_MYLAR";
  // G4cout << Zp << " in " << mat << " stopping at " << E << " MeV/A: " 
  // 	 << (thePhysicsList->getStoppingData()->GetDEDX(1.66666666667,Zp,mat)) / (MeV*cm2/mg) 
  // 	 << " MeV*cm2/mg" << G4endl;
  // mat = "G4_CESIUM_IODIDE";
  // G4cout << Zp << " in " << mat << " stopping at " << E << " MeV/A: " 
  // 	 << (thePhysicsList->getStoppingData()->GetDEDX(1.66666666667,Zp,mat)) / (MeV*cm2/mg) 
  // 	 << " MeV*cm2/mg" << G4endl;  

  G4cout<<" Beginning of run "<<G4endl;
  gettimeofday(&ts,NULL);

  thePhysicsList->getReaction()->TargetFaceCrossSection();
  recoilA=thePhysicsList->getReaction()->getTargetRecoilA();
  recoilZ=thePhysicsList->getReaction()->getTargetRecoilZ();
  theResults->SetupRun();
}


 
void RunAction::EndOfRunAction(const G4Run*)
{
 
  G4cout<<G4endl;
 
  G4cout<<" End of run "<<G4endl;
  
  

} 


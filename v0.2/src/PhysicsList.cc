#include "PhysicsList.hh"

PhysicsList::PhysicsList(Projectile*Proj, Recoil* Rec):theRecoil(Rec),theProjectile(Proj)
{

}


PhysicsList::~PhysicsList()
{;}

void PhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  G4Gamma::GammaDefinition();
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4Proton::ProtonDefinition();
  G4Alpha::AlphaDefinition();
  //  ions
  G4IonConstructor iConstructor;
  iConstructor.ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
  // Define transportation process

  AddTransportation();
  ConstructEM();
}

void PhysicsList::ConstructEM()
{
  //theParticleIterator->reset(); // pre-v10.3
  GetParticleIterator()->reset();

  //while( (*theParticleIterator)() ){ // pre-v10.3
  while ((*GetParticleIterator())()) {

    //G4ParticleDefinition* particle = theParticleIterator->value(); // pre-v10.3
    G4ParticleDefinition *particle = GetParticleIterator()->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
 
     // G4cout<<"++++ Particle name ="<<particleName<<G4endl;
     // getc(stdin);

    if (particleName == "gamma") {
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());
      pmanager->AddDiscreteProcess(new G4GammaConversion());
    }
    else if (particleName == "e-") {
      pmanager->AddProcess(new G4eMultipleScattering,-1, 1, 1);
      pmanager->AddProcess(new G4eIonisation,        -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung,    -1, 3, 3);     
    }
    else if (particleName == "e+") {
      pmanager->AddProcess(new G4eMultipleScattering,-1, 1, 1);
      pmanager->AddProcess(new G4eIonisation,        -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung,    -1, 3, 3);   
      pmanager->AddProcess(new G4eplusAnnihilation,   0,-1, 4); 
    }
    else if( particleName == "proton" ) 
      {
    	pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
    	pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
      }
    else if(particleName=="alpha")
      {
    	pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
    	G4ionIonisation* alphaIoni = new G4ionIonisation();
    	alphaIoni->SetStepFunction(0.05, 0.05*um);
    	pmanager->AddProcess(alphaIoni,                 -1, 2, 2);
    	pmanager->AddProcess(new G4NuclearStopping(), -1, 3,-1); // small for light particles

	// // alphas can react too
	// // ---> DANGER: Doesn't work unless projectile is explicitly an alpha particle! <---
	// theReaction=new Reaction(theProjectile,theRecoil);
	// theReactionMessenger=new Reaction_Messenger(theReaction);
	// pmanager->AddProcess(theReaction,               -1,-1, 3);

    	// pmanager->AddProcess(new G4StepLimiter,         -1,-1, 4); // what does this do?
      }
    else if( particleName == "GenericIon") {
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      G4ionIonisation* ionIoni = new G4ionIonisation();
      SFUIonParametrisedLossModel* theModel= new SFUIonParametrisedLossModel(); // default ICRU 73 based model


      ionIoni->SetEmModel(theModel); // set stopping power model
      ionIoni->SetStepFunction(0.05,0.05*um);
      pmanager->AddProcess(ionIoni,                   -1, 2, 2);
      pmanager->AddProcess(new G4NuclearStopping(),   -1, 3,-1);
      theReaction=new Reaction(theProjectile,theRecoil);
      theReactionMessenger=new Reaction_Messenger(theReaction);
      pmanager->AddProcess(theReaction,               -1,-1, 3);
      pmanager->AddProcess(new G4StepLimiter,         -1,-1, 4);
    }
  }
}
 

void PhysicsList::SetCuts()
{
  // suppress error messages even in case e/gamma/proton do not exist
  
  //  G4int temp = GetVerboseLevel();    
  
  // G4double len=1*mm;
  // SetVerboseLevel(0);                                                           
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
  //   the default cut value for all particle types 
  
  // SetDefaultCutValue(len);
  SetCutsWithDefault();   
  // Retrieve verbose level
  // SetVerboseLevel(temp);  
  // getc(stdin);
}


// simulation program for TIP Rutherford scattering
// Implements PIN array

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4UItcsh.hh"
#include "G4UIterminal.hh"

//thp line
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#ifdef G4UI_USE_XM
#include "G4UIXm.hh"
#endif

#ifdef G4VIS_USE
#include "VisManager.hh"
#endif

#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "EventAction_Messenger.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "Projectile.hh"
#include "Projectile_Messenger.hh"
#include "Recoil.hh"
#include "Recoil_Messenger.hh"
#include "Results.hh"
#include "Results_Messenger.hh"
#include "RunAction.hh"
#include "Run_Messenger.hh"

int main(int argc, char **argv) {


  //thp lines
  G4UIExecutive* ui = 0;
  if((argc==2)&&(strcmp(argv[1],"-u")==0))
    ui=new G4UIExecutive(1,argv);

  
  // Seed the random number generator manually
  G4long myseed = time(NULL);
  //myseed=1524603874;
  myseed = 1578965358;
  CLHEP::HepRandom::setTheSeed(myseed);
  G4cout << " Seed set to  " << myseed << G4endl;

  // Construct the default run manager
  G4RunManager *runManager = new G4RunManager;

  // set mandatory initialization classes
  DetectorConstruction* theDetector=new DetectorConstruction();
  runManager->SetUserInitialization(theDetector);


  // Construct the incoming beam
  Projectile *theProjectile = new Projectile();
  Projectile_Messenger *ProjectileMessenger;
  ProjectileMessenger = new Projectile_Messenger(theProjectile);
  // theProjectile->Report();

// Construct the outgoing beam
  Recoil* theRecoil=new Recoil();
  Recoil_Messenger* RecoilMessenger;
  RecoilMessenger=new Recoil_Messenger(theRecoil);
  // theRecoil->Report();

  
  // Setup physics list
  // Also sets up the reaction
  PhysicsList *thePhysicsList = new PhysicsList(theProjectile, theRecoil);
  runManager->SetUserInitialization(thePhysicsList);

  Run_Messenger *runMessenger;
  runMessenger = new Run_Messenger(runManager, thePhysicsList);

  PrimaryGeneratorAction *generatorAction =
      new PrimaryGeneratorAction(theDetector, theProjectile);
  runManager->SetUserAction(generatorAction);

  Results *results = new Results(theProjectile, theDetector, thePhysicsList);
  Results_Messenger *resultsMessenger;
  resultsMessenger = new Results_Messenger(results);

  RunAction *theRunAction = new RunAction(thePhysicsList, results, theDetector);
  runManager->SetUserAction(theRunAction);

  EventAction *eventAction = new EventAction(results, theRunAction,theProjectile,theRecoil);
  runManager->SetUserAction(eventAction);
  EventAction_Messenger *eventActionMessenger;
  eventActionMessenger = new EventAction_Messenger(eventAction);
  
  // get the pointer to the UI manager and set verbosities
  G4UImanager *UI = G4UImanager::GetUIpointer();
  
  //thp LINES
  if( ! ui )
    {
      if(argc==1)
	{
	  G4cout << "TIP_3a_source macro_file" << G4endl;
	  G4cout << "------------------------" << G4endl;
	  G4cout << "Please specify a macro file (.mac) or -u for UI" << G4endl;
	  exit(-1);
	}
      else
	{
	  FILE *file;
	  file = fopen(argv[1], "r");
	  if(file)
	    fclose(file);
	  else
	    {
	      G4cout << "ERROR: macro file " << argv[1] << " cannot be opened." << G4endl;
	      G4cout << "Please check whether the file exists." << G4endl;
	      exit(-1);
	    }
	}
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UI->ApplyCommand(command + fileName);
    }
  else
    {
      // interactive mode
      // Initialize visualization
      G4VisManager* visManager = new G4VisExecutive("quiet");
      visManager->Initialize();
      ui->SessionStart();
      delete ui;
      delete visManager;
    }
  
  /*  #ifdef G4VIS_USE
    G4VisManager* visManager = new VisManager;
    // visualization manager
    visManager->Initialize();
  #endif

  if (argc == 1) // Define UI session for interactive mode.
  {

 
    G4cout << "PIN_Ruth  macro_file" << G4endl;
    G4cout << "---------------------------------" << G4endl;
    G4cout << "Please specify a macro file (.mac) to run as an argument."
           << G4endl;
    exit(-1);
  } else {
    // check whether the macro file exists
    FILE *file;
    file = fopen(argv[1], "r");
    if (file) {
      fclose(file);
    } else {
      G4cout << "ERROR: macro file " << argv[1] << " cannot be opened."
             << G4endl;
      G4cout << "Please check whether the file exists." << G4endl;
      exit(-1);
    }
  }

 

  G4String command = "/control/execute ";
  G4String fileName = argv[1];
  UI->ApplyCommand(command + fileName);*/

 
  // job termination

    
  
  delete ProjectileMessenger;
  delete resultsMessenger;
  delete runMessenger;
  delete eventActionMessenger;
  delete RecoilMessenger;
 
  delete runManager;
   
#ifdef G4VIS_USE
  delete visManager;
#endif


  return 0;
}

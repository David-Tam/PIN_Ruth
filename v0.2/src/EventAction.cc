#include "EventAction.hh"

EventAction::EventAction(Results* RE,RunAction* RA,Projectile* proj,Recoil* rec): results(RE),run_action(RA),theProjectile(proj),theRecoil(rec)
{ 
  ionCollectionID=-1;
  PINCollectionID=-1;
 
  setTrigger=NOTRIGGER;
  
}

EventAction::~EventAction()
{
  ;
}

void EventAction::BeginOfEventAction(const G4Event*)
{
  G4SDManager * SDman = G4SDManager::GetSDMpointer();

  if(ionCollectionID<0)
    ionCollectionID=SDman->GetCollectionID("ionCollection");

  if(PINCollectionID<0)
    PINCollectionID=SDman->GetCollectionID("PINCollection");

  eventTrigger=-1;
}

void EventAction::EndOfEventAction(const G4Event* evt)
{
  G4bool take;
  //G4cout<<"+++++ End of event "<<evt->GetEventID()<<G4endl;
  //PJV: we are in this function at the end of each event (1 event = 1 "beamOn")
  G4int evtNb=evt->GetEventID();
  
  if (evtNb%1000 == 0) 
    {
      gettimeofday(&tc,NULL);
      ts=run_action->GetStartTime();
      timersub(&tc,&ts,&td);
      t=td.tv_sec;
      rate=(float)evtNb/t;
      G4cout<<" Number of processed events "<<std::setw(9)<<evtNb<<" in "<<std::setw(9)<<t<<" sec. at "
	    <<std::fixed<<std::setw(9)<<std::setprecision(2)<<rate<<" events per second\r"<<std::flush;
    }
  
  

  G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
  TrackerPINHitsCollection* PIN=NULL;
  TrackerIonHitsCollection* HI=NULL;
 
  take=FALSE;
  if(HCE!=NULL) 
    {    
      PIN=(TrackerPINHitsCollection*)(HCE->GetHC(PINCollectionID)); 
      HI=(TrackerIonHitsCollection*)(HCE->GetHC(ionCollectionID));
 
      if(setTrigger==NOTRIGGER)
	take=TRUE;

      if(PIN->entries()>0)
	take=TRUE;
      
      if(take)
	results->FillTree(evtNb,HI,PIN);
    }
  

 
}

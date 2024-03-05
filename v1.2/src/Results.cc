#include "Results.hh"

Results::Results(Projectile *proj, DetectorConstruction *det,PhysicsList *physl)
  : theProjectile(proj), theDetector(det), thePhysicsList(physl){

  soh = sizeof(partHit);
  soi = sizeof(gun);
  sos = sizeof(stat);
  sor=sizeof(react);
}

Results::~Results() {tree->Delete(); }
//---------------------------------------------------------
void Results::SetupRun() {
  // Get A, Z of the projectile and recoiling 
 
  Ap = theProjectile->getA();
  Zp = theProjectile->getZ();
  Ar = theDetector->GetTarget()->getTargetMass();
  Zr = theDetector->GetTarget()->getTargetCharge();

  // G4cout << "Results: projectile A: " << Ap << ", projectile Z: " << Zp << ",
  // recoil A: " << Ar << ", recoil Z: " << Zr << G4endl;

  // get HPGe and PIN crystal positions

  GetPINWallPositions();
 
  G4cout << "Results - creating tree... ";
  TreeCreate();
  G4cout << "created!" << G4endl;

  // getc(stdin);
}
//---------------------------------------------------------
void Results::TreeCreate() {

  
  if (tree == NULL) {
    tree = new TTree("tree", "tree");
   
    tree->Branch("PINfold", &partHit.PINfold, "PINfold/I");
    tree->Branch("PIN_hitA", partHit.A, "A[PINfold]/I");
    tree->Branch("PIN_hitZ", partHit.Z, "Z[PINfold]/I");
    tree->Branch("PINx", partHit.x, "x[PINfold]/D");
    tree->Branch("PINy", partHit.y, "y[PINfold]/D");
    tree->Branch("PINz", partHit.z, "z[PINfold]/D");
    tree->Branch("PINpx", partHit.px, "px[PINfold]/D");
    tree->Branch("PINpy", partHit.py, "py[PINfold]/D");
    tree->Branch("PINpz", partHit.pz, "pz[PINfold]/D");
    tree->Branch("PINE", partHit.E, "E[PINfold]/D");
    tree->Branch("PINb", partHit.b, "b[PINfold]/D");
    tree->Branch("PINw", partHit.w, "w[PINfold]/D");
    tree->Branch("PINId", partHit.Id, "Id[PINfold]/I");
    tree->Branch("PINr", partHit.r, "r[PINfold]/I");

    tree->Branch("stat", &stat,
                 "evNb/I:Ap/I:Zp/I:Ar/I:Zr/I"); // event statistics
    
    tree->Branch("projGun", &gun,
                 "x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:t/D:tROffset/D:theta/"
                 "D:phi/D:w/D"); // projectile when shot from the particle gun

    tree->Branch("projTargetIn", &pTIn,
                 "x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:t/D:tROffset/D:theta/"
                 "D:phi/D:w/D"); // projectile upon entering the target
    tree->Branch("projReactionIn", &pRIn,
                 "x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:t/D:tROffset/D:theta/"
                 "D:phi/D:w/D"); // projectile at the reaction point
    tree->Branch("projReactionOut", &pROut,
                 "x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:t/D:tROffset/D:theta/"
                 "D:phi/D:w/D"); // projectile at the reaction point
    tree->Branch("projBackingIn", &pBIn,
        "x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:t/D:tROffset/D:theta/D:phi/D:w/"
        "D"); // projectile upon leaving the target/entering the backing
    tree->Branch("projBackingOut", &pBOut,
        "x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:t/D:tROffset/D:theta/D:phi/D:w/"
        "D"); // projectile upon leaving the backing (if it makes it that far)

    
    tree->Branch("recReactionOut", &rROut,
                 "x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:t/D:tROffset/D:theta/"
                 "D:phi/D:w/D"); // recoil at the reaction point
    tree->Branch("recBackingIn", &rBIn,
        "x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:t/D:tROffset/D:theta/D:phi/D:w/"
        "D"); // recoil upon leaving the target/entering the backing
    tree->Branch("recBackingOut", &rBOut,
        "x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:t/D:tROffset/D:theta/D:phi/D:w/"
        "D"); // recoil upon leaving the backing (if it makes it that far)


    tree->Branch("reactionInfo",&react,"ECM/D:thetaCM/D:dc/D:pcx/D:pcy/D:pcz/D:ec/D");
  } else {
    printf("Tree not deleted, could not create new tree!\n");
    getc(stdin);
  }
}
//---------------------------------------------------------
void Results::TreeClear() {
  tree->Delete("all");
  tree = NULL;
  TreeCreate();
}
//---------------------------------------------------------
void Results::TreeReport() { tree->Print(); }
//---------------------------------------------------------
void Results::TreeSave(G4String name) {

  TDirectory *dir = new TDirectory();
  dir = gDirectory;
  TFile f(name, "recreate");
  f.cd();
  // tree->AutoSave();
  // tree->SetDirectory(dir);
  tree->Write();
  f.Close();
  dir->cd();
  G4cout << "Trees of simulated parameters saved in file " << name << G4endl;
}
//---------------------------------------------------------
void Results::FillTree(G4int evtNb, TrackerIonHitsCollection *IonCollection,TrackerPINHitsCollection *PINCollection)
{
  Int_t i, j;
  G4int Np = PINCollection->entries();
  G4int Nt = IonCollection->entries();

  memset(&stat, 0, sos);
  memset(&gun, 0, soi);
  memset(&pTIn, 0, soi);
  memset(&pRIn, 0, soi);
  memset(&pROut, 0, soi);
  memset(&pBIn, 0, soi);
  memset(&pBOut, 0, soi);
  memset(&rROut, 0, soi);
  memset(&rBIn, 0, soi);
  memset(&rBOut, 0, soi);
  memset(&partHit, 0, soh);
  memset(&react,0,sor);
  
  if (Nt > 0) {
    // G4cout << "Saving ion collection data..." << G4endl;
    stat.evNb = evtNb;
    stat.Ap = Ap;
    stat.Zp = Zp;
    stat.Ar = Ar;
    stat.Zr = Zr;

    react.dc=thePhysicsList->getReaction()->ReportDC();
    react.thetaCM=thePhysicsList->getReaction()->ReportTheta();
    react.ECM=thePhysicsList->getReaction()->ReportECM();
    react.pcx=thePhysicsList->getReaction()->ReportPCx();
    react.pcy=thePhysicsList->getReaction()->ReportPCy();
    react.pcz=thePhysicsList->getReaction()->ReportPCz();
    react.ec=thePhysicsList->getReaction()->ReportEC();
   // Go through the IonCollection extract the info on Proj. Reaction In for time reference
   for (i = 0; i < Nt; i++) {
      if ((*IonCollection)[i]->GetPFlag() == REACTION_IN_FLAG) {
        if ((*IonCollection)[i]->GetA() == Ap)
          if ((*IonCollection)[i]->GetZ() == Zp) {
            pRIn.x = (*IonCollection)[i]->GetPos().getX() / mm;
            pRIn.y = (*IonCollection)[i]->GetPos().getY() / mm;
            pRIn.z = (*IonCollection)[i]->GetPos().getZ() / mm;
            pRIn.px = (*IonCollection)[i]->GetMom().getX() / MeV;
            pRIn.py = (*IonCollection)[i]->GetMom().getY() / MeV;
            pRIn.pz = (*IonCollection)[i]->GetMom().getZ() / MeV;
            pRIn.b = (*IonCollection)[i]->GetBeta();
            pRIn.E = (*IonCollection)[i]->GetKE() / MeV;
            pRIn.t = (*IonCollection)[i]->GetTime() / ns;
            pRIn.tROffset = 0.;
            pRIn.theta = acos(((*IonCollection)[i]->GetMom().getZ()) /
                              ((*IonCollection)[i]->GetMom().mag())) /
                         degree; // angle between (0,0,1) and momentum vector
            pRIn.phi =
                acos((*IonCollection)[i]->GetMom().getX() /
                     sqrt((*IonCollection)[i]->GetMom().getX() *
                              (*IonCollection)[i]->GetMom().getX() +
                          (*IonCollection)[i]->GetMom().getY() *
                              (*IonCollection)[i]->GetMom().getY())) /
                degree; // angle between (1,0,0) and momentum vector in x and y
            pRIn.w = (*IonCollection)[i]->GetWeight();
          }     
      }
   }
        
    // Go through the IonCollection, extract other information
    for (i = 0; i < Nt; i++) {
      if ((*IonCollection)[i]->GetFlag() == GUN_FLAG) {
        if ((*IonCollection)[i]->GetA() == Ap)
          if ((*IonCollection)[i]->GetZ() == Zp) {
            gun.x = (*IonCollection)[i]->GetPos().getX() / mm;
            gun.y = (*IonCollection)[i]->GetPos().getY() / mm;
            gun.z = (*IonCollection)[i]->GetPos().getZ() / mm;
            gun.px = (*IonCollection)[i]->GetMom().getX() / MeV;
            gun.py = (*IonCollection)[i]->GetMom().getY() / MeV;
            gun.pz = (*IonCollection)[i]->GetMom().getZ() / MeV;
            gun.b = (*IonCollection)[i]->GetBeta();
            gun.E = (*IonCollection)[i]->GetKE() / MeV;
            gun.t = (*IonCollection)[i]->GetTime() / ns;
            gun.tROffset -= pRIn.t;
            gun.theta = acos(((*IonCollection)[i]->GetMom().getZ()) /
                             ((*IonCollection)[i]->GetMom().mag())) /
                        degree; // angle between (0,0,1) and momentum vector
            gun.phi =
                acos((*IonCollection)[i]->GetMom().getX() /
                     sqrt((*IonCollection)[i]->GetMom().getX() *
                              (*IonCollection)[i]->GetMom().getX() +
                          (*IonCollection)[i]->GetMom().getY() *
                              (*IonCollection)[i]->GetMom().getY())) /
                degree; // angle between (1,0,0) and momentum vector in x and y
            gun.w = (*IonCollection)[i]->GetWeight();
          }
      } else if ((*IonCollection)[i]->GetFlag() == TARGET_IN_FLAG) {
        if ((*IonCollection)[i]->GetA() == Ap)
          if ((*IonCollection)[i]->GetZ() == Zp) {
            pTIn.x = (*IonCollection)[i]->GetPos().getX() / mm;
            pTIn.y = (*IonCollection)[i]->GetPos().getY() / mm;
            pTIn.z = (*IonCollection)[i]->GetPos().getZ() / mm;
            pTIn.px = (*IonCollection)[i]->GetMom().getX() / MeV;
            pTIn.py = (*IonCollection)[i]->GetMom().getY() / MeV;
            pTIn.pz = (*IonCollection)[i]->GetMom().getZ() / MeV;
            pTIn.b = (*IonCollection)[i]->GetBeta();
            pTIn.E = (*IonCollection)[i]->GetKE() / MeV;
            pTIn.t = (*IonCollection)[i]->GetTime() / ns;
            pTIn.tROffset -= pRIn.t;
            pTIn.theta = acos(((*IonCollection)[i]->GetMom().getZ()) /
                              ((*IonCollection)[i]->GetMom().mag())) /
                         degree; // angle between (0,0,1) and momentum vector
            pTIn.phi =
                acos((*IonCollection)[i]->GetMom().getX() /
                     sqrt((*IonCollection)[i]->GetMom().getX() *
                              (*IonCollection)[i]->GetMom().getX() +
                          (*IonCollection)[i]->GetMom().getY() *
                              (*IonCollection)[i]->GetMom().getY())) /
                degree; // angle between (1,0,0) and momentum vector in x and y
            pTIn.w = (*IonCollection)[i]->GetWeight();
          }
      } else if ((*IonCollection)[i]->GetFlag() == BACKING_IN_FLAG) {
        if ((*IonCollection)[i]->GetA() == Ap)
          if ((*IonCollection)[i]->GetZ() == Zp) {
            pBIn.x = (*IonCollection)[i]->GetPos().getX() / mm;
            pBIn.y = (*IonCollection)[i]->GetPos().getY() / mm;
            pBIn.z = (*IonCollection)[i]->GetPos().getZ() / mm;
            pBIn.px = (*IonCollection)[i]->GetMom().getX() / MeV;
            pBIn.py = (*IonCollection)[i]->GetMom().getY() / MeV;
            pBIn.pz = (*IonCollection)[i]->GetMom().getZ() / MeV;
            pBIn.b = (*IonCollection)[i]->GetBeta();
            pBIn.E = (*IonCollection)[i]->GetKE() / MeV;
            pBIn.t = (*IonCollection)[i]->GetTime() / ns;
            pBIn.tROffset -= pRIn.t;
            pBIn.theta = acos(((*IonCollection)[i]->GetMom().getZ()) /
                              ((*IonCollection)[i]->GetMom().mag())) /
                         degree; // angle between (0,0,1) and momentum vector
            pBIn.phi =
                acos((*IonCollection)[i]->GetMom().getX() /
                     sqrt((*IonCollection)[i]->GetMom().getX() *
                              (*IonCollection)[i]->GetMom().getX() +
                          (*IonCollection)[i]->GetMom().getY() *
                              (*IonCollection)[i]->GetMom().getY())) /
                degree; // angle between (1,0,0) and momentum vector in x and y
            pBIn.w = (*IonCollection)[i]->GetWeight();	    
          }

	if ((*IonCollection)[i]->GetA() == Ar)
          if ((*IonCollection)[i]->GetZ() == Zr) {
            rBIn.x = (*IonCollection)[i]->GetPos().getX() / mm;
            rBIn.y = (*IonCollection)[i]->GetPos().getY() / mm;
            rBIn.z = (*IonCollection)[i]->GetPos().getZ() / mm;
            rBIn.px = (*IonCollection)[i]->GetMom().getX() / MeV;
            rBIn.py = (*IonCollection)[i]->GetMom().getY() / MeV;
            rBIn.pz = (*IonCollection)[i]->GetMom().getZ() / MeV;
            rBIn.b = (*IonCollection)[i]->GetBeta();
            rBIn.E = (*IonCollection)[i]->GetKE() / MeV;
            rBIn.t = (*IonCollection)[i]->GetTime() / ns;
            rBIn.tROffset -= pRIn.t;
            rBIn.theta = acos(((*IonCollection)[i]->GetMom().getZ()) /
                              ((*IonCollection)[i]->GetMom().mag())) /
                         degree; // angle between (0,0,1) and momentum vector
            rBIn.phi =
                acos((*IonCollection)[i]->GetMom().getX() /
                     sqrt((*IonCollection)[i]->GetMom().getX() *
                              (*IonCollection)[i]->GetMom().getX() +
                          (*IonCollection)[i]->GetMom().getY() *
                              (*IonCollection)[i]->GetMom().getY())) /
                degree; // angle between (1,0,0) and momentum vector in x and y
            rBIn.w = (*IonCollection)[i]->GetWeight();
          }	
      } else if ((*IonCollection)[i]->GetFlag() == BACKING_OUT_FLAG) {
        if ((*IonCollection)[i]->GetA() == Ap)
          if ((*IonCollection)[i]->GetZ() == Zp) {
            pBOut.x = (*IonCollection)[i]->GetPos().getX() / mm;
            pBOut.y = (*IonCollection)[i]->GetPos().getY() / mm;
            pBOut.z = (*IonCollection)[i]->GetPos().getZ() / mm;
            pBOut.px = (*IonCollection)[i]->GetMom().getX() / MeV;
            pBOut.py = (*IonCollection)[i]->GetMom().getY() / MeV;
            pBOut.pz = (*IonCollection)[i]->GetMom().getZ() / MeV;
            pBOut.b = (*IonCollection)[i]->GetBeta();
            pBOut.E = (*IonCollection)[i]->GetKE() / MeV;
            pBOut.t = (*IonCollection)[i]->GetTime() / ns;
            pBOut.tROffset -= pRIn.t;
            pBOut.theta = acos(((*IonCollection)[i]->GetMom().getZ()) /
                              ((*IonCollection)[i]->GetMom().mag())) /
                         degree; // angle between (0,0,1) and momentum vector
            pBOut.phi =
                acos((*IonCollection)[i]->GetMom().getX() /
                     sqrt((*IonCollection)[i]->GetMom().getX() *
                              (*IonCollection)[i]->GetMom().getX() +
                          (*IonCollection)[i]->GetMom().getY() *
                              (*IonCollection)[i]->GetMom().getY())) /
                degree; // angle between (1,0,0) and momentum vector in x and y
            pBOut.w = (*IonCollection)[i]->GetWeight();
          }

	if ((*IonCollection)[i]->GetA() == Ar)
          if ((*IonCollection)[i]->GetZ() == Zr) {
            rBOut.x = (*IonCollection)[i]->GetPos().getX() / mm;
            rBOut.y = (*IonCollection)[i]->GetPos().getY() / mm;
            rBOut.z = (*IonCollection)[i]->GetPos().getZ() / mm;
            rBOut.px = (*IonCollection)[i]->GetMom().getX() / MeV;
            rBOut.py = (*IonCollection)[i]->GetMom().getY() / MeV;
            rBOut.pz = (*IonCollection)[i]->GetMom().getZ() / MeV;
            rBOut.b = (*IonCollection)[i]->GetBeta();
            rBOut.E = (*IonCollection)[i]->GetKE() / MeV;
            rBOut.t = (*IonCollection)[i]->GetTime() / ns;
            rBOut.tROffset -= pRIn.t;
            rBOut.theta = acos(((*IonCollection)[i]->GetMom().getZ()) /
                               ((*IonCollection)[i]->GetMom().mag())) /
                          degree; // angle between (0,0,1) and momentum vector
            rBOut.phi =
                acos((*IonCollection)[i]->GetMom().getX() /
                     sqrt((*IonCollection)[i]->GetMom().getX() *
                              (*IonCollection)[i]->GetMom().getX() +
                          (*IonCollection)[i]->GetMom().getY() *
                              (*IonCollection)[i]->GetMom().getY())) /
                degree; // angle between (1,0,0) and momentum vector in x and y
            rBOut.w = (*IonCollection)[i]->GetWeight();
          }
	
	 } else if ((*IonCollection)[i]->GetPFlag() == REACTION_OUT_FLAG) {
        // residual nucleus tracking
        if ((*IonCollection)[i]->GetA() == Ap)
          if ((*IonCollection)[i]->GetZ() == Zp) {
            pROut.x = (*IonCollection)[i]->GetPos().getX() / mm;
	    pROut.y = (*IonCollection)[i]->GetPos().getY() / mm;
            pROut.z = (*IonCollection)[i]->GetPos().getZ() / mm;
            pROut.px = (*IonCollection)[i]->GetMom().getX() / MeV;
            pROut.py = (*IonCollection)[i]->GetMom().getY() / MeV;
            pROut.pz = (*IonCollection)[i]->GetMom().getZ() / MeV;
            pROut.b = (*IonCollection)[i]->GetBeta();
            pROut.E = (*IonCollection)[i]->GetKE() / MeV;
            pROut.t = (*IonCollection)[i]->GetTime() / ns;
            pROut.tROffset -= pRIn.t;
            pROut.theta = acos(((*IonCollection)[i]->GetMom().getZ()) /
                               ((*IonCollection)[i]->GetMom().mag())) /
                          degree; // angle between (0,0,1) and momentum vector
            pROut.phi =
                acos((*IonCollection)[i]->GetMom().getX() /
                     sqrt((*IonCollection)[i]->GetMom().getX() *
                              (*IonCollection)[i]->GetMom().getX() +
                          (*IonCollection)[i]->GetMom().getY() *
                              (*IonCollection)[i]->GetMom().getY())) /
                degree; // angle between (1,0,0) and momentum vector in x and y
            pROut.w = (*IonCollection)[i]->GetWeight();
          }
   
        if ((*IonCollection)[i]->GetA() == Ar)
          if ((*IonCollection)[i]->GetZ() == Zr) {
	 
            rROut.x = (*IonCollection)[i]->GetPos().getX() / mm;
	    rROut.y = (*IonCollection)[i]->GetPos().getY() / mm;
            rROut.z = (*IonCollection)[i]->GetPos().getZ() / mm;
            rROut.px = (*IonCollection)[i]->GetMom().getX() / MeV;
            rROut.py = (*IonCollection)[i]->GetMom().getY() / MeV;
            rROut.pz = (*IonCollection)[i]->GetMom().getZ() / MeV;
            rROut.b = (*IonCollection)[i]->GetBeta();
            rROut.E = (*IonCollection)[i]->GetKE() / MeV;
            rROut.t = (*IonCollection)[i]->GetTime() / ns;
            rROut.tROffset -= pRIn.t;
            rROut.theta = acos(((*IonCollection)[i]->GetMom().getZ()) /
                               ((*IonCollection)[i]->GetMom().mag())) /
                          degree; // angle between (0,0,1) and momentum vector
            rROut.phi =
                acos((*IonCollection)[i]->GetMom().getX() /
                     sqrt((*IonCollection)[i]->GetMom().getX() *
                              (*IonCollection)[i]->GetMom().getX() +
                          (*IonCollection)[i]->GetMom().getY() *
                              (*IonCollection)[i]->GetMom().getY())) /
                degree; // angle between (1,0,0) and momentum vector in x and y
            rROut.w = (*IonCollection)[i]->GetWeight();	    
          }
      }     
    }
 
  } // end of ion collection entry saving

 
 

  Hit.PINfold = 0;
  memset(&Hit, 0, sizeof(PINInf));
  
 
  // sum energy deposits per PIN diode
  if (Np > 0)
    for (i = 0; i < Np; i++) // loop through all entries in the collection     
      {
	j=(*PINCollection)[i]->GetId();
	if(j>0)
	  if(j<NPIN)
	    {
	      if(Hit.E[j]==0)//  first hit for a given detector 
		{
		  Hit.PINfold++;
		  
		  Hit.A[j] = (*PINCollection)[i]->GetA();
		  Hit.Z[j] =
		    (*PINCollection)[i]->GetZ();
		  Hit.x[j] =
		    (*PINCollection)[i]->GetPos().getX() / mm;
		  Hit.y[j] =
		    (*PINCollection)[i]->GetPos().getY() / mm;
		  Hit.z[j] =
		    (*PINCollection)[i]->GetPos().getZ() / mm;
		  Hit.px[j] =
		    (*PINCollection)[i]->GetMom().getX() / MeV;
		  Hit.py[j] =
		    (*PINCollection)[i]->GetMom().getY() / MeV;
		  Hit.pz[j] =
		    (*PINCollection)[i]->GetMom().getZ() / MeV;
		  Hit.b[j] = (*PINCollection)[i]->GetBeta();
		  Hit.E[j] =
		    (*PINCollection)[i]->GetKE() / MeV;
		  Hit.w[j] = (*PINCollection)[i]->GetWeight();
		  Hit.Id[j] = (*PINCollection)[i]->GetId();
		  Hit.r[j] = (*PINCollection)[i]->GetRingId();
		}
	      else
		Hit.E[j]+=(*PINCollection)[i]->GetKE() / MeV;
	    }
      }

  
 // assign hits per fold
  partHit.PINfold = 0;
  memset(&partHit, 0, sizeof(PINInf));


  
  
  for (j = 1; j < NPIN; j++) // loop through all entries in the collection
    if(Hit.E[j]>0.0)
      if(partHit.PINfold<NPIN)
	{
	  partHit.A[partHit.PINfold]=Hit.A[j];
	  partHit.Z[partHit.PINfold]=Hit.Z[j];
	  partHit.x[partHit.PINfold]=Hit.x[j];
	  partHit.y[partHit.PINfold]=Hit.y[j];
	  partHit.z[partHit.PINfold]=Hit.z[j];
	  partHit.px[partHit.PINfold]=Hit.px[j];
	  partHit.py[partHit.PINfold]=Hit.py[j];
	  partHit.pz[partHit.PINfold]=Hit.pz[j];
	  partHit.b[partHit.PINfold]=Hit.b[j];
	  partHit.E[partHit.PINfold]=Hit.E[j];
	  partHit.w[partHit.PINfold]=Hit.w[j];
	  partHit.Id[partHit.PINfold]=Hit.Id[j];
	  partHit.r[partHit.PINfold]=Hit.r[j];	  
	  partHit.PINfold++;
	}
 
  
  tree->Fill();
}

//=====================================================================================
void Results::GetPINWallPositions() {
  Double_t xx, yy, zz;
  for (Int_t det = 0; det < NPIN; det++) {
    xx = theDetector->GetPINWall()->GetXPos(det + 1);
    yy = theDetector->GetPINWall()->GetYPos(det + 1);
    zz = theDetector->GetPINWall()->GetZPos();
    PP[det].setX(xx);
    PP[det].setY(yy);
    PP[det].setZ(zz);
  }
}

//=====================================================================================
void Results::ReportPINWallPositions() {
  Double_t xx, yy, zz, rr, cc;
  printf("pin      x        y        z        r       cos \n");
  for (Int_t det = 0; det < NPIN; det++) {
    xx = PP[det].getX();
    yy = PP[det].getY();
    zz = PP[det].getZ();
    rr = sqrt(xx * xx + yy * yy + zz * zz);
    cc = zz / rr;
    printf(" %2d  %8.3f %8.3f %8.3f %8.3f %8.3f\n", det + 1, xx, yy, zz, rr,
           cc);
  }
}

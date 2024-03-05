//First order Coulex E2 excitation based on Rev. Mod. Phys. 28 (1956) 432, mostly page 457
#include "Reaction.hh"

Reaction::Reaction(Projectile* Proj, Recoil* Rec, const G4String& aName)
  : G4VProcess(aName), theRecoil(Rec),theProjectile(Proj) {
  dcmin=0.; // minimum allowed distance of closest approach

  reaction_here=false;
  if (verboseLevel>1) {
    G4cout <<GetProcessName() << " is created "<< G4endl;};

  bmax=50;

}
//---------------------------------------------------------------------
Reaction::~Reaction() { ; }
//---------------------------------------------------------------------
G4VParticleChange* Reaction::PostStepDoIt(const G4Track& aTrack, const G4Step&) {

  aParticleChange.Initialize(aTrack);
  //
  // Stop the current particle, if requested by G4UserLimits 
  // 
  if(reaction_here) {
    reaction_here=false;
    G4DynamicParticle* ProjectileOut;
    G4DynamicParticle* RecoilOut;
    ProjectileOut =new G4DynamicParticle();
    RecoilOut =new G4DynamicParticle();
    
    if(SetupReactionProducts(aTrack,ProjectileOut,RecoilOut)) {   
      pconserved=aTrack.GetMomentum();
      econserved=aTrack.GetDynamicParticle()->GetTotalEnergy()+RecoilOut->GetMass();
      aParticleChange.ProposeTrackStatus(fStopAndKill);
      aParticleChange.SetNumberOfSecondaries(2);    
      aParticleChange.AddSecondary(ProjectileOut,posIn,true);
      aParticleChange.AddSecondary(RecoilOut,posIn,true);
      pconserved-=ProjectileOut->GetMomentum();
      econserved-=ProjectileOut->GetTotalEnergy();
      pconserved-=RecoilOut->GetMomentum();
      econserved-=RecoilOut->GetTotalEnergy();
      aParticleChange.GetSecondary(0)->SetWeight(theWeight);
      aParticleChange.GetSecondary(1)->SetWeight(theWeight);
      sumWeights += theWeight;
      sumProj++;
    }
  }
  
  return &aParticleChange;
}
//-----------------------------------------------------------------
G4double Reaction::PostStepGetPhysicalInteractionLength(const G4Track& aTrack, G4double, G4ForceCondition* condition) {
  reaction_here=false;
  *condition=NotForced;
  
  G4String name=aTrack.GetVolume()->GetLogicalVolume()->GetName();
  
  if(name=="target_log") {
    G4UserLimits* pUserLimits = aTrack.GetVolume()->GetLogicalVolume()->GetUserLimits();
    G4double ZReaction=pUserLimits->GetUserMinRange(aTrack);
    G4double ZCurrent=aTrack.GetPosition().getZ();
    G4double Z=ZReaction-ZCurrent;
    if(Z<0) {
      // G4cout<<" Past the reaction point"<<G4endl;
      // G4cout<<" Volume "<<name<<G4endl;
      // G4cout<<" Z[mm]: reaction "<<ZReaction/mm<<" current "<<ZCurrent/mm<<" DZ "<<Z/mm<<G4endl;
      // getc(stdin);
      return DBL_MAX;
    }
    if(Z>eps)	{
      G4ThreeVector dir=aTrack.GetDynamicParticle()->GetMomentumDirection();
	      
      dir*=(ZReaction-ZCurrent);
      // G4cout<<" Before the reaction point"<<G4endl;
      // G4cout<<" Volume "<<name<<G4endl;
      // G4cout<<" Z[mm]: reaction "<<ZReaction/mm<<" current "<<ZCurrent/mm<<" DZ "<<Z/mm<<G4endl;
      // getc(stdin);
      return dir.mag();
    }
    if(Z<=eps) {	   
      reaction_here=true;
      // G4cout<<setprecision(6)<<"ZReaction: "<<ZReaction<<G4endl;
      // getc(stdin);
      aTrack.GetVolume()->GetLogicalVolume()->GetUserLimits()->SetUserMinRange(-DBL_MAX);
      return 0.;
    }
  }  
  return DBL_MAX;
}

//---------------------------------------------------------
G4bool Reaction::SetupReactionProducts(const G4Track & aTrack,G4DynamicParticle* ProjectileOut, G4DynamicParticle* RecoilOut) {
  G4double Ain;
  G4double Zin;
  G4ThreeVector dirIn,pdir,rdir,perpIn1,perpIn2;
  G4ThreeVector pIn;
  G4double KEIn;
  G4double t,tbar;
  G4double phi;
  G4double sint,cost,sinp,cosp;
  G4double p_par1,p_par2,p_perp,r;
 
  // G4double sigmaRatio; // rxn depth/face
  //  G4double rand; // uniform RNG

  Ain=aTrack.GetDynamicParticle()->GetDefinition()->GetAtomicMass();
  if(Ain != A1)
    {
      printf("Error Ain %2d not equal to A1 %2d\n",(int)Ain,(int)A1);
      return FALSE;
    }

  Zin=aTrack.GetDynamicParticle()->GetDefinition()->GetAtomicNumber();
  if(Zin != Z1)
    {
      printf("Error Zin %2d not equal to Z1 %2d\n",(int)Zin,(int)Z1);
      return FALSE;
    }
  
  ProjectileOut->SetDefinition(proj);
  RecoilOut->SetDefinition(rec);

  // check if energy in LAB is sufficienct for excitation
  // if so check that ksi and f(ksi) are good values
  // for good ksi and f(ksi) calculate ratio of XC at reaction energy
  // to ratio on face for the weight
  KEIn = aTrack.GetDynamicParticle()->GetKineticEnergy(); // energy of the incident particle
  ECM=KEIn*A2/(A1+A2);
  dirIn = aTrack.GetMomentumDirection();
  posIn = aTrack.GetPosition();
  pIn = aTrack.GetMomentum(); 

  
  theta =GetTheta() ; // in the CM
  // printf("theta = %.2f\n",theta);
  // getc(stdin);
  
  dc = 0.5*AHC*Z1*Z2/ECM*(1+1/sin(0.5*theta*DEG2RAD));
  // printf("dc %.4f\n",dc);
  // getc(stdin);
  
  if(dc > dcmin) {      
    sint=sin(theta*DEG2RAD);
    cost=cos(theta*DEG2RAD);
    phi=TWOPI*G4UniformRand();
    sinp=sin(phi);
    cosp=cos(phi);

    tbar=1;
    t=A1/A2*tbar;
    //this is momentum in the lab assuming the z axis as the beam axis
    r=A2/(A1+A2)*pIn.mag();
  
    p_perp=r*sint;
    p_par1=r*(cost+t);
    p_par2=r*(tbar-cost);
      
    //this is the rotation on the z-axis defined by the direction of the incoming projectile 
    perpIn1.setX(dirIn.getY());
    perpIn1.setY(dirIn.getX());
    perpIn1.setZ(0.);
    if(perpIn1.mag()==0.) {
      perpIn1.setX(1.);
      perpIn1.setY(0.);
      perpIn1.setZ(0.);
      perpIn2.setX(0.);
      perpIn2.setY(1.);
      perpIn2.setZ(0.);
    } 
    else {
      perpIn1.setMag(1.);
      perpIn2=dirIn.cross(perpIn1);
    }
      
    pdir=p_par1*dirIn+p_perp*cosp*perpIn1+p_perp*sinp*perpIn2;
    rdir=p_par2*dirIn-p_perp*cosp*perpIn1-p_perp*sinp*perpIn2;
      
    ProjectileOut->SetMomentum(pdir); 
    RecoilOut->SetMomentum(rdir);

    theWeight = pow(KE/KEIn,2.); // "weighted" by XC selection above

    return TRUE;
  }  
  else
    return FALSE;
}
//---------------------------------------------------------
void Reaction::TargetFaceCrossSection() {

 
   
  G4cout << "---------- SETUP OF REACTION PRODUCTS ----------" << G4endl;
 
  
  A1=theProjectile->getA();
  Z1=theProjectile->getZ(); 
 
  A2=theRecoil->getA();
  Z2=theRecoil->getZ();
  
    
 
  
 
  printf("Projectile A                        : %5d\n",(int)A1);
  printf("Projectile Z                        : %5d\n",(int)Z1);
  
  printf("Target recoil A                     : %5d\n",(int)A2);
  printf("Target recoil Z                     : %5d\n",(int)Z2);
 
  
  

  KE = theProjectile->getKE(); // this is the kinetic energy of the projectile in the lab used to get the vi two lines down
  ECM=KE*A2/(A1+A2);
  d0 = AHC*Z1*Z2/ECM;
  printf("\nMinimum distance of closest approach: %5.2f fm\n",d0);

  proj = G4IonTable::GetIonTable()->GetIon(Z1,A1,0.);
  rec = G4IonTable::GetIonTable()->GetIon(Z2,A2,0.);

  if (proj == NULL) {
    G4cerr << "Could not find outgoing projectile in the particle table "
           << Z1 << " " << A1 << G4endl;
    exit(EXIT_FAILURE);
  }

  if (rec == NULL) {
    G4cerr << "Could not find outgoing recoil in the particle table "
           << Z2 << " " << A2 << G4endl;
    exit(EXIT_FAILURE);
  }
  
 
  sumWeights = 0.0;
  sumProj = 0; // good events


  G4cout << "---------- END OF REACTION SETUP ----------" << G4endl;
}

//---------------------------------------------------------
G4double Reaction::GetTheta()
{
  return 2.*atan(d0/bmax/sqrt(G4UniformRand()))/DEG2RAD;
}

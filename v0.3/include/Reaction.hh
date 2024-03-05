#ifndef Reaction_h
#define Reaction_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "G4VProcess.hh"
#include "DetectorConstruction.hh"
#include "Recoil.hh"
#include "Projectile.hh"
#include "G4VParticleChange.hh" // standard particle change 
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4UnitsTable.hh"
#include "G4UserLimits.hh"
#include "G4IonTable.hh"
#include "G4DynamicParticle.hh"
#include "G4ParticleTable.hh"
#include "G4Track.hh"
#include "Common.hh"
#include <math.h>

using namespace std;

#define  eps 0.00001

class Reaction : public G4VProcess 
{
public:     
  G4bool reaction_here;
  
  Reaction(Projectile*,Recoil*, const G4String& processName ="Reaction" );

  virtual ~Reaction();

 
 
  virtual G4double PostStepGetPhysicalInteractionLength(
							const G4Track& track,
							G4double   previousStepSize,
							G4ForceCondition* condition
							);

  virtual G4VParticleChange* PostStepDoIt(
					  const G4Track& ,
					  const G4Step& 
					  );
			    
  //  no operation in  AtRestGPIL
  virtual G4double AtRestGetPhysicalInteractionLength(
						      const G4Track& ,
						      G4ForceCondition* 
						      ){ return -1.0; };
			    
  //  no operation in  AtRestDoIt      
  virtual G4VParticleChange* AtRestDoIt(
					const G4Track& ,
					const G4Step&
					){return NULL;};

  //  no operation in  AlongStepGPIL
  virtual G4double AlongStepGetPhysicalInteractionLength(
							 const G4Track&,
							 G4double  ,
							 G4double  ,
							 G4double& ,
							 G4GPILSelection*
							 ){ return -1.0; };

  //  no operation in  AlongStepDoIt
  virtual G4VParticleChange* AlongStepDoIt(
					   const G4Track& ,
					   const G4Step& 
					   ) {return NULL;};

  void     TargetFaceCrossSection();
  G4int    getTargetRecoilA(){return A2;};
  G4int    getTargetRecoilZ(){return Z2;};
  G4double GetTargetFaceCrossSection(){return sigmaFace;};// in barns
  G4double GetThickTargetCorrection(){return sumWeights/sumProj;};
  G4double GetThickTargetCrossSection(){return sumWeights/sumProj*sigmaFace;};// in barns
  G4int    GetNumberOfSimulatedReactions(){return sumProj;};
  void     ReportDCmin(){printf("Minimum distnace of closest approach = %.2f fm\n",dcmin);};
  void     SetDCmin(G4double d){dcmin = d;};
  G4double ReportBmax(void){return bmax;};
  void     SetBmax(G4double b){bmax = b;};
  G4double ReportDC(void){return dc;};
  G4double ReportTheta(void){return theta;};
  G4double ReportECM(void){return ECM;};
  G4double ReportPCx(void){return pconserved.getX();};
  G4double ReportPCy(void){return pconserved.getY();};
  G4double ReportPCz(void){return pconserved.getZ();};
  G4double ReportEC(void){return econserved;};
private:
  G4double dc; // distance of closest approach in fm
  G4double theta;//theta in CM
  G4double ECM;// kinetic energy in CM
  
  // hide assignment operator as private 
  Reaction& operator=(const Reaction&){return *this;};

  Recoil     *theRecoil;
  Projectile *theProjectile;
 
 
  G4bool   SetupReactionProducts(const G4Track &,G4DynamicParticle*,G4DynamicParticle*);
  G4double GetTheta();
  
  G4ThreeVector posIn;
  G4int ReactionFlag;
  G4double  KE;
  G4double sigmaFace;
  G4double sumWeights;
  G4int    sumProj;
  G4int    dropE;
 
  G4double A1,Z1,A2,Z2;
  G4double d0;
  G4double dcmin; // minimum allowed distance of closest approach in fm

  G4ParticleDefinition* proj;
  G4ParticleDefinition* rec;

  G4double theWeight;
  G4ThreeVector pconserved;
  G4double      econserved;

  G4double   bmax;
};

#endif


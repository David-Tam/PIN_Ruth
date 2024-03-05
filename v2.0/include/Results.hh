#ifndef Results_h
#define Results_h 1

#include "DetectorConstruction.hh"
#include "Flags.hh"
#include "Materials.hh"
#include "PhysicsList.hh"
#include "Projectile.hh"
#include "TrackerPINHit.hh"
#include "TrackerIonSD.hh"

#include "TBranch.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"

#include "globals.hh"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NPIN 45
#define PI 3.14159
#define S16K 16384
#define RADIAN 57.29583


typedef struct {
  Int_t evNb;
  Int_t Ap;
  Int_t Zp;
  Int_t Ar;
  Int_t Zr;
} IonStat;

typedef struct {
  Double_t x;
  Double_t y;
  Double_t z;
  Double_t px;
  Double_t py;
  Double_t pz;
  Double_t E;
  Double_t b;     // beta
  Double_t t;     // time
  Double_t tROffset; //time offset w.r.t. reaction
  Double_t theta; // angle of emission from beam (z) axis
  Double_t phi; // angle between plane of emission and horizontal (x) axis plane
  Double_t w;
  Double_t path;  // path length in material
  Double_t Eloss; // energy loss on path
} IonInf;


typedef struct {
  Int_t PINfold;
  Int_t A[NPIN];
  Int_t Z[NPIN];
  Double_t x[NPIN];
  Double_t y[NPIN];
  Double_t z[NPIN];
  Double_t px[NPIN];
  Double_t py[NPIN];
  Double_t pz[NPIN];
  Double_t E[NPIN];
  Double_t b[NPIN];
  Double_t w[NPIN];
  Int_t Id[NPIN];
  Int_t r[NPIN];
} PINInf;

typedef struct{
  Double_t ECM;//Energy in the CM in MeV
  Double_t thetaCM; // theta in CM
  Double_t dc;   // distance of closest approach in fm
  Double_t pcx; //x component of the incoming and outgoing total momentum difference in the lab 
  Double_t pcy; //y component of the incoming and outgoing total momentum difference in the lab
  Double_t pcz; //z component of the incoming and outgoing total momentum difference in the lab
  Double_t ec;//incoming and outgoing kinetic energy difference in the lab
} ReactInf;

class Results {
public:
  Results(Projectile *, DetectorConstruction *, PhysicsList *);
  ~Results();

public:
  void SetupRun();

  void FillTree(G4int, TrackerIonHitsCollection *, TrackerPINHitsCollection *);
  void TreeSave(G4String);
  void TreeRead(G4String);
  void TreeAdd(G4String);
  void TreeClear();
  void TreeCreate();
  void TreeReport();
  void ProjLabScattAngle();
  void RecLabScattAngle();
  void TargetFaceF_CM();
  void SetTargetFaceCrossSection(G4double xs) { TargetFaceCrossSection = xs; };
  void ReportPINWallPositions();
  void GetPINWallPositions();
  
 
private:
  // ROOT Tree stuff
  TTree *tree;

  Projectile *theProjectile;
  DetectorConstruction *theDetector;
  PhysicsList *thePhysicsList;
 
  
  IonStat stat;
  IonInf gun, pTIn, pRIn, pROut, pBIn, pBOut, rROut, rBIn, rBOut;
  
  PINInf Hit,partHit;     // particle hit

  ReactInf      react;

  G4int Ap, Zp, Ar, Zr, numP, numN, numA;
  G4ThreeVector rDiPos,
      rDPos; // recoil degrader position vectors for calculating distance
  G4double dDensity;   // degrader density for calculating path in mg/cm^2
  G4double PINDensity; // PIN density for calculating path in mg/cm^2
 
  size_t soh, sogh, soi, sos, soes, soint,sor;

 
 
  G4double TargetFaceCrossSection;
  G4int nPart = 0;
 
 
  G4ThreeVector PP[NPIN];

  G4Material *a;
};

#endif

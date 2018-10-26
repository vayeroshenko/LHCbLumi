/*
 * L_PrimaryGeneratorAction.h
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#ifndef SRC_L_PRIMARYGENERATORACTION_H_
#define SRC_L_PRIMARYGENERATORACTION_H_

#include <G4VUserPrimaryGeneratorAction.hh>
#include "globals.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "TROOT.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TMath.h"
#include "TChain.h"



#include "Randomize.hh"

//class G4Hep
class G4HEPEvtInterface;
class G4VPrimaryGenerator;
class G4ParticleGun;
class G4Event;

class L_PrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction {
public:
    L_PrimaryGeneratorAction();
    virtual ~L_PrimaryGeneratorAction();
public:
	void GeneratePrimaries(G4Event* anEvent);

private:
	void ReadFile();
	bool GetEvent(G4int evtID);

private:
	G4ParticleGun *_particleGun = new G4ParticleGun();

	TChain *tree;
	G4int nEvents;
	G4int nParticles;
	G4int pdgID[20000];
	G4float X[20000];
	G4float Y[20000];
	G4float Z[20000];
	G4float pX[20000];
	G4float pY[20000];
	G4float pZ[20000];

	G4int iEv;



};

#endif /* SRC_L_PrimaryGeneratorAction_H_ */

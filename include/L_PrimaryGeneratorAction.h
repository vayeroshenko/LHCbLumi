/*
 * L_PrimaryGeneratorAction.h
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#pragma once

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
#include "G4RunManager.hh"
#include "TRandom3.h"
#include "G4Timer.hh"

#include "TROOT.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TMath.h"
#include "TChain.h"

#include "Pythia8/Pythia.h"
#include "LConst.hh"
#include <time.h>

#include "Randomize.hh"

#include <iostream>

//class G4Hep
class G4HEPEvtInterface;
class G4VPrimaryGenerator;
class G4ParticleGun;
class G4Event;

class L_PrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction {
public:
    L_PrimaryGeneratorAction(long long gen_seed = -1);
    virtual ~L_PrimaryGeneratorAction();
public:
    void GeneratePrimaries(G4Event* anEvent);

private:
    bool GetEvent();
    void initBeams();

private:
    G4ParticleGun *_particleGun = new G4ParticleGun();

    // Data to be transported from pythia to gun
    G4int nEvents = 0;
    G4int nParticles = 0;

    G4ThreeVector beam1p;
    G4ThreeVector beam2p;

    G4int pdgID[20000] = {0};
    G4float X[20000] = {0};
    G4float Y[20000] = {0};
    G4float Z[20000] = {0};
    G4float pX[20000] = {0};
    G4float pY[20000] = {0};
    G4float pZ[20000] = {0};
    G4float T[20000] = {0};

    // Pythia generator declaration
    Pythia8::Pythia pythia;
};


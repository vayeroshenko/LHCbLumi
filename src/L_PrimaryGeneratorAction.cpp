/*
 * L_PrimaryGeneratorAction.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#include "L_PrimaryGeneratorAction.h"


L_PrimaryGeneratorAction::L_PrimaryGeneratorAction() {

    //    G4cout << "Start creating primary generator" << G4endl;

    // Pythia seed is generated from system time
    Int_t pythiaSeed = time(NULL)%10000000;


    // Getting number of event to be run in order to give Pythia
    // an information how many event to generate
    G4RunManager *runManager = G4RunManager::GetRunManager();
    G4int evToGen = runManager->GetNumberOfEventsToBeProcessed();

    // Reading configuration file, setting number of events and seed
    pythia.readFile("PythiaSettings.cmnd");
    pythia.readString("Main:numberOfEvents = " + std::to_string(evToGen));
    pythia.readString("Random:seed = " + std::to_string(pythiaSeed));

    // Starting up the pythia instance
    pythia.init();


    //    G4cout << "Primary generator created" << G4endl;
}

L_PrimaryGeneratorAction::~L_PrimaryGeneratorAction() {

}

void L_PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

    // filling up class variables with event data from pythia
    GetEvent();

    G4ParticleTable* particleTable;
    G4ParticleDefinition* particle;
    G4ThreeVector dir;
    G4double m, momentum, Ekin;

    // generating all primaries from event
    for (G4int pId = 0; pId < nParticles; ++pId){
        particleTable = G4ParticleTable::GetParticleTable();
        particle = particleTable->FindParticle(pdgID[pId]);
        m = particle->GetPDGMass();

        dir = G4ThreeVector(pX[pId],pY[pId],pZ[pId]);

        momentum = TMath::Sqrt(pX[pId]*pX[pId] + pY[pId]*pY[pId] + pZ[pId]*pZ[pId]);
        Ekin = (TMath::Sqrt(momentum*momentum + m*m) - m);

        _particleGun->SetParticleDefinition(particle);
        _particleGun->SetParticleMomentumDirection(dir);
        _particleGun->SetParticleEnergy(Ekin);
        _particleGun->SetParticleTime(T[pId]);

        _particleGun->SetParticlePosition(G4ThreeVector(X[pId], Y[pId], Z[pId]));

        // Cut off low-momentum particles (< 20 MeV)
        if (momentum < 20.) continue; //////////////////////// Momentum cut ////////////////////////////

        _particleGun->GeneratePrimaryVertex(anEvent);
    }


    //    G4cout << "Primaries generated" << G4endl;
}


// Method for getting an information from pythia into the class variables to pass
// into the particle gun
bool L_PrimaryGeneratorAction::GetEvent() {

    // If current event is inapropriate trying another time
    // (never actually tested how often does it happens)
    if (!pythia.next()) GetEvent();

    G4int particleID = 0;
    G4double vertexX, vertexY, vertexZ;

    // in terms of pythia:
    // 0        - total values
    // 1 and 2  - initial protons colliding
    // so starting from 4th (id = 3) particle
    for (G4int i = 3; i < pythia.event.size(); ++i){
        if (!pythia.event[i].isFinal()) continue;


        vertexX = G4RandGauss::shoot(LConst::VertexX, LConst::VertexSigmaX);
        vertexY = G4RandGauss::shoot(LConst::VertexY, LConst::VertexSigmaY);
        vertexZ = G4RandGauss::shoot(LConst::VertexZ, LConst::VertexSigmaZ);

        pdgID[particleID] = pythia.event[i].id();

        X[particleID] = pythia.event[i].xProd()*mm + vertexX;
        Y[particleID] = pythia.event[i].yProd()*mm + vertexY;
        Z[particleID] = pythia.event[i].zProd()*mm + vertexZ;

        pX[particleID] = pythia.event[i].px()*GeV;
        pY[particleID] = pythia.event[i].py()*GeV;
        pZ[particleID] = pythia.event[i].pz()*GeV;
        T[particleID] = pythia.event[i].tProd()*mm / c_light;
        particleID ++;
    }



    nParticles = particleID;

    return true;
}

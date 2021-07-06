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

//    initBeams();

//    pythia.init();


    //    G4cout << "Primary generator created" << G4endl;
}

L_PrimaryGeneratorAction::~L_PrimaryGeneratorAction() {

}

void L_PrimaryGeneratorAction::initBeams() {
    beam1p = G4ThreeVector(0.,0.,1.);
    beam2p = G4ThreeVector(0.,0.,-1.);

    beam1p.rotateY(+LConst::beam1AngleHorizontal);
    beam2p.rotateY(-LConst::beam1AngleHorizontal);

    beam1p.rotateX(+LConst::beam1AngleVertical);
    beam2p.rotateX(-LConst::beam1AngleVertical);

    beam1p *= LConst::beam1Energy;
    beam2p *= LConst::beam2Energy;

    pythia.readString("Beams:pxA = " + std::to_string(beam1p.x() / GeV));
    pythia.readString("Beams:pyA = " + std::to_string(beam1p.y() / GeV));
    pythia.readString("Beams:pzA = " + std::to_string(beam1p.z() / GeV));

    pythia.readString("Beams:pxB = " + std::to_string(beam2p.x() / GeV));
    pythia.readString("Beams:pyB = " + std::to_string(beam2p.y() / GeV));
    pythia.readString("Beams:pzB = " + std::to_string(beam2p.z() / GeV));
}

void L_PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

    // filling up class variables with event data from pythia
//    GetEvent();

    G4ParticleTable* particleTable;
    G4ParticleDefinition* particle;
    G4ThreeVector dir;
    G4double m, momentum, Ekin;

    particleTable = G4ParticleTable::GetParticleTable();
    particle = particleTable->FindParticle(11);
    m = particle->GetPDGMass();

    auto cos_theta = G4RandFlat::shoot(-1., 1.);
    auto phi = G4RandFlat::shoot(0., twopi);


//    dir = G4ThreeVector(sqrt(1 - cos_theta*cos_theta)*cos(phi),
//                        sqrt(1 - cos_theta*cos_theta)*sin(phi),
//                        cos_theta);
//                        -abs(cos_theta));

    dir = G4ThreeVector(0,
                        0,
                        -1);

    Ekin = (LConst::bi_e_energy);

    _particleGun->SetParticleDefinition(particle);
    _particleGun->SetParticleMomentumDirection(dir);
    _particleGun->SetParticleEnergy(Ekin);
    _particleGun->SetParticleTime(0);

    _particleGun->SetParticlePosition(G4ThreeVector(0,0,LConst::bi_z_pos));

    // Cut off low-momentum particles (< 20 MeV)
//    if (momentum < 20.) continue; //////////////////////// Momentum cut ////////////////////////////



    _particleGun->GeneratePrimaryVertex(anEvent);


    //    G4cout << "Primaries generated" << G4endl;
}


// Method for getting an information from pythia into the class variables to pass
// into the particle gun
bool L_PrimaryGeneratorAction::GetEvent() {

    // If current event is inapropriate trying another time
    // (never actually tested how often does it happen
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


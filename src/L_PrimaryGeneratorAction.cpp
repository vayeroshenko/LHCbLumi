/*
 * L_PrimaryGeneratorAction.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#include "L_PrimaryGeneratorAction.h"


L_PrimaryGeneratorAction::L_PrimaryGeneratorAction() {
	ReadFile();
	iEv = 0;
}

L_PrimaryGeneratorAction::~L_PrimaryGeneratorAction() {

}

void L_PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

	G4LogicalVolume* worldLV
	= G4LogicalVolumeStore::GetInstance()->GetVolume("World");

	if(GetEvent(iEv)){
		for (G4int pId = 0; pId < nParticles; ++pId){
			G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
			G4ParticleDefinition* particle = particleTable->FindParticle(pdgID[pId]);
			G4double m = particle->GetPDGMass();

			G4ThreeVector dir = G4ThreeVector(pX[pId],pY[pId],pZ[pId]);

			G4double momentum = TMath::Sqrt(pX[pId]*pX[pId] + pY[pId]*pY[pId] + pZ[pId]*pZ[pId]);
			G4double Ekin = (TMath::Sqrt(momentum*momentum + m*m) - m);

			_particleGun->SetParticleDefinition(particle);
			_particleGun->SetParticleMomentumDirection(dir);
			_particleGun->SetParticleEnergy(Ekin);
			_particleGun->SetParticlePosition(G4ThreeVector(X[pId], Y[pId], Z[pId]));
			_particleGun->GeneratePrimaryVertex(anEvent);

//			G4cout << "Particle name = " << particle->GetParticleName() << G4endl;
		}
		iEv ++;
	}
	else return;
}

void L_PrimaryGeneratorAction::ReadFile() {
	TString fileName = "events.root";
	tree = new TChain("Tree");
	tree->Add(fileName);

	nEvents = tree->GetEntries();
	tree->Print();

	tree->SetBranchAddress("nParticles",&nParticles);

	tree->SetBranchAddress("PDGid",pdgID);
	tree->SetBranchAddress("X",X);
	tree->SetBranchAddress("Y",Y);
	tree->SetBranchAddress("Z",Z);
	tree->SetBranchAddress("Px",pX);
	tree->SetBranchAddress("Py",pY);
	tree->SetBranchAddress("Pz",pZ);

}

bool L_PrimaryGeneratorAction::GetEvent(G4int evtID) {
	if (evtID < nEvents){
		tree->GetEntry(evtID);
		for (G4int i; i < nParticles; ++i){
			pX[i] *= GeV; pY[i] *= GeV; pZ[i] *= GeV;
			X[i] *= mm; Y[i] *= mm; Z[i] *= mm;
		}
		return true;
	} else return false;
}

/*
 * L_SensitiveDetector.cpp
 *
 *  Created on: Oct 3, 2018
 *      Author: vsevolod
 */

#include "L_SensitiveDetector.h"

#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

L_SensitiveDetector::L_SensitiveDetector(G4String name) : // @suppress("Class members should be properly initialized")
G4VSensitiveDetector(name){
	//  G4RunManager* runManager = G4RunManager::GetRunManager();
	G4String HCname = "Collection";
	collectionName.insert(HCname);
}

L_SensitiveDetector::~L_SensitiveDetector() {
}

void L_SensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{
	_Collection =
            new L_HitsCollection(SensitiveDetectorName, collectionName[0]);
	static G4int HCID = -1;
	if (HCID < 0) {
		HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	}
	HCE->AddHitsCollection(HCID, _Collection);

	//  G4cout << "___________________________ Detector initialized" << G4endl;
}


G4bool L_SensitiveDetector::ProcessHitsH(G4Step* aStep, G4TouchableHistory* hist) {
	return ProcessHits(aStep, hist);
}

G4bool L_SensitiveDetector::ProcessHits(G4Step* aStep,
		G4TouchableHistory*)
{

	G4Track* aTrack = aStep->GetTrack();
	G4ThreeVector globalPosition = aStep->GetPostStepPoint()->GetPosition();
    L_Hit* newHit = new L_Hit();
	newHit->myData;

	G4StepPoint *aPostPoint = aStep->GetPostStepPoint();
	G4StepPoint *aPrevPoint = aStep->GetPreStepPoint();
	G4LogicalVolume *SDVolume = aPostPoint->GetPhysicalVolume()->GetLogicalVolume();
	G4LogicalVolume *PrevVolume = aPrevPoint->GetPhysicalVolume()->GetLogicalVolume();
	G4String  PreName = SDVolume->GetName();
	G4String SDName = PrevVolume->GetName();



	//if(newHit->myData.SecID != -3)
	//G4cout<<"newHit->myData.SecID = "<<newHit->myData.SecID<<G4endl;

	// Fill the stuff that wasn't passed in the hitInfo struct
	newHit->myData.TrackID = aTrack->GetTrackID();
	newHit->myData.ParentID = aTrack->GetParentID();
	newHit->myData.Energy = aTrack->GetKineticEnergy();
	newHit->myData.PdgID = aTrack->GetParticleDefinition()->GetPDGEncoding();
	newHit->myData.Time = aTrack->GetGlobalTime();
	newHit->myData.X = globalPosition.x();
	newHit->myData.Y = globalPosition.y();
	newHit->myData.Z = globalPosition.z();
	newHit->myData.Px = aTrack->GetMomentum().x();
	newHit->myData.Py = aTrack->GetMomentum().y();
	newHit->myData.Pz = aTrack->GetMomentum().z();
	if (SDName == "HStationB2" && PreName == "World")     newHit->myData.StationID = -2;
	else if (SDName == "HStationB1" && PreName == "World") newHit->myData.StationID = -1;
	else if (SDName == "HStationB0" && PreName == "World") newHit->myData.StationID = 0;
	else if (SDName == "HStationF1" && PreName == "World") newHit->myData.StationID = 1;
	else if (SDName == "HStationF2" && PreName == "World") newHit->myData.StationID = 2;
	else return true;


	// Insert this hit
	_Collection->insert(newHit);
	newHit->Draw();
	//  }


	return true;
}

void L_SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{

	G4int NbHits = _Collection->entries();

}

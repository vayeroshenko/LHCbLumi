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


L_SensitiveDetector::L_SensitiveDetector(G4String name) :
    G4VSensitiveDetector(name){
    //  G4RunManager* runManager = G4RunManager::GetRunManager();

    // Setting up the hit collection
    G4String HCname = "Collection";
    collectionName.insert(HCname);

    G4cout << "Sensitive detector created" << G4endl;
}

L_SensitiveDetector::~L_SensitiveDetector() {
}

void L_SensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{
    // Getting a hit collection
    _Collection =
            new L_HitsCollection(SensitiveDetectorName, collectionName[0]);
    static G4int HCID = -1;
    if (HCID < 0) {
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    HCE->AddHitsCollection(HCID, _Collection);

    //  G4cout << "___________________________ Detector initialized" << G4endl;
}


G4bool L_SensitiveDetector::ProcessHitsL(G4Step* aStep, G4TouchableHistory* hist) {
    return ProcessHits(aStep, hist);
}

G4bool L_SensitiveDetector::ProcessHits(G4Step* aStep,
                                        G4TouchableHistory*)
{


//    G4cout << "Hit!" << G4endl;


    // Getting a track from hit
    G4Track* aTrack = aStep->GetTrack();
    // Getting the hit position
    G4ThreeVector globalPosition = aStep->GetPostStepPoint()->GetPosition();
    //	newHit->myData;


    G4StepPoint *aPostPoint = aStep->GetPostStepPoint();
    G4StepPoint *aPrevPoint = aStep->GetPreStepPoint();
    if (!aPostPoint->GetPhysicalVolume()) return false;

    G4LogicalVolume *PostVolume = aPostPoint->GetPhysicalVolume()->GetLogicalVolume();
    G4LogicalVolume *PrevVolume = aPrevPoint->GetPhysicalVolume()->GetLogicalVolume();


    // Names of previous and next volume
    G4String PreName = PrevVolume->GetName();
    G4String PostName = PostVolume->GetName();

    // For simplicity of using long lines
    const G4DynamicParticle *aParticle = aTrack->GetDynamicParticle();
//    auto motherTrack = aTrack->;


    // We are interested in charged particles only
    if (aParticle->GetCharge() == 0) return false;

    // Particle origin position
    G4ThreeVector initPos = aTrack->GetVertexPosition();

    // Is the particle primary or not
    G4bool isPrimary = aTrack->GetParentID() == 0;


    // New hit instance
    L_Hit* newHit = new L_Hit();

    newHit->myData.TrackID = aTrack->GetTrackID();
    newHit->myData.ParentID = aTrack->GetParentID();
    newHit->myData.Energy = aTrack->GetKineticEnergy();
    newHit->myData.PdgID = aTrack->GetParticleDefinition()->GetPDGEncoding();
    newHit->myData.Time = aTrack->GetGlobalTime();
    newHit->myData.X = globalPosition.x();
    newHit->myData.Y = globalPosition.y();
    newHit->myData.Z = globalPosition.z();
    newHit->myData.Momentum= aTrack->GetMomentum().mag();
    newHit->myData.Px = aTrack->GetMomentum().x();
    newHit->myData.Py = aTrack->GetMomentum().y();
    newHit->myData.Pz = aTrack->GetMomentum().z();

    newHit->myData.birthX = initPos.x();
    newHit->myData.birthY = initPos.y();
    newHit->myData.birthZ = initPos.z();

    newHit->myData.isPrimary = isPrimary;

    // Vectors of sector's and detector's names splitted into words
    std::vector<G4String> sectorWords;
    std::vector<G4String> detectorWords;

    // Splitting a string into words
    splitName(PreName, sectorWords);
    splitName(PostName, detectorWords);


    // Sector ID discrimination for the hit
    // If particle escapes sector into the world, save the ID of that sector
    if (detectorWords[0] == "World" && sectorWords[0] == "sector") {
        newHit->myData.StationID = atoi(sectorWords[2]);
    }
    else return false;


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



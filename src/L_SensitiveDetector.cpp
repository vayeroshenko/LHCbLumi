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


    _Collection =
            new L_HitsCollection(SensitiveDetectorName, collectionName[0]);

//    G4cout << "Collection \"" << _Collection->GetName() << "\" is created" << G4endl;



    // Getting a hit collection
    static G4int HCID = -1;



    if (HCID < 0) {
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }

//    G4cout << "Initializing SD. HCID: " << HCID << G4endl;

    HCE->AddHitsCollection(HCID, _Collection);

    //  G4cout << "___________________________ Detector initialized" << G4endl;
}


G4bool L_SensitiveDetector::ProcessHitsL(const G4Step* aStep, G4TouchableHistory* hist) {
    return ProcessHits((G4Step*)aStep, hist);
}

G4bool L_SensitiveDetector::ProcessHits(G4Step *aStep,
                                        G4TouchableHistory*)
{
    // Getting a track from step
    G4Track* aTrack = aStep->GetTrack();

    // Getting the hit position
    G4ThreeVector globalPosition = aStep->GetPostStepPoint()->GetPosition();

    G4StepPoint *aPostPoint = aStep->GetPostStepPoint();
    G4StepPoint *aPrevPoint = aStep->GetPreStepPoint();

    if (!aPostPoint->GetPhysicalVolume()) {
        return false;
    }

    G4LogicalVolume *PostVolume = aPostPoint->GetPhysicalVolume()->GetLogicalVolume();
    G4LogicalVolume *PrevVolume = aPrevPoint->GetPhysicalVolume()->GetLogicalVolume();

    // Names of previous and next volume
    G4String PreName = PrevVolume->GetName();
    G4String PostName = PostVolume->GetName();

    // For simplicity of using long lines
    //    const G4DynamicParticle *aParticle = aTrack->GetDynamicParticle();

    // Sector ID discrimination for the hit
    // If particle escapes sector into the world, save the ID of that sector
    if ( !(PostName == "World" && PreName == "detector") )
        return false;

    // We are interested in charged particles only
    //    if (aParticle->GetCharge() == 0) return false;

    // Particle origin position
    G4ThreeVector initPos = aTrack->GetVertexPosition();
    G4int trackID = aTrack->GetTrackID();
    G4int parentID = aTrack->GetParentID();



    // Hit object filling and inserting
    L_Hit* newHit = new L_Hit();

    newHit->myData.TrackID = trackID;
    newHit->myData.ParentID = parentID;
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

    newHit->myData.trackTheta = aTrack->GetMomentum().unit().dot(G4ThreeVector(0,0,-1.));

    G4double hitX = aTrack->GetMomentum().x();
    G4double hitY = aTrack->GetMomentum().y();
    G4double hitZ = aTrack->GetMomentum().z();
    G4double hitR = sqrt( hitX*hitX + hitY*hitY );
    newHit->myData.hitR = hitR;
    newHit->myData.hitTheta = - atan(hitR / hitZ);
    newHit->myData.hitPhi = acos(hitY / hitX);




    // Insert this hit
    _Collection->insert(newHit);

    //    G4cout << "Hit!" << G4endl;


    newHit->Draw();
    //  }

    return true;
}

void L_SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{

    G4int NbHits = _Collection->entries();

}



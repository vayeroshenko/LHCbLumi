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

std::ofstream photonHit("./output/photonHit.txt");
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

//    if (aParticle->GetCharge() == 0) return false;

    std::vector<G4String> prevWords;
    std::vector<G4String> postWords;

    splitName(PreName, prevWords);
    splitName(PostName, postWords);


    // New hit instance
    L_Hit* newHit = new L_Hit();
    G4String ParticleName = aParticle->GetDefinition()->GetParticleName();
    if (prevWords[0] == "World" && postWords[0] == "sector" && (ParticleName != "opticalphoton")){
        //G4cout << PreName << " " << PostName << " " << ParticleName << G4endl;
        G4ThreeVector particle_direction = aTrack->GetMomentumDirection();
        G4ThreeVector particle_position = aTrack->GetPosition();
        G4ThreeVector ort_z = G4ThreeVector(0.,0.,-1.);
                //particleHit << particle_position.getX() << " " << particle_position.getY() << " " << particle_position.getZ() << "\n";
                //angleFile  << particle_direction.angle(ort_z) / deg << "\n";
                //particleDir << particle_direction.getX() << " " << particle_direction.getY() << " " << particle_direction.getZ() << "\n";
                //newHit->myData.EntranceAngles = particle_direction.angle(ort_z) / deg;
    }


    // Handling only optical photons
    if (aParticle->GetDefinition()->GetParticleName() != "opticalphoton")
        return false;

      newHit->myData.TrackID = aTrack->GetTrackID();
      newHit->myData.ParentID = aTrack->GetParentID();
//      newHit->myData.Energy = aTrack->GetKineticEnergy();
      newHit->myData.PdgID = aTrack->GetParticleDefinition()->GetPDGEncoding();
//      newHit->myData.Time = aTrack->GetGlobalTime();
      newHit->myData.X = globalPosition.x();
      newHit->myData.Y = globalPosition.y();
      newHit->myData.Z = globalPosition.z();
//      newHit->myData.Momentum= aTrack->GetMomentum().mag();
//      newHit->myData.Px = aTrack->GetMomentum().x();
//      newHit->myData.Py = aTrack->GetMomentum().y();
//      newHit->myData.Pz = aTrack->GetMomentum().z();
////      G4cout << PreName << " " << PostName << G4endl;

//        else if (PreName == "L1PlaneOuter" && PostName == "World") newHit->myData.StationID = 1;
//        else if (PreName == "L2PlaneInner" && PostName == "World") newHit->myData.StationID = -2;
//        else if (PreName == "L2PlaneOuter" && PostName == "World") newHit->myData.StationID = 2;
//        else return false;


    // Vectors of sector's and detector's names splitted into words
    std::vector<G4String> sectorWords;
    std::vector<G4String> detectorWords;

    // Splitting a string into words
    splitName(PreName, sectorWords);
    splitName(PostName, detectorWords);


    //    if (nameWords[0] == "sector" && PostName == "World") {
    //        if (nameWords[1] == "in"){
    //            newHit->myData.StationID = - atoi(nameWords[2]);
    //        } else if (nameWords[1] == "out"){
    //            newHit->myData.StationID = atoi(nameWords[2]);
    //        }
    //    }

    /////////////// Code to get the exit angles//////////////////////
//    if (sectorWords[0] == "sector" && detectorWords[0] == "detector"){
//        G4ThreeVector photondirection = aTrack->GetMomentumDirection();
//        int sector_number = std::stoi(sectorWords[2]);
//        G4ThreeVector normal = get_normal(sector_number);
//        //G4double exit_angle = normal.angle(photondirection)/deg ;
//        //exitAngleFile << exit_angle << "\n";
//        //newHit->myData.exitAngles = exit_angle;

//    }

    /////////////// Code to get coordinates of hits in the detector////
    if (sectorWords[0] == "sector" && detectorWords[0] == "detector"){
        int sector_number = std::stoi(sectorWords[2]);
        /// global position of the photon
        G4ThreeVector photonPosition = aTrack->GetPosition();
        /// global direction of the photon
        G4ThreeVector photonDirection = aTrack->GetMomentumDirection();

        /// get current volume
        const G4VTouchable *touchable = aTrack->GetTouchable();

        /// get transformations that were applied to this volume
        const G4RotationMatrix *rotation = touchable->GetRotation();
        G4RotationMatrix rotation_inv = rotation->inverse();
        G4ThreeVector translation = touchable->GetTranslation();
        G4VSolid *sector = touchable->GetSolid();

        /// get local position of the photon hits and normal to the surface
        G4ThreeVector posLocal = *rotation * (photonPosition - translation);
        G4ThreeVector normal =  sector->SurfaceNormal(posLocal);
        G4ThreeVector dirLocal = *rotation * photonDirection;
        /// get exit angles of photons
        G4double exit_angle = normal.angle(dirLocal)/deg ;
        newHit->myData.exitAngles = exit_angle;
        G4int eID = 0;
        const G4Event* evt = G4RunManager::GetRunManager()->GetCurrentEvent();
        if(evt) eID = evt->GetEventID();
        photonHit << sector_number   << " "
                  << eID             << " "
                  << posLocal.getX() << " "
                  << posLocal.getY() << " "
                  << posLocal.getZ() << "\n";

    }
    // Sector ID discrimination for the hit
    if (sectorWords[0] == "sector" && detectorWords[0] == "detector") {
        newHit->myData.StationID = atoi(detectorWords[2]);
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



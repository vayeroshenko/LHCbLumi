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

    mcTruthMan = MCTruthManager::GetInstance();

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
    G4int trackID = aTrack->GetTrackID();
    G4int parentID = aTrack->GetParentID();

    // Getting the hit position
    G4ThreeVector globalPosition = aStep->GetPostStepPoint()->GetPosition();

    G4StepPoint *aPostPoint = aStep->GetPostStepPoint();
    G4StepPoint *aPrevPoint = aStep->GetPreStepPoint();

    if (!aPostPoint->GetPhysicalVolume()) {

        // Particle origin position
        G4ThreeVector initPos = aTrack->GetVertexPosition();

        // Is the particle primary or not
        G4bool isPrimary = parentID == 0;


        G4int grannyID = parentID;
        G4int iterID = parentID;

        auto mcEvent = mcTruthMan->GetCurrentEvent();
        auto mcParticle = mcEvent->barcode_to_particle(iterID);

        if (mcParticle){

            //        G4cout << mcParticle->production_vertex()->barcode() << G4endl;
            while (iterID != 0){
                mcParticle = mcEvent->barcode_to_particle(iterID);
                grannyID = iterID;
                auto it = mcParticle->production_vertex()->particles_in_const_begin();

                if (it == mcParticle->production_vertex()->particles_in_const_end()) iterID = 0;

                for(;
                    it!=mcParticle->production_vertex()->particles_in_const_end();
                    it++){
                    //                G4cout << (*it)->barcode() << G4endl;
                    iterID = (*it)->barcode();
                }
            }
        }


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

        newHit->myData.StationID = -1;

        newHit->myData.birthX = initPos.x();
        newHit->myData.birthY = initPos.y();
        newHit->myData.birthZ = initPos.z();

        newHit->myData.isPrimary = isPrimary;
        newHit->myData.grannyID = grannyID;

        // Insert this hit
        _Collection->insert(newHit);
        newHit->Draw();

        return true;
    }


    G4LogicalVolume *PostVolume = aPostPoint->GetPhysicalVolume()->GetLogicalVolume();
    G4LogicalVolume *PrevVolume = aPrevPoint->GetPhysicalVolume()->GetLogicalVolume();

    // Names of previous and next volume
    G4String PreName = PrevVolume->GetName();
    G4String PostName = PostVolume->GetName();

    // For simplicity of using long lines
    const G4DynamicParticle *aParticle = aTrack->GetDynamicParticle();
    //    auto motherTrack = aTrack->;

    G4int stationID = -1;

    // Vectors of sector's and detector's names splitted into words
    std::vector<G4String> sectorWords;
    std::vector<G4String> detectorWords;

    // Splitting a string into words
    splitName(PreName, sectorWords);
    splitName(PostName, detectorWords);

    // Sector ID discrimination for the hit
    // If particle escapes sector into the world, save the ID of that sector
    if (detectorWords[0] == "World" && sectorWords[0] == "sector") {
        stationID = atoi(sectorWords[2]);
    }
    else return false;

    // We are interested in charged particles only
    if (aParticle->GetCharge() == 0) return false;

    // Particle origin position
    G4ThreeVector initPos = aTrack->GetVertexPosition();

    // Is the particle primary or not
    G4bool isPrimary = parentID == 0;


    G4int grannyID = parentID;
    G4int iterID = parentID;

    auto mcEvent = mcTruthMan->GetCurrentEvent();
    auto mcParticle = mcEvent->barcode_to_particle(iterID);

    if (mcParticle){

        //        G4cout << mcParticle->production_vertex()->barcode() << G4endl;
        while (iterID != 0){
            mcParticle = mcEvent->barcode_to_particle(iterID);
            grannyID = iterID;
            auto it = mcParticle->production_vertex()->particles_in_const_begin();

            if (it == mcParticle->production_vertex()->particles_in_const_end()) iterID = 0;

            for(;
                it!=mcParticle->production_vertex()->particles_in_const_end();
                it++){
                //                G4cout << (*it)->barcode() << G4endl;
                iterID = (*it)->barcode();
            }
        }

        //        G4cout << G4endl;
    }

    //    G4cout << trackID << "\t" << grannyID << G4endl;







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
    newHit->myData.StationID = stationID;

    newHit->myData.birthX = initPos.x();
    newHit->myData.birthY = initPos.y();
    newHit->myData.birthZ = initPos.z();

    newHit->myData.isPrimary = isPrimary;
    newHit->myData.grannyID = grannyID;

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



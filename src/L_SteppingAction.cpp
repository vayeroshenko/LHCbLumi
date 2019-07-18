/*
                                 * L_SteppingAction.cpp
                                 *
                                 *  Created on: Oct 2, 2018
                                 *      Author: vsevolod
                                 */

#include "L_SteppingAction.h"
#include "L_SensitiveDetector.h"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4SDManager.hh"
#include "G4ProcessManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

L_SteppingAction::L_SteppingAction(L_PrimaryGeneratorAction *genAction) :
    _genAction(genAction){
    Reset();
    ResetPerEvent();
}

L_SteppingAction::~L_SteppingAction() {
}

void L_SteppingAction::UserSteppingAction(const G4Step* aStep) {
    //	G4cout << "_____________________________ step" << G4endl;
    G4Track* aTrack = aStep->GetTrack();
    // G4Track *aTrack = aStep->GetTrack();
    G4int trackID = aTrack->GetTrackID();
    //G4cout<<"trackID = "<<trackID<<G4endl;


    G4StepPoint* aPrePoint = aStep->GetPreStepPoint();
    G4VPhysicalVolume* aPrePV = aPrePoint->GetPhysicalVolume();

    G4StepPoint* aPostPoint = aStep->GetPostStepPoint();
    G4VPhysicalVolume* aPostPV = aPostPoint->GetPhysicalVolume();


    G4ParticleDefinition* particleType = aTrack->GetDefinition();
    if (particleType != G4OpticalPhoton::OpticalPhotonDefinition())
        return;
    //
    if (!aPostPV) return;
    if(!aPrePV->GetLogicalVolume()->GetSensitiveDetector()) return;


    if (aPrePoint->GetCharge() != 0. && aPrePoint->GetMomentum().mag() < 20. ) aTrack->SetTrackStatus(fStopAndKill);


    if (_particleID != trackID) {
        Reset();
        _particleID = trackID;
        InternalReflectionProbability(aTrack->GetTotalEnergy()/eV,
                                      _probOfReflection);
    }


    G4OpBoundaryProcessStatus boundaryStatus = Undefined;
    static G4OpBoundaryProcess* boundary = NULL;

    // Find boundary process
    if (!boundary) {
        G4ProcessManager* pm =
                aStep->GetTrack()->GetDefinition()->GetProcessManager();
        G4int nprocesses = pm->GetProcessListLength();
        G4ProcessVector* pv = pm->GetProcessList();
        for (G4int i = 0; i < nprocesses; i++) {
            if ((*pv)[i]->GetProcessName() == "OpBoundary") {
                boundary = (G4OpBoundaryProcess*)(*pv)[i];
                break;
            }
        }
    }

    if (!boundary) return;

    boundaryStatus = boundary->GetStatus();


    //    G4cout << "TOTAL INTERNAL REFLECTION"<< G4endl;

    if (aPostPoint->GetStepStatus() == fGeomBoundary) {
        G4String sdName = "LSD";
        G4SDManager* SDman = G4SDManager::GetSDMpointer();
        L_SensitiveDetector* sd =
                (L_SensitiveDetector*)SDman->FindSensitiveDetector(sdName);

        G4double flat = G4UniformRand();
        switch(boundaryStatus) {
        case Absorption:
            break;
        case FresnelRefraction:
            sd->_angle_refr.push_back(GetIncidenceAngleRefr(aPrePoint, aPostPoint, aStep) / deg);
            break;

        case FresnelReflection:
            // Reflections of surfaces of different media

            sd->_angle_fr.push_back(GetIncidenceAngleRefl(aPrePoint, aPostPoint) / deg);

            if (trackID == _currentPhotonID) {
                _numberOfReflections += 1;
                sd->_nOfReflections = _numberOfReflections;
            } else {
                _currentPhotonID = trackID;
                _numberOfReflections = 1;
                sd->_nOfReflections = _numberOfReflections;
            }

            break;
        case TotalInternalReflection:
            if (flat > _probOfReflection) {
                G4Track* aNonConstTrack = const_cast<G4Track*>(aTrack);
                aNonConstTrack->SetTrackStatus(fStopAndKill);
            } else {
                sd->_angle_tir.push_back(GetIncidenceAngleRefl(aPrePoint, aPostPoint) / deg);

                if (trackID == _currentPhotonID) {
                    _numberOfReflections += 1;
                    sd->_nOfReflections = _numberOfReflections;
                } else {
                    _currentPhotonID = trackID;
                    _numberOfReflections = 1;
                    sd->_nOfReflections = _numberOfReflections;
                }
            }
            break;
        case SpikeReflection:
            break;
        default:
            break;

        }

    }


    return;
}
void L_SteppingAction::ResetPerEvent(){
    _numberOfReflections = -1;
    _currentPhotonID = 0;
    _currentPhotonAngles = std::vector<G4double>();
}

void L_SteppingAction::Reset()
{
    _probOfReflection = 1.;
    _particleID = 0.;
}

void L_SteppingAction::InternalReflectionProbability(G4double energy,
                                                     G4double& probability)
{
    probability = 1.0;

    /* this function simulate the internal reflection probability per one
                                     bounce - each time photon bounced this function is called
                                     and the reflection is tested if photon reflects or disappear -
                                     this function estimates loss of photons during imperfections
                                     of bar */

    G4double opticalPhotonEnergy[36] = {
        1.90744901082931,1.93725290162352,1.96800294768103,1.99974493070815,
        2.03252763449025,2.06640309506508,2.10142687633737,2.13765837420526,
        2.17516115270009,2.21400331614116,2.25425792188918,2.29600343896121,
        2.33932425856425,2.38431126353664,2.43106246478245,2.4796837140781,
        2.53028950416133,2.58300386883136,2.63796139795543,2.6953083848675,
        2.75520412675345,2.81782240236148,2.88335315590477,2.95200442152155,
        3.02400452936354,3.09960464259763,3.17908168471551,3.26274172905013,
        3.35092393794338,3.44400515844181,3.54240530582586,3.64659369717368,
        3.75709653648197,3.87450580324703,3.99948986141629,4.13280619013017};

    G4double internalReflectivity[36] = {
        0.999895281,0.999891334,0.999885743,0.999878696,0.999870426,
        0.9998612,0.999851309,0.999841055,0.999830735,0.999820635,0.999811012,
        0.999802084,0.999794018,0.999786917,0.999780807,0.999775625,
        0.999771209,0.999767282,0.999763441,0.999759146,0.999753706,
        0.999746266,0.999735798,0.999721084,0.999700708,0.99967304,
        0.999636227,0.999588178,0.999526552,0.999448747,0.999351887,
        0.999232808,0.99908805,0.998913839,0.998706078,0.998460335};

    G4int i;
    for(i = 0; i < 36;i++) {
        if(energy < opticalPhotonEnergy[i]) break;
    }


    probability = ((energy-opticalPhotonEnergy[i-1])/
            (opticalPhotonEnergy[i]-opticalPhotonEnergy[i-1]))*
            (internalReflectivity[i]-internalReflectivity[i-1]) +
            internalReflectivity[i-1];

    /* because the ratio between peak1 and peak2 did not correspond,
                                     the reflection probability was change to get the same
                                     ration 2.1:1 => the original probability is multiplied by .9992 */
    probability = probability*.9992;


    // probability = 0;
}


G4double L_SteppingAction::GetIncidenceAngleRefl(G4StepPoint *preStep, G4StepPoint *postStep)
{
    G4ThreeVector preMomentum = preStep->GetMomentum();
    G4ThreeVector postMomentum = postStep->GetMomentum();

    preMomentum /= preMomentum.mag();
    postMomentum /= postMomentum.mag();

    return pi/2. - acos(preMomentum.dot(postMomentum)) / 2.;
}

G4double L_SteppingAction::GetIncidenceAngleRefr(G4StepPoint *preStep, G4StepPoint *postStep, const G4Step *aStep)
{
    G4Track *aTrack = aStep->GetTrack();



    // get refractive index for exact photon energy
    auto particle = aTrack->GetDynamicParticle();
    auto r_index_vector = preStep->GetMaterial()->GetMaterialPropertiesTable()->GetProperty("RINDEX");
    G4double r_index = r_index_vector->Value(particle->GetTotalEnergy());

    // angle between initial and final directions
    G4ThreeVector preMomentum = preStep->GetMomentum();
    G4ThreeVector postMomentum = postStep->GetMomentum();
    preMomentum /= preMomentum.mag();
    postMomentum /= postMomentum.mag();
    G4double cos_theta = preMomentum.dot(postMomentum);

    // angle of incidence
    G4double tan_alpha = sqrt(1 - cos_theta*cos_theta) / (r_index - cos_theta);

    return atan(tan_alpha);
}


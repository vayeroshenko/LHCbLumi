/*
 * L_SteppingAction.h
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#ifndef SRC_L_SteppingAction_H_
#define SRC_L_SteppingAction_H_

#include <G4UserSteppingAction.hh>
#include "L_PrimaryGeneratorAction.h"

class L_PrimaryGeneratorAction;

class L_SteppingAction: public G4UserSteppingAction {
public:
    L_SteppingAction(L_PrimaryGeneratorAction*);
    virtual ~L_SteppingAction();
    void UserSteppingAction(const G4Step*);

    void Reset();
    void ResetPerEvent();

    G4double GetIncidenceAngleRefl(G4StepPoint   *preStep,
                                          G4StepPoint   *postStep);

    G4double GetIncidenceAngleRefr(G4StepPoint   *preStep,
                                          G4StepPoint   *postStep,
                                          const G4Step  *aStep);

    G4double GetIncidenceAngle(const G4Step *aStep);

private:
    L_PrimaryGeneratorAction* _genAction;

    G4double _probOfReflection;
    G4double _particleID;

    G4int _currentPhotonID = 0;
    G4int _numberOfReflections = -1;

    std::vector<G4double> _currentPhotonAngles;

    void InternalReflectionProbability(G4double energy,
                                       G4double& probability);

};

#endif /* SRC_L_SteppingAction_H_ */

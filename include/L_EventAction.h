/*
 * L_EventAction.h
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#pragma once

#include <G4UserEventAction.hh>
#include "globals.hh"
#include "L_RunAction.h"
#include "L_Hit.h"
#include "L_SteppingAction.h"

class G4Event;

class L_RunAction;
class L_SteppingAction;
class L_PrimaryGeneratorAction;

class L_EventAction: public G4UserEventAction {
public:
    L_EventAction(L_RunAction*, L_SteppingAction*);
    virtual ~L_EventAction();
public:
    virtual void  BeginOfEventAction(const G4Event* );
    virtual void    EndOfEventAction(const G4Event* );

    void SetPrimGenerator(L_PrimaryGeneratorAction *gen){_primGenerator = gen;}
    inline void InsertPhoton(G4int secID){ _nPhot[secID - 1] ++; }

private:
    L_RunAction* runAction;
    L_SteppingAction* _steppingAction;
    G4int printModulo;
    G4int theCollectionID;

    G4int _nPhot[LConst::nSectors];

    L_PrimaryGeneratorAction* _primGenerator;
};
